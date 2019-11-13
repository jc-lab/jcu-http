/**
 * @file	request-prepare.cc
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/12
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <jcu-http/request-prepare.h>
#include <jcu-http/response.h>

namespace jcu {
    namespace http {

        RequestPrepare::RequestPrepare(std::shared_ptr<Client> client, std::shared_ptr<Request> request)
            : client_(client), request_(request) {
        }

        RequestPrepare& RequestPrepare::withUserContext(std::shared_ptr<void> user_ctx) {
            user_ctx_ = user_ctx;
            return *this;
        }

        RequestPrepare& RequestPrepare::onData(const OnDataFunction &callback) {
            cb_on_data_.reset(new OnDataFunction(callback));
            return *this;
        }

        RequestPrepare& RequestPrepare::onDone(const OnDoneFunction &callback) {
            cb_on_done_.reset(new OnDoneFunction(callback));
            return *this;
        }
        RequestPrepare&  RequestPrepare::onUniqueDone(const OnUniqueDoneFunction &callback) {
            cb_on_unique_done_.reset(new OnUniqueDoneFunction(callback));
            return *this;
        }

        std::shared_ptr<ResponseFuture> RequestPrepare::execute() {
            return ResponseFuture::create(*this);
        }

        void RequestPrepare::clear() {
            cb_on_data_.reset(nullptr);
            cb_on_done_.reset(nullptr);
            cb_on_unique_done_.reset(nullptr);
            user_ctx_.reset();
        }

    }
}
