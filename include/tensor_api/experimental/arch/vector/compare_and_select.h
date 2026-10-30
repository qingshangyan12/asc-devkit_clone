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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPARE_AND_SELECT_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPARE_AND_SELECT_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPARE_AND_SELECT_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"

namespace asc {
namespace te {
namespace experimental {
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares two register tensors for element-wise equality.
 *
 * @override_id te_experimental_operator_eq__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the equality results.
 * @param [in] src0 First source tensor.
 * @param [in] src1 Second source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator==(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares register tensor elements with a scalar for equality.
 *
 * @override_id te_experimental_operator_eq__const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the equality results.
 * @param [in] src0 Source tensor.
 * @param [in] src1 Scalar value to compare with each tensor element.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator==(const reg_tensor<T>& src0, const T& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares a scalar with register tensor elements for equality.
 *
 * @override_id te_experimental_operator_eq__const_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the equality results.
 * @param [in] src0 Scalar value to compare with each tensor element.
 * @param [in] src1 Source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator==(const T& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares two register tensors for element-wise inequality.
 *
 * @override_id te_experimental_operator_ne__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the inequality results.
 * @param [in] src0 First source tensor.
 * @param [in] src1 Second source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator!=(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares register tensor elements with a scalar for inequality.
 *
 * @override_id te_experimental_operator_ne__const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the inequality results.
 * @param [in] src0 Source tensor.
 * @param [in] src1 Scalar value to compare with each tensor element.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator!=(const reg_tensor<T>& src0, const T& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares a scalar with register tensor elements for inequality.
 *
 * @override_id te_experimental_operator_ne__const_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the inequality results.
 * @param [in] src0 Scalar value to compare with each tensor element.
 * @param [in] src1 Source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator!=(const T& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether elements in one register tensor are less than those in another.
 *
 * @override_id te_experimental_operator_lt__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the less-than results.
 * @param [in] src0 First source tensor.
 * @param [in] src1 Second source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether register tensor elements are less than a scalar.
 *
 * @override_id te_experimental_operator_lt__const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the less-than results.
 * @param [in] src0 Source tensor.
 * @param [in] src1 Scalar value to compare with each tensor element.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<(const reg_tensor<T>& src0, const T& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether a scalar is less than register tensor elements.
 *
 * @override_id te_experimental_operator_lt__const_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the less-than results.
 * @param [in] src0 Scalar value to compare with each tensor element.
 * @param [in] src1 Source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<(const T& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether elements in one register tensor are less than or equal to those in another.
 *
 * @override_id te_experimental_operator_le__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the less-than-or-equal results.
 * @param [in] src0 First source tensor.
 * @param [in] src1 Second source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<=(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether register tensor elements are less than or equal to a scalar.
 *
 * @override_id te_experimental_operator_le__const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the less-than-or-equal results.
 * @param [in] src0 Source tensor.
 * @param [in] src1 Scalar value to compare with each tensor element.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<=(const reg_tensor<T>& src0, const T& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether a scalar is less than or equal to register tensor elements.
 *
 * @override_id te_experimental_operator_le__const_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the less-than-or-equal results.
 * @param [in] src0 Scalar value to compare with each tensor element.
 * @param [in] src1 Source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<=(const T& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether elements in one register tensor are greater than those in another.
 *
 * @override_id te_experimental_operator_gt__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the greater-than results.
 * @param [in] src0 First source tensor.
 * @param [in] src1 Second source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether register tensor elements are greater than a scalar.
 *
 * @override_id te_experimental_operator_gt__const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the greater-than results.
 * @param [in] src0 Source tensor.
 * @param [in] src1 Scalar value to compare with each tensor element.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>(const reg_tensor<T>& src0, const T& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether a scalar is greater than register tensor elements.
 *
 * @override_id te_experimental_operator_gt__const_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the greater-than results.
 * @param [in] src0 Scalar value to compare with each tensor element.
 * @param [in] src1 Source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>(const T& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether elements in one register tensor are greater than or equal to those in another.
 *
 * @override_id te_experimental_operator_ge__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the greater-than-or-equal results.
 * @param [in] src0 First source tensor.
 * @param [in] src1 Second source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>=(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether register tensor elements are greater than or equal to a scalar.
 *
 * @override_id te_experimental_operator_ge__const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the greater-than-or-equal results.
 * @param [in] src0 Source tensor.
 * @param [in] src1 Scalar value to compare with each tensor element.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>=(const reg_tensor<T>& src0, const T& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Compares whether a scalar is greater than or equal to register tensor elements.
 *
 * @override_id te_experimental_operator_ge__const_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A boolean register tensor containing the greater-than-or-equal results.
 * @param [in] src0 Scalar value to compare with each tensor element.
 * @param [in] src1 Source tensor.
 */
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>=(const T& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.compare_and_select
 * @brief Selects elements from two register tensors according to a condition tensor.
 *
 * @override_id te_experimental_select__const_reg_tensor_bool_R_const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @return A register tensor containing the selected elements.
 * @param [in] condition Per-element selection condition.
 * @param [in] src0 Source tensor selected when the corresponding condition is true.
 * @param [in] src1 Source tensor selected when the corresponding condition is false.
 * @note A condition value of 1 selects the corresponding element from src0; a value of 0 selects it from src1. This
 * condition does not zero unselected elements. The result preserves src0.mask.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> select(
    const reg_tensor<bool>& condition, const reg_tensor<T>& src0, const reg_tensor<T>& src1);
} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/compare_and_select_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPARE_AND_SELECT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPARE_AND_SELECT_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPARE_AND_SELECT_H
#endif
