/**
 * @file	request.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/request.h>

namespace jcu {
    namespace http {

        Request::Request(Method method)
        : method_(method)
        {

        }

        Request::Method Request::getMethod() const {
            return method_;
        }

        const std::string& Request::getUrl() const {
            return url_;
        }

    }
}
