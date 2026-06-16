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
 * \file asc_aicore_printf_intf.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_AICORE_PRINTF_INTF_H
#define IMPL_UTILS_DEBUG_ASC_AICORE_PRINTF_INTF_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_PRINTF_INTF_H__
#endif

#include "impl/utils/sys_macros.h"

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifndef __CHECK_FEATURE_AT_PRECOMPILE
#if (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
namespace __asc_aicore {
template <class... Args>
__aicore__ inline void printf_impl(__gm__ const char* fmt, Args&&... args);

template <class... Args>
__aicore__ inline void printf(__gm__ const char* fmt, Args&&... args)
{
    printf_impl(fmt, args...);
}

template <class... Args>
__aicore__ inline void PRINTF(__gm__ const char* fmt, Args&&... args)
{
    printf_impl(fmt, args...);
}
} // namespace __asc_aicore
#else
namespace __asc_aicore {
template <class... Args>
__aicore__ inline void printf(__gm__ const char* fmt, Args&&... args) {}
template <class... Args>
__aicore__ inline void PRINTF(__gm__ const char* fmt, Args&&... args) {}
} // namespace __asc_aicore
#endif
#endif
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_PRINTF_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_PRINTF_INTF_H__
#endif

#endif
