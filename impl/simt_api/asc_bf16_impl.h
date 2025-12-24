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
 * \file asc_bf16_impl.h
 * \brief
 */
#ifndef ASCENDC_MODULE_SIMT_C_ASC_BF16_IMPL_H
#define ASCENDC_MODULE_SIMT_C_ASC_BF16_IMPL_H

__simt_callee__ inline bfloat16_t __float2bfloat16(const float x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __float2bfloat16_rn(const float x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __float2bfloat16_rz(const float x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __float2bfloat16_rd(const float x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __float2bfloat16_ru(const float x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __half2bfloat16_rn(const half x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __half2bfloat16_rz(const half x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __half2bfloat16_rd(const half x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __half2bfloat16_ru(const half x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __bfloat162half_rn(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __bfloat162half_rz(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_half<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __bfloat162half_rd(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_half<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __bfloat162half_ru(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_half<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __bfloat162float(const bfloat16_t x) {
    union Data {
        bfloat16_t bf;
        unsigned int i;
    };
    union Data d = {.bf = x};
    unsigned int u = d.i << 16;
    union Data2 {
        float f;
        unsigned int i;
    };
    union Data2 d2 = {.i = u};
    return d2.f;
}

__simt_callee__ inline unsigned int __bfloat162uint_rn(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline unsigned int __bfloat162uint_rz(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline unsigned int __bfloat162uint_rd(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline unsigned int __bfloat162uint_ru(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline int __bfloat162int_rn(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __bfloat162int_rz(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __bfloat162int_rd(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __bfloat162int_ru(const bfloat16_t x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned long long int __bfloat162ull_rn(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline unsigned long long int __bfloat162ull_rz(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline unsigned long long int __bfloat162ull_rd(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline unsigned long long int __bfloat162ull_ru(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline long long int __bfloat162ll_rn(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline long long int __bfloat162ll_rz(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline long long int __bfloat162ll_rd(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline long long int __bfloat162ll_ru(const bfloat16_t x) {
    float f = bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __uint2bfloat16_rn(const unsigned int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __uint2bfloat16_rz(const unsigned int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __uint2bfloat16_rd(const unsigned int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __uint2bfloat16_ru(const unsigned int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __int2bfloat16_rn(const int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __int2bfloat16_rz(const int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __int2bfloat16_rd(const int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __int2bfloat16_ru(const int x) {
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline bfloat16_t __ull2bfloat16_rn(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ull2bfloat16_rz(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ull2bfloat16_rd(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ull2bfloat16_ru(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ll2bfloat16_rn(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ll2bfloat16_rz(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ll2bfloat16_rd(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline bfloat16_t __ll2bfloat16_ru(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_bfloat16_t<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

#endif  //  ASCENDC_MODULE_SIMT_C_ASC_BF16_IMPL_H
