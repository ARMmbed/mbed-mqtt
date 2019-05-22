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

#include <MQTTNetwork.h>
#include <MQTTNetworkTLS.h>
#include "mqtt_tests.h"

#include <MQTTClient.h>
#include <MQTTmbed.h> // Countdown

#define MQTT_LEGACY_API_INIT() \
    arrivedcount = 0; \
    NetworkInterface *net = NetworkInterface::get_default_instance(); \
    MQTTNetwork mqttNet(net); \
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet); \
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.connect(mqtt_global::hostname, mqtt_global::port)); \
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer; \
    data.MQTTVersion = 3;

#define MQTT_LEGACY_API_DEINIT() TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.disconnect());

void MQTT_LEGACY_CONNECT()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_CONNECT";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_CONNECT_INVALID()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"";
    TEST_ASSERT_NOT_EQUAL(NSAPI_ERROR_OK, client.connect(data)); // Sending works. The retval from MQTTDeserialize_connack (2) is returned... Bug?
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_CONNECT_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    MQTTNetwork mqttNet(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_DNS_FAILURE, mqttNet.connect("i.dont.exist", mqtt_global::port));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    TEST_ASSERT_EQUAL(-1, client.connect(data));
}

void MQTT_LEGACY_SUBSCRIBE()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_SUBSCRIBE";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS0, messageArrived));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS1, messageArrived));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::topic, MQTT::QOS2, messageArrived));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::topic));
    MQTT_LEGACY_API_DEINIT();

}

void MQTT_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    MQTTNetwork mqttNet(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_DNS_FAILURE, mqttNet.connect("i.dont.exist", mqtt_global::port));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"MQTT_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED";
    TEST_ASSERT_EQUAL(-1, client.connect(data));
    TEST_ASSERT_EQUAL(-1, client.subscribe(mqtt_global::topic, MQTT::QOS0, messageArrived));
}

void MQTT_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)""; // Left blank intentionally, so client.connect() fails.
    TEST_ASSERT_NOT_EQUAL(NSAPI_ERROR_OK, client.connect(data)); // Sending works. The retval from MQTTDeserialize_connack (2) is returned... Bug?
    TEST_ASSERT_EQUAL(-1, client.subscribe(mqtt_global::topic, MQTT::QOS0, messageArrived));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(-1, client.subscribe(mqtt_global::topic_too_long, MQTT::QOS0, messageArrived));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(-1, client.subscribe(mqtt_global::topic, MQTT::QOS0, NULL));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_SUBSCRIBE_RECEIVE()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    int arrivedCountBeforeSubscription = arrivedcount;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.subscribe(mqtt_global::mbed_public_test_topic, MQTT::QOS0, messageArrived));
    while (arrivedCountBeforeSubscription == arrivedcount) {
        client.yield(100);
    }
    TEST_ASSERT_TRUE(arrivedCountBeforeSubscription < arrivedcount);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::mbed_public_test_topic));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.unsubscribe(mqtt_global::topic));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_UNSUBSCRIBE_INVALID()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_UNSUBSCRIBE_INVALID";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(-1, client.unsubscribe(""));
    TEST_ASSERT_EQUAL(-1, client.unsubscribe(mqtt_global::topic_too_long));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_PUBLISH()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_nnPUBLISH";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, mqtt_global::default_message));
    MQTT::Message msg = mqtt_global::default_message;
    msg.qos = MQTT::QOS1;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, msg));
    msg.qos = MQTT::QOS2;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.publish(mqtt_global::topic, msg));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_PUBLISH_NOT_CONNECTED()
{
    MQTT_LEGACY_API_INIT();
    TEST_ASSERT_EQUAL(-1, client.publish(mqtt_global::topic, mqtt_global::default_message));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_PUBLISH_TOPIC_TOO_LONG()
{
    MQTT_LEGACY_API_INIT();
    data.clientID.cstring = (char*)"MQTT_LEGACY_PUBLISH";
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, client.connect(data));
    // TODO verify if this is passing intentionally or if this is a bug?
//    TEST_ASSERT_EQUAL(-1, client.publish(mqtt_global::topic_too_long, mqtt_global::default_message));
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_CONNECT_USER_PASSWORD_INCORRECT()
{
    MQTT_LEGACY_API_INIT();
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"MQTT_LEGACY_CONNECT_USER_PASSWORD_INCORRECT";
    data.username.cstring = (char*)"wronguser";
    data.password.cstring = (char*)"wrongpassword";
    TEST_ASSERT_EQUAL(5, client.connect(data));
    // Sending works. The retval from MQTTDeserialize_connack (5) is returned... Bug?
    MQTT_LEGACY_API_DEINIT();
}

void MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    MQTTNetwork mqttNet(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.connect(mqtt_global::hostname, mqtt_global::port));

    send_messages< MQTT::Client<MQTTNetwork, Countdown> >(client, "MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH");

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.disconnect());
}

void MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    MQTTNetwork mqttNet(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.connect(mqtt_global::hostname, mqtt_global::port));

    send_messages< MQTT::Client<MQTTNetwork, Countdown> >(client, "MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD", true);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.disconnect());
}

void MQTT_LEGACY_TLS_CONNECT_SUBSCRIBE_PUBLISH()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    MQTTNetworkTLS mqttNet(net);

    MQTT::Client<MQTTNetworkTLS, Countdown> client(mqttNet);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.connect(mqtt_global::hostname, mqtt_global::port_tls,
            mqtt_global::SSL_CA_PEM, mqtt_global::SSL_CLIENT_CERT_PEM, mqtt_global::SSL_CLIENT_PRIVATE_KEY_PEM));

    send_messages< MQTT::Client<MQTTNetworkTLS, Countdown> >(client, "MQTT_LEGACYTLS_CONNECT_SUBSCRIBE_PUBLISH");

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, mqttNet.disconnect());
}
