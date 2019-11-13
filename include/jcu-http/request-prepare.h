/**
 * @file	request-prepare.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/12
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_REQUEST_PREPARE_H__
#define __JCU_HTTP_REQUEST_PREPARE_H__

#include <memory>
#include <functional>

namespace jcu {
    namespace http {

        class Client;
        class Request;
        class Response;
        class ResponseFuture;

        class RequestPrepare {
        public:
            typedef std::function<bool(const void *data, size_t length, Response *response)> OnDataFunction;
            typedef std::function<void(Response *response)> OnDoneFunction;
            typedef std::function<void(std::unique_ptr<Response> response)> OnUniqueDoneFunction;

        private:
            friend class Client;

            std::shared_ptr<Client> client_;
            std::shared_ptr<Request> request_;
            std::unique_ptr<OnDataFunction> cb_on_data_;
            std::unique_ptr<OnDoneFunction> cb_on_done_;
            std::unique_ptr<OnUniqueDoneFunction> cb_on_unique_done_;
            std::shared_ptr<void> user_ctx_;

            RequestPrepare(std::shared_ptr<Client> client, std::shared_ptr<Request> request);
            RequestPrepare(const RequestPrepare& o) {}

        public:
            RequestPrepare(RequestPrepare&& o) = default;

            RequestPrepare& withUserContext(std::shared_ptr<void> user_ctx);
            RequestPrepare& onData(const OnDataFunction &callback);
            RequestPrepare& onDone(const OnDoneFunction &callback);
            RequestPrepare& onUniqueDone(const OnUniqueDoneFunction &callback);
            std::shared_ptr<ResponseFuture> execute();
            void clear();

            std::shared_ptr<Client> getClient() const {
                return client_;
            }

            std::shared_ptr<void> wrapUserContext() {
                std::shared_ptr<void> temp(user_ctx_);
                user_ctx_.reset();
                return temp;
            }

            Request *getRequest() const {
                return request_.get();
            }

            OnDataFunction *getOnDataCallback() const {
                return cb_on_data_.get();
            }

            OnDoneFunction *getOnDoneCallback() const {
                return cb_on_done_.get();
            }

            OnUniqueDoneFunction *getOnUniqueDoneCallback() const {
                return cb_on_unique_done_.get();
            }
        };
    }
}

#endif //__JCU_HTTP_REQUEST_PREPARE_H__
