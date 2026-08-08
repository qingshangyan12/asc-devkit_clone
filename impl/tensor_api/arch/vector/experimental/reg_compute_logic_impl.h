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
    "impl/tensor_api/arch/vector/experimental/reg_compute_logic_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOGIC_IMPL
#endif

/*!
 * \file reg_compute_logic_impl.h
 * \brief Register bitwise, shift, and comparison API implementations for asc::te.
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_LOGIC_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_LOGIC_IMPL_H

#include "impl/tensor_api/arch/vector/experimental/reg_compute_arithmetic_impl.h"

namespace asc {
namespace te {

// =============================================================================
// Bitwise operations
// =============================================================================

template <typename T>
__simd_callee__ inline reg_tensor<T> operator!(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_not_v<T>,
        "operator! supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_not_v<T>) {
        vector_bool isZero;
        asc_eq_scalar(isZero, src.reg, static_cast<T>(0), src.mask);
        asc_duplicate_scalar(dst.reg, static_cast<T>(0), src.mask);
        asc_duplicate_scalar(dst.reg, static_cast<T>(1), isZero);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator&(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_bitwise_v<T>,
        "operator& supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_bitwise_v<T>) {
        asc_and(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator&(reg_tensor<T> src, Scalar value)
{
    static_assert(detail::supports_bitwise_v<T>,
        "scalar operator& supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_bitwise_v<T>) {
        return src & detail::broadcast_scalar<T>(static_cast<T>(value), src.mask);
    }
#endif
    return {};
}

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator&(Scalar value, reg_tensor<T> src)
{
    return src & value;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator|(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_bitwise_v<T>,
        "operator| supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_bitwise_v<T>) {
        asc_or(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator|(reg_tensor<T> src, Scalar value)
{
    static_assert(detail::supports_bitwise_v<T>,
        "scalar operator| supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_bitwise_v<T>) {
        return src | detail::broadcast_scalar<T>(static_cast<T>(value), src.mask);
    }
#endif
    return {};
}

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator|(Scalar value, reg_tensor<T> src)
{
    return src | value;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator^(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_bitwise_v<T>,
        "operator^ supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_bitwise_v<T>) {
        asc_xor(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator^(reg_tensor<T> src, Scalar value)
{
    static_assert(detail::supports_bitwise_v<T>,
        "scalar operator^ supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_bitwise_v<T>) {
        return src ^ detail::broadcast_scalar<T>(static_cast<T>(value), src.mask);
    }
#endif
    return {};
}

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator^(Scalar value, reg_tensor<T> src)
{
    return src ^ value;
}

template <typename T, typename ShiftT>
__simd_callee__ inline reg_tensor<T> operator<<(reg_tensor<T> src, reg_tensor<ShiftT> shift)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_shift_pair_v<T, ShiftT>,
        "register operator<< requires an integer source and a signed shift register of the same width");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_shift_pair_v<T, ShiftT>) {
        asc_shiftleft(dst.reg, src.reg, shift.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator<<(reg_tensor<T> src, Scalar shift)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_shift_v<T>,
        "scalar operator<< supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
    static_assert(::AscendC::Std::is_integral_v<::AscendC::Std::remove_cvref_t<Scalar>>,
        "scalar operator<< requires an integer shift value");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_shift_v<T> && ::AscendC::Std::is_integral_v<::AscendC::Std::remove_cvref_t<Scalar>>) {
        asc_shiftleft_scalar(dst.reg, src.reg, static_cast<int16_t>(shift), src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T, typename ShiftT>
__simd_callee__ inline reg_tensor<T> operator>>(reg_tensor<T> src, reg_tensor<ShiftT> shift)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_shift_pair_v<T, ShiftT>,
        "register operator>> requires an integer source and a signed shift register of the same width");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_shift_pair_v<T, ShiftT>) {
        asc_shiftright(dst.reg, src.reg, shift.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> = 0>
__simd_callee__ inline reg_tensor<T> operator>>(reg_tensor<T> src, Scalar shift)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_shift_v<T>,
        "scalar operator>> supports int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t");
    static_assert(::AscendC::Std::is_integral_v<::AscendC::Std::remove_cvref_t<Scalar>>,
        "scalar operator>> requires an integer shift value");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_shift_v<T> && ::AscendC::Std::is_integral_v<::AscendC::Std::remove_cvref_t<Scalar>>) {
        asc_shiftright_scalar(dst.reg, src.reg, static_cast<int16_t>(shift), src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

// =============================================================================
// Comparisons
// =============================================================================

namespace detail {

enum class comparison_op { EQ, NE, LT, LE, GT, GE };

template <comparison_op Op, typename T>
__simd_callee__ inline reg_tensor<bool> compare(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<bool> dst;
    static_assert(supports_compare_v<T>, "comparison does not support this element type");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        supports_compare_v<T>) {
        if constexpr (Op == comparison_op::EQ) {
            asc_eq(dst.reg, src0.reg, src1.reg, src0.mask);
        } else if constexpr (Op == comparison_op::NE) {
            asc_ne(dst.reg, src0.reg, src1.reg, src0.mask);
        } else if constexpr (Op == comparison_op::LT) {
            asc_lt(dst.reg, src0.reg, src1.reg, src0.mask);
        } else if constexpr (Op == comparison_op::LE) {
            asc_le(dst.reg, src0.reg, src1.reg, src0.mask);
        } else if constexpr (Op == comparison_op::GT) {
            asc_gt(dst.reg, src0.reg, src1.reg, src0.mask);
        } else {
            asc_ge(dst.reg, src0.reg, src1.reg, src0.mask);
        }
    }
#endif
    return dst;
}

template <comparison_op Op, typename T, typename Scalar>
__simd_callee__ inline reg_tensor<bool> compare(reg_tensor<T> src, Scalar value)
{
    reg_tensor<bool> dst;
    static_assert(supports_compare_v<T>, "comparison does not support this element type");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        supports_compare_v<T>) {
        if constexpr (Op == comparison_op::EQ) {
            asc_eq_scalar(dst.reg, src.reg, static_cast<T>(value), src.mask);
        } else if constexpr (Op == comparison_op::NE) {
            asc_ne_scalar(dst.reg, src.reg, static_cast<T>(value), src.mask);
        } else if constexpr (Op == comparison_op::LT) {
            asc_lt_scalar(dst.reg, src.reg, static_cast<T>(value), src.mask);
        } else if constexpr (Op == comparison_op::LE) {
            asc_le_scalar(dst.reg, src.reg, static_cast<T>(value), src.mask);
        } else if constexpr (Op == comparison_op::GT) {
            asc_gt_scalar(dst.reg, src.reg, static_cast<T>(value), src.mask);
        } else {
            asc_ge_scalar(dst.reg, src.reg, static_cast<T>(value), src.mask);
        }
    }
#endif
    return dst;
}

} // namespace detail

#define ASC_TE_DEFINE_COMPARISON(OP, KIND, REVERSED_KIND)                                                        \
    template <typename T>                                                                                         \
    __simd_callee__ inline reg_tensor<bool> operator OP(reg_tensor<T> src0, reg_tensor<T> src1)                  \
    {                                                                                                             \
        return detail::compare<detail::comparison_op::KIND>(src0, src1);                                         \
    }                                                                                                             \
                                                                                                                  \
    template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> = 0>                                \
    __simd_callee__ inline reg_tensor<bool> operator OP(reg_tensor<T> src, Scalar value)                          \
    {                                                                                                             \
        return detail::compare<detail::comparison_op::KIND>(src, value);                                         \
    }                                                                                                             \
                                                                                                                  \
    template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> = 0>                                \
    __simd_callee__ inline reg_tensor<bool> operator OP(Scalar value, reg_tensor<T> src)                          \
    {                                                                                                             \
        return detail::compare<detail::comparison_op::REVERSED_KIND>(src, value);                                \
    }

ASC_TE_DEFINE_COMPARISON(==, EQ, EQ)
ASC_TE_DEFINE_COMPARISON(!=, NE, NE)
ASC_TE_DEFINE_COMPARISON(<, LT, GT)
ASC_TE_DEFINE_COMPARISON(<=, LE, GE)
ASC_TE_DEFINE_COMPARISON(>, GT, LT)
ASC_TE_DEFINE_COMPARISON(>=, GE, LE)

#undef ASC_TE_DEFINE_COMPARISON

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_LOGIC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOGIC_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOGIC_IMPL
#endif
