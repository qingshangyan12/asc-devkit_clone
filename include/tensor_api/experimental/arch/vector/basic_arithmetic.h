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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_H

#include "tensor_api/tensor/tensor.h"
#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/utils/reg_enum.h"

namespace asc {
namespace te {
namespace experimental {

template <typename T>
__simd_callee__ inline reg_tensor<T> log(const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const T& scalar, const reg_tensor<T>& src);

/**
 * @brief Element-wise add with configurable mask merge mode.
 * @tparam Mode zeroing (default): inactive lanes are zeroed;
 *         merging: inactive lanes keep values from @p src0 (soft-emulated like basic_api).
 */
template <merge_mode Mode = merge_mode::zeroing, typename T>
__simd_callee__ inline reg_tensor<T> add(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const T& scalar, const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const T& scalar, const reg_tensor<T>& src);

/**
 * @brief Element-wise max with configurable mask merge mode.
 * @tparam Mode zeroing (default): inactive lanes are zeroed;
 *         merging: inactive lanes keep values from @p src0 / @p src (soft-emulated like basic_api).
 */
template <merge_mode Mode = merge_mode::zeroing, typename T>
__simd_callee__ inline reg_tensor<T> max(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <merge_mode Mode = merge_mode::zeroing, typename T>
__simd_callee__ inline reg_tensor<T> max(const reg_tensor<T>& src, const T& scalar);

template <merge_mode Mode = merge_mode::zeroing, typename T>
__simd_callee__ inline reg_tensor<T> max(const T& scalar, const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline auto abs(const reg_tensor<T>& src);

template <typename T, Std::enable_if_t<is_complex_tag_v<T>, int> = 0>
__simd_callee__ inline reg_tensor<T> operator+(
    const reg_tensor<T>& src, const complex_scalar<complex_elem_t<T>>& scalar);

template <typename T, Std::enable_if_t<is_complex_tag_v<T>, int> = 0>
__simd_callee__ inline reg_tensor<T> operator+(
    const complex_scalar<complex_elem_t<T>>& scalar, const reg_tensor<T>& src);

template <typename T, Std::enable_if_t<is_complex_tag_v<T>, int> = 0>
__simd_callee__ inline reg_tensor<T> operator*(
    const reg_tensor<T>& src, const complex_scalar<complex_elem_t<T>>& scalar);

template <typename T, Std::enable_if_t<is_complex_tag_v<T>, int> = 0>
__simd_callee__ inline reg_tensor<T> operator*(
    const complex_scalar<complex_elem_t<T>>& scalar, const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> exp(const reg_tensor<T>& src);

/**
 * @brief Element-wise sqrt with configurable precision / Subnormal policy.
 * @tparam opt Aligned with basic_api SqrtSpecificMode / SqrtConfig.
 */
template <const sqrt_options& opt = default_sqrt_ops, typename T>
__simd_callee__ inline reg_tensor<T> sqrt(const reg_tensor<T>& src);

/**
 * @brief Element-wise rsqrt (1/sqrt) with configurable precision / Subnormal policy.
 * @tparam opt Aligned with basic_api RsqrtConfig; implemented as composed sqrt+div like RegRsqrt.
 */
template <const rsqrt_options& opt = default_rsqrt_ops, typename T>
__simd_callee__ inline reg_tensor<T> rsqrt(const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> log2(const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> log10(const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> relu(const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(const reg_tensor<T>& src, const reg_tensor<T>& slope);

template <typename T, typename ScalarType>
__simd_callee__ inline reg_tensor<T> leaky_relu(const reg_tensor<T>& src, const ScalarType& slope);

template <typename T>
__simd_callee__ inline reg_tensor<T> add(
    reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> add_c(
    reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src);

template <typename T>
__simd_callee__ inline reg_tensor<T> sub(
    reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> sub_c(
    reg_tensor<bool>& carry, const reg_tensor<T>& src0, const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src);

template <typename T>
__simd_callee__ inline reg_tensor<T> mull(
    reg_tensor<T>& high, const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator/(const T& a, const U& b);

/**
 * @brief Element-wise div with configurable precision / Subnormal policy.
 * @tparam opt Aligned with basic_api DivSpecificMode / DivConfig.
 * @note @c operator/ keeps the default intrinsic path; use @c div when selecting algo.
 */
template <const div_options& opt = default_div_ops, typename T, typename U>
__simd_callee__ inline decltype(auto) div(const T& a, const U& b);

/**
 * @brief Element-wise min with configurable mask merge mode.
 * @tparam Mode zeroing (default): inactive lanes are zeroed;
 *         merging: inactive lanes keep values from the first reg_tensor operand.
 */
template <merge_mode Mode = merge_mode::zeroing, typename T, typename U>
__simd_callee__ inline decltype(auto) min(const T& a, const U& b);

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/basic_arithmetic_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC
#endif
