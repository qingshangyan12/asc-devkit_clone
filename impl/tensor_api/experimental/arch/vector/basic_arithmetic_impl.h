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
#warning "impl/tensor_api/experimental/arch/vector/basic_arithmetic_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"

namespace asc {
namespace te {
namespace experimental {

template <typename T>
__simd_callee__ inline reg_tensor<T> abs(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_abs_v<T>, "abs does not support this element type");
    reg_tensor<T> dst;
    asc_abs(dst.reg, src_reg.reg, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> exp(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_float_math_v<T>, "exp does not support this element type");
    reg_tensor<T> dst;
    asc_exp(dst.reg, src_reg.reg, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> sqrt(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_float_math_v<T>, "sqrt does not support this element type");
    reg_tensor<T> dst;
    asc_sqrt(dst.reg, src_reg.reg, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> log2(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_float_math_v<T>, "log2 does not support this element type");
    constexpr float ln2_reciprocal = 1.4426950408889634;
    reg_tensor<T> natural_log;
    asc_ln(natural_log.reg, src_reg.reg, src_reg.mask);
    reg_tensor<T> dst;
    asc_mul_scalar(dst.reg, natural_log.reg, static_cast<T>(ln2_reciprocal), src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> log10(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_float_math_v<T>, "log10 does not support this element type");
    constexpr float ln10_reciprocal = 0.43429448190325176;
    reg_tensor<T> natural_log;
    asc_ln(natural_log.reg, src_reg.reg, src_reg.mask);
    reg_tensor<T> dst;
    asc_mul_scalar(dst.reg, natural_log.reg, static_cast<T>(ln10_reciprocal), src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_neg_v<T>, "unary operator- does not support this element type");
    reg_tensor<T> dst;
    asc_neg(dst.reg, src_reg.reg, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> relu(const reg_tensor<T>& src_reg)
{
    static_assert(detail::supports_relu_v<T>, "relu does not support this element type");
    reg_tensor<T> dst;
    asc_relu(dst.reg, src_reg.reg, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(const reg_tensor<T>& src_reg, const reg_tensor<T>& slope)
{
    static_assert(detail::supports_float_math_v<T>, "prelu does not support this element type");
    reg_tensor<T> dst;
    asc_prelu(dst.reg, src_reg.reg, slope.reg, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> leaky_relu(const reg_tensor<T>& src_reg, const T& slope)
{
    static_assert(detail::supports_float_math_v<T>, "leaky_relu does not support this element type");
    reg_tensor<T> dst;
    asc_leakyrelu(dst.reg, src_reg.reg, slope, src_reg.mask);
    dst.mask = src_reg.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> addc(reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_carry_v<T>, "addc does not support this element type");
    reg_tensor<T> dst;
    asc_add(carry.reg, dst.reg, src0.reg, src1.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> addc(reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1,
                                          const reg_tensor<bool>& carry_src)
{
    static_assert(detail::supports_carry_v<T>, "addc does not support this element type");
    reg_tensor<T> dst;
    asc_addc(carry.reg, dst.reg, src0.reg, src1.reg, carry_src.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> subc(reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_carry_v<T>, "subc does not support this element type");
    reg_tensor<T> dst;
    asc_sub(carry.reg, dst.reg, src0.reg, src1.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> subc(reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1,
                                          const reg_tensor<bool>& carry_src)
{
    static_assert(detail::supports_carry_v<T>, "subc does not support this element type");
    reg_tensor<T> dst;
    asc_subc(carry.reg, dst.reg, src0.reg, src1.reg, carry_src.reg, src0.mask);
    carry.mask = src0.mask;
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_pair<T> mull(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_mull_v<T>, "mull does not support this element type");
    reg_pair<T> result;
    asc_mull(result.first.reg, result.second.reg, src0.reg, src1.reg, src0.mask);
    result.first.mask = src0.mask;
    result.second.mask = src0.mask;
    return result;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_div_v<T>, "operator/ does not support this element type");
    reg_tensor<T> dst;
    asc_div(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_min_max_v<T>, "min does not support this element type");
    reg_tensor<T> dst;
    asc_min(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_min_max_v<T>, "min does not support this element type");
    reg_tensor<T> dst;
    asc_min_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(const T& scalar, const reg_tensor<T>& src)
{
    static_assert(detail::supports_min_max_v<T>, "min does not support this element type");
    reg_tensor<T> dst;
    asc_min_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL
#endif
