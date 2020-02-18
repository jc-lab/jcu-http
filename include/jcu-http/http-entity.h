/**
 * @file	http-entity.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2020/02/18
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_HTTP_ENTITY_H__
#define __JCU_HTTP_HTTP_ENTITY_H__

#include <memory>

namespace jcu {
    namespace http {

        class HttpEntity {
        public:
            virtual const std::string& getContentType() const = 0;
            virtual int64_t getContentLength() const = 0;
            virtual size_t onReadData(void *buf_ptr, size_t buf_size) = 0;
        };

    }
}

#endif //__JCU_HTTP_HTTP_ENTITY_H__
