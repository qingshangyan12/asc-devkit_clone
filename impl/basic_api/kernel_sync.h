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
 * \file kernel_sync.h
 * \brief
 */
#ifndef IMPL_BASIC_API_KERNEL_SYNC_H
#define IMPL_BASIC_API_KERNEL_SYNC_H

#include "kernel_macros.h"
#include "common_types.h"
#include "kernel_log.h"
namespace AscendC {

enum class MemDsbT : uint8_t { ALL = 0, DDR, UB, SEQ };

#if defined(__NPU_ARCH__)
template <pipe_t pipe>
__aicore__ inline constexpr bool IsSplitVectorPipe()
{
    return pipe == PIPE_S || pipe == PIPE_V || pipe == PIPE_MTE2 || pipe == PIPE_MTE3;
}

template <pipe_t pipe>
__aicore__ inline constexpr bool IsSplitCubePipe()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 1001 || __NPU_ARCH__ == 2002)
    return pipe == PIPE_S || pipe == PIPE_MTE1 || pipe == PIPE_MTE2 || pipe == PIPE_MTE3 || pipe == PIPE_M;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
    return pipe == PIPE_S || pipe == PIPE_MTE1 || pipe == PIPE_MTE2 || pipe == PIPE_FIX || pipe == PIPE_M;
#else
    return pipe == PIPE_S || pipe == PIPE_MTE1 || pipe == PIPE_MTE2 || pipe == PIPE_MTE3 || pipe == PIPE_FIX || pipe == PIPE_M;
#endif
}

template <pipe_t pipe>
__aicore__ inline void PipeBarrierInternal()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
    if constexpr (IsSplitVectorPipe<pipe>() || pipe == PIPE_ALL) {
        if ASCEND_IS_AIV {
            pipe_barrier(pipe);
        }
    }
    if constexpr (IsSplitCubePipe<pipe>() || pipe == PIPE_ALL){
        if ASCEND_IS_AIC {
            pipe_barrier(pipe);
        }
    }
#else
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    if constexpr (pipe == PIPE_V) {
        return;
    }
#endif
    pipe_barrier(pipe);
#endif
}

template <pipe_t srcPipe, pipe_t dstPipe>
__aicore__ inline void SetFlagInternal(event_t evt)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
    if constexpr (IsSplitVectorPipe<srcPipe>() && IsSplitVectorPipe<dstPipe>()) {
        if ASCEND_IS_AIV {
            set_flag(srcPipe, dstPipe, evt);
        }
    }
    if constexpr (IsSplitCubePipe<srcPipe>() && IsSplitCubePipe<dstPipe>()){
        if ASCEND_IS_AIC {
            set_flag(srcPipe, dstPipe, evt);
        }
    }
#else
    set_flag(srcPipe, dstPipe, evt);
#endif
}

template <pipe_t srcPipe, pipe_t dstPipe>
__aicore__ inline void WaitFlagInternal(event_t evt)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
    (void)evt;
    if constexpr (IsSplitVectorPipe<srcPipe>() && IsSplitVectorPipe<dstPipe>()) {
        if ASCEND_IS_AIV {
            wait_flag(srcPipe, dstPipe, evt);
        }
    }
    if constexpr (IsSplitCubePipe<srcPipe>() && IsSplitCubePipe<dstPipe>()){
        if ASCEND_IS_AIC {
            wait_flag(srcPipe, dstPipe, evt);
        }
    }
#else
    wait_flag(srcPipe, dstPipe, evt);
#endif
}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||                       \
    (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) ||                       \
    (__NPU_ARCH__ == 3113))
template <MemDsbT arg>
__aicore__ inline void DataSyncBarrierImpl()
{
    if constexpr (arg == MemDsbT::UB) {
        if ASCEND_IS_AIC {
            return;
        }
    }
    dsb((mem_dsb_t)arg);
}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102))

