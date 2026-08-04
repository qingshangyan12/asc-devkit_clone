/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SIMD_SIMT_UB_USAGE_LIMIT_COMMON_H
#define SIMD_SIMT_UB_USAGE_LIMIT_COMMON_H

#include <cstdint>
#include <iostream>
#include <vector>
#include "acl/acl.h"
#include "kernel_operator.h"
#include "simt_api/asc_simt.h"
#include "c_api/asc_simd.h"

constexpr uint32_t THREAD_COUNT = 1024;
constexpr uint32_t UB_TOTAL_BYTES = 256 * 1024;
constexpr uint32_t RESERVED_UB_BYTES = 8 * 1024;
constexpr uint32_t MIN_SIMT_DATA_CACHE_BYTES = 32 * 1024;
constexpr uint32_t USER_UB_BYTES_WITH_SIMT = UB_TOTAL_BYTES - RESERVED_UB_BYTES - MIN_SIMT_DATA_CACHE_BYTES;

constexpr uint32_t RESULT_WORDS = 8;
constexpr uint32_t RESULT_BYTES = RESULT_WORDS * sizeof(uint32_t);

constexpr uint32_t BUFFER_WORDS = THREAD_COUNT;
constexpr uint32_t BUFFER_BYTES = BUFFER_WORDS * sizeof(uint32_t);
constexpr uint32_t CORRUPTED_VALUE = 0xDEADBEEF;

int32_t run_selected_case();

#endif
