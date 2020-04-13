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

#ifndef MQTT_CLIENT_MBED_OS_H
#define MQTT_CLIENT_MBED_OS_H

#include <TCPSocket.h>
#include <TLSSocket.h>
#include <DTLSSocket.h>
#include <UDPSocket.h>
#include "unity/unity.h"

#include "FP.h"
#include <MQTTPacket.h>
#include <MQTTClient.h>
#include <MQTTSNPacket.h>
#include <MQTTSNClient.h>
#include <MQTTmbed.h> // Countdown

/**
 * @brief Implementation of the Network class template parameter of MQTTClient.
 */
class MQTTNetworkMbedOs {
public:
    /**
     * @brief Construct the network implementation.
     *
     * If TCPSocket or TLSSocket are provided, the MQTT protocol will be used.
     * If UDPSocket or DTLSSocket are provided, the MQTT-SN protocol will be used.
     *
     * @param _socket socket to be used for MQTT communication.
     */
    MQTTNetworkMbedOs(Socket *_socket) : socket(_socket) {}

    /**
     * @brief Read data from the socket.
     *
     * @param buffer buffer to store the data
     * @param len expected amount of bytes
     * @param timeout timeout for the operation
     */
    int read(unsigned char *buffer, int len, int timeout);

    /**
     * @brief Write data to the socket.
     *
     * @param buffer buffer that contains data to be written
     * @param len amount of bytes to write
     * @param timeout timeout for the operation
     */
    int write(unsigned char *buffer, int len, int timeout);

    /**
     * @brief Connect the socket to the hostname at a given port.
     *
     * The socket must have the network interface set up and connected before.
     * This connect() is different from MQTTClient::connect, which performs the
     * connection to a broker, over the connected socket.
     *
     * @param hostname Hostname to connect to. This can be a string containing
     * IP address like "192.168.52.10" or domain address, like "mqtt.flespi.io"
     * @param port port number to be used for connection
     */
    int connect(const char *hostname, int port);

    /**
     * @brief Disconnect from the hostname.
     */
    int disconnect();

private:
    Socket *socket;
};

/**
 * @brief MQTT client mbed-os wrapper class
 *
 * This class wraps around the paho library templated MQTT(-SN)Client.
 * It depends on the type of socket provided whether MQTT or MQTT-SN will be used.
 * MQTTNetworkMbedOs will be used as a Network for the paho MQTTClient.
 */
class MQTTClient {
public:
    /** MQTT message handler */
    typedef void (*messageHandler)(MQTT::MessageData &);
    /** MQTT-SN message handler */
    typedef void (*messageHandlerSN)(MQTTSN::MessageData &);

    /**
     * @brief Constructor for the TCPSocket-based communication.
     * MQTT protocol will be used.
     *
     * @param _socket socket to be used for communication
     */
    MQTTClient(TCPSocket *_socket);
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)
    /**
     * @brief Constructor for the TLSSocket-based communication.
     * MQTT protocol will be used over a secure socket.
     *
     * @param _socket socket to be used for communication
     */
    MQTTClient(TLSSocket *_socket);
#endif
    /**
     * @brief Constructor for the UDPSocket-based communication.
     * MQTT-SN protocol will be used.
     *
     * @param _socket socket to be used for communication
     */
    MQTTClient(UDPSocket *_socket);
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)
    /**
     * @brief Constructor for the DTLSSocket-based communication.
     * MQTT-SN protocol will be used over a secure socket.
     *
     * @param _socket socket to be used for communication
     */
    MQTTClient(DTLSSocket *_socket);
#endif

    /**
     * @brief Destructor for the TCPSocket-based communication.
     */
    ~MQTTClient();

    /**
     * @brief Connect to the MQTT broker
     *
     * This is different from connecting to the server, done in MQTTNetworkMbedOs::connect()
     * and also different from connecting the network interface, that the socket is using.
     *
     * @param options options to be used for the connection.
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t connect(MQTTPacket_connectData &options);
    /**
     * @brief Connect to the MQTT broker via an MQTT-SN gateway
     *
     * This is different from connecting to the server, done in MQTTNetworkMbedOs::connect()
     * and also different from connecting the network interface, that the socket is using.
     *
     * @param options options to be used for the connection.
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t connect(MQTTSNPacket_connectData &options);

    /**
     * @brief Publish message to a topic.
     * @param topicName string with a topic name
     * @param message message to be published
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t publish(const char *topicName, MQTT::Message &message);
    /**
     * @brief Publish message to an MQTT-SN topic.
     * @param topicName MQTTSN_topicid structure specifying the topic.
     * @param message message to be published
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t publish(MQTTSN_topicid &topicName, MQTTSN::Message &message);

    /**
     * @brief Subscribe to a topic.
     * @param topicFilter string with a topic filter
     * @param qos level of qos to be received
     * @param mh message handler to be called upon message reception
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t subscribe(const char *topicFilter, enum MQTT::QoS qos, messageHandler mh);
    /**
     * @brief Subscribe to an MQTT-SN topic.
     * @param topicFilter string with a topic filter
     * @param qos level of qos to be received
     * @param mh message handler to be called upon message reception
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t subscribe(MQTTSN_topicid &topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh);

    /**
     * @brief Unsubscribe from a topic.
     * @param topicFilter string with a topic filter
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t unsubscribe(const char *topicFilter);
    /**
     * @brief Unsubscribe from an MQTT-SN topic.
     * @param topicFilter string with a topic filter
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t unsubscribe(MQTTSN_topicid &topicFilter);

    /**
     * @brief Yield current thread execution and handle other events
     * @param timeout_ms ms to yield for
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t yield(unsigned long timeout_ms = 1000L) ;

    /**
     * @brief Disconnect from a broker, that the client has been connected to.
     * @retval NSAPI_ERROR_OK on success, error code on failure
     */
    nsapi_error_t disconnect();

    /**
     * @brief Check whether client is connected to a broker.
     * @retval true if the client is connected, false otherwise
     */
    bool isConnected();

    /** Set the default message handling callback - used for any message which does not match a subscription message handler
     *  @param mh - pointer to the callback function.  Set to 0 to remove.
     */
    void setDefaultMessageHandler(messageHandler mh);

    /** Set a message handling callback.  This can be used outside of the the subscribe method.
     *  @param topicFilter - a topic pattern which can include wildcards
     *  @param mh - pointer to the callback function. If 0, removes the callback if any
     */
    nsapi_error_t setMessageHandler(const char *topicFilter, messageHandler mh);

private:
    /**
     * @brief Helper function to initialize member variables.
     */
    void init(Socket *sock);

    Socket *socket;
    MQTTNetworkMbedOs *mqttNet;
    NetworkInterface *net;

    MQTT::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS> *client;
    MQTTSN::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS> *clientSN;
};

#endif // MQTT_CLIENT_MBED_OS_H
