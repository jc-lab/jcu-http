/**
 * @file	client.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_CLIENT_H__
#define __JCU_HTTP_CLIENT_H__

#include <memory>
#include <mutex>
#include <string>
#include <list>

#include "request-prepare.h"

namespace jcu {
    namespace http {

        class Client;
        class Request;
        class ResponseFuture;
        class SslCert;

        class Client {
        private:
            struct Impl;

            void *curl_;
            std::mutex session_mutex_;
            std::weak_ptr<Client> self_;

            std::string api_endpoint_;

            std::list<std::shared_ptr<SslCert>> trust_ca_certs_;

            Client();

        public:
            class Lock {
            private:
                std::unique_lock<std::mutex> lock_;
                void *curl_;

            public:
                Lock(Client &session);
                inline void *getCurl() {
                    return curl_;
                }
            };

            virtual ~Client();

            static std::shared_ptr<Client> create();

            void setApiEndpoint(const std::string &url);
            const std::string& getApiEndpoint() const;

            void addTrustCACert(std::shared_ptr<SslCert> cert);

            RequestPrepare prepare(std::shared_ptr<Request> request);
        };
    }
}

#endif //__JCU_HTTP_CLIENT_H__
