/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_debug_utils.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_DEBUG_UTILS_H
#define IMPL_UTILS_DEBUG_ASC_DEBUG_UTILS_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS__
#warning "asc_debug_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#include "impl/utils/base/asc_utils_macros.h"
#include "impl/utils/base/asc_utils_types.h"

#if __NPU_ARCH__ == 1001
#include "impl/utils/debug/npu_arch_2201/asc_debug_utils_impl.h"
#elif __NPU_ARCH__ == 3501
#include "impl/utils/debug/npu_arch_3510/asc_debug_utils_impl.h"
#endif

inline __gm__ uint8_t* __gm__ g_sysPrintFifoSpace = nullptr;

__aicore__ inline void enable_asc_diagnostics()
{
#if defined(__ENABLE_ASCENDC_PRINTF__)
#if (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0)) || defined(ASCENDC_TIME_STAMP_ON)
    static const struct AscTlv __asc_debug_meta_section__ __attribute__ ((used, section (".ascend.meta"))) =
    {4, 4, 1};
#endif // defined(ASCENDC_DUMP) || defined(ASCENDC_TIME_STAMP_ON)
#endif // __ENABLE_ASCENDC_PRINTF__
}

__aicore__ inline void asc_entire_dcci(__gm__ uint64_t* ptr)
{
    asc_entire_dcci_impl(ptr);
}




#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_DEBUG_UTILS_H