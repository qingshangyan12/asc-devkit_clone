/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file saturation_flag_perf.h
 * \brief Kernel implementations for saturation flag functional effect verification
 *
 * 3 cases verifying that asc_set_saturation_flag controls overflow handling
 * in actual computation, observed by comparing saturation-ON vs saturation-OFF results:
 *   Case 1: FLOAT mode — half add overflow (60000 + 10000 > 65504)
 *   Case 2: INT mode   — int16 add overflow (30000 + 30000 > 32767)
 *   Case 3: CAST mode  — int32→int16 conversion overflow (50000 > 32767)
 *
 * Uses register-level APIs (__simd_vf__ helpers) because UB-level conversion
 * APIs are not available on dav-3510.
 */

#ifndef SATURATION_FLAG_PERF_H
#define SATURATION_FLAG_PERF_H

#include "c_api/asc_simd.h"

constexpr uint32_t WORK_SET_SIZE_16 = 128;
constexpr uint32_t WORK_SET_SIZE_32 = 64;

/* !
 * \brief half vector add (register-level, respects CTRL[48])
 */
__simd_vf__ inline void add_vf_half(
    __ubuf__ half* x, __ubuf__ half* y, __ubuf__ half* z, uint32_t count)
{
    uint16_t oneRep = asc_get_vf_len() / sizeof(half);
    uint16_t repTime = (count + oneRep - 1) / oneRep;

    vector_bool vmask;
    vector_half r0;
    vector_half r1;
    vector_half rd;
    for (uint16_t i = 0; i < repTime; ++i) {
        vmask = asc_update_mask_b16(count);
        asc_load(r0, x + i * oneRep);
        asc_load(r1, y + i * oneRep);
        asc_add(rd, r0, r1, vmask);
        asc_store(z + i * oneRep, rd);
    }
}

/* !
 * \brief int16 vector add (register-level, respects CTRL[53])
 */
__simd_vf__ inline void add_vf_int16(
    __ubuf__ int16_t* x, __ubuf__ int16_t* y, __ubuf__ int16_t* z, uint32_t count)
{
    uint16_t oneRep = asc_get_vf_len() / sizeof(int16_t);
    uint16_t repTime = (count + oneRep - 1) / oneRep;

    vector_bool vmask;
    vector_int16_t r0;
    vector_int16_t r1;
    vector_int16_t rd;
    for (uint16_t i = 0; i < repTime; ++i) {
        vmask = asc_update_mask_b16(count);
        asc_load(r0, x + i * oneRep);
        asc_load(r1, y + i * oneRep);
        asc_add(rd, r0, r1, vmask);
        asc_store(z + i * oneRep, rd);
    }
}

/* !
 * \brief int32→int16 conversion (register-level, non-_sat, respects CTRL[59])
 *
 * Output layout: valid int16 values at even indices, 0 at odd indices
 * (due to PART_EVEN + MODE_ZEROING in the vcvt instruction).
 * Output element count = 2 × input count.
 */
__simd_vf__ inline void cast_vf_int322int16(
    __ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    uint16_t oneRep = asc_get_vf_len() / sizeof(int32_t);
    uint16_t repTime = (count + oneRep - 1) / oneRep;

    vector_bool vmask;
    vector_int32_t rs;
    vector_int16_t rd;
    for (uint16_t i = 0; i < repTime; ++i) {
        vmask = asc_update_mask_b32(count);
        asc_load(rs, src + i * oneRep);
        asc_int322int16(rd, rs, vmask);
        asc_store(dst + i * oneRep * 2, rd);
    }
}

/* !
 * \brief Case 1: FLOAT — half add overflow
 *
 * Input: x = 60000.0(half), y = 10000.0(half) → sum = 70000.0 > 65504(half MAX)
 * Saturation ON  (CTRL[48]=0): INF → 65504.0
 * Saturation OFF (CTRL[48]=1): +INF (half 0x7C00)
 */
