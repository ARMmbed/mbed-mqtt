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

#ifndef _MQTTNETWORKUDP_H_
#define _MQTTNETWORKUDP_H_

#include "UDPSocket.h"

class MQTTSNNetworkUDP {
public:
    MQTTSNNetworkUDP(NetworkInterface *net) :
        network(net) {
        socket = new UDPSocket();
    }

    ~MQTTSNNetworkUDP() {
        delete socket;
    }

    int read(unsigned char* buffer, int len, int timeout) {
        return socket->recv(buffer, len);
    }

    int write(unsigned char* buffer, int len, int timeout) {
        return socket->send(buffer, len);
    }

    int connect(const char* hostname, int port) {
        socket->open(network);
        SocketAddress addr(hostname, port);
        return socket->connect(addr);
    }

    int disconnect(void) {
        return 0;
    }

private:
    NetworkInterface* network;
    UDPSocket* socket;
};

#endif // _MQTTNETWORKUDP_H_
