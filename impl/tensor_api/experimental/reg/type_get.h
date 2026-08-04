/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/experimental/reg/type_get.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_TYPE_GET
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_REG_TYPE_GET_H
#define IMPL_TENSOR_API_EXPERIMENTAL_REG_TYPE_GET_H

#include <cstdint>

#include "c_api/asc_simd.h"

namespace AscendC {
namespace Te {

// Keep the primary template incomplete so unsupported element types fail at
// compile time when reg_tensor<T> requests the corresponding register type.
template <typename DataType>
struct type_get;

template <>
struct type_get<bool> {
    using T = vector_bool;
};

template <>
struct type_get<uint8_t> {
    using T = vector_uint8_t;
};

template <>
struct type_get<uint16_t> {
    using T = vector_uint16_t;
};

template <>
struct type_get<uint32_t> {
    using T = vector_uint32_t;
};

template <>
struct type_get<uint64_t> {
    using T = vector_uint64_t;
};

template <>
struct type_get<int8_t> {
    using T = vector_int8_t;
};

template <>
struct type_get<int16_t> {
    using T = vector_int16_t;
};

template <>
struct type_get<int32_t> {
    using T = vector_int32_t;
};

template <>
struct type_get<int64_t> {
    using T = vector_int64_t;
};

template <>
struct type_get<half> {
    using T = vector_half;
};

template <>
struct type_get<bfloat16_t> {
    using T = vector_bfloat16_t;
};

template <>
struct type_get<float> {
    using T = vector_float;
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_EXPERIMENTAL_REG_TYPE_GET_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_TYPE_GET)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_TYPE_GET
#endif
