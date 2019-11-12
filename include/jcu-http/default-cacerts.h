/**
 * @file	default-cacerts.h
 * @author	Jichan (development@jc-lab.net / http://ablog.jc-lab.net/ )
 * @date	2019/11/11
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_HTTP_DEFAULT_CACERTS_H__
#define __JCU_HTTP_DEFAULT_CACERTS_H__

#include <memory>
#include "ssl-cert.h"

namespace jcu {
    namespace http {

        extern std::shared_ptr<SslCert> defaultCacerts();

    }
}

#endif //__JCU_HTTP_DEFAULT_CACERTS_H__
