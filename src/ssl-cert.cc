/**
 * @file	ssl-cert.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/ssl-cert.h>

#include <vector>

#include <openssl/bio.h>
#include <openssl/ssl.h>

namespace jcu {
    namespace http {
        struct SslCert::ItemImpl : public Item {
            X509 *cert_;
            X509_CRL *crl_;

            ItemImpl(X509* cert, X509_CRL *crl) : cert_(cert), crl_(crl) {}

            ~ItemImpl() {
                if(cert_) {
                    X509_free(cert_);
                    cert_ = NULL;
                }
                if(crl_) {
                    X509_CRL_free(crl_);
                    crl_ = NULL;
                }
            }

            void *getX509Cert() const override {
                return cert_;
            }
            void *getX509Crl() const override {
                return crl_;
            }
        };

        struct SslCert::Impl {
            std::vector<ItemImpl> list_;
        };

        SslCert::SslCert()
        : impl_(nullptr) {
        }

        SslCert::~SslCert() {
            delete impl_;
        }

        int SslCert::getCount() const {
            return impl_->list_.size();
        }

        const SslCert::Item *SslCert::item(int index) const {
            return &impl_->list_[index];
        }

        std::shared_ptr<SslCert> SslCert::createFromPEM(const std::string &pem) {
            int i;
            STACK_OF(X509_INFO) *x509_info;
            BIO *bio = BIO_new_mem_buf((char *)pem.data(), pem.length());
            if(bio) {
                x509_info = PEM_X509_INFO_read_bio(bio, NULL, NULL, NULL);
                BIO_free(bio);
            }

            if(!x509_info) {
                return nullptr;
            }

            std::shared_ptr<SslCert> instance(new SslCert());
            instance->impl_ = new Impl();
            instance->impl_->list_.reserve(sk_X509_INFO_num(x509_info));
            for(i = 0; i < sk_X509_INFO_num(x509_info); i++) {
                X509_INFO *itmp = sk_X509_INFO_value(x509_info, i);
                X509* cert = itmp->x509 ? X509_dup(itmp->x509) : NULL;
                X509_CRL* crl = itmp->crl ? X509_CRL_dup(itmp->crl) : NULL;
                instance->impl_->list_.emplace_back(cert, crl);
            }

            sk_X509_INFO_pop_free(x509_info, X509_INFO_free);

            return instance;
        }
    }
}
