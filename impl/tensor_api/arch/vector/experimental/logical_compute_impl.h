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
    "impl/tensor_api/arch/vector/experimental/logical_compute_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE_IMPL
#endif

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_LOGICAL_COMPUTE_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_LOGICAL_COMPUTE_IMPL_H

#include "impl/tensor_api/arch/utils/reg_compute_traits.h"

namespace asc {
namespace te {

namespace detail {

enum class bitwise_op {
    bit_and,
    bit_or,
    bit_xor,
};

__simd_callee__ inline vector_bool full_predicate_mask()
{
    return asc_create_mask_b8(PAT_ALL);
}

template <typename T, typename U>
__simd_callee__ inline reg_tensor<T> make_logic_operand(U value, vector_bool mask)
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
__simd_callee__ inline vector_bool logic_mask(T a, U b)
{
    static_assert(is_reg_tensor_v<T> || is_reg_tensor_v<U>, "at least one operand must be reg_tensor");
    if constexpr (is_reg_tensor_v<T>) {
        return a.mask;
    } else {
        return b.mask;
    }
}

template <typename T, typename U>
__simd_callee__ inline void check_logic_reg_types()
{
    if constexpr (is_reg_tensor_v<T> && is_reg_tensor_v<U>) {
        static_assert(Std::is_same_v<reg_elem_t<T>, reg_elem_t<U>>,
            "the reg_tensor operands must have the same element type");
    }
}

template <bitwise_op op, typename T>
__simd_callee__ inline void check_bitwise_type()
{
    if constexpr (op == bitwise_op::bit_and) {
        static_assert(supports_and_v<T>, "operator& does not support this element type");
    } else if constexpr (op == bitwise_op::bit_or) {
        static_assert(supports_or_v<T>, "operator| does not support this element type");
    } else {
        static_assert(supports_xor_v<T>, "operator^ does not support this element type");
    }
}

template <bitwise_op op, typename T, typename U>
__simd_callee__ inline decltype(auto) bitwise_compute(T a, U b)
{
    using elem_type = binary_elem_t<T, U>;
    check_logic_reg_types<T, U>();
    check_bitwise_type<op, elem_type>();

    if constexpr (Std::is_same_v<elem_type, bool>) {
        static_assert(is_reg_tensor_v<T> && is_reg_tensor_v<U>,
            "bool bitwise operators require two reg_tensor<bool> operands");
        reg_tensor<bool> dst;
        vector_bool mask = full_predicate_mask();
        if constexpr (op == bitwise_op::bit_and) {
            asc_and(dst.reg, a.reg, b.reg, mask);
        } else if constexpr (op == bitwise_op::bit_or) {
            asc_or(dst.reg, a.reg, b.reg, mask);
        } else {
            asc_xor(dst.reg, a.reg, b.reg, mask);
        }
        return dst;
    } else {
        vector_bool mask = logic_mask(a, b);
        auto left = make_logic_operand<elem_type>(a, mask);
        auto right = make_logic_operand<elem_type>(b, mask);
        reg_tensor<elem_type> dst;
        if constexpr (op == bitwise_op::bit_and) {
            asc_and(dst.reg, left.reg, right.reg, mask);
        } else if constexpr (op == bitwise_op::bit_or) {
            asc_or(dst.reg, left.reg, right.reg, mask);
        } else {
            asc_xor(dst.reg, left.reg, right.reg, mask);
        }
        dst.mask = mask;
        return dst;
    }
}

} // namespace detail

template <typename T>
__simd_callee__ inline decltype(auto) operator!(reg_tensor<T> src)
{
    static_assert(detail::supports_not_v<T>, "operator! does not support this element type");

    if constexpr (Std::is_same_v<T, bool>) {
        reg_tensor<bool> dst;
        asc_not(dst.reg, src.reg, detail::full_predicate_mask());
        return dst;
    } else {
        reg_tensor<T> dst;
        vector_bool is_zero;
        asc_eq_scalar(is_zero, src.reg, static_cast<T>(0), src.mask);
        asc_duplicate_scalar(dst.reg, static_cast<T>(0), src.mask);
        asc_duplicate_scalar(dst.reg, static_cast<T>(1), is_zero);
        dst.mask = src.mask;
        return dst;
    }
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator&(T a, U b)
{
    return detail::bitwise_compute<detail::bitwise_op::bit_and>(a, b);
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator|(T a, U b)
{
    return detail::bitwise_compute<detail::bitwise_op::bit_or>(a, b);
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator^(T a, U b)
{
    return detail::bitwise_compute<detail::bitwise_op::bit_xor>(a, b);
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator<<(reg_tensor<T> src, U shift)
{
    static_assert(detail::supports_shift_v<T>, "operator<< does not support this element type");

    reg_tensor<T> dst;
    if constexpr (detail::is_reg_tensor_v<U>) {
        using shift_type = detail::reg_elem_t<U>;
        static_assert(detail::supports_shift_pair_v<T, shift_type>,
            "register operator<< requires a signed shift register of the same width");
        asc_shiftleft(dst.reg, src.reg, shift.reg, src.mask);
    } else {
        static_assert(Std::is_same_v<Std::remove_cvref_t<U>, int16_t>,
            "scalar operator<< requires an int16_t shift value");
        asc_shiftleft_scalar(dst.reg, src.reg, shift, src.mask);
    }
    dst.mask = src.mask;
    return dst;
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator>>(reg_tensor<T> src, U shift)
{
    static_assert(detail::supports_shift_v<T>, "operator>> does not support this element type");

    reg_tensor<T> dst;
    if constexpr (detail::is_reg_tensor_v<U>) {
        using shift_type = detail::reg_elem_t<U>;
        static_assert(detail::supports_shift_pair_v<T, shift_type>,
            "register operator>> requires a signed shift register of the same width");
        asc_shiftright(dst.reg, src.reg, shift.reg, src.mask);
    } else {
        static_assert(Std::is_same_v<Std::remove_cvref_t<U>, int16_t>,
            "scalar operator>> requires an int16_t shift value");
        asc_shiftright_scalar(dst.reg, src.reg, shift, src.mask);
    }
    dst.mask = src.mask;
    return dst;
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_LOGICAL_COMPUTE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LOGICAL_COMPUTE_IMPL
#endif
