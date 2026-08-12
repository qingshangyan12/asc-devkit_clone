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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_REG_LOGICAL_COMPUTE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_REG_LOGICAL_COMPUTE_H

#include "impl/tensor_api/arch/utils/reg_compute_traits.h"

namespace asc {
namespace te {

// Bitwise operations
template <typename T>
__simd_callee__ inline reg_tensor<T> operator!(reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator&(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator&(reg_tensor<T> src, Scalar value);

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator&(Scalar value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator|(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator|(reg_tensor<T> src, Scalar value);

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator|(Scalar value, reg_tensor<T> src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator^(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator^(reg_tensor<T> src, Scalar value);

template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator^(Scalar value, reg_tensor<T> src);

template <typename T, typename ShiftT>
__simd_callee__ inline reg_tensor<T> operator<<(reg_tensor<T> src, reg_tensor<ShiftT> shift);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator<<(reg_tensor<T> src, Scalar shift);

template <typename T, typename ShiftT>
__simd_callee__ inline reg_tensor<T> operator>>(reg_tensor<T> src, reg_tensor<ShiftT> shift);

template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable = 0>
__simd_callee__ inline reg_tensor<T> operator>>(reg_tensor<T> src, Scalar shift);

// Comparisons
#define ASC_TE_DECLARE_COMPARISON(OP)                                                                                  \
    template <typename T>                                                                                              \
    __simd_callee__ inline reg_tensor<bool> operator OP(reg_tensor<T> src0, reg_tensor<T> src1);                      \
                                                                                                                       \
    template <typename T, typename Scalar, detail::enable_if_scalar_t<Scalar> Enable = 0>                              \
    __simd_callee__ inline reg_tensor<bool> operator OP(reg_tensor<T> src, Scalar value);                             \
                                                                                                                       \
    template <typename Scalar, typename T, detail::enable_if_scalar_t<Scalar> Enable = 0>                              \
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

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/arch/vector/experimental/logical_compute_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_REG_LOGICAL_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE
#endif
