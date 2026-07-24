/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License"). Please refer to the License for details.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file softmax.h
 * \brief Softmax C API kernel implementation.
 */

#ifndef SOFTMAX_CAPI_SOFTMAX_H
#define SOFTMAX_CAPI_SOFTMAX_H

#include <cstdint>
#include "c_api/asc_simd.h"

namespace Custom {
union DataUnion {
    constexpr __aicore__ DataUnion() : f(0.0f) {}
    constexpr __aicore__ DataUnion(uint32_t val) : i(val) {}
    float f;
    uint32_t i;
};
constexpr DataUnion fp32_min_value(0xFF7FFFFFu);
} // namespace Custom

constexpr uint32_t LOOP_COUNT = 2048;
constexpr uint32_t WIDTH = 128;
constexpr uint32_t SINGLE_VF_HEIGHT = 40;
constexpr uint32_t SINGLE_VF_DATA_LEN = SINGLE_VF_HEIGHT * WIDTH;

__simd_vf__ inline void softmax_vf(__ubuf__ float* dstUb, __ubuf__ float* srcUb, __ubuf__ float* expUb)
{
    constexpr uint16_t oneRepeatCount = asc_get_vf_len() / sizeof(float);
    uint16_t repeatTimes = (WIDTH + oneRepeatCount - 1) / oneRepeatCount;
    vector_bool maskFull = asc_create_mask_b32(PAT_ALL);

    vector_float srcReg;
    vector_float maxReg;
    vector_float expReg;
    vector_float sumReg;
    vector_float divReg;

    vector_float srcReg1;
    vector_float maxReg1;
    vector_float expReg1;
    vector_float sumReg1;
    vector_float divReg1;

    uint16_t halfHeight = SINGLE_VF_HEIGHT >> 1;

    for (uint16_t i = 0; i < halfHeight; i++) {
        asc_duplicate_scalar(maxReg, Custom::fp32_min_value.f, maskFull);
        asc_duplicate_scalar(maxReg1, Custom::fp32_min_value.f, maskFull);
        for (uint16_t j = 0; j < repeatTimes; j++) {
            asc_loadalign(srcReg, srcUb + i * WIDTH + j * oneRepeatCount);
            asc_loadalign(srcReg1, srcUb + i * WIDTH + j * oneRepeatCount + halfHeight * WIDTH);
            asc_max(maxReg, maxReg, srcReg, maskFull);
            asc_max(maxReg1, maxReg1, srcReg1, maskFull);
        }
        asc_reduce_max(maxReg, maxReg, maskFull);
        asc_reduce_max(maxReg1, maxReg1, maskFull);
        asc_duplicate(maxReg, maxReg, maskFull);
        asc_duplicate(maxReg1, maxReg1, maskFull);
        for (uint16_t j = 0; j < repeatTimes; j++) {
            asc_loadalign(srcReg, srcUb + i * WIDTH + j * oneRepeatCount);
            asc_loadalign(srcReg1, srcUb + i * WIDTH + j * oneRepeatCount + halfHeight * WIDTH);

            asc_exp_sub(expReg, srcReg, maxReg, maskFull);
            asc_exp_sub(expReg1, srcReg1, maxReg1, maskFull);
            asc_storealign(expUb + i * WIDTH + j * oneRepeatCount, expReg, maskFull);
            asc_storealign(expUb + i * WIDTH + j * oneRepeatCount + halfHeight * WIDTH, expReg1, maskFull);
        }
    }

    asc_mem_bar(VST_VLD);

    for (uint16_t i = 0; i < halfHeight; i++) {
        asc_duplicate_scalar(sumReg, 0.0f, maskFull);
        asc_duplicate_scalar(sumReg1, 0.0f, maskFull);
        for (uint16_t j = 0; j < repeatTimes; j++) {
            asc_loadalign(srcReg, expUb + i * WIDTH + j * oneRepeatCount);
            asc_loadalign(srcReg1, expUb + i * WIDTH + j * oneRepeatCount + halfHeight * WIDTH);
            asc_add(sumReg, sumReg, srcReg, maskFull);
            asc_add(sumReg1, sumReg1, srcReg1, maskFull);
        }
        asc_reduce_sum(sumReg, sumReg, maskFull);
        asc_reduce_sum(sumReg1, sumReg1, maskFull);
        asc_duplicate(sumReg, sumReg, maskFull);
        asc_duplicate(sumReg1, sumReg1, maskFull);
        for (uint16_t j = 0; j < repeatTimes; j++) {
            asc_loadalign(maxReg, expUb + i * WIDTH + j * oneRepeatCount);
            asc_loadalign(maxReg1, expUb + i * WIDTH + j * oneRepeatCount + halfHeight * WIDTH);
            asc_div(divReg, maxReg, sumReg, maskFull);
            asc_div(divReg1, maxReg1, sumReg1, maskFull);
            asc_storealign(dstUb + i * WIDTH + j * oneRepeatCount, divReg, maskFull);
            asc_storealign(dstUb + i * WIDTH + j * oneRepeatCount + halfHeight * WIDTH, divReg1, maskFull);
        }
    }
}

