/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the license.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/utils/reg_compute_traits.h is an internal header file and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_TRAITS
#endif

#ifndef IMPL_TENSOR_API_ARCH_UTILS_REG_COMPUTE_TRAITS_H
#define IMPL_TENSOR_API_ARCH_UTILS_REG_COMPUTE_TRAITS_H

#include "impl/tensor_api/experimental/reg/reg_tensor_impl.h"
#include "impl/tensor_api/utils/utils_impl.h"

namespace asc {
namespace te {
namespace detail {

template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

template <typename T>
struct is_reg_tensor : ::AscendC::Std::false_type {};

template <typename T>
struct is_reg_tensor<reg_tensor<T>> : ::AscendC::Std::true_type {};

template <typename T>
using enable_if_scalar_t =
    ::AscendC::Std::enable_if_t<!is_reg_tensor<::AscendC::Std::remove_cvref_t<T>>::value, int>;

// These capability sets are the intersection of the documented Ascend 950
// type matrices and the operations implemented by the current reg_tensor/C
// API backend. The legacy documentation additionally lists software-emulated
// int64_t/uint64_t/complex32/complex64 combinations for several APIs. Those
// combinations must stay rejected until their multi-register implementations
// are ported to the new reg_tensor representation.
template <typename T>
inline constexpr bool supports_abs_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, int16_t, half, int32_t, float>;

template <typename T>
inline constexpr bool supports_float_math_v = ::AscendC::Std::is_one_of_v<T, half, float>;

template <typename T>
inline constexpr bool supports_neg_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, int16_t, half, int32_t, float>;

template <typename T>
inline constexpr bool supports_relu_v = ::AscendC::Std::is_one_of_v<T, half, int32_t, float>;

template <typename T>
inline constexpr bool supports_add_sub_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_mul_v =
    ::AscendC::Std::is_one_of_v<T, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_mul_scalar_v =
    ::AscendC::Std::is_one_of_v<T, int16_t, uint16_t, half, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_div_v =
    ::AscendC::Std::is_one_of_v<T, int16_t, uint16_t, half, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_min_max_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_not_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t>;

template <typename T>
inline constexpr bool supports_bitwise_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t>;

template <typename T>
inline constexpr bool supports_shift_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t>;

template <typename T, typename ShiftT>
inline constexpr bool supports_shift_pair_v =
    (::AscendC::Std::is_one_of_v<T, int8_t, uint8_t> && ::AscendC::Std::is_same_v<ShiftT, int8_t>) ||
    (::AscendC::Std::is_one_of_v<T, int16_t, uint16_t> && ::AscendC::Std::is_same_v<ShiftT, int16_t>) ||
    (::AscendC::Std::is_one_of_v<T, int32_t, uint32_t> && ::AscendC::Std::is_same_v<ShiftT, int32_t>);

template <typename T>
inline constexpr bool supports_compare_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>;

} // namespace detail
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_UTILS_REG_COMPUTE_TRAITS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_TRAITS)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_TRAITS
#endif
