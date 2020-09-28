/*
 * Copyright (c) 2020, ARM Limited, All Rights Reserved
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

#ifndef _MQTTNETWORK_UTIL_H_
#define _MQTTNETWORK_UTIL_H_

/* MQTT doesn't expect nsapi error values so we translate them */
static int convert_nsapi_error_to_mqtt_error(int nsapi_error)
{
    if (nsapi_error == NSAPI_ERROR_WOULD_BLOCK) {
        /* MQTT expects 0 on timeout */
        return 0;
    } else if (nsapi_error == 0) {
        /* MQTT expect -1 on closed sockets */
        return -1;
    }
    return nsapi_error;
}

/** Reads data and returns number of bytes read or a translated error that MQTT expects. This will call
 * read on the socket multiple times until all data is retrieved.
 *
 * @tparam SocketType Socket type like TCPSocket.
 * @param socket Socket to read data from.
 * @param buffer Buffer to store data.
 * @param len Length of expected data.
 * @param timeout Timeout for the operation.
 * @return Always returns the full length if successful or an error.
 */
template<typename SocketType>
static int accumulate_mqtt_read(SocketType socket, unsigned char *buffer, int len, int timeout)
{
    /* TODO: Timout should be applied to whole operation not partial recv */
    socket->set_timeout(timeout);

    /* MQTT Client expects the full packet so we accumulate until we get all bytes */
    int remaining = len;
    while (remaining) {
        int ret = socket->recv(buffer, remaining);
        if (ret > 0) {
            remaining -= ret;
            buffer += ret;
        } else {
            return convert_nsapi_error_to_mqtt_error(ret);
        }
    }
    return len;
}

/** Sends data and returns number of bytes sent or a translated error that MQTT expects.
 *
 * @tparam SocketType Socket type like TCPSocket.
 * @param socket Socket to send data on.
 * @param buffer Data to send.
 * @param len Length of data.
 * @param timeout Timeout for the operation.
 * @return Always returns the full length if successful or an error.
 */
template<typename SocketType>
static int mqtt_write(SocketType socket, unsigned char *buffer, int len, int timeout)
{
    socket->set_timeout(timeout);
    int ret = socket->send(buffer, len);
    if (ret > 0) {
        return ret;
    } else {
        return convert_nsapi_error_to_mqtt_error(ret);
    }
}

#endif // _MQTTNETWORK_UTIL_H_
