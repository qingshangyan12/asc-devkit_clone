/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file data_copy.h
 * \brief Header file for data_copy best practices
 */

#ifndef DATA_COPY_H
#define DATA_COPY_H

#include "data_utils.h"
#include "kernel_operator.h"
#include "acl/acl.h"

template <uint32_t M, uint32_t N, uint32_t NUM_BLOCKS, uint32_t SINGLE_CORE_M, uint32_t TILE_M, uint32_t BLOCK_LEN>
class KernelDataCopyPadGm2Ub {
public:
    __aicore__ inline KernelDataCopyPadGm2Ub() {}

    __aicore__ inline void Init(GM_ADDR src, GM_ADDR dst)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(src), M * N);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(dst), M * N);
    }

    __aicore__ inline void Process()
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * SINGLE_CORE_M;

        uint64_t ubAddr = 0;
        AscendC::LocalTensor<half> ubLocal(AscendC::TPosition::VECIN, ubAddr, UB_BUFFER_SIZE);

        constexpr uint32_t nLoopCount = N / BLOCK_ELEMS;
        constexpr uint32_t mLoopCount = SINGLE_CORE_M / TILE_M;


        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(TILE_M),
            static_cast<uint16_t>(BLOCK_LEN),
            static_cast<uint16_t>(GM_STRIDE),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
            uint32_t nIdx = nBlockIdx * BLOCK_ELEMS;
            for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
                uint32_t mIdx = mStart + mBlockIdx * TILE_M;

                AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * N + nIdx], copyInParams, padParams);
                // AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
                // AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);

                // AscendC::DataCopyParams copyOutParams{
                //     static_cast<uint16_t>(TILE_M),
                //     static_cast<uint16_t>(BLOCK_LEN),
                //     0,
                //     static_cast<uint16_t>(GM_STRIDE)};
                // AscendC::DataCopyPad(dstGlobal[mIdx * N + nIdx], ubLocal, copyOutParams);
                // AscendC::SetFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
                // AscendC::WaitFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
            }
        }
    }

private:
    static constexpr uint32_t BLOCK_ELEMS = BLOCK_LEN / sizeof(half);
    static constexpr uint32_t UB_BUFFER_SIZE = TILE_M * BLOCK_ELEMS;
    static constexpr uint32_t GM_STRIDE = (N - BLOCK_ELEMS) * sizeof(half);

    AscendC::GlobalTensor<half> srcGlobal;
    AscendC::GlobalTensor<half> dstGlobal;
};

template <uint32_t M, uint32_t N, uint32_t NUM_BLOCKS, uint32_t SINGLE_CORE_M, uint32_t TILE_M, uint32_t TILE_N>
class KernelDataCopyGm2L1 {
public:
    __aicore__ inline KernelDataCopyGm2L1() {}

    __aicore__ inline void Init(GM_ADDR src)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(src), M * N);
    }

    __aicore__ inline void Process()
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();

        uint32_t mStart = blockIdx * SINGLE_CORE_M;
        uint32_t mEnd = mStart + SINGLE_CORE_M;

        AscendC::LocalTensor<half> l1Local(AscendC::TPosition::A1, L1_ADDR, L1_BUFFER_SIZE);

        for (uint32_t mIdx = mStart; mIdx < mEnd; mIdx += TILE_M) {
            for (uint32_t nIdx = 0; nIdx < N; nIdx += TILE_N) {
                uint32_t curCols = (nIdx + TILE_N > N) ? (N - nIdx) : TILE_N;
                AscendC::Nd2NzParams nd2nzParams;
                nd2nzParams.ndNum = 1;
                nd2nzParams.nValue = TILE_M;
                nd2nzParams.dValue = curCols;
                nd2nzParams.srcNdMatrixStride = 0;
                nd2nzParams.srcDValue = N;
                nd2nzParams.dstNzC0Stride = AlignUp(TILE_M, BLOCK_CUBE);
                nd2nzParams.dstNzNStride = 1;
                nd2nzParams.dstNzMatrixStride = 0;
                AscendC::DataCopy(l1Local, srcGlobal[mIdx * N + nIdx], nd2nzParams);
                AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
            }
        }

        AscendC::PipeBarrier<PIPE_ALL>();
    }

private:
    static constexpr uint32_t AlignUp(uint32_t numerator, uint32_t denominator)
    {
        return (numerator + denominator - 1) / denominator * denominator;
    }

    static constexpr uint32_t BLOCK_CUBE = 16;
    static constexpr uint32_t C0_SIZE = 32 / sizeof(half);
    static constexpr uint32_t L1_BUFFER_SIZE = AlignUp(TILE_M, BLOCK_CUBE) * AlignUp(TILE_N, C0_SIZE);
    static constexpr uint64_t L1_ADDR = 0;

    AscendC::GlobalTensor<half> srcGlobal;
};

#endif
