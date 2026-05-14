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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_PRINT_H
#endif

/*!
 * \file print.h
 * \brief Tensor API debug print helpers.
 */
#ifndef INCLUDE_TENSOR_API_UTILS_PRINT_H
#define INCLUDE_TENSOR_API_UTILS_PRINT_H

#include "include/tensor_api/utils/utils.h"
#include "include/utils/std/type_traits.h"

namespace AscendC {
namespace Te {
namespace detail {

template <typename T>
struct IsPrintCharPointer : Std::false_type {};

#ifdef ASCENDC_CPU_DEBUG
template <>
struct IsPrintCharPointer<char*> : Std::true_type {};

template <>
struct IsPrintCharPointer<const char*> : Std::true_type {};
#else
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
template <>
struct IsPrintCharPointer<__gm__ char*> : Std::true_type {};

template <>
struct IsPrintCharPointer<__gm__ const char*> : Std::true_type {};
#pragma clang diagnostic pop
#endif

template <typename T>
constexpr bool IsPrintStringV = IsPrintCharPointer<Std::decay_t<T>>::value;

} // namespace detail

#ifdef ASCENDC_CPU_DEBUG
__aicore__ inline void Print(const char* fmt);

template <typename Arg0, typename... Args>
__aicore__ inline void Print(const char* fmt, Arg0&& arg0, Args&&... args);

#else
__aicore__ inline void Print(__gm__ const char* fmt);

template <typename Arg0, typename... Args>
__aicore__ inline void Print(__gm__ const char* fmt, Arg0&& arg0, Args&&... args);
#endif

template <typename T, typename Std::enable_if_t<!detail::IsPrintStringV<T>, int> = 0>
__aicore__ inline void Print(const T& value);

} // namespace Te
} // namespace AscendC

#include "impl/tensor_api/utils/print_impl.h"

#endif // INCLUDE_TENSOR_API_UTILS_PRINT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_PRINT_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_PRINT_H
#endif
