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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_TYPE_CONVERSION
#endif

#ifndef INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TYPE_CONVERSION_H
#define INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TYPE_CONVERSION_H

#include "impl/tensor_api/experimental/reg/reg_tensor_impl.h"

namespace asc {
namespace te {

/** @brief Data layout selected by a width-changing register cast. */
enum class cast_layout {
    unknow = -1,
    zero,
    one,
    two,
    three,
};

/** @brief Rounding behavior for register cast. */
enum class cast_round_mode {
    unknow = -1,
    rint,
    round,
    floor,
    ceil,
    trunc,
};

/** @brief Saturation behavior for register cast. */
enum class cast_sat_mode {
    unknow = -1,
    not_sat,
    sat,
};

struct cast_options {
    cast_layout layout = cast_layout::zero;
    cast_round_mode round = cast_round_mode::rint;
    cast_sat_mode sat = cast_sat_mode::not_sat;
};

/**
 * @brief Converts a register vector from From to To using a hardware Cast instruction.
 *
 * The source mask must be created in the predicate domain of the wider element
 * type. Width-changing casts preserve that mask and use options.layout to select
 * the physical lane part. Callers must pair widening casts with unpacked loads
 * or narrowing casts with packed stores as required by the selected data path.
 */
#if __cplusplus >= 202002L
template <typename To, cast_options Options = cast_options {}, typename From>
__simd_callee__ inline reg_tensor<To> cast(reg_tensor<From> src);
#else
// C++17 cannot use cast_options as a non-type template parameter.
template <typename To, cast_layout Layout = cast_layout::zero, cast_round_mode Round = cast_round_mode::rint,
    cast_sat_mode Sat = cast_sat_mode::not_sat, typename From>
__simd_callee__ inline reg_tensor<To> cast(reg_tensor<From> src);
#endif

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

#include "impl/tensor_api/arch/vector/experimental/type_conversion_impl.h"

#endif // INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_TYPE_CONVERSION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_TYPE_CONVERSION)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_TYPE_CONVERSION
#endif
