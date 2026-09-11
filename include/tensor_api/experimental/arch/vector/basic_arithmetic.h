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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_H

#include "tensor_api/tensor/tensor.h"
#include "tensor_api/experimental/arch/vector/reg_tensor.h"

namespace asc {
namespace te {
namespace experimental {

template <typename T>
__simd_callee__ inline reg_tensor<T> log(const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const T& scalar, const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const T& scalar, const reg_tensor<T>& src);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const T& scalar, const reg_tensor<T>& src);

template <typename T>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.basic_arithmetic
 * @override_id max__RCregtensorg1x2_pT_RCregtensorg1x2_pT
 * @archs 3510
 * @brief 该接口根据输入reg_tensor携带的mask，对两个源操作数按元素求最大值，并返回计算结果。支持reg_tensor与reg_tensor、reg_tensor与标量、标量与reg_tensor三种操作数组合。
 * @param [in] src0 reg_tensor与reg_tensor形式中的源操作数，类型为reg_tensor&lt;T&gt;。其中，src0.reg保存矢量数据，src0.mask用于控制各元素是否参与计算。src0.mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src1 reg_tensor与reg_tensor形式中的源操作数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。
 */
__simd_callee__ inline reg_tensor<T> max(const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.basic_arithmetic
 * @override_id max__RCregtensorg1x2_pT_RCpT
 * @archs 3510
 * @brief 该接口根据输入reg_tensor携带的mask，对两个源操作数按元素求最大值，并返回计算结果。支持reg_tensor与reg_tensor、reg_tensor与标量、标量与reg_tensor三种操作数组合。
 * @param [in] src reg_tensor与标量形式中的源操作数，类型为reg_tensor&lt;T&gt;。其中，src.reg保存矢量数据，src.mask用于控制各元素是否参与计算。src.mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] scalar 源操作数，类型为标量T。
 */
__simd_callee__ inline reg_tensor<T> max(const reg_tensor<T>& src, const T& scalar);

template <typename T>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.basic_arithmetic
 * @override_id max__RCpT_RCregtensorg1x2_pT
 * @archs 3510
 * @brief 该接口根据输入reg_tensor携带的mask，对两个源操作数按元素求最大值，并返回计算结果。支持reg_tensor与reg_tensor、reg_tensor与标量、标量与reg_tensor三种操作数组合。
 * @param [in] scalar 源操作数，类型为标量T。
 * @param [in] src reg_tensor与标量形式中的源操作数，类型为reg_tensor&lt;T&gt;。其中，src.reg保存矢量数据，src.mask用于控制各元素是否参与计算。src.mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline reg_tensor<T> max(const T& scalar, const reg_tensor<T>& src);

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/basic_arithmetic_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC
#endif
