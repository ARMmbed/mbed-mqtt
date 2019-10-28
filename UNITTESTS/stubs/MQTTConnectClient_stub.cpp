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

#include "MQTTPacket.h"

int MQTTSerialize_connectLength(MQTTPacket_connectData* options)
{
    int len = 1;

    return len;
}

int MQTTSerialize_connect(unsigned char* buf, int buflen, MQTTPacket_connectData* options)
{
    return buflen;
}

int MQTTDeserialize_connack(unsigned char* sessionPresent, unsigned char* connack_rc, unsigned char* buf, int buflen)
{
    return 1;
}

int MQTTSerialize_zero(unsigned char* buf, int buflen, unsigned char packettype)
{
    return 1;
}

int MQTTSerialize_disconnect(unsigned char* buf, int buflen)
{
    return MQTTSerialize_zero(buf, buflen, DISCONNECT);
}

int MQTTSerialize_pingreq(unsigned char* buf, int buflen)
{
    return MQTTSerialize_zero(buf, buflen, PINGREQ);
}
