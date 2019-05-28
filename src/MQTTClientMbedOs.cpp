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

#include "MQTTClientMbedOs.h"

int MQTTNetworkMbedOs::read(unsigned char* buffer, int len, int timeout) {
    nsapi_size_or_error_t rc = 0;
    socket->set_timeout(timeout);
    rc = socket->recv(buffer, len);
    if (rc == NSAPI_ERROR_WOULD_BLOCK){
        // time out and no data
        // MQTTClient.readPacket() requires 0 on time out and no data.
        return 0;
    }
    return rc;
}

int MQTTNetworkMbedOs::write(unsigned char* buffer, int len, int timeout) {
    nsapi_size_or_error_t rc = 0;
    socket->set_timeout(timeout);
    rc = socket->send(buffer, len);
    if (rc == NSAPI_ERROR_WOULD_BLOCK){
        // time out and no data
        // MQTTClient.writePacket() requires 0 on time out and no data.
        return 0;
    }
    return rc;
}

int MQTTNetworkMbedOs::connect(const char* hostname, int port) {
    SocketAddress sockAddr(hostname, port);
    return socket->connect(sockAddr);
}

int MQTTNetworkMbedOs::disconnect() {
    return socket->close();
}

MQTTClient::MQTTClient(TCPSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkMbedOs(socket);
    client = new MQTT::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS>(*mqttNet);
};

MQTTClient::MQTTClient(TLSSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkMbedOs(socket);
    client = new MQTT::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS>(*mqttNet);
};

MQTTClient::MQTTClient(UDPSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkMbedOs(socket);
    clientSN = new MQTTSN::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS>(*mqttNet);
};

 MQTTClient::MQTTClient(DTLSSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkMbedOs(socket);
    clientSN = new MQTTSN::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS>(*mqttNet);
};

nsapi_error_t MQTTClient::connect(MQTTPacket_connectData& options) {
    if (client == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = client->connect(options);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::connect(MQTTSNPacket_connectData& options) {
    if (clientSN == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = clientSN->connect(options);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::publish(const char* topicName, MQTT::Message& message) {
    if (client == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = client->publish(topicName, message);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::publish(MQTTSN_topicid& topicName, MQTTSN::Message& message) {
    if (clientSN == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = clientSN->publish(topicName, message);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh) {
    if (client == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = client->subscribe(topicFilter, qos, mh);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::subscribe(MQTTSN_topicid& topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh) {
    if (clientSN == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = clientSN->subscribe(topicFilter, qos, mh);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::unsubscribe(const char* topicFilter) {
    if (client == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = client->unsubscribe(topicFilter);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::unsubscribe(MQTTSN_topicid& topicFilter) {
    if (clientSN == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    nsapi_error_t ret = clientSN->unsubscribe(topicFilter);
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::yield(unsigned long timeout_ms) {
    nsapi_error_t ret = NSAPI_ERROR_OK;
    if (client != NULL) {
        ret = client->yield(timeout_ms);
    } else if (clientSN != NULL) {
        ret = clientSN->yield(timeout_ms);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

nsapi_error_t MQTTClient::disconnect() {
    nsapi_error_t ret = NSAPI_ERROR_OK;
    if (client != NULL) {
        ret = client->disconnect();
    } else if (clientSN != NULL) {
        ret = clientSN->disconnect(0);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    return ret < 0 ? NSAPI_ERROR_NO_CONNECTION : ret;
}

bool MQTTClient::isConnected() {
    if (client != NULL) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else if (clientSN == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    } else {
        return clientSN->isConnected();
    }
}

void MQTTClient::setDefaultMessageHandler(messageHandler mh) {
    nsapi_error_t ret = NSAPI_ERROR_OK;
    if (client != NULL) {
        client->setDefaultMessageHandler(mh);
    } else if (clientSN != NULL) {
        client->setDefaultMessageHandler(mh);
    }
}

nsapi_error_t MQTTClient::setMessageHandler(const char* topicFilter, messageHandler mh) {
    if (clientSN != NULL) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else if (client == NULL) {
        return NSAPI_ERROR_NO_CONNECTION;
    } else {
        return client->setMessageHandler(topicFilter, mh);
    }
}

void MQTTClient::init(Socket* sock) {
    socket = sock;
    client = NULL;
    clientSN = NULL;
}
