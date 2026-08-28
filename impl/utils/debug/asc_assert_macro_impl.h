/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 */

/* !
 * \file asc_assert_macro_impl.h
 * \brief Internal assert macro helpers.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_ASSERT_MACRO_IMPL__
#warning \
    "impl/utils/debug/asc_assert_macro_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"utils/debug/asc_assert.h\" and use public functions or variables defined in interface headers files."
#endif

#ifndef IMPL_UTILS_DEBUG_ASC_ASSERT_MACRO_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_ASSERT_MACRO_IMPL_H

#define ASC_INTERNAL_COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define ASC_INTERNAL_COUNT_ARGS(...) ASC_INTERNAL_COUNT_ARGS_IMPL(__VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1)
#define ASC_INTERNAL_GET_ARG_COUNT(...) ASC_INTERNAL_COUNT_ARGS(__VA_ARGS__)
#define ASC_INTERNAL_ASSERT_IMPL(N, ...) ASC_INTERNAL_ASSERT_FUNC(N, __VA_ARGS__)
#define ASC_INTERNAL_ASSERT_FUNC(N, ...) ASC_INTERNAL_ASSERT_##N(__VA_ARGS__)
#define ASC_INTERNAL_ASSERT_1(expr) \
    (static_cast<bool>(expr) ? void(0) : __assert_fail(#expr, __FILE__, __LINE__, (__PRETTY_FUNCTION__)))
#ifndef ASCENDC_CPU_DEBUG
#define ASC_INTERNAL_ASSERT_2(expr, fmt, ...) \
    (static_cast<bool>(expr) ? void(0) : __assert_fail_msg(#expr, __FILE__, __LINE__, (__PRETTY_FUNCTION__), fmt, ##__VA_ARGS__))
#else
#define ASC_INTERNAL_ASSERT_2(expr, ...) ASC_INTERNAL_ASSERT_1(expr)
#endif

#endif // IMPL_UTILS_DEBUG_ASC_ASSERT_MACRO_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_ASSERT_MACRO_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_ASSERT_MACRO_IMPL__
#endif
