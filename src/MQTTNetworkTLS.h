/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MQTTNETWORKTLS_H_
#define _MQTTNETWORKTLS_H_

#include "NetworkInterface.h"
#include "TLSSocket.h"
#include "MQTTNetworkUtil.h"

#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

class MQTTNetworkTLS {
public:
    MQTTNetworkTLS(NetworkInterface *aNetwork) : network(aNetwork)
    {
        socket = new TLSSocket();
    }

    ~MQTTNetworkTLS()
    {
        delete socket;
    }

    int read(unsigned char *buffer, int len, int timeout)
    {
        return accumulate_mqtt_read(socket, buffer, len, timeout);
    }

    int write(unsigned char *buffer, int len, int timeout)
    {
        return mqtt_write(socket, buffer, len, timeout);
    }

    int connect(const char *hostname, int port, const char *ssl_ca_pem = NULL,
                const char *ssl_cli_pem = NULL, const char *ssl_pk_pem = NULL)
    {
        int ret = socket->open(network);
        if (ret < 0) {
            return ret;
        }
        SocketAddress addr;
        ret = network->gethostbyname(hostname, &addr);
        if (ret < 0) {
            return ret;
        }
        addr.set_port(port);
        ret = socket->set_root_ca_cert(ssl_ca_pem);
        if (ret < 0) {
            return ret;
        }
        if (ssl_cli_pem != NULL && ssl_pk_pem != NULL) {
            ret = socket->set_client_cert_key(ssl_cli_pem, ssl_pk_pem);
            if (ret < 0) {
                return ret;
            }
        }
        return socket->connect(addr);
    }

    int disconnect()
    {
        return socket->close();
    }

    int set_root_ca_cert(const char *root_ca_pem)
    {
        return socket->set_root_ca_cert(root_ca_pem);
    }

private:
    NetworkInterface *network;
    TLSSocket *socket;
};

#endif // defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)
#endif // _MQTTNETWORKTLS_H_
