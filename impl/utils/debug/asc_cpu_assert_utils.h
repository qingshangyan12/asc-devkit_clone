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
 * \file asc_cpu_assert_utils.h
 * \brief CPU assertion message formatting.
 */
#ifndef IMPL_UTILS_DEBUG_ASC_CPU_ASSERT_UTILS_H
#define IMPL_UTILS_DEBUG_ASC_CPU_ASSERT_UTILS_H

#include <cstdio>
#include <cstdarg>

namespace __asc_aicore {
inline void printf_impl_assert_msg(
    const char* __assertion, const char* __file, unsigned int __line, const char* __function,
    const char* fmt, ...)
{
    std::printf("[ASSERT] %s:%u: %s: Assertion '%s' failed. ", __file, __line, __function, __assertion);
    std::va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
}
} // namespace __asc_aicore

#endif // IMPL_UTILS_DEBUG_ASC_CPU_ASSERT_UTILS_H
