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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_SEL_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_COMPUTE_SEL_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_COMPUTE_SEL_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/utils/reg_enum.h"

namespace asc {
namespace te {
namespace experimental {
/**
 * @brief Packs a register tensor into the selected low or high half-width representation.
 * @tparam part Destination half selected for the pack operation.
 * @tparam T Element type of the source register tensor.
 * @param src Source register tensor.
 * @return A register tensor containing the packed data. The result mask follows src.
 */
template <high_low_part part = high_low_part::low, typename T>
__simd_callee__ inline decltype(auto) pack(reg_tensor<T> src);

/**
 * @brief Unpacks a register tensor from the selected low or high half-width representation.
 * @tparam part Source half selected for the unpack operation.
 * @tparam T Element type of the source register tensor.
 * @param src Source register tensor.
 * @return A register tensor containing the unpacked data. The result mask follows src.
 */
template <high_low_part part = high_low_part::low, typename T>
__simd_callee__ inline decltype(auto) unpack(reg_tensor<T> src);

/**
 * @brief Compresses the lanes selected by mask into contiguous low lanes.
 * @tparam T Element type of the source register tensor.
 * @param src Source register tensor.
 * @param mask Lane-selection mask.
 * @return A register tensor containing selected elements packed into low lanes.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> compress(reg_tensor<T> src, reg_tensor<bool> mask);

/**
 * @brief Generates exclusive prefix-count indexes from a mask.
 * @tparam T Element type of the generated index register tensor.
 * @param mask Lane-selection mask.
 * @return A register tensor containing prefix-count indexes. The result mask is mask.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> expand(reg_tensor<bool> mask);
} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/reg_compute_sel_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_COMPUTE_SEL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_SEL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_SEL_H
#endif
