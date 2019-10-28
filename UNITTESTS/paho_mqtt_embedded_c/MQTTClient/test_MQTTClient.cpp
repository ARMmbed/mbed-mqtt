/*
 * Copyright (c) 2019, Arm Limited and affiliates
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../mocks/Network_mock.h"
#include "../stubs/Countdown_stub.h"
#include "gtest/gtest.h"
#include "MQTTClient.h"

using ::testing::SetArrayArgument;

class TestMQTTClient : public testing::Test {
public:
protected:
    MQTT::Client<Network_mock, Countdown_stub, 100>* client;
    Network_mock* net;

    virtual void SetUp()
    {
        net = new Network_mock;
        client = new MQTT::Client<Network_mock, Countdown_stub, 100>(*net);
    }

    virtual void TearDown()
    {
        delete client;
        delete net;
    }
};

static void messageHandler(MQTT::MessageData& data) {}

TEST_F(TestMQTTClient, incorrect_topic_length)
{
    ::testing::InSequence s;

    // Install a message handler so that isTopicMatched() can get called.
    client->setMessageHandler("*", &messageHandler);

    unsigned char bytes[10] = {
            0x30, // Packet type (PUBLISH) + flags
            0x03, // Remaining length byte
            0x02, 0x03, // Topic length (h'0x0203 = d'515)
            0x04, 0x05, 0x06 // Topic (much too short, for the length)
            };

    // Packet type (PUBLISH) + flags
    EXPECT_CALL(*net, read(_, _, _))
        .Times(1)
         .WillOnce(DoAll(SetArrayArgument<0>(bytes, bytes+1), Return(1)));

    // Remaining length byte
    EXPECT_CALL(*net, read(_, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<0>(bytes+1, bytes+2), Return(1)));

    // Remaining length byte
    EXPECT_CALL(*net, read(_, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<0>(bytes+2, bytes+5), Return(3)));

    EXPECT_EQ(-1, client->yield(5));
}
