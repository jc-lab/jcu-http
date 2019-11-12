/**
 * @file	http-get.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/http-get.h>

namespace jcu {
    namespace http {

        std::unique_ptr<HttpGet> HttpGet::create(const char *url) {
            std::unique_ptr<HttpGet> instance(new HttpGet());
            if(url) {
                instance->setUrl(url);
            }
            return std::move(instance);
        }

        std::unique_ptr<HttpGet> HttpGet::create(const std::string &url) {
            std::unique_ptr<HttpGet> instance(new HttpGet());
            instance->setUrl(url);
            return std::move(instance);
        }

        HttpGet::HttpGet()
        : Request(METHOD_GET) {
        }

        void HttpGet::setUrl(const std::string& url) {
            url_ = url;
        }

    }
}
