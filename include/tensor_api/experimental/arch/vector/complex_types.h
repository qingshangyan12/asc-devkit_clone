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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPLEX_TYPES_H
#endif

/*!
 * \file complex_types.h
 * \brief Complex register tags, scalar value type, and complex<T> alias.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPLEX_TYPES_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPLEX_TYPES_H

#include "utils/std/type_traits.h"

namespace asc {
namespace te {
namespace experimental {

/** @brief Tag type for half-precision complex register tensors (complex32). */
struct complex_half {};

/** @brief Tag type for single-precision complex register tensors (complex64). */
struct complex_float {};

/**
 * @brief Host/device scalar complex value used by Duplicate / Adds / Muls.
 * @tparam Elem Underlying real/imag element type (@c half or @c float).
 */
template <typename Elem>
struct complex_scalar {
    using elem_type = Elem;

    __simd_callee__ complex_scalar() : real(), imag() {}

    __simd_callee__ complex_scalar(Elem re, Elem im = Elem()) : real(re), imag(im) {}

    Elem real;
    Elem imag;
};

/**
 * @brief Maps a real element type to its complex register-tensor type.
 * @tparam T @c half or @c float.
 */
template <typename T>
struct complex_map;

template <>
struct complex_map<half> {
    using type = complex_half;
};

template <>
struct complex_map<float> {
    using type = complex_float;
};

/**
 * @brief Element type carried by a complex tag.
 * @tparam Tag @c complex_half or @c complex_float.
 */
template <typename Tag>
struct complex_elem;

template <>
struct complex_elem<complex_half> {
    using type = half;
};

template <>
struct complex_elem<complex_float> {
    using type = float;
};

template <typename Tag>
using complex_elem_t = typename complex_elem<Tag>::type;

template <typename T>
inline constexpr bool is_complex_tag_v =
    AscendC::Std::is_same_v<T, complex_half> || AscendC::Std::is_same_v<T, complex_float>;

} // namespace experimental
} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPLEX_TYPES_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPLEX_TYPES_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPLEX_TYPES_H
#endif
