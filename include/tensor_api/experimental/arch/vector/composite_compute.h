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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPOSITE_COMPUTE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPOSITE_COMPUTE_H

#include "tensor_api/experimental/utils/reg_enum.h"
#include "tensor_api/experimental/arch/vector/reg_tensor.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Updates dst with scalar times src plus dst under dst.mask and returns the updated register tensor.
 *
 * @override_id te_experimental_axpy__reg_tensor_T_R_const_reg_tensor_T_R_const_T_R__template_T
 * @archs 3510
 * @param [in,out] dst Destination tensor.
 * @param [in] src Source operand.
 * @param [in] scalar Scalar operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> axpy(reg_tensor<T>& dst, const reg_tensor<T>& src, const T& scalar);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Returns element-wise absolute differences between src0 and src1 under src0.mask.
 *
 * @override_id te_experimental_abs_diff__const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> abs_diff(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Returns element-wise exponentials of the differences between src0 and src1.
 *
 * @override_id te_experimental_exp_diff__const_reg_tensor_U_R_const_reg_tensor_U_R__template_T_U
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T, typename U>
__simd_callee__ inline reg_tensor<T> exp_diff(const reg_tensor<U>& src0, const reg_tensor<U>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Returns element-wise exponentials of the differences between src0 and src1.
 *
 * @override_id
 * te_experimental_exp_diff__const_reg_tensor_U_R_const_reg_tensor_U_R_PositionType__template_T_U_PositionType
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 * @param [in] src_pos Source operand.
 */
template <typename T, typename U, typename PositionType>
__simd_callee__ inline reg_tensor<T> exp_diff(
    const reg_tensor<U>& src0, const reg_tensor<U>& src1, PositionType src_pos);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Updates dst with dst times src0 plus src1 under dst.mask and returns the updated register tensor.
 *
 * @override_id te_experimental_madd__reg_tensor_T_R_const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in,out] dst Destination tensor.
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> madd(reg_tensor<T>& dst, const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Adds src0 times src1 to dst element-wise under dst.mask and returns the updated register tensor.
 *
 * @override_id te_experimental_mula__reg_tensor_T_R_const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in,out] dst Destination tensor.
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> mula(reg_tensor<T>& dst, const reg_tensor<T>& src0, const reg_tensor<T>& src1);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Returns element-wise fused multiply-add results of the three source operands.
 *
 * @override_id te_experimental_fma__const_reg_tensor_T_R_const_reg_tensor_T_R_const_reg_tensor_T_R__template_T
 * @archs 3510
 * @param [in] src0 First source operand.
 * @param [in] src1 Second source operand.
 * @param [in] src2 Source operand.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> fma(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<T>& src2);

/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.composite_compute
 * @brief Returns register values multiplied by scalar and converted to the destination element type.
 *
 * @override_id te_experimental_muls_cast__const_reg_tensor_U_R_const_U_R__template_Layout_T_U
 * @archs 3510
 * @param [in] src Source operand.
 * @param [in] scalar Scalar operand.
 */
template <cast_layout Layout = cast_layout::zero, typename T, typename U>
__simd_callee__ inline reg_tensor<T> muls_cast(const reg_tensor<U>& src, const U& scalar);

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/composite_compute_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPOSITE_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_H
#endif
