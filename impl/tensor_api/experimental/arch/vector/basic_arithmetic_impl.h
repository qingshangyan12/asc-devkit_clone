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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"

namespace asc {
namespace te {
namespace experimental {
template <typename T>
__simd_callee__ inline reg_tensor<T> log(const reg_tensor<T>& src)
{
    static_assert(detail::supports_float_math_v<T>, "log supports half and float");
    reg_tensor<T> dst;
    asc_ln(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_add_sub_v<T>, "operator+ does not support this element type");
    reg_tensor<T> dst;
    asc_add(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_add_sub_v<T>, "scalar operator+ does not support this element type");
    reg_tensor<T> dst;
    asc_add_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const T& scalar, const reg_tensor<T>& src)
{
    return src + scalar;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> addc(reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_carry_v<T>, "addc supports uint32_t");
    reg_tensor<T> dst;
    asc_add(carry.reg, dst.reg, src0.reg, src1.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> addc(
    reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<bool>& carry_src)
{
    static_assert(detail::supports_carry_v<T>, "addc supports uint32_t");
    reg_tensor<T> dst;
    asc_addc(carry.reg, dst.reg, src0.reg, src1.reg, carry_src.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator-(const T& a, const U& b)
{
    using elem_type = detail::binary_elem_t<T, U>;
    detail::check_binary_reg_types<T, U>();
    static_assert(detail::supports_add_sub_v<elem_type>, "operator- does not support this element type");

    vector_bool mask = detail::binary_mask(a, b);
    auto left = detail::make_reg_operand<elem_type>(a, mask);
    auto right = detail::make_reg_operand<elem_type>(b, mask);
    reg_tensor<elem_type> dst;
    asc_sub(dst.reg, left.reg, right.reg, mask);
    dst.mask = mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> subc(reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_carry_v<T>, "subc supports uint32_t");
    reg_tensor<T> dst;
    asc_sub(carry.reg, dst.reg, src0.reg, src1.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> subc(
    reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<bool>& carry_src)
{
    static_assert(detail::supports_carry_v<T>, "subc supports uint32_t");
    reg_tensor<T> dst;
    asc_subc(carry.reg, dst.reg, src0.reg, src1.reg, carry_src.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_mul_scalar_v<T>, "scalar operator* does not support this element type");
    reg_tensor<T> dst;
    asc_mul_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const T& scalar, const reg_tensor<T>& src)
{
    return src * scalar;
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) max(const T& a, const U& b)
{
    using elem_type = detail::binary_elem_t<T, U>;
    detail::check_binary_reg_types<T, U>();
    static_assert(detail::supports_min_max_v<elem_type>, "max does not support this element type");

    reg_tensor<elem_type> dst;
    vector_bool mask = detail::binary_mask(a, b);
    if constexpr (is_reg_tensor_v<T> && is_reg_tensor_v<U>) {
        asc_max(dst.reg, a.reg, b.reg, mask);
    } else if constexpr (is_reg_tensor_v<T>) {
        asc_max_scalar(dst.reg, a.reg, static_cast<elem_type>(b), mask);
    } else {
        asc_max_scalar(dst.reg, b.reg, static_cast<elem_type>(a), mask);
    }
    dst.mask = mask;
    return dst;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL_H
#endif
