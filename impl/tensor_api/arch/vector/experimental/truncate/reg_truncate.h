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
#warning                                                                                                               \
    "reg_truncate.h is internal; include <tensor_api/arch/vector/experimental/truncate/reg_truncate.h> instead."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_TRUNCATE
#endif

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H

#if !defined(INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H)
#include "tensor_api/arch/vector/experimental/truncate/reg_truncate.h"
#endif
#include "c_api/reg_compute/reg_convert.h"

namespace asc {
namespace te {
namespace detail {

template <typename T>
struct is_supported {
    static constexpr bool value = false;
};

template <>
struct is_supported<half> {
    static constexpr bool value = true;
};

template <>
struct is_supported<bfloat16_t> {
    static constexpr bool value = true;
};

template <>
struct is_supported<float> {
    static constexpr bool value = true;
};

} // namespace detail

template <typename T>
__simd_callee__ inline reg_tensor<T> trunc(reg_tensor<T> src)
{
    if constexpr (detail::is_supported<T>::value) {
        reg_tensor<T> dst;
        asc_trunc(dst.reg, src.reg, src.mask);
        dst.mask = src.mask;
        return dst;
    } else {
        static_assert(detail::is_supported<T>::value,
            "reg_tensor trunc supports only half, bfloat16_t, and float");
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TRUNCATE_REG_TRUNCATE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_TRUNCATE)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_TRUNCATE
#endif
