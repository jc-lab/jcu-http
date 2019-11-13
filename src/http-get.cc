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

        std::shared_ptr<HttpGet> HttpGet::create(const char *url) {
            std::shared_ptr<HttpGet> instance(new HttpGet());
            if(url) {
                instance->url_ = url;
            }
            return std::move(instance);
        }

        std::shared_ptr<HttpGet> HttpGet::create(const std::string &url) {
            std::shared_ptr<HttpGet> instance(new HttpGet());
            instance->url_ = url;
            return std::move(instance);
        }

        HttpGet::HttpGet()
        : Request(METHOD_GET) {
        }

    }
}
