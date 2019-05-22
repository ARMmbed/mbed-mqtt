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

#ifndef MQTT_TESTS_H
#define MQTT_TESTS_H

#include <MQTTClientMbedOs.h>

#include "unity/unity.h"

namespace mqtt_global {
// For TLS hostname must match the "Common Name" set in the server certificate
//const char* hostname = "test.mosquitto.org";
static const char* hostname = "192.168.8.52";
static const char* topic = "test";
static const char* mbed_public_test_topic = "mbed_public_test_topic";
static char topic_too_long[100+1];
static MQTTSN_topicid mbed_public_test_topic_sn;
static const int port = 1883;
static const int port_tls = 8883;
static const int port_udp = 10000;
extern const char* SSL_CA_PEM;
extern const char* SSL_CLIENT_CERT_PEM;
extern const char* SSL_CLIENT_PRIVATE_KEY_PEM;
extern MQTT::Message default_message;
extern MQTTSN::Message default_message_sn;
static char message_buffer[100];
}

extern int arrivedcount;
extern int arrivedcountSN;
void messageArrived(MQTT::MessageData& md);
void messageArrivedSN(MQTTSN::MessageData& md);

/*
 * Test cases
 */
void MQTT_LEGACY_CONNECT_NOT_CONNECTED();
void MQTT_LEGACY_CONNECT();
void MQTT_LEGACY_CONNECT_INVALID();
void MQTT_LEGACY_SUBSCRIBE();
void MQTT_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED();
void MQTT_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED();
void MQTT_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG();
void MQTT_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER();
void MQTT_LEGACY_SUBSCRIBE_RECEIVE();
void MQTT_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE();
void MQTT_LEGACY_UNSUBSCRIBE_INVALID();
void MQTT_LEGACY_PUBLISH();
void MQTT_LEGACY_PUBLISH_NOT_CONNECTED();
void MQTT_LEGACY_PUBLISH_TOPIC_TOO_LONG();
void MQTT_LEGACY_CONNECT_USER_PASSWORD_INCORRECT();
void MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH();
void MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD();
void MQTT_LEGACY_TLS_CONNECT_SUBSCRIBE_PUBLISH();

void MQTTSN_LEGACY_CONNECT_NOT_CONNECTED();
void MQTTSN_LEGACY_TEST_CONNECT(); // Avoid clash with MQTTSN enum.
void MQTTSN_LEGACY_CONNECT_INVALID();
void MQTTSN_LEGACY_TEST_SUBSCRIBE(); // Avoid clash with MQTTSN enum.
void MQTTSN_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED();
void MQTTSN_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED();
void MQTTSN_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG();
void MQTTSN_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER();
void MQTTSN_LEGACY_SUBSCRIBE_RECEIVE();
void MQTTSN_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE();
void MQTTSN_LEGACY_UNSUBSCRIBE_INVALID();
void MQTTSN_LEGACY_TEST_PUBLISH();
void MQTTSN_LEGACY_PUBLISH_NOT_CONNECTED();
void MQTTSN_LEGACY_PUBLISH_TOPIC_TOO_LONG();
void MQTTSN_LEGACY_IS_CONNECTED();
void MQTTSN_LEGACY_IS_CONNECTED_CLIENT_NOT_CONNECTED();
void MQTTSN_LEGACY_IS_CONNECTED_NETWORK_NOT_CONNECTED();
void MQTTSN_LEGACY_UDP_CONNECT_SUBSCRIBE_PUBLISH();

// New API:
void MQTT_CONNECT_NOT_CONNECTED();
void MQTT_CONNECT();
void MQTT_CONNECT_INVALID();
void MQTT_SUBSCRIBE();
void MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED();
void MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED();
void MQTT_SUBSCRIBE_TOPIC_TOO_LONG();
void MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER();
void MQTT_SUBSCRIBE_RECEIVE();
void MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE();
void MQTT_UNSUBSCRIBE_INVALID();
void MQTT_PUBLISH();
void MQTT_PUBLISH_NOT_CONNECTED();
void MQTT_PUBLISH_TOPIC_TOO_LONG();
void MQTT_CONNECT_USER_PASSWORD_INCORRECT();
void MQTT_CONNECT_SUBSCRIBE_PUBLISH();
void MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD();
void MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH();


void MQTTSN_CONNECT_NOT_CONNECTED();
void MQTTSN_TEST_CONNECT(); // Avoid clash with MQTTSN enum.
void MQTTSN_CONNECT_INVALID();
void MQTTSN_TEST_SUBSCRIBE(); // Avoid clash with MQTTSN enum.
void MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED();
void MQTTSN_SUBSCRIBE_CLIENT_NOT_CONNECTED();
void MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG();
void MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER();
void MQTTSN_SUBSCRIBE_RECEIVE();
void MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE();
void MQTTSN_UNSUBSCRIBE_INVALID();
void MQTTSN_TEST_PUBLISH();
void MQTTSN_PUBLISH_NOT_CONNECTED();
void MQTTSN_PUBLISH_TOPIC_TOO_LONG();
void MQTTSN_IS_CONNECTED();
void MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED();
void MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED();
void MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH();

template <class Client> void send_messages(Client &client, char *clientID, bool user_password = false) {
    arrivedcount = 0;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)clientID;
    if (user_password) {
        data.username.cstring = (char*)"mbed";
        data.password.cstring = (char*)"1234";
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS2, messageArrived));

    MQTT::Message message = mqtt_global::default_message;

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, message));
    while (arrivedcount < 1)
        client.yield(100);

    // QoS 1
    char buf[100];
    sprintf(buf, "QoS 1 %s\n", clientID);
    message.qos = MQTT::QOS1;
    message.payloadlen = strlen(buf)+1;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, message));
    while (arrivedcount < 2)
        client.yield(100);

//    // QoS 2
//    sprintf(buf, "QoS 2 %s\n", clientID);
//    message.qos = MQTT::QOS2;
//    message.payloadlen = strlen(buf)+1;
//    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(topic, message));
//    while (arrivedcount < 3)
//        client.yield(100);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::topic));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.disconnect() != 0);
}

/**
 * @brief Initialize the topic with default values.
 *
 * MQTT-SN subscribe() might modify the topic, so we can't use a predefined global variable.
 */
void init_topic_sn(MQTTSN_topicid& topic_sn);
void init_topic_sn_too_long(MQTTSN_topicid& topic_sn);

template <class Client> void send_messages_sn(Client &client, char *clientID) {
    arrivedcountSN = 0;
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;
    data.clientID.cstring = clientID;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTTSN_topicid topic;
    init_topic_sn(topic);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(topic, MQTTSN::QOS2, messageArrivedSN));

    // QoS 0
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(topic, mqtt_global::default_message_sn));
    while (arrivedcountSN < 1)
        client.yield(100);

    // QoS 1
    MQTTSN::Message message;
    char buf[100];
    sprintf(buf, "QoS 1 %s\n", clientID);
    message.qos = MQTTSN::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(topic, message));
    while (arrivedcountSN < 2)
        client.yield(100);

//    // QoS 2
//    sprintf(buf, "QoS 2 %s\n", clientID);
//    message.qos = MQTTSN::QOS2;
//    message.payloadlen = strlen(buf)+1;
//    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK,client.publish(topic, message));
//    while (arrivedcountSN < 3)
//        client.yield(100);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(topic)); //mqtt_global::topic_sn
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.disconnect() != 0);
}

#endif //MQTT_TESTS_H
