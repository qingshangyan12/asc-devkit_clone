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

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "../common/conv_forward_util.h"
#else
#include "impl/adv_api/detail/conv/common/conv_forward_util.h"
#endif

namespace Conv2dApi {
using namespace AscendC;
using namespace ConvApi;

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
    const static uint64_t L0A_SIZE = 0;
    const static uint64_t L0B_SIZE = 131072;
    const static uint64_t L0C_SIZE = 0;
    const static uint64_t L0A_HALF_SIZE = 0;
    const static uint64_t L0B_HALF_SIZE = 65536;
    const static uint64_t L0C_HALF_SIZE = 0;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161)
    const static uint64_t L0A_SIZE = 0;
    const static uint64_t L0B_SIZE = 65536;
    const static uint64_t L0C_SIZE = 0;
    const static uint64_t L0A_HALF_SIZE = 0;
    const static uint64_t L0B_HALF_SIZE = 32768;
    const static uint64_t L0C_HALF_SIZE = 0;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5165)
    const static uint64_t L0A_SIZE = 0;
    const static uint64_t L0B_SIZE = 16384;
    const static uint64_t L0C_SIZE = 0;
    const static uint64_t L0A_HALF_SIZE = 0;
    const static uint64_t L0B_HALF_SIZE = 8192;
    const static uint64_t L0C_HALF_SIZE = 0;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5163)
    const static uint64_t L0A_SIZE = 0;
    const static uint64_t L0B_SIZE = 65536;
    const static uint64_t L0C_SIZE = 0;
    const static uint64_t L0A_HALF_SIZE = 0;
    const static uint64_t L0B_HALF_SIZE = 32768;
    const static uint64_t L0C_HALF_SIZE = 0;
#else
    const static uint64_t L0A_SIZE = 65536;
    const static uint64_t L0B_SIZE = 65536;
    const static uint64_t L0C_SIZE = 262144;
    const static uint64_t L0A_HALF_SIZE = 32768;
    const static uint64_t L0B_HALF_SIZE = 32768;
    const static uint64_t L0C_HALF_SIZE = 131072;
#endif
const static uint64_t PADDING_ALIGN_SIZE = 32;
const static uint32_t FB_BLOCK_SIZE = 32;
const static uint8_t C04_CIN_SIZE  = 4;
const static uint8_t DOUBLE_BUF = 2;

const static uint64_t S8SIZE = 1;   // sizeof(int8_t)
const static uint64_t S16SIZE = 2;  // sizeof(int16_t)

#define FIX_VAL 16
#define FIXED_SHIFT_VAL_FP16 42 // 58 - 16
#define FIXED_SHIFT_VAL_FP8 10 // 26 - 16

const static uint64_t DST_MEM_BLOCK_PRE_QUANT = 0;
const static uint64_t DST_MEM_BLOCK_PRE_RELU = 1;
const static uint64_t DST_MEM_BLOCK_PRE_CLIP_RELU = -1;
const static uint64_t DST_MEM_BLOCK_POST_QUANT = 3;
const static uint64_t DST_MEM_BLOCK_POST_RELU = 2;
const static uint64_t DST_MEM_BLOCK_POST_CLIP_RELU = -1;
const static uint64_t DST_MEM_BLOCK_ANTI_QUANT = 4;

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

const static uint64_t PADDING_TABLE_SIZE = 1 * 256; // B
const static uint64_t PADDING_TABLE_ALIGN_SIZE = 32; // B

const static uint64_t DOUBLE_BUFFER_A_L0 = 0;
const static uint64_t DOUBLE_BUFFER_B_L0 = 1;
const static uint64_t DOUBLE_BUFFER_C_L0 = 2;
const static uint64_t DOUBLE_BUFFER_A_L1 = 3;
const static uint64_t DOUBLE_BUFFER_B_L1 = 4;

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#define ASCEND_IS_AIC_CONV constexpr(true)
#define ASCEND_IS_AIV_CONV constexpr(true)
#else
#define ASCEND_IS_AIC_CONV ASCEND_IS_AIC
#define ASCEND_IS_AIV_CONV ASCEND_IS_AIV
#endif

} // namespace Conv2dApi

#endif // API_CONV2D_UTIL_H
