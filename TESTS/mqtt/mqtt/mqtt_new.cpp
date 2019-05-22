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

#include <MQTTClientMbedOs.h>
#include "mqtt_tests.h"
#include "unity/unity.h"

#define MQTT_API_INIT() \
    arrivedcount = 0; \
    NetworkInterface *net = NetworkInterface::get_default_instance(); \
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port); \
    TCPSocket socket; \
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net)); \
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.connect(sockAddr)); \
    MQTTClient client(&socket); \
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer; \
    data.MQTTVersion = 3;

#define MQTT_API_DEINIT() \
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.close());

void MQTT_CONNECT()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_CONNECT";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTT_API_DEINIT();
}

void MQTT_CONNECT_INVALID()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"";
    TEST_ASSERT_NOT_EQUAL(NSAPI_ERROR_OK, client.connect(data)); // Sending works. The retval from MQTTDeserialize_connack (2) is returned... Bug?
    MQTT_API_DEINIT();
}

void MQTT_CONNECT_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port);
    TCPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));
    MQTTClient client(&socket);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));
}

void MQTT_SUBSCRIBE()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_SUBSCRIBE";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS0, messageArrived));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS1, messageArrived));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS2, messageArrived));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::topic));
    MQTT_API_DEINIT();

}

void MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr("i.dont.exist", mqtt_global::port);
    TCPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, socket.connect(sockAddr));
    MQTTClient client(&socket);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(mqtt_global::topic, MQTT::QOS0, messageArrived));
}

void MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)""; // Left blank intentionally, so client.connect() fails.
    TEST_ASSERT_NOT_EQUAL(NSAPI_ERROR_OK, client.connect(data)); // Sending works. The retval from MQTTDeserialize_connack (2) is returned... Bug?
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(mqtt_global::topic, MQTT::QOS0, messageArrived));
    MQTT_API_DEINIT();
}

void MQTT_SUBSCRIBE_TOPIC_TOO_LONG()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_SUBSCRIBE_TOPIC_TOO_LONG";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(mqtt_global::topic_too_long, MQTT::QOS0, messageArrived));
    MQTT_API_DEINIT();
}

void MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(mqtt_global::topic, MQTT::QOS0, NULL));
    MQTT_API_DEINIT();
}

void MQTT_SUBSCRIBE_RECEIVE()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    int arrivedCountBeforeSubscription = arrivedcount;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::mbed_public_test_topic, MQTT::QOS0, messageArrived));
    while (arrivedCountBeforeSubscription == arrivedcount) {
        client.yield(100);
    }
    TEST_ASSERT_TRUE(arrivedCountBeforeSubscription < arrivedcount);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::mbed_public_test_topic));
    MQTT_API_DEINIT();
}

void MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::topic));
    MQTT_API_DEINIT();
}

void MQTT_UNSUBSCRIBE_INVALID()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_UNSUBSCRIBE_INVALID";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.unsubscribe(""));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.unsubscribe(mqtt_global::topic_too_long));
    MQTT_API_DEINIT();
}

void MQTT_PUBLISH()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_nnPUBLISH";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, mqtt_global::default_message));
    MQTT::Message msg = mqtt_global::default_message;
    msg.qos = MQTT::QOS1;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, msg));
    msg.qos = MQTT::QOS2;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, msg));
    MQTT_API_DEINIT();
}

void MQTT_PUBLISH_NOT_CONNECTED()
{
    MQTT_API_INIT();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.publish(mqtt_global::topic, mqtt_global::default_message));
    MQTT_API_DEINIT();
}

void MQTT_PUBLISH_TOPIC_TOO_LONG()
{
    MQTT_API_INIT();
    data.clientID.cstring = (char*)"MQTT_PUBLISH";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    // TODO verify if this is passing intentionally or if this is a bug?
//    TEST_ASSERT_EQUAL(-1, client.publish(mqtt_global::topic_too_long, mqtt_global::default_message));
    MQTT_API_DEINIT();
}

void MQTT_CONNECT_USER_PASSWORD_INCORRECT()
{
    MQTT_API_INIT();
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"MQTT_CONNECT_USER_PASSWORD_INCORRECT";
    data.username.cstring = (char*)"wronguser";
    data.password.cstring = (char*)"wrongpassword";
    TEST_ASSERT_EQUAL(5, client.connect(data));
    // Sending works. The retval from MQTTDeserialize_connack (5) is returned... Bug?
    MQTT_API_DEINIT();
}

void MQTT_CONNECT_SUBSCRIBE_PUBLISH()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port);
    TCPSocket socket;
    socket.open(net);
    socket.connect(sockAddr);

    MQTTClient client(&socket);

    send_messages<MQTTClient>(client, "MQTT_CONNECT_SUBSCRIBE_PUBLISH");

    socket.close();
}

void MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    TLSSocket *socket = new TLSSocket; // Allocate on heap to avoid stack overflow.
    TEST_ASSERT(NSAPI_ERROR_OK == socket->open(net));
    TEST_ASSERT(NSAPI_ERROR_OK == socket->set_root_ca_cert(mqtt_global::SSL_CA_PEM));
    int ret = socket->connect(mqtt_global::hostname, mqtt_global::port_tls);
    TEST_ASSERT(NSAPI_ERROR_OK == ret);

    MQTTClient client(socket);

    send_messages<MQTTClient>(client, "MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH");

    socket->close();
    delete socket;
}

void MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port);
    TCPSocket socket;
    socket.open(net);
    socket.connect(sockAddr);

    MQTTClient client(&socket);

    send_messages<MQTTClient>(client, "MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD");

    socket.close();
}
