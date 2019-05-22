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

#include "mbed.h"
#include "mqtt_tests.h"
#include "greentea-client/test_env.h"
#include "utest.h"

// Test for parameters
//#if !defined(MBED_CONF_APP_AP_MAC_SECURE)      || \
//    !defined(MBED_CONF_APP_MAX_SCAN_SIZE)      || \
//    !defined(MBED_CONF_APP_WIFI_CH_SECURE)     || \
//    !defined(MBED_CONF_APP_WIFI_PASSWORD)      || \
//    !defined(MBED_CONF_APP_WIFI_SECURE_SSID)   || \
//    !defined MBED_CONF_APP_WIFI_SECURE_PROTOCOL
//#error [NOT_SUPPORTED] Requires parameters from mbed_app.json (for secure connections)
//#endif

using namespace utest::v1;

const char* mqtt_global::SSL_CA_PEM =
/* Local mosquitto server certificate */
"-----BEGIN CERTIFICATE-----\n"
"MIIDzzCCAregAwIBAgIJAJF6EpVf4OtEMA0GCSqGSIb3DQEBCwUAMH4xCzAJBgNV\n"
"BAYTAlBMMRAwDgYDVQQIDAdtYXNvdmlhMREwDwYDVQQHDAhXYXJzemF3YTENMAsG\n"
"A1UECgwEbW9iaTEMMAoGA1UECwwDc2lsMRUwEwYDVQQDDAwxOTIuMTY4LjguNTIx\n"
"FjAUBgkqhkiG9w0BCQEWB21AbS5jb20wHhcNMTkwNTIxMDkzNjQ4WhcNMjQwNTIw\n"
"MDkzNjQ4WjB+MQswCQYDVQQGEwJQTDEQMA4GA1UECAwHbWFzb3ZpYTERMA8GA1UE\n"
"BwwIV2Fyc3phd2ExDTALBgNVBAoMBG1vYmkxDDAKBgNVBAsMA3NpbDEVMBMGA1UE\n"
"AwwMMTkyLjE2OC44LjUyMRYwFAYJKoZIhvcNAQkBFgdtQG0uY29tMIIBIjANBgkq\n"
"hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAz9+wntWYYOeDrd4EdUXF09OG54LbRyCY\n"
"nB+hzjVaPZM36RLE1ea/S04sb3T5MT5QzGi987B9KtAcDQMY9sSjv7XC8wqCv9wd\n"
"HDeSf3zRyiVFSbhjWBi4i7BcEHLEjXUinWowwE7QV8UpAKJUsWBX1w8vbe7ooYvX\n"
"y3JNPMfhtwGSVtqY0PeAHviLo6Lg8oFJFocFHeDh08W9p5J51SdRh4/CK+eVRpLd\n"
"N1vtEePNuemZyM2HT839rPO53j9/Vvbl2nEfDCn3+jSPCcNSwiBsz0ErxZgW76fV\n"
"wJnhcBSStMVrd1KhAcf1Um56jSIXMT0sMDk4j8sr/7ROJ72HjXSntwIDAQABo1Aw\n"
"TjAdBgNVHQ4EFgQUhOGXTz5aztPFKmEV2g/tKwA8NMswHwYDVR0jBBgwFoAUhOGX\n"
"Tz5aztPFKmEV2g/tKwA8NMswDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOC\n"
"AQEATnQzWf+MYEwtKDZfrxR5SJnBHc5H4Frq2wNVY+h+p/dOUxpshfa0/2AgDGzX\n"
"+pIQpA9o5Zo00sX6Fa7gazheWw0iytSEIlFy9vvEm9EG2iH/Bsoki4E6GM334xUF\n"
"1o+LH6vVm0J3DIGOAUup6gLs2efGME59hFJefooEAzVhVqgSgPVV09ZCSMo8M6no\n"
"qlUfl2sGOJ5mchA3bdGuF2YlWACngwgf/cOUunkoZixz2Nou6ma8antAKTGl9fkK\n"
"320LiO6s7tnQ13B9fjd9sZMnIZVZVO962oGm7KaLWAQPrLNX5hypIpo3iHqdepbG\n"
"cOTba8KCydjqsOaXl/nYT+1ULQ==\n"
"-----END CERTIFICATE-----";

