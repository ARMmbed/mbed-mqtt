### MQTT protocol

MQTT is a machine-to-machine (M2M)/"Internet of Things" connectivity protocol. It was designed as an extremely lightweight publish/subscribe messaging transport. For more details see [http://mqtt.org/](http://mqtt.org/).

By design MQTT operates over TCP protocol, so an MQTTSN (For MQTT for Sensor Networks) was designed, which uses less resources and can operate over connectionless procols (such as UDP), but it requires a special gateway.

Mbed-os only allows you to create an MQTT client device, an external broker is needed for MQTT to operate. Clients can subscribe or publish to a topic, after connecting to a broker.

### API and usage

Mbed-os uses [Eclipse paho project emmbedded c implementation of MQTT protocol](https://github.com/eclipse/paho.mqtt.embedded-c) and [MQTT-SN protocol](https://github.com/eclipse/paho.mqtt-sn.embedded-c/).

Mbed-os supports the original paho library interface and provides its own API, which wraps around the paho library.

#### New API ####

The new API expects to receive a pointer to a configured and connected socket. This socket will be used for further communication.

For normal, non-secure operation of the MQTT a `TCPSocket` has to be provided.

For secure communication over TCP a `TLSSocket` has to be provided.

For MQTT-SN communication a `UDPSocket` has to be provided.

Secure MQTT-SN communication (over a `DTLSSocket`) is not yet supported.

After the socket is created, an instance of class `MQTTClient` can be created with the pointer to the socket as an argument. The class will distinguish between MQTT and MQTT-SN operation based on the socket's type.

Example code:

```cpp
#include <MQTTClientMbedOs.h>

NetworkInterface *net = NetworkInterface::get_default_instance();
SocketAddress sockAddr(hostname, port);
TCPSocket socket;
MQTTClient client(&socket);
socket.open(net);
socket.connect(sockAddr);
```

The socket has to be opened and connected in order for the client to be able to interact with the broker.

#### Legacy API ####

The original paho library can be used, with mbed-os providing our own `Network` template arguments (see below).

To communicate over MQTT an instance of a template class `MQTT::Client<Network, Timer>` has to be created. mbed-os provides two Network specializations:

* `MQTTNetwork` - to communicate over mbed-os's `TCPSocket`

* `MQTTNetworkTLS` - to communicate over mbed-os's `TLSSocket`

Paho's default `Timer` implementation, called `Countdown` and available from `MQTTmbed.h` can be used.

The `MQTT::Client` constructor takes one argument which is a pointer to a successfully connected NetworkInterface.

Below is an example of how to create an instance of an MQTTClient:

```cpp
#include <MQTTNetwork.h>
#include <MQTTClient.h>
#include <MQTTmbed.h> // Countdown

NetworkInterface *net = NetworkInterface::get_default_instance();
MQTTNetwork mqttNet(net);
MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);
mqttNet.connect(mqtt_global::hostname, mqtt_global::port);
```

The MQTTNetwork has to `connect` to a broker, given its address and port number.



Apart from lanaguage details and different creation and initialization procedure, the APIs of both the legacy and new API are the same.

Once connected the MQTT/MQTT-SN client is able to:

* `connect` to the broker, based on a filled `MQTTPacket_connectData` structure,
* `diconnect` from the broker,
* `subscribe` to a topic and register a callback function to be called whenever a new message arrives,
* `unsubscribe` from a topic,
* `publish` messages defined with `MQTT::Message` structure to a topic,
* if no operation is required, but an MQTT connection should be kept open, the client can `yield`,
* `MQTTNetworkTLS` requires a certificate to be set using `set_root_cert()` before calling connect.