__global__ __vector__ void softmax_custom(__gm__ uint8_t* x, __gm__ uint8_t* y)
{
    asc_init();

    __ubuf__ float src0Ub[SINGLE_VF_DATA_LEN];
    __ubuf__ float dst0Ub[SINGLE_VF_DATA_LEN];
    __ubuf__ float exp0Ub[SINGLE_VF_DATA_LEN];

    __ubuf__ float src1Ub[SINGLE_VF_DATA_LEN];
    __ubuf__ float dst1Ub[SINGLE_VF_DATA_LEN];
    __ubuf__ float exp1Ub[SINGLE_VF_DATA_LEN];

    uint8_t mutexIdSrc0 = 1;
    uint8_t mutexIdDst0 = 2;
    uint8_t mutexIdSrc1 = 3;
    uint8_t mutexIdDst1 = 4;

    __gm__ float* xGm = reinterpret_cast<__gm__ float*>(x);
    __gm__ float* yGm = reinterpret_cast<__gm__ float*>(y);

    for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        __ubuf__ float* srcUb;
        __ubuf__ float* dstUb;
        __ubuf__ float* expUb;
        uint8_t mutexIdSrc;
        uint8_t mutexIdDst;

        if (i % 2 == 0) {
            srcUb = src0Ub;
            dstUb = dst0Ub;
            expUb = exp0Ub;
            mutexIdSrc = mutexIdSrc0;
            mutexIdDst = mutexIdDst0;
        } else {
            srcUb = src1Ub;
            dstUb = dst1Ub;
            expUb = exp1Ub;
            mutexIdSrc = mutexIdSrc1;
            mutexIdDst = mutexIdDst1;
        }

        asc_lock(PIPE_MTE2, mutexIdSrc);
        asc_copy_gm2ub_align(srcUb, &xGm[i * SINGLE_VF_DATA_LEN], 1, SINGLE_VF_DATA_LEN * sizeof(float),
            0, 0, true, 4, 0, 0);
        asc_unlock(PIPE_MTE2, mutexIdSrc);

        asc_lock(PIPE_V, mutexIdSrc);
        asc_lock(PIPE_V, mutexIdDst);
        softmax_vf(dstUb, srcUb, expUb);
        asc_unlock(PIPE_V, mutexIdSrc);
        asc_unlock(PIPE_V, mutexIdDst);

        asc_lock(PIPE_MTE3, mutexIdDst);
        asc_copy_ub2gm_align(&yGm[i * SINGLE_VF_DATA_LEN], dstUb, 1, SINGLE_VF_DATA_LEN * sizeof(float), 4, 0, 0);
        asc_unlock(PIPE_MTE3, mutexIdDst);
    }
}

#endif // SOFTMAX_CAPI_SOFTMAX_H