/*
 * (optional) Client certificate here in PEM format.
 * Set NULL if you don't use.
 * "-----BEGIN CERTIFICATE-----\n"
 * ...
 * "-----END CERTIFICATE-----\n";
 */
const char* mqtt_global::SSL_CLIENT_CERT_PEM = NULL;

/*
 * (optional) Client private key here in PEM format.
 * Set NULL if you don't use.
 * "-----BEGIN RSA PRIVATE KEY-----\n"
 * ...
 * "-----END RSA PRIVATE KEY-----\n";
 */
const char* mqtt_global::SSL_CLIENT_PRIVATE_KEY_PEM = NULL;

#define MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
#define RUN_INTEGRATION_TESTS
#define MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE

MQTT::Message mqtt_global::default_message;
MQTTSN::Message mqtt_global::default_message_sn;

Case cases[] = {

    // Legacy API
    Case("MQTT_LEGACY_CONNECT", MQTT_LEGACY_CONNECT),
    Case("MQTT_LEGACY_CONNECT_INVALID", MQTT_LEGACY_CONNECT_INVALID),
    Case("MQTT_LEGACY_CONNECT_NOT_CONNECTED", MQTT_LEGACY_CONNECT_NOT_CONNECTED),
    Case("MQTT_LEGACY_SUBSCRIBE", MQTT_LEGACY_SUBSCRIBE),
    Case("MQTT_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED", MQTT_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED),
    Case("MQTT_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED", MQTT_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED),
    Case("MQTT_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG", MQTT_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG),
    Case("MQTT_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER", MQTT_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER),
#ifdef MQTT_LEGACY_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
    Case("MQTT_LEGACY_SUBSCRIBE_RECEIVE", MQTT_LEGACY_SUBSCRIBE_RECEIVE),
#endif
    Case("MQTT_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE", MQTT_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE),
    Case("MQTT_LEGACY_UNSUBSCRIBE_INVALID", MQTT_LEGACY_UNSUBSCRIBE_INVALID),
    Case("MQTT_LEGACY_PUBLISH", MQTT_LEGACY_PUBLISH),
    Case("MQTT_LEGACY_PUBLISH_NOT_CONNECTED", MQTT_LEGACY_PUBLISH_NOT_CONNECTED),
    Case("MQTT_LEGACY_PUBLISH_TOPIC_TOO_LONG", MQTT_LEGACY_PUBLISH_TOPIC_TOO_LONG),
#ifdef MQTT_LEGACY_MBED_PASSWORD_PROTECTION_AVAILABLE
    Case("MQTT_LEGACY_CONNECT_USER_PASSWORD_INCORRECT", MQTT_LEGACY_CONNECT_USER_PASSWORD_INCORRECT),
#endif
#ifdef RUN_INTEGRATION_TESTS
#ifdef MQTT_LEGACY_MBED_PASSWORD_PROTECTION_AVAILABLE
    Case("MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD", MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD),
#endif
    Case("MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH", MQTT_LEGACY_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_LEGACY_TLS_CONNECT_SUBSCRIBE_PUBLISH", MQTT_LEGACY_TLS_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_CONNECT_SUBSCRIBE_PUBLISH", MQTT_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH", MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH),
#endif

    // MQTT-SN
    Case("MQTTSN_LEGACY_TEST_CONNECT", MQTTSN_LEGACY_TEST_CONNECT),
    Case("MQTTSN_LEGACY_CONNECT_INVALID", MQTTSN_LEGACY_CONNECT_INVALID),
    Case("MQTTSN_LEGACY_CONNECT_NOT_CONNECTED", MQTTSN_LEGACY_CONNECT_NOT_CONNECTED),
    Case("MQTTSN_LEGACY_TEST_SUBSCRIBE", MQTTSN_LEGACY_TEST_SUBSCRIBE),
    Case("MQTTSN_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED", MQTTSN_LEGACY_SUBSCRIBE_NETWORK_NOT_CONNECTED),
    Case("MQTTSN_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED", MQTTSN_LEGACY_SUBSCRIBE_CLIENT_NOT_CONNECTED),
    Case("MQTTSN_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG", MQTTSN_LEGACY_SUBSCRIBE_TOPIC_TOO_LONG),
    Case("MQTTSN_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER", MQTTSN_LEGACY_SUBSCRIBE_INVALID_MESSAGE_HANDLER),
#ifdef MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
    Case("MQTTSN_LEGACY_SUBSCRIBE_RECEIVE", MQTTSN_LEGACY_SUBSCRIBE_RECEIVE),
#endif
    Case("MQTTSN_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE", MQTTSN_LEGACY_UNSUBSCRIBE_WITHOUT_SUBSCRIBE),
    Case("MQTTSN_LEGACY_UNSUBSCRIBE_INVALID", MQTTSN_LEGACY_UNSUBSCRIBE_INVALID),
    Case("MQTTSN_LEGACY_TEST_PUBLISH", MQTTSN_LEGACY_TEST_PUBLISH),
    Case("MQTTSN_LEGACY_PUBLISH_NOT_CONNECTED", MQTTSN_LEGACY_PUBLISH_NOT_CONNECTED),
    Case("MQTTSN_LEGACY_PUBLISH_TOPIC_TOO_LONG", MQTTSN_LEGACY_PUBLISH_TOPIC_TOO_LONG),
#ifdef RUN_INTEGRATION_TESTS
    Case("MQTTSN_LEGACY_UDP_CONNECT_SUBSCRIBE_PUBLISH", MQTTSN_LEGACY_UDP_CONNECT_SUBSCRIBE_PUBLISH),
#endif
    Case("MQTTSN_LEGACY_IS_CONNECTED", MQTTSN_LEGACY_IS_CONNECTED),
    Case("MQTTSN_LEGACY_IS_CONNECTED_CLIENT_NOT_CONNECTED", MQTTSN_LEGACY_IS_CONNECTED_CLIENT_NOT_CONNECTED),
    Case("MQTTSN_LEGACY_IS_CONNECTED_NETWORK_NOT_CONNECTED", MQTTSN_LEGACY_IS_CONNECTED_NETWORK_NOT_CONNECTED),


    // New mbed-os API
    Case("MQTT_CONNECT", MQTT_CONNECT),
    Case("MQTT_CONNECT_INVALID", MQTT_CONNECT_INVALID),
    Case("MQTT_CONNECT_NOT_CONNECTED", MQTT_CONNECT_NOT_CONNECTED),
    Case("MQTT_SUBSCRIBE", MQTT_SUBSCRIBE),
    Case("MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED", MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED),
    Case("MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED", MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED),
    Case("MQTT_SUBSCRIBE_TOPIC_TOO_LONG", MQTT_SUBSCRIBE_TOPIC_TOO_LONG),
    Case("MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER", MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER),
#ifdef MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
    Case("MQTT_SUBSCRIBE_RECEIVE", MQTT_SUBSCRIBE_RECEIVE),
#endif
    Case("MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE", MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE),
    Case("MQTT_UNSUBSCRIBE_INVALID", MQTT_UNSUBSCRIBE_INVALID),
    Case("MQTT_PUBLISH", MQTT_PUBLISH),
    Case("MQTT_PUBLISH_NOT_CONNECTED", MQTT_PUBLISH_NOT_CONNECTED),
    Case("MQTT_PUBLISH_TOPIC_TOO_LONG", MQTT_PUBLISH_TOPIC_TOO_LONG),
#ifdef MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE
    Case("MQTT_CONNECT_USER_PASSWORD_INCORRECT", MQTT_CONNECT_USER_PASSWORD_INCORRECT),
#endif
#ifdef RUN_INTEGRATION_TESTS
#ifdef MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE
    Case("MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD", MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD),
#endif
    Case("MQTT_CONNECT_SUBSCRIBE_PUBLISH", MQTT_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH", MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_CONNECT_SUBSCRIBE_PUBLISH", MQTT_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH", MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH),
#endif

    // MQTT-SN new API
    Case("MQTTSN_TEST_CONNECT", MQTTSN_TEST_CONNECT),
    Case("MQTTSN_CONNECT_INVALID", MQTTSN_CONNECT_INVALID),
    Case("MQTTSN_CONNECT_NOT_CONNECTED", MQTTSN_CONNECT_NOT_CONNECTED),
    Case("MQTTSN_TEST_SUBSCRIBE", MQTTSN_TEST_SUBSCRIBE),
    Case("MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED", MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED),
    Case("MQTTSN_SUBSCRIBE_CLIENT_NOT_CONNECTED", MQTTSN_SUBSCRIBE_CLIENT_NOT_CONNECTED),
    Case("MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG", MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG),
    Case("MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER", MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER),
#ifdef MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
    Case("MQTTSN_SUBSCRIBE_RECEIVE", MQTTSN_SUBSCRIBE_RECEIVE),
#endif
    Case("MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE", MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE),
    Case("MQTTSN_UNSUBSCRIBE_INVALID", MQTTSN_UNSUBSCRIBE_INVALID),
    Case("MQTTSN_TEST_PUBLISH", MQTTSN_TEST_PUBLISH),
    Case("MQTTSN_PUBLISH_NOT_CONNECTED", MQTTSN_PUBLISH_NOT_CONNECTED),
    Case("MQTTSN_PUBLISH_TOPIC_TOO_LONG", MQTTSN_PUBLISH_TOPIC_TOO_LONG),
#ifdef RUN_INTEGRATION_TESTS
    Case("MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH", MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH),
#endif
    Case("MQTTSN_IS_CONNECTED", MQTTSN_IS_CONNECTED),
    Case("MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED", MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED),
    Case("MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED", MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED),
};

int arrivedcount = 0;
int arrivedcountSN = 0;

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
    printf("arrived msg: %d\n", arrivedcount);
}

void messageArrivedSN(MQTTSN::MessageData& md)
{
    MQTTSN::Message &message = md.message;
    printf("Message arrived SN: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcountSN;
    printf("arrived msg: %d\n", arrivedcountSN);
}

void init_topic_sn(MQTTSN_topicid& topic_sn) {
    topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topic_sn.data.long_.len = strlen(mqtt_global::topic);
    topic_sn.data.long_.name = const_cast<char*>(mqtt_global::topic);
}

void init_topic_sn_too_long(MQTTSN_topicid& topic_sn) {
    topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topic_sn.data.long_.len = strlen(mqtt_global::topic_too_long);
    topic_sn.data.long_.name = const_cast<char*>(mqtt_global::topic_too_long);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifdef MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S
    GREENTEA_SETUP(MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S, "default_auto");
#else
    GREENTEA_SETUP(480, "default_auto");
#endif

    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    // Generate an string of length MAX_MQTT_PACKET_SIZE+1 with alphabet letters.
    char c = 'a';
    for (int i = 0; i < 100+1; i++) {
        mqtt_global::topic_too_long[i] = c;
        if (c < 'z') {
            c++;
        } else {
            c = 'a';
        }
    }

    // Prepare a default message, to avoid code duplication at a later stage.
    sprintf(mqtt_global::message_buffer, "%s", "MQTT_MBED_OS_TEST_MESSAGE");
    mqtt_global::default_message.qos = MQTT::QOS0;
    mqtt_global::default_message.retained = false;
    mqtt_global::default_message.dup = false;
    mqtt_global::default_message.payload = (void*)mqtt_global::message_buffer;
    mqtt_global::default_message.payloadlen = strlen(mqtt_global::message_buffer)+1;

    sprintf(mqtt_global::message_buffer, "%s", "MQTTSN_MBED_OS_TEST_MESSAGE");
    mqtt_global::default_message_sn.qos = MQTTSN::QOS0;
    mqtt_global::default_message_sn.retained = false;
    mqtt_global::default_message_sn.dup = false;
    mqtt_global::default_message_sn.payload = (void*)mqtt_global::message_buffer;
    mqtt_global::default_message_sn.payloadlen = strlen(mqtt_global::message_buffer)+1;

    mqtt_global::mbed_public_test_topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    mqtt_global::mbed_public_test_topic_sn.data.long_.len = strlen(mqtt_global::mbed_public_test_topic)+1;
    mqtt_global::mbed_public_test_topic_sn.data.long_.name = const_cast<char*>(mqtt_global::mbed_public_test_topic);

    return verbose_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    NetworkInterface::get_default_instance()->disconnect();
    return greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
