/**
 * @file	response.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/response.h>
#include <jcu-http/request.h>
#include <jcu-http/client.h>

#include <curl/curl.h>

namespace jcu {
    namespace http {

        static std::string resolveUrl(const std::string &prefix, const std::string& req) {
            size_t protocol_pos = req.find("://");
            size_t first_slash_pos = req.find_first_of('/');
            if((protocol_pos != std::string::npos) && (first_slash_pos != std::string::npos)) {
                if((protocol_pos + 1) == first_slash_pos) {
                    return req;
                }
            }
            return prefix + req;
        }

        std::shared_ptr<ResponseFuture> ResponseFuture::create(RequestPrepare &prepare) {
            std::shared_ptr<ResponseFuture> instance(new ResponseFuture(prepare));
            instance->self_ = instance;
            instance->thread_ = std::thread(workThreadEntry, instance.get());
            return instance;
        }

        ResponseFuture::ResponseFuture(RequestPrepare &prepare)
        : info_(std::move(prepare)), state_(STATE_INIT), future_(promise_.get_future()) {
        }

        ResponseFuture::~ResponseFuture() {
            if(thread_.joinable()) {
                if(std::this_thread::get_id() == thread_.get_id()) {
                    thread_.detach();
                }else {
                    thread_.join();
                }
            }
        }

        void ResponseFuture::workThreadEntry(ResponseFuture *self) {
            std::shared_ptr<ResponseFuture> keeper = self->self_;
            ((void)keeper);
            self->self_.reset();
            self->workThreadProc();
            self->info_.clear();
        }

        void ResponseFuture::workThreadProc() {
            std::shared_ptr<Client> client(info_.getClient());
            Request *request = info_.getRequest();

            Client::Lock lock(*client.get());

            CURL *curl = (CURL*)lock.getCurl();
            long status_code = 0;

            void *write_cb_args[] = {this, 0};

            std::string url = resolveUrl(client->getApiEndpoint(), request->getUrl());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            switch(request->getMethod()) {
                case Request::METHOD_POST:
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);
                    break;
                case Request::METHOD_PUT:
                    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
                    break;
            }

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlOnWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, write_cb_args);

            response_.reset(new Response());
            response_->user_ctx_ = info_.wrapUserContext();

            state_ = STATE_READ_BODY_FIRST;
            curl_ = curl;

            CURLcode res = curl_easy_perform(curl);
            response_->curl_res_ = res;
            if(response_->operation_error_code_ != Response::E_OK) {
                response_->error_code_ = response_->operation_error_code_;
            }else {
                switch (res) {
                    case CURLE_OK:
                        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
                        response_->status_code_ = status_code;
                        response_->error_code_ = Response::E_OK;
                        break;
                    case CURLE_OPERATION_TIMEDOUT:
                        response_->error_code_ = Response::E_OPERATION_TIMEOUT;
                        break;
                    case CURLE_OUT_OF_MEMORY:
                        response_->error_code_ = Response::E_SYSTEM_OUTOFMEMORY;
                        break;
                    default:
                        response_->error_code_ = Response::E_FAILED;
                }
            }

            auto done_callback = info_.getOnDoneCallback();
            if(done_callback) {
                (*done_callback)(response_.get());
            }

            auto unique_done_callback = info_.getOnUniqueDoneCallback();
            if(unique_done_callback) {
                (*unique_done_callback)(std::move(response_));
            }

            promise_.set_value(std::move(response_));
        }

        size_t ResponseFuture::curlOnWrite(void *contents, size_t size, size_t nmemb, void *userp) {
            size_t total_bytes = size * nmemb;
            void **write_cb_args = (void**)userp;
            ResponseFuture *self = (ResponseFuture*)write_cb_args[0];
            Response *response = self->response_.get();

            RequestPrepare::OnDataFunction *on_data_callback = self->info_.getOnDataCallback();

            if(self->state_ == STATE_READ_BODY_FIRST) {
                long status_code = 0;
                curl_easy_getinfo((CURL*)self->curl_, CURLINFO_RESPONSE_CODE, &status_code);
                response->status_code_ = status_code;
                response->error_code_ = Response::E_OK;
                self->state_ = STATE_READ_BODY;
            }

            if(on_data_callback) {
                if((*on_data_callback)(contents, total_bytes, response)) {
                    return total_bytes;
                }else{
                    return 0;
                }
            }

            try {
                response->raw_body_.insert(response->raw_body_.end(), ((const unsigned char*)contents), ((const unsigned char*)contents) + total_bytes);
            }catch(std::exception& e){
                response->operation_error_code_ = Response::E_SYSTEM_OUTOFMEMORY;
                return 0;
            }

            return total_bytes;
        }

        Response::Response()
        : error_code_(E_OK), operation_error_code_(E_OK) {

        }
        int Response::getStatusCode() const {
            return status_code_;
        }
        Response::ErrorCode Response::getErrorCode() const {
            return error_code_;
        }
        int Response::getCurlRes() const {
            return curl_res_;
        }
        const std::vector<unsigned char> &Response::getRawBody() const {
            return raw_body_;
        }
        std::shared_ptr<void> Response::getUserContext() const {
            return user_ctx_;
        }

    }
}