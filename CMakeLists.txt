# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mqtt INTERFACE)

target_include_directories(mbed-mqtt
    INTERFACE
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/
        ./paho_mqtt-sn_embedded_c/MQTTSNClient/src/
        ./paho_mqtt_embedded_c/MQTTPacket/src/
        ./paho_mqtt_embedded_c/MQTTClient/src/
        ./paho_mqtt_embedded_c/MQTTClient/src/mbed/
        ./src/
)

target_sources(mbed-mqtt
    INTERFACE
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNSerializePublish.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNSubscribeServer.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNUnsubscribeServer.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNPacket.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNConnectServer.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNSearchClient.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNUnsubscribeClient.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNSubscribeClient.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNSearchServer.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNConnectClient.c
        ./paho_mqtt-sn_embedded_c/MQTTSNPacket/src/MQTTSNDeserializePublish.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTSerializePublish.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTConnectServer.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTConnectClient.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTDeserializePublish.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTUnsubscribeServer.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTPacket.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTUnsubscribeClient.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTSubscribeClient.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTSubscribeServer.c
        ./paho_mqtt_embedded_c/MQTTPacket/src/MQTTFormat.c
        ./src/MQTTClientMbedOs.cpp
)
