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

int MQTTSerialize_publishLength(int qos, MQTTString topicName, int payloadlen)
{
	return payloadlen;
}

int MQTTSerialize_publish(unsigned char* buf, int buflen, unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
		MQTTString topicName, unsigned char* payload, int payloadlen)
{
	return payloadlen;
}

int MQTTSerialize_ack(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid)
{
	return buflen;
}

int MQTTSerialize_puback(unsigned char* buf, int buflen, unsigned short packetid)
{
	return MQTTSerialize_ack(buf, buflen, PUBACK, 0, packetid);
}

int MQTTSerialize_pubrel(unsigned char* buf, int buflen, unsigned char dup, unsigned short packetid)
{
	return MQTTSerialize_ack(buf, buflen, PUBREL, dup, packetid);
}

int MQTTSerialize_pubcomp(unsigned char* buf, int buflen, unsigned short packetid)
{
	return MQTTSerialize_ack(buf, buflen, PUBCOMP, 0, packetid);
}


