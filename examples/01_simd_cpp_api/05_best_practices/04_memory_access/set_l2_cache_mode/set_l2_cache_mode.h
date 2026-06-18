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
 * \file set_l2_cache_mode.h
 * \brief L2 Cache mode best practices – kernel implementations
 *
 * 本文件包含两个 kernel 类的实现：
 * 1. KernelDataCopyPadGm2Ub – 抽取自 data_copy_ub.h（GM→UB DataCopy 搬运）
 *    包含 Case 1（分片重复+L2开）、Case 2（分片重复+L2关）、Case 3（整块重复+L2开）
 * 2. KernelAdd – 抽取自 add.asc（矩阵加法）
 *    包含 Case 4（双缓冲+L2开）、Case 5（双缓冲+L2 bypass）
 */

#ifndef SET_L2_CACHE_MODE_H
#define SET_L2_CACHE_MODE_H

#include "kernel_operator.h"

// ====================================================================
// 第一部分：KernelDataCopyPadGm2Ub（抽取自 data_copy_ub.h）
// 来源：examples\01_simd_cpp_api\05_best_practices\04_memory_access\data_copy\data_copy_ub.h
// 仅在末尾新增 ProcessSplitFourAndRepeat4TimesL2Disable() 用于 Case 2
// ====================================================================

template <uint32_t m, uint32_t n, uint32_t numBlocks, uint32_t singleCoreM, uint32_t tileM, uint32_t tileN>
class KernelDataCopyPadGm2Ub {
public:
    __aicore__ inline KernelDataCopyPadGm2Ub() {}

    __aicore__ inline void Init(__gm__ half* src)
    {
        srcGlobal.SetGlobalBuffer(src, m * n);
        ubLocal = AscendC::LocalTensor<half>(AscendC::TPosition::VECIN, ubAddr, ubBufferSize);
    }

    __aicore__ inline void Process()
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * singleCoreM;

        // 按 tile 粒度切分 M/N 方向，便于比较不同分块大小的搬运效率。
        constexpr uint32_t nLoopCount = (n + tileN - 1) / tileN;
        constexpr uint32_t mLoopCount = singleCoreM / tileM;
        AscendC::DataCopyParams copyInParams;
        copyInParams.blockCount = static_cast<uint16_t>(tileM);
        copyInParams.dstStride = 0;
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
            uint32_t mIdx = mStart + mBlockIdx * tileM;
            // N 方向按 tileN 切分，尾块场景中最后一次 curCols 小于 tileN。
            for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                uint32_t nIdx = nBlockIdx * tileN;
                uint32_t curCols = (nIdx + tileN > n) ? (n - nIdx) : tileN;

                copyInParams.blockLen = static_cast<uint16_t>(curCols * sizeof(half));
                copyInParams.srcStride = static_cast<uint16_t>((n - curCols) * sizeof(half));
                AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nIdx], copyInParams, padParams);
            }
        }
    }

    __aicore__ inline void ProcessRepeatWhole4Times()
    {
        // 场景 5：整块矩阵沿相同路径连续重复 4 次，用于观察整块重复访问。
        RepeatCopy(0, n, 4);
    }

    __aicore__ inline void ProcessSplitFourAndRepeat4Times()
    {
        static_assert(n % 4 == 0, "n must be divisible by 4");
        constexpr uint32_t quarterN = n / 4;

        // 场景 6：先按 N 方向切成 4 份，每份连续搬 4 次，便于观察分片后的 L2Cache 复用。
        for (uint32_t splitIdx = 0; splitIdx < 4; splitIdx++) {
            RepeatCopy(splitIdx * quarterN, quarterN, 4);
        }
    }

    // [新增] Case 2：分片重复搬运 + L2 Cache 关闭
    // 与 ProcessSplitFourAndRepeat4Times 搬运模式完全相同，唯一区别是关闭了 L2 Cache。
    // 设计意图：与 Case 1 对比，展示对于重复访问的数据，关闭 L2 会导致每次都要从 GM 重新读取，
    // 无法享受 L2 复用的加速效果。
    __aicore__ inline void ProcessSplitFourAndRepeat4TimesL2Disable()
    {
        srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        ProcessSplitFourAndRepeat4Times();
    }

    __aicore__ inline void ProcessFullMatrixSameAddr()
    {
        // 场景 7：所有核按相同顺序访问同一块输入矩阵，容易产生同地址访问冲突。
        FullMatrixCopy(false);
    }

    __aicore__ inline void ProcessFullMatrixOffsetAddr()
    {
        // 场景 8：所有核按块组内轮转顺序访问，和场景 7 对比冲突规避效果。
        FullMatrixCopy(true);
    }

