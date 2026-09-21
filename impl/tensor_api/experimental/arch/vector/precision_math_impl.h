/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PRECISION_MATH_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_PRECISION_MATH_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_PRECISION_MATH_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"
#include "impl/tensor_api/experimental/arch/vector/bitcast_impl.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

// Soft-emulates basic_api SqrtFastInverseImpl / high-precision sqrt (float, 0ulp).
template <typename T>
__simd_callee__ inline reg_tensor<T> sqrt_fast_inverse(const reg_tensor<T>& src)
{
    static_assert(Std::is_same_v<T, float>, "sqrt fast_inverse only supports float");
    constexpr float subnormal_bound = 1.0f;
    constexpr float half_factor = 0.5f;
    constexpr float neg_one = -1.0f;
    constexpr float scale_up = 16777216.0f;       // 2^24
    constexpr float scale_down = 0.000244140625f; // 2^-12
    constexpr uint32_t pos_inf_bits = 0x7f800000u;
    constexpr uint32_t neg_zero_bits = 0x80000000u;

    const vector_bool mask = src.mask;
    reg_tensor<T> src_work = src;
    reg_tensor<bool> is_sub;
    asc_lt_scalar(is_sub.reg, src_work.reg, subnormal_bound, mask);

    reg_tensor<T> tmp;
    asc_mul_scalar(tmp.reg, src_work.reg, scale_up, mask);
    asc_select(src_work.reg, tmp.reg, src_work.reg, is_sub.reg);

    reg_tensor<T> one;
    asc_duplicate_scalar(one.reg, 1.0f, mask);
    one.mask = mask;

    reg_tensor<T> sqrt_tmp;
    asc_sqrt(sqrt_tmp.reg, src_work.reg, mask);
    reg_tensor<T> x;
    asc_div(x.reg, one.reg, sqrt_tmp.reg, mask); // x ~= rsqrt(b)

    // x2 = 1 - b*x*x; x = x + x2 * 0.5x
    reg_tensor<T> neg_x;
    asc_mul_scalar(neg_x.reg, x.reg, neg_one, mask);
    reg_tensor<T> bx;
    asc_mul(bx.reg, x.reg, src_work.reg, mask);
    reg_tensor<T> one_acc = one;
    reg_tensor<T> bx_neg_x;
    asc_mul(bx_neg_x.reg, bx.reg, neg_x.reg, mask);
    asc_add(one_acc.reg, one_acc.reg, bx_neg_x.reg, mask); // 1 + (b*x)*(-x) = 1 - b*x*x

    reg_tensor<T> half_x;
    asc_mul_scalar(half_x.reg, x.reg, half_factor, mask);
    reg_tensor<T> corr;
    asc_mul(corr.reg, one_acc.reg, half_x.reg, mask);
    asc_add(x.reg, x.reg, corr.reg, mask);

    // x1 = x*b; err = b - x1*x1; x2 = 0.5x * err + x1
    reg_tensor<T> x1;
    asc_mul(x1.reg, x.reg, src_work.reg, mask);
    reg_tensor<T> neg_x1;
    asc_mul_scalar(neg_x1.reg, x1.reg, neg_one, mask);
    reg_tensor<T> err = src_work;
    reg_tensor<T> x1_neg_x1;
    asc_mul(x1_neg_x1.reg, x1.reg, neg_x1.reg, mask);
    asc_add(err.reg, err.reg, x1_neg_x1.reg, mask); // b + x1*(-x1)

    asc_mul_scalar(half_x.reg, x.reg, half_factor, mask);
    asc_madd(half_x.reg, err.reg, x1.reg, mask); // half_x = half_x * err + x1

    reg_tensor<T> scaled;
    asc_mul_scalar(scaled.reg, half_x.reg, scale_down, mask);
    asc_select(tmp.reg, scaled.reg, half_x.reg, is_sub.reg);

    // if (isinf(b) || b==0) keep b
    reg_tensor<uint32_t> src_bits = bitcast<uint32_t>(src_work);
    reg_tensor<uint32_t> pos_inf;
    asc_duplicate_scalar(pos_inf.reg, pos_inf_bits, mask);
    reg_tensor<bool> is_inf;
    asc_eq(is_inf.reg, src_bits.reg, pos_inf.reg, mask);

    reg_tensor<uint32_t> neg_zero;
    asc_duplicate_scalar(neg_zero.reg, neg_zero_bits, mask);
    reg_tensor<uint32_t> or_neg;
    asc_or(or_neg.reg, src_bits.reg, neg_zero.reg, mask);
    reg_tensor<bool> is_zero;
    asc_eq(is_zero.reg, or_neg.reg, neg_zero.reg, mask);

    reg_tensor<bool> keep_src;
    asc_or(keep_src.reg, is_zero.reg, is_inf.reg, mask);

    reg_tensor<T> dst;
    asc_select(dst.reg, src_work.reg, tmp.reg, keep_src.reg);
    dst.mask = mask;
    return dst;
}

