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
 * \file kernel_pipe.h
 * \brief
 */
#ifndef IMPL_KERNEL_PIPE_IMPL_H
#define IMPL_KERNEL_PIPE_IMPL_H

#include "impl/basic_api/kernel_macros.h"
#include "impl/basic_api/common_types.h"
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#include "stub_fun.h"
#endif

namespace AscendC {
using QuePosition = TPosition;
enum class Hardware : uint8_t { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF, MAX };

enum class HardEvent : uint8_t {
    // src_dst
    MTE2_MTE1,
    MTE1_MTE2,
    MTE1_M,
    M_MTE1,
    MTE2_V,
    V_MTE2,
    MTE3_V,
    V_MTE3,
    M_V,
    V_M,
    V_V,
    MTE3_MTE1,
    MTE1_MTE3,
    MTE1_V,
    MTE2_M,
    M_MTE2,
    V_MTE1,
    M_FIX,
    FIX_M,
    MTE3_MTE2,
    MTE2_MTE3,
    S_V,
    V_S,
    S_MTE2,
    MTE2_S,
    S_MTE3,
    MTE3_S,
    MTE2_FIX,
    FIX_MTE2,
    FIX_S,
    M_S,
    FIX_MTE3,
    MTE1_FIX,
    FIX_MTE1,
    FIX_FIX,
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003))
    FIX_V,
#endif
    MAX,
};

enum class HardEventAic : uint8_t {
    // src_dst
    MTE2_MTE1,
    MTE1_MTE2,
    MTE1_M,
    M_MTE1,
    MTE3_MTE1,
    MTE1_MTE3,
    MTE2_M,
    M_MTE2,
    M_FIX,
    FIX_M,
    MTE3_MTE2,
    MTE2_MTE3,
    S_MTE2,
    MTE2_S,
    S_MTE3,
    MTE3_S,
    MTE2_FIX,
    FIX_MTE2,
    FIX_S,
    M_S,
    FIX_MTE3,
    MTE1_FIX,
    FIX_MTE1,
    FIX_FIX,
    MAX,
};

enum class HardEventAiv : uint8_t {
    // src_dst
    MTE2_V,
    V_MTE2,
    MTE3_V,
    V_MTE3,
    V_V,
    MTE3_MTE2,
    MTE2_MTE3,
    S_V,
    V_S,
    S_MTE2,
    MTE2_S,
    S_MTE3,
    MTE3_S,
    MAX,
};

enum class MemoryT : uint8_t { L1 = 0, L0A, L0B, L0C, UB, BIAS };

enum class MemDsbT : uint8_t { ALL = 0, DDR, UB, SEQ };

#if defined(ASCENDC_CPU_DEBUG) || (defined(__NPU_ARCH__) && (__NPU_ARCH__ != 2201) && (__NPU_ARCH__ != 3101))
constexpr uint8_t EVENT_NUM = static_cast<uint8_t>(HardEvent::MAX);
#else
#ifdef SPLIT_CORE_CUBE
constexpr uint8_t EVENT_NUM = static_cast<uint8_t>(HardEventAic::MAX);
#else
constexpr uint8_t EVENT_NUM = static_cast<uint8_t>(HardEventAiv::MAX);
#endif
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 1001) &&                         \
        (__NPU_ARCH__ != 2002)
constexpr int32_t PIPE_NUM = 7;
constexpr pipe_t SUPPORTED_PIPE[PIPE_NUM] = { PIPE_S, PIPE_V, PIPE_M, PIPE_MTE1, PIPE_MTE2, PIPE_MTE3, PIPE_FIX };
#else
constexpr int32_t PIPE_NUM = 6;
constexpr pipe_t SUPPORTED_PIPE[PIPE_NUM] = { PIPE_S, PIPE_V, PIPE_M, PIPE_MTE1, PIPE_MTE2, PIPE_MTE3 };
#endif

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

__aicore__ constexpr bool IsSupportedPipe(pipe_t pipe)
{
    for (int i = 0; i < PIPE_NUM; i++) {
        if (pipe == SUPPORTED_PIPE[i]) {
            return true;
        }
    }
    return false;
}

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

} // namespace AscendC

#endif // ASCENDC_KERNEL_EVENT_IMPL_H