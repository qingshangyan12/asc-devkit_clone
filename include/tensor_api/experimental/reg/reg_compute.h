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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE
#endif

/*!
 * \file reg_compute.h
 * \brief Return-value-style register compute APIs for asc::te.
 *
 * This header provides chapter 4, "Basic compute", of vector_compute_api_manual.html.
 * These APIs are available only on NPU architecture 3510.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_REG_REG_COMPUTE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_REG_REG_COMPUTE_H

#include "impl/tensor_api/arch/vector/experimental/reg_compute_impl.h"

namespace asc {
namespace te {

// Unary arithmetic
template <typename T>
__simd_callee__ inline reg_tensor<T> abs(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> exp(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> sqrt(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> log(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> log2(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> log10(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> relu(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(reg_tensor<T> src, reg_tensor<T> slope);

template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(reg_tensor<T> src, detail::type_identity_t<T> slope);

template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> leaky_relu(reg_tensor<T> src, detail::type_identity_t<T> slope);

template <typename T>
__simd_callee__ inline reg_tensor<T> leaky_relu(reg_tensor<T> src);

// Binary arithmetic
template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(reg_tensor<T> src, detail::type_identity_t<T> value);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(detail::type_identity_t<T> value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(reg_tensor<T> src, detail::type_identity_t<T> value);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(detail::type_identity_t<T> value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(reg_tensor<T> src, detail::type_identity_t<T> value);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(detail::type_identity_t<T> value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(reg_tensor<T> src, detail::type_identity_t<T> value);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(detail::type_identity_t<T> value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> max(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> max(reg_tensor<T> src, detail::type_identity_t<T> value);

template <typename T>
__simd_callee__ inline reg_tensor<T> max(detail::type_identity_t<T> value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> min(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> min(reg_tensor<T> src, detail::type_identity_t<T> value);

template <typename T>
__simd_callee__ inline reg_tensor<T> min(detail::type_identity_t<T> value, reg_tensor<T> src);

// Bitwise operations
template <typename T>
__simd_callee__ inline reg_tensor<T> operator!(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator&(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator&(reg_tensor<T> src, Scalar value);

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator&(Scalar value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator|(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator|(reg_tensor<T> src, Scalar value);

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator|(Scalar value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator^(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator^(reg_tensor<T> src, Scalar value);

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator^(Scalar value, reg_tensor<T> src);

template <typename T, typename ShiftT>
__simd_callee__ inline reg_tensor<T> operator<<(reg_tensor<T> src, reg_tensor<ShiftT> shift);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator<<(reg_tensor<T> src, Scalar shift);

template <typename T, typename ShiftT>
__simd_callee__ inline reg_tensor<T> operator>>(reg_tensor<T> src, reg_tensor<ShiftT> shift);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable>
__simd_callee__ inline reg_tensor<T> operator>>(reg_tensor<T> src, Scalar shift);

// Comparisons
#define ASC_TE_DECLARE_COMPARISON(OP)                                                                                  \
    template <typename T>                                                                                              \
    __simd_callee__ inline reg_tensor<bool> operator OP(reg_tensor<T> src0, reg_tensor<T> src1);                      \
                                                                                                                       \
    template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable>                                  \
    __simd_callee__ inline reg_tensor<bool> operator OP(reg_tensor<T> src, Scalar value);                             \
                                                                                                                       \
    template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable>                                  \
    __simd_callee__ inline reg_tensor<bool> operator OP(Scalar value, reg_tensor<T> src)

ASC_TE_DECLARE_COMPARISON(==);
ASC_TE_DECLARE_COMPARISON(!=);
ASC_TE_DECLARE_COMPARISON(<);
ASC_TE_DECLARE_COMPARISON(<=);
ASC_TE_DECLARE_COMPARISON(>);
ASC_TE_DECLARE_COMPARISON(>=);

#undef ASC_TE_DECLARE_COMPARISON

} // namespace te
} // namespace asc
#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_REG_REG_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE
#endif
