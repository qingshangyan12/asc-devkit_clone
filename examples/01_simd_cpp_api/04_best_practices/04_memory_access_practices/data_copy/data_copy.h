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

#include "kernel_operator.h"
#include "acl/acl.h"

template <uint32_t m, uint32_t n, uint32_t numBlocks, uint32_t singleCoreM, uint32_t tileM, uint32_t tileN>
class KernelDataCopyPadGm2Ub {
public:
    __aicore__ inline KernelDataCopyPadGm2Ub() {}

    __aicore__ inline void Init(GM_ADDR src, GM_ADDR dst)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(src), m * n);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(dst), m * n);
    }

    __aicore__ inline void Process()
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * singleCoreM;

        uint64_t ubAddr = 0;
        AscendC::LocalTensor<half> ubLocal(AscendC::TPosition::VECIN, ubAddr, ubBufferSize);

        constexpr uint32_t nLoopCount = n / tileN;
        constexpr uint32_t mLoopCount = singleCoreM / tileM;


        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(tileM),
            static_cast<uint16_t>(tileN * sizeof(half)),
            static_cast<uint16_t>(gmStride),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
            uint32_t mIdx = mStart + mBlockIdx * tileM;
            for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                uint32_t nIdx = nBlockIdx * tileN;

                AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nIdx], copyInParams, padParams);
                // AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
                // AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);

                // AscendC::DataCopyParams copyOutParams{
                //     static_cast<uint16_t>(tileM),
                //     static_cast<uint16_t>(tileN * sizeof(half)),
                //     0,
                //     static_cast<uint16_t>(gmStride)};
                // AscendC::DataCopyPad(dstGlobal[mIdx * n + nIdx], ubLocal, copyOutParams);
                // AscendC::SetFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
                // AscendC::WaitFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
            }
        }
    }

private:
    static constexpr uint32_t ubBufferSize = tileM * tileN;
    static constexpr uint32_t gmStride = (n - tileN) * sizeof(half);

    AscendC::GlobalTensor<half> srcGlobal;
    AscendC::GlobalTensor<half> dstGlobal;
};

template <uint32_t m, uint32_t n, uint32_t numBlocks, uint32_t singleCoreM, uint32_t tileM, uint32_t tileN>
class KernelDataCopyGm2L1 {
public:
    __aicore__ inline KernelDataCopyGm2L1() {}

    __aicore__ inline void Init(GM_ADDR src)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(src), m * n);
    }

    __aicore__ inline void Process()
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();

        uint32_t mStart = blockIdx * singleCoreM;
        uint32_t mEnd = mStart + singleCoreM;

        AscendC::LocalTensor<half> l1Local(AscendC::TPosition::A1, l1Addr, l1BufferSize);

        for (uint32_t mIdx = mStart; mIdx < mEnd; mIdx += tileM) {
            for (uint32_t nIdx = 0; nIdx < n; nIdx += tileN) {
                uint32_t curCols = (nIdx + tileN > n) ? (n - nIdx) : tileN;
                AscendC::Nd2NzParams nd2nzParams;
                nd2nzParams.ndNum = 1;
                nd2nzParams.nValue = tileM;
                nd2nzParams.dValue = curCols;
                nd2nzParams.srcNdMatrixStride = 0;
                nd2nzParams.srcDValue = n;
                nd2nzParams.dstNzC0Stride = AlignUp(tileM, blockCube);
                nd2nzParams.dstNzNStride = 1;
                nd2nzParams.dstNzMatrixStride = 0;
                AscendC::DataCopy(l1Local, srcGlobal[mIdx * n + nIdx], nd2nzParams);
                AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
            }
        }

        AscendC::PipeBarrier<PIPE_ALL>();
    }

private:
    static constexpr uint32_t AlignUp(uint32_t value, uint32_t align)
    {
        return (value + align - 1) / align * align;
    }

    static constexpr uint32_t blockCube = 16;
    static constexpr uint32_t c0Size = 32 / sizeof(half);
    static constexpr uint32_t l1BufferSize = AlignUp(tileM, blockCube) * AlignUp(tileN, c0Size);
    static constexpr uint64_t l1Addr = 0;

    AscendC::GlobalTensor<half> srcGlobal;
};

#endif
