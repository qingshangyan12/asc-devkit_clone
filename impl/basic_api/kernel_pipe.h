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
#ifndef IMPL_BASIC_API_KERNEL_PIPE_H
#define IMPL_BASIC_API_KERNEL_PIPE_H

#include "kernel_macros.h"
#include "common_types.h"
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#include "stub_fun.h"
#endif

namespace AscendC {

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
template <pipe_t pipe, bool mode>
__aicore__ inline void GetBufInternal(uint8_t bufId) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
    if constexpr (IsSplitVectorPipe<pipe>()) {
        if ASCEND_IS_AIV {
            get_buf(pipe, bufId, mode);
        }
    }
    if constexpr (IsSplitCubePipe<pipe>()){
        if ASCEND_IS_AIC {
            get_buf(pipe, bufId, mode);
        }
    }
#else
    get_buf(pipe, bufId, mode);
#endif
}

template <pipe_t pipe, bool mode>
__aicore__ inline void RlsBufInternal(uint8_t bufId) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
    if constexpr (IsSplitVectorPipe<pipe>()) {
        if ASCEND_IS_AIV {
            rls_buf(pipe, bufId, mode);
        }
    }
    if constexpr (IsSplitCubePipe<pipe>()){
        if ASCEND_IS_AIC {
            rls_buf(pipe, bufId, mode);
        }
    }
#else
    rls_buf(pipe, bufId, mode);
#endif
}

__aicore__ constexpr pipe_t GetPipeByPos(TPosition pos, TPosition altPos)
{
    if (pos == TPosition::GM) {
        return altPos == TPosition::GM ? PIPE_MTE2 : PIPE_MTE3;
    } else if (pos == TPosition::VECIN) {
        return altPos == TPosition::GM ? PIPE_V : PIPE_MTE2;
    } else if (pos == TPosition::VECCALC) {
        return PIPE_V;
    } else if (pos == TPosition::VECOUT) {
        return altPos == TPosition::GM ? PIPE_V : PIPE_MTE3;
    }
    return PIPE_V;
}

__aicore__ constexpr pipe_t GetPipe(Hardware src, Hardware dst, bool fwdDirect)
{
    if (src == Hardware::GM) {
        if (dst == Hardware::UB) {
            return fwdDirect ? PIPE_MTE2 : PIPE_V;
        }
        if (dst == Hardware::L1) {
            return fwdDirect ? PIPE_MTE2 : PIPE_MTE1;
        }
    } else if (src == Hardware::UB) {
        if (dst == Hardware::GM) {
            return fwdDirect ? PIPE_V : PIPE_MTE3;
        }
        if (dst == Hardware::L1) {
            return fwdDirect ? PIPE_MTE3 : PIPE_MTE1;
        }
        if (dst == Hardware::UB) {
            return fwdDirect ? PIPE_MTE2 : PIPE_MTE3;
        }
    } else if (src == Hardware::L1) {
        if (dst == Hardware::L0A) {
            return fwdDirect ? PIPE_MTE1 : PIPE_M;
        } else if (dst == Hardware::L0B) {
            return fwdDirect ? PIPE_MTE1 : PIPE_M;
        }
    } else if (src == Hardware::L0C) {
        return fwdDirect ? PIPE_M : PIPE_FIX;
    }
    return PIPE_S;
}

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

#endif // ASCENDC_KERNEL_PIPE_IMPL_H