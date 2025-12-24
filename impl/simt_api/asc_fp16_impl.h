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
 * \file asc_fp16_impl.h
 * \brief
 */
#ifndef ASCENDC_MODULE_SIMT_C_ASC_FP16_IMPL_H
#define ASCENDC_MODULE_SIMT_C_ASC_FP16_IMPL_H

__simt_callee__ inline half __float2half(const float x) {
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __float2half_rn(const float x) {
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __float2half_rz(const float x) {
    return bisheng::cce::simt::__cvt_half<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __float2half_rd(const float x) {
    return bisheng::cce::simt::__cvt_half<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __float2half_ru(const float x) {
    return bisheng::cce::simt::__cvt_half<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __half2float(const half x) {
    union Data {
        half h;
        unsigned int i;
    };
    union Data d = {.h = x};

    unsigned int sign = ((d.i >> 15U) & 1U);
    unsigned int exponent = ((d.i >> 10U) & 0x1fU);
    if (exponent == 0) {
        return x;
    }
    unsigned int mantissa = ((d.i & 0x3ffU) << 13U);

    if (exponent == 0x1fU) {
        sign = ((mantissa != 0U) ? 0U : sign);
        mantissa = ((mantissa != 0U) ? 0x7fffffU : 0U);
        exponent = 0xffU;
    } else if (exponent == 0U) {
        if (mantissa != 0U) {
            unsigned int msb;
            exponent = 0x71U;
            do {
                msb = (mantissa & 0x400000U);
                mantissa <<= 1U;
                --exponent;
            } while (msb != 0U);
            mantissa &= 0x7fffffU;
        }
    } else {
        exponent += 0x70U;
    }

    unsigned int u = ((sign << 31) | (exponent << 23U) | mantissa);
    union Data1 {
        float f;
        unsigned int i;
    };
    union Data1 d1{.i = u};
    return d1.f;
}

__simt_callee__ inline unsigned int __half2uint_rn(const half x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned int __half2uint_rz(const half x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned int __half2uint_rd(const half x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned int __half2uint_ru(const half x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __half2int_rn(const half x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __half2int_rz(const half x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __half2int_rd(const half x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __half2int_ru(const half x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned long long int __half2ull_rn(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline unsigned long long int __half2ull_rz(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline unsigned long long int __half2ull_rd(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline unsigned long long int __half2ull_ru(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline long long int __half2ll_rn(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_int64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline long long int __half2ll_rz(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_int64_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline long long int __half2ll_rd(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_int64_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline long long int __half2ll_ru(const half x) {
    float x_fp32 = x;
    return bisheng::cce::simt::__cvt_int64_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x_fp32);
}

__simt_callee__ inline half __uint2half_rn(const unsigned int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __uint2half_rz(const unsigned int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __uint2half_rd(const unsigned int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __uint2half_ru(const unsigned int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __int2half_rn(const int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __int2half_rz(const int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __int2half_rd(const int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __int2half_ru(const int x) {
    return bisheng::cce::simt::__cvt_half<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline half __ull2half_rn(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ull2half_rz(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ull2half_rd(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ull2half_ru(const unsigned long long int x) {
    uint64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ll2half_rn(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ll2half_rz(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ll2half_rd(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

__simt_callee__ inline half __ll2half_ru(const long long int x) {
    int64_t y = x;
    float f = bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(y);
    return bisheng::cce::simt::__cvt_half<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(f);
}

#endif  // ASCENDC_MODULE_SIMT_C_ASC_FP16_IMPL_H
