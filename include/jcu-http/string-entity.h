/**
 * @file	string-entity.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2020/02/18
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_STRING_ENTITY_H__
#define __JCU_HTTP_STRING_ENTITY_H__

#include "http-entity.h"
#include <memory>
#include <string>

namespace jcu {
    namespace http {

        class StringEntity : public HttpEntity {
        private:
            std::string content_type_;
            std::string content_;

            int64_t read_offset_;

            StringEntity() : content_type_("text/plain"), read_offset_(0) {}

        public:
            static std::shared_ptr<StringEntity> create() {
                return std::shared_ptr<StringEntity>(new StringEntity());
            }

            const std::string& getContentType() const override {
                return content_type_;
            }

            int64_t getContentLength() const override {
                return content_.length();
            }

            size_t onReadData(void *buf_ptr, size_t buf_size) override {
                size_t remaining = content_.length() - read_offset_;
                if(remaining > 0) {
                    size_t avail = buf_size < remaining ? buf_size : remaining;
                    memcpy(buf_ptr, content_.data() + read_offset_, avail);
                    read_offset_ += avail;
                    return avail;
                }
                return 0;
            }

            void setContentType(const std::string& content_type) {
                content_type_ = content_type;
            }
            void setContent(const std::string& content) {
                content_ = content;
            }
        };

    }
}

#endif //__JCU_HTTP_STRING_ENTITY_H__