private:
    __aicore__ inline void FullMatrixCopy(bool offsetAddr)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        // 关闭 L2Cache hint，避免把当前场景的访问顺序和缓存策略混在一起。
        srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);

        constexpr uint32_t fullMBlockCount = m / singleCoreM;
        constexpr uint32_t nLoopCount = n / tileN;
        constexpr uint32_t mTileCount = singleCoreM / tileM;
        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(tileM),
            static_cast<uint16_t>(tileN * sizeof(half)),
            static_cast<uint16_t>((n - tileN) * sizeof(half)),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t mBlockIdx = 0; mBlockIdx < fullMBlockCount; mBlockIdx++) {
            // M 方向先按 numBlocks 切成大块，offsetAddr 为 true 时在每组 numBlocks 个大块内轮转访问顺序。
            uint32_t blockGroupStart = (mBlockIdx / numBlocks) * numBlocks;
            uint32_t curMBlockIdx = offsetAddr ? blockGroupStart + (mBlockIdx + blockIdx) % numBlocks : mBlockIdx;
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

        uint32_t nLoopCount = nCount / tileN;
        constexpr uint32_t mLoopCount = singleCoreM / tileM;
        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(tileM),
            static_cast<uint16_t>(tileN * sizeof(half)),
            static_cast<uint16_t>((n - tileN) * sizeof(half)),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t repeatIdx = 0; repeatIdx < repeatTimes; repeatIdx++) {
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
    static constexpr uint64_t ubAddr = 0;
    AscendC::GlobalTensor<half> srcGlobal;
    AscendC::LocalTensor<half> ubLocal;
};


// ====================================================================
// 第二部分：KernelAdd（抽取自 add.asc）
// 来源：examples\01_simd_cpp_api\05_best_practices\00_vector_compute\add_high_performance\add.asc
// ====================================================================

