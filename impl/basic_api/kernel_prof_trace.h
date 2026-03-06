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
#ifndef ASCENDC_KERNEL_PROF_TRACE_IMPL_H
#define ASCENDC_KERNEL_PROF_TRACE_IMPL_H
#include "kernel_utils.h"
#include "impl/utils/debug/asc_aicore_time_impl.h"

namespace AscendC {
__aicore__ inline void ProfStartImpl()
{
#ifndef ASCENDC_CPU_DEBUG
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510
    asc_prof_start();
#else
    ASCENDC_DEBUG_ASSERT(false, KERNEL_LOG_INTERNAL(KERNEL_ERROR, "MetricsProfStart is not supported on current device\n"));
#endif
#endif
}

__aicore__ inline void ProfStopImpl()
{
#ifndef ASCENDC_CPU_DEBUG
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510
    asc_prof_stop();
#else
    ASCENDC_DEBUG_ASSERT(false, KERNEL_LOG_INTERNAL(KERNEL_ERROR, "MetricsProfStart is not supported on current device\n"));
#endif
#endif
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template<pipe_t pipe, uint16_t index>
__aicore__ inline void MarkStampImpl()
{
#ifndef ASCENDC_CPU_DEBUG
    asc_mark_stamp<pipe, index>();
#endif
}

template<pipe_t pipe>
__aicore__ inline void MarkStampImpl(uint16_t index)
{
#ifndef ASCENDC_CPU_DEBUG
    asc_mark_stamp<pipe>(index);
#endif
}
#endif
} // namespace AscendC
#endif // ASCENDC_KERNEL_PROF_TRACE_IMPL_H
