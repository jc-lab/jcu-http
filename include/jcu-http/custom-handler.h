/**
 * @file	custom-handler.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/12
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_CUSTOM_HANDLER_H__
#define __JCU_HTTP_CUSTOM_HANDLER_H__

#include <memory>
#include <future>
#include <string>
#include <vector>

namespace jcu {
    namespace http {
        class Client;
        class Request;

        class CustomHandler {
        public:
            /**
             * on data event
             *
             * @param data
             * @param length
             * @param readed_bytes
             * @return Handled
             */
            virtual bool onData(const void *data, size_t length, size_t *readed_bytes) {
                return false;
            }
        };
    }
}

#endif //__JCU_HTTP_CUSTOM_HANDLER_H__