template <pipe_t pipe> __aicore__ inline void PipeBarrierImpl()
{
#if (__NPU_ARCH__ == 5102)
    if constexpr (pipe == PIPE_MTE3) {
        return;
    }
#else
    if ASCEND_IS_AIC {
        if constexpr (pipe == PIPE_MTE3) {
            return;
        }
    }
#endif
    if constexpr (pipe != PIPE_V) {
        pipe_barrier(pipe);
    }
    return;
}
#else
template <pipe_t pipe> __aicore__ inline void PipeBarrierImpl()
{
#if __NPU_ARCH__ == 3102
    return;
#endif
#if (__NPU_ARCH__ == 3002)
    if constexpr (pipe == PIPE_S || pipe == PIPE_V) {
        return;
    }
#endif
#if (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113)
    if constexpr (pipe == PIPE_V) {
        return;
    }
#endif
#if (__NPU_ARCH__ == 2201)
    if ASCEND_IS_AIC {
        if constexpr (pipe == PIPE_V) {
            return;
        }
    }
#endif
    pipe_barrier(pipe);
}
#endif

template <HardEvent event>
__aicore__ inline void SetFlagImpl(int32_t eventID)
{
    ASCENDC_ASSERT((eventID >= 0 && eventID < QUE_MAX_EVENT),
                   { KERNEL_LOG(KERNEL_ERROR, "eventID %d should be in range [0, %d)", eventID, QUE_MAX_EVENT); });
    event_t e = static_cast<event_t>(eventID);
    switch (event) {
        case HardEvent::MTE2_MTE1:
            SetFlagInternal<PIPE_MTE2, PIPE_MTE1>(e);
            break;
        case HardEvent::MTE1_MTE2:
            SetFlagInternal<PIPE_MTE1, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE2_MTE3:
            SetFlagInternal<PIPE_MTE2, PIPE_MTE3>(e);
            break;
        case HardEvent::MTE3_MTE2:
            SetFlagInternal<PIPE_MTE3, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE1_M:
            SetFlagInternal<PIPE_MTE1, PIPE_M>(e);
            break;
        case HardEvent::M_MTE1:
            SetFlagInternal<PIPE_M, PIPE_MTE1>(e);
            break;
        case HardEvent::MTE2_V:
            SetFlagInternal<PIPE_MTE2, PIPE_V>(e);
            break;
        case HardEvent::V_MTE2:
            SetFlagInternal<PIPE_V, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE3_V:
            SetFlagInternal<PIPE_MTE3, PIPE_V>(e);
            break;
        case HardEvent::V_MTE3:
            SetFlagInternal<PIPE_V, PIPE_MTE3>(e);
            break;
        case HardEvent::M_V:
            SetFlagInternal<PIPE_M, PIPE_V>(e);
            break;
        case HardEvent::M_S:
            SetFlagInternal<PIPE_M, PIPE_S>(e);
            break;
        case HardEvent::V_M:
            SetFlagInternal<PIPE_V, PIPE_M>(e);
            break;
        case HardEvent::S_V:
            SetFlagInternal<PIPE_S, PIPE_V>(e);
            break;
        case HardEvent::V_S:
            SetFlagInternal<PIPE_V, PIPE_S>(e);
            break;
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ != 3002) || (__NPU_ARCH__ == 3003))
        case HardEvent::V_V:
            PipeBarrierInternal<PIPE_V>();
            return;
