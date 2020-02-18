/**
 * @file	http-post.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/http-post.h>

namespace jcu {
    namespace http {

        std::shared_ptr<HttpPost> HttpPost::create(const char *url) {
            std::shared_ptr<HttpPost> instance(new HttpPost());
            if(url) {
                instance->url_ = url;
            }
            return std::move(instance);
        }

        std::shared_ptr<HttpPost> HttpPost::create(const std::string &url) {
            std::shared_ptr<HttpPost> instance(new HttpPost());
            instance->url_ = url;
            return std::move(instance);
        }

        HttpPost::HttpPost()
            : Request(METHOD_POST, true) {
        }

        void HttpPost::setEntity(std::shared_ptr<HttpEntity> entity) {
            entity_ = entity;
        }

    }
}
