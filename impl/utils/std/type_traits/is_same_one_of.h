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
 * \file is_same_one_of.h
 * \brief
 */
#ifndef IMPL_STD_ASCENDC_STD_IS_SAME_ONE_OF_IMPL__H
#define IMPL_STD_ASCENDC_STD_IS_SAME_ONE_OF_IMPL__H

#include "impl/utils/std/type_traits/integral_constant.h"
#include "impl/utils/std/type_traits/conditional.h"

namespace AscendC {
namespace Std {
template <typename T, T... Value> struct is_same_one_of : public Std::false_type {};

template <typename T, T Value, T First, T... Rest>
struct is_same_one_of<T, Value, First, Rest...>
    : Std::conditional_t<Value == First, Std::true_type, is_same_one_of<T, Value, Rest...>> {};

template <typename T, T... Value> inline constexpr bool is_same_one_of_v = is_same_one_of<T, Value...>::value;
}
}

#endif // IMPL_STD_ASCENDC_STD_IS_ONE_OF_IMPL__H
