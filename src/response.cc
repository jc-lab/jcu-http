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

        std::shared_ptr<ResponseFuture> ResponseFuture::create(std::shared_ptr<Client> client, std::unique_ptr<Request> request, CustomHandler *custom_handler) {
            std::shared_ptr<ResponseFuture> instance(new ResponseFuture(client, std::move(request), custom_handler));
            instance->self_ = instance;
            instance->thread_ = std::thread(workThreadEntry, instance.get());
            return instance;
        }

        ResponseFuture::ResponseFuture(std::shared_ptr<Client> session, std::unique_ptr<Request> request, CustomHandler *custom_handler)
        : session_(session),
        request_(std::move(request)),
        future_(promise_.get_future()),
          custom_handler_(custom_handler) {
        }

        ResponseFuture::~ResponseFuture() {
            if(thread_.joinable())
                thread_.join();
        }

        void ResponseFuture::workThreadEntry(ResponseFuture *self) {
            std::shared_ptr<ResponseFuture> keeper = self->self_;
            ((void)keeper);
            self->self_ = nullptr;
            self->workThreadProc();
        }

        void ResponseFuture::workThreadProc() {
            Client::Lock lock(*session_.get());
            CURL *curl = (CURL*)lock.getCurl();
            long status_code = 0;

            void *write_cb_args[] = {this, 0};

            std::string url = resolveUrl(session_->getApiEndpoint(), request_->getUrl());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            switch(request_->getMethod()) {
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

            promise_.set_value(std::move(response_));
        }

        size_t ResponseFuture::curlOnWrite(void *contents, size_t size, size_t nmemb, void *userp) {
            size_t total_bytes = size * nmemb;
            void **write_cb_args = (void**)userp;
            ResponseFuture *self = (ResponseFuture*)write_cb_args[0];
            Response *response = self->response_.get();

            if(self->custom_handler_) {
                size_t handled_bytes = 0;
                if(self->custom_handler_->onData(contents, total_bytes, &handled_bytes)) {
                    return handled_bytes;
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

    }
}