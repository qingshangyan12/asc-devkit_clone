/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_MATH_C_IMPL_H
#define ASCENDC_MODULE_SIMT_MATH_C_IMPL_H

__simt_callee__ inline long int lroundf(float x)
{
    float tmp = bisheng::cce::simt::roundf(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::A, RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__simt_callee__ inline long long int llroundf(float x)
{
    float tmp = bisheng::cce::simt::roundf(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::A, RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__simt_callee__ inline long int lrintf(float x)
{
    float tmp = bisheng::cce::simt::rintf(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__simt_callee__ inline long long int llrintf(float x)
{
    float tmp = bisheng::cce::simt::rintf(x);
    return bisheng::cce::simt::__cvt_int64_t<ROUND::R, RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__simt_callee__ inline float truncf(float x)
{
    if (x > 0.0f) {
        return bisheng::cce::simt::floorf(x);
    } else {
        return bisheng::cce::simt::ceilf(x);
    }
}

__simt_callee__ inline float roundf(float x)
{
    return bisheng::cce::simt::roundf(x);
}

__simt_callee__ inline float rintf(float x)
{
    return bisheng::cce::simt::rintf(x);
}

__simt_callee__ inline float floorf(float x)
{
    return bisheng::cce::simt::floorf(x);
}

__simt_callee__ inline float ceilf(float x)
{
    return bisheng::cce::simt::ceilf(x);
}

__simt_callee__ inline float fabsf(float x)
{
    if (x < 0.0f) {
        return -x;
    } else {
        return x;
    }
}

__simt_callee__ inline float fmaf(float x, float y, float z)
{
    return bisheng::cce::simt::__fma(x, y, z);
}

__simt_callee__ inline float expf(float x)
{
    return __expf(x);
}

__simt_callee__ inline float logf(float x)
{
    if (x > 0.0f && x < 1.17549435e-38f) { // 1.17549435e-38f: subnormal floating-point number boundary
        return __logf(expf(23.0f) * x) - 23.0f;
    }
    return __logf(x);
}

__simt_callee__ inline float log2f(float x)
{
    return logf(x) / logf(2.0f);
}

__simt_callee__ inline float sqrtf(float x)
{
    return __sqrtf(x);
}

__simt_callee__ inline float rsqrtf(float x)
{
    return 1.0f / sqrtf(x);
}

__simt_callee__ inline bool is_inf(float x)
{
    uint32_t* intX = (uint32_t*)&x;
    return (*intX == 0x7F800000) || (*intX == 0xFF800000);
}

/**
 * Calculate the inverse of the standard normal cumulative distribution function.
 * @param x a value
 * @return Normcdfinv(x)
 * Special cases:
 *      if x is 0, return -inf;
 *      if x is Nan, return Nan;
 *      if x is Inf, return Nan;
 *      if x is -Inf, return -Nan;
 *      if x>1 or x < 0, return NaN.
 */
__simt_callee__ inline float normcdfinvf(float x)
{
    float double_x = x + x;
    float item = 2.0f - double_x;
    float result = 0.0f;
    if (double_x >= 0.0034f && double_x <= 1.9966f) {
        float item1 = item * double_x;
        item1 = log2f(item1);
        float w = -item1;
        float poly = fmaf(-2.51727084e-10f, w, 9.42742862e-09f);
        poly = fmaf(poly, w, -1.20547526e-07f);
        poly = fmaf(poly, w, 2.16970051e-07f);
        poly = fmaf(poly, w, 8.06214848e-06f);
        poly = fmaf(poly, w, -3.16754922e-05f);
        poly = fmaf(poly, w, -0.000774363114f);
        poly = fmaf(poly, w, 0.00554658799f);
        poly = fmaf(poly, w, 0.160820231f);
        poly = fmaf(poly, w, 0.886226892f);
        result = fmaf(poly, -double_x, poly);
    } else {
        if (double_x <= 1) {
            item = double_x;
        }
        float item1 = log2f(item);
        float w = rsqrtf(-item1);
        float poly = fmaf(-63.113224f, w, 127.484688f);
        poly = fmaf(poly, w, -114.105682f);
        poly = fmaf(poly, w, 60.3257866f);
        poly = fmaf(poly, w, -21.7898922f);
        poly = fmaf(poly, w, 6.46740913f);
        poly = fmaf(poly, w, -1.83294737f);
        poly = fmaf(poly, w, -0.0303277746f);
        poly = fmaf(poly, w, 0.832877457f);
        float recp_w = 1.0f / w;
        if (double_x > 1) {
            recp_w = -recp_w;
        }
        result = poly * recp_w;
    }
    result = fmaf(result, -1.41421354f, 0.0f); // -1.41421354f : -sqrt(2.0f)
    return result;
}

/**
 * Break down the input argument into fractional and integral parts.
 * @param x a value
 * @return Modf(x)
 * Special cases:
 *      if x is 0, return 0;
 *      if x is Nan, return Nan;
 *      if x is Inf, return 0;
 *      if x is -Inf, return 0;
 */
__simt_callee__ inline float modff(float x, float &n)
{
    float absX = fabsf(x);
    float result;
    union Data {
        float f;
        unsigned int i;
    };
    if (absX < 0x7F800000) {
        float integral_x = truncf(x);
        n = integral_x;
        float decimal = x - integral_x;
        union Data data{.f = decimal};
        uint32_t decimal_u32 = data.i;
        union Data data_x{.f = x};
        uint32_t u32 = data_x.i;
        uint32_t y_bits = (u32 & 0x80000000) | decimal_u32;
        union Data dataY{.i = y_bits};
        result = dataY.f;
    } else {
        if (is_inf(absX)) {
            union Data data {.f = x};
            uint32_t u32 = data.i;
            uint32_t y_bits = u32 & 0x80000000;
            union Data data_y {.i = y_bits};
            result = data_y.f;
            n = x;
        } else {
            result = x + x;
            n = x;
        }
    }
    return result;
}

#endif  // ASCENDC_MODULE_SIMT_MATH_C_IMPL_H