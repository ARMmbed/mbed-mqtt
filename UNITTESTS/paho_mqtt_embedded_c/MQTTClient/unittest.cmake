
####################
# UNIT TESTS
####################

set(unittest-sources
  ../paho_mqtt_embedded_c/MQTTClient/src/MQTTClient.h
  ../paho_mqtt_embedded_c/MQTTPacket/src/MQTTDeserializePublish.c
  ../paho_mqtt_embedded_c/MQTTPacket/src/MQTTPacket.c
)

set(unittest-test-sources
  paho_mqtt_embedded_c/MQTTClient/test_MQTTClient.cpp
  mocks/Network_mock.h
  stubs/MQTTConnectClient_stub.cpp
  stubs/MQTTSerializePublish_stub.cpp
)
