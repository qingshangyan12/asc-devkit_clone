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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DATA_PADDING_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_DATA_PADDING_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_DATA_PADDING_H

#include <cstddef>

#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"

namespace asc {
namespace te {
namespace experimental {
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.data_padding
 * @brief Fills masked elements of a register tensor with a scalar value.
 *
 * @override_id te_experimental_fill__const_T_R_const_reg_tensor_bool_R__template_N_T
 * @archs 3510
 * @tparam N Number of register tensors to fill. Only 1 is supported.
 * @tparam T Element type of the register tensor.
 * @return A register tensor containing the filled elements and the input mask.
 * @param [in] value Scalar value copied to each enabled element.
 * @param [in] mask Mask that specifies the elements to fill.
 * @note The active-lane mask is taken from mask.reg. A value of 1 selects the corresponding element; a value of 0
 * excludes it. Unselected elements are set to zero. The result preserves the active-lane mask.
 */
template <size_t N = 1, typename T>
__simd_callee__ inline reg_tensor<T> fill(const T& value, const reg_tensor<bool>& mask = all_mask<T>());

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.data_padding
 * @brief Copies masked elements from a register tensor.
 *
 * @override_id te_experimental_fill__const_reg_tensor_T_R_const_reg_tensor_bool_R__template_N_T
 * @archs 3510
 * @tparam N Number of register tensors to fill. Only 1 is supported.
 * @tparam T Element type of the register tensor.
 * @return A register tensor containing the copied elements and the input mask.
 * @param [in] src Source register tensor.
 * @param [in] mask Mask that specifies the elements to copy.
 * @note The active-lane mask is taken from mask.reg. A value of 1 selects the corresponding element; a value of 0
 * excludes it. Unselected elements are set to zero. The result preserves the active-lane mask.
 */
template <size_t N = 1, typename T>
__simd_callee__ inline reg_tensor<T> fill(const reg_tensor<T>& src, const reg_tensor<bool>& mask = all_mask<T>());
} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/data_padding_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_DATA_PADDING_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DATA_PADDING_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DATA_PADDING_H
#endif
