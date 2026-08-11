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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_REG_TRUNCATE
#endif

#ifndef INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H
#define INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H

#include "impl/tensor_api/experimental/reg/reg_tensor_impl.h"

namespace asc {
namespace te {

/**
 * @brief Truncates each active element toward zero.
 * @tparam T Element type. Supported types are half, bfloat16_t, and float.
 * @param src Source register and predicate mask.
 * @return Truncated register with the source predicate mask.
 */
template <typename T>
__simd_callee__ inline reg_tensor<T> trunc(reg_tensor<T> src);

} // namespace te
} // namespace asc

#include "impl/tensor_api/arch/vector/experimental/truncate/reg_truncate.h"

#endif // INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_REG_TRUNCATE)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_REG_TRUNCATE
#endif
