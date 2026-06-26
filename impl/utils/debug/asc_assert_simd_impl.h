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
 * \file asc_assert_simd_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_ASSERT_SIMD_IMPL__
#warning "impl/utils/debug/asc_assert_simd_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "utils/debug/asc_assert.h" and use public functions or variables defined in interface header files."
#endif

#ifndef IMPL_UTILS_DEBUG_ASC_ASSERT_SIMD_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_ASSERT_SIMD_IMPL_H

#ifndef ASCENDC_CPU_DEBUG
#include "impl/utils/debug/npu_arch_3510/asc_aicore_printf_utils.h"

namespace __asc_simd_vf {
__ASC_USE_RESERVED_UBUF__(3510, "assert is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
static __attribute__((noinline)) __simd_callee__ void __assert_fail(__ubuf__ const char* __assertion,
    __ubuf__ const char* __file, unsigned int __line, __ubuf__ const char* __function) noexcept
{
    (void)__function;
    enable_asc_diagnostics();
    scalar_printf_impl(DumpType::DUMP_ASSERT, "[ASSERT] %s:%u: Assertion %s failed.\n", __file, __line, __assertion);
    __ubuf__ BlockVFBufInfo* blockInfo = get_printf_ubuf_addr(0);
    __ubuf__ volatile uint8_t* assertFlag = &blockInfo->assert;
    *assertFlag = 1;
    while (*assertFlag != 2) {
    }
    *((__ubuf__ uint8_t*)-1) = 0;
}

template <typename... Args>
static __attribute__((noinline)) __simd_callee__ void __assert_fail_msg(__ubuf__ const char* __assertion,
    __ubuf__ const char* __file, unsigned int __line, __ubuf__ const char* __function, __ubuf__ const char* fmt,
    Args&&... args) noexcept
{
    scalar_printf_impl(DumpType::DUMP_ASSERT, fmt, args...);
    __assert_fail(__assertion, __file, __line, __function);
}
} // namespace __asc_simd_vf
#endif

#endif // IMPL_UTILS_DEBUG_ASC_ASSERT_SIMD_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_ASSERT_SIMD_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_ASSERT_SIMD_IMPL__
#endif
