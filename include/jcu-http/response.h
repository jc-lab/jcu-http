/**
 * @file	response.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_RESPONSE_H__
#define __JCU_HTTP_RESPONSE_H__

#include <memory>
#include <future>
#include <string>
#include <vector>

#include "request-prepare.h"

namespace jcu {
    namespace http {
        class Client;
        class Request;

        class ResponseFuture;

        class Response {
        public:
            enum ErrorCode {
                E_OK = 0,
                E_FAILED = 1,
                E_OPERATION_TIMEOUT = 101,
                E_SYSTEM_OUTOFMEMORY = 201,
            };

            Response();

            int getStatusCode() const;
            ErrorCode getErrorCode() const;
            int getCurlRes() const;
            const std::vector<unsigned char> &getRawBody() const;
            std::shared_ptr<void> getUserContext() const;

        private:
            friend class ResponseFuture;

            std::shared_ptr<void> user_ctx_;

            int status_code_;
            ErrorCode error_code_;
            ErrorCode operation_error_code_;
            int curl_res_;
            std::vector<unsigned char> raw_body_;
        };

        class ResponseFuture {
        private:
            enum State {
                STATE_INIT = 0,
                STATE_READ_HEAD = 0x0001,
                STATE_READ_BODY = 0x0002,
                STATE_READ_BODY_FIRST = 0x0004 | STATE_READ_BODY,
            };

            std::shared_ptr<ResponseFuture> self_;

            RequestPrepare info_;

            std::thread thread_;
            State state_;
            void *curl_;
            std::unique_ptr<Response> response_;
            std::promise<std::unique_ptr<Response>> promise_;
            std::future<std::unique_ptr<Response>> future_;

            ResponseFuture(RequestPrepare &info);

        public:
            ~ResponseFuture();

            static std::shared_ptr<ResponseFuture> create(RequestPrepare &prepare);

            void wait() {
                future_.wait();
            }

            template<typename _Rep, typename _Period>
            std::future_status wait_for(const std::chrono::duration<_Rep, _Period>& __rel) {
                return future_.wait_for(__rel);
            }

            std::unique_ptr<Response> get() {
                return std::move(future_.get());
            }

        private:
            static void workThreadEntry(ResponseFuture *self);
            void workThreadProc();

            static size_t curlOnWrite(void *contents, size_t size, size_t nmemb, void *userp);
        };
    }
}

#endif //__JCU_HTTP_RESPONSE_H__