#endif
        case HardEvent::MTE3_MTE1:
            SetFlagInternal<PIPE_MTE3, PIPE_MTE1>(e);
            break;
        case HardEvent::MTE1_MTE3:
            SetFlagInternal<PIPE_MTE1, PIPE_MTE3>(e);
            break;
        case HardEvent::MTE1_V:
            SetFlagInternal<PIPE_MTE1, PIPE_V>(e);
            break;
        case HardEvent::MTE2_M:
            SetFlagInternal<PIPE_MTE2, PIPE_M>(e);
            break;
        case HardEvent::M_MTE2:
            SetFlagInternal<PIPE_M, PIPE_MTE2>(e);
            break;
        case HardEvent::S_MTE2:
            SetFlagInternal<PIPE_S, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE2_S:
            SetFlagInternal<PIPE_MTE2, PIPE_S>(e);
            break;
        case HardEvent::V_MTE1:
            SetFlagInternal<PIPE_V, PIPE_MTE1>(e);
            break;
        case HardEvent::S_MTE3:
            SetFlagInternal<PIPE_S, PIPE_MTE3>(e);
            break;
        case HardEvent::MTE3_S:
            SetFlagInternal<PIPE_MTE3, PIPE_S>(e);
            break;

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||           \
    (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) ||                            \
    ((__NPU_ARCH__ == 3113)))
        case HardEvent::M_FIX:
            SetFlagInternal<PIPE_M, PIPE_FIX>(e);
            break;
        case HardEvent::FIX_M:
            SetFlagInternal<PIPE_FIX, PIPE_M>(e);
            break;
        case HardEvent::FIX_MTE3:
            SetFlagInternal<PIPE_FIX, PIPE_MTE3>(e);
            break;
        case HardEvent::FIX_MTE2:
            SetFlagInternal<PIPE_FIX, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE2_FIX:
            SetFlagInternal<PIPE_MTE2, PIPE_FIX>(e);
            break;
        case HardEvent::FIX_S:
            SetFlagInternal<PIPE_FIX, PIPE_S>(e);
            break;
        case HardEvent::MTE1_FIX:
            SetFlagInternal<PIPE_MTE1, PIPE_FIX>(e);
            break;
        case HardEvent::FIX_MTE1:
            SetFlagInternal<PIPE_FIX, PIPE_MTE1>(e);
            break;
        case HardEvent::FIX_FIX:
            PipeBarrierInternal<PIPE_FIX>();
            break;
#endif
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102))
        case HardEvent::FIX_V:
            SetFlagInternal<PIPE_FIX, PIPE_V>(e);
            break;
#endif
        case HardEvent::MAX:
            break;
        default:
            ASCENDC_ASSERT((0), { KERNEL_LOG(KERNEL_ERROR, "invalid event %d", static_cast<int32_t>(event)); });
            break;
    }
}

__aicore__ inline void WaitFlagImpl(const HardEvent event, int32_t eventID)
{
    ASCENDC_ASSERT((eventID >= 0 && eventID < QUE_MAX_EVENT),
                   { KERNEL_LOG(KERNEL_ERROR, "eventID %d should be in range [0, %d)", eventID, QUE_MAX_EVENT); });
    event_t e = static_cast<event_t>(eventID);
    switch (event) {
#ifndef SPLIT_CORE_VEC // CUBE core
        case HardEvent::MTE2_MTE1:
            WaitFlagInternal<PIPE_MTE2, PIPE_MTE1>(e);
            break;
        case HardEvent::MTE1_MTE2:
            WaitFlagInternal<PIPE_MTE1, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE1_M:
            WaitFlagInternal<PIPE_MTE1, PIPE_M>(e);
            break;
        case HardEvent::M_MTE1:
            WaitFlagInternal<PIPE_M, PIPE_MTE1>(e);
            break;
        case HardEvent::MTE3_MTE1:
            WaitFlagInternal<PIPE_MTE3, PIPE_MTE1>(e);
            break;
        case HardEvent::MTE1_MTE3:
            WaitFlagInternal<PIPE_MTE1, PIPE_MTE3>(e);
            break;
        case HardEvent::MTE2_M:
            WaitFlagInternal<PIPE_MTE2, PIPE_M>(e);
            break;
        case HardEvent::M_MTE2:
            WaitFlagInternal<PIPE_M, PIPE_MTE2>(e);
            break;
#endif
#ifndef SPLIT_CORE_CUBE // VECTOR core
        case HardEvent::MTE2_V:
            WaitFlagInternal<PIPE_MTE2, PIPE_V>(e);
            break;
        case HardEvent::V_MTE2:
            WaitFlagInternal<PIPE_V, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE3_V:
            WaitFlagInternal<PIPE_MTE3, PIPE_V>(e);
            break;
        case HardEvent::V_MTE3:
            WaitFlagInternal<PIPE_V, PIPE_MTE3>(e);
            break;
#endif
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ != 2201) && (__NPU_ARCH__ != 3101))
        case HardEvent::M_V:
            WaitFlagInternal<PIPE_M, PIPE_V>(e);
            break;
        case HardEvent::V_M:
            WaitFlagInternal<PIPE_V, PIPE_M>(e);
            break;
        case HardEvent::MTE1_V:
            WaitFlagInternal<PIPE_MTE1, PIPE_V>(e);
            break;
        case HardEvent::V_MTE1:
            WaitFlagInternal<PIPE_V, PIPE_MTE1>(e);
            break;
