/**
 * @file	http-get.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_GET_H__
#define __JCU_HTTP_GET_H__

#include <memory>

#include "request.h"

namespace jcu {
    namespace http {
        class HttpGet : public Request {
        private:
            HttpGet();

        public:
            static std::unique_ptr<HttpGet> create(const char *url = nullptr);
            static std::unique_ptr<HttpGet> create(const std::string &url);
            void setUrl(const std::string& url);
        };
    }
}

#endif //__JCU_HTTP_GET_H__
