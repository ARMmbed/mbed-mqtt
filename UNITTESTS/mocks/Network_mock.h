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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Return;

class Network_mock {
public:
    Network_mock() {

    }

    int connect(const char* hostname, int port) {
        return 0;
    }

    MOCK_METHOD3(read, int(unsigned char* buffer, int len, int timeout_ms));

    int write(unsigned char* buffer, int len, int timeout) {
        return len;
    }

    int disconnect() {
        return 0;
    }
};
