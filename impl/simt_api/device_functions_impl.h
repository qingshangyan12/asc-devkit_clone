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
 * \file device_functions_impl.h
 * \brief
 */
#ifndef IMPL_SIMT_API_DEVICE_FUNCTIONS_IMPL_H
#define IMPL_SIMT_API_DEVICE_FUNCTIONS_IMPL_H

__simt_callee__ inline unsigned int __float2uint_rn(const float x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned int __float2uint_rz(const float x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned int __float2uint_rd(const float x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned int __float2uint_ru(const float x) {
    return bisheng::cce::simt::__cvt_uint32_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __float2int_rn(const float x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __float2int_rz(const float x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __float2int_rd(const float x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline int __float2int_ru(const float x) {
    return bisheng::cce::simt::__cvt_int32_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned long long int __float2ull_rn(const float x) {
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned long long int __float2ull_rz(const float x) {
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned long long int __float2ull_rd(const float x) {
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline unsigned long long int __float2ull_ru(const float x) {
    return bisheng::cce::simt::__cvt_uint64_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline long long int __float2ll_rn(const float x) {
    return bisheng::cce::simt::__cvt_int64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline long long int __float2ll_rz(const float x) {
    return bisheng::cce::simt::__cvt_int64_t<ROUND::Z, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline long long int __float2ll_rd(const float x) {
    return bisheng::cce::simt::__cvt_int64_t<ROUND::F, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline long long int __float2ll_ru(const float x) {
    return bisheng::cce::simt::__cvt_int64_t<ROUND::C, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__simt_callee__ inline float __uint2float_rn(const unsigned int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __uint2float_rz(const unsigned int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __uint2float_rd(const unsigned int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __uint2float_ru(const unsigned int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __int2float_rn(const int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __int2float_rz(const int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __int2float_rd(const int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __int2float_ru(const int x) {
    return bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__simt_callee__ inline float __ull2float_rn(const unsigned long long int x) {
    uint64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ull2float_rz(const unsigned long long int x) {
    uint64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ull2float_rd(const unsigned long long int x) {
    uint64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ull2float_ru(const unsigned long long int x) {
    uint64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ll2float_rn(const long long int x) {
    int64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ll2float_rz(const long long int x) {
    int64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::Z, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ll2float_rd(const long long int x) {
    int64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::F, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __ll2float_ru(const long long int x) {
    int64_t y = x;
    return bisheng::cce::simt::__cvt_float<ROUND::C, RoundingSaturation::RS_DISABLE_VALUE>(y);
}

__simt_callee__ inline float __int_as_float(const int x) {
    union Data {
        float f;
        int i;
    };
    union Data data = {.i = x};
    return data.f;
}

__simt_callee__ inline float __uint_as_float(const unsigned int x) {
    union Data {
        float f;
        unsigned int i;
    };
    union Data data = {.i = x};
    return data.f;
}

__simt_callee__ inline int __float_as_int(const float x) {
    union Data {
        float f;
        int i;
    };
    union Data data = {.f = x};
    return data.i;
}

__simt_callee__ inline unsigned int __float_as_uint(const float x) {
    union Data {
        float f;
        unsigned int u;
    };
    union Data data = {.f = x};
    return data.u;
}

#endif  // IMPL_SIMT_API_DEVICE_FUNCTIONS_IMPL_H
