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
#warning                                                                                                               \
    "impl/tensor_api/arch/vector/experimental/reg_compute_composite_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPOSITE_IMPL
#endif

/*!
 * \file reg_compute_composite_impl.h
 * \brief Register composite compute API implementations for asc::te.
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_COMPOSITE_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_COMPOSITE_IMPL_H

#include "impl/tensor_api/arch/utils/reg_compute_traits.h"

namespace asc {
namespace te {

namespace detail {

using uint32_reg = typename reg_tensor<uint32_t>::reg_type;
using int32_reg = typename reg_tensor<int32_t>::reg_type;

struct reg_uint64_parts {
    uint32_reg low;
    uint32_reg high;
};

__simd_callee__ inline vector_bool pack_uint64_mask(vector_bool mask)
{
    vector_bool packed;
    ppack(packed, mask, LOWER);
    return packed;
}

template <typename T>
__simd_callee__ inline reg_uint64_parts split_uint64(typename reg_tensor<T>::reg_type src)
{
    reg_uint64_parts dst;
    uint32_reg zero;
    vector_bool fullMask = asc_create_mask_b32(PAT_ALL);
    asc_duplicate_scalar(zero, static_cast<uint32_t>(0), fullMask);
    asc_deintlv(dst.low, dst.high, reinterpret_cast<uint32_reg&>(src), zero);
    return dst;
}

template <typename T>
__simd_callee__ inline typename reg_tensor<T>::reg_type combine_uint64(reg_uint64_parts src)
{
    typename reg_tensor<T>::reg_type dst;
    uint32_reg unused;
    asc_intlv(reinterpret_cast<uint32_reg&>(dst), unused, src.low, src.high);
    return dst;
}

template <typename T>
__simd_callee__ inline reg_uint64_parts duplicate_uint64(T value, vector_bool mask)
{
    const uint64_t bits = static_cast<uint64_t>(value);
    reg_uint64_parts dst;
    asc_duplicate_scalar(dst.low, static_cast<uint32_t>(bits), mask);
    asc_duplicate_scalar(dst.high, static_cast<uint32_t>(bits >> 32), mask);
    return dst;
}

__simd_callee__ inline reg_uint64_parts add_uint64(
    reg_uint64_parts src0, reg_uint64_parts src1, vector_bool mask)
{
    reg_uint64_parts dst;
    vector_bool carry;
    vector_bool unusedCarry;
    asc_add(carry, dst.low, src0.low, src1.low, mask);
    asc_addc(unusedCarry, dst.high, src0.high, src1.high, carry, mask);
    return dst;
}

__simd_callee__ inline reg_uint64_parts sub_uint64(
    reg_uint64_parts src0, reg_uint64_parts src1, vector_bool mask)
{
    reg_uint64_parts dst;
    vector_bool borrow;
    vector_bool unusedBorrow;
    asc_sub(borrow, dst.low, src0.low, src1.low, mask);
    asc_subc(unusedBorrow, dst.high, src0.high, src1.high, borrow, mask);
    return dst;
}

__simd_callee__ inline reg_uint64_parts mul_uint64(
    reg_uint64_parts src0, reg_uint64_parts src1, vector_bool mask)
{
    reg_uint64_parts dst;
    asc_mull(dst.low, dst.high, src0.low, src1.low, mask);
    vmula(dst.high, src0.low, src1.high, mask, MODE_ZEROING);
    vmula(dst.high, src0.high, src1.low, mask, MODE_ZEROING);
    return dst;
}

__simd_callee__ inline reg_uint64_parts abs_int64(reg_uint64_parts src, vector_bool mask)
{
    reg_uint64_parts zero;
    vector_bool negative;
    asc_duplicate_scalar(zero.low, static_cast<uint32_t>(0), mask);
    asc_duplicate_scalar(zero.high, static_cast<uint32_t>(0), mask);
    asc_lt(negative, reinterpret_cast<int32_reg&>(src.high), reinterpret_cast<int32_reg&>(zero.high), mask);
    reg_uint64_parts negated = sub_uint64(zero, src, negative);

    reg_uint64_parts dst;
    asc_select(dst.low, negated.low, src.low, negative);
    asc_select(dst.high, negated.high, src.high, negative);
    return dst;
}

} // namespace detail

/** @brief Computes scalar * src + dst element by element. */
template <typename T>
__simd_callee__ inline reg_tensor<T> axpy(
    reg_tensor<T> dst, reg_tensor<T> src, detail::type_identity_t<T> scalar)
{
    static_assert(detail::supports_axpy_v<T>, "axpy supports half, float, int64_t, and uint64_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (detail::supports_axpy_v<T>) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            vector_bool mask = detail::pack_uint64_mask(dst.mask);
            auto product = detail::mul_uint64(
                detail::split_uint64<T>(src.reg), detail::duplicate_uint64(scalar, mask), mask);
            auto result = detail::add_uint64(product, detail::split_uint64<T>(dst.reg), mask);
            dst.reg = detail::combine_uint64<T>(result);
        } else {
            asc_axpy(dst.reg, src.reg, scalar, dst.mask);
        }
    }