template <uint32_t totalM, uint32_t totalN, uint32_t splitM, uint32_t splitN, uint32_t dataCopyLen>
class KernelAdd {
public:
    __aicore__ inline KernelAdd() = default;
    __aicore__ inline void Init(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
    {
        static_assert(splitN == 1, "This optimized sample expects splitN == 1 for contiguous row-wise DataCopy.");
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t blockIdxM = blockIdx;
        constexpr uint32_t baseCoreM = totalM / splitM;
        constexpr uint32_t remainderM = totalM % splitM;
        uint32_t actualCoreM;
        uint32_t startM;
        if (blockIdxM < remainderM) {
            actualCoreM = baseCoreM + 1;
            startM = blockIdxM * actualCoreM;
        } else {
            actualCoreM = baseCoreM;
            startM = remainderM * (baseCoreM + 1) + (blockIdxM - remainderM) * baseCoreM;
        }
        this->singleCoreM = actualCoreM;
        this->singleCoreN = totalN;
        this->totalElementsPerCore = this->singleCoreM * this->singleCoreN;
        xGm.SetGlobalBuffer((__gm__ half*)x + startM * totalN, this->totalElementsPerCore);
        yGm.SetGlobalBuffer((__gm__ half*)y + startM * totalN, this->totalElementsPerCore);
        zGm.SetGlobalBuffer((__gm__ half*)z + startM * totalN, this->totalElementsPerCore);
    }

    __aicore__ inline void Process()
    {
        // VECCALC 在硬件上映射为 UB
        AscendC::LocalTensor<half> xLocal(AscendC::TPosition::VECCALC, xAddr, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yLocal(AscendC::TPosition::VECCALC, yAddr, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zLocal(AscendC::TPosition::VECCALC, zAddr, MAX_DATA_COPY_LEN);

        uint32_t totalBlocks = GetTotalBlocks();

        for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = totalElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            if (loopIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }

            AscendC::DataCopy(xLocal, xGm[startElement], curLen);
            AscendC::DataCopy(yLocal, yGm[startElement], curLen);

            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            if (loopIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }

            AscendC::Add(zLocal, xLocal, yLocal, curLen);

            if (loopIdx != (totalBlocks - 1)) {
                AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

            AscendC::DataCopy(zGm[startElement], zLocal, curLen);

            if (loopIdx != (totalBlocks - 1)) {
                AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }
        }
    }

    __aicore__ inline void ProcessScalar()
    {
        // VECCALC 在硬件上映射为 UB
        AscendC::LocalTensor<half> xLocal(AscendC::TPosition::VECCALC, xAddr, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yLocal(AscendC::TPosition::VECCALC, yAddr, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zLocal(AscendC::TPosition::VECCALC, zAddr, MAX_DATA_COPY_LEN);

        uint32_t totalLoop = GetTotalBlocks();

        for (uint32_t loopIdx = 0; loopIdx < totalLoop; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = totalElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            if (loopIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }

            AscendC::DataCopy(xLocal, xGm[startElement], curLen);
            AscendC::DataCopy(yLocal, yGm[startElement], curLen);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            if (loopIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }

            for (uint32_t i = 0; i < curLen; i++) {
                float xVal = (float)xLocal.GetValue(i);
                float yVal = (float)yLocal.GetValue(i);
                zLocal.SetValue(i, (half)(xVal + yVal));
            }

            if (loopIdx != (totalLoop - 1)) {
                AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

            AscendC::DataCopy(zGm[startElement], zLocal, curLen);
            if (loopIdx != (totalLoop - 1)) {
                AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }
        }
    }

    __aicore__ inline void ProcessSingle()
    {
        // VECCALC 在硬件上映射为 UB
        AscendC::LocalTensor<half> xLocal(AscendC::TPosition::VECCALC, xAddr, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yLocal(AscendC::TPosition::VECCALC, yAddr, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zLocal(AscendC::TPosition::VECCALC, zAddr, MAX_DATA_COPY_LEN);

        uint32_t totalLoop = GetTotalBlocks();

        for (uint32_t loopIdx = 0; loopIdx < totalLoop; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = totalElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            if (loopIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }

            AscendC::DataCopy(xLocal, xGm[startElement], curLen);
            AscendC::DataCopy(yLocal, yGm[startElement], curLen);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            if (loopIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }

            AscendC::Add(zLocal, xLocal, yLocal, curLen);

            if (loopIdx != (totalLoop - 1)) {
                AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

            AscendC::DataCopy(zGm[startElement], zLocal, curLen);
            if (loopIdx != (totalLoop - 1)) {
                AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }
        }
    }

    __aicore__ inline void ProcessDoubleBuffer()
    {
        // VECCALC 在硬件上映射为 UB
        AscendC::LocalTensor<half> xPing(AscendC::TPosition::VECCALC, xAddrPing, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yPing(AscendC::TPosition::VECCALC, yAddrPing, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zPing(AscendC::TPosition::VECCALC, zAddrPing, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> xPong(AscendC::TPosition::VECCALC, xAddrPong, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yPong(AscendC::TPosition::VECCALC, yAddrPong, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zPong(AscendC::TPosition::VECCALC, zAddrPong, MAX_DATA_COPY_LEN);

        uint32_t totalBlocks = GetTotalBlocks();

        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);

        for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = totalElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            int32_t eventID = ((loopIdx & 1) == 0 ? EVENT_ID0 : EVENT_ID1);
            AscendC::LocalTensor<half>& xLocal = ((loopIdx & 1) == 0 ? xPing : xPong);
            AscendC::LocalTensor<half>& yLocal = ((loopIdx & 1) == 0 ? yPing : yPong);
            AscendC::LocalTensor<half>& zLocal = ((loopIdx & 1) == 0 ? zPing : zPong);

            AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(eventID);

            AscendC::DataCopy(xLocal, xGm[startElement], curLen);
            AscendC::DataCopy(yLocal, yGm[startElement], curLen);

            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(eventID);

            AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(eventID);

            AscendC::Add(zLocal, xLocal, yLocal, curLen);

            AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(eventID);

            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(eventID);

            AscendC::DataCopy(zGm[startElement], zLocal, curLen);

            AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(eventID);
        }

        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);
    }

    __aicore__ inline void ProcessDoubleBufferL2Bypass()
    {
        xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        ProcessDoubleBuffer();
    }

    __aicore__ inline void ProcessDoubleBufferBankConflict()
    {
        xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);

        // VECCALC 在硬件上映射为 UB
        AscendC::LocalTensor<half> xPing(AscendC::TPosition::VECCALC, xAddrPingBC, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yPing(AscendC::TPosition::VECCALC, yAddrPingBC, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zPing(AscendC::TPosition::VECCALC, zAddrPingBC, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> xPong(AscendC::TPosition::VECCALC, xAddrPongBC, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yPong(AscendC::TPosition::VECCALC, yAddrPongBC, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zPong(AscendC::TPosition::VECCALC, zAddrPongBC, MAX_DATA_COPY_LEN);

        uint32_t totalBlocks = GetTotalBlocks();

        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);

        for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = totalElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            int32_t eventID = ((loopIdx & 1) == 0 ? EVENT_ID0 : EVENT_ID1);
            AscendC::LocalTensor<half>& xLocal = ((loopIdx & 1) == 0 ? xPing : xPong);
            AscendC::LocalTensor<half>& yLocal = ((loopIdx & 1) == 0 ? yPing : yPong);
            AscendC::LocalTensor<half>& zLocal = ((loopIdx & 1) == 0 ? zPing : zPong);

            AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(eventID);

            AscendC::DataCopy(xLocal, xGm[startElement], curLen);
            AscendC::DataCopy(yLocal, yGm[startElement], curLen);

            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(eventID);

            AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(eventID);

            AscendC::Add(zLocal, xLocal, yLocal, curLen);

            AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(eventID);

            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(eventID);

            AscendC::DataCopy(zGm[startElement], zLocal, curLen);

            AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(eventID);
        }

        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);
    }

private:
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    static constexpr uint32_t MAX_DATA_COPY_LEN = 21840;
    static constexpr uint32_t BANK_CONFLICT_DATA_COPY_LEN = 21632;
#else
    static constexpr uint32_t MAX_DATA_COPY_LEN = 16384;
    static constexpr uint32_t BANK_CONFLICT_DATA_COPY_LEN = 16256;
#endif

    __aicore__ inline uint32_t GetTotalBlocks() const { return AscendC::DivCeil(totalElementsPerCore, dataCopyLen); }

    static constexpr uint32_t xAddr = 0;
    static constexpr uint32_t yAddr = MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddr = yAddr + MAX_DATA_COPY_LEN * sizeof(half);

    static constexpr uint32_t xAddrPing = 0;
    static constexpr uint32_t yAddrPing = MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddrPing = yAddrPing + MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t xAddrPong = zAddrPing + MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t yAddrPong = xAddrPong + MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddrPong = yAddrPong + MAX_DATA_COPY_LEN * sizeof(half);

    static constexpr uint32_t xAddrPingBC = 0;
    static constexpr uint32_t yAddrPingBC = BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t xAddrPongBC = MAX_DATA_COPY_LEN * sizeof(half) * 2;
    static constexpr uint32_t yAddrPongBC = xAddrPongBC + BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddrPingBC = MAX_DATA_COPY_LEN * sizeof(half) * 4;
    static constexpr uint32_t zAddrPongBC = zAddrPingBC + BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);

    AscendC::GlobalTensor<half> xGm;
    AscendC::GlobalTensor<half> yGm;
    AscendC::GlobalTensor<half> zGm;
    uint32_t singleCoreM;
    uint32_t singleCoreN;
    uint32_t totalElementsPerCore;
};


// ====================================================================
// 第三部分：KernelRepeatAndAdd（Case 6-7 新增）
// 将 Case 1 的分片重复搬运和 Case 4/5 的双缓冲 Add 合并到同一个 Process() 函数中。
// Add 矩阵：[addM, addN]（与 Case 4/5 一致，[8192, 8192]）
// Repeat 矩阵：[repeatM, repeatN]（与 Case 1 一致，[12288, 12288]）
//
// 模板参数 addL2Bypass 控制 Add 阶段的 L2 Cache 配置：
//   false: Case 6 — Add L2 默认开启，流式数据写入 L2，然后 Repeat L2 启用
//   true:  Case 7 — Add L2 bypass，流式数据不污染 L2，然后 Repeat L2 启用
// ====================================================================

template <uint32_t addM, uint32_t addN, uint32_t repeatM, uint32_t repeatN, uint32_t splitM,
    uint32_t dataCopyLen, uint32_t repeatTileM, uint32_t repeatTileN, bool addL2Bypass>
class KernelRepeatAndAdd {
public:
    __aicore__ inline KernelRepeatAndAdd() = default;

    /* !
     * \brief 初始化：Add 矩阵 [addM, addN]，Repeat 矩阵 [repeatM, repeatN]，分别切分
     */
    __aicore__ inline void Init(__gm__ uint8_t* addX, __gm__ uint8_t* addY,
                                __gm__ uint8_t* repeatSrc, __gm__ uint8_t* addZ)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();

        // ---- Add 阶段的切分（基于 addM）----
        constexpr uint32_t addBaseM = addM / splitM;
        constexpr uint32_t addRemainder = addM % splitM;
        uint32_t addActualM;
        uint32_t addStartM;
        if (blockIdx < addRemainder) {
            addActualM = addBaseM + 1;
            addStartM = blockIdx * addActualM;
        } else {
            addActualM = addBaseM;
            addStartM = addRemainder * (addBaseM + 1) + (blockIdx - addRemainder) * addBaseM;
        }
        this->addElementsPerCore = addActualM * addN;
        addXGm.SetGlobalBuffer((__gm__ half*)addX + addStartM * addN, this->addElementsPerCore);
        addYGm.SetGlobalBuffer((__gm__ half*)addY + addStartM * addN, this->addElementsPerCore);
        addZGm.SetGlobalBuffer((__gm__ half*)addZ + addStartM * addN, this->addElementsPerCore);

        // ---- Repeat 阶段的切分（基于 repeatM）----
        constexpr uint32_t repBaseM = repeatM / splitM;
        constexpr uint32_t repRemainder = repeatM % splitM;
        uint32_t repActualM;
        uint32_t repStartM;
        if (blockIdx < repRemainder) {
            repActualM = repBaseM + 1;
            repStartM = blockIdx * repActualM;
        } else {
            repActualM = repBaseM;
            repStartM = repRemainder * (repBaseM + 1) + (blockIdx - repRemainder) * repBaseM;
        }
        this->repeatCoreM = repActualM;
        repeatGm.SetGlobalBuffer((__gm__ half*)repeatSrc + repStartM * repeatN, repActualM * repeatN);
    }

    /* !
     * \brief Case 6/7 的合并 Process：Phase 1 Add（双缓冲） + Phase 2 Repeat（分片重复 4 次）
     */
    __aicore__ inline void Process()
    {
        // ---- Phase 1: Add 双缓冲 [addM, addN]（抽取自 KernelAdd::ProcessDoubleBuffer）----
        AscendC::LocalTensor<half> xPing(AscendC::TPosition::VECCALC, xAddrPing, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yPing(AscendC::TPosition::VECCALC, yAddrPing, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zPing(AscendC::TPosition::VECCALC, zAddrPing, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> xPong(AscendC::TPosition::VECCALC, xAddrPong, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> yPong(AscendC::TPosition::VECCALC, yAddrPong, MAX_DATA_COPY_LEN);
        AscendC::LocalTensor<half> zPong(AscendC::TPosition::VECCALC, zAddrPong, MAX_DATA_COPY_LEN);

        if constexpr (addL2Bypass) {
            addXGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
            addYGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        }

        uint32_t totalBlocks = AscendC::DivCeil(addElementsPerCore, dataCopyLen);
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);

        for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = addElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            int32_t eventID = ((loopIdx & 1) == 0 ? EVENT_ID0 : EVENT_ID1);
            AscendC::LocalTensor<half>& xLocal = ((loopIdx & 1) == 0 ? xPing : xPong);
            AscendC::LocalTensor<half>& yLocal = ((loopIdx & 1) == 0 ? yPing : yPong);
            AscendC::LocalTensor<half>& zLocal = ((loopIdx & 1) == 0 ? zPing : zPong);

            AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(eventID);
            AscendC::DataCopy(xLocal, addXGm[startElement], curLen);
            AscendC::DataCopy(yLocal, addYGm[startElement], curLen);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(eventID);
            AscendC::Add(zLocal, xLocal, yLocal, curLen);
            AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(eventID);
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(eventID);
            AscendC::DataCopy(addZGm[startElement], zLocal, curLen);
            AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(eventID);
        }
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);

        // ---- Phase 2: Repeat 分片重复搬运 4 次（抽取自 KernelDataCopyPadGm2Ub）----
        AscendC::LocalTensor<half> ubLocal(AscendC::TPosition::VECIN, repeatUbAddr, repeatUbBufferSize);

        constexpr uint32_t quarterN = repeatN / 4;
        uint32_t mStart = 0;   // repeatGm 在 Init 中已偏移到当前核的分片起始地址
        uint32_t nLoopCount = quarterN / repeatTileN;
        const uint32_t mLoopCount = repeatCoreM / repeatTileM;
        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(repeatTileM),
            static_cast<uint16_t>(repeatTileN * sizeof(half)),
            static_cast<uint16_t>((repeatN - repeatTileN) * sizeof(half)),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        for (uint32_t splitIdx = 0; splitIdx < 4; splitIdx++) {
            uint32_t nStart = splitIdx * quarterN;
            for (uint32_t repeatIdx = 0; repeatIdx < 4; repeatIdx++) {
                for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
                    uint32_t mIdx = mBlockIdx * repeatTileM;  // repeatGm 已偏移，从 0 开始
                    for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                        uint32_t nIdx = nBlockIdx * repeatTileN;
                        AscendC::DataCopyPad(
                            ubLocal, repeatGm[mIdx * repeatN + nStart + nIdx], copyInParams, padParams);
                    }
                }
            }
        }
    }

private:
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    static constexpr uint32_t MAX_DATA_COPY_LEN = 21840;
#else
    static constexpr uint32_t MAX_DATA_COPY_LEN = 16384;
#endif

    // Add 双缓冲 UB 地址布局
    static constexpr uint32_t xAddrPing = 0;
    static constexpr uint32_t yAddrPing = MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddrPing = yAddrPing + MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t xAddrPong = zAddrPing + MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t yAddrPong = xAddrPong + MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddrPong = yAddrPong + MAX_DATA_COPY_LEN * sizeof(half);

    // Repeat 阶段复用 UB 空间
    static constexpr uint32_t repeatUbAddr = 0;
    static constexpr uint32_t repeatUbBufferSize = repeatTileM * repeatTileN;

    AscendC::GlobalTensor<half> addXGm;       // Add 输入 x
    AscendC::GlobalTensor<half> addYGm;       // Add 输入 y
    AscendC::GlobalTensor<half> addZGm;       // Add 输出 z
    AscendC::GlobalTensor<half> repeatGm;     // Repeat 输入
    uint32_t addElementsPerCore;              // Add 阶段当前核元素数
    uint32_t repeatCoreM;                     // Repeat 阶段当前核行数
};

#endif // SET_L2_CACHE_MODE_H
