MQTT test plan
========================

This is a test plan for the Mbed OS MQTT and MQTT-SN protocols. This describes all test cases and their intended behaviors. When an API document is not clear, use this as a reference for implementing correct behavior.

Target API
----------

The target for this plan is to test (TODO: update links to point to mbed-os master after merge):

-   [legacy MQTTClient paho implementation](../../features/mqtt/paho_mqtt_embedded_c/MQTTClient) with [mbed-os MQTTNetwork implementations](../../mqtt/features/mqtt/src).
-   [new MQTTClient mbed-os wrapper (both for MQTT and MQTT-SN)](../../mqtt/src/MQTTClientNew.h).

Mbed-os tests are NOT meant to test the underlying Eclipse paho [MQTT](https://github.com/michalpasztamobica/mbed-os/tree/mqtt/features/mqtt/paho_mqtt_embedded_c) and [MQTT-SN](../../features/mqtt/paho_mqtt-sn_embedded_c) libraries. The tests provided are integration tests, checking whether MQTT works correctly with the mbed-os sockets.

Tools to use
------------

-   Mbed OS.
-   Standard Mbed OS development tools as described in the [Arm Mbed tools overview](https://os.mbed.com/docs/latest/tools/index.html).
-   Test server with an MQTT broker.

These test cases themselves do not require any special tooling, other than the MQTT test server described in "Test environment" chapter.

Test environment
----------------

As a general guideline, network connectivity with public Internet access is required. This satisfies Socket tests, but specific connectivity methods might require some extra configuration or devices within the network.

The test environment consist of DUTs, network connection and an MQTT broker.

### Test server setup

**Required services in the test server**

-   MQTT protcol is enabled on TCP port 1883. Port 8883 for TLS
-   MQTT-SN protocol is enabled on UDP port 10000.

Configure the firewall to allow this traffic to access the MQTT test server.

Some tests are wrapped with a MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE macro. They will only work if a topic is provided in the broker named "mbed_public_test_topic", that publishes a message with QoS = 0 and content "Test message" with a constant frequency. This is intended for use in testing the subscription without having to publish any additional messages.

**Example configuration for Debian/Ubuntu Linux**

The example test server is using an open-source [Eclipse Mosquitto MQTT broker](https://mosquitto.org/) and [Eclipse paho MQTT-SN Gateway](https://www.eclipse.org/paho/components/mqtt-sn-transparent-gateway/), which are available on many operating systems, and installing them is out of scope of this document. Below is an example of how to install these services into a Debian/Ubuntu based Linux distribution.

***MQTT***

```.sh
$ sudo apt install mosquitto
```

The mosquitto broker will start automatically as a deamon on system startup. You can start, stop or restart it using:
```.sh
$ sudo service mosquitto [start|stop|restart]
```

Install the mosquitto clients to be able to test your mosquitto broker locally and generate traffic for a predefined channel:

```.sh
$ sudo apt install mosquitto-clients
```

When running the below commands in separate shells:

```.sh
$ mosquitto_sub -p 1883 -t "testtopic"
$ mosquitto_pub -p 1883 -t "testtopic" -m "testmessage"
```
You should be able to receive the "testmessage" on the subscriber side.

Create a channel that will be fed with messages in a periodic manner. An example of how this can be achieved:

```
$ watch -n1 mosquitto_pub -p 1883 -m \"Test message\" -t \"mbed_public_test_topic\" -q 0
```

Make sure QoS is set to 0 (`-q 0`), as this message might be received by multiple devices (or no devices after the tests ends).

Tests which will fail without this functionality are wrapped with a MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE macro.

Optionally edit (or create) the mosquitto broker configuration in /etc/mosquitto/conf.d/ to adjust the port that is used for communication (by default it's 1883):

```
# Port to use for default listener
listener 1883
```

To be able to access the broker from the network you need to open the tcp port that the broker is using:
```.sh
$ sudo ufw allow 1883/tcp
```

Some tests are protected with MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE macro. Those tests can only be run when the MQTT broker is configured to check for a user and password. Follow [this tutorial](http://www.steves-internet-guide.com/mqtt-username-password-example/) to configure the broker.

Make sure that allow_anonymous is set to true, as most test access the broker anonymously.

The tests are by default using user "mbed" and password "1234", so consider setting those on your broker or adjust the tests.

Bear in mind that MQTT user/password is sent in raw text and it has nothing to do with proper TLS encryption described below.

***MQTT TLS***

By default port 8883 is used for TLS-protected MQTT communication.

When the MQTT broker is set up as described above, just follow [this tutorial](http://www.steves-internet-guide.com/mosquitto-tls/) to set up the mqtt broker and all certificates. Use `tls_version tlsv1.2`.

***MQTT-SN***

Once MQTT broker is running it is necessary to set up an MQTT-SN Gateway that would establish a connection to the broker and forward the UDP packets.

Download or clone the [Eclipse paho MQTT-SN implementation](https://github.com/eclipse/paho.mqtt-sn.embedded-c/tree/master/MQTTSNGateway). Follow the guidelines in their repository.

Make sure you adjust the gateway.conf file:

```
BrokerName=localhost
```

Also it might be necessary to allow the `PredefinedTopicsList` option in case the Gateway crashes when starting up.

Finally, make sure the Gateway port is emabled for external devices:

```.sh
$ sudo ufw allow 10000/udp
```


Building test binaries
--------------------------

For testing the board and driver, test against the Mbed OS master branch to get the most recent, up-to-date test cases and drivers.

To create a build environment:

```.sh
mbed new network_test
cd network_test
cd mbed-os
git checkout master
cd ..
```

MQTT tests will run on any type of connection. See the [relevant section of netsocket tests](../netsocket/README.md#building-test-binaries) to find more information on how to establish connectivity.

Now build test binaries:

```.sh
mbed test --compile -t <toolchain> -m <target> -n mbed-os-tests-mqtt-mqtt
```

Running tests
-------------

When device is connected to network, or in case of wireless device near the access point.

```.sh
mbed test -n mbed-os-tests-mqtt-mqtt
```

Test cases for MQTT
-------------------

The API for legacy MQTT and new MQTT wrapper are identical, so the tests description apply to both of them.

Also the MQTT-SN has the same interface as MQTT, so all "MQTT_*" test also apply to MQTT-SN. 
 
The only difference is in how the Client class is created.


### MQTT_CONNECT

**Description:**

Client can connect to broker.

**Preconditions:**

Device is connected to network and connection to MQTT server is established.

**Test steps:**

1. Call `client.connect()` with an valid connectData provided. Check that no error is returned.

**Expected result:**

Call to `client.connect()` succeeds (returns NSAPI_ERROR_OK).


### MQTT_CONNECT_INVALID

**Description:**

Client cannot connect to broker with an empty clientID.

**Preconditions:**

Device is connected to network and a connection to MQTT server is established.

**Test steps:**

1. Call `client.connect()` with an invalid connectData provided (for example: empty clientID).

**Expected result:**

Call to `client.connect()` returns -1 error in step 1.


### MQTT_CONNECT_NOT_CONNECTED

**Description:**

Client fails to connect when network is not connected.

**Preconditions:**

Device is connected to network.

**Test steps:**

1. Try to connect to a nonexistent MQTT server address or incorrect port.
2. Call `client.connect()` with a valid connectData provided.

**Expected result:**

Connection fails in step 1. Call to `client.connect()` returns -1 error in step 2.


### MQTT_SUBSCRIBE

Client can subscribe to a topic.

**Preconditions:**

Device is connected to network, connection to MQTT broker is established (see the _CONNECT tests).

**Test steps:**

1. Call `client.subscribe()` with a valid topic and QoS0.

**Expected result:**

Call to `client.connect()` succeeds (returns NSAPI_ERROR_OK) in step 1.


### MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED

Client returns an error if subscribing to a topic when there is no connection to MQTT server.

**Preconditions:**

Device is connected to network.

**Test steps:**

1. Try to connect to a nonexistent MQTT server address or incorrect port.
2. Call `client.connect()` with a valid connectData provided.
3. Call `client.subscribe()` with a valid topic and QoS0.

**Expected result:**

Call to `client.subscribe()` returns -1 in step 3.


### MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED

Client returns an error if subscribing to a topic when there is no connection to MQTT broker.

**Preconditions:**

Device is connected to network and a connection to MQTT server is established.

**Test steps:**

1. Call `client.connect()` with an invalid connectData provided (for example: empty clientID).
2. Call `client.subscribe()` with a valid topic and QoS0.

**Expected result:**

Call to `client.subscribe()` returns -1 in step 3.


### MQTT_SUBSCRIBE_TOPIC_TOO_LONG

Client returns an error if subscribing to a topic longer than MAX_MQTT_PACKET_SIZE.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

1. Generate a valid string of length larger than MAX_MQTT_PACKET_SIZE.
2. Call `client.subscribe()` with the too long topic created in step 1 and QoS0.

**Expected result:**

Call to `client.subscribe()` returns -1 in step 2.


### MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER

Client returns an error if subscribing to a topic without providing a valid message handler.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

1. Call `client.connect()` with an invalid connectData provided (for example: empty clientID).
2. Call `client.subscribe()` with a valid topic, QoS0 and NULL provided intead of message handler function pointer.

**Expected result:**

Call to `client.subscribe()` returns -1 in step 2.


### MQTT_SUBSCRIBE_RECEIVE

Client is able to receive MQTT messages after subscribing to a predefined topic.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

The MQTT broker has a topic "mbed_public_test_topic", which is guaranteed to generate a message before the test times out.

**Test steps:**

1. Call `client.subscribe()` with topic set to "mbed_public_test_topic", QoS0 and a message handler that would prove successful message reception (for example increment a counter).
2. Wait until a message is received.
3. Verify that a message was received.
4. Unsubscribe from the "mbed_public_test_topic" topic.

**Expected result:**

A message is received from the topic in steps 2 and 3. Test does not time out. 


### MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE

Client is able to unsubscribe without subscribing to any topic.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

1. Unsubscribe from a valid topic.

**Expected result:**

Call to `client.unsubscribe()` returns NSAPI_ERROR_OK. 


### MQTT_UNSUBSCRIBE_INVALID

Client returns an error when invalid topic name is provided.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

1. Unsubscribe from a topic which is an empty string.
2. Unsubscribe from a topic which has topic of length longer than MAX_MQTT_PACKET_SIZE.

**Expected result:**

Both calls to `client.unsubscribe()` return -1.

### MQTT_PUBLISH

Client is able to publish MQTT messages to a topic.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

1. Call `client.publish()` with a valid topic and message with QoS1.
2. Call `client.publish()` with a valid topic and message with QoS2.
3. Call `client.publish()` with a valid topic and message with QoS3.

**Expected result:**

All calls return NSAPI_ERROR_OK.

### MQTT_PUBLISH_NOT_CONNECTED

Client returns an error if publishing to a topic when there is no connection to MQTT broker.

**Preconditions:**

Device is connected to network and a connection to MQTT server is established.

**Test steps:**

1. Call `client.publish()` with a valid topic and QoS0.

**Expected result:**

Call to `client.publish()` returns -1 in step 1.


### MQTT_PUBLISH_TOPIC_TOO_LONG

Client returns an error if publishing to a topic with name longer than MAX_MQTT_PACKET_SIZE.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

1. Generate a valid string of length larger than MAX_MQTT_PACKET_SIZE.
2. Call `client.subscribe()` with the too long topic created in step 1 and QoS0.

**Expected result:**

Call to `client.publish()` returns -1 in step 2.

### MQTT_CONNECT_USER_PASSWORD_INCORRECT

Client returns an error when incorrect password is provided to a password-protected broker.

**Preconditions:**

Device is connected to network and a connection to MQTT server is established.

The broker is password-protected (see set up instructions for details).

**Test steps:**

1. Call `client.connect()` with an incorrect user and password specified.

**Expected result:**

Call to `client.connect()` returns -1.

### MQTT_CONNECT_SUBSCRIBE_PUBLISH

Client is able to subscribe to a topic, publish to it and receive its own messages in a loopback.

**Preconditions:**

Device is connected to network and a connection to MQTT broker is established.

**Test steps:**

All steps should be performed for QoS 0, 1 and 2.

1. Call `client.subscribe()` with a valid topic and register a message handler to verify that messages are recieved (for example with a message counter).
2. Call `client.publish()` with a previously used topic.
3. Compare the message counter

**Expected result:**

All calls return no errors and the message is received correctly (for example - message counter increases).

### MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD

Client is able to provide user and password and subscribe to a topic, publish to it and receive its own messages in a loopback.

**Preconditions:**

Device is connected to network and a connection to MQTT server is established.

The broker is password-protected (see set up instructions for details).

**Test steps:**

All steps should be performed for QoS 0, 1 and 2.

1. Call `client.connect()` with a valid and correct user and password specified.
2. Call `client.subscribe()` with a valid topic and register a message handler to verify that messages are recieved (for example with a message counter). 
3. Call `client.publish()` with a previously used topic.
4. Compare the message counter.

**Expected result:**

All calls return no errors and the message is received correctly (for example - message counter increases).

Test case for MQTT with TLS
---------------------------

These tests apply only for TLSSocket class both for MQTT legacy API and the new mbed-os wrapper.

In fact MQTT does not provide any TLS support, it uses the underlying TLSSocket to take care of this.

The test case is however relevant in order to verify correct integration of MQTT with TLSSocket and provides a valuable example of how to set up a secure MQTT connection.

### MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH

Client is able to securely subscribe to a topic, publish to it and receive its own messages in a loopback.

Important: a secure port should be used for this test (by default 8883).

**Preconditions:**

Device is connected to network over a secured socket and a connection to MQTT broker is established.

**Test steps:**

All steps should be performed for QoS 0, 1 and 2.

1. Call `client.subscribe()` with a valid topic and register a message handler to verify that messages are recieved (for example with a message counter).
2. Call `client.publish()` with a previously used topic.
3. Compare the message counter

**Expected result:**

All calls return no errors and the message is received correctly (for example - message counter increases).


Test cases for MQTT-SN
----------------------

Most of the MQTT-SN API is identical to MQTT and should follow the tests described earlier. Exceptions:

* MQTT-SN does not provide the user/password functionality so MQTT_CONNECT_USER_PASSWORD_INCORRECT and MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD cannot be executed for MQTT-SN

The MQTT-SN-specific tests are listed below.

### MQTTSN_IS_CONNECTED

Client can tell that it is connected.

**Preconditions:**

Device is connected to network and a connection to MQTT-SN broker is established.

**Test steps:**

1. Call `client.isConnected()`

**Expected result:**

Call to `client.isConnected()` returns true.

### MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED

Client can tell that it is not connected.

**Preconditions:**

Device is connected to network and a connection to MQTT-SN server (but not broker) is established.

**Test steps:**

1. Call `client.isConnected()`

**Expected result:**

Call to `client.isConnected()` returns false.

### MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED

Client can tell that it is not connected.

**Preconditions:**

Device is connected to network.

**Test steps:**

1. Call `client.isConnected()`
2. Try to connect client to a non-existent broker.
3. Call `client.isConnected()`

**Expected result:**

Connection fails in step 2.
Call to `client.isConnected()` returns false in steps 1 and 3.

### MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH

Client is able to use a UDP Socket to subscribe to a topic, publish to it and receive its own messages in a loopback.

Important: See MQTT-SN section to set up an MQTT-SN gateway for communication. Use an appropriate port (by default 10000).

**Preconditions:**

Device is connected to network over a UDP socket and a connection to MQTT broker is established.

**Test steps:**

All steps should be performed for QoS 0, 1 and 2.

1. Call `client.subscribe()` with a valid topic and register a message handler to verify that messages are recieved (for example with a message counter).
2. Call `client.publish()` with a previously used topic.
3. Compare the message counter

**Expected result:**

All calls return no errors and the message is received correctly (for example - message counter increases).