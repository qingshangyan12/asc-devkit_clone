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

    __aicore__ inline void Init(GM_ADDR src)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(src), m * n);
    }

    __aicore__ inline void Process()
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * singleCoreM;

        uint64_t ubAddr = 0;
        AscendC::LocalTensor<half> ubLocal(AscendC::TPosition::VECIN, ubAddr, ubBufferSize);

        constexpr uint32_t nLoopCount = (n + tileN - 1) / tileN;
        constexpr uint32_t mLoopCount = singleCoreM / tileM;
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
            uint32_t mIdx = mStart + mBlockIdx * tileM;
            for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                uint32_t nIdx = nBlockIdx * tileN;
                uint32_t curCols = (nIdx + tileN > n) ? (n - nIdx) : tileN;

                AscendC::DataCopyParams copyInParams{
                    static_cast<uint16_t>(tileM),
                    static_cast<uint16_t>(curCols * sizeof(half)),
                    static_cast<uint16_t>((n - curCols) * sizeof(half)),
                    0};

                AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nIdx], copyInParams, padParams);
            }
        }
    }

    __aicore__ inline void ProcessRepeatWhole4Times()
    {
        RepeatCopy(0, n, 4);
    }

    __aicore__ inline void ProcessSplitFourAndRepeat4Times()
    {
        static_assert(n % 4 == 0, "n must be divisible by 4");
        constexpr uint32_t quarterN = n / 4;

        for (uint32_t splitIdx = 0; splitIdx < 4; splitIdx++) {
            RepeatCopy(splitIdx * quarterN, quarterN, 4);
        }
    }

    __aicore__ inline void ProcessFullMatrixSameAddr()
    {
        FullMatrixCopy(false);
    }

    __aicore__ inline void ProcessFullMatrixOffsetAddr()
    {
        FullMatrixCopy(true);
    }

private:
    __aicore__ inline void FullMatrixCopy(bool offsetAddr)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        uint64_t ubAddr = 0;
        AscendC::LocalTensor<half> ubLocal(AscendC::TPosition::VECIN, ubAddr, ubBufferSize);

        constexpr uint32_t nLoopCount = n / tileN;
        constexpr uint32_t mTileCount = singleCoreM / tileM;
        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(tileM),
            static_cast<uint16_t>(tileN * sizeof(half)),
            static_cast<uint16_t>((n - tileN) * sizeof(half)),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t mBlockIdx = 0; mBlockIdx < numBlocks; mBlockIdx++) {
            uint32_t curMBlockIdx = offsetAddr ? (mBlockIdx + blockIdx) % numBlocks : mBlockIdx;
            uint32_t mStart = curMBlockIdx * singleCoreM;
            for (uint32_t mTileIdx = 0; mTileIdx < mTileCount; mTileIdx++) {
                uint32_t mIdx = mStart + mTileIdx * tileM;
                for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                    uint32_t nIdx = nBlockIdx * tileN;
                    AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nIdx], copyInParams, padParams);
                }
            }
        }
    }

    __aicore__ inline void RepeatCopy(uint32_t nStart, uint32_t nCount, uint32_t repeatTimes)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * singleCoreM;

        uint64_t ubAddr = 0;
        AscendC::LocalTensor<half> ubLocal(AscendC::TPosition::VECIN, ubAddr, ubBufferSize);

        uint32_t nLoopCount = nCount / tileN;
        constexpr uint32_t mLoopCount = singleCoreM / tileM;
        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(tileM),
            static_cast<uint16_t>(tileN * sizeof(half)),
            static_cast<uint16_t>((n - tileN) * sizeof(half)),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t repeatIdx = 0; repeatIdx < repeatTimes; repeatIdx++) {
            (void)repeatIdx;
            for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
                uint32_t mIdx = mStart + mBlockIdx * tileM;
                for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                    uint32_t nIdx = nBlockIdx * tileN;
                    AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nStart + nIdx], copyInParams, padParams);
                }
            }
        }
    }

    static constexpr uint32_t ubBufferSize = tileM * tileN;
    AscendC::GlobalTensor<half> srcGlobal;
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

    __aicore__ inline void ProcessRepeatWhole4Times()
    {
        RepeatCopy(0, n, 4);
    }

    __aicore__ inline void ProcessSplitFourAndRepeat4Times()
    {
        static_assert(n % 4 == 0, "n must be divisible by 4");
        constexpr uint32_t quarterN = n / 4;

        for (uint32_t splitIdx = 0; splitIdx < 4; splitIdx++) {
            RepeatCopy(splitIdx * quarterN, quarterN, 4);
        }
    }

    __aicore__ inline void ProcessFullMatrixSameAddr()
    {
        FullMatrixCopy(false);
    }

    __aicore__ inline void ProcessFullMatrixOffsetAddr()
    {
        FullMatrixCopy(true);
    }

