/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_COMPUTE_TRAITS_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_COMPUTE_TRAITS_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/utils/utils.h"
#include "utils/std/type_traits.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

template <typename T>
struct reg_tensor_traits;

template <typename T>
struct reg_tensor_traits<reg_tensor<T>> {
    using elem_type = T;
};

template <typename T>
using reg_elem_t = typename reg_tensor_traits<::AscendC::Std::remove_cvref_t<T>>::elem_type;

template <typename T, typename U, bool = is_reg_tensor_v<T>>
struct binary_elem_type {
    static_assert(is_reg_tensor_v<U>, "at least one operand must be reg_tensor");
    using type = reg_elem_t<U>;
};

template <typename T, typename U>
struct binary_elem_type<T, U, true> {
    using type = reg_elem_t<T>;
};

template <typename T, typename U>
using binary_elem_t = typename binary_elem_type<T, U>::type;

template <typename T, typename U>
__simd_callee__ inline reg_tensor<T> make_reg_operand(const U& value, const vector_bool& mask)
{
    if constexpr (is_reg_tensor_v<U>) {
        return value;
    } else {
        reg_tensor<T> dst;
        asc_duplicate_scalar(dst.reg, static_cast<T>(value), mask);
        dst.mask = mask;
        return dst;
    }
}

template <typename T, typename U>
__simd_callee__ inline vector_bool binary_mask(const T& a, const U& b)
{
    static_assert(is_reg_tensor_v<T> || is_reg_tensor_v<U>, "at least one operand must be reg_tensor");
    if constexpr (is_reg_tensor_v<T>) {
        return a.mask;
    } else {
        return b.mask;
    }
}

template <typename T, typename U>
__simd_callee__ inline void check_binary_reg_types()
{
    if constexpr (is_reg_tensor_v<T> && is_reg_tensor_v<U>) {
        static_assert(
            ::AscendC::Std::is_same_v<reg_elem_t<T>, reg_elem_t<U>>,
            "the reg_tensor operands must have the same element type");
    }
}

template <typename T>
inline constexpr bool supports_float_math_v = ::AscendC::Std::is_one_of_v<T, half, float>;

template <typename T>
inline constexpr bool supports_add_sub_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_mul_scalar_v =
    ::AscendC::Std::is_one_of_v<T, int16_t, uint16_t, half, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_min_max_v =
    ::AscendC::Std::is_one_of_v<T, int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>;

template <typename T>
inline constexpr bool supports_or_v =
    ::AscendC::Std::is_one_of_v<T, bool, int8_t, uint8_t, int16_t, uint16_t, half, int32_t, uint32_t, float>;

} // namespace detail
} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_COMPUTE_TRAITS_H
