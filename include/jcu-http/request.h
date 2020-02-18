/**
 * @file	request.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_REQUEST_H__
#define __JCU_HTTP_REQUEST_H__

#include <memory>
#include <map>
#include <list>

#include "client.h"

namespace jcu {
    namespace http {

        class HttpEntity;

        class Request {
        public:
            enum Method {
                METHOD_GET,
                METHOD_POST,
                METHOD_PUT
            };

            typedef std::map<std::string, std::list<std::string>> HeadersType;

        protected:
            Method method_;
            std::string url_;

            std::map<std::string, std::list<std::string>> headers_;
            std::shared_ptr<HttpEntity> entity_;

        public:
            Request(Method method, bool has_data);
            Method getMethod() const;
            virtual const std::string& getUrl() const;
            virtual HttpEntity* getEntity() const;
            virtual const HeadersType& headers() const {
                return headers_;
            }

            virtual void clearHeader();
            virtual void addHeader(const char *name, const char *value);
            virtual void setHeader(const char *name, const char *value);
            virtual void setHeader(const char *name, const std::list<std::string>& values);
        };
    }
}

#endif //__JCU_HTTP_REQUEST_H__
