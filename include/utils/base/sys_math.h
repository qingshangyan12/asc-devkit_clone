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
 * \file sys_math.h
 * \brief
 */
#ifndef INCLUDE_UTILS_BASE_SYS_MATH_H
#define INCLUDE_UTILS_BASE_SYS_MATH_H

#include "impl/utils/std/algorithm/min.h"

namespace AscendC {
namespace detail {

template <typename T>
struct CeilDivIntegerTraits {
    static constexpr bool is_integral = false;
    static constexpr bool is_signed = false;
    using unsigned_type = T;
};

template <>
struct CeilDivIntegerTraits<bool> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = bool;
};

template <>
struct CeilDivIntegerTraits<char> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = char;
};

template <>
struct CeilDivIntegerTraits<signed char> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = true;
    using unsigned_type = unsigned char;
};

template <>
struct CeilDivIntegerTraits<unsigned char> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = unsigned char;
};

template <>
struct CeilDivIntegerTraits<short> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = true;
    using unsigned_type = unsigned short;
};

template <>
struct CeilDivIntegerTraits<unsigned short> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = unsigned short;
};

template <>
struct CeilDivIntegerTraits<int> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = true;
    using unsigned_type = unsigned int;
};

template <>
struct CeilDivIntegerTraits<unsigned int> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = unsigned int;
};

template <>
struct CeilDivIntegerTraits<long> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = true;
    using unsigned_type = unsigned long;
};

template <>
struct CeilDivIntegerTraits<unsigned long> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = unsigned long;
};

template <>
struct CeilDivIntegerTraits<long long> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = true;
    using unsigned_type = unsigned long long;
};

template <>
struct CeilDivIntegerTraits<unsigned long long> {
    static constexpr bool is_integral = true;
    static constexpr bool is_signed = false;
    using unsigned_type = unsigned long long;
};

template <typename T>
struct CeilDivIntegerTraits<const T> : CeilDivIntegerTraits<T> {};

template <typename T>
struct CeilDivIntegerTraits<volatile T> : CeilDivIntegerTraits<T> {};

template <typename T>
struct CeilDivIntegerTraits<const volatile T> : CeilDivIntegerTraits<T> {};

template <typename T, typename U>
__host__ __aicore__ inline constexpr auto CeilDivGeneric(T a, U b)
{
    static_assert(CeilDivIntegerTraits<T>::is_integral, "AscendC::CeilDiv: T must be an integral type");
    static_assert(CeilDivIntegerTraits<U>::is_integral, "AscendC::CeilDiv: U must be an integral type");
    using Prom = decltype(T{} / U{});
    using UProm = typename CeilDivIntegerTraits<Prom>::unsigned_type;

    const auto a1 = static_cast<UProm>(a);
    const auto b1 = static_cast<UProm>(b);
    if constexpr (CeilDivIntegerTraits<Prom>::is_signed) {
        return static_cast<Prom>((a1 + b1 - 1) / b1);
    } else {
        return Std::min(a1, static_cast<UProm>(1) + ((a1 - 1) / b1));
    }
}

} // namespace detail

template <typename T, typename U>
__host__ __aicore__ inline constexpr auto CeilDiv(T a, U b)
{
    using ReturnType = decltype(detail::CeilDivGeneric(a, b));
    if (b == 0) {
        return ReturnType(0);
    }
    return detail::CeilDivGeneric(a, b);
}

} // namespace AscendC

#endif // INCLUDE_UTILS_BASE_SYS_MATH_H
