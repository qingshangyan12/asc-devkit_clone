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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_LOGICAL_COMPUTE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_LOGICAL_COMPUTE_H

#include "tensor_api/tensor/tensor_interface.h"
#include "tensor_api/experimental/arch/vector/reg_tensor.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.logical_compute
 * @brief Returns the element-wise bitwise complement of the source register under src.mask.
 *
 * @override_id te_experimental_operator_logical_not__const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in] src Source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator!(const reg_tensor<T>& src);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.logical_compute
 * @brief Returns element-wise bitwise AND results of the operands.
 *
 * @override_id te_experimental_operator_and__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator&(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.logical_compute
 * @brief Returns element-wise bitwise OR results of the operands.
 *
 * @override_id te_experimental_operator_or__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator|(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.logical_compute
 * @brief Returns element-wise bitwise XOR results of the operands.
 *
 * @override_id te_experimental_operator_xor__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator^(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup tensor_api.experimental.reg_compute.reg_logic
 * @override_id te_experimental_operator_shl__const_reg_tensor_T_R_const_reg_tensor_ShiftType_R__template_T_ShiftType
 * @archs 3510
 * @param [in] src Source operand.
 * @param [in] shift Shift.
 */
template <typename T, typename ShiftType>
__simd_callee__ inline reg_tensor<T> operator<<(const reg_tensor<T>& src, const reg_tensor<ShiftType>& shift);

/**
 * @ingroup tensor_api.experimental.reg_compute.reg_logic
 * @override_id te_experimental_operator_shl__const_reg_tensor_T_R_const_int16_t_R__template_T
 * @archs 3510
 * @param [in] src Source operand.
 * @param [in] shift Shift.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator<<(const reg_tensor<T>& src, const int16_t& shift);

/**
 * @ingroup tensor_api.experimental.reg_compute.reg_logic
 * @override_id te_experimental_operator_shr__const_reg_tensor_T_R_const_reg_tensor_ShiftType_R__template_T_ShiftType
 * @archs 3510
 * @param [in] src Source operand.
 * @param [in] shift Shift.
 */
template <typename T, typename ShiftType>
__simd_callee__ inline reg_tensor<T> operator>>(const reg_tensor<T>& src, const reg_tensor<ShiftType>& shift);

/**
 * @ingroup tensor_api.experimental.reg_compute.reg_logic
 * @override_id te_experimental_operator_shr__const_reg_tensor_T_R_const_int16_t_R__template_T
 * @archs 3510
 * @param [in] src Source operand.
 * @param [in] shift Shift.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> operator>>(const reg_tensor<T>& src, const int16_t& shift);

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/logical_compute_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_LOGICAL_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE_H
#endif
