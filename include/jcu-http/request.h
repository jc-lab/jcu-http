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

#include "client.h"

namespace jcu {
    namespace http {
        class Request {
        public:
            enum Method {
                METHOD_GET,
                METHOD_POST,
                METHOD_PUT
            };

        protected:
            Method method_;
            std::string url_;

        public:
            Request(Method method);
            Method getMethod() const;
            const std::string& getUrl() const;
        };
    }
}

#endif //__JCU_HTTP_REQUEST_H__
