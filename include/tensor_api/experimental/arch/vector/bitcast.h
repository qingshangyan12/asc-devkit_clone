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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_BITCAST_H
#endif

/*!
 * \file bitcast.h
 * \brief Same-width bit reinterpretation for experimental register tensors.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BITCAST_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BITCAST_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @brief Reinterprets register bits as another same-width element type (by value).
 * @tparam DstType Destination element type.
 * @tparam SrcType Source element type (deduced).
 * @param src Source register tensor.
 * @return A new @c reg_tensor whose @c reg bits match @p src and whose @c mask is a copy of @p src.mask.
 *
 * @note Same-width only. Orthogonal to numeric @c cast / @c trunc.
 * @note Does not support @c bool element types. Prefer this over reinterpret_cast on a whole @c reg_tensor.
 */
template <typename DstType, typename SrcType>
__simd_callee__ inline reg_tensor<DstType> bitcast(const reg_tensor<SrcType>& src);

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/bitcast_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BITCAST_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_BITCAST_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_BITCAST_H
#endif
