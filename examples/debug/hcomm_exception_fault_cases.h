/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCOMM_EXCEPTION_FAULT_CASES_H
#define HCOMM_EXCEPTION_FAULT_CASES_H

#include <cstdint>

constexpr uint64_t HCOMM_URMA_TIMEOUT_TEST_LEN = 256ULL * 1024ULL * 1024ULL + 4096ULL;
constexpr uint64_t HCOMM_CQE_ERROR_TEST_LEN = 4096ULL;
constexpr uint64_t HCOMM_INVALID_BUFFER_ADDR = 0x7fff00000000ULL;

struct CommContext {
    uint64_t channelHandle;
    uint64_t localBufferAddr;
    uint64_t remoteBufferAddr;
    uint64_t len;
    uint64_t invalidBufferAddr;
    uint32_t rankId;
    uint32_t reserved;
};

#endif // HCOMM_EXCEPTION_FAULT_CASES_H
