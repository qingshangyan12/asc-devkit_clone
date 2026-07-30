/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_operator.h"

namespace AscendC {
namespace {
constexpr uint32_t SRC_COUNT = 16320;
constexpr uint32_t DST_COUNT = 16320;
constexpr uint32_t OFFSET_COUNT = 2040;
constexpr uint32_t DATA_BLOCK_BYTES = 32;
constexpr uint32_t FLOATS_PER_BLOCK = DATA_BLOCK_BYTES / sizeof(float);
constexpr uint32_t BLOCKS_PER_REPEAT = 8;
constexpr uint8_t REPEAT_TIME = 255;
constexpr uint8_t DST_BLOCK_STRIDE = 2;
constexpr uint8_t DST_REPEAT_STRIDE = 8;
constexpr uint32_t LOCAL_DST_BLOCK_COUNT =
    (REPEAT_TIME - 1) * DST_REPEAT_STRIDE + (BLOCKS_PER_REPEAT - 1) * DST_BLOCK_STRIDE + 1;
constexpr uint32_t LOCAL_DST_COUNT = LOCAL_DST_BLOCK_COUNT * FLOATS_PER_BLOCK;
constexpr int32_t BUFFER_NUM = 1;
} // namespace

class KernelGatherb1911Custom {
public:
    __aicore__ inline KernelGatherb1911Custom() = default;

    __aicore__ inline void Init(GM_ADDR src, GM_ADDR offset, GM_ADDR dst)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(src), SRC_COUNT);
        offsetGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ uint32_t*>(offset), OFFSET_COUNT);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(dst), DST_COUNT);

        pipe.InitBuffer(srcQueue, BUFFER_NUM, SRC_COUNT * sizeof(float));
        pipe.InitBuffer(offsetQueue, BUFFER_NUM, OFFSET_COUNT * sizeof(uint32_t));
        pipe.InitBuffer(dstQueue, BUFFER_NUM, LOCAL_DST_COUNT * sizeof(float));
    }

    __aicore__ inline void Process()
    {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn()
    {
        LocalTensor<float> srcLocal = srcQueue.AllocTensor<float>();
        LocalTensor<uint32_t> offsetLocal = offsetQueue.AllocTensor<uint32_t>();
        DataCopy(srcLocal, srcGlobal, SRC_COUNT);
        DataCopy(offsetLocal, offsetGlobal, OFFSET_COUNT);
        srcQueue.EnQue(srcLocal);
        offsetQueue.EnQue(offsetLocal);
    }

    __aicore__ inline void Compute()
    {
        LocalTensor<float> srcLocal = srcQueue.DeQue<float>();
        LocalTensor<uint32_t> offsetLocal = offsetQueue.DeQue<uint32_t>();
        LocalTensor<float> dstLocal = dstQueue.AllocTensor<float>();

        // The input contains DataBlock indices. Gatherb consumes byte offsets.
        LocalTensor<int32_t> byteOffsetLocal = offsetLocal.ReinterpretCast<int32_t>();
        Muls(byteOffsetLocal, byteOffsetLocal, static_cast<int32_t>(DATA_BLOCK_BYTES), OFFSET_COUNT);

        // Strided stores leave gaps, so initialize the whole addressable destination range.
        Duplicate(dstLocal, 0.0f, LOCAL_DST_COUNT);
        GatherRepeatParams repeatParams{DST_BLOCK_STRIDE, DST_REPEAT_STRIDE};
        Gatherb(dstLocal, srcLocal, offsetLocal, REPEAT_TIME, repeatParams);

        dstQueue.EnQue(dstLocal);
        srcQueue.FreeTensor(srcLocal);
        offsetQueue.FreeTensor(offsetLocal);
    }

    __aicore__ inline void CopyOut()
    {
        LocalTensor<float> dstLocal = dstQueue.DeQue<float>();
        DataCopy(dstGlobal, dstLocal, DST_COUNT);
        dstQueue.FreeTensor(dstLocal);
    }

private:
    TPipe pipe;
    TQue<QuePosition::VECIN, BUFFER_NUM> srcQueue;
    TQue<QuePosition::VECIN, BUFFER_NUM> offsetQueue;
    TQue<QuePosition::VECOUT, BUFFER_NUM> dstQueue;
    GlobalTensor<float> srcGlobal;
    GlobalTensor<uint32_t> offsetGlobal;
    GlobalTensor<float> dstGlobal;
};
} // namespace AscendC

extern "C" __global__ __aicore__ void gatherb1911_custom_kernel(GM_ADDR src, GM_ADDR offset, GM_ADDR dst)
{
    AscendC::KernelGatherb1911Custom op;
    op.Init(src, offset, dst);
    op.Process();
}
