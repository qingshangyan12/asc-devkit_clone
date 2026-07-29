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
 * \file saturation_strategy_perf.h
 * \brief Kernel implementations for saturation strategy functional effect verification
 *
 * Verifies that asc_set_saturation_strategy (CTRL[60]) switches the source of
 * saturation control between API-level (_sat suffix) and global (CTRL[59] flag).
 *
 * Uses int32→int16 conversion (CAST mode, CTRL[59]) as the test vehicle.
 * Input: 50000 (int32), exceeds int16 MAX (32767) → overflow.
 *
 * 4 cases (cross-product of strategy × API variant):
 *   Case 1: USE_API   + non-_sat API → truncation  (-15536)
 *   Case 2: USE_API   + _sat API     → saturation  (32767)
 *   Case 3: USE_GLOBAL + flag=true   → saturation  (32767), flag overrides non-_sat API
 *   Case 4: USE_GLOBAL + flag=false  → truncation  (-15536), flag overrides _sat API
 *
 * Output layout: int32→int16 conversion uses PART_EVEN + MODE_ZEROING,
 * valid values at even indices, zeros at odd indices.
 * Output element count = 2 × input count per case.
 */

#ifndef SATURATION_STRATEGY_PERF_H
#define SATURATION_STRATEGY_PERF_H

#include "c_api/asc_simd.h"

constexpr uint32_t WORK_SET_SIZE = 64;
constexpr uint32_t PACKED_OUTPUT_SIZE = WORK_SET_SIZE * 2;
constexpr uint32_t TOTAL_OUTPUT_SIZE = PACKED_OUTPUT_SIZE * 4;

/* !
 * \brief int32→int16 conversion (non-_sat, respects CTRL[59] only when USE_GLOBAL)
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
 * \brief int32→int16 saturated conversion (_sat, respects _sat suffix only when USE_API)
 */
__simd_vf__ inline void cast_vf_int322int16_sat(
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
        asc_int322int16_sat(rd, rs, vmask);
        asc_store(dst + i * oneRep * 2, rd);
    }
}

/* !
 * \brief Run all 4 cases, output packed int16 results to GM
 *
 * GM layout (int16 elements):
 *   [0 .. 127]     : Case 1 result (USE_API + non-_sat → truncation)
 *   [128 .. 255]   : Case 2 result (USE_API + _sat → saturation)
 *   [256 .. 383]   : Case 3 result (USE_GLOBAL + flag=true → saturation)
 *   [384 .. 511]   : Case 4 result (USE_GLOBAL + flag=false → truncation)
 */
__aicore__ inline void ProcessStrategyTest(__gm__ uint8_t* gmInput, __gm__ uint8_t* gmOutput)
{
    constexpr uint32_t inputSize = WORK_SET_SIZE * sizeof(int32_t);
    constexpr uint32_t caseOutputSize = PACKED_OUTPUT_SIZE * sizeof(int16_t);

    __ubuf__ int32_t xUB[WORK_SET_SIZE];
    __ubuf__ int16_t resUB[PACKED_OUTPUT_SIZE];

    asc_copy_gm2ub_sync(xUB, gmInput, inputSize);

    __gm__ int16_t* gmOut = reinterpret_cast<__gm__ int16_t*>(gmOutput);

    // Case 1: USE_API + non-_sat API → truncation (-15536)
    asc_set_saturation_strategy(asc_override_strategy::USE_API);
    asc_set_saturation_flag(asc_saturation_mode::CAST, true);
    cast_vf_int322int16(resUB, xUB, WORK_SET_SIZE);
    asc_copy_ub2gm_sync(gmOut, resUB, caseOutputSize);

    // Case 2: USE_API + _sat API → saturation (32767)
    asc_set_saturation_strategy(asc_override_strategy::USE_API);
    asc_set_saturation_flag(asc_saturation_mode::CAST, false);
    cast_vf_int322int16_sat(resUB, xUB, WORK_SET_SIZE);
    asc_copy_ub2gm_sync(gmOut + PACKED_OUTPUT_SIZE, resUB, caseOutputSize);

    // Case 3: USE_GLOBAL + flag=true → saturation (32767), flag overrides non-_sat
    asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
    asc_set_saturation_flag(asc_saturation_mode::CAST, true);
    cast_vf_int322int16(resUB, xUB, WORK_SET_SIZE);
    asc_copy_ub2gm_sync(gmOut + PACKED_OUTPUT_SIZE * 2, resUB, caseOutputSize);

    // Case 4: USE_GLOBAL + flag=false → truncation (-15536), flag overrides _sat
    asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
    asc_set_saturation_flag(asc_saturation_mode::CAST, false);
    cast_vf_int322int16_sat(resUB, xUB, WORK_SET_SIZE);
    asc_copy_ub2gm_sync(gmOut + PACKED_OUTPUT_SIZE * 3, resUB, caseOutputSize);

    // Restore default strategy
    asc_set_saturation_strategy(asc_override_strategy::USE_API);
}

#endif // SATURATION_STRATEGY_PERF_H
