/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file conv2d_util.h
 * \brief
 */

#ifndef API_CONV2D_UTIL_H
#define API_CONV2D_UTIL_H

#include "../common/conv_util.h"

namespace Conv2dApi {
using namespace AscendC;
using namespace ConvApi;

#define FIX_VAL 16
#define FIXED_SHIFT_VAL 42 // 58 - 16

constexpr uint64_t DST_MEM_BLOCK_PRE_QUANT = 0;
constexpr uint64_t DST_MEM_BLOCK_PRE_RELU = 1;
constexpr uint64_t DST_MEM_BLOCK_PRE_CLIP_RELU = -1;
constexpr uint64_t DST_MEM_BLOCK_POST_QUANT = 3;
constexpr uint64_t DST_MEM_BLOCK_POST_RELU = 2;
constexpr uint64_t DST_MEM_BLOCK_POST_CLIP_RELU = -1;
constexpr uint64_t DST_MEM_BLOCK_ANTI_QUANT = 4;

const static uint64_t FB_PRE_QUANT_SIZE = 2 * 1024; // B
const static uint64_t FB_PRE_RELU_SIZE = 1 * 1024; // B
const static uint64_t FB_PRE_CLIP_RELU_SIZE = 0; // B
const static uint64_t FB_POST_QUANT_SIZE = 2 * 1024; // B
const static uint64_t FB_POST_RELU_SIZE = 1 * 1024; // B
const static uint64_t FB_POST_CLIP_RELU_SIZE = 0; // B
const static uint64_t FB_ANTI_QUANT_SIZE = 1 * 1024; // B

const static uint64_t FB_PRE_QUANT_ALIGN_SIZE = 128; // B
const static uint64_t FB_PRE_RELU_ALIGN_SIZE = 64; // B
const static uint64_t FB_PRE_CLIP_RELU_ALIGN_SIZE = 0;  // B
const static uint64_t FB_POST_QUANT_ALIGN_SIZE = 128; // B
const static uint64_t FB_POST_RELU_ALIGN_SIZE = 64; // B
const static uint64_t FB_POST_CLIP_RELU_ALIGN_SIZE = 0;  // B
const static uint64_t FB_ANTI_QUANT_ALIGN_SIZE = 64; // B

const static uint64_t BIAS_TABLE_SIZE = 1 * 1024; // B
const static uint64_t BIAS_TABLE_ALIGN_SIZE = 64; // B

// constexpr uint64_t DOUBLE_BUFFER_A_L0 = 0;
// constexpr uint64_t DOUBLE_BUFFER_B_L0 = 1;
// constexpr uint64_t DOUBLE_BUFFER_C_L0 = 2;
// constexpr uint64_t DOUBLE_BUFFER_A_L1 = 3;
// constexpr uint64_t DOUBLE_BUFFER_B_L1 = 4;

} // namespace Conv2dApi

#endif // API_CONV2D_UTIL_H
