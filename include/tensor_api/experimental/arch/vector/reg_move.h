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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_MOVE_H
#endif

/*!
 * \file reg_move.h
 * \brief Register move for complex64 (soft-emulated).
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_MOVE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_MOVE_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @brief Copies a complex64 register tensor (Move).
 * @note Only @c complex_float / @c complex<float> is supported (complex64).
 */
__simd_callee__ inline complex<float> move(const complex<float>& src);

/**
 * @brief Merging move into @p dst for active lanes of @p mask (complex64 only).
 */
__simd_callee__ inline complex<float> move(
    const complex<float>& dst, const complex<float>& src, const reg_tensor<bool>& mask = all_mask<float>());

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/reg_move_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_MOVE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_MOVE_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_MOVE_H
#endif