// Soft-emulates basic_api SqrtImpl Subnormal scale path (PRECISION_1ULP_FTZ_FALSE).
template <typename T>
__simd_callee__ inline reg_tensor<T> sqrt_subnormal_scale(const reg_tensor<T>& src)
{
    static_assert(supports_float_math_v<T>, "sqrt_subnormal_scale supports half/float");
    const vector_bool mask = src.mask;
    reg_tensor<T> src_work = src;
    reg_tensor<T> tmp;
    reg_tensor<T> sqrt_tmp;
    reg_tensor<bool> is_sub;
    reg_tensor<T> dst;

    if constexpr (Std::is_same_v<T, half>) {
        // 0x6C00 = 2^10, 0x2400 = 2^-10, 0x03FF = max subnormal
        constexpr uint16_t scale_up_bits = 0x6C00;
        constexpr uint16_t scale_down_bits = 0x2400;
        constexpr uint16_t sub_thr_bits = 0x03FF;
        half scale_up;
        half scale_down;
        half sub_thr;
        *reinterpret_cast<uint16_t*>(&scale_up) = scale_up_bits;
        *reinterpret_cast<uint16_t*>(&scale_down) = scale_down_bits;
        *reinterpret_cast<uint16_t*>(&sub_thr) = sub_thr_bits;

        asc_lt_scalar(is_sub.reg, src_work.reg, sub_thr, mask);
        asc_mul_scalar(tmp.reg, src_work.reg, scale_up, mask);
        asc_select(src_work.reg, tmp.reg, src_work.reg, is_sub.reg);
        asc_sqrt(sqrt_tmp.reg, src_work.reg, mask);
        asc_mul_scalar(tmp.reg, sqrt_tmp.reg, scale_down, mask);
        asc_select(dst.reg, tmp.reg, sqrt_tmp.reg, is_sub.reg);
    } else {
        constexpr uint32_t scale_up_bits = 0x4B800000u;   // 2^24
        constexpr uint32_t scale_down_bits = 0x39800000u; // 2^-12
        constexpr uint32_t sub_thr_bits = 0x007FFFFFu;
        float scale_up;
        float scale_down;
        float sub_thr;
        *reinterpret_cast<uint32_t*>(&scale_up) = scale_up_bits;
        *reinterpret_cast<uint32_t*>(&scale_down) = scale_down_bits;
        *reinterpret_cast<uint32_t*>(&sub_thr) = sub_thr_bits;

        asc_lt_scalar(is_sub.reg, src_work.reg, sub_thr, mask);
        asc_mul_scalar(tmp.reg, src_work.reg, scale_up, mask);
        asc_select(src_work.reg, tmp.reg, src_work.reg, is_sub.reg);
        asc_sqrt(sqrt_tmp.reg, src_work.reg, mask);
        asc_mul_scalar(tmp.reg, sqrt_tmp.reg, scale_down, mask);
        asc_select(dst.reg, tmp.reg, sqrt_tmp.reg, is_sub.reg);
    }
    dst.mask = mask;
    return dst;
}

