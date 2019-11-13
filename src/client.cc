/**
 * @file	client.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/client.h>
#include <jcu-http/request.h>
#include <jcu-http/response.h>
#include <jcu-http/ssl-cert.h>
#include <curl/curl.h>

#include <openssl/ssl.h>

namespace jcu {
    namespace http {

        struct Client::Impl {
            static CURLcode curlOnSslCtx(CURL *curl, void *psslctx, void *param);
        };

        Client::Client() : session_mutex_() {
            curl_ = curl_easy_init();
        }

        Client::~Client() {
            curl_easy_cleanup((CURL*)curl_);
        }

        Client::Lock::Lock(jcu::http::Client &session) : lock_(session.session_mutex_) {
            CURL *curl = (CURL*)session.curl_;
            curl_easy_reset(curl);
            curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, Impl::curlOnSslCtx);
            curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, &session);
            curl_ = curl;
        }

        std::shared_ptr<Client> Client::create() {
            std::shared_ptr<Client> instance(new Client());
            instance->self_ = instance;
            return instance;
        }

        void Client::setApiEndpoint(const std::string &url) {
            api_endpoint_ = url;
        }

        const std::string& Client::getApiEndpoint() const {
            return api_endpoint_;
        }

        void Client::addTrustCACert(std::shared_ptr<SslCert> cert) {
            trust_ca_certs_.push_back(cert);
        }

        RequestPrepare Client::prepare(std::shared_ptr<Request> request) {
            return std::move(RequestPrepare(self_.lock(), request));
        }

        CURLcode Client::Impl::curlOnSslCtx(CURL *curl, void *psslctx, void *param) {
            Client *client = (Client*)param;
            SSL_CTX *ssl_ctx = (SSL_CTX*)psslctx;
            X509_STORE *store;
            int rc;

            /* get a pointer to the X509 certificate store (which may be empty) */
            store = SSL_CTX_get_cert_store(ssl_ctx);

            for(auto it = client->trust_ca_certs_.cbegin(); it != client->trust_ca_certs_.cend(); it++) {
                const SslCert *ssl_cert = it->get();
                for(int i=0; i<ssl_cert->getCount(); i++) {
                    const SslCert::Item *item = ssl_cert->item(i);
                    if(item->getX509Cert())
                        X509_STORE_add_cert(store, (X509*)item->getX509Cert());
                    if(item->getX509Crl())
                        X509_STORE_add_crl(store, (X509_CRL*)item->getX509Crl());
                }
            }

            return CURLE_OK;
        }

    }
}
