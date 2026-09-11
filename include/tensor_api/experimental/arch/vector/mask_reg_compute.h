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
 * @brief Creates a register mask with the specified pattern for an element type.
 * @return A register mask containing the selected pattern.
 * @note Supported element widths are b8, b16 and b32.
 */
template <mask_pattern Pattern = mask_pattern::all, typename DataType>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @override_id make_mask__z
 * @archs 3510
 * @brief 根据模式和数据元素宽度创建掩码寄存器，用于指示在计算过程中哪些元素参与计算。
 */
__simd_callee__ inline reg_tensor<bool> make_mask();

/**
 * @brief Creates a register mask with all lanes enabled for an element type.
 * @return A register mask with all lanes enabled.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @override_id all_mask__z
 * @archs 3510
 * @brief 创建`reg_tensor<bool>`，其成员变量reg是与`DataType`元素类型宽度匹配的全有效的掩码寄存器，等价于`make_mask<mask_pattern::all, DataType>()`。
 */
__simd_callee__ inline reg_tensor<bool> all_mask();

/**
 * @brief Creates a register mask with all lanes disabled for an element type.
 * @return A register mask with all lanes disabled.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @override_id none_mask__z
 * @archs 3510
 * @brief 创建`reg_tensor<bool>`，其成员变量reg是与`DataType`元素位宽匹配的全无效掩码寄存器，等价于`make_mask<mask_pattern::none, DataType>()`。
 */
__simd_callee__ inline reg_tensor<bool> none_mask();

/**
 * @brief Creates a mask for a vector iteration and updates the remaining element count.
 * @param remain Element count used to create the mask and updated for the subsequent call.
 * @return A register mask enabling the elements processed by the current iteration.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @override_id update_mask__Ru32
 * @archs 3510
 * @brief 根据当前剩余待处理元素数的值生成对应长度的有效位掩码，并自动将前剩余待处理元素数减去当前向量长度以更新剩余待处理元素数量。
 */
__simd_callee__ inline reg_tensor<bool> update_mask(uint32_t& remain);

/**
 * @brief Interleaves the lanes of two register masks for the specified element type.
 * @tparam DataType Element type whose width determines the mask lane granularity.
 * @param src0 First source register mask.
 * @param src1 Second source register mask.
 * @return A register-mask pair containing the interleaved sequence. The first and second halves of the sequence
 *         are stored in the first and second members, respectively.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
__simd_callee__ inline reg_pair<bool> interleave(reg_tensor<bool> src0, reg_tensor<bool> src1);

/**
 * @brief Deinterleaves the lanes of two register masks for the specified element type.
 * @tparam DataType Element type whose width determines the mask lane granularity.
 * @param src0 First half of the interleaved register-mask sequence.
 * @param src1 Second half of the interleaved register-mask sequence.
 * @return A register-mask pair containing the deinterleaved sequence. The first member contains the even lanes,
 *         and the second member contains the odd lanes.
 * @note Supported element widths are b8, b16 and b32.
 */
template <typename DataType>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.mask_reg_compute
 * @override_id deinterleave__regtensorg1x1_b_regtensorg1x1_b
 * @archs 3510
 * @brief 将两个源`reg_tensor<bool>`寄存器的reg按`DataType`对应的元素宽度解交织。`src0`和`src1`中偶数索引的分组依次写入返回值的`first.reg`，奇数索引的分组依次写入`second.reg`。
 * @param [in] src0 第一个源Mask寄存器，类型为`reg_tensor`，必须保证其`reg`成员已初始化。其偶数索引分组写入`first.reg`前半部分，奇数索引分组写入`second.reg`前半部分。
 * @param [in] src1 第二个源Mask寄存器，类型为`reg_tensor`，必须保证其`reg`成员已初始化。其偶数索引分组写入`first.reg`后半部分，奇数索引分组写入`second.reg`后半部分。
 */
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
