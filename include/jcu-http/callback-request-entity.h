/**
 * @file	callback-request-entity.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2020/02/18
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_CALLBACK_REQUEST_ENTITY_H__
#define __JCU_HTTP_CALLBACK_REQUEST_ENTITY_H__

#include "http-entity.h"
#include <memory>
#include <functional>
#include <string>

namespace jcu {
    namespace http {

        class CallbackRequestEntity : public HttpEntity {
        public:
            typedef std::function<size_t(void *buf_ptr, size_t buf_size)> DataWriteCallback_t;

        private:
            int64_t content_length_;
            std::string content_type_;

            DataWriteCallback_t write_callback_;

            CallbackRequestEntity() : content_length_(-1) {}

        public:
            static std::shared_ptr<CallbackRequestEntity> create() {
                return std::shared_ptr<CallbackRequestEntity>(new CallbackRequestEntity());
            }

            const std::string& getContentType() const override {
                return content_type_;
            }

            int64_t getContentLength() const override {
                return content_length_;
            }

            size_t onReadData(void *buf_ptr, size_t buf_size) override {
                return write_callback_(buf_ptr, buf_size);
            }

            void setContentLength(int64_t content_length) {
                content_length_ = content_length;
            }
            void setContentType(const std::string& content_type) {
                content_type_ = content_type;
            }
            void setDataWriteCallback(const DataWriteCallback_t& callback) {
                write_callback_ = callback;
            }
            void setDataWriteCallback(DataWriteCallback_t&& callback) {
                write_callback_ = std::move(callback);
            }
        };

    }
}

#endif //__JCU_HTTP_CALLBACK_REQUEST_ENTITY_H__
