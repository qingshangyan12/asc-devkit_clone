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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_REG_COMPOSITE_COMPUTE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_REG_COMPOSITE_COMPUTE_H

#include "impl/tensor_api/arch/utils/reg_compute_traits.h"

namespace asc {
namespace te {

template <typename T, typename scalar_type>
__simd_callee__ inline decltype(auto) axpy(reg_tensor<T> dst, reg_tensor<T> src, scalar_type scalar);

template <typename T>
__simd_callee__ inline decltype(auto) absdiff(reg_tensor<T> src0, reg_tensor<T> src1);

template <reg_layout layout = reg_layout::zero, typename T>
__simd_callee__ inline decltype(auto) exp_sub(reg_tensor<T> src0, reg_tensor<T> src1);

template <typename T>
__simd_callee__ inline decltype(auto) fma(
    reg_tensor<T> src0, reg_tensor<T> src1, reg_tensor<T> src2);

template <reg_layout layout = reg_layout::zero>
__simd_callee__ inline decltype(auto) muls_cast(reg_tensor<float> src, float scalar);

} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/arch/vector/experimental/composite_compute_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_REG_COMPOSITE_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE
#endif