__aicore__ inline void ProcessFloatSaturation(__gm__ uint8_t* gmInput, __gm__ uint8_t* gmOutput)
{
    constexpr uint32_t N = WORK_SET_SIZE_16;
    constexpr uint32_t inputSize = N * sizeof(half) * 2;
    constexpr uint32_t resultSize = N * sizeof(half);

    __ubuf__ half xUB[N];
    __ubuf__ half yUB[N];
    __ubuf__ half resUB[N];

    asc_copy_gm2ub_sync(xUB, gmInput, inputSize);

    asc_set_saturation_flag(asc_saturation_mode::FLOAT, true);
    add_vf_half(xUB, yUB, resUB, N);
    asc_copy_ub2gm_sync(gmOutput, resUB, resultSize);

    asc_set_saturation_flag(asc_saturation_mode::FLOAT, false);
    add_vf_half(xUB, yUB, resUB, N);
    asc_copy_ub2gm_sync(gmOutput + resultSize, resUB, resultSize);
}

/* !
 * \brief Case 2: INT — int16 add overflow
 *
 * Input: x = 30000(int16), y = 30000(int16) → sum = 60000 > 32767(int16 MAX)
 * Saturation ON  (CTRL[53]=1): 32767
 * Saturation OFF (CTRL[53]=0): 60000 truncated to int16 = -5536
 */
__aicore__ inline void ProcessIntSaturation(__gm__ uint8_t* gmInput, __gm__ uint8_t* gmOutput)
{
    constexpr uint32_t N = WORK_SET_SIZE_16;
    constexpr uint32_t inputSize = N * sizeof(int16_t) * 2;
    constexpr uint32_t resultSize = N * sizeof(int16_t);

    __ubuf__ int16_t xUB[N];
    __ubuf__ int16_t yUB[N];
    __ubuf__ int16_t resUB[N];

    asc_copy_gm2ub_sync(xUB, gmInput, inputSize);

    asc_set_saturation_flag(asc_saturation_mode::INT, true);
    add_vf_int16(xUB, yUB, resUB, N);
    asc_copy_ub2gm_sync(gmOutput, resUB, resultSize);

    asc_set_saturation_flag(asc_saturation_mode::INT, false);
    add_vf_int16(xUB, yUB, resUB, N);
    asc_copy_ub2gm_sync(gmOutput + resultSize, resUB, resultSize);
}

/* !
 * \brief Case 3: CAST — int32→int16 conversion overflow
 *
 * Input: x = 50000(int32) → exceeds int16 MAX (32767)
 * Saturation ON  (CTRL[59]=0): 32767
 * Saturation OFF (CTRL[59]=1): 50000 truncated to int16 = -15536
 *
 * Note: CTRL[59] only takes effect when CTRL[60]=1 (USE_GLOBAL).
 * Output layout: valid at even indices, 0 at odd (due to PART_EVEN).
 */
__aicore__ inline void ProcessCastSaturation(__gm__ uint8_t* gmInput, __gm__ uint8_t* gmOutput)
{
    constexpr uint32_t N = WORK_SET_SIZE_32;
    constexpr uint32_t inputSize = N * sizeof(int32_t);
    constexpr uint32_t resultSize = N * sizeof(int16_t) * 2;

    __ubuf__ int32_t xUB[N];
    __ubuf__ int16_t resUB[N * 2];

    asc_copy_gm2ub_sync(xUB, gmInput, inputSize);

    asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);

    asc_set_saturation_flag(asc_saturation_mode::CAST, true);
    cast_vf_int322int16(resUB, xUB, N);
    asc_copy_ub2gm_sync(gmOutput, resUB, resultSize);

    asc_set_saturation_flag(asc_saturation_mode::CAST, false);
    cast_vf_int322int16(resUB, xUB, N);
    asc_copy_ub2gm_sync(gmOutput + resultSize, resUB, resultSize);

    asc_set_saturation_strategy(asc_override_strategy::USE_API);
}

#endif // SATURATION_FLAG_PERF_H
