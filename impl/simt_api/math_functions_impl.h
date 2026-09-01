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
#include "impl/simt_api/internal_functions_impl.h"
#include "impl/simt_api/device_functions_impl.h"

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)

#define ASCRT_FOUR_BYTE_LEN_U 32U
constexpr float __internal_subnormal_boundary =
    1.17549435e-38f;                                      // 1.17549435e-38f: subnormal floating-point number boundary
constexpr float __internal_fp32_scale_2p24 = 16777216.0f; // 2^24, used to lift subnormal inputs into the normal range.
constexpr float __internal_fp32_log2e = 1.4426950216293334961f; // log2(e), used for ln-to-exp2 conversion.
constexpr float __internal_fp32_ln2_hi =
    0.69314718246459960938f; // High part of ln(2) for split residual reconstruction.
constexpr float __internal_fp32_ln2_tail =
    1.9046542121259335545e-09f; // Low tail of ln(2) for split residual reconstruction.
constexpr float __internal_fp32_subnormal_exponent_fix =
    -24.0f; // Exponent correction after scaling subnormals by 2^24.
constexpr float __internal_fp32_log_exponent_scale =
    1.1920928955078125e-07f;                             // 2^-23, converts exponent bits into log2 units.
constexpr uint32_t __internal_fp32_exponent_shift = 23U; // Bit offset of the fp32 exponent field.
constexpr float __internal_fp32_max_exp = 126.0f;        // Largest finite fp32 exponent used by the clamped paths.
constexpr int32_t __internal_fp32_subnormal_bias = 149;  // Subnormal exponent/reference bias for fp32 reconstruction.

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
    constexpr float scale_up = __internal_fp32_scale_2p24; // 2^24
    constexpr float scale_down = 0.000244140625f;          // 2^-12
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
    constexpr float scale_up = __internal_fp32_scale_2p24; // 2^24
    constexpr float scale_down = 0.000244140625f;          // 2^-12
    // Scale positive subnormal inputs to the normal range, then scale the result back.
    if (x < __internal_subnormal_boundary) {
        return __sqrtf(x * scale_up) * scale_down;
    }
    return __sqrtf(x);
}
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_log2f_positive_finite(float x, float& log2_hi, float& log2_lo)
{
    constexpr uint32_t log_reduction_mask = 0xFF800000U;   // Keeps sign/exponent bits and clears mantissa bits.
    constexpr uint32_t sqrt_half_bits = 0x3F3504F3U;       // sqrt(0.5), selects a mantissa range near 1.
    constexpr float log2e_hi = __internal_fp32_log2e;      // High part of log2(e).
    constexpr float log2e_lo = 1.9251366722983220825e-08f; // Low correction part of log2(e).

    // The polynomial is accurate only on a small range.  Subnormal inputs are scaled up first, then the
    // exponent compensation is added back through exponent_base.
    const bool is_normal_x = x >= __internal_subnormal_boundary;
    const float log_input = is_normal_x ? x : x * __internal_fp32_scale_2p24;
    const float exponent_base = is_normal_x ? 0.0f : __internal_fp32_subnormal_exponent_fix;
    const uint32_t log_input_bits = __float_as_uint(log_input);

    // Split x into mantissa * 2^k using bit arithmetic.  Subtracting sqrt(0.5) before masking chooses k so
    // that mantissa stays close to 1, which reduces the log polynomial error.
    const uint32_t reduction_bits = (log_input_bits - sqrt_half_bits) & log_reduction_mask;
    const float mantissa = __uint_as_float(log_input_bits - reduction_bits);
    const float exponent_part =
        fmaf(__int2float_rn(static_cast<int32_t>(reduction_bits)), __internal_fp32_log_exponent_scale, exponent_base);

    // Use r = 2 * (mantissa - 1) / (mantissa + 1).  This is the atanh-style log reduction; r is small
    // around mantissa == 1, so a short odd-power correction polynomial is enough.
    const float mantissa_minus_one = mantissa - 1.0f;
    const float reciprocal = 1.0f / (mantissa + 1.0f);
    const float reduced_hi = reciprocal * (mantissa_minus_one + mantissa_minus_one);
    const float reduced_square = reduced_hi * reduced_hi;

    // Polynomial correction for log(mantissa), translated from the CUDA-style FFMA chain and evaluated in
    // Horner form.  The final multiply by reduced_square accounts for the higher-order terms.
    float log_poly = fmaf(reduced_square, 0.0006568862590938807f, 0.0032181653659790754318f);
    log_poly = fmaf(reduced_square, log_poly, 0.018033718690276145935f);
    log_poly = fmaf(reduced_square, log_poly, 0.12022458761930465698f);
    log_poly = reduced_square * log_poly;

    // log2_hi carries the rounded main result.  The following reduced_err/reduced_lo path reconstructs the
    // division residual so powf can later multiply log2(x) by y with a useful low part.
    log2_hi = fmaf(reduced_hi, log2e_hi, exponent_part);
    float reduced_err = mantissa_minus_one - reduced_hi;
    reduced_err = fmaf(mantissa_minus_one, -reduced_hi, reduced_err + reduced_err);
    const float reduced_lo = reciprocal * reduced_err;

    // Accumulate low-order corrections: exponent rounding error, reduced_hi/reduced_lo conversion to log2,
    // log2(e) low part, and the polynomial tail.  The output satisfies log2(x) ~= log2_hi + log2_lo.
    log2_lo = exponent_part - log2_hi;
    log2_lo = fmaf(reduced_hi, log2e_hi, log2_lo);
    log2_lo = fmaf(reduced_lo, log2e_hi, log2_lo);
    log2_lo = fmaf(reduced_hi, log2e_lo, log2_lo);
    log2_lo = fmaf(reduced_lo, log_poly * 3.0f, log2_lo);
    log2_lo = fmaf(reduced_hi, log_poly, log2_lo);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_exp2f_reconstruct(float x_hi, float x_lo)
{
    constexpr float ln2 = __internal_fp32_ln2_hi; // ln(2), used by the local exp2 polynomial.
    constexpr float overflow_abs_bound = 152.0f;  // Beyond this range exp2f is guaranteed to overflow/underflow.
    constexpr int32_t fp32_exponent_shift = 23;   // Offset of the fp32 exponent field.

    // Split x into an integer exponent and a small fractional residual:
    // exp2(x_hi + x_lo) = 2^rounded_x * exp2(exp2_fraction).
    const float rounded_x = roundf(x_hi);
    const float exp2_fraction = (x_hi - rounded_x) + x_lo;
    const int32_t exp2_exponent = __float2int_rz(rounded_x);

    // Approximate 2^fraction as exp(fraction * ln2) on a small interval around zero.  The coefficients are
    // evaluated in the same FFMA order as the CUDA-derived implementation.
    float exp_poly = fmaf(exp2_fraction, 0.00015239251661114395f, 0.0013391353422775864601f);
    exp_poly = fmaf(exp2_fraction, exp_poly, 0.0096188392490148544312f);
    exp_poly = fmaf(exp2_fraction, exp_poly, 0.055503588169813156128f);
    exp_poly = fmaf(exp2_fraction, exp_poly, 0.24022644758224487305f);
    exp_poly = fmaf(exp2_fraction, exp_poly, ln2);
    exp_poly = fmaf(exp2_fraction, exp_poly, 1.0f);

    // Construct 2^rounded_x by multiplying two fp32 scale factors.  The split scale keeps both positive and
    // negative exponents representable without doing a slow generic pow/ldexp path.
    const bool rounded_x_is_positive = rounded_x > 0.0f;
    const uint32_t scale_hi_bits = rounded_x_is_positive ? 0x7F000000U : 0x02000000U;
    const uint32_t scale_adjust = rounded_x_is_positive ? 0U : 0x83000000U;
    const uint32_t scale_lo_bits = (static_cast<uint32_t>(exp2_exponent) << fp32_exponent_shift) - scale_adjust;
    float output = exp_poly * __uint_as_float(scale_hi_bits);
    output = output * __uint_as_float(scale_lo_bits);

    // Guard extreme inputs after polynomial reconstruction.  The sign of x_hi decides whether the result is
    // +inf or +0.
    if (fabsf(x_hi) > overflow_abs_bound) {
        output = x_hi >= 0.0f ? ASCRT_INF_F : 0.0f;
    }
    return output;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_with_sign_bit(float value, float sign_source)
{
    constexpr uint32_t sign_mask = 0x80000000U;
    constexpr uint32_t value_mask = 0x7FFFFFFFU;
    const uint32_t value_bits = __float_as_uint(value) & value_mask;
    const uint32_t sign_bits = __float_as_uint(sign_source) & sign_mask;
    return __uint_as_float(value_bits | sign_bits);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float expf(float x) { return __internal_expf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float logf(float x)
{
    if (x > 0.0f && x < 1.17549435e-38f) { // 1.17549435e-38f: subnormal floating-point number boundary
        return __logf(expf(23.0f) * x) - 23.0f;
    }
    return __logf(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log2f(float x)
{
    if (isnan(x)) {
        return x;
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sqrtf(float x) { return __internal_sqrtf(x); }

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

#define __INTERNAL_NANF(tagp)                                                                       \
    do {                                                                                            \
        unsigned long long i = 0;                                                                   \
        int c;                                                                                      \
        int ovfl = 0;                                                                               \
        int invld = 0;                                                                              \
        if (tagp && (*tagp == '0')) {                                                               \
            tagp++;                                                                                 \
            if ((*tagp == 'x') || (*tagp == 'X')) {                                                 \
                tagp++;                                                                             \
                while (*tagp == '0')                                                                \
                    tagp++;                                                                         \
                while (*tagp) {                                                                     \
                    if (i > 0x0fffffffffffffffULL) {                                                \
                        ovfl = 1;                                                                   \
                    }                                                                               \
                    c = (((*tagp) >= 'A') && ((*tagp) <= 'F')) ? (*tagp + 'a' - 'A') : (*tagp);     \
                    if ((c >= 'a') && (c <= 'f')) {                                                 \
                        c = c - 'a' + 10;                                                           \
                        i = i * 16 + c;                                                             \
                    } else if ((c >= '0') && (c <= '9')) {                                          \
                        c = c - '0';                                                                \
                        i = i * 16 + c;                                                             \
                    } else {                                                                        \
                        invld = 1;                                                                  \
                    }                                                                               \
                    tagp++;                                                                         \
                }                                                                                   \
            } else {                                                                                \
                while (*tagp == '0')                                                                \
                    tagp++;                                                                         \
                while (*tagp) {                                                                     \
                    if (i > 0x1fffffffffffffffULL) {                                                \
                        ovfl = 1;                                                                   \
                    }                                                                               \
                    c = *tagp;                                                                      \
                    if ((c >= '0') && (c <= '7')) {                                                 \
                        c = c - '0';                                                                \
                        i = i * 8 + c;                                                              \
                    } else {                                                                        \
                        invld = 1;                                                                  \
                    }                                                                               \
                    tagp++;                                                                         \
                }                                                                                   \
            }                                                                                       \
        } else if (tagp) {                                                                          \
            while (*tagp) {                                                                         \
                c = *tagp;                                                                          \
                if ((i > 1844674407370955161ULL) || ((i == 1844674407370955161ULL) && (c > '5'))) { \
                    ovfl = 1;                                                                       \
                }                                                                                   \
                if ((c >= '0') && (c <= '9')) {                                                     \
                    c = c - '0';                                                                    \
                    i = i * 10 + c;                                                                 \
                } else {                                                                            \
                    invld = 1;                                                                      \
                }                                                                                   \
                tagp++;                                                                             \
            }                                                                                       \
        }                                                                                           \
        if (ovfl) {                                                                                 \
            i = ~0ULL;                                                                              \
        }                                                                                           \
        if (invld) {                                                                                \
            i = 0ULL;                                                                               \
        }                                                                                           \
        i = (i & 0x000fffffffffffffULL) | 0x7ff8000000000000ULL;                                    \
        unsigned int i1 = (unsigned int)i;                                                          \
        i1 = (i1 & 0x007fffff) | 0x7fc00000;                                                        \
        union Data {                                                                                \
            unsigned int u;                                                                         \
            float f;                                                                                \
        };                                                                                          \
        union Data data = {.u = i1};                                                                \
        return data.f;                                                                              \
    } while (0)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float nanf(const char* tagp) { __INTERNAL_NANF(tagp); }
#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#if defined(__NPU_ARCH__) && !defined(ASCENDC_CPU_DEBUG)
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float nanf(const __gm__ char* tagp) { __INTERNAL_NANF(tagp); }
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float nanf(const __ubuf__ char* tagp) { __INTERNAL_NANF(tagp); }
#endif
#endif

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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_set_res_mod_neg(float mod_res)
{
    uint32_t* u_mod_res = reinterpret_cast<uint32_t*>(&mod_res);
    *u_mod_res = (*u_mod_res) | ASCRT_NEG_SIGN_BIT_U;
    return mod_res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float fmodf(float x, float y)
{
    bool is_x_pos = x > 0;
    float abs_x = fabsf(x);
    float abs_y = fabsf(y);
    bool is_x_nan = isnan(x);
    bool is_y_nan = isnan(y);

    bool is_inf_not_nan = isinf(abs_x) && !is_x_nan;
    bool is_zero_not_nan = (abs_y == 0) && !is_y_nan;
    if (is_inf_not_nan | is_zero_not_nan) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    if (is_y_nan || is_x_nan || abs_x < abs_y) {
        bool gt_inf_or_nan = (abs_y > ASCRT_INF_F) || is_x_nan || is_y_nan;
        float xy_val = (gt_inf_or_nan) ? (x + y) : x;
        bool lt_zero_or_nan = (abs_x <= 0) || is_x_nan;
        return (lt_zero_or_nan) ? (xy_val + x) : xy_val;
    }

    uint32_t* uabs_y = reinterpret_cast<uint32_t*>(&abs_y);
    uint32_t y_man_bits = (*uabs_y) & ASCRT_MAN_BIT_FLOAT_U;
    uint32_t* uabs_x = reinterpret_cast<uint32_t*>(&abs_x);
    uint32_t x_exp_bits = (*uabs_x) & ASCRT_EXP_BIT_FLOAT_U;
    uint32_t xy_bits = y_man_bits | x_exp_bits;

    float xy_val = 0;
    uint32_t* uxy_val = reinterpret_cast<uint32_t*>(&xy_val);
    *uxy_val = xy_bits;
    bool is_gt_x = (xy_val > abs_x) && !isnan(xy_val) && !is_x_nan;
    float half_xy_val = xy_val * 0.5f;
    xy_val = (is_gt_x) ? half_xy_val : xy_val;
    float mod_res = abs_x;

    if (xy_val < abs_y || isnan(xy_val) || is_y_nan) {
        if (!is_x_pos) {
            return __internal_set_res_mod_neg(mod_res);
        }
        return mod_res;
    }
    float sub_tmp;
    bool xy_val_ge_y = true;
    bool cmp_tmp;
    while (xy_val_ge_y) {
        sub_tmp = mod_res - xy_val;
        cmp_tmp = mod_res < xy_val || isnan(mod_res) || isnan(xy_val);
        mod_res = (cmp_tmp) ? mod_res : sub_tmp;
        xy_val = xy_val * 0.5f;
        xy_val_ge_y = (xy_val >= abs_y) || isnan(xy_val) || is_y_nan;
    }
    if (!is_x_pos) {
        return __internal_set_res_mod_neg(mod_res);
    }
    return mod_res;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float copysignf(float x, float y) { return __internal_with_sign_bit(x, y); }

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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanf(float x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atan2f(float y, float x)
{
    if (isnan(y)) {
        return y;
    } else if (isnan(x)) {
        return x;
    }

    int d = (y >= 0) ? 1 : -1;
    if (y == 0.0f) {
        if (x > 0.0f) {
            return y;
        }
        uint32_t x_bits = *reinterpret_cast<uint32_t*>(&x);
        if ((x_bits & ASCRT_NEG_SIGN_BIT_U) != 0) {
            uint32_t y_bits = *reinterpret_cast<uint32_t*>(&y);
            int zero_sign = ((y_bits & ASCRT_NEG_SIGN_BIT_U) != 0) ? -1 : 1;
            return zero_sign * ASCRT_PI_F;
        }
        return y;
    } else if (isinf(y) && isinf(x)) {
        int s = 1;
        if (x < 0) {
            s = 3; // 3 : ATAN2_THREE
        }
        return d * ASCRT_PIO4_F * s;
    } else if (isinf(y)) {
        return d * ASCRT_PIO2_F;
    } else if (isinf(x)) {
        if (x > 0) {
            d = 0;
        }
        return d * ASCRT_PI_F;
    }

    if (x == 0) {
        return d * ASCRT_PIO2_F;
    } else if (x > 0) {
        d = 0;
    }

    return atanf(y / x) + d * ASCRT_PI_F;
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
    float y = fabsf(x);
    const float tmp = expf(y - ASCRT_SCALAR_LN2_F);
    return tmp + 0.25f / tmp;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cospif(float x) { return cosf(x * ASCRT_PI_F); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float acoshf(float x)
{
    if (x < 1) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    return logf(x + sqrtf(x * x - 1.0f));
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float asinhf(float x)
{
    if (fabsf(x) > 0.1f) {
        return x > 0 ? logf(x + sqrtf(x * x + 1.0f)) : logf(sqrtf(x * x + 1.0f) - x) * (-1);
    } else {
        float square_v = 0;
        float dst = 0;
        float src = x;
        float factor[] = {
            1.0,
            -0.16666666666666666666666666666667,
            0.075,
            -0.04464285714285714285714285714286,
            0.03038194444444444444444444444444,
            -0.02237215909090909090909090909091,
            0.01735276442307692307692307692308,
            -0.01396484375,
        };
        __internal_taylor_expand(dst, src, square_v, 7, factor); // 7 : Taylor expansion count
        return dst;
    }
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, float* s, float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifdef __NPU_ARCH__
#ifndef ASCENDC_CPU_DEBUG
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __ubuf__ float* s, float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __ubuf__ float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __ubuf__ float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __gm__ float* s, float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __gm__ float* s, __ubuf__ float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void sincospif(float x, __gm__ float* s, __gm__ float* c)
{
    __INTERNAL_SINCOSF(x * ASCRT_PI_F, s, c);
}
#endif
#endif
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float powf(float x, float y);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float exp2f(float x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float exp10f(float x) { return powf(10.0f, x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float expm1f(float x) { return expf(x) - 1.0f; }

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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float logbf(float x)
{
    if (isnan(x)) {
        return x;
    }
    if (x < 0) {
        x = -x;
    }
    float inf = ASCRT_INF_F;
    if (isinf(x)) {
        return inf;
    }
    if (x == 0) {
        return -inf;
    }

    uint32_t fp32_inf_exponent = 255;
    uint32_t fp32_decimal_bit = 23;
    uint32_t fp32_sign_bit = 256;
    uint32_t fp32_exponent_h = 127;
    uint32_t* exponent = reinterpret_cast<uint32_t*>(&x);
    (*exponent) >>= fp32_decimal_bit;
    uint32_t sign = fp32_sign_bit;
    if ((*exponent) > sign) {
        (*exponent) -= sign;
    }
    if ((*exponent) == fp32_inf_exponent) {
        return inf;
    } else {
        float res = (*exponent);
        res -= fp32_exponent_h;
        return res;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline int32_t ilogbf(float x)
{
    if (x == 0.0f || isnan(x)) {
        return ASCRT_MIN_VAL_S;
    }
    if (isinf(x)) {
        return ASCRT_MAX_VAL_S;
    }
    if (x < 0) {
        x = -x;
    }
    return static_cast<int>(logbf(x));
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float cbrtf(float x)
{
    uint32_t x_bits = *reinterpret_cast<uint32_t*>(&x);
    int32_t exp_bits = (x_bits >> 23) & 0xFF;
    if (x == 0.0f || exp_bits == 0xFF) {
        return x;
    }

    // In order for Newtonian iteration method to converge quickly, we need to reduce x to a certain range(0.125, 8).
    // Depending on the computer's float number storage structure, we can adjust the exponential part of x.
    // the adjustment factor(k) ensures the exponent of x' is in (-3, 3)
    int32_t exponent = exp_bits - 127;
    int32_t k;
    if (exponent >= 3) {              // 3:ensures the exponent of x' is in (-3, 3)
        k = ((exponent - 3) / 3) + 1; // 3:ensures the exponent of x' is in (-3, 3)
    } else if (exponent <= -4) {      //-4:ensures the exponent of x' is in (-3, 3)
        k = (exponent + 1) / 3;       // 3:ensures the exponent of x' is in (-3, 3)
    } else {
        k = 0;
    }

    // get the adjusted x value
    int32_t exp_adjusted_bits = exponent - 3 * k + 127;
    uint32_t x_adjusted_bits = (x_bits & 0x7FFFFF) | (exp_adjusted_bits << 23);
    float x_adjusted = *reinterpret_cast<float*>(&x_adjusted_bits);

    // Newton's iteration method,f(x) = x^3 - b, x_i+1 = x_i - f(x_i)/f'(x_i) = (2*x_i + b/x_i^2)/3
    // the initial value of x_i = 1.0
    float y = 1.0f;
    y = (2.0f * y + x_adjusted / (y * y)) / 3.0f;
    y = (2.0f * y + x_adjusted / (y * y)) / 3.0f;
    y = (2.0f * y + x_adjusted / (y * y)) / 3.0f;
    y = (2.0f * y + x_adjusted / (y * y)) / 3.0f;
    y = (2.0f * y + x_adjusted / (y * y)) / 3.0f;

    // adjust the exponent of y by k
    uint32_t y_bits = *reinterpret_cast<uint32_t*>(&y);
    int32_t yexp_bits = ((y_bits >> 23) & 0xFF) + k;
    y_bits = (y_bits & 0x807FFFFF) | ((yexp_bits & 0xFF) << 23) | // 23:the number of bits to shift left
             (x_bits & 0x80000000);
    return *reinterpret_cast<float*>(&y_bits);
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
    float abs_x = fabsf(x);

    float poly = __internal_cal_poly(abs_x);

    float tmp2 = fmaf(2.0f, abs_x, 1.0f);
    float inv_tmp2 = 1.0f / tmp2;
    float q = poly * inv_tmp2;
    float t = fmaf(abs_x, q * -2.0f, poly);
    float u = t - q;
    float v = fmaf(u, inv_tmp2, q);

    float x_squared = abs_x * abs_x;
    float neg_x2 = -x_squared;
    float f1 = 1.442695f;
    float scaled = neg_x2 * f1;
    float int_part = scaled > 0 ? __floorf(x) : __ceilf(x);
    float abs_part = fabsf(int_part);
    uint32_t sign_bit = *reinterpret_cast<uint32_t*>(&int_part) & 0x80000000;
    float clamped_bits = sign_bit | 0x42FC0000;
    float clamped = *reinterpret_cast<float*>(&clamped_bits);
    float safe_int = (abs_part > 126.0f) ? clamped : int_part;

    float remainder = fmaf(safe_int, -0.6931472f, neg_x2);
    remainder = fmaf(safe_int, 1.9046542e-9f, remainder);
    float exponent_arg = remainder * f1;
    float exponent_base = safe_int + 12583039.0f;
    uint32_t exponent_bits = *reinterpret_cast<uint32_t*>(&exponent_base) << 23;
    float exponent_scale = *reinterpret_cast<float*>(&exponent_bits);
    float exp_val = exp2f(exponent_arg) * exponent_scale;

    float term3 = fmaf(-abs_x, abs_x, x_squared);
    float term4 = fmaf(exp_val, term3, exp_val);
    float result = v * term4;

    if (abs_x > 10.055f) {
        result = 0.0f;
    }

    return (x < 0) ? (2.0f - result) : result;
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
    // (4 * n^2 - 1) / (8 * x) + (4 * n^2 - 1) * (4 * n^2 - 25) / (384 * x^3) + (4 * n^2 - 1) * (16 * n^4 - 456 * n^2 +
    // 1073) / (5120 * x^5)
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float j0f(float x)
{
    if (isnan(x)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float f1 = fabsf(x);
    if (f1 > 1e13f && isfinite(f1)) {
        return 0;
    }
    if (f1 > 8.0f) {
        return __internal_cal_j0_x_larger8(f1);
    } else {
        return __internal_cal_j0_x_less8(f1);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float j1f(float x)
{
    if (isnan(x)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float f1 = fabsf(x);
    if (f1 > 1e13f && isfinite(f1)) {
        return 0;
    }
    float res;
    if (f1 > 8.0f) {
        res = __internal_cal_j1_x_lager8(f1);
    } else {
        res = __internal_cal_j1_x_less8(f1);
    }
    return (x < 0) ? -res : res;
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

#if defined(ASCENDC_USE_LEGACY_PRECISION)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float powf(float x, float y) { return __powf(x, y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float exp2f(float x) { return powf(2.0f, x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normcdff(float x)
{
    if (fabsf(x) > 14.5f) {
        x = copysignf(14.5f, x);
    }

    float one_over_sqrt2_high = -0.707106769f; // -0.707106769f: -1/sqrt(2) high
    float x_over_sqrt2_high = x * one_over_sqrt2_high;
    float compensate_value = fmaf(x, one_over_sqrt2_high, -x_over_sqrt2_high);

    float one_over_sqrt2_low = -1.21016175e-8f; // -1.21016175e-8f: -1/sqrt(2) low
    float x_over_sqrt2_low = fmaf(x, one_over_sqrt2_low, compensate_value);
    float x_over_sqrt2 = x_over_sqrt2_high + x_over_sqrt2_low;

    float erfc_value = erfcf(x_over_sqrt2);
    if (x <= -1.0f) {
        erfc_value =
            fmaf(-2.0f * x_over_sqrt2 * erfc_value, x_over_sqrt2_high - x_over_sqrt2 + x_over_sqrt2_low, erfc_value);
    }
    return 0.5f * erfc_value;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rcbrtf(float x)
{
    if (x == 0.0f) {
        return ASCRT_INF_F;
    }
    if (isnan(x)) {
        return x;
    }
    if (isinf(x)) {
        return 0.0f;
    }

    // get the exponent part of x
    uint32_t x_bits = *reinterpret_cast<uint32_t*>(&x);
    int32_t exp_bits = (x_bits >> 23) & 0xFF;

    // Depending on the computer's float number storage structure
    // The exponent bits of x is E = (x >> 23) && 0xFF
    // The exponent value is e = E - 127
    // The exponent value of rcbrt(x) is e' = -e/3
    // The exponent bits of rcbrt(x) is E' = round(127 + e') = 127 - e/3 = (3*127 - e)/3 = (508 - E) / 3
    // Assume that the initial value of the Newton's iteration method is y, the exponent bits of y is E'
    int32_t yexp_bits = (508 - exp_bits) / 3;
    uint32_t y_bits = (x_bits & 0x80000000) | (yexp_bits << 23);
    float y = *reinterpret_cast<float*>(&y_bits);

    // The Newton's iteration method, f(x) = x^(-3) - b;
    // x_i+1 = x_i - f(x_i)/f'(x_i)
    // x_i+1 = x_i - (x_i^(-3) - b)/(-3*x_i^(-4))
    // x_i+1 = x_i*(4 - b * x_i^3) / 3
    y = y * (4.0f - x * y * y * y) / 3.0f;
    y = y * (4.0f - x * y * y * y) / 3.0f;
    y = y * (4.0f - x * y * y * y) / 3.0f;
    y = y * (4.0f - x * y * y * y) / 3.0f;
    y = y * (4.0f - x * y * y * y) / 3.0f;
    return y;
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __internal_cal_remquo(
    float& abs_x, float& n_x_y_val, uint32_t& sign_flag, float& abs_y, float& tmp_val, int32_t* quo)
{
    bool is_x_lt_xy = abs_x < n_x_y_val;
    sign_flag = 0;
    bool is_x_y_ge_y = true;
    float neg_two = -2.0;
    float pos_two = 2.0;
    int32_t n = 0;
    while (is_x_y_ge_y) {
        n = n + n;
        if (is_x_lt_xy) {
            n_x_y_val = n_x_y_val * 0.5f;
            is_x_y_ge_y = n_x_y_val >= abs_y;
            if (is_x_y_ge_y) {
                is_x_lt_xy = abs_x < n_x_y_val;
                sign_flag = 0;
                continue;
            }
            break;
        }
        tmp_val = (pos_two * abs_x) + (n_x_y_val * neg_two);
        abs_x = abs_x - n_x_y_val;
        sign_flag = 1;
        n += 1;
        n_x_y_val = n_x_y_val * 0.5f;
        is_x_y_ge_y = n_x_y_val >= abs_y;
        if (is_x_y_ge_y) {
            is_x_lt_xy = abs_x < n_x_y_val;
            sign_flag = 0;
        }
    }
    *quo = n;
}

#define __INTERNAL_REMQUOF(x, y, quo)                                                                            \
    do {                                                                                                         \
        bool is_x_pos = (x) >= 0;                                                                                \
        float abs_x = fabsf(x);                                                                                  \
        float abs_y = fabsf(y);                                                                                  \
        bool is_x_inf = abs_x > ASCRT_INF_F || isnan(x);                                                         \
        bool is_y_inf = abs_y > ASCRT_INF_F || isnan(y);                                                         \
        *(quo) = 0;                                                                                              \
        int32_t tmp_quo = 0;                                                                                     \
        int32_t n_sign = (((x) <= 0 && (y) <= 0) || ((x) >= 0 && (y) >= 0)) ? 1 : -1;                            \
        float res = (x) + (y);                                                                                   \
        if (is_x_inf | is_y_inf) {                                                                               \
            return res;                                                                                          \
        }                                                                                                        \
                                                                                                                 \
        res = ASCRT_INF_F / ASCRT_INF_F;                                                                         \
        if ((abs_x == ASCRT_INF_F) || (abs_y == 0)) {                                                            \
            return res;                                                                                          \
        }                                                                                                        \
                                                                                                                 \
        float tmp_val = 0.0;                                                                                     \
        uint32_t sign_flag = 0;                                                                                  \
        if (abs_x < abs_y) {                                                                                     \
            res = (x);                                                                                           \
            float result = __internal_x_le_y(abs_x, tmp_val, abs_y, is_x_pos, sign_flag, res, &tmp_quo, n_sign); \
            *(quo) = tmp_quo;                                                                                    \
            return result;                                                                                       \
        }                                                                                                        \
                                                                                                                 \
        uint32_t* u_abs_y = reinterpret_cast<uint32_t*>(&abs_y);                                                 \
        uint32_t u_y = (*u_abs_y) & ASCRT_MAN_BIT_FLOAT_U;                                                       \
        uint32_t* u_abs_x = reinterpret_cast<uint32_t*>(&abs_x);                                                 \
        uint32_t u_x = (*u_abs_x) & ASCRT_EXP_BIT_FLOAT_U;                                                       \
        float x_y_val = 0.0;                                                                                     \
        uint32_t* uf26 = reinterpret_cast<uint32_t*>(&x_y_val);                                                  \
        *uf26 = u_y | u_x;                                                                                       \
        bool is_gt_abs_x = x_y_val > abs_x && !isnan(x_y_val);                                                   \
        res = 0.0;                                                                                               \
        float n_x_y_val = (is_gt_abs_x) ? (x_y_val * 0.5f) : x_y_val;                                            \
        if (abs_x == n_x_y_val && !isnan(n_x_y_val)) {                                                           \
            return res;                                                                                          \
        }                                                                                                        \
                                                                                                                 \
        tmp_val = 0.0;                                                                                           \
        res = abs_x;                                                                                             \
        *(quo) = 0;                                                                                              \
        if (n_x_y_val < abs_y || isnan(n_x_y_val)) {                                                             \
            float result = __internal_x_le_y(abs_x, tmp_val, abs_y, is_x_pos, sign_flag, res, &tmp_quo, n_sign); \
            *(quo) = tmp_quo;                                                                                    \
            return result;                                                                                       \
        }                                                                                                        \
        __internal_cal_remquo(abs_x, n_x_y_val, sign_flag, abs_y, tmp_val, &tmp_quo);                            \
        res = abs_x;                                                                                             \
                                                                                                                 \
        float result = __internal_x_le_y(abs_x, tmp_val, abs_y, is_x_pos, sign_flag, res, &tmp_quo, n_sign);     \
        *(quo) = tmp_quo;                                                                                        \
        return result;                                                                                           \
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float remainderf(float x, float y)
{
    int32_t quo = -1;
    return remquof(x, y, &quo);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rhypotf(float x, float y) { return 1.0f / hypotf(x, y); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnorm3df(float a, float b, float c) { return 1.0f / norm3df(a, b, c); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnorm4df(float a, float b, float c, float d)
{
    return 1.0f / norm4df(a, b, c, d);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float scalbnf(float x, int32_t n)
{
    if (isinf(x) || isnan(x)) {
        return x;
    } else if (x == 0) {
        return x;
    }

    float two = 2.0;
    float fp32_exponent_mid_val = 127;

    if (n < 0) {
        n = -n;
        if (n > fp32_exponent_mid_val) {
            int mul_val_exp = n - fp32_exponent_mid_val;
            n = fp32_exponent_mid_val;
            x = x / __powf(two, static_cast<float>(mul_val_exp));
        }
        return x / __powf(two, n);
    }
    if (n > fp32_exponent_mid_val) {
        int mul_val_exp = n - fp32_exponent_mid_val;
        n = fp32_exponent_mid_val;
        x = x * __powf(two, static_cast<float>(mul_val_exp));
    }
    return x * __powf(two, static_cast<float>(n));
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float scalblnf(float x, int64_t n) { return scalbnf(x, static_cast<int32_t>(n)); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinpif(float x) { return sinf(x * ASCRT_PI_F); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanpif(float x) { return tanf(x * ASCRT_PI_F); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_euler_gamma_function(float x)
{
    float frac = x - nearbyintf(x);
    //  1/gamma(x + 1)
    //  = 1 + γx + (γ^2 - pi^2/6) * x^2/2! + O(3)
    float y = -0.00107286568f;         // -0.00107286568f  : Coefficient of O(8)
    y = fmaf(y, frac, 0.00711105345f); // 0.00711105345f   : Coefficient of O(7)
    y = fmaf(frac, y, -0.0096437186f); // -0.0096437186f   : Coefficient of O(6)
    y = fmaf(frac, y, -0.042180188f);  // -0.042180188f    : Coefficient of O(5)
    y = fmaf(frac, y, 0.166540906f);   // 0.166540906f     : Coefficient of O(4)
    y = fmaf(frac, y, -0.0420036502f); // -0.0420036502f   : Coefficient of O(3)
    y = fmaf(frac, y, -0.655878186f);  // -0.655878186f    : [0.577*0.577-pi*pi/6]/2
    y = fmaf(frac, y, 0.577215672f);   // 0.577215672f     : Euler-Mascheroni constant
    y = fmaf(frac, y, 1.0f);

    if (x < -0.5f) {
        //  1/gamma(x)
        //  = 1/gamma(frac-1)
        //  = 1/[frac*(frac-1)*gamma(frac+1)] = 1/[frac*x*gamma(frac+1)]
        y = y * x * frac;
    }
    if (x <= 0.5f && x >= -0.5f) {
        //  1/gamma(x)
        //  = 1/gamma(frac)
        //  = 1/[frac*gamma(x+1)]
        y = y * frac;
    }
    //  1/(1/gamma(x))
    if (fabsf(y) < 1.1754943e-38f) {
        int32_t e = 0;
        float m = frexpf(y, &e);
        return ldexpf(1.0f / m, 0 - e);
    } else {
        return 1.0f / y;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_abs_x(float x)
{
    float abs_x = fabsf(x);
    if (abs_x > 41.0999985f) {
        x = copysignf(41.0999985f, x);
        abs_x = fabsf(x);
    }
    return abs_x;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_stirling_and_euler_reflection(float x)
{
    float abs_x = __internal_cal_abs_x(x);

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
    uint32_t u32 = reinterpret_cast<uint32_t&>(abs_x);
    int32_t exp_u32 = (u32 - 1060439283) & 0xFF800000; // 0xFF800000: 2^128
    int32_t man_u32 = u32 - exp_u32;
    float mantissa = *reinterpret_cast<float*>(&man_u32);
    float exponent = fmaf(static_cast<float>(exp_u32), 1.1920929e-07f, 0.0f); // 1.1920929e-07 : 2^-23
    float ln_mantissa = 2.0f / (mantissa + 1.0f) * (mantissa - 1.0f);

    //  log(x) = log(m*2^exp) = log(m) + exp= ln(m)/loge + exp
    float log_x = fmaf(ln_mantissa, 1.44269502f, exponent); //  1.44269502f : log_2(e)

    //  Calculates log(x)'s error-value
    float log_x_diff = fmaf(ln_mantissa, 1.44269502f, exponent - log_x); //  1.44269502f : log_2(e)

    float y3 = __internal_cal_y3(ln_mantissa);

    float r = 2.0f * (mantissa - 1.0f - ln_mantissa) - ln_mantissa * (mantissa - 1.0f); // 2.0 :
    log_x_diff = fmaf(1.0f / (mantissa + 1.0f) * r, 1.44269502f, log_x_diff);           //  1.44269502f : log_2(e)
    log_x_diff = fmaf(ln_mantissa, 1.92513667e-08f, log_x_diff); // 1.92513667e-08f : Coefficient of O(1)
    log_x_diff = fmaf(y3, ln_mantissa, log_x_diff);

    float diff0 = log_x - (log_x + log_x_diff) + log_x_diff;
    log_x = log_x + log_x_diff;

    //  Calculates the exponent of Stirling's approximation
    float y01 = log_x * (abs_x - 0.5f); //  0.5f : Coefficient of sqrt(x)
    float y02 = 1.44269502f * abs_x;    //  1.44269502f : log_2(e)
    float y0 = y01 - y02;

    //  Calculates the exponent[y01] error-value
    float diff1 = fmaf(log_x, abs_x - 0.5f, -y01);
    diff1 = fmaf(diff0, abs_x - 0.5f, diff1);

    //  Calculates the exponent[y02] error-value
    float diff2 = fmaf(1.44269502f, abs_x, -y02); //  1.44269502f : log_2(e)
    diff2 = fmaf(1.92596303e-08f, abs_x, diff2);
    float y0_diff = (diff1 - diff2) - (y0 - y01 + y02);

    float offset = 0.0f;
    if (abs_x > 33.0f) { // 33.0f : threshold
        offset = 48.0f;
    }
    if (x < 0.0f) {
        y0 = offset - y0;
        y0_diff = -y0_diff;
    }

    //  Split a float-value into integer[i] and decimal[f]
    float i = nearbyintf(y0);
    float f = y0 - i + y0_diff;

    // 2^f * 2^i * sqrt(2*pi)
    float y5 = powf(2.0f, f) * powf(2.0f, i) * 2.5066282f; //  2.5066282f : sqrt(2*PI)

    //  Calculate Stirling's approximation remainder minus 1
    //  y6 = {[1 + 1/(12*x) + 1/(288*x^2) - 139/(51840*x^3) - 571/(2488320*x^4)] - 1}*x^-1
    float y6 = __internal_cal_y6(abs_x);
    if (x > 0) {
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
        float sinpi = __internal_compute_sinpi(abs_x);

        //  (y6 * x * sinpi)'s Error value
        float y_diff = fmaf(y6 * x, sinpi, -y6 * x * sinpi);
        float y7 = 1 / (y6 * x * sinpi);

        return __internal_cal_y(abs_x, y_diff, y5, y7);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tgammaf(float x)
{
    if (x == 0.0f) {
        return 1.0f / x;
    }
    if (x < 0.0f && nearbyintf(x) == x) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float abs_x = fabsf(x);
    if (abs_x < 1.5f) {
        return __internal_euler_gamma_function(x);
    } else {
        return __internal_stirling_and_euler_reflection(x);
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanhf(float x) { return 1.0f - (2.0f / (expf(2.0f * x) + 1.0f)); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanf(float x)
{
    if (isnan(x)) {
        return x;
    }
    float clip = fminf(x, 10000.0f); // 10000 : MAX_INPUT_VALUE
    clip = fmaxf(clip, -10000.0f);   // -10000 : MIN_INPUT_VALUE
    float abs_v = fabsf(clip);

    float dst = 0;
    float square_v = 0;
    float tmp = 0;
    float tmp2 = 0;

    __internal_taylor_expand(dst, abs_v, square_v, 4);            // 4 : Taylor expansion count
    __internal_atan_expand(tmp, abs_v, tmp2, 0.4142135623730950); // 0.4142135623730950 : TAN_PI_OF_8
    __internal_taylor_expand(tmp2, tmp, square_v, 4);             // 4 : Taylor expansion count

    tmp2 = tmp2 + ASCRT_PIO8_F;
    dst = fminf(dst, tmp2);

    tmp2 = abs_v + 1.0f;
    tmp = abs_v - 1.0f;
    tmp = tmp / tmp2;
    tmp = fabsf(tmp);

    __internal_taylor_expand(tmp2, tmp, square_v, 4); // 4 : Taylor expansion count
    tmp2 = tmp2 + ASCRT_PIO4_F;
    dst = fminf(dst, tmp2);

    __internal_atan_expand(tmp2, tmp, square_v, 0.4142135623730950); // 0.4142135623730950 : TAN_PI_OF_8
    __internal_taylor_expand(tmp, tmp2, square_v, 6);                // 6 : Taylor expansion count

    tmp = tmp + ASCRT_PIO8_F;
    tmp = tmp + ASCRT_PIO4_F;
    dst = fminf(dst, tmp);

    __internal_sign(tmp, clip, tmp2);

    dst = dst * tmp;
    return dst;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanhf(float x) { return logf((1.0f + x) / (1.0f - x)) / 2.0f; }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float asinf(float x)
{
    if (fabsf(x) > 1) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }
    float square_v = 0;
    float dst = 0;
    float src = x;
    float factor[] = {
        1.0,
        0.16666666666666666666666666666667,
        0.075,
        0.04464285714285714285714285714286,
        0.03038194444444444444444444444444,
        0.02237215909090909090909090909091,
        0.01735276442307692307692307692308,
        0.01396484375,
    };
    if (fabsf(x) <= 0.7071067811865476f) {                       // 0.7071067811865476 : SCALAR_ACOS_MAX_LIMIT
        __internal_taylor_expand(dst, src, square_v, 7, factor); // 7 : Taylor expansion count
        return dst;
    } else if (x < -0.7071067811865476f) { // -0.7071067811865476 : SCALAR_ACOS_MIN_LIMIT
        src = sqrtf(1.0f - x * x);
        __internal_taylor_expand(dst, src, square_v, 7, factor); // 7 : Taylor expansion count
        return dst - ASCRT_PIO2_F;
    } else {
        src = sqrtf(1.0f - x * x);
        __internal_taylor_expand(dst, src, square_v, 7, factor); // 7 : Taylor expansion count
        return ASCRT_PIO2_F - dst;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float acosf(float x) { return ASCRT_PIO2_F - asinf(x); }

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinhf(float x)
{
    if (fabsf(x) > 0.1f) {
        return expf(x - ASCRT_SCALAR_LN2_F) - expf(x * (-1.0f) - ASCRT_SCALAR_LN2_F);
    } else {
        float square_v = 0;
        float dst = 0;
        float src = x;
        float factor[] = {
            1.0,
            0.16666666666666666666666666666667,
            0.00833333333333333333333333333333,
            0.0001984126984126984,
            2.7557319223985893e-06,
            2.505210838544172e-08};
        __internal_taylor_expand(dst, src, square_v, 5, factor); // 5: Taylor expansion count
        return dst;
    }
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log1pf(float x) { return logf(1.0f + x); }

#else

/**
 * Computes x raised to the power y for float inputs.
 *
 * The implementation first handles powf special cases whose result is fixed
 * by IEEE-style semantics, such as y == 0, x == 1, NaN inputs, signed zero,
 * signed infinity, and negative bases with non-integer exponents.
 *
 * For finite nonzero magnitudes, the main calculation uses:
 *   powf(x, y) = exp2(y * log2(abs(x)))
 *
 * log2(abs(x)) is split into high and low parts:
 *   log2(abs(x)) ~= log2_hi + log2_lo
 *
 * The split preserves the residual when multiplying by y:
 *   product_hi = (log2_hi + log2_lo) * y
 *   product_lo = fma(log2_abs_x, y, -product_hi) + correction
 *
 * The final magnitude is reconstructed by the exp2 helper:
 *   output = exp2(product_hi + product_lo)
 *
 * For negative x, the magnitude is computed from abs(x). The sign is applied
 * only when y is an odd integer; negative bases with non-integer y return NaN.
 *
 * @param x The base value.
 * @param y The exponent value.
 * @return The computed x^y value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float powf(float x, float y)
{
    const float abs_x = fabsf(x);
    const float abs_y = fabsf(y);

    // pow(x, 0) and pow(1, y) are exactly 1.  These cases have priority over NaN propagation by the
    // documented powf semantics.
    if (y == 0.0f || x == 1.0f) {
        return 1.0f;
    }

    // Propagate NaN after the identity cases above.  x + y keeps the NaN payload behavior close to hardware
    // arithmetic.
    if (isnan(x) || isnan(y)) {
        return ASCRT_NAN_F;
    }

    // For signed zero and signed infinity, magnitude is determined only by the sign of y; the output sign is
    // kept only when y is an odd integer.
    const float half_y_trunc = truncf(y * 0.5f);
    const float odd_remainder = y - (half_y_trunc + half_y_trunc);
    const bool exponent_is_odd_integer = fabsf(odd_remainder) == 1.0f;
    if (x == 0.0f || abs_x == ASCRT_INF_F) {
        float special_output = x;
        if (y < 0.0f) {
            special_output = abs_x == ASCRT_INF_F ? 0.0f : ASCRT_INF_F;
            // Preserve the sign of x for odd powers such as pow(-0, -3) and pow(-inf, -3).
            special_output = __internal_with_sign_bit(special_output, x);
        }
        return exponent_is_odd_integer ? special_output : fabsf(special_output);
    }

    // For |y| == inf and finite nonzero x, the result depends only on whether |x| is below or above 1.
    if (abs_y == ASCRT_INF_F) {
        if (x == -1.0f) {
            return 1.0f;
        }
        const bool result_is_inf = (abs_x > 1.0f && y > 0.0f) || (abs_x < 1.0f && y < 0.0f);
        return result_is_inf ? ASCRT_INF_F : 0.0f;
    }

    bool output_is_negative = false;
    if (x < 0.0f) {
        // Negative bases are only valid for integer exponents in the real-valued powf result.
        const float y_floor = floorf(y);
        if (y_floor != y) {
            return ASCRT_INF_F / ASCRT_INF_F;
        }
        // The main path computes pow(abs(x), y); apply the sign at the end for odd integer exponents.
        output_is_negative = exponent_is_odd_integer;
    }

    // Main finite path:
    //   pow(x, y) = exp2(y * log2(abs(x))).
    // log2 is returned as high/low parts so the multiplication by y can preserve the low-order residual.
    float log2_hi = 0.0f;
    float log2_lo = 0.0f;
    __internal_log2f_positive_finite(abs_x, log2_hi, log2_lo);
    const float log2_abs_x = log2_hi + log2_lo;

    // Multiply y by the high/low log2 value.  product_hi is the rounded main product; product_lo restores the
    // FMA residual and the low part of log2(abs_x), improving accuracy before exp2 reconstruction.
    const float product_hi = log2_abs_x * y;
    float product_lo = fmaf(log2_abs_x, y, -product_hi);
    product_lo = fmaf(log2_lo - (log2_abs_x - log2_hi), y, product_lo);

    // Reconstruct exp2(product_hi + product_lo), then attach the sign required by negative odd-integer bases.
    float output = __internal_exp2f_reconstruct(product_hi, product_lo);
    return output_is_negative ? -output : output;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float exp2f(float x)
{
    if (isnan(x)) {
        return x;
    }
    if (x == ASCRT_INF_F) {
        return ASCRT_INF_F;
    }
    if (x == -ASCRT_INF_F) {
        return 0.0f;
    }
    return __internal_exp2f_reconstruct(x, 0.0f);
}

/**
 * Computes the standard normal cumulative distribution function for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, CDF(-inf) = 0, and CDF(+inf) = 1.
 *
 * For finite inputs, it evaluates the CDF through the complementary error function:
 *   normcdff(x) = 0.5 * erfc(-x / sqrt(2))
 *
 * The argument is reduced with a high/low split of -1/sqrt(2):
 *   z = x * (-1/sqrt(2)) ~= z_hi + z_lo
 *
 * The tail is then approximated with a rational/polynomial form in:
 *   q = (|z| - 4) / (|z| + 4)
 *
 * The exponential factor is reconstructed with an exp2-based split path to keep
 * the tail stable:
 *   exp(-z^2) ~= exp_scale * exp2_residual
 *
 * Finally, the erfc tail is assembled, mirrored for negative x, and scaled by 1/2.
 *
 * @param x The input value.
 * @return The computed normcdff(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float normcdff(float x)
{
    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // CDF(-inf) = 0.
    if (x == -ASCRT_INF_F) {
        return 0.0f;
    }
    // CDF(+inf) = 1.
    if (x == ASCRT_INF_F) {
        return 1.0f;
    }

    // Clamp the input range used by the approximation to avoid overflow in the tail path.
    constexpr float clamp_bound = 14.5f;                     // Input clamp used to keep the tail approximation stable.
    constexpr float inv_sqrt2_hi = -0.70710676908493041992f; // High part of -1/sqrt(2).
    constexpr float inv_sqrt2_lo =
        -1.2101617485882343317e-08f;               // Low part of -1/sqrt(2) for split-constant compensation.
    constexpr float log2e = __internal_fp32_log2e; // log2(e), used to convert the exponent from ln to exp2 domain.
    constexpr float neg_ln2_hi = -__internal_fp32_ln2_hi; // High part of -ln(2).
    constexpr float ln2_tail = __internal_fp32_ln2_tail;  // Low tail of -ln(2) for residual correction.

    // Reduce x to z = -x/sqrt(2) with a hi/lo split for better precision.
    const float abs_x = fabsf(x);
    float clamped_x = abs_x > clamp_bound ? __internal_with_sign_bit(clamp_bound, x) : x;
    const bool ge_neg_one = !(clamped_x < -1.0f);

    float z_hi = clamped_x * inv_sqrt2_hi;
    float z_comp = fmaf(clamped_x, inv_sqrt2_hi, -z_hi);
    float z_lo = fmaf(clamped_x, inv_sqrt2_lo, z_comp);
    float z = z_hi + z_lo;
    const float abs_z = fabsf(z);
    const bool z_nonnegative = !(z < 0.0f);

    // Build q = (|z| - 4) / (|z| + 4) with a residual compensation term.
    const float rcp_abs_z_plus_4 = 1.0f / (abs_z + 4.0f);
    float q = (abs_z - 4.0f) * rcp_abs_z_plus_4;
    float q_residual = fmaf(q + 1.0f, -4.0f, abs_z);
    q_residual = fmaf(abs_z, -q, q_residual);
    q = fmaf(rcp_abs_z_plus_4, q_residual, q);

    // Approximate exp(-z^2) using exp2 with split hi/lo reconstruction.
    float exp_k = truncf((z * z) * -log2e);
    const bool exp_k_out_of_range = fabsf(exp_k) > __internal_fp32_max_exp;
    float exp_k_limited = exp_k_out_of_range ? __internal_with_sign_bit(__internal_fp32_max_exp, exp_k) : exp_k;
    float exp_residual = fmaf(exp_k_limited, neg_ln2_hi, -(z * z));
    exp_residual = fmaf(exp_k_limited, ln2_tail, exp_residual);
    float exp2_residual = exp2f(exp_residual * log2e);
    const uint32_t exp_scale_bits = static_cast<uint32_t>(__float2int_rz(exp_k_limited + 12583039.0f))
                                    << __internal_fp32_exponent_shift;
    float exp_scale = __uint_as_float(exp_scale_bits);
    float square_residual = fmaf(-abs_z, abs_z, z * z);
    float exp_term = fmaf(exp_scale, square_residual, exp_scale) * exp2_residual;

    // Polynomial approximation for the erfc rational core.
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

    // Rational reconstruction for the erfc tail.
    const float rcp_two_abs_z_plus_1 = 1.0f / fmaf(abs_z, 2.0f, 1.0f);
    float rational = poly * rcp_two_abs_z_plus_1;
    float rational_err = fmaf(abs_z, rational * -2.0f, poly);
    rational_err = -rational + rational_err;
    rational = fmaf(rcp_two_abs_z_plus_1, rational_err, rational);

    // Combine rational core with exponential tail.
    float erfc_value = rational * exp_term;
    if (abs_z > 10.05500030517578125f) {
        erfc_value = 0.0f;
    }
    // For negative inputs, use erfc(-z) = 2 - erfc(z).
    if (!z_nonnegative) {
        erfc_value = 2.0f - erfc_value;
    }
    // Restore the original x sign after clamping, correcting the reduced input path.
    if (!ge_neg_one) {
        const float correction_scale = z * -2.0f * erfc_value;
        erfc_value = fmaf(z_hi - z + z_lo, correction_scale, erfc_value);
    }
    return erfc_value * 0.5f;
}

/**
 * Computes the reciprocal cube root for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, rcbrtf(0) = signed infinity, and rcbrtf(inf) = signed zero.
 *
 * For finite nonzero inputs, it uses the identity:
 *   rcbrtf(x) = exp2(-log2(|x|) / 3)
 *
 * Subnormal magnitudes are scaled into the normal range before taking log2:
 *   |x| * 2^24
 * and the exponent correction is applied back as -24.
 *
 * A Newton-style refinement then corrects the initial exp2 estimate:
 *   y <- y + (1 - |x| * y^3) * (y / 3)
 *
 * For negative x, the magnitude is computed from |x| and the sign is restored
 * at the end.
 *
 * @param x The input value.
 * @return The computed rcbrtf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rcbrtf(float x)
{
    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // rcbrtf(0) = signed infinity.
    if (x == 0.0f) {
        return __internal_with_sign_bit(ASCRT_INF_F, x);
    }
    // rcbrtf(inf) = signed zero.
    if (isinf(x)) {
        return __internal_with_sign_bit(0.0f, x);
    }

    // Subnormal magnitudes are scaled into the normal range before taking log2.

    constexpr float one_third = 0.3333333432674407959f; // 1/3, used in the reciprocal cube-root exponent.

    // Compute an initial magnitude estimate from rcbrtf(x) = exp2(-log2(|x|)/3).
    const float abs_x = fabsf(x);
    const bool is_normal_or_larger = abs_x >= __internal_subnormal_boundary;
    float log_input = is_normal_or_larger ? abs_x : abs_x * __internal_fp32_scale_2p24;
    float exponent = log2f(log_input);
    if (!is_normal_or_larger) {
        exponent = exponent + __internal_fp32_subnormal_exponent_fix;
    }

    // Reconstruct the magnitude and apply one Newton-style correction step.
    float y = exp2f(exponent * -one_third);
    const float y_square = y * y;
    const float abs_x_times_y = abs_x * y;
    const float correction = fmaf(y_square, -abs_x_times_y, 1.0f);
    y = fmaf(correction, y * one_third, y);
    // Restore the sign for negative inputs.
    if (x < 0.0f) {
        y = -y;
    }
    return y;
}

/**
 * Extracts the mantissa bits from the absolute-value bit pattern of a float.
 *
 * For normal inputs, the hidden leading 1 is restored into the mantissa field.
 * For subnormal inputs, the raw mantissa bits are returned unchanged.
 *
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ static inline uint32_t __asc_float_mantissa_bits_from_abs_bits(uint32_t bits)
{
    const uint32_t exponent_bits = bits & ASCRT_EXP_BIT_FLOAT_U;
    const uint32_t mantissa_bits = bits & ASCRT_MAN_BIT_FLOAT_U;
    return exponent_bits == 0U ? mantissa_bits : (mantissa_bits | 0x00800000U);
}

/**
 * Returns the effective binary exponent for a float bit pattern.
 *
 * Subnormal inputs are treated as having exponent -149, while normal inputs
 * are converted from biased exponent bits to the corresponding unbiased value.
 *
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ static inline int32_t __asc_float_mantissa_exponent_from_abs_bits(uint32_t bits)
{
    const uint32_t exponent_bits = bits & ASCRT_EXP_BIT_FLOAT_U;
    return exponent_bits == 0U ? -__internal_fp32_subnormal_bias :
                                 (static_cast<int32_t>(exponent_bits >> __internal_fp32_exponent_shift) - 150);
}

/**
 * Computes floor(log2(x)) for an unsigned integer by repeated shifting.
 *
 * This helper is used to locate the highest set bit in a mantissa-like value,
 * which in turn determines how to normalize the reconstructed remainder.
 *
 * @param x Input unsigned integer.
 * @return The floor of log2(x), or 0 for x == 0.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ static inline int32_t __asc_uint_floor_log2(uint32_t x)
{
    int32_t shift = 0;
    while ((x >> 1U) != 0U) {
        x >>= 1U;
        ++shift;
    }
    return shift;
}

/**
 * Reconstructs a float from a mantissa integer and an exponent.
 *
 * The mantissa is normalized when possible, with separate handling for:
 * - zero mantissa,
 * - overflow to infinity,
 * - underflow to zero,
 * - normal-range results,
 * - subnormal results.
 *
 */
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
    if (result_exponent < -__internal_fp32_subnormal_bias) {
        return 0.0f;
    }
    if (result_exponent >= -126) {
        const uint32_t normalized_mantissa = mantissa << static_cast<uint32_t>(23 - mantissa_log2);
        const uint32_t bits = (static_cast<uint32_t>(result_exponent + 127) << __internal_fp32_exponent_shift) |
                              (normalized_mantissa & ASCRT_MAN_BIT_FLOAT_U);
        return __uint_as_float(bits);
    }

    const uint32_t subnormal_shift = static_cast<uint32_t>(exponent + __internal_fp32_subnormal_bias);
    return __uint_as_float(mantissa << subnormal_shift);
}

/**
 * Adjusts a float absolute-value bit pattern for one extra exponent step.
 *
 * The helper is used by the remquo core when the remainder mantissa must be
 * advanced to the next representable absolute float during quotient tracking.
 *
 */
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

/**
 * Computes the unsigned-magnitude remainder for remquo and tracks low quotient bits.
 *
 * The routine compares the absolute-value bit patterns of x and y, reconstructs
 * mantissa/exponent pairs, and performs a bitwise long-division style reduction
 * to obtain:
 *   remainder ~= abs(x) mod abs(y)
 * along with the low quotient bits needed by remquo.
 *
 * @param abs_x_bits Absolute bit pattern of |x|.
 * @param abs_y_bits Absolute bit pattern of |y|.
 * @param quo_abs_low Output low bits of the unsigned quotient magnitude.
 * @return The unsigned remainder as a float.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ static inline float __asc_remquo_core(
    uint32_t abs_x_bits, uint32_t abs_y_bits, uint32_t* quo_abs_low)
{
    uint32_t quotient_low = 0U;
    if (abs_x_bits < abs_y_bits) {
        *quo_abs_low = 0U;
        return __uint_as_float(abs_x_bits);
    }

    // Reconstruct mantissa and exponent information for both inputs.
    uint32_t remainder_mantissa = __asc_float_mantissa_bits_from_abs_bits(abs_x_bits);
    const uint32_t y_mantissa = __asc_float_mantissa_bits_from_abs_bits(abs_y_bits);
    const int32_t x_exponent = __asc_float_mantissa_exponent_from_abs_bits(abs_x_bits);
    const int32_t y_exponent = __asc_float_mantissa_exponent_from_abs_bits(abs_y_bits);
    const int32_t exponent_diff = x_exponent - y_exponent;

    // Use a bit-serial division path when x is at least as large as y in exponent.
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

/**
 * Applies the final remquo tail adjustment to the provisional remainder.
 *
 * If 2 * remainder exceeds |y|, or exactly ties with an odd low quotient bit,
 * the remainder is nudged by one divisor step and the quotient low bits are
 * incremented accordingly.
 *
 * @param abs_y_bits Absolute bit pattern of |y|.
 * @param remainder Provisional remainder, updated in place.
 * @param quotient_low Low quotient bits, updated in place.
 */
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

/**
 * Computes remquo(x, y) for float inputs using absolute-value reduction and
 * quotient-sign reconstruction.
 *
 * The implementation first handles special values, then reduces |x| by |y|
 * with a bitwise quotient tracker, applies the remquo tail correction, restores
 * the sign of the remainder, and finally folds the quotient sign into the low bits.
 *
 * @param x Dividend.
 * @param y Divisor.
 * @param quo Output quotient pointer.
 * @return The computed remainder value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ static inline float __asc_remquof_impl(float x, float y, int32_t* quo)
{
    constexpr uint32_t abs_mask = 0x7FFFFFFFU;
    const uint32_t x_bits = __float_as_uint(x);
    const uint32_t y_bits = __float_as_uint(y);
    const uint32_t abs_x_bits = x_bits & abs_mask;
    const uint32_t abs_y_bits = y_bits & abs_mask;
    *quo = 0;

    // NaN, infinity, and zero-divisor cases follow IEEE-style propagation rules.
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

    // Restore the sign of the remainder to match the sign of x.
    if ((x_bits & ASCRT_NEG_SIGN_BIT_U) != 0U) {
        const float abs_remainder = fabsf(remainder);
        if (abs_remainder <= ASCRT_INF_F) {
            remainder = -remainder;
        }
    }

    // Fold the sign of the quotient into the low quotient bits.
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

/**
 * Computes remquof(x, y) and stores the quotient in quo.
 *
 * This is a thin wrapper around the internal remquo implementation used by
 * the SIMT math library.
 *
 * @param x Dividend.
 * @param y Divisor.
 * @param quo Output quotient pointer.
 * @return The computed remainder value.
 */
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

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float remainderf(float x, float y)
{
    int32_t quo = -1;
    return remquof(x, y, &quo);
}

/**
 * Computes the reciprocal hypotenuse for float inputs.
 *
 * The implementation first reduces the input pair to absolute values and
 * chooses the smaller/larger magnitude for stable scaling.  The larger value
 * determines a shared power-of-two scale factor so that the squared sum stays
 * in a numerically safe range.
 *
 * With the scaled values, the core formula is:
 *   rhypotf(x, y) = 1 / sqrt(x^2 + y^2)
 *
 * rewritten as:
 *   scale = 2^k
 *   square_sum = (scaled_max)^2 + (scaled_min)^2
 *   result = rsqrt(square_sum) * scale
 *
 * This keeps the intermediate square sum well-conditioned while preserving
 * the final magnitude after rescaling.
 *
 * @param x The first input value.
 * @param y The second input value.
 * @return The computed rhypotf(x, y) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rhypotf(float x, float y)
{
    // Shared scaling constants used to keep the squared sum in range.
    constexpr uint32_t scale_base = 0x7E800000U;
    constexpr uint32_t scale_mask = 0xFE000000U;

    // Work with absolute values, then pick the smaller and larger magnitudes.
    const float abs_x = fabsf(x);
    const float abs_y = fabsf(y);
    const uint32_t abs_x_bits = __float_as_uint(abs_x);
    const uint32_t abs_y_bits = __float_as_uint(abs_y);
    const uint32_t min_bits = abs_x_bits < abs_y_bits ? abs_x_bits : abs_y_bits;
    const uint32_t max_bits = abs_x_bits > abs_y_bits ? abs_x_bits : abs_y_bits;
    const float min_abs = __uint_as_float(min_bits);
    const float max_abs = __uint_as_float(max_bits);

    // Build a power-of-two scale from the larger magnitude so the sum of squares remains stable.
    const float scale = __uint_as_float(scale_base - (max_bits & scale_mask));
    const float scaled_min = min_abs * scale;
    const float scaled_max = max_abs * scale;
    // Evaluate the scaled square sum and take the reciprocal square root.
    const float square_sum = fmaf(scaled_max, scaled_max, scaled_min * scaled_min);
    const float result = rsqrtf(square_sum) * scale;

    // If the smaller magnitude is infinite, the reciprocal hypotenuse collapses to zero.
    return min_abs == ASCRT_INF_F ? 0.0f : result;
}

/**
 * Computes the reciprocal Euclidean norm for three float inputs.
 *
 * The implementation first validates special values: any infinity collapses
 * the reciprocal norm to 0, and any NaN propagates as NaN.
 *
 * For finite inputs, it scales the largest magnitude into a safe range and
 * evaluates:
 *   rnorm3df(a, b, c) = 1 / sqrt(a^2 + b^2 + c^2)
 *
 * The squared sum is accumulated on scaled magnitudes to avoid overflow:
 *   scale = 2^k
 *   square_sum = (scaled_a)^2 + (scaled_b)^2 + (scaled_c)^2
 *
 * If the squared sum becomes subnormal, it is temporarily scaled up before
 * the reciprocal square root and scaled back after the Newton correction.
 *
 * @param a The first input value.
 * @param b The second input value.
 * @param c The third input value.
 * @return The computed reciprocal 3D norm.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnorm3df(float a, float b, float c)
{
    // Shared scaling constants used to keep the squared sum numerically safe.
    constexpr uint32_t scale_base = 0x7E800000U; // fp32 scale anchor used to build a power-of-two rescaling factor.
    constexpr uint32_t scale_mask = 0xFE000000U; // Mask that keeps the sign/exponent region and clears mantissa bits.

    constexpr float rsqrt_subnormal_scale = 4096.0f; // 2^12, compensates the temporary scaling after rsqrt refinement.

    // Work with absolute values so the norm depends only on magnitudes.
    const float abs_a = fabsf(a);
    const float abs_b = fabsf(b);
    const float abs_c = fabsf(c);
    if (isinf(abs_a) || isinf(abs_b) || isinf(abs_c)) {
        return 0.0f;
    }
    if (isnan(abs_a) || isnan(abs_b) || isnan(abs_c)) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    // Sort magnitudes so the largest term determines the scaling exponent.
    const float max_ab = fmaxf(abs_a, abs_b);
    const float min_ab = fminf(abs_a, abs_b);
    const float max_abs = fmaxf(abs_c, max_ab);
    const float mid_abs = fminf(abs_c, max_ab);
    const uint32_t max_bits = __float_as_uint(max_abs);
    const float scale = __uint_as_float(scale_base - (max_bits & scale_mask));

    // Accumulate the scaled sum of squares in magnitude order.
    const float scaled_min = min_ab * scale;
    const float scaled_mid = mid_abs * scale;
    const float scaled_max = max_abs * scale;
    float square_sum = scaled_mid * scaled_mid;
    square_sum = fmaf(scaled_min, scaled_min, square_sum);
    square_sum = fmaf(scaled_max, scaled_max, square_sum);

    // Subnormal squared sums are temporarily lifted before rsqrtf.
    const bool is_subnormal_square_sum = fabsf(square_sum) < __internal_subnormal_boundary;
    if (is_subnormal_square_sum) {
        square_sum *= __internal_fp32_scale_2p24;
    }
    // Zero norm means the reciprocal norm is infinite.
    if (square_sum == 0.0f) {
        return ASCRT_INF_F;
    }

    // Compute an approximate reciprocal norm and refine it with one Newton step.
    float inv_norm = rsqrtf(square_sum);
    inv_norm = inv_norm * fmaf(-0.5f * square_sum, inv_norm * inv_norm, 1.5f);
    if (is_subnormal_square_sum) {
        inv_norm *= rsqrt_subnormal_scale;
    }
    // Restore the outer scale factor.
    return scale * inv_norm;
}

/**
 * Computes the reciprocal Euclidean norm for four float inputs.
 *
 * The implementation follows the same structure as rnorm3df:
 * special-value handling, magnitude scaling, squared-sum accumulation,
 * subnormal lifting, reciprocal square root, Newton refinement, and final
 * scale restoration.
 *
 * @param a The first input value.
 * @param b The second input value.
 * @param c The third input value.
 * @param d The fourth input value.
 * @return The computed reciprocal 4D norm.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float rnorm4df(float a, float b, float c, float d)
{
    // Shared scaling constants used to keep the squared sum numerically safe.
    constexpr uint32_t scale_base = 0x7E800000U; // fp32 scale anchor used to build a power-of-two rescaling factor.
    constexpr uint32_t scale_mask = 0xFE000000U; // Mask that keeps the sign/exponent region and clears mantissa bits.

    constexpr float rsqrt_subnormal_scale = 4096.0f; // 2^12, compensates the temporary scaling after rsqrt refinement.

    // Work with absolute values so the norm depends only on magnitudes.
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

    // Sort magnitudes so the largest term determines the scaling exponent.
    const float max_ab = fmaxf(abs_a, abs_b);
    const float min_ab = fminf(abs_a, abs_b);
    const float max_abc = fmaxf(abs_c, max_ab);
    const float max_abs = fmaxf(abs_d, max_abc);
    const float second_abs = fminf(abs_d, max_abc);
    const float third_abs = fminf(abs_c, max_ab);
    const uint32_t max_bits = __float_as_uint(max_abs);
    const float scale = __uint_as_float(scale_base - (max_bits & scale_mask));

    // Accumulate the scaled sum of squares in magnitude order.
    const float scaled_min = min_ab * scale;
    const float scaled_second = second_abs * scale;
    const float scaled_third = third_abs * scale;
    const float scaled_max = max_abs * scale;
    float square_sum = scaled_second * scaled_second;
    square_sum = fmaf(scaled_third, scaled_third, square_sum);
    square_sum = fmaf(scaled_min, scaled_min, square_sum);
    square_sum = fmaf(scaled_max, scaled_max, square_sum);

    // Subnormal squared sums are temporarily lifted before rsqrtf.
    const bool is_subnormal_square_sum = fabsf(square_sum) < __internal_subnormal_boundary;
    if (is_subnormal_square_sum) {
        square_sum *= __internal_fp32_scale_2p24;
    }
    // Zero norm means the reciprocal norm is infinite.
    if (square_sum == 0.0f) {
        return ASCRT_INF_F;
    }

    // Compute an approximate reciprocal norm and refine it with one Newton step.
    float inv_norm = rsqrtf(square_sum);
    inv_norm = inv_norm * fmaf(-0.5f * square_sum, inv_norm * inv_norm, 1.5f);
    if (is_subnormal_square_sum) {
        inv_norm *= rsqrt_subnormal_scale;
    }
    // Restore the outer scale factor.
    return scale * inv_norm;
}

/**
 * Scales a float by an integer power of two with clamped exponent handling.
 *
 * The implementation first clamps the requested exponent to a safe range so
 * the intermediate scale factor does not overflow the fp32 exponent field.
 *
 * For small exponent magnitudes, the scale is formed directly as:
 *   x * 2^n
 *
 * For larger magnitudes, the exponent is split into a high part and a residual:
 *   n + 0x1FC = split_exponent * 3 + residual_exponent
 *
 * The result is then reconstructed with repeated multiplication by the split
 * scale factor to keep the intermediate values in range:
 *   result = x * 2^(residual_exponent) * (2^(split_exponent))^3
 *
 * @param x The input value.
 * @param n The scaling exponent.
 * @return The computed x * 2^n value with clamped exponent handling.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_scalbnf_clamped(float x, int32_t n)
{
    // Maximum absolute exponent used to keep the scale factor within the safe fp32 range.
    constexpr int32_t max_scale_exponent = 0x116;
    // Threshold above which the exponent is split into a high part and a residual part.
    constexpr int32_t split_threshold = 0x65;
    // Bias used when the exponent can be encoded directly into a single fp32 scale.
    constexpr int32_t simple_exponent_bias = 0x7F;
    // Bias used for the split-exponent path so the high/low parts stay well-formed.
    constexpr int32_t split_exponent_bias = 0x1FC;
    // Bit pattern for 1.0f, used as the default scale factor when no split is needed.
    constexpr uint32_t fp32_one_bits = 0x3F800000U;
    // Clamp the requested exponent to a bounded interval.
    int32_t clamped_n = n > max_scale_exponent ? max_scale_exponent : n;
    clamped_n = clamped_n < -max_scale_exponent ? -max_scale_exponent : clamped_n;

    // Build either a direct exponent scale or a split high/residual scale.
    uint32_t scale_bits = 0U;
    uint32_t split_scale_bits = fp32_one_bits;
    if (clamped_n >= split_threshold || clamped_n <= -split_threshold) {
        const int32_t biased_exponent = clamped_n + split_exponent_bias;
        const int32_t split_exponent = static_cast<uint32_t>(biased_exponent) >> 2U;
        const int32_t residual_exponent = biased_exponent - split_exponent * 3;
        split_scale_bits = static_cast<uint32_t>(split_exponent) << __internal_fp32_exponent_shift;
        scale_bits = static_cast<uint32_t>(residual_exponent) << __internal_fp32_exponent_shift;
    } else {
        scale_bits = static_cast<uint32_t>(clamped_n + simple_exponent_bias) << fp32_exponent_shift;
    }

    // Apply the direct scale first, then fold in the split high-part scale three times.
    float result = __uint_as_float(scale_bits) * x;
    const float split_scale = __uint_as_float(split_scale_bits);
    result = split_scale * result;
    result = split_scale * result;
    result = split_scale * result;
    return result;
}

/**
 * Scales a float by an integer power of two.
 *
 * This is a thin wrapper around the clamped scaling helper used by the SIMT
 * math library.
 *
 * @param x The input value.
 * @param n The scaling exponent.
 * @return The computed x * 2^n value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float scalbnf(float x, int32_t n) { return __internal_scalbnf_clamped(x, n); }

/**
 * Scales a float by a long integer power of two.
 *
 * The long exponent is clamped to the same safe interval as scalbnf before
 * being forwarded to the shared scaling helper.
 *
 * @param x The input value.
 * @param n The scaling exponent.
 * @return The computed x * 2^n value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float scalblnf(float x, int64_t n)
{
    // Clamp the long exponent to the same bounded interval used by scalbnf.
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

/**
 * Computes sin(pi * x) for float inputs.
 *
 * The implementation first handles special values and large magnitudes with
 * fixed IEEE-style semantics:
 *   - NaN / infinity propagate to NaN
 *   - very large |x| collapse to signed zero
 *
 * The core reduction uses the nearest half-integer decomposition:
 *   sin(pi * x) = sin(pi * r) or cos(pi * r)
 * where r = x - round(2x) / 2
 *
 * The parity of round(2x) selects the sine or cosine polynomial path.
 * A final sign flip is applied from the quadrant parity, and exact integers
 * return signed zero.
 *
 * @param x The input value.
 * @return The computed sin(pi * x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinpif(float x)
{
    // Large inputs are reduced to signed zero to avoid loss of significance.
    constexpr float large_input_bound = __internal_fp32_scale_2p24;
    constexpr float pi_hi = 3.1415927410125732422f; // High-precision single-precision pi constant.

    // NaN and infinity propagate to NaN.
    if (isnan(x) || isinf(x)) {
        return ASCRT_NAN_F;
    }

    // Very large magnitudes are treated as zero after range reduction.
    if (fabsf(x) > large_input_bound) {
        return 0.0f * x;
    }

    // Keep a truncated copy so exact integers can be forced to signed zero later.
    const float truncated_x = truncf(x);

    // Reduce x around the nearest half-integer by working with 2x.
    const float two_x = x + x;
    const int32_t quadrant = __float2int_rn(two_x);
    const float rounded_two_x = __int2float_rn(quadrant);
    const bool use_cos_poly = ((quadrant & 1) != 0);

    // Reduced argument r = x - round(2x)/2.
    const float reduced = fmaf(-rounded_two_x, 0.5f, x);
    const float reduced2 = reduced * reduced;

    // Evaluate the sine or cosine polynomial branch depending on parity.
    float poly = use_cos_poly ? 0.22686031460762023926f : -0.59248024225234985352f;
    poly = fmaf(reduced2, poly, use_cos_poly ? -1.334560394287109375f : 2.550144195556640625f);
    poly = fmaf(reduced2, poly, use_cos_poly ? 4.0586924552917480469f : -5.1677198410034179688f);

    float result = 0.0f;
    if (use_cos_poly) {
        // Cosine branch for half-integer neighborhoods.
        poly = fmaf(reduced2, poly, -4.9348020553588867188f);
        result = fmaf(poly, reduced2, 1.0f);
    } else {
        // Sine branch for integer neighborhoods.
        result = fmaf(poly, reduced * reduced2, reduced * pi_hi);
    }

    // Quadrant parity determines the final sign.
    if ((quadrant & 2) != 0) {
        result = -result;
    }
    // Exact integers map to signed zero.
    if (truncated_x == x) {
        result = 0.0f * x;
    }

    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_tanpif_small(float x)
{
    // Split pi into hi/lo parts so the argument reduction stays accurate.
    constexpr float pi_hi = 3.1415927410125732422f;
    constexpr float pi_lo = -8.7422776573475857731e-08f;
    const float arg = fmaf(x, pi_hi, x * pi_lo);
    return tanf(arg);
}

/**
 * Computes tan(pi * x) for float inputs.
 *
 * The implementation first handles special values and large magnitudes with
 * fixed IEEE-style semantics:
 *   - NaN / infinity propagate to NaN
 *   - very large |x| collapse to signed zero
 *
 * The core reduction uses the nearest integer decomposition:
 *   tan(pi * x) = tan(pi * r), where r = x - round(x)
 *
 * For small |r|, the tangent polynomial is evaluated directly through a split
 * pi argument:
 *   tan(pi * r) = tanf(r * pi_hi + r * pi_lo)
 *
 * For half-integer neighborhoods, the implementation switches to the cotangent
 * identity:
 *   tan(pi * x) = sign(x) / tan(pi * (0.5 - |r|))
 *
 * Exact integers return signed zero, and exact half-integers return signed
 * infinity.
 *
 * @param x The input value.
 * @return The computed tan(pi * x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanpif(float x)
{
    // Very large magnitudes are treated as zero after range reduction.
    constexpr float large_input_bound = __internal_fp32_scale_2p24;

    // NaN and infinity propagate to NaN.
    if (isnan(x) || isinf(x)) {
        return ASCRT_NAN_F;
    }
    // Large inputs collapse to signed zero.
    if (fabsf(x) >= large_input_bound) {
        return 0.0f * x;
    }

    // Reduce x by the nearest integer and work on the fractional part.
    const int32_t nearest_integer = __float2int_rn(x);
    const float rounded_integer = __int2float_rn(nearest_integer);
    const float reduced = x - rounded_integer;
    const float abs_reduced = fabsf(reduced);

    // Exact integers map to signed zero.
    if (reduced == 0.0f) {
        return 0.0f * x;
    }
    // Exact half-integers map to signed infinity.
    if (abs_reduced == 0.5f) {
        return __internal_with_sign_bit(ASCRT_INF_F, reduced);
    }
    // Small reduced arguments use the direct tangent polynomial path.
    if (abs_reduced <= 0.25f) {
        return __internal_tanpif_small(reduced);
    }

    // Near half-integers, use tan(pi*x) = 1 / tan(pi*(0.5 - |r|)).
    const float distance_to_half = 0.5f - abs_reduced;
    const float cot_base = __internal_tanpif_small(distance_to_half);
    const float result = 1.0f / cot_base;
    return __internal_with_sign_bit(result, reduced);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_euler_gamma_function(float x)
{
    // Reduce the argument around the nearest integer when x is nonzero.
    float reduced = x;
    if (x != 0.0f) {
        reduced = x - nearbyintf(x);
    }

    // Polynomial approximation for the Euler gamma correction term.
    float poly = fmaf(reduced, -0.0010728656779974699020f, 0.0071110534481704235077f);
    poly = fmaf(poly, reduced, -0.0096437186002731323242f);
    poly = fmaf(poly, reduced, -0.042180188000202178955f);
    poly = fmaf(poly, reduced, 0.16654090583324432373f);
    poly = fmaf(poly, reduced, -0.04200365021824836731f);
    poly = fmaf(poly, reduced, -0.65587818622589111328f);
    poly = fmaf(poly, reduced, 0.57721567153930664062f);

    // Build the denominator used by the reciprocal gamma correction path.
    float scale = x < -0.5f ? x : 1.0f;
    if (x <= 0.5f) {
        scale = scale * reduced;
    }

    float denominator = fmaf(scale * poly, reduced, scale);
    // Fall back to a scaled reciprocal when the denominator becomes subnormal.
    if (fabsf(denominator) < __internal_subnormal_boundary) {
        int32_t e = 0;
        float m = frexpf(denominator, &e);
        return ldexpf(1.0f / m, 0 - e);
    }
    return 1.0f / denominator;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_cal_abs_x(float x)
{
    constexpr float tgamma_stirling_bound = 41.09999847412109375f;
    // Clamp the absolute magnitude to the Stirling training range.
    return fabsf(x) > tgamma_stirling_bound ? tgamma_stirling_bound : fabsf(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_stirling_and_euler_reflection(float x)
{
    // Constants used by the log/exp split, Stirling reconstruction, and tail handling.
    constexpr float log2e_hi = __internal_fp32_log2e;
    constexpr float log2e_lo = 1.925963033500011079e-08f;
    constexpr float sqrt_two_pi = 2.5066282749176025391f;
    constexpr float subnormal_scale = __internal_fp32_scale_2p24;
    constexpr float subnormal_exponent_fix = __internal_fp32_subnormal_exponent_fix;
    constexpr float log_exponent_scale = __internal_fp32_log_exponent_scale;
    constexpr uint32_t log_reduction_mask = 0xFF800000U;
    constexpr uint32_t sqrt_half_bits = 0x3F3504F3U;
    constexpr float large_reflection_bound = 33.0f;
    constexpr float negative_exponent_offset = 48.0f;
    constexpr float exp2_overflow_abs_bound = 152.0f;

    // Clamp the Stirling input range and keep only the absolute magnitude.
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
    const bool is_normal_x = abs_x >= __internal_subnormal_boundary;
    const float log_input = is_normal_x ? abs_x : abs_x * subnormal_scale;
    const float exponent_base = is_normal_x ? 0.0f : subnormal_exponent_fix;
    const uint32_t log_input_bits = __float_as_uint(log_input);

    // Split the logarithm reduction around sqrt(0.5) to keep the mantissa near 1.
    const uint32_t reduction_bits = (log_input_bits - sqrt_half_bits) & log_reduction_mask;
    const float mantissa = __uint_as_float(log_input_bits - reduction_bits);
    const float exponent_part =
        fmaf(__int2float_rn(static_cast<int32_t>(reduction_bits)), log_exponent_scale, exponent_base);

    // Evaluate the log polynomial and its high/low residual corrections.
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

    // Reconstruct log(x) as a high part plus a low residual.
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

    // Accumulate the residual terms for the Stirling exponent.
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
        // Reflect the exponent for the negative-x branch.
        exponent_hi = offset - exponent_hi;
        exponent_lo = -exponent_lo;
    }

    // 2^exponent * sqrt(2*pi)
    // Split the exponent into integer and fractional parts for exp2 reconstruction.
    const float rounded_exponent = roundf(exponent_hi);
    float exp2_fraction = exponent_hi - rounded_exponent;
    exp2_fraction = exp2_fraction + exponent_lo;
    const int32_t exp2_exponent = __float2int_rz(rounded_exponent);

    // Evaluate the exp2 polynomial on the fractional part.
    float exp2_poly = fmaf(exp2_fraction, 0.00015239251661114395f, 0.0013391353422775864601f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.0096188392490148544312f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.055503588169813156128f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 0.24022644758224487305f);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, __internal_fp32_ln2_hi);
    exp2_poly = fmaf(exp2_fraction, exp2_poly, 1.0f);

    const bool exponent_is_positive = rounded_exponent > 0.0f;
    const uint32_t scale_hi_bits = exponent_is_positive ? 0x7F000000U : 0x02000000U;
    const uint32_t scale_adjust = exponent_is_positive ? 0U : 0x83000000U;
    const uint32_t scale_lo_bits =
        (static_cast<uint32_t>(exp2_exponent) << __internal_fp32_exponent_shift) - scale_adjust;
    float exp2_value = exp2_poly * __uint_as_float(scale_hi_bits);
    exp2_value = exp2_value * __uint_as_float(scale_lo_bits);
    // Clamp exp2 overflow in the Stirling branch.
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
        // Reuse the sin(pi*x) style quadrant split for the reflection denominator.
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

        // Track the residual of the reflection denominator for the final correction.
        float y_diff = fmaf(y6 * x, sinpi, -y6 * x * sinpi);
        float y7 = 1 / (y6 * x * sinpi);

        return __internal_cal_y(abs_x, y_diff, y5, y7);
    }
}

/**
 * Computes the gamma function for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, +inf returns +inf, -inf returns NaN, and zero returns signed
 * infinity.
 *
 * For small |x|, it uses a local Euler-gamma polynomial path:
 *   Gamma(x) ~= 1 / [ x * P(reduced) ]
 *
 * For larger magnitudes, it switches to a Stirling-style approximation:
 *   Gamma(x) ~= sqrt(2*pi) * (x/e)^(x-1/2) * correction(x)
 *
 * Negative non-integers use Euler's reflection identity:
 *   Gamma(x) = pi / (sin(pi*x) * Gamma(1-x))
 *
 * The reflection branch reuses the sin(pi*x) style reduction so the sign and
 * half-integer behavior stay consistent.
 *
 * @param x The input value.
 * @return The computed tgammaf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tgammaf(float x)
{
    constexpr float tgamma_stirling_bound = 41.09999847412109375f;

    // Special values follow IEEE-style gamma semantics.
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

    // Small magnitudes use the Euler gamma correction path; larger ones use Stirling/reflection.
    float result = fabsf(x) < 1.5f ? __internal_euler_gamma_function(x) : __internal_stirling_and_euler_reflection(x);

    // Integer inputs on the negative side are poles of gamma.
    const float truncated_x = truncf(x);
    if (truncated_x == x) {
        return x < 0.0f ? ASCRT_INF_F / ASCRT_INF_F : result;
    }

    // For large negative non-integers, odd integer truncations force a sign correction to zero.
    if (x < -tgamma_stirling_bound && truncated_x != x) {
        const int32_t truncated_int = __float2int_rz(truncated_x);
        if ((truncated_int & 1) != 0) {
            result = 0.0f;
        }
    }
    return result;
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float __internal_asin_acos_reduced_arg(float abs_x)
{
    constexpr float threshold = 0.56000000238418579102f;

    float reduced = 0.0f;
    if (abs_x != 1.0f) {
        const float half_one_minus_abs = fmaf(0.5f, -abs_x, 0.5f);
        const float inv_sqrt = rsqrtf(half_one_minus_abs);
        float sqrt_term = half_one_minus_abs * inv_sqrt;
        const float correction = fmaf(-sqrt_term, inv_sqrt * 0.5f, 0.5f);
        reduced = fmaf(sqrt_term, correction, sqrt_term);
    }
    return (abs_x > threshold) ? reduced : abs_x;
}

/**
 * Computes tanh(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, and infinities saturate to signed one.
 *
 * For finite inputs, it uses a small-argument polynomial near zero and an
 * exp2-based reconstruction for larger magnitudes:
 *   tanh(x) = sign(x) * (1 - 2 / (exp(2|x|) + 1))
 *
 * The small-argument branch keeps the local odd polynomial around x = 0,
 * while the large-argument branch saturates once the exponential tail is
 * sufficiently close to one.
 *
 * @param x The input value.
 * @return The computed tanhf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float tanhf(float x)
{
    // Polynomial and threshold constants for the small and large magnitude paths.
    constexpr float poly_first_coeff = 0.01573968306183815002f;
    constexpr float small_threshold = 0.60000002384185791016f;
    constexpr float saturation_threshold = 9.010913848876953125f;
    constexpr uint32_t sign_mask = 0x80000000U;

    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // Infinities saturate to signed one.
    if (isinf(x)) {
        return __internal_with_sign_bit(1.0f, x);
    }

    // Use the absolute value for the large-argument exp2 reconstruction.
    const float abs_x = fabsf(x);

    // Large-argument path based on the exponential identity for tanh.
    const float exp2_arg = abs_x * 2.8853900432586669922f;
    const float exp2_value = exp2f(exp2_arg);

    // Small-argument odd polynomial around zero.
    const float x2 = x * x;
    float poly = fmaf(x2, poly_first_coeff, -0.052303962409496307373f);
    poly = fmaf(x2, poly, 0.13315297663211822510f);
    poly = fmaf(x2, poly, -0.33332768082618713379f);
    poly = fmaf(x2, poly, 0.0f);

    // Reconstruct 1 - 2 / (exp(2|x|) + 1) and clamp to signed one in saturation range.
    float large_result = fmaf(1.0f / (exp2_value + 1.0f), -2.0f, 1.0f);
    if (abs_x >= saturation_threshold) {
        large_result = 1.0f;
    }
    const uint32_t sign_bit = __float_as_uint(x) & sign_mask;
    const uint32_t large_result_bits = __float_as_uint(large_result) | sign_bit;
    float result = __uint_as_float(large_result_bits);

    // Near zero, switch to the polynomial branch.
    if (abs_x < small_threshold) {
        result = fmaf(poly, x, x);
    }
    return result;
}

/**
 * Computes atan(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, and infinities map to signed pi/2.
 *
 * For finite inputs, it uses a reciprocal reduction for |x| > 1:
 *   atan(x) = sign(x) * (pi/2 - atan(1/|x|))
 *
 * The reduced argument is then evaluated with an odd polynomial in x^2.
 *
 * @param x The input value.
 * @return The computed atanf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanf(float x)
{
    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // Infinities map to signed pi/2.
    if (isinf(x)) {
        return __internal_with_sign_bit(ASCRT_PIO2_F, x);
    }

    // Use reciprocal reduction for large magnitudes.
    const float abs_x = fabsf(x);
    const bool use_reciprocal = abs_x > 1.0f;

    float reduced = use_reciprocal ? (1.0f / abs_x) : abs_x;
    const float reduced2 = reduced * reduced;

    // Odd polynomial approximation in powers of x^2.
    float poly = fmaf(reduced2, 0.00245002890005707741f, -0.014396979473531246185f);
    poly = fmaf(reduced2, poly, 0.039849750697612762451f);
    poly = fmaf(reduced2, poly, -0.072529748082160949707f);
    poly = fmaf(reduced2, poly, 0.10518480092287063599f);
    poly = fmaf(reduced2, poly, -0.14171802997589111328f);
    poly = fmaf(reduced2, poly, 0.19988775253295898438f);
    poly = fmaf(reduced2, poly, -0.33332940936088562012f);
    poly *= reduced2;
    float result = fmaf(reduced, poly, reduced);

    // Recover the pi/2 complement for the reciprocal branch.
    if (use_reciprocal) {
        result = fmaf(0.93318945169448852539f, 1.6832555532455444336f, -result);
    }

    return __internal_with_sign_bit(result, x);
}

/**
 * Computes asin(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, and |x| > 1 returns NaN.
 *
 * For finite inputs, it uses a reduced-argument polynomial for asin(|x|), and
 * switches to a pi/2 correction near the endpoints:
 *   asin(x) ~= reduced + polynomial(reduced)
 *   asin(x) = pi/2 - 2 * reduced_path(x)  near |x| close to 1
 *
 * The sign of the original input is restored at the end.
 *
 * @param x The input value.
 * @return The computed asinf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float asinf(float x)
{
    // Endpoint correction threshold and pi/2 split constants.
    constexpr float threshold = 0.56000000238418579102f;
    constexpr float half_pi_hi = 1.6832555532455444336f;
    constexpr float half_pi_lo_scale = 0.93318945169448852539f;

    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // Out-of-domain inputs return NaN.
    if (fabsf(x) > 1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    // Reduce the argument into the polynomial training range.
    const float abs_x = fabsf(x);
    float reduced = __internal_asin_acos_reduced_arg(abs_x);
    const float reduced2 = reduced * reduced;

    // Odd polynomial approximation for asin(reduced).
    float poly = fmaf(reduced2, 0.05025001987814903259f, 0.018773360177874565125f);
    poly = fmaf(reduced2, poly, 0.046769052743911743164f);
    poly = fmaf(reduced2, poly, 0.074823014438152313232f);
    poly = fmaf(reduced2, poly, 0.16667181253433227539f);
    poly *= reduced2;
    float result = fmaf(reduced, poly, reduced);

    // Near |x| = 1, switch to the complementary pi/2 correction.
    if (abs_x > threshold) {
        result = fmaf(half_pi_hi, half_pi_lo_scale, -2.0f * result);
    }

    return __internal_with_sign_bit(result, x);
}

/**
 * Computes acos(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, and |x| > 1 returns NaN.
 *
 * For finite inputs, it reuses the reduced asin-style core and then applies the
 * complementary angle relation:
 *   acos(x) = pi/2 - asin(x)
 *
 * The same endpoint threshold is used to switch between the direct reduced path
 * and the pi/2 correction path.
 *
 * @param x The input value.
 * @return The computed acosf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float acosf(float x)
{
    // Endpoint correction threshold and pi/2 split constants.
    constexpr float threshold = 0.56000000238418579102f;
    constexpr float half_pi_hi = 1.6832555532455444336f;
    constexpr float half_pi_lo_scale = 0.93318945169448852539f;

    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // Out-of-domain inputs return NaN.
    if (fabsf(x) > 1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    // Reuse the asin reduction with sign handling folded into the reduced argument.
    const float abs_x = fabsf(x);
    float reduced = __internal_asin_acos_reduced_arg(abs_x);
    reduced = __internal_with_sign_bit(reduced, x);
    const float reduced2 = reduced * reduced;

    // Odd polynomial approximation for the reduced asin core.
    float poly = fmaf(reduced2, 0.03538220748305320740f, 0.016980519518256187439f);
    poly = fmaf(reduced2, poly, 0.030762933194637298584f);
    poly = fmaf(reduced2, poly, 0.044709417968988418579f);
    poly = fmaf(reduced2, poly, 0.074989043176174163818f);
    poly = fmaf(reduced2, poly, 0.16666707396507263184f);
    poly *= reduced2;
    float asin_reduced = fmaf(reduced, poly, reduced);

    // Build acos from the asin core and the pi/2 complement when needed.
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

/**
 * Computes sinh(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN and infinities propagate directly.
 *
 * For finite inputs, it uses a small-argument odd polynomial near zero and a
 * split exp2-based reconstruction for larger magnitudes:
 *   sinh(x) = (e^x - e^-x) / 2
 *
 * The large-argument branch reconstructs the exponential magnitude with a
 * split exponent path to keep the intermediate values stable.
 *
 * @param x The input value.
 * @return The computed sinhf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float sinhf(float x)
{
    // NaN and infinities propagate directly.
    if (isnan(x)) {
        return x;
    }
    if (isinf(x)) {
        return x;
    }

    // Use absolute value for the large-argument exponential reconstruction.
    const float abs_x = fabsf(x);
    const float x2 = x * x;

    // Small-argument odd polynomial around zero.
    float poly = fmaf(x2, 0.00000281695110970758826f, 0.00019836159481201320887f);
    poly = fmaf(x2, poly, 0.0083333496004343032837f);
    poly = fmaf(x2, poly, 0.16666667163372039795f);
    poly *= x2;

    // Split the exponent for the large-argument exp2 reconstruction.
    float n = truncf(abs_x * __internal_fp32_log2e);
    if (fabsf(n) > __internal_fp32_max_exp) {
        n = __internal_with_sign_bit(__internal_fp32_max_exp, n);
    }

    // Reconstruct the exponential tail with hi/lo corrections.
    float r = fmaf(n, -__internal_fp32_ln2_hi, abs_x);
    r = fmaf(n, __internal_fp32_ln2_tail, r);
    const float exp2_residual = exp2f(r * __internal_fp32_log2e);

    const float exponent_base = n + 12583037.0f;
    const uint32_t exponent_base_bits = __float_as_uint(exponent_base);
    const float exp_quarter = __uint_as_float(exponent_base_bits << 23) * exp2_residual;
    float result = fmaf(exp_quarter, 2.0f, -0.125f / exp_quarter);
    result = __internal_with_sign_bit(result, x);

    // Near zero, switch to the polynomial branch.
    if (abs_x < 1.0f) {
        result = fmaf(poly, x, x);
    }
    return result;
}

/**
 * Computes log1p(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * zero returns zero, x == -1 returns -inf, x < -1 returns NaN, +inf returns +inf,
 * and NaN propagates.
 *
 * For finite values, it reduces 1 + x into a normalized interval and evaluates:
 *   log1p(x) = log(1 + x)
 *
 * The reduction uses a bit-level decomposition around 1/2 and 4 so that the
 * polynomial sees a small reduced argument and the exponent contribution is
 * accumulated separately.
 *
 * @param x The input value.
 * @return The computed log1pf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float log1pf(float x)
{
    // Special values follow the standard log1p semantics.
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
        return x;
    }

    // Reduction constants for the log1p polynomial path.
    constexpr uint32_t log1p_reduction_mask = 0xFF800000U;
    constexpr uint32_t fp32_one_half_bits = 0x3F400000U;
    constexpr uint32_t fp32_four_bits = 0x40800000U;
    constexpr float poly_first_coeff = 0.04534861445426940918f;

    // Build the reduced mantissa and exponent contribution from 1 + x.
    const float one_add_x = 1.0f + x;
    const uint32_t x_bits = __float_as_uint(x);
    const uint32_t one_add_x_bits = __float_as_uint(one_add_x);

    const uint32_t reduction_bits = (one_add_x_bits - fp32_one_half_bits) & log1p_reduction_mask;
    const uint32_t normalized_x_bits = x_bits - reduction_bits;
    const uint32_t range_scale_bits = fp32_four_bits - reduction_bits;
    const float normalized_x = __uint_as_float(normalized_x_bits);
    const float range_scale = __uint_as_float(range_scale_bits);
    const float reduced = fmaf(0.25f, range_scale, -1.0f) + normalized_x;
    const float exponent =
        static_cast<float>(static_cast<int32_t>(reduction_bits)) * __internal_fp32_log_exponent_scale;

    // Polynomial approximation for log1p(reduced).
    float poly = fmaf(-poly_first_coeff, reduced, 0.10546888411045074463f);
    poly = fmaf(poly, reduced, -0.13229703903198242188f);
    poly = fmaf(poly, reduced, 0.14491446316242218018f);
    poly = fmaf(poly, reduced, -0.16641564667224884033f);
    poly = fmaf(poly, reduced, 0.19988867640495300293f);
    poly = fmaf(poly, reduced, -0.25000196695327758789f);
    poly = fmaf(poly, reduced, 0.33333510160446166992f);
    poly = fmaf(poly, reduced, -0.5f);

    const float reduced_poly = reduced * poly;
    float output = fmaf(reduced_poly, reduced, reduced);
    // Add the separate exponent contribution back as ln(2) * exponent.
    output = fmaf(exponent, __internal_fp32_ln2_hi, output);

    return output;
}

/**
 * Computes atanh(x) for float inputs.
 *
 * The implementation first handles special values with fixed IEEE-style semantics:
 * NaN propagates, x == +/-1 returns signed infinity, and |x| > 1 returns NaN.
 *
 * For finite values, it uses the identity:
 *   atanh(x) = 0.5 * log1p(2|x| / (1 - |x|))
 *
 * The sign of the original input is restored at the end.
 *
 * @param x The input value.
 * @return The computed atanhf(x) value.
 */
__SIMT_DEVICE_FUNCTIONS_DECL__ inline float atanhf(float x)
{
    // NaN propagates directly.
    if (isnan(x)) {
        return x;
    }
    // Saturate the poles at +/-1 to signed infinity.
    if (x == 1.0f) {
        return ASCRT_INF_F;
    }
    if (x == -1.0f) {
        return -ASCRT_INF_F;
    }
    if (fabsf(x) > 1.0f) {
        return ASCRT_INF_F / ASCRT_INF_F;
    }

    // Use the log1p identity on the absolute value and restore sign at the end.
    const float abs_x = fabsf(x);
    float log_arg = (2.0f / (1.0f - abs_x)) * abs_x;

    return __internal_with_sign_bit(0.5f, x) * log1pf(log_arg);
}

#endif // ASCENDC_USE_LEGACY_PRECISION

#endif
#endif // IMPL_SIMT_API_MATH_FUNCTIONS_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_FUNCTIONS_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_FUNCTIONS_IMPL__
#endif
