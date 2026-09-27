/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_LOG_TEST_STUB_H
#define HCCL_LOG_TEST_STUB_H

#include <cstdint>
#include <string>
#include <vector>

extern "C" void SetTestLogLevel(uint32_t level);
void ClearTestLogs();
std::vector<std::string> GetTestLogs();

#endif // HCCL_LOG_TEST_STUB_H
