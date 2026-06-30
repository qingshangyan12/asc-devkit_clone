/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file integral_constant.h
 * \brief
 */

#include "enable_if.h" 
#include "is_const.h" 
#include "../algorithm/max.h" 
#include "../algorithm/min.h" 
#include "../cmath/divide.h" 
#include "../cmath/ceil_division.h" 
#include "../cmath/ceil_align.h" 
#include "../cmath/sqrt.h" 
#include "is_integral.h"

#ifndef IMPL_STD_ASCENDC_STD_INTEGRAL_CONSTANT_IMPL__H
#define IMPL_STD_ASCENDC_STD_INTEGRAL_CONSTANT_IMPL__H

namespace AscendC {
namespace Std {

template <typename Tp, Tp v>
struct integral_constant
{
    static constexpr const Tp value = v;

    using value_type = Tp;
    using type = integral_constant;

    ASCENDC_HOST_AICORE inline constexpr operator value_type() const noexcept {
        return value;
    }

    ASCENDC_HOST_AICORE inline constexpr value_type operator()() const noexcept {
        return value;
    }
};

template <typename Tp, Tp v>
constexpr const Tp integral_constant<Tp, v>::value;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <bool b>
using bool_constant = integral_constant<bool, b>;

template <size_t v>
using Int = integral_constant<size_t, v>;

#define STD_INT_BINARY_OP(OP) \
template <auto t, auto u> \
__aicore__ inline constexpr Int<(t OP u)> operator OP (Int<t>, Int<u>) { \
    return {}; \
}

STD_INT_BINARY_OP(+);
STD_INT_BINARY_OP(-);
STD_INT_BINARY_OP(*);
STD_INT_BINARY_OP(/);
STD_INT_BINARY_OP(%);
#undef STD_INT_BINARY_OP

#define __CANN_ASC_STD_INT_NAMED_BINARY_FN(OP, CMP) \
template <auto t, auto u> \
ASCENDC_HOST_AICORE inline constexpr Int<(CMP ? t : u)> OP(Int<t>, Int<u>) { \
    return {}; \
} \
template <auto t, typename U, typename = enable_if_t<is_integral<U>::value>> \
ASCENDC_HOST_AICORE inline constexpr auto OP(Int<t>, const U& u) { \
    return OP(static_cast<size_t>(t), u); \
} \
template <typename T, auto u, typename = enable_if_t<is_integral<T>::value>> \
ASCENDC_HOST_AICORE inline constexpr auto OP(const T& t, Int<u>) { \
    return OP(t, static_cast<size_t>(u)); \
}

__CANN_ASC_STD_INT_NAMED_BINARY_FN(max, t > u);
__CANN_ASC_STD_INT_NAMED_BINARY_FN(min, t < u);
#undef __CANN_ASC_STD_INT_NAMED_BINARY_FN

#define __CANN_ASC_STD_INT_BINARY_FN(OP, EXPR) \
template <auto t, auto u> \
ASCENDC_HOST_AICORE inline constexpr Int<EXPR> OP(Int<t>, Int<u>) { \
    return {}; \
} \
template <auto t, typename U, typename = enable_if_t<is_integral<U>::value>> \
ASCENDC_HOST_AICORE inline constexpr auto OP(Int<t>, const U& u) { \
    return OP(static_cast<size_t>(t), u); \
} \
template <typename T, auto u, typename = enable_if_t<is_integral<T>::value>> \
ASCENDC_HOST_AICORE inline constexpr auto OP(const T& t, Int<u>) { \
    return OP(t, static_cast<size_t>(u)); \
}

__CANN_ASC_STD_INT_BINARY_FN(divide, (t / u));
__CANN_ASC_STD_INT_BINARY_FN(ceil_division, ((t + u - 1) / u));
__CANN_ASC_STD_INT_BINARY_FN(ceil_align, (((t + u - 1) / u) * u));
#undef __CANN_ASC_STD_INT_BINARY_FN
}
}

#endif // IMPL_STD_ASCENDC_STD_INTEGRAL_CONSTANT_IMPL__H