#endif
    return dst;
}

/** @brief Computes the absolute value of src0 - src1 element by element. */
template <typename T>
__simd_callee__ inline reg_tensor<T> abs_sub(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_abs_sub_v<T>, "abs_sub supports half, float, and int64_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (detail::supports_abs_sub_v<T>) {
        if constexpr (::AscendC::Std::is_same_v<T, int64_t>) {
            vector_bool mask = detail::pack_uint64_mask(src0.mask);
            auto difference = detail::sub_uint64(
                detail::split_uint64<T>(src0.reg), detail::split_uint64<T>(src1.reg), mask);
            dst.reg = detail::combine_uint64<T>(detail::abs_int64(difference, mask));
        } else {
            asc_abs_sub(dst.reg, src0.reg, src1.reg, src0.mask);
        }
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

/** @brief Computes exp(src0 - src1); half inputs are widened to float by the instruction. */
template <reg_layout Layout = reg_layout::zero, typename T>
__simd_callee__ inline reg_tensor<float> exp_sub(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<float> dst;
    static_assert(detail::supports_exp_sub_v<T>, "exp_sub supports half and float inputs and returns float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (detail::supports_exp_sub_v<T>) {
        if constexpr (Layout == reg_layout::zero) {
            vexpdif(dst.reg, src0.reg, src1.reg, src0.mask, PART_EVEN);
        } else {
            vexpdif(dst.reg, src0.reg, src1.reg, src0.mask, PART_ODD);
        }
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

/** @brief Computes dst * src0 + src1 element by element. */
template <typename T>
__simd_callee__ inline reg_tensor<T> mul_dst_add(
    reg_tensor<T> dst, reg_tensor<T> src0, reg_tensor<T> src1)
{
    static_assert(detail::supports_mul_dst_add_v<T>, "mul_dst_add supports half, bfloat16_t, and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (detail::supports_mul_dst_add_v<T>) {
        vmadd(dst.reg, src0.reg, src1.reg, dst.mask, MODE_ZEROING);
    }
#endif
    return dst;
}

/** @brief Computes src0 * src1 + dst element by element. */
template <typename T>
__simd_callee__ inline reg_tensor<T> mul_add_dst(
    reg_tensor<T> dst, reg_tensor<T> src0, reg_tensor<T> src1)
{
    static_assert(detail::supports_mul_add_dst_v<T>,
        "mul_add_dst supports int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float, int64_t, and uint64_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (detail::supports_mul_add_dst_v<T>) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            vector_bool mask = detail::pack_uint64_mask(dst.mask);
            auto product = detail::mul_uint64(
                detail::split_uint64<T>(src0.reg), detail::split_uint64<T>(src1.reg), mask);
            auto result = detail::add_uint64(product, detail::split_uint64<T>(dst.reg), mask);
            dst.reg = detail::combine_uint64<T>(result);
        } else {
            vmula(dst.reg, src0.reg, src1.reg, dst.mask, MODE_ZEROING);
        }
    }
#endif
    return dst;
}

/** @brief Multiplies float input by scalar and rounds the result to half. */
template <reg_layout Layout = reg_layout::zero>
__simd_callee__ inline reg_tensor<half> muls_cast(reg_tensor<float> src, float scalar)
{
    reg_tensor<half> dst;
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (Layout == reg_layout::zero) {
        vmulscvt(dst.reg, src.reg, scalar, src.mask, PART_EVEN);
    } else {
        vmulscvt(dst.reg, src.reg, scalar, src.mask, PART_ODD);
    }
    dst.mask = src.mask;
#endif
    return dst;
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_COMPOSITE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPOSITE_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPOSITE_IMPL
#endif
