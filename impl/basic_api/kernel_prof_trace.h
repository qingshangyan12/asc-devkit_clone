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
 * \file kernel_prof_trace.h
 * \brief
 */
#if !defined(_ASCENDC_INCLUDE_INTERNAL_HEADERS_)
#pragma message("impl/basic_api/kernel_prof_trace.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_prof_trace_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define _ASCENDC_INCLUDE_INTERNAL_HEADERS_
#define UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PROF_TRACE_H
#endif

#ifndef ASCENDC_KERNEL_PROF_TRACE_IMPL_H
#define ASCENDC_KERNEL_PROF_TRACE_IMPL_H
#include "kernel_utils.h"

namespace AscendC {
#ifdef ASCENDC_TRACE_ON
constexpr uint32_t PROF_START_EVENT = 0x80000000;
constexpr uint32_t PROF_STOP_EVENT = 0xc0000000;

__aicore__ inline void ProfMarkEvent(void)
{
    if (g_coreType == AIV) {
        __asm__ volatile("NOP_BAR.V");
    } else if (g_coreType == AIC) {
        __asm__ volatile("NOP_BAR.M");
        __asm__ volatile("NOP_BAR.MTE1");
    } else {
        __asm__ volatile("NOP_BAR.V");
        __asm__ volatile("NOP_BAR.M");
        __asm__ volatile("NOP_BAR.MTE1");
    }
    __asm__ volatile("NOP_BAR.MTE2");
    __asm__ volatile("NOP_BAR.MTE3");
}
#endif

__aicore__ inline void ProfStartImpl()
{
#ifndef ASCENDC_CPU_DEBUG
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3101
    bisheng::cce::metrics_prof_start();
#else
    ASCENDC_DEBUG_ASSERT(false, KERNEL_LOG_INTERNAL(KERNEL_ERROR, "MetricsProfStart is not supported on current device\n"));
#endif
#endif
}

__aicore__ inline void ProfStopImpl()
{
#ifndef ASCENDC_CPU_DEBUG
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3101
    bisheng::cce::metrics_prof_stop();
#else
    ASCENDC_DEBUG_ASSERT(false, KERNEL_LOG_INTERNAL(KERNEL_ERROR, "MetricsProfStart is not supported on current device\n"));
#endif
#endif
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
template<pipe_t pipe, uint16_t index>
__aicore__ inline void MarkStampImpl()
{
#ifndef ASCENDC_CPU_DEBUG
    bisheng::cce::mark_stamp<pipe, index>();
#endif
}
#endif
} // namespace AscendC
#endif // ASCENDC_KERNEL_PROF_TRACE_IMPL_H

#if defined(UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PROF_TRACE_H)
#undef _ASCENDC_INCLUDE_INTERNAL_HEADERS_
#undef UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PROF_TRACE_H
#endif