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
 * \file hccl_ccu_v0_prepare.h
 * \brief
 */
#ifndef IMPL_HCCL_CCU_V0_PREEPARE_H
#define IMPL_HCCL_CCU_V0_PREEPARE_H

#include "hccl_ccu_xn_utils.h"
#include "../impl/platform_v310/hccl_ccu_v0_def.h"

namespace AscendC {
template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllV(
    __gm__ CommonPrepareParamCcu* commParam, __gm__ AlltoAllVParamCcu* allToAllVParam)
{
    AscendC::PRINTF(
        "[CcuPrepareForAllToAllV] enter repeatIndex=%u, rankId=%u, rankNum=%u, count=%llu, dataType=%u\n",
        static_cast<uint32_t>(ccuParam_.repeatIndex), static_cast<uint32_t>(ccuParam_.rankId),
        static_cast<uint32_t>(ccuParam_.rankNum), commParam->count, static_cast<uint32_t>(commParam->dataType));
    xnData_[0] = GetOpId(commParam); // ccu xn0
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * DATA_TYPE_MAP[commParam->dataType];
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2
    xnData_[3] = 0;                                     // 3 is index of xnData
    xnData_[4] = 0;                                     // 4 is index of xnData
    // 按照卡分组，sendSize 、sendOffset、recvSize、recvOffset  以字节为单位 * DataSzie(DataType)
    // ccu xn5
    AssembleHcclMsgExtForCCU(ccuParam_, commParam, allToAllVParam);

    uint64_t loopCount = 8;
    auto dataSlice = ((allToAllVParam->sendCounts[ccuParam_.rankId]) * DATA_TYPE_MAP[commParam->dataType]) %
        CCU_MAX_COMM_DATA;
    CalcGoSize(dataSlice, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[5]);
    xnData_[9] = reinterpret_cast<uint64_t>(ccuParam_.ccuMsgExt) + CCU_MSG_EXT_RANK_OFFSET * ccuParam_.alltoallvCnt++;
    AscendC::PRINTF(
        "[CcuPrepareForAllToAllV] offset=%llu, dataSlice=%llu, xn1=0x%llx, xn2=0x%llx, xn9=0x%llx\n", offset,
        dataSlice, xnData_[1], xnData_[2], xnData_[9]);
    return;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAll(
    __gm__ CommonPrepareParamCcu* commParam)
{
    AscendC::PRINTF(
        "[CcuPrepareForAllToAll] enter repeatIndex=%u, rankId=%u, count=%llu, strideCount=%llu, dataType=%u\n",
        static_cast<uint32_t>(ccuParam_.repeatIndex), static_cast<uint32_t>(ccuParam_.rankId), commParam->count,
        commParam->strideCount, static_cast<uint32_t>(commParam->dataType));
    uint64_t dataSize = DATA_TYPE_MAP[commParam->dataType];
    xnData_[0] = GetOpId(commParam); // ccu xn0
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2
    uint64_t sliceSizeAlltoall = commParam->count * dataSize;
    uint64_t strideSize = commParam->strideCount == 0 ? sliceSizeAlltoall : commParam->strideCount * dataSize;
    xnData_[3] = sliceSizeAlltoall;
    xnData_[4] = strideSize;
    xnData_[5] = 0;
    xnData_[6] = strideSize * ccuParam_.rankId;
    uint64_t loopCount = 8;
    CalcGoSize(sliceSizeAlltoall, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[7]);
    AscendC::PRINTF(
        "[CcuPrepareForAllToAll] offset=%llu, sliceSize=%llu, strideSize=%llu, xn1=0x%llx, xn2=0x%llx, xn6=%llu\n",
        offset, sliceSizeAlltoall, strideSize, xnData_[1], xnData_[2], xnData_[6]);
    return;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllVWrite(
    __gm__ CommonPrepareParamCcu* commParam)
{
    AscendC::PRINTF(
        "[CcuPrepareForAllToAllVWrite] enter repeatIndex=%u, count=%llu, sendOffsets=0x%llx, sendSizes=0x%llx, "
        "remoteWinOffset=%llu\n",
        static_cast<uint32_t>(ccuParam_.repeatIndex), commParam->count, commParam->wParamExt.sendOffsets,
        commParam->wParamExt.sendSizes, commParam->wParamExt.remoteWinOffset);
    xnData_[0] = GetOpId(commParam); // ccu xn0
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * DATA_TYPE_MAP[commParam->dataType];
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t loopCount = CCU_LOOP_COUNT_ATAVW;

    xnData_[1] = reinterpret_cast<uint64_t>(commParam->sendBuf); // 1 is index of xnData
    xnData_[2] = commParam->wParamExt.sendSizes;                 // 2 is index of xnData
    xnData_[8] = commParam->wParamExt.remoteWinOffset;           // 8 is index of xnData
    uint64_t sliceSize = commParam->count;

    uint64_t loopSize = loopCount * CCU_MEMSLICE_SIZE;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / CCU_MEMSLICE_SIZE;
    uint64_t p = sliceSize - m * loopSize - n * CCU_MEMSLICE_SIZE;

    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];

    xnData_[3] = commParam->wParamExt.sendOffsets; // 3 is index of xnData
    xnData_[4] = loopSize * m;                     // 4 is index of xnData
    xnData_[5] = m;                                // 5 is index of xnData
    CalcLoopGroupParam(xnData_, m, n, p);
    AscendC::PRINTF(
        "[CcuPrepareForAllToAllVWrite] sliceSize=%llu, loopSize=%llu, m=%llu, n=%llu, p=%llu, xn8=%llu\n",
        sliceSize, loopSize, m, n, p, xnData_[8]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllReduce(
    __gm__ CommonPrepareParamCcu* commParam)
{
    xnData_[0] = GetOpId(commParam); // ccu xn0
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * DATA_TYPE_MAP[commParam->dataType];
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t tmpCount = commParam->count / ccuParam_.rankNum;
    uint64_t loopCount = CCU_LOOP_COUNT;
    uint64_t sliceCount = (ccuParam_.rankId == ccuParam_.rankNum - 1) ?
                              (commParam->count - (ccuParam_.rankNum - 1) * tmpCount) :
                              tmpCount;
    uint64_t sliceSize = sliceCount * DATA_TYPE_MAP[commParam->dataType];

    uint64_t loopSize = loopCount * CCU_MEMSLICE_SIZE;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / CCU_MEMSLICE_SIZE;
    uint64_t p = sliceSize - m * loopSize - n * CCU_MEMSLICE_SIZE;

    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    xnData_[3] = (commParam->strideCount == 0) ?
                     tmpCount * dataSize * ccuParam_.rankId :
                     (commParam->strideCount * dataSize * ccuParam_.rankId); // 3 is index of xnData
    xnData_[4] = loopSize * m;                                               // 4 is index of xnData
    xnData_[5] = m;                                                          // 5 is index of xnData
    CalcLoopGroupParam(xnData_, m, n, p);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllGather(
    __gm__ CommonPrepareParamCcu* commParam)
{
    xnData_[0] = GetOpId(commParam); // ccu xn0
    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t tmpCount = commParam->count / ccuParam_.rankNum;
    uint64_t loopCount = CCU_LOOP_COUNT;
    uint64_t sliceCount = commParam->count;

    uint64_t sliceSize = sliceCount * dataSize;

    uint64_t loopSize = loopCount * CCU_MEMSLICE_SIZE;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / CCU_MEMSLICE_SIZE;
    uint64_t p = sliceSize - m * loopSize - n * CCU_MEMSLICE_SIZE;

    xnData_[3] = (commParam->strideCount == 0) ?
                     sliceSize * ccuParam_.rankId :
                     (commParam->strideCount * dataSize * ccuParam_.rankId); // 3 is index of xnData
    xnData_[4] = loopSize * m;                                               // 4 is index of xnData
    xnData_[5] = m;                                                          // 5 is index of xnData
    CalcLoopGroupParam(xnData_, m, n, p);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForReduceScatter(
    __gm__ CommonPrepareParamCcu* commParam)
{
    CcuPrepareForAllGather(commParam);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllReduceM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    AscendC::PRINTF(
        "[CcuPrepareForAllReduceM2M] enter repeatIndex=%u, rankId=%u, rankNum=%u, count=%llu, strideCount=%llu, "
        "dataType=%u\n",
        static_cast<uint32_t>(ccuParam_.repeatIndex), static_cast<uint32_t>(ccuParam_.rankId),
        static_cast<uint32_t>(ccuParam_.rankNum), commParam->count, commParam->strideCount,
        static_cast<uint32_t>(commParam->dataType));
    xnData_[0] = GetOpId(commParam); // ccu xn0
    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t loopCount = CCU_LOOP_COUNT_M2M_RE;
    uint64_t tmpCount = (commParam->count + ccuParam_.rankNum - 1) / ccuParam_.rankNum;
    uint64_t sliceCount = (ccuParam_.rankId == ccuParam_.rankNum - 1) ?
                              (commParam->count - (ccuParam_.rankNum - 1) * tmpCount) :
                              tmpCount;
    uint64_t sliceSize = sliceCount * DATA_TYPE_MAP[commParam->dataType];

    xnData_[3] = ccuParam_.scratchAddr;
    uint64_t rankSliceOffset =
        ccuParam_.rankId * ((commParam->strideCount == 0) ? sliceSize : (commParam->strideCount * dataSize));
    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForAllReduceM2M scratchAddr:0x%llx, rankSliceOffset:%d",
        ccuParam_.scratchAddr, rankSliceOffset);
    xnData_[4] = rankSliceOffset;
    xnData_[5] = rankSliceOffset;

    uint64_t normalSliceCount =
        (commParam->count + ccuParam_.rankNum - 1) / ccuParam_.rankNum; // count/rankNum 向上取整
    uint64_t normalSliceSize = normalSliceCount * dataSize;
    uint64_t normalRankCount = commParam->count / normalSliceCount;
    uint64_t lastSliceSize = 0;
    if (normalRankCount == ccuParam_.rankNum) {
        lastSliceSize = (commParam->count * dataSize) - ((normalRankCount - 1) * normalSliceSize);
    } else {
        lastSliceSize = (commParam->count * dataSize) - (normalRankCount * normalSliceSize);
    }
    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForAllReduceM2M normalSliceSize:%d, lastSliceSize:%d", normalSliceSize,
        lastSliceSize);

    xnData_[6] = normalSliceSize;
    xnData_[7] = lastSliceSize;
    xnData_[8] = ccuParam_.rankId < normalRankCount ? normalSliceSize : lastSliceSize;
    xnData_[9] = ccuParam_.rankId * normalSliceSize;
    xnData_[10] = 0; // input output not equals
    if (ccuParam_.rankId == ccuParam_.rankNum - 1) {
        CalcGoSize(lastSliceSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[11]);
    } else {
        CalcGoSize(normalSliceSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[11]);
    }
    AscendC::PRINTF(
        "[CcuPrepareForAllReduceM2M] offset=%llu, sliceCount=%llu, sliceSize=%llu, normalSliceSize=%llu, "
        "lastSliceSize=%llu, xn3=0x%llx, xn4=%llu, xn5=%llu\n",
        offset, sliceCount, sliceSize, normalSliceSize, lastSliceSize, xnData_[3], xnData_[4], xnData_[5]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllGatherM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    AscendC::PRINTF(
        "[CcuPrepareForAllGatherM2M] enter repeatIndex=%u, rankId=%u, count=%llu, strideCount=%llu, dataType=%u\n",
        static_cast<uint32_t>(ccuParam_.repeatIndex), static_cast<uint32_t>(ccuParam_.rankId), commParam->count,
        commParam->strideCount, static_cast<uint32_t>(commParam->dataType));
    xnData_[0] = GetOpId(commParam); // ccu xn0
    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t loopCount = CCU_LOOP_COUNT_M2M_AG;
    uint64_t sliceCount = commParam->count;
    uint64_t sliceSize = sliceCount * dataSize;

    xnData_[3] = ccuParam_.rankId * ((commParam->strideCount == 0) ? sliceSize : (commParam->strideCount * dataSize));
    xnData_[4] = sliceSize;
    CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[5]);
    AscendC::PRINTF(
        "[CcuPrepareForAllGatherM2M] offset=%llu, sliceSize=%llu, xn1=0x%llx, xn2=0x%llx, xn3=%llu, xn4=%llu\n",
        offset, sliceSize, xnData_[1], xnData_[2], xnData_[3], xnData_[4]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForReduceScatterM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    AscendC::PRINTF(
        "[CcuPrepareForReduceScatterM2M] enter repeatIndex=%u, rankId=%u, count=%llu, strideCount=%llu, "
        "dataType=%u\n",
        static_cast<uint32_t>(ccuParam_.repeatIndex), static_cast<uint32_t>(ccuParam_.rankId), commParam->count,
        commParam->strideCount, static_cast<uint32_t>(commParam->dataType));
    xnData_[0] = GetOpId(commParam); // ccu xn0
    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t loopCount = CCU_LOOP_COUNT_M2M_RE;
    uint64_t sliceCount = commParam->count;
    uint64_t sliceSize = sliceCount * dataSize;

    xnData_[3] = ccuParam_.scratchAddr;
    uint64_t rankSliceOffset =
        ccuParam_.rankId * ((commParam->strideCount == 0) ? sliceSize : (commParam->strideCount * dataSize));
    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForReduceScatterM2M scratchAddr:0x%llx, rankSliceOffset:%d",
        ccuParam_.scratchAddr, rankSliceOffset);
    xnData_[4] = rankSliceOffset;
    xnData_[7] = sliceSize;
    xnData_[8] = UINT64_MAX - 1;
    CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[9]);
    AscendC::PRINTF(
        "[CcuPrepareForReduceScatterM2M] offset=%llu, sliceSize=%llu, xn3=0x%llx, xn4=%llu, xn7=%llu, xn8=0x%llx\n",
        offset, sliceSize, xnData_[3], xnData_[4], xnData_[7], xnData_[8]);
}
} // namespace AscendC

#endif
