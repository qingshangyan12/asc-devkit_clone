/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file l2_cache_kernel.h
 * \brief Workload parameters and device processing shared by both traversal orders.
 */

#ifndef ADD_L2_CACHE_SPLIT_L2_CACHE_KERNEL_H
#define ADD_L2_CACHE_SPLIT_L2_CACHE_KERNEL_H

#include "kernel_operator.h"
#include "tiling/platform/platform_ascendc.h"

namespace AddL2Cache {

constexpr uint32_t vectorCoreCount = 20;
constexpr uint32_t computeRoundCount = 2;

// The negative example scans the whole 384MiB range. The positive example splits it into L2-sized batches.
constexpr uint32_t totalByteSize = 384 * 1024 * 1024;
constexpr uint32_t L2_TILE_THRESHOLD = 100 * 1024 * 1024;
constexpr uint32_t l2BatchCount = (totalByteSize + L2_TILE_THRESHOLD - 1) / L2_TILE_THRESHOLD;
constexpr uint32_t l2BatchSizeBytes = totalByteSize / l2BatchCount;
constexpr uint32_t totalElementCount = totalByteSize / sizeof(half);
constexpr uint32_t l2BatchElementCount = l2BatchSizeBytes / sizeof(half);

constexpr uint32_t dataBlockSizeBytes = 32;
constexpr uint32_t elementsPerDataBlock = dataBlockSizeBytes / sizeof(half);

constexpr uint32_t MAX_DATA_COPY_LEN = 16384;
constexpr uint32_t syncInt32ElementsPerCore = dataBlockSizeBytes / sizeof(int32_t);
constexpr uint32_t logicalGmToUbBytesPerKernel = totalByteSize * computeRoundCount;
constexpr uint32_t logicalUbToGmBytesPerKernel = logicalGmToUbBytesPerKernel;

class KernelAdd {
public:
    __aicore__ inline void Init(__gm__ uint8_t* data, __gm__ uint8_t* sync)
    {
        dataGm_.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(data), totalElementCount);
        syncGm_.SetGlobalBuffer(
            reinterpret_cast<__gm__ int32_t*>(sync), vectorCoreCount * syncInt32ElementsPerCore);
        inputLocal_ =
            AscendC::LocalTensor<half>(AscendC::TPosition::VECIN, inputUbOffsetBytes, MAX_DATA_COPY_LEN);
        outputLocal_ =
            AscendC::LocalTensor<half>(AscendC::TPosition::VECOUT, outputUbOffsetBytes, MAX_DATA_COPY_LEN);
        syncLocal_ = AscendC::LocalTensor<int32_t>(
            AscendC::TPosition::VECCALC,
            syncUbOffsetBytes,
            vectorCoreCount * syncInt32ElementsPerCore);
    }

    __aicore__ inline void ProcessL2Batch(uint32_t l2BatchIdx)
    {
        ProcessDataRange(static_cast<int64_t>(l2BatchIdx) * l2BatchElementCount, l2BatchElementCount);
    }

    __aicore__ inline void ProcessWholeData()
    {
        ProcessDataRange(0, totalElementCount);
    }

    __aicore__ inline void SyncAllCores()
    {
        AscendC::SyncAll(syncGm_, syncLocal_, vectorCoreCount);
    }

private:
    static constexpr uint32_t inputUbOffsetBytes = 0;
    static constexpr uint32_t outputUbOffsetBytes = MAX_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t syncUbOffsetBytes = outputUbOffsetBytes + MAX_DATA_COPY_LEN * sizeof(half);

    __aicore__ inline void ProcessDataRange(int64_t rangeOffsetInData, uint32_t rangeElementNum)
    {
        const uint32_t coreIdx = AscendC::GetBlockIdx();
        const uint32_t rangeDataBlockNum = rangeElementNum / elementsPerDataBlock;
        const uint32_t baseDataBlocksPerCore = rangeDataBlockNum / vectorCoreCount;
        const uint32_t coresWithExtraDataBlock = rangeDataBlockNum % vectorCoreCount;
        const uint32_t extraDataBlockNumBeforeCore =
            coreIdx < coresWithExtraDataBlock ? coreIdx : coresWithExtraDataBlock;
        const uint32_t coreDataBlockNum =
            baseDataBlocksPerCore + (coreIdx < coresWithExtraDataBlock ? 1U : 0U);
        const uint32_t coreDataBlockOffset =
            coreIdx * baseDataBlocksPerCore + extraDataBlockNumBeforeCore;
        const uint32_t coreElementNum = coreDataBlockNum * elementsPerDataBlock;
        const int64_t coreOffsetInData =
            rangeOffsetInData + static_cast<int64_t>(coreDataBlockOffset) * elementsPerDataBlock;
        ProcessCoreRange(coreOffsetInData, coreElementNum);
    }

    __aicore__ inline void ProcessCoreRange(int64_t coreOffsetInData, uint32_t coreElementNum)
    {
        const uint32_t ubTileNum = AscendC::Ceil(coreElementNum, MAX_DATA_COPY_LEN);
        for (uint32_t ubTileIdx = 0; ubTileIdx < ubTileNum; ++ubTileIdx) {
            const uint32_t processedElementNum = ubTileIdx * MAX_DATA_COPY_LEN;
            const uint32_t remainingElementNum = coreElementNum - processedElementNum;
            const uint32_t tileElementNum = remainingElementNum > MAX_DATA_COPY_LEN ?
                                                MAX_DATA_COPY_LEN :
                                                remainingElementNum;
            const int64_t gmElementOffset = coreOffsetInData + processedElementNum;

            if (ubTileIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }
            AscendC::DataCopy(inputLocal_, dataGm_[gmElementOffset], tileElementNum);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            if (ubTileIdx != 0) {
                AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }
            AscendC::Adds(outputLocal_, inputLocal_, half(1.0), tileElementNum);
            if (ubTileIdx != ubTileNum - 1) {
                AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
            }

            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::DataCopy(dataGm_[gmElementOffset], outputLocal_, tileElementNum);
            if (ubTileIdx != ubTileNum - 1) {
                AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
            }
        }
    }

private:
    AscendC::GlobalTensor<half> dataGm_;
    AscendC::GlobalTensor<int32_t> syncGm_;
    AscendC::LocalTensor<half> inputLocal_;
    AscendC::LocalTensor<half> outputLocal_;
    AscendC::LocalTensor<int32_t> syncLocal_;
};

} // namespace AddL2Cache

#endif