private:
    __aicore__ inline void FullMatrixCopy(bool offsetAddr)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        AscendC::LocalTensor<half> l1Local(AscendC::TPosition::A1, l1Addr, l1BufferSize);

        constexpr uint32_t nLoopCount = n / tileN;
        constexpr uint32_t mTileCount = singleCoreM / tileM;

        AscendC::Nd2NzParams nd2nzParams;
        nd2nzParams.ndNum = 1;
        nd2nzParams.nValue = tileM;
        nd2nzParams.dValue = tileN;
        nd2nzParams.srcNdMatrixStride = 0;
        nd2nzParams.srcDValue = n;
        nd2nzParams.dstNzC0Stride = AlignUp(tileM, blockCube);
        nd2nzParams.dstNzNStride = 1;
        nd2nzParams.dstNzMatrixStride = 0;
                
        for (uint32_t mBlockIdx = 0; mBlockIdx < numBlocks; mBlockIdx++) {
            uint32_t curMBlockIdx = offsetAddr ? (mBlockIdx + blockIdx) % numBlocks : mBlockIdx;
            uint32_t mStart = curMBlockIdx * singleCoreM;
            for (uint32_t mTileIdx = 0; mTileIdx < mTileCount; mTileIdx++) {
                uint32_t mIdx = mStart + mTileIdx * tileM;
                for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                    uint32_t nIdx = nBlockIdx * tileN;

                    AscendC::DataCopy(l1Local, srcGlobal[mIdx * n + nIdx], nd2nzParams);
                    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                }
            }
        }

        AscendC::PipeBarrier<PIPE_ALL>();
    }

    __aicore__ inline void RepeatCopy(uint32_t nStart, uint32_t nCount, uint32_t repeatTimes)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();

        uint32_t mStart = blockIdx * singleCoreM;
        uint32_t mEnd = mStart + singleCoreM;

        AscendC::LocalTensor<half> l1Local(AscendC::TPosition::A1, l1Addr, l1BufferSize);

        AscendC::Nd2NzParams nd2nzParams;
        nd2nzParams.ndNum = 1;
        nd2nzParams.nValue = tileM;
        nd2nzParams.dValue = tileN;
        nd2nzParams.srcNdMatrixStride = 0;
        nd2nzParams.srcDValue = n;
        nd2nzParams.dstNzC0Stride = AlignUp(tileM, blockCube);
        nd2nzParams.dstNzNStride = 1;
        nd2nzParams.dstNzMatrixStride = 0;

        for (uint32_t repeatIdx = 0; repeatIdx < repeatTimes; repeatIdx++) {
            (void)repeatIdx;
            for (uint32_t mIdx = mStart; mIdx < mEnd; mIdx += tileM) {
                for (uint32_t nIdx = 0; nIdx < nCount; nIdx += tileN) {

                    AscendC::DataCopy(l1Local, srcGlobal[mIdx * n + nStart + nIdx], nd2nzParams);
                    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                }
            }
        }

        AscendC::PipeBarrier<PIPE_ALL>();
    }

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
