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
    "impl/tensor_api/arch/vector/experimental/reg_compute_arithmetic_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_ARITHMETIC_IMPL
#endif

/*!
 * \file reg_compute_arithmetic_impl.h
 * \brief Register arithmetic API implementations for asc::te.
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_ARITHMETIC_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_ARITHMETIC_IMPL_H

#include "impl/tensor_api/arch/utils/reg_compute_traits.h"
#include "impl/tensor_api/utils/constant_impl.h"

namespace asc {
namespace te {

namespace detail {

template <typename T>
__simd_callee__ inline reg_tensor<T> broadcast_scalar(T value, vector_bool mask)
{
    reg_tensor<T> dst;
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510) {
        asc_duplicate_scalar(dst.reg, static_cast<T>(value), mask);
        dst.mask = mask;
    }
#endif
    return dst;
}

} // namespace detail

// =============================================================================
// Unary arithmetic
// =============================================================================

/** @brief Element-wise absolute value. */
template <typename T>
__simd_callee__ inline reg_tensor<T> abs(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_abs_v<T>,
        "abs supports int8_t, int16_t, half, int32_t, and float; documented int64_t/complex inputs "
        "require software-emulation support that has not been ported to reg_tensor yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_abs_v<T>) {
        asc_abs(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise natural exponential. */
template <typename T>
__simd_callee__ inline reg_tensor<T> exp(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "exp supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        asc_exp(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise square root. */
template <typename T>
__simd_callee__ inline reg_tensor<T> sqrt(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "sqrt supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        asc_sqrt(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise natural logarithm. */
template <typename T>
__simd_callee__ inline reg_tensor<T> log(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "log supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        asc_ln(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise base-2 logarithm. */
template <typename T>
__simd_callee__ inline reg_tensor<T> log2(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "log2 supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        reg_tensor<T> ln = log(src);
        asc_mul_scalar(dst.reg, ln.reg, static_cast<T>(1.4426950408889634), src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise base-10 logarithm. */
template <typename T>
__simd_callee__ inline reg_tensor<T> log10(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "log10 supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        reg_tensor<T> ln = log(src);
        asc_mul_scalar(dst.reg, ln.reg, static_cast<T>(0.4342944819032518), src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise arithmetic negation. */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_neg_v<T>,
        "unary operator- supports int8_t, int16_t, half, int32_t, and float; documented int64_t support "
        "requires software emulation that has not been ported to reg_tensor yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_neg_v<T>) {
        asc_neg(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise ReLU. */
template <typename T>
__simd_callee__ inline reg_tensor<T> relu(reg_tensor<T> src)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_relu_v<T>,
        "relu supports half, int32_t, and float; documented int64_t support requires software emulation "
        "that has not been ported to reg_tensor yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_relu_v<T>) {
        asc_relu(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/**
 * @brief Element-wise PReLU.
 * @param src Input values.
 * @param slope Per-lane negative-axis slopes.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(reg_tensor<T> src, reg_tensor<T> slope)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "prelu supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        asc_prelu(dst.reg, src.reg, slope.reg, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise PReLU with a broadcast scalar slope. */
template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(reg_tensor<T> src, detail::type_identity_t<T> slope)
{
    return prelu(src, detail::broadcast_scalar<T>(slope, src.mask));
}

/** @brief Element-wise PReLU with the one-argument form's default slope. */
template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(reg_tensor<T> src)
{
    return prelu(src, static_cast<T>(0.25f));
}

/** @brief Element-wise Leaky ReLU with a scalar negative-axis slope. */
template <typename T>
__simd_callee__ inline reg_tensor<T> leaky_relu(reg_tensor<T> src, detail::type_identity_t<T> slope)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_float_math_v<T>, "leaky_relu supports half and float");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_float_math_v<T>) {
        asc_leakyrelu(dst.reg, src.reg, slope, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

/** @brief Element-wise Leaky ReLU with the one-argument form's default slope. */
template <typename T>
__simd_callee__ inline reg_tensor<T> leaky_relu(reg_tensor<T> src)
{
    return leaky_relu(src, static_cast<T>(0.01f));
}

// =============================================================================
// Binary arithmetic
// =============================================================================

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_add_sub_v<T>,
        "operator+ supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, "
        "and float; documented 64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_add_sub_v<T>) {
        asc_add(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(reg_tensor<T> src, detail::type_identity_t<T> value)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_add_sub_v<T>,
        "scalar operator+ supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, "
        "and float; documented 64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_add_sub_v<T>) {
        asc_add_scalar(dst.reg, src.reg, value, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(detail::type_identity_t<T> value, reg_tensor<T> src)
{
    return src + value;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_add_sub_v<T>,
        "binary operator- supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, "
        "and float; documented 64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_add_sub_v<T>) {
        asc_sub(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(reg_tensor<T> src, detail::type_identity_t<T> value)
{
    static_assert(detail::supports_add_sub_v<T>,
        "scalar operator- supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, "
        "and float; documented 64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_add_sub_v<T>) {
        return src - detail::broadcast_scalar<T>(value, src.mask);
    }
#endif
    return {};
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(detail::type_identity_t<T> value, reg_tensor<T> src)
{
    static_assert(detail::supports_add_sub_v<T>,
        "reversed scalar operator- supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, "
        "uint32_t, and float; documented 64-bit/complex types require pending software emulation");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_add_sub_v<T>) {
        return detail::broadcast_scalar<T>(value, src.mask) - src;
    }
#endif
    return {};
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_mul_v<T>,
        "binary operator* supports int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, and float; "
        "documented 64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_mul_v<T>) {
        asc_mul(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(reg_tensor<T> src, detail::type_identity_t<T> value)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_mul_scalar_v<T>,
        "scalar operator* supports int16_t, uint16_t, half, int32_t, uint32_t, and float; documented "
        "64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_mul_scalar_v<T>) {
        asc_mul_scalar(dst.reg, src.reg, value, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(detail::type_identity_t<T> value, reg_tensor<T> src)
{
    return src * value;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_div_v<T>,
        "operator/ supports int16_t, uint16_t, half, int32_t, uint32_t, and float; documented 64-bit/complex "
        "types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_div_v<T>) {
        asc_div(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(reg_tensor<T> src, detail::type_identity_t<T> value)
{
    static_assert(detail::supports_div_v<T>,
        "scalar operator/ supports int16_t, uint16_t, half, int32_t, uint32_t, and float; documented "
        "64-bit/complex types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_div_v<T>) {
        return src / detail::broadcast_scalar<T>(value, src.mask);
    }
#endif
    return {};
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(detail::type_identity_t<T> value, reg_tensor<T> src)
{
    static_assert(detail::supports_div_v<T>,
        "reversed scalar operator/ supports int16_t, uint16_t, half, int32_t, uint32_t, and float; "
        "documented 64-bit/complex types require pending software emulation");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_div_v<T>) {
        return detail::broadcast_scalar<T>(value, src.mask) / src;
    }
#endif
    return {};
}

template <typename T>
__simd_callee__ inline reg_tensor<T> max(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_min_max_v<T>,
        "max supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, and float; "
        "documented 64-bit types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_min_max_v<T>) {
        asc_max(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> max(reg_tensor<T> src, detail::type_identity_t<T> value)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_min_max_v<T>,
        "scalar max supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, "
        "and float; documented 64-bit types require pending software emulation");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_min_max_v<T>) {
        asc_max_scalar(dst.reg, src.reg, value, src.mask);
        dst.mask = src.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> max(detail::type_identity_t<T> value, reg_tensor<T> src)
{
    return max(src, value);
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(reg_tensor<T> src0, reg_tensor<T> src1)
{
    reg_tensor<T> dst;
    static_assert(detail::supports_min_max_v<T>,
        "min supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, and float; "
        "documented 64-bit types require software emulation that has not been ported yet");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_min_max_v<T>) {
        asc_min(dst.reg, src0.reg, src1.reg, src0.mask);
        dst.mask = src0.mask;
    }
#endif
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(reg_tensor<T> src, detail::type_identity_t<T> value)
{
    static_assert(detail::supports_min_max_v<T>,
        "scalar min supports int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, "
        "and float; documented 64-bit types require pending software emulation");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (::AscendC::Te::CURRENT_ARCH_VERSION == ::AscendC::Te::ArchVersion::V3510 &&
        detail::supports_min_max_v<T>) {
        // asc_min_scalar has no bfloat16_t overload in the current backend.
        // Broadcasting preserves the documented combination without widening.
        return min(src, detail::broadcast_scalar<T>(value, src.mask));
    }
#endif
    return {};
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(detail::type_identity_t<T> value, reg_tensor<T> src)
{
    return min(src, value);
}


} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_COMPUTE_ARITHMETIC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_ARITHMETIC_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_ARITHMETIC_IMPL
#endif
