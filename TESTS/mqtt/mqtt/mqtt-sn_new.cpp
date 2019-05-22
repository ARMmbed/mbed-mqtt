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

#include "mqtt_tests.h"

#include <MQTTClientMbedOs.h>

#define MQTTSN_API_INIT() \
    arrivedcountSN = 0; \
    NetworkInterface *net = NetworkInterface::get_default_instance(); \
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port_udp); \
    UDPSocket socket; \
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net)); \
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.connect(sockAddr)); \
    MQTTClient client(&socket); \
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;

#define MQTTSN_API_DEINIT() \
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.close());\

void MQTTSN_TEST_CONNECT()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_CONNECT";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_API_DEINIT();
}

void MQTTSN_CONNECT_INVALID()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));
    MQTTSN_API_DEINIT();
}

void MQTTSN_CONNECT_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr("i.dont.exist", mqtt_global::port_udp);
    UDPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));

    //Connect in UDP is not real, it will return success...
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.connect(sockAddr));
    MQTTClient client(&socket);
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;

    //... but we should not be able to connect to the server.
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));
}

void MQTTSN_TEST_SUBSCRIBE()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_TEST_SUBSCRIBE";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(topic_sn, MQTTSN::QOS0, messageArrivedSN));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(topic_sn));
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(topic_sn, MQTTSN::QOS1, messageArrivedSN));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(topic_sn));
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(topic_sn, MQTTSN::QOS2, messageArrivedSN));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(topic_sn));
    MQTTSN_API_DEINIT();

}

void MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr("i.dont.exist", mqtt_global::port_udp);
    UDPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));

    //Connect in UDP is not real, it will return success...
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.connect(sockAddr));
    MQTTClient client(&socket);
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;
    data.clientID.cstring = (char*)"MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED";
    // ... but the connect should fail ...
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    // ... and subscribe should also fail.
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(topic_sn, MQTTSN::QOS0, messageArrivedSN));
}

void MQTTSN_SUBSCRIBE_CLIENT_NOT_CONNECTED()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(topic_sn, MQTTSN::QOS0, messageArrivedSN));
    MQTTSN_API_DEINIT();
}

void MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic_sn;
    init_topic_sn_too_long(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.subscribe(topic_sn, MQTTSN::QOS0, messageArrivedSN));
    MQTTSN_API_DEINIT();
}

void MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    // There is no NULL check inside the subscribe function.
//    TEST_ASSERT_EQUAL(-1, client.subscribe(topic_sn, MQTTSN::QOS0, NULL));
    MQTTSN_API_DEINIT();
}

void MQTTSN_SUBSCRIBE_RECEIVE()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    int arrivedCountBeforeSubscription = arrivedcountSN;
    MQTTSN_topicid topic_sn;
    topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topic_sn.data.long_.len = strlen(mqtt_global::mbed_public_test_topic);
    topic_sn.data.long_.name = const_cast<char*>(mqtt_global::mbed_public_test_topic);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(topic_sn, MQTTSN::QOS0, messageArrivedSN));
    // TODO get the callbacks working for MQTT-SN
    while (arrivedCountBeforeSubscription == arrivedcountSN) {
        client.yield(100);
    }
    TEST_ASSERT_TRUE(arrivedCountBeforeSubscription < arrivedcountSN);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(topic_sn));
    MQTTSN_API_DEINIT();
}

void MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(topic_sn));
    MQTTSN_API_DEINIT();
}

void MQTTSN_UNSUBSCRIBE_INVALID()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_UNSUBSCRIBE_INVALID";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic_sn_too_long;
    init_topic_sn_too_long(topic_sn_too_long);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.unsubscribe(topic_sn_too_long));
    MQTTSN_API_DEINIT();
}

void MQTTSN_TEST_PUBLISH()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_PUBLISH";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(topic_sn, mqtt_global::default_message_sn));
    MQTTSN::Message msg = mqtt_global::default_message_sn;
    msg.qos = MQTTSN::QOS1;
    // TODO: get QoS 1 and 2 working. Maybe we need to reconnect?
//    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(topic_sn, msg));
//    msg.qos = MQTTSN::QOS2;
//    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(topic_sn, msg));
    MQTTSN_API_DEINIT();
}

void MQTTSN_PUBLISH_NOT_CONNECTED()
{
    MQTTSN_API_INIT();
    MQTTSN_topicid topic_sn;
    init_topic_sn(topic_sn);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.publish(topic_sn, mqtt_global::default_message_sn));
    MQTTSN_API_DEINIT();
}

void MQTTSN_PUBLISH_TOPIC_TOO_LONG()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTT_PUBLISH";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    // TODO verify if this is passing intentionally or if this is a bug?
//    TEST_ASSERT_EQUAL(-1, client.publish(mqtt_global::topic_too_long, mqtt_global::default_message));
    MQTTSN_API_DEINIT();
}

// QTT-SN does not provide the user/password functionality so
// MQTT_CONNECT_USER_PASSWORD_INCORRECT and MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD
// cannot be executed for MQTT-SN

void MQTTSN_IS_CONNECTED()
{
    MQTTSN_API_INIT();
    data.clientID.cstring = (char*)"MQTTSN_IS_CONNECTED";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_TRUE(client.isConnected());
    MQTTSN_API_DEINIT();
}

void MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED()
{
    MQTTSN_API_INIT();
    TEST_ASSERT_FALSE(client.isConnected());
    MQTTSN_API_DEINIT();
}

void MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr("i.dont.exist", mqtt_global::port_udp);
    UDPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));

    //Connect in UDP is not real, it will return success...
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.connect(sockAddr));
    MQTTClient client(&socket);
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;
    data.clientID.cstring = (char*)"MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED";

    TEST_ASSERT_FALSE(client.isConnected());

    // ... but the connect should fail ...
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, client.connect(data));

    TEST_ASSERT_FALSE(client.isConnected());
}

void MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port_udp);
    UDPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));
    TEST_ASSERT(NSAPI_ERROR_OK == socket.connect(sockAddr));
    MQTTClient client(&socket);

    send_messages_sn< MQTTClient >(client, "MQTT_FULL_NEW_UDP");

    socket.close();
}
