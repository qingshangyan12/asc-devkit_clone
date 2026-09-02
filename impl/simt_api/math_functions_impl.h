/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file math_functions_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_FUNCTIONS_IMPL__
#warning "impl/simt_api/math_functions_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "simt_api/math_functions.h" and use public functions or variables defined in interface header files."
#endif

#ifndef IMPL_SIMT_API_MATH_FUNCTIONS_IMPL_H
#define IMPL_SIMT_API_MATH_FUNCTIONS_IMPL_H

#include "simt_api/device_types.h"
#include "simt_api/math_constants.h"
#include "impl/simt_api/device_functions_impl.h"
#include "impl/simt_api/internal_functions_impl.h"

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)

#define ASCRT_FOUR_BYTE_LEN_U 32U
constexpr float __internal_subnormal_boundary =
    1.17549435e-38f; // 1.17549435e-38f: subnormal floating-point number boundary

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long int lroundf(float x)
{
    float tmp = roundf(x);
    return __cvt_int64_t<__internal_get_round<__RoundMode::CAST_ROUND>(), RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long long int llroundf(float x)
{
    float tmp = roundf(x);
    return __cvt_int64_t<__internal_get_round<__RoundMode::CAST_ROUND>(), RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long int lrintf(float x)
{
    float tmp = rintf(x);
    return __cvt_int64_t<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long long int llrintf(float x)
{
    float tmp = rintf(x);
    return __cvt_int64_t<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_ENABLE_VALUE>(tmp);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float truncf(float x)
{
    if (x > 0.0f) {
        return __floorf(x);
    } else {
        return __ceilf(x);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float roundf(float x) { return __roundf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rintf(float x) { return __rintf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float floorf(float x) { return __floorf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float ceilf(float x) { return __ceilf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fabsf(float x) { return __fabsf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fmaf(float x, float y, float z) { return __fma(x, y, z); }

/*
 * Computes a refined sqrt approximation for float inputs.
 *
 * calculation to move subnormal or very small values into a more stable
 * range. The result is scaled back by 2^-12 because sqrt(x * 2^24)
 * equals sqrt(x) * 2^12.
 *
 * then applies one Newton-style correction to inv:
 *   err0 = 1 - b * inv * inv
 *   inv = inv + err0 * inv * 0.5
 *
 * The final sqrt value is reconstructed and corrected:
 *   y = inv * b
 *   err1 = b - y * y
 *   y = y + 0.5 * inv * err1
 *
 * @param x The input value.
 * @return The refined square root approximation.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sqrtf_precise_impl(float x)
{
    constexpr float scale_up = 16777216.0f;       // 2^24
    constexpr float scale_down = 0.000244140625f; // 2^-12
    constexpr float half = 0.5f;

    // Return early for zero and positive infinity.
    if (x == 0.0f || x == ASCRT_INF_F) {
        return x;
    }

    // Scale inputs smaller than 1 to the normal range.
    bool scaled = x < 1.0f;
    float b = scaled ? x * scale_up : x;

    // Compute an approximate reciprocal square root.
    float sqrt_b = __sqrtf(b);
    float inv = 1.0f / sqrt_b;

    // Refine the reciprocal square root.
    float err0 = fmaf(-(b * inv), inv, 1.0f);
    inv = fmaf(err0, inv * half, inv);

    // Convert to a square-root approximation, then compensate the sqrt residual.
    float y = inv * b;
    float err1 = fmaf(-y, y, b);
    y = fmaf(inv * half, err1, y);

    // Scale the result back if the input was multiplied by 2^24.
    return scaled ? y * scale_down : y;
}

#ifdef __ASC_FTZ__
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_expf(float x) { return __expf(x); }

#ifdef __ASC_PREC_SQRT__
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sqrtf(float x)
{
    // Treat positive subnormal inputs as 0 before entering the precise sqrt path.
    if (x > 0.0f && x < __internal_subnormal_boundary) {
        x = 0.0f;
    }
    return __internal_sqrtf_precise_impl(x);
}
#else
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sqrtf(float x) { return __sqrtf(x); }
#endif
#endif

#ifndef __ASC_FTZ__
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_expf(float x)
{
    constexpr float __internal_subnormal_input_boundary =
        -87.3365478515625f; // Input range where expf returns a subnormal result
    if (x <= __internal_subnormal_input_boundary) {
        float half_y = __expf(x * 0.5f);
        return half_y * half_y;
    }
    return __expf(x);
}

#ifdef __ASC_PREC_SQRT__
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sqrtf(float x) { return __internal_sqrtf_precise_impl(x); }
#else
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sqrtf(float x)
{
    constexpr float scale_up = 16777216.0f;       // 2^24
    constexpr float scale_down = 0.000244140625f; // 2^-12
    // Scale positive subnormal inputs to the normal range, then scale the result back.
    if (x < __internal_subnormal_boundary) {
        return __sqrtf(x * scale_up) * scale_down;
    }
    return __sqrtf(x);
}
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float expf(float x) { return __internal_expf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sqrtf(float x) { return __internal_sqrtf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float logf(float x)
{
    if (x > 0.0f && x < 1.17549435e-38f) { // 1.17549435e-38f: subnormal floating-point number boundary
        return __logf(expf(23.0f) * x) - 23.0f;
    }
    return __logf(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_log2f_positive_finite(float x, float& log2_hi, float& log2_lo)
{
    constexpr float fp32_min_normal = 1.175494350822287508e-38f;
    constexpr float subnormal_scale = 16777216.0f;
    constexpr float subnormal_exponent_fix = -24.0f;
    constexpr float log_exponent_scale = 1.1920928955078125e-07f;
    constexpr uint32_t log_reduction_mask = 0xFF800000U;
    constexpr uint32_t sqrt_half_bits = 0x3F3504F3U;
    constexpr float log2e_hi = 1.4426950216293334961f;
    constexpr float log2e_lo = 1.9251366722983220825e-08f;

    const bool is_normal_x = x >= fp32_min_normal;
    const float log_input = is_normal_x ? x : x * subnormal_scale;
    const float exponent_base = is_normal_x ? 0.0f : subnormal_exponent_fix;
    const uint32_t log_input_bits = __float_as_uint(log_input);

    const uint32_t reduction_bits = (log_input_bits - sqrt_half_bits) & log_reduction_mask;
    const float mantissa = __uint_as_float(log_input_bits - reduction_bits);
    const float exponent_part =
        fmaf(__int2float_rn(static_cast<int32_t>(reduction_bits)), log_exponent_scale, exponent_base);

    const float mantissa_minus_one = mantissa - 1.0f;
    const float reciprocal = 1.0f / (mantissa + 1.0f);
    const float reduced_hi = reciprocal * (mantissa_minus_one + mantissa_minus_one);
    const float reduced_square = reduced_hi * reduced_hi;

    float log_poly = fmaf(reduced_square, 0.0006568862590938807f, 0.0032181653659790754318f);
    log_poly = fmaf(reduced_square, log_poly, 0.018033718690276145935f);
    log_poly = fmaf(reduced_square, log_poly, 0.12022458761930465698f);
    log_poly = reduced_square * log_poly;

    log2_hi = fmaf(reduced_hi, log2e_hi, exponent_part);
    float reduced_err = mantissa_minus_one - reduced_hi;
    reduced_err = fmaf(mantissa_minus_one, -reduced_hi, reduced_err + reduced_err);
    const float reduced_lo = reciprocal * reduced_err;

    log2_lo = exponent_part - log2_hi;
    log2_lo = fmaf(reduced_hi, log2e_hi, log2_lo);
    log2_lo = fmaf(reduced_lo, log2e_hi, log2_lo);
    log2_lo = fmaf(reduced_hi, log2e_lo, log2_lo);
    log2_lo = fmaf(reduced_lo, log_poly * 3.0f, log2_lo);
    log2_lo = fmaf(reduced_hi, log_poly, log2_lo);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log2f(float x)
{
    if (isnan(x)) {
        return x + x;
    }
    if (x == ASCRT_INF_F) {
        return ASCRT_INF_F;
    }
    if (x == 0.0f) {
        return -ASCRT_INF_F;
    }
    if (x < 0.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    float log2_hi = 0.0f;
    float log2_lo = 0.0f;
    __internal_log2f_positive_finite(x, log2_hi, log2_lo);
    return log2_hi + log2_lo;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rsqrtf(float x) { return 1.0f / sqrtf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normcdfinvf(float x)
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

#define __INTERNAL_MODFF(x, n)                                  \
    do {                                                        \
        float abs_x = fabsf(x);                                 \
        float result;                                           \
        union Data {                                            \
            float f;                                            \
            unsigned int i;                                     \
        };                                                      \
        if (__isfinite(abs_x)) {                                \
            float integral_x = truncf(x);                       \
            *(n) = integral_x;                                  \
            float decimal = (x) - integral_x;                   \
            union Data data {                                   \
                .f = decimal                                    \
            };                                                  \
            uint32_t decimal_u32 = data.i;                      \
            union Data data_x {                                 \
                .f = (x)                                        \
            };                                                  \
            uint32_t u32 = data_x.i;                            \
            uint32_t y_bits = (u32 & 0x80000000) | decimal_u32; \
            union Data dataY {                                  \
                .i = y_bits                                     \
            };                                                  \
            result = dataY.f;                                   \
        } else {                                                \
            if (__isinf(abs_x)) {                               \
                union Data data {                               \
                    .f = (x)                                    \
                };                                              \
                uint32_t u32 = data.i;                          \
                uint32_t y_bits = u32 & 0x80000000;             \
                union Data data_y {                             \
                    .i = y_bits                                 \
                };                                              \
                result = data_y.f;                              \
                *(n) = (x);                                     \
            } else {                                            \
                result = (x) + (x);                             \
                *(n) = (x);                                     \
            }                                                   \
        }                                                       \
        return result;                                          \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float modff(float x, float* n) { __INTERNAL_MODFF(x, n); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float modff(float x, __ubuf__ float* n) { __INTERNAL_MODFF(x, n); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float modff(float x, __gm__ float* n) { __INTERNAL_MODFF(x, n); }
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bool isfinite(float x) { return __isfinite(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bool isnan(float x) { return __isnan(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bool isinf(float x) { return __isinf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fdimf(float x, float y)
{
    if (isnan(x)) {
        return x;
    } else if (isnan(y)) {
        return y;
    }
    return (x > y) ? (x - y) : 0;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sub_set_res_pos(float abs_x, float abs_y)
{
    return (abs_x < abs_y) ? abs_x - abs_y : abs_y - abs_x;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_set_quo(int32_t* quo, int32_t n_sign)
{
    int32_t neg_e = -8;
    int32_t max_s32 = 0xffffffff;
    int32_t one = 1;
    int32_t low_3bit = 0x7;

    if (n_sign < 0) {
        *quo = *quo ^ max_s32;
        *quo = *quo | neg_e;
        *quo = *quo + one;
    } else {
        *quo = *quo & low_3bit;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_x_le_y(
    float abs_x, float tmp_val, float abs_y, bool is_x_pos, uint32_t sign_flag, float res, int32_t* quo, int32_t n_sign)
{
    float double_x = abs_x + abs_x;
    float sign = (is_x_pos) ? 1.0 : -1.0;

    if (double_x > abs_y) {
        *quo += 1;
        __internal_set_quo(quo, n_sign);
        return sign * __internal_sub_set_res_pos(abs_x, abs_y);
    }

    if ((double_x != abs_y) | (sign_flag == 0)) {
        __internal_set_quo(quo, n_sign);
        if (is_x_pos) {
            return res;
        } else {
            return -abs_x;
        }
    }
    *quo += 1;
    __internal_set_quo(quo, n_sign);
    return sign * __internal_sub_set_res_pos(abs_x, abs_y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline uint32_t __asc_float_mantissa_bits_from_abs_bits(uint32_t bits)
{
    const uint32_t exponent_bits = bits & ASCRT_EXP_BIT_FLOAT_U;
    const uint32_t mantissa_bits = bits & ASCRT_MAN_BIT_FLOAT_U;
    return exponent_bits == 0U ? mantissa_bits : (mantissa_bits | 0x00800000U);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline int32_t __asc_float_mantissa_exponent_from_abs_bits(uint32_t bits)
{
    const uint32_t exponent_bits = bits & ASCRT_EXP_BIT_FLOAT_U;
    return exponent_bits == 0U ? -149 : (static_cast<int32_t>(exponent_bits >> 23U) - 150);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline int32_t __asc_uint_floor_log2(uint32_t x)
{
    int32_t shift = 0;
    while ((x >> 1U) != 0U) {
        x >>= 1U;
        ++shift;
    }
    return shift;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline float __asc_make_float_from_mantissa(uint32_t mantissa, int32_t exponent)
{
    if (mantissa == 0U) {
        return 0.0f;
    }

    const int32_t mantissa_log2 = __asc_uint_floor_log2(mantissa);
    const int32_t result_exponent = exponent + mantissa_log2;
    if (result_exponent > 127) {
        return ASCRT_INF_F;
    }
    if (result_exponent < -149) {
        return 0.0f;
    }
    if (result_exponent >= -126) {
        const uint32_t normalized_mantissa = mantissa << static_cast<uint32_t>(23 - mantissa_log2);
        const uint32_t bits =
            (static_cast<uint32_t>(result_exponent + 127) << 23U) | (normalized_mantissa & ASCRT_MAN_BIT_FLOAT_U);
        return __uint_as_float(bits);
    }

    const uint32_t subnormal_shift = static_cast<uint32_t>(exponent + 149);
    return __uint_as_float(mantissa << subnormal_shift);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline uint32_t __asc_double_abs_float_bits(uint32_t abs_bits)
{
    if (abs_bits >= ASCRT_INF_U) {
        return abs_bits;
    }
    const uint32_t exponent_bits = abs_bits & ASCRT_EXP_BIT_FLOAT_U;
    if (exponent_bits == 0U) {
        return (abs_bits & ASCRT_MAN_BIT_FLOAT_U) << 1U;
    }
    if (exponent_bits == 0x7F000000U) {
        return ASCRT_INF_U;
    }
    return abs_bits + 0x00800000U;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline float __asc_remquo_core(
    uint32_t abs_x_bits, uint32_t abs_y_bits, uint32_t* quo_abs_low)
{
    uint32_t quotient_low = 0U;
    if (abs_x_bits < abs_y_bits) {
        *quo_abs_low = 0U;
        return __uint_as_float(abs_x_bits);
    }

    uint32_t remainder_mantissa = __asc_float_mantissa_bits_from_abs_bits(abs_x_bits);
    const uint32_t y_mantissa = __asc_float_mantissa_bits_from_abs_bits(abs_y_bits);
    const int32_t x_exponent = __asc_float_mantissa_exponent_from_abs_bits(abs_x_bits);
    const int32_t y_exponent = __asc_float_mantissa_exponent_from_abs_bits(abs_y_bits);
    const int32_t exponent_diff = x_exponent - y_exponent;

    if (exponent_diff >= 0 && y_mantissa != 0U) {
        uint32_t remainder_bits = 0U;
        const int32_t x_mantissa_log2 = __asc_uint_floor_log2(remainder_mantissa);
        for (int32_t bit = x_mantissa_log2; bit >= 0; --bit) {
            remainder_bits = (remainder_bits << 1U) | ((remainder_mantissa >> static_cast<uint32_t>(bit)) & 1U);
            quotient_low = (quotient_low << 1U) & ASCRT_REMQUO_MASK_F;
            if (remainder_bits >= y_mantissa) {
                remainder_bits -= y_mantissa;
                quotient_low |= 1U;
            }
        }
        for (int32_t bit = 0; bit < exponent_diff; ++bit) {
            remainder_bits <<= 1U;
            quotient_low = (quotient_low << 1U) & ASCRT_REMQUO_MASK_F;
            if (remainder_bits >= y_mantissa) {
                remainder_bits -= y_mantissa;
                quotient_low |= 1U;
            }
        }
        remainder_mantissa = static_cast<uint32_t>(remainder_bits);
    }

    *quo_abs_low = quotient_low;
    return __asc_make_float_from_mantissa(remainder_mantissa, y_exponent);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline void __asc_remquo_tail(
    uint32_t abs_y_bits, float* remainder, uint32_t* quotient_low)
{
    const float abs_y = __uint_as_float(abs_y_bits);
    const float double_remainder = *remainder + *remainder;
    if (double_remainder > abs_y || (double_remainder == abs_y && ((*quotient_low & 1U) != 0U))) {
        *remainder = *remainder - abs_y;
        *quotient_low = (*quotient_low + 1U) & ASCRT_REMQUO_MASK_F;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ static inline float __asc_remquof_impl(float x, float y, int32_t* quo)
{
    constexpr uint32_t abs_mask = 0x7FFFFFFFU;
    const uint32_t x_bits = __float_as_uint(x);
    const uint32_t y_bits = __float_as_uint(y);
    const uint32_t abs_x_bits = x_bits & abs_mask;
    const uint32_t abs_y_bits = y_bits & abs_mask;
    *quo = 0;

    if (abs_x_bits > ASCRT_INF_U || abs_y_bits > ASCRT_INF_U) {
        return x + y;
    }
    if (abs_x_bits == ASCRT_INF_U || abs_y_bits == 0U) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (abs_y_bits == ASCRT_INF_U) {
        return x;
    }

    uint32_t quotient_low = 0U;
    float remainder = __asc_remquo_core(abs_x_bits, abs_y_bits, &quotient_low);
    __asc_remquo_tail(abs_y_bits, &remainder, &quotient_low);

    if ((x_bits & ASCRT_NEG_SIGN_BIT_U) != 0U) {
        const float abs_remainder = fabsf(remainder);
        if (abs_remainder <= ASCRT_INF_F) {
            remainder = -remainder;
        }
    }

    const bool quotient_is_negative = ((x_bits ^ y_bits) & ASCRT_NEG_SIGN_BIT_U) != 0U;
    const int32_t sign_mask = quotient_is_negative ? -1 : 0;
    *quo = (static_cast<int32_t>(quotient_low) ^ sign_mask) - sign_mask;
    if (fabsf(remainder) > ASCRT_INF_F) {
        *quo = 0;
    }
    return remainder;
}

#define __INTERNAL_REMQUOF(x, y, quo)                           \
    do {                                                        \
        int32_t tmp_quo = 0;                                    \
        float tmp_res = __asc_remquof_impl((x), (y), &tmp_quo); \
        *(quo) = tmp_quo;                                       \
        return tmp_res;                                         \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float remquof(float x, float y, int* quo) { __INTERNAL_REMQUOF(x, y, quo); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float remquof(float x, float y, __ubuf__ int* quo)
{
    __INTERNAL_REMQUOF(x, y, quo);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float remquof(float x, float y, __gm__ int* quo)
{
    __INTERNAL_REMQUOF(x, y, quo);
}
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_set_res_mod_neg(float mod_res)
{
    uint32_t* u_mod_res = reinterpret_cast<uint32_t*>(&mod_res);
    *u_mod_res = (*u_mod_res) | ASCRT_NEG_SIGN_BIT_U;
    return mod_res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fmodf(float x, float y)
{
    uint32_t ux = reinterpret_cast<uint32_t&>(x);
    uint32_t uy = reinterpret_cast<uint32_t&>(y);
    uint32_t sx = ux & 0x80000000U;
    ux &= 0x7FFFFFFFU;
    uy &= 0x7FFFFFFFU;

    if (uy == 0U || ux >= 0x7F800000U || uy > 0x7F800000U) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (ux < uy) {
        return x;
    }
    if (ux == uy) {
        return reinterpret_cast<float&>(sx);
    }

    int32_t ex = static_cast<int32_t>(ux >> 23);
    int32_t ey = static_cast<int32_t>(uy >> 23);
    uint32_t mx = ux;
    uint32_t my = uy;

    if (ex == 0) {
        int32_t i = 0;
        mx = ux;
        while ((mx & 0x00800000U) == 0U) {
            mx <<= 1;
            i++;
        }
        ex = 1 - i;
    } else {
        mx = (ux & 0x007FFFFFU) | 0x00800000U;
    }

    if (ey == 0) {
        int32_t i = 0;
        my = uy;
        while ((my & 0x00800000U) == 0U) {
            my <<= 1;
            i++;
        }
        ey = 1 - i;
    } else {
        my = (uy & 0x007FFFFFU) | 0x00800000U;
    }

    while (ex > ey) {
        uint32_t d = mx - my;
        if ((d & 0x80000000U) == 0U) {
            if (d == 0U) {
                return reinterpret_cast<float&>(sx);
            }
            mx = d;
        }
        mx <<= 1;
        ex--;
    }

    uint32_t d = mx - my;
    if ((d & 0x80000000U) == 0U) {
        if (d == 0U) {
            return reinterpret_cast<float&>(sx);
        }
        mx = d;
    }

    while ((mx & 0x00800000U) == 0U) {
        mx <<= 1;
        ex--;
    }

    uint32_t out;
    if (ex > 0) {
        mx -= 0x00800000U;
        out = mx | (static_cast<uint32_t>(ex) << 23);
    } else {
        mx >>= static_cast<uint32_t>(1 - ex);
        out = mx;
    }
    out |= sx;
    return reinterpret_cast<float&>(out);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float remainderf(float x, float y)
{
    int32_t quo = -1;
    return remquof(x, y, &quo);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_with_sign_bit(float value, float sign_source)
{
    constexpr uint32_t sign_mask = 0x80000000U;
    constexpr uint32_t value_mask = 0x7FFFFFFFU;
    const uint32_t value_bits = __float_as_uint(value) & value_mask;
    const uint32_t sign_bits = __float_as_uint(sign_source) & sign_mask;
    return __uint_as_float(value_bits | sign_bits);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float copysignf(float x, float y) { return __internal_with_sign_bit(x, y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_asin_acos_reduced_arg(float abs_x)
{
    constexpr float threshold = 0.56000000238418579102f;

    float reduced = 0.0f;
    if (abs_x != 1.0f) {
        // 对应 nv.md 中 asin/acos 的 MUFU.RSQ + 两次 FFMA：计算 sqrt(0.5 * (1 - |x|)) 并做一次修正。
        const float half_one_minus_abs = fmaf(0.5f, -abs_x, 0.5f);
        const float inv_sqrt = rsqrtf(half_one_minus_abs);
        float sqrt_term = half_one_minus_abs * inv_sqrt;
        const float correction = fmaf(-sqrt_term, inv_sqrt * 0.5f, 0.5f);
        reduced = fmaf(sqrt_term, correction, sqrt_term);
    }
    return (abs_x > threshold) ? reduced : abs_x;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float nearbyintf(float x)
{
    if (isinf(x) || isnan(x)) {
        return x;
    }
    return __rintf(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float nextafterf(float x, float y)
{
    if (isnan(x) || isnan(y)) {
        return ASCRT_NAN_F;
    }

    uint32_t* f = reinterpret_cast<uint32_t*>(&x);
    if (x > 0) {
        if (x < y) { // when x < src, x bit +1
            (*f)++;
        } else if (x > y) { // when x > src, x bit -1
            (*f)--;
        }
    } else if (x < 0) {
        if (x > y) {
            (*f)++;
        } else if (x < y) {
            (*f)--;
        }
    } else if (x == 0) {
        if (y > 0) {
            *f = 1;
        } else if (y < 0) {
            *f = 0x80000001;
        }
    }
    return x;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_scalbnf_clamped(float x, int32_t n)
{
    constexpr int32_t max_scale_exponent = 0x116;
    constexpr int32_t split_threshold = 0x65;
    constexpr int32_t simple_exponent_bias = 0x7F;
    constexpr int32_t split_exponent_bias = 0x1FC;
    constexpr uint32_t fp32_one_bits = 0x3F800000U;
    constexpr uint32_t fp32_exponent_shift = 23U;

    int32_t clamped_n = n > max_scale_exponent ? max_scale_exponent : n;
    clamped_n = clamped_n < -max_scale_exponent ? -max_scale_exponent : clamped_n;

    uint32_t scale_bits = 0U;
    uint32_t split_scale_bits = fp32_one_bits;
    if (clamped_n >= split_threshold || clamped_n <= -split_threshold) {
        const int32_t biased_exponent = clamped_n + split_exponent_bias;
        const int32_t split_exponent = static_cast<uint32_t>(biased_exponent) >> 2U;
        const int32_t residual_exponent = biased_exponent - split_exponent * 3;
        split_scale_bits = static_cast<uint32_t>(split_exponent) << fp32_exponent_shift;
        scale_bits = static_cast<uint32_t>(residual_exponent) << fp32_exponent_shift;
    } else {
        scale_bits = static_cast<uint32_t>(clamped_n + simple_exponent_bias) << fp32_exponent_shift;
    }

    float result = __uint_as_float(scale_bits) * x;
    const float split_scale = __uint_as_float(split_scale_bits);
    result = split_scale * result;
    result = split_scale * result;
    result = split_scale * result;
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float scalbnf(float x, int32_t n) { return __internal_scalbnf_clamped(x, n); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float scalblnf(float x, int64_t n)
{
    constexpr int64_t max_scale_exponent = 0x116;
    int32_t clamped_n = 0;
    if (n > max_scale_exponent) {
        clamped_n = static_cast<int32_t>(max_scale_exponent);
    } else if (n < -max_scale_exponent) {
        clamped_n = static_cast<int32_t>(-max_scale_exponent);
    } else {
        clamped_n = static_cast<int32_t>(n);
    }
    return __internal_scalbnf_clamped(x, clamped_n);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fmaxf(float x, float y)
{
    if (isnan(x)) {
        return y;
    } else if (isnan(y)) {
        return x;
    }
    return __fmaxf(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fminf(float x, float y)
{
    if (isnan(x)) {
        return y;
    } else if (isnan(y)) {
        return x;
    }
    return __fminf(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_payne_hanek_radian_reduction(float x, int* output_quadrant)
{
    // Step 1: Extract raw bits of the input angle
    uint32_t input_bits = reinterpret_cast<uint32_t&>(x);

    // Step 2: Extract exponent and compute index into 2/pi table
    int32_t exponent = ((input_bits & 0x7F800000) >> 23) - 127;
    uint32_t exponent_index = static_cast<uint32_t>(exponent) >> 5;

    // Step 3: Get the 2/pi table entries for this exponent index
    constexpr uint32_t two_over_pi_table[] = {0x517cc1b7, 0x27220a94, 0xfe13abe8, 0xfa9a6ee0, 0x6db14acc, 0x9e21c820};
    uint32_t high_term = exponent_index ? two_over_pi_table[exponent_index - 1] : 0;
    uint32_t mid_term = two_over_pi_table[exponent_index];
    uint32_t low_term = two_over_pi_table[exponent_index + 1];
    uint32_t last_term = two_over_pi_table[exponent_index + 2];

    // Step 4: Compute exponent remainder and shift table entries accordingly
    int32_t exponent_remainder = static_cast<uint32_t>(exponent) & 0x1F;
    if (exponent_remainder != 0) {
        high_term = (high_term << exponent_remainder) | (mid_term >> (ASCRT_FOUR_BYTE_LEN_U - exponent_remainder));
        mid_term = (mid_term << exponent_remainder) | (low_term >> (ASCRT_FOUR_BYTE_LEN_U - exponent_remainder));
        low_term = (low_term << exponent_remainder) | (last_term >> (ASCRT_FOUR_BYTE_LEN_U - exponent_remainder));
    }

    // Step 5: Extract and normalize the mantissa
    uint32_t mantissa = (input_bits & 0x007FFFFF) | 0x4F000000;
    uint32_t normalized_mantissa = static_cast<uint32_t>(reinterpret_cast<float&>(mantissa));

    // Step 6: Compute product = (mantissa * high_term) << 32 + mantissa * mid_term + mantissa * low_term
    uint64_t product = static_cast<uint64_t>(normalized_mantissa) * low_term;
    product = static_cast<uint64_t>(normalized_mantissa) * mid_term + (product >> ASCRT_FOUR_BYTE_LEN_U);
    product = (static_cast<uint64_t>(normalized_mantissa * high_term) << ASCRT_FOUR_BYTE_LEN_U) + product;

    // Step 7: Extract quotient and remainder
    int32_t quotient = static_cast<int32_t>(product >> 62);
    product = product & 0x3FFFFFFFFFFFFFFFULL;

    // Step 8: Handle carry
    if (product & 0x2000000000000000ULL) {
        product -= 0x4000000000000000ULL;
        quotient += 1;
    }

    // Step 9: Split product into high and low
    int64_t product_int64 = static_cast<int64_t>(product);
    int64_t high_float = static_cast<float>(product_int64);
    product_int64 = product_int64 - static_cast<int64_t>(high_float);
    int64_t low_float = static_cast<float>(product_int64);

    // Step 10: Compute final result = (high + low) * pi/2 * 2^-62
    float pi_over_two_low = 3.4061215800865545e-19f; // pi/2 * 2^-62
    float reduced_angle = (high_float + low_float) * pi_over_two_low;

    // Step 11: Handle negative input
    if (x < 0.0f) {
        reduced_angle = -reduced_angle;
        quotient = -quotient;
    }

    // Step 12: Return result
    *output_quadrant = quotient;
    return reduced_angle;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cody_waite_radian_reduction(float x, int* quadrant)
{
    float y = fmaf(x, 0.636619747f, 12582912.0f); // 0.636619747f: 2/pi
    *quadrant = reinterpret_cast<int&>(y);
    y = y - 12582912.0f;                 // 12582912.0f: used to truncate mantissa of x*(2/pi)
    x = fmaf(y, -1.57079601e+00f, x);    // 1.57079601e+00f: high of pi/2
    x = fmaf(y, -3.13916473e-07f, x);    // 3.13916473e-07f: middle of pi/2
    return fmaf(y, -5.39030253e-15f, x); // 5.39030253e-15f: low of pi/2
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_trig_radian_reduction(float x, float threshold, int* quadrant)
{
    x = fmaf(x, 0.0f, x);
    if (fabsf(x) > threshold) {
        return __internal_payne_hanek_radian_reduction(x, quadrant);
    } else {
        return __internal_cody_waite_radian_reduction(x, quadrant);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_tan_poly(float x)
{
    x = x * x;
    float y = fmaf(x, 4.38117981e-3f, 8.94600598e-5f); // 4.38117981e-3f: 8.94600598e-5f:
    y = fmaf(x, y, 1.08341556e-2f);                    // 1.08341556e-2f:
    y = fmaf(x, y, 2.12811474e-2f);                    // 2.12811474e-2f: 62/2838
    y = fmaf(x, y, 5.40602170e-2f);                    // 5.40602170e-2f: 17/315
    y = fmaf(x, y, 1.33326918e-1f);                    // 1.33326918e-1f: 2/15
    y = fmaf(x, y, 3.33333433e-1f);                    // 3.33333433e-1f: 1/3
    return x * y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanf(float x)
{
    // Step 1: Reduce the angle to the range [0, pi/2) and determine the quadrant
    int quadrant;
    float y =
        __internal_trig_radian_reduction(x, 252.898206f, &quadrant); // 252.898206f: Threshold for reduction algorithm

    // Step 2: Compute the tangent using polynomial approximation
    float t = __internal_tan_poly(y);

    // Step 3: Compute the initial approximation of tan(y)
    float z = fmaf(t, y, y);

    // Step 4: Adjust the tangent value based on the quadrant
    if (quadrant & 1) { // Quadrants 1 and 3: tan(pi/2 + x) = -cot(x)
        float s = y - z;
        s = fmaf(t, y, s);
        t = -1.0f / z;
        z = fmaf(z, t, 1.0f);
        z = fmaf(s, t, z);
        z = fmaf(z, t, t);
    }

    return z;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanhf(float x)
{
    // 对应 MOV R5, 0x3c80f082；该系数在首个 FFMA 中以负号使用。
    constexpr float poly_first_coeff = 0.01573968306183815002f;
    constexpr float small_threshold = 0.60000002384185791016f;
    constexpr float saturation_threshold = 9.010913848876953125f;
    constexpr uint32_t sign_mask = 0x80000000U;

    if (isnan(x)) {
        return x + x;
    }
    if (isinf(x)) {
        return __internal_with_sign_bit(1.0f, x);
    }

    const float abs_x = fabsf(x);

    // 对应 FMUL |x|, 2.8853900432586669922 和 MUFU.EX2：
    // 2.8853900432586669922 约等于 2 / ln(2)，因此 exp2(abs_x * 2 / ln2) 等价于 exp(2 * abs_x)。
    const float exp2_arg = abs_x * 2.8853900432586669922f;
    const float exp2_value = exp2f(exp2_arg);

    // 对应 FMUL R4, x, x：小区间多项式使用 x^2 作为自变量。
    const float x2 = x * x;
    // 对应连续 FFMA：P(x^2)，最终小区间结果为 x + x * x^2 * P(x^2)。
    float poly = fmaf(x2, poly_first_coeff, -0.052303962409496307373f);
    poly = fmaf(x2, poly, 0.13315297663211822510f);
    poly = fmaf(x2, poly, -0.33332768082618713379f);
    poly = fmaf(x2, poly, 0.0f);

    // 对应 FADD + MUFU.RCP + FFMA：1 - 2 / (exp(2 * |x|) + 1)。
    float large_result = fmaf(1.0f / (exp2_value + 1.0f), -2.0f, 1.0f);
    // 对应 FSEL：当 |x| >= 9.010913848876953125 时结果按幅值饱和为 1。
    if (abs_x >= saturation_threshold) {
        large_result = 1.0f;
    }
    // 对应 LOP3.LUT：把输入 x 的符号位合成到大区间结果上。
    const uint32_t sign_bit = __float_as_uint(x) & sign_mask;
    const uint32_t large_result_bits = __float_as_uint(large_result) | sign_bit;
    float result = __uint_as_float(large_result_bits);

    // 对应 @!P1 FFMA：|x| < 0.60000002384185791016 时使用奇多项式近似 tanh(x)。
    if (abs_x < small_threshold) {
        result = fmaf(poly, x, x);
    }
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_tanpif_small(float x)
{
    constexpr float pi_hi = 3.1415927410125732422f;
    constexpr float pi_lo = -8.7422776573475857731e-08f;
    const float arg = fmaf(x, pi_hi, x * pi_lo);
    return tanf(arg);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanpif(float x)
{
    constexpr float large_input_bound = 16777216.0f;

    if (isnan(x) || isinf(x)) {
        return x - x;
    }
    if (fabsf(x) >= large_input_bound) {
        return 0.0f * x;
    }

    const int32_t nearest_integer = __float2int_rn(x);
    const float rounded_integer = __int2float_rn(nearest_integer);
    const float reduced = x - rounded_integer;
    const float abs_reduced = fabsf(reduced);

    if (reduced == 0.0f) {
        return 0.0f * x;
    }
    if (abs_reduced == 0.5f) {
        return __internal_with_sign_bit(ASCRT_INF_F, reduced);
    }
    if (abs_reduced <= 0.25f) {
        return __internal_tanpif_small(reduced);
    }

    const float distance_to_half = 0.5f - abs_reduced;
    const float cot_base = __internal_tanpif_small(distance_to_half);
    const float result = 1.0f / cot_base;
    return __internal_with_sign_bit(result, reduced);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_taylor_expand(
    float& dst, float& src, float& square_v, uint32_t expand_level, float* factor)
{
    square_v = src * src;
    dst = src * src;
    dst = dst * factor[expand_level];
    for (int i = expand_level - 1; i > 0; i--) {
        dst = dst + factor[i];
        dst = dst * square_v;
    }
    dst = dst + factor[0];
    dst = dst * src;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_taylor_expand(
    float& dst, float& src, float& square_v, uint32_t expand_level)
{
    float factor[] = {1,
                      -0.3333333333333333,
                      0.2,
                      -0.14285714285714285,
                      0.1111111111111111,
                      -0.09090909090909091,
                      0.07692307692307693};
    __internal_taylor_expand(dst, src, square_v, expand_level, factor);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_atan_expand(
    float& dst, float& src, float& tmp, float trans_factor)
{
    dst = src * trans_factor;
    dst = dst + 1.0f;
    tmp = src - trans_factor;
    dst = tmp / dst;
    dst = fabsf(dst);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_sign(float& dst, float& src, float& denominator)
{
    dst = src * 4611686018427387904.0f; // 4611686018427387904 : ATAN_FP32_MAX
    denominator = fabsf(dst);
    denominator = denominator + 2.168404344971009e-19f; // 2.168404344971009e-19 : ATAN_FP32_MIN
    dst = dst / denominator;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanf(float x)
{
    if (isnan(x)) {
        return x + x;
    }
    if (isinf(x)) {
        return __internal_with_sign_bit(ASCRT_PIO2_F, x);
    }

    const float abs_x = fabsf(x);
    const bool use_reciprocal = abs_x > 1.0f;

    // 对应 MUFU.RCP + FSEL：|x| > 1 时使用 1 / |x| 规约，否则直接使用 |x|。
    float reduced = use_reciprocal ? (1.0f / abs_x) : abs_x;
    const float reduced2 = reduced * reduced;

    // 对应 nv.md 中连续 FFMA：atan(reduced) ~= reduced + reduced^3 * P(reduced^2)。
    float poly = fmaf(reduced2, 0.00245002890005707741f, -0.014396979473531246185f);
    poly = fmaf(reduced2, poly, 0.039849750697612762451f);
    poly = fmaf(reduced2, poly, -0.072529748082160949707f);
    poly = fmaf(reduced2, poly, 0.10518480092287063599f);
    poly = fmaf(reduced2, poly, -0.14171802997589111328f);
    poly = fmaf(reduced2, poly, 0.19988775253295898438f);
    poly = fmaf(reduced2, poly, -0.33332940936088562012f);
    poly *= reduced2;
    float result = fmaf(reduced, poly, reduced);

    // 对应 @P0 FFMA：|x| > 1 时由 atan(x)=pi/2-atan(1/x) 还原。
    if (use_reciprocal) {
        result = fmaf(0.93318945169448852539f, 1.6832555532455444336f, -result);
    }

    return __internal_with_sign_bit(result, x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atan2f(float y, float x)
{
    float ay = __fabsf(y);
    float ax = __fabsf(x);
    float sum = ax + ay;
    if (__isnan(sum)) {
        return sum;
    }

    bool y_gt_x = ay > ax;
    float hi = y_gt_x ? ay : ax;
    float lo = y_gt_x ? ax : ay;

    float a = 0.0f;
    if (hi != 0.0f) {
        float r = lo / hi;
        float z = r * r;
        float p = 0.0027380611281841993332f /* 0x3b33710b */;
        p = __fma(z, p, -0.015681877732276916504f /* 0xbc807748 */);
        p = __fma(z, p, 0.042200751602649688721f /* 0x3d2cdab2 */);
        p = __fma(z, p, -0.074792981147766113281f /* 0xbd992d10 */);
        p = __fma(z, p, 0.10640415549278259277f /* 0x3dd9ea6c */);
        p = __fma(z, p, -0.14207722246646881104f /* 0xbe117cb1 */);
        p = __fma(z, p, 0.19993925094604492188f /* 0x3e4cbce0 */);
        p = __fma(z, p, -0.33333197236061096191f /* 0xbeaaaa7d */);
        float t = __fma(z * p, r, r);

        if (ay == ax) {
            a = signbit(x) ? 2.35619449615478515625f /* 0x4016cbe4 */
                             :
                             0.78539818525314331055f /* 0x3f490fdb */;
        } else if (y_gt_x) {
            float pio2 = 1.57079637050628662109f /* 0x3fc90fdb */;
            a = signbit(x) ? (pio2 + t) : (pio2 - t);
        } else {
            a = signbit(x) ? (3.14159274101257324219f /* 0x40490fdb */ - t) : t;
        }
    } else {
        a = signbit(x) ? 3.14159274101257324219f /* 0x40490fdb */ : 0.0f;
    }

    return signbit(y) ? -a : a;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log1pf(float x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanhf(float x)
{
    if (isnan(x)) {
        return x + x;
    }
    if (x == 1.0f) {
        return ASCRT_INF_F;
    }
    if (x == -1.0f) {
        return -ASCRT_INF_F;
    }
    if (fabsf(x) > 1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    const float abs_x = fabsf(x);
    // 对应 FADD + MUFU.RCP + FADD + FMUL：构造 log1p 的输入 2*|x|/(1-|x|)。
    float log_arg = (2.0f / (1.0f - abs_x)) * abs_x;

    // 对应后续 log1p 规约多项式以及最终乘以带 x 符号的 0.5。
    return __internal_with_sign_bit(0.5f, x) * log1pf(log_arg);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cos_poly(float x)
{
    x = x * x;
    float y = fmaf(x, 2.44677067e-5f, -1.38877297e-3f); // 2.44677067e-5f: 1/8! -1.38877297e-3f: -1/6!
    y = fmaf(x, y, 4.16666567e-2f);                     //  4.16666567e-2f: 1/4!
    y = fmaf(x, y, -5.00000000e-1f);                    // -5.00000000e-1f: -1/2!
    return fmaf(x, y, 1.00000000e+0f);                  //  1.00000000e+0f: 1
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sin_poly(float x)
{
    float y = x * x;
    float m = fmaf(x, y, 0.0f);

    float z = fmaf(y, 2.86567956e-6f, -1.98559923e-4f); //  2.86567956e-6f:  1/9! * x^2 -1.98559923e-4f: -1/7!
    z = fmaf(y, z, 8.33338592e-3f);                     // 8.33338592e-3f: 1/5! * x^2
    z = fmaf(y, z, -1.66666672e-1f);                    // -1.66666672e-1f: -1/3! * x^2

    return fmaf(z, m, x); // * x^3 + x
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cosf(float x)
{
    // Step 1: Reduce the angle to the range [0, pi/2) and determine the quadrant
    int quadrant;
    float y =
        __internal_trig_radian_reduction(x, 71476.0625f, &quadrant); // 71476.0625f: Threshold for reduction algorithm

    // Step 2: Compute cosine and sine of the reduced angle using polynomial approximations
    float c = __internal_cos_poly(y);
    float s = __internal_sin_poly(y);

    // Step 3: Adjust the cosine value based on the quadrant
    if (quadrant & 2) { // Quadrants 2 and 3: cos(pi + x) = -cos(x)
        c = -c;
        s = -s;
    }
    if (quadrant & 1) { // Quadrants 1 and 3: cos(pi/2 + x) = -sin(x)
        c = -s;
    }

    // Return the final cosine value
    return c;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float coshf(float x)
{
    float ax = __fabsf(x);
    if (__isnan(ax) || ax == ASCRT_INF_F) {
        return ax;
    }

    float n = truncf(ax * 1.4426950216293334961f /* 0x3fb8aa3b */);
    if (__fabsf(n) > 126.0f /* 0x42fc0000 */) {
        n = 126.0f;
    }

    float r = __fma(n, -0.69314718246459960938f /* 0xbf317218 */, ax);
    r = __fma(n, 1.9046542121259335545e-09f /* 0x3102e308 */, r);
    float scale_base = n + 12583037.0f;
    uint32_t scale_bits = reinterpret_cast<uint32_t&>(scale_base) << 23;
    float scale = reinterpret_cast<float&>(scale_bits);
    float e = scale * __expf(r);
    float inv_term = (1.0f / e) * 0.125f /* 0x3e000000 */;
    return __fma(e, 2.0f, inv_term);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cospif(float x)
{
    if (__isnan(x)) {
        return x + x;
    }
    if (x == ASCRT_INF_F || x == -ASCRT_INF_F) {
        return x * 0.0f;
    }
    if (__fabsf(x) > 16777216.0f) {
        return 1.0f;
    }

    float t = x + x;
    int k = __cvt_int32_t<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_ENABLE_VALUE>(t);
    float kf = rintf(t);
    float r = __fma(-kf, 0.5f, x);
    float z = r * r;

    float c = __fma(z, 0.226860314607620239257812f /* 0x3e684e12 */, -1.334560394287109375f /* 0xbfaad2e0 */);
    c = __fma(z, c, 4.058692455291748046875f /* 0x4081e0cf */);
    c = __fma(z, c, -4.93480205535888671875f /* 0xc09de9e6 */);
    c = __fma(z, c, 1.0f);

    float s = __fma(z, -0.592480242252349853515625f /* 0xbf17acc9 */, 2.550144195556640625f /* 0x40233590 */);
    s = __fma(z, s, -5.16771984100341796875f /* 0xc0a55df6 */);
    s = s * (r * z);
    s = __fma(r, 3.1415927410125732421875f /* 0x40490fdb */, s);

    int q = k + 1;
    float y = ((q & 1) != 1) ? s : c;
    return (q & 2) ? -y : y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float asinf(float x)
{
    constexpr float threshold = 0.56000000238418579102f;
    constexpr float half_pi_hi = 1.6832555532455444336f;
    constexpr float half_pi_lo_scale = 0.93318945169448852539f;

    if (isnan(x)) {
        return x + x;
    }
    if (fabsf(x) > 1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    const float abs_x = fabsf(x);
    float reduced = __internal_asin_acos_reduced_arg(abs_x);
    const float reduced2 = reduced * reduced;

    // 对应 nv.md 中 asinf 连续 FFMA：asin(reduced) ~= reduced + reduced^3 * P(reduced^2)。
    float poly = fmaf(reduced2, 0.05025001987814903259f, 0.018773360177874565125f);
    poly = fmaf(reduced2, poly, 0.046769052743911743164f);
    poly = fmaf(reduced2, poly, 0.074823014438152313232f);
    poly = fmaf(reduced2, poly, 0.16667181253433227539f);
    poly *= reduced2;
    float result = fmaf(reduced, poly, reduced);

    // 对应 @P0 FFMA：|x| > 0.56 时使用 asin(x)=sign(x)*(pi/2-2*asin(sqrt((1-|x|)/2)))。
    if (abs_x > threshold) {
        result = fmaf(half_pi_hi, half_pi_lo_scale, -2.0f * result);
    }

    return __internal_with_sign_bit(result, x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float acosf(float x)
{
    constexpr float threshold = 0.56000000238418579102f;
    constexpr float half_pi_hi = 1.6832555532455444336f;
    constexpr float half_pi_lo_scale = 0.93318945169448852539f;

    if (isnan(x)) {
        return x + x;
    }
    if (fabsf(x) > 1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    const float abs_x = fabsf(x);
    float reduced = __internal_asin_acos_reduced_arg(abs_x);
    reduced = __internal_with_sign_bit(reduced, x);
    const float reduced2 = reduced * reduced;

    // 对应 nv.md 中 acosf 连续 FFMA：先计算 asin(reduced)。
    float poly = fmaf(reduced2, 0.03538220748305320740f, 0.016980519518256187439f);
    poly = fmaf(reduced2, poly, 0.030762933194637298584f);
    poly = fmaf(reduced2, poly, 0.044709417968988418579f);
    poly = fmaf(reduced2, poly, 0.074989043176174163818f);
    poly = fmaf(reduced2, poly, 0.16666707396507263184f);
    poly *= reduced2;
    float asin_reduced = fmaf(reduced, poly, reduced);

    // 对应 FSEL/@!P1/@P0：按 |x| 和 x 的符号选择 pi/2-asin(x)、2*asin(reduced) 或 pi-2*asin(reduced)。
    float result = asin_reduced;
    if (!(x > threshold)) {
        const float correction = (abs_x > threshold) ? asin_reduced : -asin_reduced;
        result = fmaf(half_pi_hi, half_pi_lo_scale, correction);
    }
    if (abs_x > threshold) {
        result = result + result;
    }
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float acoshf(float x)
{
    if (x < 1) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float t = x - 1.0f;
    if (t <= 0.5f) {
        float factor = 0.000045124618889065459371f;
        factor = __fma(factor, t, -0.000109100341796875f);
        factor = __fma(factor, t, 0.00027113739657215774059f);
        factor = __fma(factor, t, -0.00069930072128772735596f);
        factor = __fma(factor, t, 0.0018988715019077062607f);
        factor = __fma(factor, t, -0.0055803572759032249451f);
        factor = __fma(factor, t, 0.018750000745058059692f);
        factor = __fma(factor, t, -0.083333335816860198975f);
        factor = __fma(factor, t, 1.0f);
        return __sqrtf(2.0f * t) * factor;
    }
    if (x > 8388609.0f) {
        return __logf(x) + 0.69314718246459960938f;
    }
    return __logf(1.0f + t + __sqrtf(t * (x + 1.0f)));
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinf(float x)
{
    int quadrant;
    float y =
        __internal_trig_radian_reduction(x, 71476.0625f, &quadrant); // 71476.0625f: Threshold for reduction algorithm

    // Step 2: Compute cosine and sine of the reduced angle using polynomial approximations
    float s = __internal_sin_poly(y);
    float c = __internal_cos_poly(y);

    // Step 3: Adjust the sine value based on the quadrant
    if (quadrant & 2) { // Quadrants 2 and 3: sin(pi + x) = -sin(x)
        s = -s;
        c = -c;
    }
    if (quadrant & 1) { // Quadrants 1 and 3: sin(pi/2 + x) = cos(x)
        s = c;
    }

    return s;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinhf(float x)
{
    if (isnan(x)) {
        return x + x;
    }
    if (isinf(x)) {
        return x;
    }

    const float abs_x = fabsf(x);
    const float x2 = x * x;

    // 对应 nv.md 中小区间 sinh(x) = x + x*x^2*P(x^2)。
    float poly = fmaf(x2, 0.00000281695110970758826f, 0.00019836159481201320887f);
    poly = fmaf(x2, poly, 0.0083333496004343032837f);
    poly = fmaf(x2, poly, 0.16666667163372039795f);
    poly *= x2;

    // 对应 FRND.TRUNC(|x|/ln2) 和 |n| > 126 的饱和指数选择。
    float n = truncf(abs_x * 1.4426950216293334961f);
    if (fabsf(n) > 126.0f) {
        n = __internal_with_sign_bit(126.0f, n);
    }

    // 对应 FFMA 两段常数拆分：r = |x| - n*ln2。
    float r = fmaf(n, -0.69314718246459960938f, abs_x);
    r = fmaf(n, 1.9046542121259335545e-09f, r);
    const float exp2_residual = exp2f(r * 1.4426950216293334961f);

    // 对应 FADD 12583037 + SHF.L：构造 2^(n-2)，再乘 exp2(r) 得到 exp(|x|)/4。
    const float exponent_base = n + 12583037.0f;
    const uint32_t exponent_base_bits = __float_as_uint(exponent_base);
    const float exp_quarter = __uint_as_float(exponent_base_bits << 23) * exp2_residual;
    float result = fmaf(exp_quarter, 2.0f, -0.125f / exp_quarter);
    result = __internal_with_sign_bit(result, x);

    // 对应 @!P0 FFMA：|x| < 1 时使用小区间多项式。
    if (abs_x < 1.0f) {
        result = fmaf(poly, x, x);
    }
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sinpif_core(float x)
{
    constexpr float large_input_bound = 16777216.0f;
    constexpr float pi_hi = 3.1415927410125732422f;

    if (isnan(x) || isinf(x)) {
        return x - x;
    }

    if (fabsf(x) > large_input_bound) {
        return 0.0f * x;
    }

    const float truncated_x = truncf(x);
    const float two_x = x + x;
    const int32_t quadrant = __float2int_rn(two_x);
    const float rounded_two_x = __int2float_rn(quadrant);
    const bool use_cos_poly = ((quadrant & 1) != 0);
    const float reduced = fmaf(-rounded_two_x, 0.5f, x);
    const float reduced2 = reduced * reduced;

    float poly = use_cos_poly ? 0.22686031460762023926f : -0.59248024225234985352f;
    poly = fmaf(reduced2, poly, use_cos_poly ? -1.334560394287109375f : 2.550144195556640625f);
    poly = fmaf(reduced2, poly, use_cos_poly ? 4.0586924552917480469f : -5.1677198410034179688f);

    float result = 0.0f;
    if (use_cos_poly) {
        poly = fmaf(reduced2, poly, -4.9348020553588867188f);
        result = fmaf(poly, reduced2, 1.0f);
    } else {
        result = fmaf(poly, reduced * reduced2, reduced * pi_hi);
    }

    if ((quadrant & 2) != 0) {
        result = -result;
    }
    if (truncated_x == x) {
        result = 0.0f * x;
    }

    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinpif(float x) { return __internal_sinpif_core(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float asinhf(float x)
{
    float ax = __fabsf(x);
    if (ax == ASCRT_INF_F) {
        return x;
    }
    if (ax < 1.0e-8f) {
        return x;
    }

    float y;
    if (ax <= 0.5f) {
        float z = ax * ax;
        float p = -0.01396484375f;
        p = __fma(p, z, 0.017352764423076923077f);
        p = __fma(p, z, -0.022372159090909090909f);
        p = __fma(p, z, 0.030381944444444444444f);
        p = __fma(p, z, -0.044642857142857142857f);
        p = __fma(p, z, 0.075f);
        p = __fma(p, z, -0.16666666666666666667f);
        y = __fma(ax * z, p, ax);
    } else if (ax > 1.0e19f) {
        y = __logf(ax) + 0.69314718246459960938f;
    } else {
        float s = __sqrtf(__fma(ax, ax, 1.0f));
        float u = ax + ax * ax / (1.0f + s);
        y = __logf(1.0f + u);
    }
    return signbit(x) ? -y : y;
}

#define __INTERNAL_SINCOSF(x, s, c)                                              \
    do {                                                                         \
        int quadrant;                                                            \
        float t;                                                                 \
        float y = __internal_trig_radian_reduction((x), 71476.0625f, &quadrant); \
        float cos = __internal_cos_poly(y);                                      \
        float sin = __internal_sin_poly(y);                                      \
        if (quadrant & 2) {                                                      \
            sin = -sin;                                                          \
            cos = -cos;                                                          \
        }                                                                        \
        if (quadrant & 1) {                                                      \
            t = -sin;                                                            \
            sin = cos;                                                           \
            cos = t;                                                             \
        }                                                                        \
        *(s) = sin;                                                              \
        *(c) = cos;                                                              \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, float* s, float* c) { __INTERNAL_SINCOSF(x, s, c); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, float* s, __gm__ float* c) { __INTERNAL_SINCOSF(x, s, c); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, __gm__ float* s, float* c) { __INTERNAL_SINCOSF(x, s, c); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, __gm__ float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, __gm__ float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, __ubuf__ float* s, float* c)
{
    __INTERNAL_SINCOSF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, __ubuf__ float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincosf(float x, __ubuf__ float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSF(x, s, c);
}
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_sincospif_core(float x, float* sin_result, float* cos_result)
{
    constexpr float large_input_bound = 16777216.0f;
    constexpr float pi_hi = 3.1415927410125732422f;

    if (isnan(x) || isinf(x)) {
        const float nan_result = x - x;
        *sin_result = nan_result;
        *cos_result = nan_result;
        return;
    }

    if (fabsf(x) > large_input_bound) {
        *sin_result = 0.0f * x;
        *cos_result = 1.0f;
        return;
    }

    const float two_x = x + x;
    const int32_t quadrant = __float2int_rn(two_x);
    const float rounded_two_x = __int2float_rn(quadrant);
    const bool is_integer = (truncf(x) == x);
    const float reduced = fmaf(-rounded_two_x, 0.5f, x);
    const float reduced2 = reduced * reduced;

    float sin_poly = fmaf(reduced2, -0.59248024225234985352f, 2.550144195556640625f);
    sin_poly = fmaf(reduced2, sin_poly, -5.1677198410034179688f);
    const float reduced3 = reduced * reduced2;
    float sin_value = fmaf(sin_poly, reduced3, reduced * pi_hi);

    float cos_value = fmaf(reduced2, 0.22686031460762023926f, -1.334560394287109375f);
    cos_value = fmaf(reduced2, cos_value, 4.0586924552917480469f);
    cos_value = fmaf(reduced2, cos_value, -4.9348020553588867188f);
    cos_value = fmaf(reduced2, cos_value, 1.0f);

    float selected_sin = ((quadrant & 1) != 0) ? cos_value : sin_value;
    float selected_cos = ((quadrant & 1) != 0) ? sin_value : cos_value;
    if ((quadrant & 2) != 0) {
        selected_sin = -selected_sin;
    }
    if (((quadrant + 1) & 2) != 0) {
        selected_cos = -selected_cos;
    }
    if (is_integer) {
        selected_sin = 0.0f * x;
    }

    *sin_result = selected_sin;
    *cos_result = selected_cos;
}

#define __INTERNAL_SINCOSPIF(x, s, c)                             \
    do {                                                          \
        float sin_result = 0.0f;                                  \
        float cos_result = 0.0f;                                  \
        __internal_sincospif_core((x), &sin_result, &cos_result); \
        *(s) = sin_result;                                        \
        *(c) = cos_result;                                        \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, float* s, float* c) { __INTERNAL_SINCOSPIF(x, s, c); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __ubuf__ float* s, float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __ubuf__ float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __ubuf__ float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __gm__ float* s, float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __gm__ float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __gm__ float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSPIF(x, s, c);
}
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_exp2f_reconstruct(float x_hi, float x_lo)
{
    constexpr float ln2 = 0.69314718246459960938f;
    constexpr float overflow_abs_bound = 152.0f;
    constexpr int32_t fp32_exponent_shift = 23;

    const float rounded_x = roundf(x_hi);
    const float exp2_fraction = (x_hi - rounded_x) + x_lo;
    const int32_t exp2_exponent = __float2int_rz(rounded_x);

    float exp_poly = fmaf(exp2_fraction, 0.00015239251661114395f, 0.0013391353422775864601f);
    exp_poly = fmaf(exp2_fraction, exp_poly, 0.0096188392490148544312f);
    exp_poly = fmaf(exp2_fraction, exp_poly, 0.055503588169813156128f);
    exp_poly = fmaf(exp2_fraction, exp_poly, 0.24022644758224487305f);
    exp_poly = fmaf(exp2_fraction, exp_poly, ln2);
    exp_poly = fmaf(exp2_fraction, exp_poly, 1.0f);

    const bool rounded_x_is_positive = rounded_x > 0.0f;
    const uint32_t scale_hi_bits = rounded_x_is_positive ? 0x7F000000U : 0x02000000U;
    const uint32_t scale_adjust = rounded_x_is_positive ? 0U : 0x83000000U;
    const uint32_t scale_lo_bits = (static_cast<uint32_t>(exp2_exponent) << fp32_exponent_shift) - scale_adjust;
    float output = exp_poly * __uint_as_float(scale_hi_bits);
    output = output * __uint_as_float(scale_lo_bits);

    if (fabsf(x_hi) > overflow_abs_bound) {
        output = x_hi >= 0.0f ? ASCRT_INF_F : 0.0f;
    }
    return output;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_exp2f_finite(float x)
{
    return __internal_exp2f_reconstruct(x, 0.0f);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bool __internal_is_odd_integer_f32(float value)
{
    const float half_trunc = truncf(value * 0.5f);
    const float remainder = value - (half_trunc + half_trunc);
    return fabsf(remainder) == 1.0f;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float powf(float x, float y)
{
    const float abs_x = fabsf(x);
    const float abs_y = fabsf(y);

    float output = ASCRT_NAN_F;

    if (!isnan(x) && !isnan(y) && abs_x != 0.0f && abs_x != ASCRT_INF_F && abs_y != ASCRT_INF_F) {
        float log2_hi = 0.0f;
        float log2_lo = 0.0f;
        __internal_log2f_positive_finite(abs_x, log2_hi, log2_lo);

        const float log2_abs_x = log2_hi + log2_lo;
        const float product_hi = log2_abs_x * y;
        float product_lo = fmaf(log2_abs_x, y, -product_hi);
        product_lo = fmaf(log2_lo - (log2_abs_x - log2_hi), y, product_lo);

        output = __internal_exp2f_reconstruct(product_hi, product_lo);

        if (x < 0.0f) {
            if (floorf(y) != y) {
                output = ASCRT_NAN_F;
            } else if (__internal_is_odd_integer_f32(y)) {
                output = -output;
            }
        }
    }

    if (y == 0.0f || x == 1.0f) {
        output = 1.0f;
    } else if (x == 0.0f || abs_x == ASCRT_INF_F) {
        if (isnan(y)) {
            output = x + y;
        } else {
            float special_output = x + x;
            if (y < 0.0f) {
                special_output = __internal_with_sign_bit(abs_x == ASCRT_INF_F ? 0.0f : ASCRT_INF_F, x);
            }
            output = __internal_is_odd_integer_f32(y) ? special_output : fabsf(special_output);
        }
    } else if (abs_y == ASCRT_INF_F) {
        if (isnan(x)) {
            output = x + y;
        } else if (x == -1.0f) {
            output = 1.0f;
        } else {
            const bool result_is_inf = (abs_x > 1.0f && y > 0.0f) || (abs_x < 1.0f && y < 0.0f);
            output = result_is_inf ? ASCRT_INF_F : 0.0f;
        }
    } else if (isnan(x) || isnan(y)) {
        output = x + y;
    }

    return output;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float exp2f(float x)
{
    if (isnan(x)) {
        return x + x;
    }
    if (x == ASCRT_INF_F) {
        return ASCRT_INF_F;
    }
    if (x == -ASCRT_INF_F) {
        return 0.0f;
    }
    return __internal_exp2f_finite(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float exp10f(float x)
{
    if (__isnan(x)) {
        return x;
    }
    if (__isinf(x)) {
        return x > 0.0f ? x : 0.0f;
    }

    float t = __fma(x, 0.0131822545081377029418945f, 0.5f); // 0x3c57fa62
    if (t < 0.0f) {
        t = 0.0f;
    } else if (t > 1.0f) {
        t = 1.0f;
    }

    float biased = __floorf(t * 252.0f) + 12582913.0f; // 252: 0x437c0000
    float k = biased - 12583039.0f;
    uint32_t scale_bits = reinterpret_cast<uint32_t&>(biased) << 23;
    float scale = reinterpret_cast<float&>(scale_bits);

    float r = __fma(x, 3.3219280242919921875f, -k); // 0x40549a78
    r = __fma(x, 7.0595369550119357882e-08f, r);    // 0x33979a37
    return scale * __powf(2.0f, r);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float expm1f(float x)
{
    if (x == 0.0f) {
        return x;
    }
    if (__isnan(x)) {
        return x;
    }
    if (__isinf(x)) {
        return x > 0.0f ? x : -1.0f;
    }

    float ax = __fabsf(x);
    float z = x;
    if (ax > 88.72283935546875f /* 0x42b17218 */) {
        z = x > 0.0f ? 88.72283935546875f : -88.72283935546875f;
    }

    float biased = __fma(z, 1.44269502162933349609375f /* 0x3fb8aa3b */, 12583039.0f);
    float k = biased - 12583039.0f;
    float r = __fma(-k, 0.69314712285995483398f /* 0x3f317217 */, z);
    r = __fma(-k, 5.7699988786907852045e-08f /* 0x3377d1cf */, r);

    float p = __fma(r, 0.00138624827377498149871826f /* 0x3ab5b2c6 */, 0.0083664264529943466187f /* 0x3c091356 */);
    p = __fma(r, p, 0.041665729135274887085f /* 0x3d2aa9af */);
    p = __fma(r, p, 0.16666544973850250244f /* 0x3e2aaa59 */);
    p = __fma(r, p, 0.50000017881393432617f /* 0x3f000003 */);
    p = r * p;
    float em1_r = __fma(r, p, r);

    uint32_t scale_bits = reinterpret_cast<uint32_t&>(biased) << 23;
    bool large_k = k >= 25.0f;
    if (large_k) {
        scale_bits -= 0x00800000U;
    }
    float scale = (k != -128.0f) ? reinterpret_cast<float&>(scale_bits) : 0.0f;
    float one_minus_scale = 1.0f - scale;
    float y = __fma(scale, em1_r, -one_minus_scale);
    if (large_k) {
        y = y + y;
    }
    return y;
}

#define __INTERNAL_FREXPF(x, exp)                        \
    do {                                                 \
        if ((x) == 0.0f || isinf(x) || isnan(x)) {       \
            *(exp) = 0;                                  \
            return (x);                                  \
        }                                                \
        uint32_t u32 = reinterpret_cast<uint32_t&>(x);   \
        int32_t exponent = u32 & 0x7f800000;             \
        int32_t f32_exp_val = exponent >> 23;            \
        uint32_t man_u32 = u32 & 0x007fffff;             \
        float f32_man_u32 = static_cast<float>(man_u32); \
        f32_man_u32 = f32_man_u32 / (1 << 23);           \
        if (f32_exp_val == 0) {                          \
            if (f32_man_u32 < 0.5f) {                    \
                while (f32_man_u32 < 0.5f) {             \
                    f32_man_u32 = f32_man_u32 * 2;       \
                    f32_exp_val--;                       \
                }                                        \
            }                                            \
        } else {                                         \
            f32_man_u32 = f32_man_u32 / 2 + 0.5f;        \
        }                                                \
        *(exp) = f32_exp_val - 126;                      \
        return copysignf(f32_man_u32, (x));              \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float frexpf(float x, int* exp) { __INTERNAL_FREXPF(x, exp); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float frexpf(float x, __ubuf__ int* exp) { __INTERNAL_FREXPF(x, exp); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float frexpf(float x, __gm__ int* exp) { __INTERNAL_FREXPF(x, exp); }
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float ldexpf(float x, int exp)
{
    if (x == 0.0f || isinf(x) || isnan(x) || exp == 0) {
        return x;
    }
    if (exp > 280) { // 280: 1e-45*(2^280) = inf
        return copysignf(ASCRT_INF_F, x);
    }
    if (exp < -280) { // -280: 3.4028234e+38*(2^-280) = 0
        return copysignf(0.0f, x);
    }
    int32_t shift = 30;
    if (exp > 0) {
        while (exp > shift) {
            x *= (1 << shift);
            exp -= shift;
        }
        x *= (1 << exp);
    } else {
        while (exp < -30) { // -30: exp < -30, move 30
            x *= 1.0f / (1 << shift);
            exp += shift;
        }
        x *= 1.0f / (1 << (-exp));
    }
    return x;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float hypotf(float x, float y)
{
    float abs_x = fabsf(x);
    float abs_y = fabsf(y);
    if (isinf(x) || isinf(y)) {
        return ASCRT_INF_F;
    }
    if (isnan(abs_x)) {
        return abs_x;
    }
    if (isnan(abs_y)) {
        return abs_y;
    }
    float a = fmaxf(abs_x, abs_y);
    float b = fminf(abs_x, abs_y);
    if (b == 0.0f) {
        return a;
    }
    float r = b / a;
    return a * sqrtf(fmaf(r, r, 1.0f));
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rhypotf(float x, float y)
{
    constexpr uint32_t scale_base = 0x7E800000U;
    constexpr uint32_t scale_mask = 0xFE000000U;

    const float abs_x = fabsf(x);
    const float abs_y = fabsf(y);
    const uint32_t abs_x_bits = __float_as_uint(abs_x);
    const uint32_t abs_y_bits = __float_as_uint(abs_y);
    const uint32_t min_bits = abs_x_bits < abs_y_bits ? abs_x_bits : abs_y_bits;
    const uint32_t max_bits = abs_x_bits > abs_y_bits ? abs_x_bits : abs_y_bits;
    const float min_abs = __uint_as_float(min_bits);
    const float max_abs = __uint_as_float(max_bits);

    const float scale = __uint_as_float(scale_base - (max_bits & scale_mask));
    const float scaled_min = min_abs * scale;
    const float scaled_max = max_abs * scale;
    const float square_sum = fmaf(scaled_max, scaled_max, scaled_min * scaled_min);
    const float result = rsqrtf(square_sum) * scale;

    return min_abs == ASCRT_INF_F ? 0.0f : result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float norm3df(float a, float b, float c)
{
    if (isinf(a) || isinf(b) || isinf(c)) {
        return ASCRT_INF_F;
    }
    if (isnan(a) || isnan(b) || isnan(c)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float m = fmaxf(fabsf(a), fabsf(b));
    m = fmaxf(m, fabsf(c));
    if (m == 0.0f) {
        return 0.0f;
    }
    float r = 0.0f;
    r = fmaf((a / m), (a / m), r);
    r = fmaf((b / m), (b / m), r);
    r = fmaf((c / m), (c / m), r);
    return m * sqrtf(r);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnorm3df(float a, float b, float c)
{
    constexpr uint32_t scale_base = 0x7E800000U;
    constexpr uint32_t scale_mask = 0xFE000000U;
    constexpr float min_normal = 1.175494350822287508e-38f;
    constexpr float subnormal_scale = 16777216.0f;
    constexpr float rsqrt_subnormal_scale = 4096.0f;

    const float abs_a = fabsf(a);
    const float abs_b = fabsf(b);
    const float abs_c = fabsf(c);
    if (isinf(abs_a) || isinf(abs_b) || isinf(abs_c)) {
        return 0.0f;
    }
    if (isnan(abs_a) || isnan(abs_b) || isnan(abs_c)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    const float max_ab = fmaxf(abs_a, abs_b);
    const float min_ab = fminf(abs_a, abs_b);
    const float max_abs = fmaxf(abs_c, max_ab);
    const float mid_abs = fminf(abs_c, max_ab);
    const uint32_t max_bits = __float_as_uint(max_abs);
    const float scale = __uint_as_float(scale_base - (max_bits & scale_mask));

    const float scaled_min = min_ab * scale;
    const float scaled_mid = mid_abs * scale;
    const float scaled_max = max_abs * scale;
    float square_sum = scaled_mid * scaled_mid;
    square_sum = fmaf(scaled_min, scaled_min, square_sum);
    square_sum = fmaf(scaled_max, scaled_max, square_sum);

    const bool is_subnormal_square_sum = fabsf(square_sum) < min_normal;
    if (is_subnormal_square_sum) {
        square_sum *= subnormal_scale;
    }
    if (square_sum == 0.0f) {
        return ASCRT_INF_F;
    }

    float inv_norm = rsqrtf(square_sum);
    inv_norm = inv_norm * fmaf(-0.5f * square_sum, inv_norm * inv_norm, 1.5f);
    if (is_subnormal_square_sum) {
        inv_norm *= rsqrt_subnormal_scale;
    }
    return scale * inv_norm;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float norm4df(float a, float b, float c, float d)
{
    if (isinf(a) || isinf(b) || isinf(c) || isinf(d)) {
        return ASCRT_INF_F;
    }
    if (isnan(a) || isnan(b) || isnan(c) || isnan(d)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float m = fmaxf(fabsf(a), fabsf(b));
    m = fmaxf(m, fabsf(c));
    m = fmaxf(m, fabsf(d));
    if (m == 0.0f) {
        return 0.0f;
    }
    float r = 0.0f;
    r = fmaf((a / m), (a / m), r);
    r = fmaf((b / m), (b / m), r);
    r = fmaf((c / m), (c / m), r);
    r = fmaf((d / m), (d / m), r);
    return m * sqrtf(r);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnorm4df(float a, float b, float c, float d)
{
    constexpr uint32_t scale_base = 0x7E800000U;
    constexpr uint32_t scale_mask = 0xFE000000U;
    constexpr float min_normal = 1.175494350822287508e-38f;
    constexpr float subnormal_scale = 16777216.0f;
    constexpr float rsqrt_subnormal_scale = 4096.0f;

    const float abs_a = fabsf(a);
    const float abs_b = fabsf(b);
    const float abs_c = fabsf(c);
    const float abs_d = fabsf(d);
    if (isinf(abs_a) || isinf(abs_b) || isinf(abs_c) || isinf(abs_d)) {
        return 0.0f;
    }
    if (isnan(abs_a) || isnan(abs_b) || isnan(abs_c) || isnan(abs_d)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    const float max_ab = fmaxf(abs_a, abs_b);
    const float min_ab = fminf(abs_a, abs_b);
    const float max_abc = fmaxf(abs_c, max_ab);
    const float max_abs = fmaxf(abs_d, max_abc);
    const float second_abs = fminf(abs_d, max_abc);
    const float third_abs = fminf(abs_c, max_ab);
    const uint32_t max_bits = __float_as_uint(max_abs);
    const float scale = __uint_as_float(scale_base - (max_bits & scale_mask));

    const float scaled_min = min_ab * scale;
    const float scaled_second = second_abs * scale;
    const float scaled_third = third_abs * scale;
    const float scaled_max = max_abs * scale;
    float square_sum = scaled_second * scaled_second;
    square_sum = fmaf(scaled_third, scaled_third, square_sum);
    square_sum = fmaf(scaled_min, scaled_min, square_sum);
    square_sum = fmaf(scaled_max, scaled_max, square_sum);

    const bool is_subnormal_square_sum = fabsf(square_sum) < min_normal;
    if (is_subnormal_square_sum) {
        square_sum *= subnormal_scale;
    }
    if (square_sum == 0.0f) {
        return ASCRT_INF_F;
    }

    float inv_norm = rsqrtf(square_sum);
    inv_norm = inv_norm * fmaf(-0.5f * square_sum, inv_norm * inv_norm, 1.5f);
    if (is_subnormal_square_sum) {
        inv_norm *= rsqrt_subnormal_scale;
    }
    return scale * inv_norm;
}

#define __INTERNAL_NORMF(n, a)                                          \
    do {                                                                \
        if ((n) <= 0) {                                                 \
            return fabsf((a)[0]);                                       \
        }                                                               \
        float m = 0;                                                    \
        int remainder = (n) & 3;                                        \
        int end = (n) - remainder;                                      \
        if ((n) > 3) {                                                  \
            for (int i = 0; i < end; i += 4) {                          \
                float a0 = (a)[i];                                      \
                float a1 = (a)[i + 1];                                  \
                float a2 = (a)[i + 2];                                  \
                float a3 = (a)[i + 3];                                  \
                if (isinf(a0) || isinf(a1) || isinf(a2) || isinf(a3)) { \
                    return ASCRT_INF_F;                                 \
                }                                                       \
                m = __fmaxf(m, fabsf(a0));                              \
                m = __fmaxf(m, fabsf(a1));                              \
                m = __fmaxf(m, fabsf(a2));                              \
                m = __fmaxf(m, fabsf(a3));                              \
            }                                                           \
        }                                                               \
        if (remainder != 0) {                                           \
            for (int i = end; i < n; i++) {                             \
                if (isinf((a)[i])) {                                    \
                    return ASCRT_INF_F;                                 \
                }                                                       \
                m = __fmaxf(m, fabsf((a)[i]));                          \
            }                                                           \
        }                                                               \
        if (m == 0.0f || isnan(m)) {                                    \
            return m;                                                   \
        }                                                               \
        float sum = 0.0f;                                               \
        if ((n) > 3) {                                                  \
            for (int i = 0; i < end; i += 4) {                          \
                float n0 = (a)[i] / m;                                  \
                float n1 = (a)[i + 1] / m;                              \
                float n2 = (a)[i + 2] / m;                              \
                float n3 = (a)[i + 3] / m;                              \
                sum = fmaf(n0, n0, sum);                                \
                sum = fmaf(n1, n1, sum);                                \
                sum = fmaf(n2, n2, sum);                                \
                sum = fmaf(n3, n3, sum);                                \
            }                                                           \
        }                                                               \
        if (remainder != 0) {                                           \
            for (int i = end; i < n; i++) {                             \
                float ni = (a)[i] / m;                                  \
                sum = fmaf(ni, ni, sum);                                \
            }                                                           \
        }                                                               \
        return m * sqrtf(sum);                                          \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normf(int n, float* a) { __INTERNAL_NORMF(n, a); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normf(int n, __gm__ float* a) { __INTERNAL_NORMF(n, a); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normf(int n, __ubuf__ float* a) { __INTERNAL_NORMF(n, a); }
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnormf(int n, float* a) { return 1.0f / normf(n, a); }

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnormf(int n, __ubuf__ float* a) { return 1.0f / normf(n, a); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnormf(int n, __gm__ float* a) { return 1.0f / normf(n, a); }
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log10f(float x) { return logf(x) / logf(10.0f); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log1pf(float x)
{
    if (x == 0.0f) {
        return x;
    }
    if (x == -1.0f) {
        return -ASCRT_INF_F;
    }
    if (x < -1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (x == ASCRT_INF_F) {
        return ASCRT_INF_F;
    }
    if (isnan(x)) {
        return x + x;
    }

    constexpr uint32_t log1p_reduction_mask = 0xFF800000U;
    constexpr uint32_t fp32_one_half_bits = 0x3F400000U;
    constexpr uint32_t fp32_four_bits = 0x40800000U;
    constexpr float poly_first_coeff = 0.04534861445426940918f; // 0x3d39bf78，对应反汇编中的 R5。

    // 对应 FADD.RZ R0, R4, 1；当前 SIMT C++ 未暴露专用的 RZ 浮点加法接口。
    const float one_add_x = 1.0f + x;
    // 对应 MOV/位解释：R4 保存 x 的原始位，R0 保存 1 + x 的原始位。
    const uint32_t x_bits = __float_as_uint(x);
    const uint32_t one_add_x_bits = __float_as_uint(one_add_x);

    // 对应 IADD3 + LOP3：从 1 + x 中提取指数区间分量 reduction_bits。
    const uint32_t reduction_bits = (one_add_x_bits - fp32_one_half_bits) & log1p_reduction_mask;
    // 对应 IADD3：构造归一化后的 x 位模式，以及用于区间缩放的位模式。
    const uint32_t normalized_x_bits = x_bits - reduction_bits;
    const uint32_t range_scale_bits = fp32_four_bits - reduction_bits;
    const float normalized_x = __uint_as_float(normalized_x_bits);
    const float range_scale = __uint_as_float(range_scale_bits);
    // 对应 FFMA R3, R3, 0.25, -1 与 FADD：得到多项式输入 reduced。
    const float reduced = fmaf(0.25f, range_scale, -1.0f) + normalized_x;
    // 对应 I2F 后乘以 2^-23，得到指数补偿项。
    const float exponent = static_cast<float>(static_cast<int32_t>(reduction_bits)) * 1.1920928955078125e-07f;

    // 对应连续 FFMA：按反汇编顺序计算 log1p(reduced) 的多项式部分。
    float poly = fmaf(-poly_first_coeff, reduced, 0.10546888411045074463f);
    poly = fmaf(poly, reduced, -0.13229703903198242188f);
    poly = fmaf(poly, reduced, 0.14491446316242218018f);
    poly = fmaf(poly, reduced, -0.16641564667224884033f);
    poly = fmaf(poly, reduced, 0.19988867640495300293f);
    poly = fmaf(poly, reduced, -0.25000196695327758789f);
    poly = fmaf(poly, reduced, 0.33333510160446166992f);
    poly = fmaf(poly, reduced, -0.5f);

    const float reduced_poly = reduced * poly;
    // 对应 FMUL + FFMA：reduced + reduced * reduced * poly。
    float output = fmaf(reduced_poly, reduced, reduced);
    // 对应最后一次 FFMA：加上 exponent * ln2。
    output = fmaf(exponent, 0.69314718246459960938f, output);

    return output;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline int32_t __internal_ilogbf_finite_abs(float ax)
{
    if (ax >= 1.17549435082228750797e-38f) {
        uint32_t bits = reinterpret_cast<uint32_t&>(ax);
        return static_cast<int32_t>((bits >> 23) & 0xFFU) - 127;
    }

    float scaled = ax * 8388608.0f;
    uint32_t bits = reinterpret_cast<uint32_t&>(scaled);
    return static_cast<int32_t>((bits >> 23) & 0xFFU) - 127 - 23;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float logbf(float x)
{
    if (__isnan(x)) {
        return x;
    }

    float ax = __fabsf(x);
    if (ax == 0.0f) {
        return -ASCRT_INF_F;
    }
    if (ax == ASCRT_INF_F) {
        return ASCRT_INF_F;
    }

    return static_cast<float>(__internal_ilogbf_finite_abs(ax));
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline int32_t ilogbf(float x)
{
    if (__isnan(x) || x == 0.0f) {
        return static_cast<int32_t>(0x80000000U);
    }

    float ax = __fabsf(x);
    if (ax == ASCRT_INF_F) {
        return static_cast<int32_t>(0x7FFFFFFFU);
    }

    return __internal_ilogbf_finite_abs(ax);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cbrtf(float x)
{
    if (x == 0.0f || isinf(x) || __isnan(x)) {
        return x;
    }

    float ax = __fabsf(x);
    float loga;
    if (ax < 1.175494350822287508e-38f /* 0x00800000 */) {
        float scaled = ax * 16777216.0f;
        loga = __logf(scaled) - 16.635532333438686f;
    } else {
        loga = __logf(ax);
    }

    float inv2 = __expf(loga * -0.6666666865348815918f /* 0xbf2aaaab */);
    float y = ax * inv2;
    float t = inv2 * y;
    float corr = __fma(-y, t, 1.0f) * 0.3333333432674407959f /* 0x3eaaaaab */;
    y = __fma(y, corr, y);
    return signbit(x) ? -y : y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rcbrtf(float x)
{
    if (isnan(x)) {
        return x;
    }
    if (x == 0.0f) {
        return __internal_with_sign_bit(ASCRT_INF_F, x);
    }
    if (isinf(x)) {
        return __internal_with_sign_bit(0.0f, x);
    }

    constexpr float float_min_normal = 1.175494350822287508e-38f;
    constexpr float scale_to_normal = 16777216.0f;
    constexpr float one_third = 0.3333333432674407959f;

    const float abs_x = fabsf(x);
    const bool is_normal_or_larger = abs_x >= float_min_normal;
    float log_input = is_normal_or_larger ? abs_x : abs_x * scale_to_normal;
    float exponent = log2f(log_input);
    if (!is_normal_or_larger) {
        exponent = exponent - 24.0f;
    }

    float y = exp2f(exponent * -one_third);
    const float y_square = y * y;
    const float abs_x_times_y = abs_x * y;
    const float correction = fmaf(y_square, -abs_x_times_y, 1.0f);
    y = fmaf(correction, y * one_third, y);
    if (x < 0.0f) {
        y = -y;
    }
    return y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float erff(float x)
{
    float abs_x = fabsf(x);
    float x_squared = x * x;
    if (abs_x >= 1.00296f) {
        float term = abs_x;
        const float a1 = 0.000112198715f;
        const float a2 = -0.0013275252f;
        const float a3 = 0.008396535f;
        const float a4 = -0.040246583f;
        const float a5 = 0.15950431f;
        const float a6 = 0.9129177f;
        const float a7 = 0.62906002f;

        float poly_term = fmaf(a1, term, a2);
        poly_term = fmaf(poly_term, term, a3);
        poly_term = fmaf(poly_term, term, a4);
        poly_term = fmaf(poly_term, term, a5);
        poly_term = fmaf(poly_term, term, a6);
        poly_term = fmaf(poly_term, term, a7);

        float result = fmaf(poly_term, -abs_x, -abs_x);
        float exp_result = exp2f(result);
        float adjusted_exp = 1.0f - exp_result;
        uint32_t sign_bit = *reinterpret_cast<uint32_t*>(&x) & 0x80000000;
        uint32_t final_bits = sign_bit | *reinterpret_cast<uint32_t*>(&adjusted_exp);

        return *reinterpret_cast<float*>(&final_bits);
    } else {
        float term = x_squared;
        const float a1 = 0.000084834944f;
        const float a2 = -0.00082130916f;
        const float a3 = 0.005213489f;
        const float a4 = -0.026868773f;
        const float a5 = 0.11284005f;
        const float a6 = -0.37612664f;
        const float a7 = 0.12837915f;

        float poly_term = fmaf(a1, term, a2);
        poly_term = fmaf(poly_term, term, a3);
        poly_term = fmaf(poly_term, term, a4);
        poly_term = fmaf(poly_term, term, a5);
        poly_term = fmaf(poly_term, term, a6);
        poly_term = fmaf(poly_term, term, a7);

        return fmaf(poly_term, x, x);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_poly(float abs_x)
{
    float term1 = abs_x + -4.0f;
    float term2 = abs_x + 4.0f;
    float inv_term2 = 1.0f / term2;
    float y = term1 * inv_term2;
    float z = y + 1.0f;
    float numerator = fmaf(-4.0f, z, abs_x);
    float tmp = fmaf(-y, abs_x, numerator);
    float w = fmaf(inv_term2, tmp, y);

    float poly = fmaf(0.0008912171f, w, 0.007045788f);
    poly = fmaf(poly, w, -0.015866896f);
    poly = fmaf(poly, w, 0.036429625f);
    poly = fmaf(poly, w, -0.06664343f);
    poly = fmaf(poly, w, 0.09381453f);
    poly = fmaf(poly, w, -0.10099056f);
    poly = fmaf(poly, w, 0.068094f);
    poly = fmaf(poly, w, 0.015377387f);
    poly = fmaf(poly, w, -0.13962108f);
    poly = fmaf(poly, w, 1.2329951f);
    return poly;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float erfcf(float x)
{
    float abs_x = __fabsf(x);

    float term1 = abs_x + -4.0f;
    float term2 = abs_x + 4.0f;
    float inv_term2 = 1.0f / term2;
    float y = term1 * inv_term2;
    float z = y + 1.0f;
    float numerator = __fma(-4.0f, z, abs_x);
    float tmp = __fma(-y, abs_x, numerator);
    float w = __fma(inv_term2, tmp, y);

    float poly = __fma(0.00089121708879247307777f /* 0x3a69a091 */, w, 0.0070457882247865200043f /* 0x3be6e05b */);
    poly = __fma(poly, w, -0.015866896137595176697f /* 0xbc81fb4b */);
    poly = __fma(poly, w, 0.036429625004529953003f /* 0x3d15373b */);
    poly = __fma(poly, w, -0.066643431782722473145f /* 0xbd887c5a */);
    poly = __fma(poly, w, 0.093814529478549957275f /* 0x3dc021d5 */);
    poly = __fma(poly, w, -0.10099056363105773926f /* 0xbdced424 */);
    poly = __fma(poly, w, 0.06809400022029876709f /* 0x3d8b74de */);
    poly = __fma(poly, w, 0.015377387404441833496f /* 0x3c7bf170 */);
    poly = __fma(poly, w, -0.1396210789680480957f /* 0xbe0ef8d4 */);
    poly = __fma(poly, w, 1.232995152473449707f /* 0x3f9dd2c9 */);

    float tmp2 = __fma(2.0f, abs_x, 1.0f);
    float inv_tmp2 = 1.0f / tmp2;
    float q = poly * inv_tmp2;
    float t = __fma(abs_x, q * -2.0f, poly);
    float u = t - q;
    float v = __fma(u, inv_tmp2, q);

    float x_squared = abs_x * abs_x;
    float neg_x2 = -x_squared;
    float scaled = neg_x2 * 1.4426950216293334961f /* 0x3fb8aa3b */;
    float int_part = truncf(scaled);
    float abs_part = __fabsf(int_part);
    uint32_t sign_bit = reinterpret_cast<uint32_t&>(int_part) & 0x80000000U;
    uint32_t clamped_bits = sign_bit | 0x42FC0000U;
    float clamped = reinterpret_cast<float&>(clamped_bits);
    float safe_int = (abs_part > 126.0f) ? clamped : int_part;

    float remainder = __fma(safe_int, -0.69314718246459960938f /* 0xbf317218 */, neg_x2);
    remainder = __fma(safe_int, 1.9046542121259335545e-09f /* 0x3102e308 */, remainder);
    float exponent_arg = remainder * 1.4426950216293334961f /* 0x3fb8aa3b */;
    float exponent_base = safe_int + 12583039.0f;
    uint32_t exponent_bits = reinterpret_cast<uint32_t&>(exponent_base) << 23;
    float exponent_scale = reinterpret_cast<float&>(exponent_bits);
    float exp_val = __powf(2.0f, exponent_arg) * exponent_scale;

    float term3 = __fma(-abs_x, abs_x, x_squared);
    float term4 = __fma(exp_val, term3, exp_val);
    float result = v * term4;

    if (abs_x > 10.05500030517578125f /* 0x4120e148 */) {
        result = 0.0f;
    }

    return (x < 0.0f) ? (2.0f - result) : result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float erfinvf(float x)
{
    float opposite_x = -x;
    float temp1 = fmaf(x, opposite_x, 1.0f);
    float log2_temp = log2f(temp1);
    float neg_log2 = -log2_temp;
    if (log2_temp < -8.2f) {
        float rsqrt_neg_log = rsqrtf(neg_log2);
        float poly = fmaf(-0.5899144f, rsqrt_neg_log, -0.6630042f);
        poly = fmaf(poly, rsqrt_neg_log, 1.5970111f);
        poly = fmaf(poly, rsqrt_neg_log, -0.67521554f);
        poly = fmaf(poly, rsqrt_neg_log, -0.09522479f);
        poly = fmaf(poly, rsqrt_neg_log, 0.83535343f);
        float denominator = 1.0f / rsqrt_neg_log;
        float final_term = denominator * poly;

        uint32_t sign_bit = *reinterpret_cast<uint32_t*>(&x) & 0x80000000;
        uint32_t result_bits = sign_bit | *reinterpret_cast<uint32_t*>(&final_term);
        return *reinterpret_cast<float*>(&result_bits);
    } else {
        float poly = fmaf(-2.5172708e-10f, neg_log2, 9.427429e-9f);
        poly = fmaf(poly, neg_log2, -1.2054752e-7f);
        poly = fmaf(poly, neg_log2, 2.1697005e-7f);
        poly = fmaf(poly, neg_log2, 0.0000080621484f);
        poly = fmaf(poly, neg_log2, -0.000031675492f);
        poly = fmaf(poly, neg_log2, -0.0007743631f);
        poly = fmaf(poly, neg_log2, 0.005546588f);
        poly = fmaf(poly, neg_log2, 0.16082023f);
        poly = fmaf(poly, neg_log2, 0.8862269f);
        return poly * x;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float erfcinvf(float x)
{
    float opposite_x = -x;
    float term = 2.0f + opposite_x;

    if (x <= 1.9966f && x >= 0.0034f) {
        float term2 = term * x;
        float log_term = log2f(term2);
        float neg_log = -log_term;

        float poly = fmaf(-2.5172708e-10f, neg_log, 9.427429e-9f);
        poly = fmaf(poly, neg_log, -1.2054752e-7f);
        poly = fmaf(poly, neg_log, 2.1697005e-7f);
        poly = fmaf(poly, neg_log, 0.0000080621484f);
        poly = fmaf(poly, neg_log, -0.000031675492f);
        poly = fmaf(poly, neg_log, -0.0007743631f);
        poly = fmaf(poly, neg_log, 0.005546588f);
        poly = fmaf(poly, neg_log, 0.16082023f);
        poly = fmaf(poly, neg_log, 0.8862269f);
        return fmaf(poly, opposite_x, poly);
    } else {
        bool is_gt_one = x > 1.0f;
        float term2 = is_gt_one ? term : x;
        float log_term = log2f(term2);
        float neg_log = -log_term;
        float rsqrt_log = rsqrtf(neg_log);
        float poly = fmaf(-63.113224f, rsqrt_log, 127.48469f);
        poly = fmaf(poly, rsqrt_log, -114.10568f);
        poly = fmaf(poly, rsqrt_log, 60.325786f);
        poly = fmaf(poly, rsqrt_log, -21.789892f);
        poly = fmaf(poly, rsqrt_log, 6.467409f);
        poly = fmaf(poly, rsqrt_log, -1.8329474f);
        poly = fmaf(poly, rsqrt_log, -0.030327774f);
        poly = fmaf(poly, rsqrt_log, 0.83287745f);
        float inv_rsqrt = 1.0f / rsqrt_log;
        float sign_adj = is_gt_one ? -inv_rsqrt : inv_rsqrt;
        return poly * sign_adj;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float erfcxf(float x)
{
    if (x < -9.43f) {
        return ASCRT_INF_F;
    }
    float abs_x = fabsf(x);
    if (abs_x < 10.055f) {
        float poly = __internal_cal_poly(abs_x);
        float term3 = fmaf(2.0f, abs_x, 1.0f);
        float inv_term3 = 1.0f / term3;
        float q = poly * inv_term3;
        float t = fmaf(abs_x, q * -2.0f, poly);
        float u = t - q;
        float result = fmaf(u, inv_term3, q);
        if (x > 0) {
            return result;
        }
        float x_sq = abs_x * abs_x;
        float neg_x2 = -x_sq;
        float term4 = fmaf(abs_x, abs_x, neg_x2);
        float term5 = fmaf(x_sq, 0.00572498f, 0.5f);
        term5 = fminf(term5, ASCRT_INF_F); // prevent overflow
        float term6 = fmaf(term5, 252.0f, 12582913.0f);
        float term7 = term6 - 12583039.0f;
        float neg_term7 = -term7;
        float term8 = fmaf(x_sq, 1.442695f, neg_term7);
        float term9 = fmaf(x_sq, 1.925963e-8f, term8);
        uint32_t exponent = *reinterpret_cast<uint32_t*>(&term6) << 23; // Extract exponent bits from term6
        float exponent_scale = *reinterpret_cast<float*>(&exponent);
        float term9_exp = exp2f(term9);
        float scaled_exp = term9_exp * exponent_scale;
        float exp_approx = fmaf(term9_exp, exponent_scale, scaled_exp);
        float finalexp_approx = fmaf(exp_approx, term4, exp_approx);
        bool is_inf = isinf(exp_approx);
        result = is_inf ? exp_approx : (finalexp_approx - result);
        return result;
    } else {
        float scaled_x = abs_x * 0.25f;
        float reciprocal_x = 0.25f / scaled_x;
        float w = reciprocal_x * reciprocal_x;
        float poly = fmaf(6.5625f, w, -1.875f);
        poly = fmaf(poly, w, 0.75f);
        poly = fmaf(poly, w, -0.5f);
        poly = fmaf(poly, w, 1.0f);
        float scaled_reciprocal = reciprocal_x * 0.5641896f;
        float result = scaled_reciprocal * poly;
        return result;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_compute_sinpi(float x)
{
    float double_x = x * 2;

    //  Split a float-value into integer[i] and decimal[f]
    float y0 = static_cast<float>(nearbyintf(double_x));
    int32_t i = static_cast<int32_t>(y0);
    float f = fmaf(-y0, 0.5f, x);
    float f_pi = f * 3.14159274f;
    float f_pi_square = f_pi * f_pi;

    float y = 0.0f;
    if ((i & 1) != 0) {
        //  (2k + 1 + f) * pi
        y = 2.42795795e-05f;                       // 2.42795795e-05f : 1/8!
        y = fmaf(y, f_pi_square, -0.00138878601f); // -0.001388786f   : -1/6!
        y = fmaf(y, f_pi_square, 0.0416667275f);   // 0.041666727f    : 1/4!
        y = fmaf(y, f_pi_square, -0.49999997f);    // -0.49999997f    : -1/2!
        float y2 = fmaf(f_pi_square, 1.0f, 0.0f);
        y = fmaf(y, y2, 1.0f);
    } else {
        //  (2k + f) * pi
        y = -0.000195746587f;                     // -0.000195746587f : 1/7!
        y = fmaf(y, f_pi_square, 0.00833270326f); // 0.008332703f     : 1/5!
        y = fmaf(y, f_pi_square, -0.166666627f);  // -0.16666662f     : 1/3!
        float y2 = fmaf(f_pi_square, f_pi, 0.0f);
        y = fmaf(y, y2, f_pi);
    }

    if ((i & 2) != 0) { //  2: sin(pi+x) = -sin(x)
        y = fmaf(y, -1.0f, 0.0f);
    }
    return y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_compute_ln(float x)
{
    float offset = 0;
    // sub-norm - > norm
    if (x < 1.17549435e-38f) {
        offset = -23;    //  -23 : sub-norm   - > norm
        x = x * 8388608; //  8388608 : 2^23
    }
    uint32_t u32 = *reinterpret_cast<uint32_t*>(&x);
    int32_t y1 = (u32 - 1059760811) & -8388608; //  -8388608 : -2^23
    int32_t y2 = u32 - y1;
    float mantissa = *reinterpret_cast<float*>(&y2);
    mantissa = mantissa - 1.0f;
    float exponent = fmaf(static_cast<float>(y1), 1.1920929e-07f, offset); // 1.1920929e-07: 2^-23

    //  ln(mantissa)
    float y = -0.130188569f;              // -0.130188569f   :   Coefficient of O(10)
    y = fmaf(y, mantissa, 0.140846103f);  //  0.140846103f    :   Coefficient of O(9)
    y = fmaf(y, mantissa, -0.121486276f); //  -0.121486276f  :   Coefficient of O(8)
    y = fmaf(y, mantissa, 0.139806107f);  //  0.139806107f    :   Coefficient of O(7)
    y = fmaf(y, mantissa, -0.166842356f); //  -0.166842356f  :   -1/6
    y = fmaf(y, mantissa, 0.200122997f);  //  0.200122997f     :   1/5
    y = fmaf(y, mantissa, -0.249996692f); //  -0.249996692f  :   -1/4
    y = fmaf(y, mantissa, 0.333331823f);  //  0.333331823f   :   1/3
    y = fmaf(y, mantissa, -0.5f);         //  -0.5f         :   -1/2
    y = mantissa * y;
    y = fmaf(y, mantissa, mantissa);

    // ln(mantissa) + exponent*ln(2)
    y = fmaf(exponent, 0.693147182f, y); // 0.693147182f     :   ln2

    if (u32 >= ASCRT_INT32_INF_S || x == 0) {
        y = fmaf(x, ASCRT_INF_F, ASCRT_INF_F);
    }
    return y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_euler_gamma_function(float x)
{
    float reduced = x;
    if (x != 0.0f) {
        reduced = x - nearbyintf(x);
    }

    float poly = fmaf(reduced, -0.0010728656779974699020f, 0.0071110534481704235077f);
    poly = fmaf(poly, reduced, -0.0096437186002731323242f);
    poly = fmaf(poly, reduced, -0.042180188000202178955f);
    poly = fmaf(poly, reduced, 0.16654090583324432373f);
    poly = fmaf(poly, reduced, -0.04200365021824836731f);
    poly = fmaf(poly, reduced, -0.65587818622589111328f);
    poly = fmaf(poly, reduced, 0.57721567153930664062f);

    float scale = x < -0.5f ? x : 1.0f;
    if (x <= 0.5f) {
        scale = scale * reduced;
    }

    float denominator = fmaf(scale * poly, reduced, scale);
    if (fabsf(denominator) < 1.175494350822287508e-38f) {
        int32_t e = 0;
        float m = frexpf(denominator, &e);
        return ldexpf(1.0f / m, 0 - e);
    }
    return 1.0f / denominator;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_abs_x(float x)
{
    constexpr float tgamma_stirling_bound = 41.09999847412109375f;
    return fabsf(x) > tgamma_stirling_bound ? tgamma_stirling_bound : fabsf(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y3(float ln_mantissa)
{
    float y3 = 0.000656886259f;
    y3 = fmaf(y3, ln_mantissa * ln_mantissa, 0.00321816537f); // 0.00321816537f : Coefficient of O(3)
    y3 = fmaf(y3, ln_mantissa * ln_mantissa, 0.0180337187f);  // 0.0180337187f : Coefficient of O(2)
    y3 = fmaf(y3, ln_mantissa * ln_mantissa, 0.120224588f);   // 0.120224588f : Coefficient of O(1)
    y3 = fmaf(y3, ln_mantissa * ln_mantissa, 0.0f);
    return y3;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y6(float abs_x)
{
    float recabs_x = 1.0f / abs_x;
    float y6 = 0.000068413915f;                //  0.000068413915f    : Coefficient of O(8)
    y6 = fmaf(y6, recabs_x, -0.000050603266f); //  -0.000050603266f   : Coefficient of O(7)
    y6 = fmaf(y6, recabs_x, -0.00042276637f);  //  -0.00042276637f    : Coefficient of O(6)
    y6 = fmaf(y6, recabs_x, 0.0009921414f);    //  0.0009921414f      : Coefficient of O(5)
    y6 = fmaf(y6, recabs_x, -0.00027855476f);  //  -0.00027855476f    : -571/2488320
    y6 = fmaf(y6, recabs_x, -0.002674901f);    //  -0.002674901f      : -139/51840
    y6 = fmaf(y6, recabs_x, 0.0034718033f);    //  0.0034718033f      : 1/288
    y6 = fmaf(y6, recabs_x, 0.08333334f);      //  0.08333334f        : 1/12
    y6 = fmaf(y6, recabs_x, 0.0f);
    return y6;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y(float abs_x, float y_diff, float y5, float y7)
{
    //  y5 / [sin(pi*x) * 2 * x * y6]
    float y = fmaf(y5, y7, -y5 * y7 * y_diff * y7);
    y = y * 0.5f;
    if (abs_x > 33) {           //  33 : threshold
        y = y * 3.5527136e-15f; // 3.5527136e-15 : 2^-48
    }
    return y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_stirling_and_euler_reflection(float x)
{
    constexpr float log2e_hi = 1.4426950216293334961f;
    constexpr float log2e_lo = 1.925963033500011079e-08f;
    constexpr float sqrt_two_pi = 2.5066282749176025391f;
    constexpr float fp32_min_normal = 1.175494350822287508e-38f;
    constexpr float subnormal_scale = 16777216.0f;
    constexpr float subnormal_exponent_fix = -24.0f;
    constexpr float log_exponent_scale = 1.1920928955078125e-07f;
    constexpr uint32_t log_reduction_mask = 0xFF800000U;
    constexpr uint32_t sqrt_half_bits = 0x3F3504F3U;
    constexpr float large_reflection_bound = 33.0f;
    constexpr float negative_exponent_offset = 48.0f;
    constexpr float exp2_overflow_abs_bound = 152.0f;

    const float abs_x = __internal_cal_abs_x(x);

    //  Split the Stirling's Approximation into the main term and the remainder term
    //  Calculate the main term: sqrt(2*pi*x) * (x/e)^x * x^(-1)
    //  sqrt(2*pi*x) * (x/e)^(x-1)
    //  = sqrt(2*pi) * x^(0.5)*(x/e)^x*x^(-1)
    //  = sqrt(2*pi) * (x/e)^x*x^(-0.5)
    //  = sqrt(2*pi) * [x^(x-0.5)/e^x]
    //  = sqrt(2*pi) * 2^log[x^(x-0.5)/e^x]
    //  = sqrt(2*pi) * 2^[(x-0.5)log(x) - xlog(e)]
    //      note:
    //          let y0 = [(x-0.5)log(x) - xlog(e)], split float-value[y0] into integer[i] and decimal[f]
    //          let y01 = (x-0.5)log(x), y02 = xlog(e), then y0 = y01 - y02
    //  = sqrt(2*pi) * 2^[i+f]
    //  = sqrt(2*pi) * 2^f * 2^i
    const bool is_normal_x = abs_x >= fp32_min_normal;
    const float log_input = is_normal_x ? abs_x : abs_x * subnormal_scale;
    const float exponent_base = is_normal_x ? 0.0f : subnormal_exponent_fix;
    const uint32_t log_input_bits = __float_as_uint(log_input);

    const uint32_t reduction_bits = (log_input_bits - sqrt_half_bits) & log_reduction_mask;
    const float mantissa = __uint_as_float(log_input_bits - reduction_bits);
    const float exponent_part =
        fmaf(__int2float_rn(static_cast<int32_t>(reduction_bits)), log_exponent_scale, exponent_base);

    const float mantissa_minus_one = mantissa - 1.0f;
    const float mantissa_plus_one = mantissa + 1.0f;
    const float reciprocal = 1.0f / mantissa_plus_one;
    const float reduced_hi = reciprocal * (mantissa_minus_one + mantissa_minus_one);
    const float reduced_square = reduced_hi * reduced_hi;

    float log_poly = fmaf(reduced_square, 0.0006568862590938807f, 0.0032181653659790754318f);
    log_poly = fmaf(reduced_square, log_poly, 0.018033718690276145935f);
    log_poly = fmaf(reduced_square, log_poly, 0.12022458761930465698f);
    log_poly = reduced_square * log_poly;

    const float log_x_rounded = fmaf(reduced_hi, log2e_hi, exponent_part);
    float reduced_err = mantissa_minus_one - reduced_hi;
    reduced_err = fmaf(mantissa_minus_one, -reduced_hi, reduced_err + reduced_err);
    const float reduced_lo = reciprocal * reduced_err;

    float log_x_diff = exponent_part - log_x_rounded;
    log_x_diff = fmaf(reduced_hi, log2e_hi, log_x_diff);
    log_x_diff = fmaf(reduced_lo, log2e_hi, log_x_diff);
    log_x_diff = fmaf(reduced_hi, log2e_lo, log_x_diff);
    log_x_diff = fmaf(reduced_lo, log_poly * 3.0f, log_x_diff);
    log_x_diff = fmaf(reduced_hi, log_poly, log_x_diff);

    //  Calculates the exponent of Stirling's approximation
    float log_x = log_x_rounded + log_x_diff;
    const float log_x_round_err = log_x_diff - (log_x - log_x_rounded);
    const float abs_x_minus_half = abs_x - 0.5f;
    const float log_mul_hi = log_x * abs_x_minus_half;
    const float exp_mul_hi = abs_x * log2e_hi;
    float exponent_hi = log_mul_hi - exp_mul_hi;

    float log_mul_err = fmaf(log_x, abs_x_minus_half, -log_mul_hi);
    log_mul_err = fmaf(log_x_round_err, abs_x_minus_half, log_mul_err);
    float exp_mul_err = fmaf(abs_x, log2e_hi, -exp_mul_hi);
    exp_mul_err = fmaf(abs_x, log2e_lo, exp_mul_err);

    const float exp_sum_hi = exp_mul_hi + exponent_hi;
    const float exponent_round_err = exponent_hi - exp_sum_hi;
    const float log_mul_round_err = log_mul_hi - exp_sum_hi;
    const float exp_mul_round_err = -exp_mul_hi - exponent_round_err;
    float exponent_lo = log_mul_round_err + exp_mul_round_err;
    exponent_lo = exponent_lo + (log_mul_err - exp_mul_err);

    const float offset = abs_x > large_reflection_bound ? negative_exponent_offset : 0.0f;
    if (x < 0.0f) {
        exponent_hi = offset - exponent_hi;
        exponent_lo = -exponent_lo;
    }

    // 2^exponent * sqrt(2*pi)
    const float rounded_exponent = roundf(exponent_hi);
    float exp2_fraction = exponent_hi - rounded_exponent;
    exp2_fraction = exp2_fraction + exponent_lo;
    const int32_t exp2_exponent = __float2int_rz(rounded_exponent);

    float exp2_poly = fmaf(exp2_fraction, 0.00015239251661114395f, 0.0013391353422775864601f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.0096188392490148544312f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.055503588169813156128f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.24022644758224487305f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.69314718246459960938f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 1.0f);

    const bool exponent_is_positive = rounded_exponent > 0.0f;
    const uint32_t scale_hi_bits = exponent_is_positive ? 0x7F000000U : 0x02000000U;
    const uint32_t scale_adjust = exponent_is_positive ? 0U : 0x83000000U;
    const uint32_t scale_lo_bits = (static_cast<uint32_t>(exp2_exponent) << 23U) - scale_adjust;
    float exp2_value = exp2_poly * __uint_as_float(scale_hi_bits);
    exp2_value = exp2_value * __uint_as_float(scale_lo_bits);
    if (fabsf(exponent_hi) > exp2_overflow_abs_bound) {
        exp2_value = exponent_hi >= 0.0f ? ASCRT_INF_F : 0.0f;
    }
    const float y5 = exp2_value * sqrt_two_pi;

    //  Calculate Stirling's approximation remainder minus 1
    //  y6 = {[1 + 1/(12*x) + 1/(288*x^2) - 139/(51840*x^3) - 571/(2488320*x^4)] - 1}*x^-1
    float y6 = __internal_cal_y6(abs_x);
    if (x > 0.0f) {
        //  y5 * (1.0f + y6)
        return fmaf(y5, y6, y5);
    } else {
        //  According Euler's Reflection Formula
        //  Gamma(x)Gamma(1-x)=PI/sin(pi*x)  ,  x<0
        //  Gamma(x)Gamma(-x)(-x)=PI/sin(pi*x)
        //  Gamma(x)
        //  = pi / {sin(pi*x) * Gamma(-x) * (-x)}
        //  = pi / {sin(pi*x) * Gamma(|x|) * (|x|)}
        //  = pi / {sin(pi*x) * sqrt(2*pi*|x|) * (|x|/e)^|x|*y6*(x)}
        //  = sqrt(2*pi*|x|) / {sin(pi*|x|) * 2 * |x| * (|x|/e)^|x|*y6*(|x|)}
        //  = {sqrt(2*pi*|x|) * (|x|/e)^(x)} / {sin(pi*|x|) * 2* |x| * y6 * (|x|)}
        //  = {[sqrt(2*pi*|x|) * (|x|/e)^(x)]/x} / {sin(pi*|x|) * 2 * x * y6}
        //  = y5 / {sin(pi*|x|) * 2 * x * y6}
        //  = y5 / {sin(pi*|x|) * x * y6} * 0.5

        //  Remaining items of Stirling's Approximation
        y6 = (y6 + 1);

        //  sin(pi*|x|)
        const float two_abs_x = abs_x + abs_x;
        const int32_t sinpi_quadrant = __float2int_rn(two_abs_x);
        const float rounded_two_abs_x = __int2float_rn(sinpi_quadrant);
        const bool use_cos_poly = ((sinpi_quadrant & 1) != 0);
        float reduced_pi_arg = fmaf(rounded_two_abs_x, -0.5f, abs_x);
        const float reduced_pi_arg_square = reduced_pi_arg * reduced_pi_arg;
        float sinpi_poly = 0.0f;
        if (use_cos_poly) {
            sinpi_poly = fmaf(reduced_pi_arg_square, 0.22686031460762023926f, -1.334560394287109375f);
            sinpi_poly = fmaf(reduced_pi_arg_square, sinpi_poly, 4.0586924552917480469f);
            sinpi_poly = fmaf(reduced_pi_arg_square, sinpi_poly, -4.9348020553588867188f);
            sinpi_poly = fmaf(reduced_pi_arg_square, sinpi_poly, 1.0f);
        } else {
            sinpi_poly = fmaf(reduced_pi_arg_square, -0.59248024225234985352f, 2.550144195556640625f);
            sinpi_poly = fmaf(reduced_pi_arg_square, sinpi_poly, -5.1677198410034179688f);
            sinpi_poly =
                fmaf(sinpi_poly, reduced_pi_arg * reduced_pi_arg_square, reduced_pi_arg * 3.1415927410125732422f);
        }
        float sinpi = ((sinpi_quadrant & 2) != 0) ? -sinpi_poly : sinpi_poly;

        //  (y6 * x * sinpi)'s Error value
        float y_diff = fmaf(y6 * x, sinpi, -y6 * x * sinpi);
        float y7 = 1 / (y6 * x * sinpi);

        return __internal_cal_y(abs_x, y_diff, y5, y7);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tgammaf(float x)
{
    constexpr float tgamma_stirling_bound = 41.09999847412109375f;

    if (isnan(x)) {
        return x;
    }
    if (x == ASCRT_INF_F) {
        return ASCRT_INF_F;
    }
    if (x == -ASCRT_INF_F) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (x == 0.0f) {
        return 1.0f / x;
    }

    float result = fabsf(x) < 1.5f ? __internal_euler_gamma_function(x) : __internal_stirling_and_euler_reflection(x);

    const float truncated_x = truncf(x);
    if (truncated_x == x) {
        return x < 0.0f ? ASCRT_INF_F / ASCRT_INF_F : result;
    }

    if (x < -tgamma_stirling_bound && truncated_x != x) {
        const int32_t truncated_int = __float2int_rz(truncated_x);
        if ((truncated_int & 1) != 0) {
            result = 0.0f;
        }
    }
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y0(float abs_x)
{
    //  1/[gamma(x)] = 1/[gamma(x+1) * x]
    float y0 = 0.0035875155f;             //  0.0035875155f    :   Coefficient of O(7)
    y0 = fmaf(y0, abs_x, -0.0054712854f); //  -0.0054712854f   :   Coefficient of O(6)
    y0 = fmaf(y0, abs_x, -0.044627126f);  //  -0.044627126f    :   Coefficient of O(5)
    y0 = fmaf(y0, abs_x, 0.1673177f);     //  0.1673177f       :   Coefficient of O(4)
    y0 = fmaf(y0, abs_x, -0.04213598f);   //  -0.04213598f     :   Coefficient of O(3)
    y0 = fmaf(y0, abs_x, -0.6558673f);    //  -0.6558673f      :   Coefficient of O(2)
    y0 = fmaf(y0, abs_x, 0.5772154f);     //  0.5772154f       :   Euler-Mascheroni constant
    y0 = fmaf(y0, abs_x, 0.0f);
    y0 = fmaf(y0, abs_x, abs_x);
    return y0;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_result_case1(float abs_x)
{
    //  log[gamma(x)]
    //  = −γ(x-1) + ∑(n=2 to inf)[(-1)^n*h(n)/n](x-1)^n
    //  = γ(1-x) + ∑(n=2 to inf)[h(n)/n](-x+1)^n
    float one_minus_x = 1.0f - abs_x;
    float result = 0.045882664f;                      //  0.045882664f    :  Coefficient of O(11)
    result = fmaf(result, one_minus_x, 0.10373967f);  //  0.10373967f     :   Coefficient of O(10)
    result = fmaf(result, one_minus_x, 0.122803635f); //  0.122803635f    :   Coefficient of O(9)
    result = fmaf(result, one_minus_x, 0.12752421f);  //  0.12752421f     :   Coefficient of O(8)
    result = fmaf(result, one_minus_x, 0.14321668f);  //  0.14321668f     :   Coefficient of O(7)
    result = fmaf(result, one_minus_x, 0.16934357f);  //  0.16934357f     :   Coefficient of O(6)
    result = fmaf(result, one_minus_x, 0.20740793f);  //  0.20740793f     :   Coefficient of O(5)
    result = fmaf(result, one_minus_x, 0.2705875f);   //  0.2705875f      :   pi^4/360
    result = fmaf(result, one_minus_x, 0.40068542f);  //  0.40068542f     :   1.20/3
    result = fmaf(result, one_minus_x, 0.82246696f);  //  0.82246696f     :   (pi^2)/12
    result = fmaf(result, one_minus_x, 0.5772157f);   //  0.5772157f      :   Euler-Mascheroni constant
    result = fmaf(result, one_minus_x, 0.0f);
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_result_case2(float abs_x)
{
    //  log[gamma(x)]
    //  = (1−γ)(x-2) + [(pi^2-6)/12](x-2)^2 + O(3)
    float x_minus_two = abs_x - 2.0f;
    float result = 0.0000495984932f;                     //  -0.000049598493f    :   Coefficient of O(10)
    result = fmaf(result, x_minus_two, -0.00022089484f); //  -0.000220894843f    :   Coefficient of O(9)
    result = fmaf(result, x_minus_two, 0.000541314250f); //   0.00054131424f    :   Coefficient of O(8)
    result = fmaf(result, x_minus_two, -0.00120451697f); //  -0.001204517f      :   Coefficient of O(7)
    result = fmaf(result, x_minus_two, 0.00288425176f);  //  0.0028842517f      :   Coefficient of O(6)
    result = fmaf(result, x_minus_two, -0.00738275796f); //  -0.007382758f      :   Coefficient of O(5)
    result = fmaf(result, x_minus_two, 0.0205813199f);   //  0.02058132f        :   Coefficient of O(4)
    result = fmaf(result, x_minus_two, -0.0673524886f);  //  -0.06735249f       :   Coefficient of O(3)
    result = fmaf(result, x_minus_two, 0.322467029f);    //  0.32246702f        :   (pi^2-6)/12
    result = fmaf(result, x_minus_two, 0.42278432f);     //  0.42278432f        :   1-γ
    result = fmaf(result, abs_x, -result - result);
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_result_case3(float abs_x)
{
    float x_minus_three = abs_x - 3.0f;
    float a0 = -143033.4f, a1 = -48310.664f, a2 = -41061.375f, a3 = -12349.742f, a4 = -748.8903f;
    float b0 = -206353.58f, b1 = -92685.05f, b2 = -10777.18f, b3 = -259.25097f;

    float y0 = fmaf(a4, x_minus_three, a3);
    y0 = fmaf(y0, x_minus_three, a2);
    y0 = fmaf(y0, x_minus_three, a1);
    y0 = fmaf(y0, x_minus_three, a0);

    float y1 = fmaf(1.0f, x_minus_three, b3);
    y1 = fmaf(y1, x_minus_three, b2);
    y1 = fmaf(y1, x_minus_three, b1);
    y1 = fmaf(y1, x_minus_three, b0);

    return fmaf(y0, 1.0f / y1, x_minus_three);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float lgammaf(float x)
{
    float abs_x = fabsf(x);
    float result = 0.0f;
    if (isinf(abs_x)) {
        return abs_x;
    } else if (abs_x < 0.7f) {
        float y0 = __internal_cal_y0(abs_x);

        //  ln[1/gamma(x)]
        result = __internal_compute_ln(y0);

        //  ln(gamma(x))
        //  = ln(gamma(x)^{-1*-1})
        //  = -ln(1/gamma(x))
        result = -result;

        if (y0 == 0.0f) {
            result = 1.0f / 0.0f;
        }
    } else if (abs_x < 1.5f) {
        result = __internal_cal_result_case1(abs_x);
    } else if (abs_x < 3.0f) {
        result = __internal_cal_result_case2(abs_x);
    } else if (abs_x < 7.8f) {
        result = __internal_cal_result_case3(abs_x);
    } else {
        //  According Stirling's Approximation
        //  ln(gamma(x))
        //  = ln((x-1)!)
        //  = ln({sqrt(2*pi*x) * [(x/e)^x] * e^[(1/12)/x - (1/360)/x^3] + (1/1260)/x^5+O(6)]}/x)
        //  = ln(sqrt(2*pi)) + 0.5*ln(x) + xln(x)-x + [(1/12)/x-(1/360)/x^3]+(1/1260)/x^5] - ln(x)
        //  = ln(sqrt(2*pi)) + 0.5*ln(x) + xln(x)-x + y_2 -ln(x)
        //  = ln(sqrt(2*pi)) + xln(x)-x  -0.5*ln(x) + y_2
        //  = ln(sqrt(2*pi)) + (x-0.5)ln(x) -x + y2

        //  [(1/12) - (1/360)/(1/x^2) + (1/1260)/((1/x^2)^2)] * (1/x)
        float y0 = (1.0f / abs_x);
        float y1 = y0 * y0;
        float y2 = 0.00077783066f;         //  0.00077783066f  : 1/1260
        y2 = fmaf(y2, y1, -0.0027776553f); //  -0.0027776553f  : -1/360
        y2 = fmaf(y2, y1, 0.083333276f);   //  0.083333276     : 1/12
        y2 = fmaf(y2, y0, 0.0f);

        //  ln(x) * 0.5 * (|x| - 0.5)
        float y3 = __internal_compute_ln(abs_x) * 0.5f * (abs_x - 0.5f);

        //  (x-0.5) * ln(x) * 0.5 -|x| + (x-0.5) * ln(x) * 0.5 + y2 + ln(sqrt(2*pi))
        result = y3 - abs_x + y3 + y2 + 0.9189385f; //  0.9189385f : ln[(2*pi)/2]
    }
    if (x < 0) {
        if (__floorf(abs_x) == abs_x) {
            return ASCRT_INF_F;
        } else if (abs_x < 9.9999996e-20f) { // 9.9999996e-20 : minimum-value
            //  According Euler's Reflection Formula
            //  As x ~ 0 : then sin(pi*x) ~ pi*x
            //  ln(|gamma(x)|)
            //  ~ ln(pi) - ln(|sin(pi*x)|) - ln(gamma(|x|)) - ln(|x|)
            //  ~ ln(pi)-ln(-pi*x) - ln(gamma(|x|)) - ln(|x|)
            //  ~ ln(-pi/(-pi*x)) - ln(gamma(|x|)) - ln(|x|)
            //  ~ ln(-1/x) - ln(gamma(-x)) - ln(|x|)
            //  ~ -ln([-x*gamma(-x)]) - ln(|x|)
            //  ~ -ln([gamma(1-x)]) - ln(|x|)
            //  ~ -ln([gamma(1)]) - ln(|x|)
            //  ~ -ln(1!) - ln(|x|)
            //  ~ -ln(|x|)
            result = -__internal_compute_ln(abs_x);
        } else {
            //  According Euler's Reflection Formula & Stirling's Approximation
            //  gamma(x)gamma(1-x) = pi/sin(pi*x)
            //  gamma(x)gamma(-x)*(-x) = pi/sin(pi*x)
            //  gamma(x)
            //  = pi/[sin(pi*x) * gamma(-x)*(-x)]
            //  = pi/[sin(pi*x) * gamma(|x|)*(|x|)] , x<0
            //  ln[|gamma(x)|]
            //  = ln{|pi/[sin(pi*x) * gamma(-x) * (-x)|]}
            //  = ln{pi/[|sin(pi*x)| * gamma(|x|) * (|x|)]}
            //  = ln(pi) - ln(|sin(pi*x)|) - ln(gamma(|x|)) - ln(|x|)
            //  = ln(pi) - ln(|sin(pi*x)|) - ln(gamma(|x|)) - ln(|x|)
            //  = ln(pi) - ln(|sin(pi*|x|)|*|x|]) - ln(gamma(|x|))
            float sinpi = __internal_compute_sinpi(abs_x);
            float ln_x_sinpi = __internal_compute_ln(abs_x * fabsf(sinpi));
            float y = 1.14472985f - ln_x_sinpi; //  1.1447298f : ln(pi)
            result = fmaf(y, 1.0f, -result);
        }
    }
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cyl_bessel_i0f(float x)
{
    float abs_x = fabsf(x);
    if (isinf(abs_x)) {
        return abs_x;
    }
    if (abs_x >= 9) { // 9:the boundary of x
        //  I(x) ~ exp(x) * 1/sqrt(2*pi*x) * [1 + 1/(8x) + 9/(128x^2) + O(3)]
        float reciprocal_x = 1.0f / abs_x;
        float y = 0.34872168f;                     // 0.34872168f     : Coefficient of O(5)
        y = fmaf(y, reciprocal_x, -0.0054563344f); //  -0.0054563344f  : Coefficient of O(4)
        y = fmaf(y, reciprocal_x, 0.033347155f);   //  0.033347155f    : Coefficient of O(3)
        y = fmaf(y, reciprocal_x, 0.027889195f);   //  0.027889195f    : 9/[sqrt(2*pi)*128]
        y = fmaf(y, reciprocal_x, 0.04987063f);    //  0.04987063f     : 1/[sqrt(2*pi)*8]
        y = fmaf(y, reciprocal_x, 0.39894226f);    //  0.39894226f     : 1/sqrt(2*pi)
        y = y * rsqrtf(abs_x);
        return y * (expf(abs_x * 0.5f) - 1) * (expf(abs_x * 0.5f) + 1) + y;
    } else {
        //  I_0(x) = ∑(k=0 to inf)[1/k!Γ(k+1)*(x/2)^2k ]
        float square_x = abs_x * abs_x;
        float y = 1.551427e-19;                  // 1.551427e-19        : Coefficient of O(10)
        y = fmaf(y, square_x, 1.4492505e-17f);   // 1.4492505e-17        : Coefficient of O(9)
        y = fmaf(y, square_x, 1.0687647e-14f);   // 1.0687647e-14f       : Coefficient of O(8)
        y = fmaf(y, square_x, 2.3349575e-12f);   // 2.3349575e-12f       : 1/25401600*16384
        y = fmaf(y, square_x, 4.7306625e-10f);   // 4.7306625e-10f       : 1/518400*4096
        y = fmaf(y, square_x, 6.7778003e-8f);    // 6.7778003e-8f        : 1/(14400*1024)
        y = fmaf(y, square_x, 0.0000067820783f); // 0.0000067820783f     : 1/(576*256)
        y = fmaf(y, square_x, 0.00043402583f);   // 0.00043402583f       : 1/(36*64)
        y = fmaf(y, square_x, 0.015625f);        // 0.015625             : 1/(4*16)
        y = fmaf(y, square_x, 0.25f);            // 0.25f                : 1/(1*4)
        y = fmaf(y, square_x, 1);
        return y;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cyl_bessel_i1f(float x)
{
    float abs_x = fabsf(x);
    if (isinf(abs_x)) {
        return x;
    }
    if (isnan(abs_x)) {
        return abs_x;
    }
    if (abs_x >= 8.085f) {
        //  I(x) ~ exp(x) * 1/sqrt(2*pi*x) * [1 - 3/(8x) -15/(128x^2) + O(3)]
        float reciprocal_x = 1.0f / abs_x;
        float y = -0.5028813f;                   //  -0.5028813   : Coefficient of O(5)
        y = fmaf(y, reciprocal_x, 0.028471555f); //  0.028471555  : Coefficient of O(4)
        y = fmaf(y, reciprocal_x, -0.04873671f); //  -0.04873671  : Coefficient of O(3)
        y = fmaf(y, reciprocal_x, -0.04641596f); //  -0.04641596  : -15/[sqrt(2*pi)*128]
        y = fmaf(y, reciprocal_x, -0.14960973f); //  -0.14960973  : -3/[sqrt(2*pi)*8]
        y = fmaf(y, reciprocal_x, 0.39894232f);  //  0.39894232   : 1/sqrt(2*pi)
        y = y * rsqrtf(abs_x);
        y = y * (expf(abs_x * 0.5f) - 1) * (expf(abs_x * 0.5f) + 1) + y;
        return copysignf(y, x);
    } else {
        //  I(x) = x * [1/2 + (x^2)/16 + (x^2)^2/384 + (x^2)^3/18432 + (x^2)^4/1474560 + (x^2)^5/176947200 + O(6)]
        float square_x = x * x;
        float y = 2.7848253e-18f;              // 2.7848253e-18f    :  Coefficient of O(9)
        y = fmaf(y, square_x, 3.4224707e-16f); // 3.4224707e-16f     : Coefficient of O(8)
        y = fmaf(y, square_x, 1.6258002e-13f); // 1.6258002e-13f     : Coefficient of O(7)
        y = fmaf(y, square_x, 3.3142173e-11f); // 3.3142173e-11f     : Coefficient of O(6)
        y = fmaf(y, square_x, 5.6632734e-9f);  // 5.6632734e-9f      : 1/176947200
        y = fmaf(y, square_x, 6.780027e-7f);   // 6.780027e-7f       : 1/1474560
        y = fmaf(y, square_x, 0.00005425474f); // 0.00005425474f     : 1/18432
        y = fmaf(y, square_x, 0.002604162f);   // 0.002604162f       : 1/384
        y = fmaf(y, square_x, 0.0625000f);     // 0.06250001f        : 1/16
        y = fmaf(y, square_x, 0.5f);           // 0.5f               : 1/2
        return y * x;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normcdff(float x)
{
    if (isnan(x)) {
        return x + x;
    }
    if (x == -ASCRT_INF_F) {
        return 0.0f;
    }
    if (x == ASCRT_INF_F) {
        return 1.0f;
    }

    constexpr float clamp_bound = 14.5f;
    constexpr float inv_sqrt2_hi = -0.70710676908493041992f;
    constexpr float inv_sqrt2_lo = -1.2101617485882343317e-08f;
    constexpr float log2e = 1.4426950216293334961f;
    constexpr float neg_ln2_hi = -0.69314718246459960938f;
    constexpr float ln2_tail = 1.9046542121259335545e-09f;

    const float abs_x = fabsf(x);
    float clamped_x = abs_x > clamp_bound ? __internal_with_sign_bit(clamp_bound, x) : x;
    const bool ge_neg_one = !(clamped_x < -1.0f);

    float z_hi = clamped_x * inv_sqrt2_hi;
    float z_comp = fmaf(clamped_x, inv_sqrt2_hi, -z_hi);
    float z_lo = fmaf(clamped_x, inv_sqrt2_lo, z_comp);
    float z = z_hi + z_lo;
    const float abs_z = fabsf(z);
    const bool z_nonnegative = !(z < 0.0f);

    const float rcp_abs_z_plus_4 = 1.0f / (abs_z + 4.0f);
    float q = (abs_z - 4.0f) * rcp_abs_z_plus_4;
    float q_residual = fmaf(q + 1.0f, -4.0f, abs_z);
    q_residual = fmaf(abs_z, -q, q_residual);
    q = fmaf(rcp_abs_z_plus_4, q_residual, q);

    float exp_k = truncf((z * z) * -log2e);
    const bool exp_k_out_of_range = fabsf(exp_k) > 126.0f;
    float exp_k_limited = exp_k_out_of_range ? __internal_with_sign_bit(126.0f, exp_k) : exp_k;
    float exp_residual = fmaf(exp_k_limited, neg_ln2_hi, -(z * z));
    exp_residual = fmaf(exp_k_limited, ln2_tail, exp_residual);
    float exp2_residual = exp2f(exp_residual * log2e);
    const uint32_t exp_scale_bits = static_cast<uint32_t>(__float2int_rz(exp_k_limited + 12583039.0f)) << 23;
    float exp_scale = __uint_as_float(exp_scale_bits);
    float square_residual = fmaf(-abs_z, abs_z, z * z);
    float exp_term = fmaf(exp_scale, square_residual, exp_scale) * exp2_residual;

    float poly = fmaf(q, 0.0008912170887924731f, 0.0070457882247865200043f);
    poly = fmaf(q, poly, -0.015866896137595176697f);
    poly = fmaf(q, poly, 0.036429625004529953003f);
    poly = fmaf(q, poly, -0.066643431782722473145f);
    poly = fmaf(q, poly, 0.093814529478549957275f);
    poly = fmaf(q, poly, -0.10099056363105773926f);
    poly = fmaf(q, poly, 0.06809400022029876709f);
    poly = fmaf(q, poly, 0.015377387404441833496f);
    poly = fmaf(q, poly, -0.1396210789680480957f);
    poly = fmaf(q, poly, 1.232995152473449707f);

    const float rcp_two_abs_z_plus_1 = 1.0f / fmaf(abs_z, 2.0f, 1.0f);
    float rational = poly * rcp_two_abs_z_plus_1;
    float rational_err = fmaf(abs_z, rational * -2.0f, poly);
    rational_err = -rational + rational_err;
    rational = fmaf(rcp_two_abs_z_plus_1, rational_err, rational);

    float erfc_value = rational * exp_term;
    if (abs_z > 10.05500030517578125f) {
        erfc_value = 0.0f;
    }
    if (!z_nonnegative) {
        erfc_value = 2.0f - erfc_value;
    }
    if (!ge_neg_one) {
        const float correction_scale = z * -2.0f * erfc_value;
        erfc_value = fmaf(z_hi - z + z_lo, correction_scale, erfc_value);
    }
    return erfc_value * 0.5f;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_trig_red_slowpath_f_fast_mode(float a, int* quadrant)
{
    uint64_t q, q2;
    q = static_cast<uint64_t>(a * ASCRT_2OPI_F);
    a = fmaf(q, ASCRT_MINUS_PIO2_HI_F, a);
    a = fmaf(q, ASCRT_MINUS_PIO2_LO_F, a);
    q2 = static_cast<uint64_t>(a * ASCRT_2OPI_F);
    a = fmaf(q2, ASCRT_MINUS_PIO2_HI_F, a);
    q = q % 4 + q2 % 4; // 4:Number of quadrants
    a = a - 0.7853982f;
    *quadrant = q % 4; // 4:Number of quadrants
    return a;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sinf_poly(float a, float s)
{
    float r = 2.86567956e-6f;
    r = fmaf(r, s, -1.98559923e-4f);
    r = fmaf(r, s, 8.33338592e-3f);
    r = fmaf(r, s, -1.66666672e-1f);
    float t = fmaf(a, s, 0.0f);
    r = fmaf(r, t, a);
    return r;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cosf_poly(float s)
{
    float r = 2.44677067e-5f;
    r = fmaf(r, s, -1.38877297e-3f);
    r = fmaf(r, s, 4.16666567e-2f);
    r = fmaf(r, s, -5.00000000e-1f);
    r = fmaf(r, s, 1.00000000e+0f);
    return r;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_sin_cosf_minus_pi_over_four(float a, int index)
{
    float r;
    int i;
    a = a * 0.0f + a;
    r = __internal_trig_red_slowpath_f_fast_mode(a, &i);
    float c, s, t;
    s = r * r;
    c = __internal_cosf_poly(s);
    s = __internal_sinf_poly(r, s);
    if (i & 2) { // 2:Third and Fourth Quadrants
        s = 0.0f - s;
        c = 0.0f - c;
    }
    if (index == 0) { // 0:Calculate CosfMinusPIOverFour
        if (i & 1) {
            c = 0.0f - s;
        }
        return c;
    } else {
        if (i & 1) {
            s = c;
        }
        return s;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j0_y0_pre_coeff(float x, float inv_x, float inv_x2)
{
    float beta = fmaf(5.848699569702148f, inv_x2, -0.5428466796875f);
    beta = fmaf(beta, inv_x2, 0.103515625f);
    beta = fmaf(beta, inv_x2, -0.0625f);
    beta = fmaf(beta, inv_x2, 1.0f);
    float theta = rsqrtf(x);
    theta = theta * 0.7978846f;
    return beta * theta;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j0_y0_alpha(float x, float inv_x, float inv_x2)
{
    float alpha = fmaf(1.6380658830915178f, inv_x2, -0.2095703125f);
    alpha = fmaf(alpha, inv_x2, 0.06510416666666666f);
    alpha = fmaf(alpha, inv_x2, -0.125f);
    alpha = fmaf(alpha, inv_x, x);
    return alpha;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j1_y1_pre_coeff(float x, float inv_x, float inv_x2)
{
    float beta = fmaf(-7.739953994751f, inv_x2, 0.8052978515625f);
    beta = fmaf(beta, inv_x2, -0.193359375f);
    beta = fmaf(beta, inv_x2, 0.1875f);
    beta = fmaf(beta, inv_x2, 1.0f);
    float theta = rsqrtf(x);
    theta = theta * 0.7978846f;
    return beta * theta;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j1_y1_alpha(float x, float inv_x, float inv_x2)
{
    float alpha = fmaf(-2.3693978445870534f, inv_x2, 0.3708984375f);
    alpha = fmaf(alpha, inv_x2, -0.1640625f);
    alpha = fmaf(alpha, inv_x2, 0.375f);
    alpha = fmaf(alpha, inv_x, x);
    return alpha;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j0_x_larger8(float x)
{
    if (isinf(x)) {
        return 0.0f;
    }
    float inv_x = 1.0f / x;
    float inv_x2 = inv_x * inv_x;
    float alpha = __internal_cal_j0_y0_alpha(x, inv_x, inv_x2);
    float after_coeff = __internal_sin_cosf_minus_pi_over_four(alpha, 0); // 0:Calculate CosfMinusPIOverFour
    float pre_coeff = __internal_cal_j0_y0_pre_coeff(x, inv_x, inv_x2);
    return after_coeff * pre_coeff;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j1_x_lager8(float x)
{
    if (isinf(x)) {
        return 0.0f;
    }
    float inv_x = 1.0f / x;
    float inv_x2 = inv_x * inv_x;
    float alpha = __internal_cal_j1_y1_alpha(x, inv_x, inv_x2);
    float after_coeff = __internal_sin_cosf_minus_pi_over_four(alpha, 1); // 1:Calculate SinfMinusPIOverFour
    float pre_coeff = __internal_cal_j1_y1_pre_coeff(x, inv_x, inv_x2);
    return after_coeff * pre_coeff;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j0_x_less8(float x)
{
    float d1 = x - 2.4048254f;
    d1 = d1 - 1.087059e-7f;
    float res = 9.619266247e-13f;
    res = fmaf(res, d1, 5.702105547e-12f);
    res = fmaf(res, d1, -4.398487105e-10f);
    res = fmaf(res, d1, 4.604940853e-10f);
    res = fmaf(res, d1, 5.847321173e-08f);
    res = fmaf(res, d1, 2.084518856e-09f);
    res = fmaf(res, d1, -5.452075416e-06f);
    res = fmaf(res, d1, -7.342953250e-06f);
    res = fmaf(res, d1, 3.017067874e-04f);
    res = fmaf(res, d1, 7.739535477e-04f);
    res = fmaf(res, d1, -7.283463700e-03f);
    res = fmaf(res, d1, -2.666820353e-02f);
    res = d1 * res;
    float d2 = x - 5.520078f;
    d2 = d2 + 7.1934145e-8f;
    res = d2 * res;

    float d3 = x - 8.653728f;
    d3 = d3 - 3.8147791e-7f;
    res = d3 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_j1_x_less8(float x)
{
    float d1 = x - 3.831706f;
    d1 = d1 + 7.685059e-8f;
    float res = 9.206492556e-14f;
    res = fmaf(res, d1, 9.126927192e-13f);
    res = fmaf(res, d1, -2.641634001e-11f);
    res = fmaf(res, d1, -2.014359882e-10f);
    res = fmaf(res, d1, 4.525844770e-09f);
    res = fmaf(res, d1, 2.701145918e-08f);
    res = fmaf(res, d1, -5.348958058e-07f);
    res = fmaf(res, d1, -2.360248564e-06f);
    res = fmaf(res, d1, 4.121127279e-05f);
    res = fmaf(res, d1, 1.191702295e-04f);
    res = fmaf(res, d1, -1.807559530e-03f);
    res = fmaf(res, d1, -2.554892713e-03f);
    res = fmaf(res, d1, 3.301389139e-02f);
    res = d1 * res;
    float d2 = x - 7.015587f;
    d2 = d2 + 1.8321172e-7f;
    res = d2 * res;
    res = res * x;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_jn_yn_asymptotic_bessel_amplitude(int n, float x, int index)
{
    float s = 1.0f;
    float mu = 4 * n * n;
    float txq = 2 * x;
    txq *= txq;
    if (index == 0) { // 0:Calculate JnAsymptoticBesselAmplitude
        // 1 + (4 * n^2 - 1) / (8 * x^2) + 3 * (4 * n^2 - 1) * (4 * n^2 - 9) / (128 * x^4)
        s += (mu - 1) / (2 * txq);                      // 1,2:Constants in formulas
        s += 3 * (mu - 1) * (mu - 9) / (txq * txq * 8); // 3,1,9,8:Constants in formulas
    } else {
        s += (mu - 1) / (2 * txq);                                             // 1,2:Constants in formulas
        s += 3 * (mu - 1) * (mu - 9) / (txq * txq * 8);                        // 3,1,9,8:Constants in formulas
        s += 15 * (mu - 1) * (mu - 9) * (mu - 25) / (txq * txq * txq * 8 * 6); // 15,1,9,25,8,6:Constants in formulas
    }
    return sqrtf(s * 2 / (ASCRT_PI_F * x)); // 2:Constants in formulas    sqrt(2*s/(π*x))
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_jn_yn_asymptotic_bessel_phase_mx(int n, float x)
{
    float mu = 4 * n * n;
    float denom = 4 * x;
    float denom_mult = denom * denom;
    float s = 0;
    // (4 * n^2 - 1) / (8 * x) + (4 * n^2 - 1) * (4 * n^2 - 25) / (384 * x^3) + (4 * n^2 - 1) * (16 * n^4 - 456 *
    // n^2 + 1073) / (5120 * x^5)
    s += (mu - 1) / (2 * denom); // 1,2:Constants in formulas
    denom *= denom_mult;
    s += (mu - 1) * (mu - 25) / (6 * denom); // 1,25,6:Constants in formulas
    denom *= denom_mult;
    s += (mu - 1) * (mu * mu - 114 * mu + 1073) / (5 * denom); // 1,114,1073,5:Constants in formulas
    return s;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_jn_case1(int n, float x)
{
    float ampl = __internal_jn_yn_asymptotic_bessel_amplitude(n, x, 0);
    float phase = __internal_jn_yn_asymptotic_bessel_phase_mx(n, x);
    float cx, sx, ci, si, cp, sp;
    sincosf(x, &sx, &cx);
    float offset = static_cast<float>(n) / 2 + 0.25f;
    sincospif(offset, &si, &ci);
    sincosf(phase, &sp, &cp);
    float sin_phase = cp * (cx * ci + sx * si) - sp * (sx * ci - cx * si);
    return sin_phase * ampl;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_jn_case2(int n, float x)
{
    float prev = j0f(x);
    float current = j1f(x);
    for (int k = 1; k < n; k++) {
        float value = (2 * k * current / x) - prev;
        prev = current;
        current = value;
    }
    return current;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_jn_case3(int n, float x)
{
    float prefix = n * logf(x / 2);
    for (int i = 2; i < n + 1; i++) {
        prefix = prefix - logf(static_cast<float>(i));
    }
    prefix = expf(prefix);
    float mult = x / 2;
    mult *= -mult;
    float term = 1;
    float res = 0;
    int case3_k = 14;
    for (int i = 1; i < case3_k + 1; i++) { // 1:Order of Series Expansion 14
        res += term;
        term *= mult / (i * (i + n));
    }
    return prefix * res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_jn_case4(int n, float x)
{
    float max_value = powf(2.0f, 60.0f);
    int N = n + 30;
    float prev = 1e-30f;
    float current = 0;
    float s = 0;
    float scale = 1;
    float res;
    for (int k = N - 1; k >= 0; k--) {
        float fact = 2 * (k + 1) / x;
        if (fact > 1 && fabsf(current) > max_value) {
            prev /= max_value;
            s /= max_value;
            scale /= max_value;
            current /= max_value;
        }
        float tmp = 2 * (k + 1) / x * current - prev;
        if (k % 2 == 0) { // 2:Used for summation:U(2i, x)
            s += 2 * tmp; // 2:coefficient of summation
        }
        if (k == n) {
            res = tmp / scale;
        }
        prev = current;
        current = tmp;
    }
    s -= current;
    res /= s;
    return res * scale;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0_x_less8(float x)
{
    float d1 = x - 2.4048254f;
    d1 = d1 - -1.087059e-7f;
    float res = -1.026110251e-13f;
    res = fmaf(res, d1, 2.926116439e-12f);
    res = fmaf(res, d1, -6.819261288e-12f);
    res = fmaf(res, d1, -4.233725725e-10f);
    res = fmaf(res, d1, 5.903298799e-10f);
    res = fmaf(res, d1, 5.804848319e-08f);
    res = fmaf(res, d1, 1.808731234e-09f);
    res = fmaf(res, d1, -5.449918970e-06f);
    res = fmaf(res, d1, -7.343399316e-06f);
    res = fmaf(res, d1, 3.017029154e-04f);
    res = fmaf(res, d1, 7.739547690e-04f);
    res = fmaf(res, d1, -7.283461771e-03f);
    res = fmaf(res, d1, -2.666820378e-02f);
    res = d1 * res;
    float d2 = x - 5.520078f;
    d2 = d2 + 7.1934145e-8f;
    res = d2 * res;

    float d3 = x - 8.653728f;
    d3 = d3 - 3.8147791e-7f;
    res = d3 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y0_x_lessdot5(float x)
{
    float part1 = 0.636619772367f * __internal_j0_x_less8(x) * logf(x);
    float part2 = fmaf(0.0007977247950890495f, x, -0.016524315326267768f);
    part2 = fmaf(part2, x, 0.0001196180186f);
    part2 = fmaf(part2, x, 0.17759110676f);
    part2 = fmaf(part2, x, 0.00000074368805978f);
    part2 = fmaf(part2, x, -0.07380430393219066f);
    return part1 + part2;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y0_x_part1(float x)
{
    float d1 = x - 0.893576980f;
    d1 = d1 + 1.33579787e-8f;
    float res = -4.485103697e-03f;
    res = fmaf(res, d1, 3.231012427e-02f);
    res = fmaf(res, d1, -1.014045593e-01f);
    res = fmaf(res, d1, 1.847167541e-01f);
    res = fmaf(res, d1, -2.253558856e-01f);
    res = fmaf(res, d1, 2.147535120e-01f);
    res = fmaf(res, d1, -1.959638733e-01f);
    res = fmaf(res, d1, 1.944536283e-01f);
    res = fmaf(res, d1, -2.040570397e-01f);
    res = fmaf(res, d1, 2.190628354e-01f);
    res = fmaf(res, d1, -2.261730477e-01f);
    res = fmaf(res, d1, 2.205519494e-01f);
    res = fmaf(res, d1, -4.920781667e-01f);
    res = fmaf(res, d1, 8.794208015e-01f);
    res = d1 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y0_x_part2(float x)
{
    float d3 = x - 7.08605099f;
    d3 = d3 - 7.30581178e-8f;
    float res = 8.146675423e-11f;
    res = fmaf(res, d3, 1.030741112e-09f);
    res = fmaf(res, d3, 1.610027889e-09f);
    res = fmaf(res, d3, 1.063494888e-08f);
    res = fmaf(res, d3, 6.693347461e-07f);
    res = fmaf(res, d3, 7.816005861e-07f);
    res = fmaf(res, d3, -4.836658731e-05f);
    res = fmaf(res, d3, 1.049324298e-05f);
    res = fmaf(res, d3, 2.142965752e-03f);
    res = fmaf(res, d3, -3.385610246e-03f);
    res = fmaf(res, d3, -3.743254148e-02f);
    res = fmaf(res, d3, 9.592770584e-02f);
    res = d3 * res;
    float d2 = x - 3.95767832f;
    d2 = d2 - 1.01291178e-7f;
    res = d2 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y0_x_larger8(float x)
{
    if (isinf(x)) {
        return 0.0f;
    }
    float inv_x = 1.0f / x;
    float inv_x2 = inv_x * inv_x;
    float alpha = __internal_cal_j0_y0_alpha(x, inv_x, inv_x2);
    float after_coeff = __internal_sin_cosf_minus_pi_over_four(alpha, 1); // 1:Calculate SinfMinusPIOverFour
    float pre_coeff = __internal_cal_j0_y0_pre_coeff(x, inv_x, inv_x2);
    return after_coeff * pre_coeff;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y1_x_less1dot2(float x, float minus_two_over_pi_mul_inv_x)
{
    float part1 = 0.636619772367f * __internal_cal_j1_x_less8(x) * logf(x);
    float part2 = fmaf(0.0002798307076f, x, -0.0034028867918f);
    part2 = fmaf(part2, x, 0.0003643335439f);
    part2 = fmaf(part2, x, 0.0541922288594f);
    part2 = fmaf(part2, x, 0.00003339972037f);
    part2 = fmaf(part2, x, -0.1960600316f);
    part2 = fmaf(part2, x, 0.0000000624278f);

    return part1 + minus_two_over_pi_mul_inv_x + part2;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y1_x_part1(float x)
{
    float d1 = x - 2.19714141f;
    d1 = d1 + 8.28892723e-8f;
    float res = -7.210192066e-05f;
    res = fmaf(res, d1, 6.665645689e-05f);
    res = fmaf(res, d1, -3.106003176e-05f);
    res = fmaf(res, d1, 2.276838750e-04f);
    res = fmaf(res, d1, -5.566432475e-04f);
    res = fmaf(res, d1, 1.068050095e-03f);
    res = fmaf(res, d1, -2.582285756e-03f);
    res = fmaf(res, d1, 7.422557063e-03f);
    res = fmaf(res, d1, -4.799279782e-03f);
    res = fmaf(res, d1, -3.285740200e-02f);
    res = fmaf(res, d1, -1.185144993e-01f);
    res = fmaf(res, d1, 5.207864124e-01f);
    res = d1 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y1_x_part2(float x)
{
    float d2 = x - 5.42968082f;
    d2 = d2 - 2.16514351e-7f;
    float res = -4.575132868e-10f;
    res = fmaf(res, d2, 4.435273368e-09f);
    res = fmaf(res, d2, 3.963341878e-08f);
    res = fmaf(res, d2, -4.231424306e-07f);
    res = fmaf(res, d2, -4.201841643e-06f);
    res = fmaf(res, d2, 3.316061621e-05f);
    res = fmaf(res, d2, 2.516106023e-04f);
    res = fmaf(res, d2, -1.369325160e-03f);
    res = fmaf(res, d2, -8.495834725e-03f);
    res = fmaf(res, d2, 2.404736994e-02f);
    res = fmaf(res, d2, 1.074804589e-01f);
    res = d2 * res;
    float d3 = x - 8.59600544f;
    d3 = d3 - 4.28572861e-7f;
    res = d3 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_y1_x_larger8(float x)
{
    if (isinf(x)) {
        return 0.0f;
    }
    float inv_x = 1.0f / x;
    float inv_x2 = inv_x * inv_x;
    float alpha = __internal_cal_j1_y1_alpha(x, inv_x, inv_x2);
    float after_coeff = __internal_sin_cosf_minus_pi_over_four(alpha, 0); // 0:Calculate CosfMinusPIOverFour
    float pre_coeff = __internal_cal_j1_y1_pre_coeff(x, inv_x, inv_x2);
    return -after_coeff * pre_coeff;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_yn_case1(int n, float x)
{
    float lgamma_n = lgammaf(n);
    float gamma_n = expf(lgamma_n);
    return -gamma_n / ASCRT_PI_F *
           powf(2 / x, static_cast<float>(n)); // 2:Constants in formulas -(n - 1)! * (2 / x)^n / π
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_yn_case2(int n, float x)
{
    float ampl = __internal_jn_yn_asymptotic_bessel_amplitude(n, x, 1); // 1: Calculate YnAsymptoticBesselAmplitude
    float phase = __internal_jn_yn_asymptotic_bessel_phase_mx(n, x);
    float phase_shift = n * ASCRT_PI_F / 2 + ASCRT_PI_F / 4;
    float cos_x = cosf(x);
    float sin_x = sinf(x);
    float cos_shift = cosf(phase_shift - phase);
    float sin_shift = sinf(phase_shift - phase);
    float sin_combined = sin_x * cos_shift - cos_x * sin_shift;
    return sin_combined * ampl;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_yn_case3(int n, float x)
{
    float prev = y0f(x);
    float current = y1f(x);
    float scale_factor = 1.0f;
    float inv_x = 2.0f / x;

    float mult = 0.0f;
    float value = 0.0f;
    float inv = 0.0f;
    int k = 1;
    for (; k + 2 < n; k += 3) { // 2,3: loop unrolling parameters
        mult = k * inv_x;
        value = mult * current - prev;
        prev = current;
        current = value;

        mult = (k + 1) * inv_x;
        value = mult * current - prev;
        prev = current;
        current = value;

        mult = (k + 2) * inv_x; // 2: offset for the third unrolled iteration
        value = mult * current - prev;
        prev = current;
        current = value;

        if (fabsf(mult) > 1.0f && fabsf(current) > 1.0f) {
            inv = 1.0f / current;
            prev *= inv;
            scale_factor *= inv;
            value *= inv;
            current = 1.0f;
        }
    }
    while (k < n) {
        mult = k * inv_x;
        value = mult * current - prev;
        prev = current;
        current = value;
        k++;
    }
    return value / scale_factor;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0f_huge_range(float ax)
{
    float inv = 1.0f / ax;
    float inv2 = inv * inv;

    float alpha_poly = __fma(inv2, 1.13964951038360595703125f, -0.205326750874519348144531f);
    alpha_poly = __fma(inv2, alpha_poly, 0.0650917366147041320800781f);
    alpha_poly = __fma(inv2, alpha_poly, -0.124999992549419403076172f);
    float alpha = __fma(inv, alpha_poly, ax);

    float beta = __fma(inv2, 3.3592879772186279296875f, -0.51452267169952392578125f);
    beta = __fma(inv2, beta, 0.103370569646358489990234f);
    beta = __fma(inv2, beta, -0.0624997243285179138183594f);
    beta = __fma(inv2, beta, 1.0f);

    float r = alpha;
    int q = 0;
    if (alpha >= 105615.0f) {
        r = __internal_payne_hanek_radian_reduction(alpha, &q);
        q = q & 3;
        r = r + static_cast<float>(q) * 1.57079637050628662109375f;
    }

    float amp = beta * rsqrtf(ax) * 0.79788458347320556640625f;
    return amp * cosf(r - 0.785398185253143310546875f);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j1f_huge_range(float ax)
{
    float inv = 1.0f / ax;
    float inv2 = inv * inv;

    float alpha_poly = __fma(inv2, -1.5799448490142822265625f, 0.361485868692398071289062f);
    alpha_poly = __fma(inv2, alpha_poly, -0.164012610912322998046875f);
    alpha_poly = __fma(inv2, alpha_poly, 0.374999910593032836914062f);
    float alpha = __fma(inv, alpha_poly, ax);

    float beta = __fma(inv2, 4.087306499481201171875f, 0.749876558780670166015625f);
    beta = __fma(inv2, beta, -0.192915648221969604492188f);
    beta = __fma(inv2, beta, 0.187498256564140319824219f);
    beta = __fma(inv2, beta, 1.0f);

    float r = alpha;
    int q = 0;
    if (alpha >= 105615.0f) {
        r = __internal_payne_hanek_radian_reduction(alpha, &q);
        q = q & 3;
        r = r + static_cast<float>(q) * 1.57079637050628662109375f;
    }

    float amp = beta * rsqrtf(ax) * 0.79788458347320556640625f;
    return amp * cosf(r - 2.35619449615478515625f);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0j1_middle_trig_red_slowpath_f_fast_mode(float a, int* quadrant)
{
    int64_t q = static_cast<int64_t>(a * ASCRT_2OPI_F);
    a = fmaf(static_cast<float>(q), ASCRT_MINUS_PIO2_HI_F, a);
    a = fmaf(static_cast<float>(q), ASCRT_MINUS_PIO2_LO_F, a);
    int64_t q2 = static_cast<int64_t>(a * ASCRT_2OPI_F);
    a = fmaf(static_cast<float>(q2), ASCRT_MINUS_PIO2_HI_F, a);
    int q_mod = static_cast<int>((q + q2) % 4);
    if (q_mod < 0) {
        q_mod += 4;
    }
    a = a - 0.7853982f;
    *quadrant = q_mod;
    return a;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0j1_middle_sinf_poly(float a, float s)
{
    float r = 2.86567956e-6f;
    r = fmaf(r, s, -1.98559923e-4f);
    r = fmaf(r, s, 8.33338592e-3f);
    r = fmaf(r, s, -1.66666672e-1f);
    float t = fmaf(a, s, 0.0f);
    r = fmaf(r, t, a);
    return r;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0j1_middle_cosf_poly(float s)
{
    float r = 2.44677067e-5f;
    r = fmaf(r, s, -1.38877297e-3f);
    r = fmaf(r, s, 4.16666567e-2f);
    r = fmaf(r, s, -5.00000000e-1f);
    r = fmaf(r, s, 1.00000000e+0f);
    return r;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0j1_middle_sin_cosf_minus_pi_over_four(float a, int index)
{
    int i = 0;
    a = a * 0.0f + a;
    float r = __internal_j0j1_middle_trig_red_slowpath_f_fast_mode(a, &i);
    float s2 = r * r;
    float c = __internal_j0j1_middle_cosf_poly(s2);
    float s = __internal_j0j1_middle_sinf_poly(r, s2);
    if (i & 2) {
        s = 0.0f - s;
        c = 0.0f - c;
    }
    if (index == 0) {
        if (i & 1) {
            c = 0.0f - s;
        }
        return c;
    }
    if (i & 1) {
        s = c;
    }
    return s;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0_middle_pre_coeff(float x, float inv_x, float inv_x2)
{
    float beta = fmaf(5.848699569702148f, inv_x2, -0.5428466796875f);
    beta = fmaf(beta, inv_x2, 0.103515625f);
    beta = fmaf(beta, inv_x2, -0.0625f);
    beta = fmaf(beta, inv_x2, 1.0f);
    float theta = rsqrtf(x);
    theta = theta * 0.7978846f;
    return beta * theta;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0_middle_alpha(float x, float inv_x, float inv_x2)
{
    float alpha = fmaf(1.6380658830915178f, inv_x2, -0.2095703125f);
    alpha = fmaf(alpha, inv_x2, 0.06510416666666666f);
    alpha = fmaf(alpha, inv_x2, -0.125f);
    alpha = fmaf(alpha, inv_x, x);
    return alpha;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j1_middle_pre_coeff(float x, float inv_x, float inv_x2)
{
    float beta = fmaf(-7.739953994751f, inv_x2, 0.8052978515625f);
    beta = fmaf(beta, inv_x2, -0.193359375f);
    beta = fmaf(beta, inv_x2, 0.1875f);
    beta = fmaf(beta, inv_x2, 1.0f);
    float theta = rsqrtf(x);
    theta = theta * 0.7978846f;
    return beta * theta;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j1_middle_alpha(float x, float inv_x, float inv_x2)
{
    float alpha = fmaf(-2.3693978445870534f, inv_x2, 0.3708984375f);
    alpha = fmaf(alpha, inv_x2, -0.1640625f);
    alpha = fmaf(alpha, inv_x2, 0.375f);
    alpha = fmaf(alpha, inv_x, x);
    return alpha;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0f_middle_range(float x)
{
    if (isinf(x)) {
        return 0.0f;
    }
    float inv_x = 1.0f / x;
    float inv_x2 = inv_x * inv_x;
    float alpha = __internal_j0_middle_alpha(x, inv_x, inv_x2);
    float after_coeff = __internal_j0j1_middle_sin_cosf_minus_pi_over_four(alpha, 0);
    float pre_coeff = __internal_j0_middle_pre_coeff(x, inv_x, inv_x2);
    return after_coeff * pre_coeff;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j1f_middle_range(float x)
{
    if (isinf(x)) {
        return 0.0f;
    }
    float inv_x = 1.0f / x;
    float inv_x2 = inv_x * inv_x;
    float alpha = __internal_j1_middle_alpha(x, inv_x, inv_x2);
    float after_coeff = __internal_j0j1_middle_sin_cosf_minus_pi_over_four(alpha, 1);
    float pre_coeff = __internal_j1_middle_pre_coeff(x, inv_x, inv_x2);
    return after_coeff * pre_coeff;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j0f_less8(float x)
{
    float d1 = x - 2.4048254f;
    d1 = d1 - 1.087059e-7f;
    float res = 9.619266247e-13f;
    res = fmaf(res, d1, 5.702105547e-12f);
    res = fmaf(res, d1, -4.398487105e-10f);
    res = fmaf(res, d1, 4.604940853e-10f);
    res = fmaf(res, d1, 5.847321173e-08f);
    res = fmaf(res, d1, 2.084518856e-09f);
    res = fmaf(res, d1, -5.452075416e-06f);
    res = fmaf(res, d1, -7.342953250e-06f);
    res = fmaf(res, d1, 3.017067874e-04f);
    res = fmaf(res, d1, 7.739535477e-04f);
    res = fmaf(res, d1, -7.283463700e-03f);
    res = fmaf(res, d1, -2.666820353e-02f);
    res = d1 * res;
    float d2 = x - 5.520078f;
    d2 = d2 + 7.1934145e-8f;
    res = d2 * res;
    float d3 = x - 8.653728f;
    d3 = d3 - 3.8147791e-7f;
    res = d3 * res;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_j1f_less8(float x)
{
    float d1 = x - 3.831706f;
    d1 = d1 + 7.685059e-8f;
    float res = 9.206492556e-14f;
    res = fmaf(res, d1, 9.126927192e-13f);
    res = fmaf(res, d1, -2.641634001e-11f);
    res = fmaf(res, d1, -2.014359882e-10f);
    res = fmaf(res, d1, 4.525844770e-09f);
    res = fmaf(res, d1, 2.701145918e-08f);
    res = fmaf(res, d1, -5.348958058e-07f);
    res = fmaf(res, d1, -2.360248564e-06f);
    res = fmaf(res, d1, 4.121127279e-05f);
    res = fmaf(res, d1, 1.191702295e-04f);
    res = fmaf(res, d1, -1.807559530e-03f);
    res = fmaf(res, d1, -2.554892713e-03f);
    res = fmaf(res, d1, 3.301389139e-02f);
    res = d1 * res;
    float d2 = x - 7.015587f;
    d2 = d2 + 1.8321172e-7f;
    res = d2 * res;
    res = res * x;
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float j0f(float x)
{
    if (__isnan(x)) {
        return x;
    }
    float ax = __fabsf(x);
    if (__isinf(ax)) {
        return 0.0f;
    }
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);
    }
    if (ax <= 1.0e13f) {
        return __internal_j0f_middle_range(ax);
    }
    return __internal_j0f_huge_range(ax);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float j1f(float x)
{
    if (__isnan(x)) {
        return x;
    }
    if (x == 0.0f) {
        return x;
    }
    float ax = __fabsf(x);
    if (__isinf(ax)) {
        if (x < 0.0f) {
            uint32_t neg_zero_bits = 0x80000000U;
            return reinterpret_cast<float&>(neg_zero_bits);
        }
        return 0.0f;
    }
    float y;
    if (ax <= 8.0f) {
        y = __internal_j1f_less8(ax);
    } else if (ax <= 1.0e13f) {
        y = __internal_j1f_middle_range(ax);
    } else {
        y = __internal_j1f_huge_range(ax);
    }
    if (x < 0.0f) {
        y = -y;
    }
    return y;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float jnf(int n, float x)
{
    if (n == 0) {
        return j0f(x);
    }
    if (n == 1) {
        return j1f(x);
    }
    if (n < 0 || isnan(x)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (x == 0) {
        return 0;
    }
    if (isinf(x)) {
        return 0;
    }
    float res = 1;
    if (x < 0) {
        res *= (n & 1) ? -1 : 1;
        x = -x;
    }
    if (n < x * 0.1f) {
        res = res * __internal_jn_case1(n, x);
    } else if (n < x) {
        res *= __internal_jn_case2(n, x);
    } else if (n > x * x / 10) { // 10:the denominator of the expression x squared over ten
        res *= __internal_jn_case3(n, x);
    } else {
        res *= __internal_jn_case4(n, x);
    }
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float y0f(float x)
{
    if (x < 0 || isnan(x)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float f1 = fabsf(x);
    if (f1 > 1e13f && isfinite(f1)) {
        return 0;
    }
    float res;
    if (f1 < 0.5f) {
        res = __internal_cal_y0_x_lessdot5(f1);
    } else if (f1 < 2.1971413260310170351f) {
        res = __internal_cal_y0_x_part1(f1);
    } else if (f1 < 8.0f) {
        res = __internal_cal_y0_x_part2(f1);
    } else {
        res = __internal_cal_y0_x_larger8(f1);
    }
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float y1f(float x)
{
    if (x < 0 || isnan(x)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float f1 = fabsf(x);
    if (f1 > 1e13f && isfinite(f1)) {
        return 0;
    }
    if (x == 0.0f) {
        return -ASCRT_INF_F;
    }
    float minus_two_over_pi_mul_inv_x = -0.636619772367f / f1;
    float res;
    if (f1 < 1.17549435e-38f) {
        res = minus_two_over_pi_mul_inv_x;
    }
    if (f1 < 1.2f) {
        res = __internal_cal_y1_x_less1dot2(f1, minus_two_over_pi_mul_inv_x);
    } else if (f1 < 3.0f) {
        res = __internal_cal_y1_x_part1(f1);
    } else if (f1 < 8.0f) {
        res = __internal_cal_y1_x_part2(f1);
    } else {
        res = __internal_cal_y1_x_larger8(f1);
    }
    return res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float ynf(int n, float x)
{
    if (n < 0 || x < 0 || isnan(x)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (x == 0) {
        return -ASCRT_INF_F;
    }
    if (isinf(x)) {
        return 0;
    }
    if (n == 0) {
        return y0f(x);
    }
    if (n == 1) {
        return y1f(x);
    }

    float large_x_threshold = n * 10;
    float small_x_threshold = 1e-8f;

    if (x < small_x_threshold) {
        return __internal_yn_case1(n, x);
    }
    if (x > large_x_threshold) {
        return __internal_yn_case2(n, x);
    }

    return __internal_yn_case3(n, x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long int labs(long int x) { return abs(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long long int llabs(long long int x) { return abs(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long long int llmax(const long long int x, const long long int y)
{
    return max(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long int ullmax(
    const unsigned long long int x, const unsigned long long int y)
{
    return max(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int umax(const unsigned int x, const unsigned int y)
{
    return max(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline long long int llmin(const long long int x, const long long int y)
{
    return min(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long int ullmin(
    const unsigned long long int x, const unsigned long long int y)
{
    return min(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int umin(const unsigned int x, const unsigned int y)
{
    return min(x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int max(unsigned int x, int y) { return max(x, (unsigned int)y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int max(int x, unsigned int y) { return max((unsigned int)x, y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long int max(long int x, unsigned long int y)
{
    return ullmax((unsigned long int)x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long int max(unsigned long int x, long int y)
{
    return ullmax(x, (unsigned long int)y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long int max(long long int x, unsigned long long int y)
{
    return max((unsigned long long int)x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long int max(unsigned long long int x, long long int y)
{
    return max(x, (unsigned long long int)y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int min(unsigned int x, int y) { return min(x, (unsigned int)y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int min(int x, unsigned int y) { return min((unsigned int)x, y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long int min(long int x, unsigned long int y)
{
    return ullmin((unsigned long int)x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long int min(unsigned long int x, long int y)
{
    return ullmin(x, (unsigned long int)(y));
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long int min(long long int x, unsigned long long int y)
{
    return min((unsigned long long int)x, y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long int min(unsigned long long int x, long long int y)
{
    return min(x, (unsigned long long int)y);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fdividef(float x, float y) { return x / y; }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline int signbit(float x) { return signbitf(x); }

#endif
#endif // IMPL_SIMT_API_MATH_FUNCTIONS_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_FUNCTIONS_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_FUNCTIONS_IMPL__
#endif
