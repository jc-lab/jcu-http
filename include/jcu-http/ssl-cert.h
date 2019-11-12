/**
 * @file	ssl-cert.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_SSL_CERT_H__
#define __JCU_HTTP_SSL_CERT_H__

#include <string>
#include <memory>

namespace jcu {
    namespace http {

        class SslCert {
        private:
            struct Impl;
            struct ItemImpl;
            Impl *impl_;

            SslCert();

        public:
            class Item {
            public:
                virtual void *getX509Cert() const = 0;
                virtual void *getX509Crl() const = 0;
            };

            ~SslCert();
            int getCount() const;
            const Item *item(int index) const;

            static std::shared_ptr<SslCert> createFromPEM(const std::string& pem);
        };

    }
}

#endif //__JCU_HTTP_CLIENT_H__