template <const sqrt_options& opt, typename T>
__simd_callee__ inline reg_tensor<T> sqrt_dispatch(const reg_tensor<T>& src)
{
    static_assert(supports_float_math_v<T>, "sqrt supports half and float");
    if constexpr (opt.precision_mode) {
        static_assert(Std::is_same_v<T, float>, "sqrt precision_mode (fast_inverse) only supports float");
        return sqrt_fast_inverse<T>(src);
    } else if constexpr (
        opt.algo == sqrt_algo::precision_0ulp_ftz_false || opt.algo == sqrt_algo::fast_inverse) {
        static_assert(Std::is_same_v<T, float>, "sqrt fast_inverse / 0ulp_ftz_false only supports float");
        return sqrt_fast_inverse<T>(src);
    } else {
#if !defined(__ASC_FTZ__)
        constexpr bool use_hw = opt.algo == sqrt_algo::precision_1ulp_ftz_true;
#else
        constexpr bool use_hw = opt.algo != sqrt_algo::precision_1ulp_ftz_false;
#endif
        if constexpr (use_hw) {
            reg_tensor<T> dst;
            asc_sqrt(dst.reg, src.reg, src.mask);
            dst.mask = src.mask;
            return dst;
        } else {
            return sqrt_subnormal_scale<T>(src);
        }
    }
}

// Soft-emulates basic_api DivPrecisionImpl core: residual compensation (float, ~0ulp).
template <typename T>
__simd_callee__ inline reg_tensor<T> div_diff_compensation(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(Std::is_same_v<T, float>, "div diff_compensation only supports float");
    const vector_bool mask = src0.mask;

    reg_tensor<T> z;
    asc_div(z.reg, src0.reg, src1.reg, mask);

    // Detect inf/nan/zero early results and keep hardware quotient there.
    constexpr uint32_t inf_nan_bound = 0xff800000u;
    constexpr uint32_t sign_bit = 0x80000000u;
    reg_tensor<uint32_t> z_bits = bitcast<uint32_t>(z);
    reg_tensor<uint32_t> neg_zero;
    asc_duplicate_scalar(neg_zero.reg, sign_bit, mask);
    reg_tensor<uint32_t> or_bits;
    asc_or(or_bits.reg, z_bits.reg, neg_zero.reg, mask);
    reg_tensor<uint32_t> bound;
    asc_duplicate_scalar(bound.reg, inf_nan_bound, mask);
    reg_tensor<bool> inf_nan;
    asc_ge(inf_nan.reg, or_bits.reg, bound.reg, mask);
    reg_tensor<bool> is_zero;
    asc_eq_scalar(is_zero.reg, z.reg, 0.0f, mask);
    reg_tensor<bool> keep_hw;
    asc_or(keep_hw.reg, inf_nan.reg, is_zero.reg, mask);

    // residual = a - b * z; e = residual / b; x = z + e
    reg_tensor<T> neg_b;
    asc_mul_scalar(neg_b.reg, src1.reg, -1.0f, mask);
    reg_tensor<T> residual = src0;
    reg_tensor<T> bz;
    asc_mul(bz.reg, z.reg, neg_b.reg, mask); // z * (-b)
    asc_add(residual.reg, residual.reg, bz.reg, mask);

    // Replace nan residual with 1 so the second div stays defined.
    reg_tensor<bool> residual_ok;
    asc_eq(residual_ok.reg, residual.reg, residual.reg, mask);
    reg_tensor<T> one;
    asc_duplicate_scalar(one.reg, 1.0f, mask);
    asc_select(residual.reg, residual.reg, one.reg, residual_ok.reg);

    reg_tensor<T> e;
    asc_div(e.reg, residual.reg, src1.reg, mask);
    reg_tensor<T> refined;
    asc_add(refined.reg, z.reg, e.reg, mask);

    reg_tensor<T> dst;
    asc_select(dst.reg, z.reg, refined.reg, keep_hw.reg);
    dst.mask = mask;
    return dst;
}