#endif
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||           \
    (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) ||                            \
    ((__NPU_ARCH__ == 3113)))
        case HardEvent::FIX_M:
            WaitFlagInternal<PIPE_FIX, PIPE_M>(e);
            break;
        case HardEvent::M_FIX:
            WaitFlagInternal<PIPE_M, PIPE_FIX>(e);
            break;
        case HardEvent::MTE2_FIX:
            WaitFlagInternal<PIPE_MTE2, PIPE_FIX>(e);
            break;
        case HardEvent::FIX_MTE2:
            WaitFlagInternal<PIPE_FIX, PIPE_MTE2>(e);
            break;
        case HardEvent::FIX_S:
            WaitFlagInternal<PIPE_FIX, PIPE_S>(e);
            break;
        case HardEvent::FIX_MTE3:
            WaitFlagInternal<PIPE_FIX, PIPE_MTE3>(e);
            break;
        case HardEvent::MTE1_FIX:
            WaitFlagInternal<PIPE_MTE1, PIPE_FIX>(e);
            break;
        case HardEvent::FIX_MTE1:
            WaitFlagInternal<PIPE_FIX, PIPE_MTE1>(e);
            break;
        case HardEvent::FIX_FIX:
            PipeBarrierInternal<PIPE_FIX>();
            break;
#endif
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102))
        case HardEvent::FIX_V:
            WaitFlagInternal<PIPE_FIX, PIPE_V>(e);
            break;
#endif
        case HardEvent::MTE3_MTE2:
            WaitFlagInternal<PIPE_MTE3, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE2_MTE3:
            WaitFlagInternal<PIPE_MTE2, PIPE_MTE3>(e);
            break;
        case HardEvent::S_MTE2:
            WaitFlagInternal<PIPE_S, PIPE_MTE2>(e);
            break;
        case HardEvent::MTE2_S:
            WaitFlagInternal<PIPE_MTE2, PIPE_S>(e);
            break;
        case HardEvent::S_MTE3:
            WaitFlagInternal<PIPE_S, PIPE_MTE3>(e);
            break;
        case HardEvent::MTE3_S:
            WaitFlagInternal<PIPE_MTE3, PIPE_S>(e);
            break;
        case HardEvent::M_S:
            WaitFlagInternal<PIPE_M, PIPE_S>(e);
            break; 
        case HardEvent::S_V:
            WaitFlagInternal<PIPE_S, PIPE_V>(e);
            break;
        case HardEvent::V_S:
            WaitFlagInternal<PIPE_V, PIPE_S>(e);
            break;
        case HardEvent::V_V:
            return;
        case HardEvent::MAX:
            break;
        default:
            break;
    }
    return;
}

} // namespace AscendC

#endif // ASCENDC__KERNEL_SYNC_IMPL_H