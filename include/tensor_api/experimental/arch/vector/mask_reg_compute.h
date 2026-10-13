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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_MASK_REG_COMPUTE_H
#endif

/*!
 * \file mask_reg_compute.h
 * \brief Register mask construction and update interfaces.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_MASK_REG_COMPUTE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_MASK_REG_COMPUTE_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/utils/reg_enum.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @brief Creates a register mask with the specified pattern for an element type.
 *
 * @override_id te_experimental_make_mask__void__template_Pattern_DataType
 * @archs 3510
 * @return A register mask containing the selected pattern.
 * @note Supported element widths are b8, b16 and b32.
 */
template <mask_pattern Pattern = mask_pattern::all, typename DataType>
__simd_callee__ inline reg_tensor<bool> make_mask();

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @brief Creates a register mask with all lanes enabled for an element type.
 *
 * @override_id te_experimental_all_mask__void__template_DataType
 * @archs 3510
 * @return A register mask with all lanes enabled.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
__simd_callee__ inline reg_tensor<bool> all_mask();

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @brief Creates a register mask with all lanes disabled for an element type.
 *
 * @override_id te_experimental_none_mask__void__template_DataType
 * @archs 3510
 * @return A register mask with all lanes disabled.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
__simd_callee__ inline reg_tensor<bool> none_mask();

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @brief Creates a mask for a vector iteration and updates the remaining element count.
 *
 * @override_id te_experimental_update_mask__uint32_t_R__template_DataType
 * @archs 3510
 * @return A register mask enabling the elements processed by the current iteration.
 * @note Supported element widths are b8, b16 and b32.
 * @param [in,out] remain Element count used to create the mask and updated for the subsequent call.
 */
template <typename DataType>
__simd_callee__ inline reg_tensor<bool> update_mask(uint32_t& remain);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @brief Interleaves the lanes of two register masks for the specified element type.
 *
 * @override_id te_experimental_interleave__reg_tensor_bool_reg_tensor_bool__template_DataType
 * @archs 3510
 * @tparam DataType Element type whose width determines the mask lane granularity.
 * @return A register-mask pair containing the interleaved sequence. The first and second halves of the sequence are
 * stored in the first and second members, respectively.
 * @note Supported element widths are b8, b16 and b32.
 * @param [in] src0 First source register mask.
 * @param [in] src1 Second source register mask.
 */
template <typename DataType>
__simd_callee__ inline reg_pair<bool> interleave(reg_tensor<bool> src0, reg_tensor<bool> src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @brief Deinterleaves the lanes of two register masks for the specified element type.
 *
 * @override_id te_experimental_deinterleave__reg_tensor_bool_reg_tensor_bool__template_DataType
 * @archs 3510
 * @tparam DataType Element type whose width determines the mask lane granularity.
 * @return A register-mask pair containing the deinterleaved sequence. The first member contains the even lanes, and the
 * second member contains the odd lanes.
 * @note Supported element widths are b8, b16 and b32.
 * @param [in] src0 First half of the interleaved register-mask sequence.
 * @param [in] src1 Second half of the interleaved register-mask sequence.
 */
template <typename DataType>
__simd_callee__ inline reg_pair<bool> deinterleave(reg_tensor<bool> src0, reg_tensor<bool> src1);

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/mask_reg_compute_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_MASK_REG_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_MASK_REG_COMPUTE_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_MASK_REG_COMPUTE_H
#endif
