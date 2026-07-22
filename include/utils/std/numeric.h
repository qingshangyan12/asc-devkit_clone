/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file numeric.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "numeric.h cannot be used with compile flag --enable-simt enabled."
#endif

#ifndef AICORE_UTILS_STD_NUMERIC_H
#define AICORE_UTILS_STD_NUMERIC_H

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include "stub_def.h"
#endif

#include "algorithm.h"
#include "type_traits.h"

namespace AscendC {
namespace Std {

namespace numeric_detail {

// detect signed integral types: partial specialization for known signed types
template <typename T>
struct is_signed_impl : false_type {};
template <>
struct is_signed_impl<signed char> : true_type {};
template <>
struct is_signed_impl<short> : true_type {};
template <>
struct is_signed_impl<int> : true_type {};
template <>
struct is_signed_impl<long> : true_type {};
template <>
struct is_signed_impl<long long> : true_type {};

template <typename T>
static constexpr bool is_signed_impl_v = is_signed_impl<T>::value;

// make_unsigned mapping from signed to unsigned, default keep-as-is for unsigned
template <typename T>
struct make_unsigned_impl {
    using type = T;
};
template <>
struct make_unsigned_impl<signed char> {
    using type = unsigned char;
};
template <>
struct make_unsigned_impl<short> {
    using type = unsigned short;
};
template <>
struct make_unsigned_impl<int> {
    using type = unsigned int;
};
template <>
struct make_unsigned_impl<long> {
    using type = unsigned long;
};
template <>
struct make_unsigned_impl<long long> {
    using type = unsigned long long;
};

template <typename T>
using make_unsigned_t = typename make_unsigned_impl<T>::type;

} // namespace numeric_detail

// ceil_div: divides a by b, rounding up if there is a remainder
// Precondition: a >= 0 (for signed types), b > 0
template <typename T, typename U>
__host__ __aicore__ inline constexpr auto ceil_div(const T& a, const U& b)
{
    static_assert(is_integral_v<T>, "AscendC::Std::ceil_div: T must be an integral type");
    static_assert(is_integral_v<U>, "AscendC::Std::ceil_div: U must be an integral type");

    using Prom  = decltype(T{} / U{});
    using UProm = numeric_detail::make_unsigned_t<Prom>;

    auto a1 = static_cast<UProm>(a);
    auto b1 = static_cast<UProm>(b);

    // When _Prom is signed, (a + b - 1) / b is safe because:
    // a <= INT_MAX, b <= INT_MAX, INT_MAX + INT_MAX fits in unsigned
    if constexpr (numeric_detail::is_signed_impl_v<Prom>)
    {
        return (a1 + b1 - 1) / b1;
    }
    else
    {
        // When _Prom is unsigned, a could be near UINT_MAX, so (a + b - 1) could overflow/wrap.
        // Use min-based branchless form: min(a, 1 + (a - 1) / b) avoids overflow risk.
        return min(a1, static_cast<UProm>(1) + ((a1 - 1) / b1));
    }
}

} // namespace Std
} // namespace AscendC
#endif  // AICORE_UTILS_STD_NUMERIC_H