template <const div_options& opt, typename T>
__simd_callee__ inline reg_tensor<T> div_dispatch(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(supports_div_v<T>, "div does not support this element type");

    if constexpr (Std::is_same_v<T, float>) {
        if constexpr (opt.precision_mode) {
            if constexpr (opt.algo == div_algo::precision_0ulp_ftz_false) {
                // Full IEEE754 soft path is large; use compensation + note FTZ_FALSE via same refine.
                return div_diff_compensation<T>(src0, src1);
            } else {
                return div_diff_compensation<T>(src0, src1);
            }
        } else if constexpr (
            opt.algo == div_algo::diff_compensation || opt.algo == div_algo::precision_0ulp_ftz_true ||
            opt.algo == div_algo::precision_0ulp_ftz_false) {
            return div_diff_compensation<T>(src0, src1);
        } else if constexpr (opt.algo == div_algo::precision_1ulp_ftz_false) {
            // Soft Subnormal-preserving 1ulp: reuse compensation as stronger path.
            return div_diff_compensation<T>(src0, src1);
        }
    } else if constexpr (Std::is_same_v<T, half>) {
        if constexpr (opt.algo == div_algo::precision_1ulp_ftz_false
#if !defined(__ASC_FTZ__)
                      || opt.algo == div_algo::intrinsic
#endif
        ) {
            // Without a full half IEEE754 soft port, keep hardware div (FTZ_FALSE needs further work).
            reg_tensor<T> dst;
            asc_div(dst.reg, src0.reg, src1.reg, src0.mask);
            dst.mask = src0.mask;
            return dst;
        }
    }

    reg_tensor<T> dst;
    asc_div(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <const rsqrt_options& opt, typename T>
__simd_callee__ inline reg_tensor<T> rsqrt_dispatch(const reg_tensor<T>& src)
{
    static_assert(supports_float_math_v<T>, "rsqrt supports half and float");
    // Aligned with basic_api RegRsqrt: rsqrt = 1 / sqrt, with matching sqrt/div precision.
#if !defined(__ASC_FTZ__)
    constexpr bool use_hw_pair = opt.algo == rsqrt_algo::precision_1ulp_ftz_true;
#else
    constexpr bool use_hw_pair =
        opt.algo == rsqrt_algo::intrinsic || opt.algo == rsqrt_algo::precision_1ulp_ftz_true;
#endif
    if constexpr (use_hw_pair) {
        static constexpr sqrt_options k_sqrt_opt = {false, sqrt_algo::precision_1ulp_ftz_true};
        static constexpr div_options k_div_opt = {false, div_algo::precision_1ulp_ftz_true};
        auto s = sqrt_dispatch<k_sqrt_opt>(src);
        auto one = make_reg_operand<T>(static_cast<T>(1), src.mask);
        auto out = div_dispatch<k_div_opt>(one, s);
        reg_tensor<bool> is_neg;
        asc_lt_scalar(is_neg.reg, src.reg, static_cast<T>(0), src.mask);
        asc_select(out.reg, src.reg, out.reg, is_neg.reg);
        return out;
    } else if constexpr (Std::is_same_v<T, half>) {
        static constexpr sqrt_options k_sqrt_opt = {false, sqrt_algo::precision_1ulp_ftz_false};
        static constexpr div_options k_div_opt = {false, div_algo::precision_1ulp_ftz_false};
        auto s = sqrt_dispatch<k_sqrt_opt>(src);
        auto one = make_reg_operand<T>(static_cast<T>(1), src.mask);
        auto out = div_dispatch<k_div_opt>(one, s);
        reg_tensor<bool> is_neg;
        asc_lt_scalar(is_neg.reg, src.reg, static_cast<T>(0), src.mask);
        asc_select(out.reg, src.reg, out.reg, is_neg.reg);
        return out;
    } else {
        static constexpr sqrt_options k_sqrt_opt = {false, sqrt_algo::precision_0ulp_ftz_false};
        static constexpr div_options k_div_opt = {false, div_algo::precision_0ulp_ftz_false};
        auto s = sqrt_dispatch<k_sqrt_opt>(src);
        auto one = make_reg_operand<T>(static_cast<T>(1), src.mask);
        auto out = div_dispatch<k_div_opt>(one, s);
        reg_tensor<bool> is_neg;
        asc_lt_scalar(is_neg.reg, src.reg, static_cast<T>(0), src.mask);
        asc_select(out.reg, src.reg, out.reg, is_neg.reg);
        return out;
    }
}

} // namespace detail
} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_PRECISION_MATH_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PRECISION_MATH_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PRECISION_MATH_IMPL_H
#endif
