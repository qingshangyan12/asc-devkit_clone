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
#include "../common/kfc_server_message.h"
#include "../impl/platform_v310/hccl_ccu_v0_def.h"

namespace AscendC {
using namespace HcclKfcProtocol;
static_assert(KFC_CONCURRENT_AG_PARAM_NUM <= CCU_USED_XN_NUM, "Concurrent AllGather parameters exceed XN capacity");
static_assert(
    KFC_RS_SOLE_NHR_PARAM_NUM <= CCU_USED_XN_NUM, "Sole NHR MultiLink ReduceScatter parameters exceed XN capacity");
static_assert(KFC_CONCURRENT_A2A_PARAM_NUM <= CCU_USED_XN_NUM, "Concurrent AllToAll parameters exceed XN capacity");
static_assert(KFC_CONCURRENT_RS_PARAM_NUM <= CCU_USED_XN_NUM, "Concurrent ReduceScatter parameters exceed XN capacity");
static_assert(KFC_PARALLEL_AG_STORAGE_NUM <= CCU_USED_XN_NUM, "Parallel AllGather parameters exceed XN capacity");
static_assert(CCU_USED_XN_NUM <= CCU_MSG_XN_NUM, "KFC loaded parameters exceed the message slot");

__aicore__ inline void CalcPeerOnlyChunkParams(uint64_t sliceSize, uint64_t* tailSize, uint64_t* chunkLoopNum)
{
    const uint64_t fullChunkCount = sliceSize == 0 ? 0 : (sliceSize - 1) / CCU_MAX_COMM_DATA;
    *tailSize = sliceSize - fullChunkCount * CCU_MAX_COMM_DATA;
    const uint64_t chunkCount = sliceSize == 0 ? 0 : fullChunkCount + 1;
    *chunkLoopNum = UINT64_MAX - chunkCount;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllV(
    __gm__ CommonPrepareParamCcu* commParam, __gm__ AlltoAllVParamCcu* allToAllVParam)
{
    const uint64_t dataSize = GetHcclDataTypeSize(commParam->dataType);
    xnData_[0] = GetOpId(commParam); // ccu xn0
    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForAllToAllV count:%llu, sendBuf:0x%llx, recvBuf:0x%llx, dataType:%d",
        (unsigned long long)commParam->count, (unsigned long long)(uint64_t)commParam->sendBuf,
        (unsigned long long)(uint64_t)commParam->recvBuf, static_cast<int>(commParam->dataType));
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2
    xnData_[3] = 0;                                     // 3 is index of xnData
    xnData_[4] = 0;                                     // 4 is index of xnData
    // 按照卡分组，sendSize 、sendOffset、recvSize、recvOffset  以字节为单位 * DataSzie(DataType)
    // ccu xn5
    AssembleHcclMsgExtForCCU(ccuParam_, commParam, allToAllVParam);

    uint64_t loopCount = 8;
    auto dataSlice = ((allToAllVParam->sendCounts[ccuParam_.rankId]) * dataSize) % CCU_MAX_COMM_DATA;
    CalcGoSize(dataSlice, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[5]);
    xnData_[9] = reinterpret_cast<uint64_t>(ccuParam_.ccuMsgExt) + CCU_MSG_EXT_RANK_OFFSET * ccuParam_.alltoallvCnt;
    KERNEL_LOG(
        KERNEL_INFO,
        "ApiClient CcuPrepareForAllToAllV xn0(opId):0x%llx, xn1(sendBuf+off):0x%llx, xn2(recvBuf+off):0x%llx, "
        "xn5(goSize):0x%llx, xn9(msgExt):0x%llx",
        (unsigned long long)xnData_[0], (unsigned long long)xnData_[1], (unsigned long long)xnData_[2],
        (unsigned long long)xnData_[5], (unsigned long long)xnData_[9]);
    return;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAll(
    __gm__ CommonPrepareParamCcu* commParam)
{
    uint64_t dataSize = GetHcclDataTypeSize(commParam->dataType);
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
    return;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllMultiJetty(
    __gm__ CommonPrepareParamCcu* commParam)
{
    uint64_t dataSize = GetHcclDataTypeSize(commParam->dataType);
    xnData_[KFC_A2A_MJ_OP_ID] = GetOpId(commParam);
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[KFC_A2A_MJ_INPUT] = (uint64_t)commParam->sendBuf + offset;
    xnData_[KFC_A2A_MJ_OUTPUT] = (uint64_t)commParam->recvBuf + offset;
    uint64_t sliceSizeAlltoall = commParam->count * dataSize;
    uint64_t strideSize = commParam->strideCount == 0 ? sliceSizeAlltoall : commParam->strideCount * dataSize;
    xnData_[KFC_A2A_MJ_SLICE_SIZE] = sliceSizeAlltoall;
    xnData_[KFC_A2A_MJ_SRC_STRIDE] = strideSize;
    xnData_[KFC_A2A_MJ_SRC_OFFSET] = 0;
    xnData_[KFC_A2A_MJ_DST_OFFSET] = strideSize * ccuParam_.rankId;
    uint64_t loopCount = 8;
    CalcGoSize(sliceSizeAlltoall, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[KFC_A2A_MJ_GO_SIZE_0]);
    constexpr uint64_t splitAlignment = 128U;
    // 升 N>1 时须与 template 侧 A2A_JETTY_NUM 同步修改（ccu_temp_all_to_all_mesh1d_multi_jetty.cc）
    constexpr uint64_t a2aJettyNum = 1U;
    const uint64_t a2aSliceSizePerJetty = (sliceSizeAlltoall / a2aJettyNum / splitAlignment) * splitAlignment;
    const uint64_t a2aLastSliceSizePerJetty = sliceSizeAlltoall - a2aSliceSizePerJetty * (a2aJettyNum - 1U);
    xnData_[KFC_A2A_MJ_SLICE_SIZE_PER_JETTY] = a2aSliceSizePerJetty;
    xnData_[KFC_A2A_MJ_LAST_SLICE_SIZE_PER_JETTY] = a2aLastSliceSizePerJetty;
    return;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllVWrite(
    __gm__ CommonPrepareParamCcu* commParam)
{
    xnData_[0] = GetOpId(commParam); // ccu xn0
    uint64_t loopCount = CCU_LOOP_COUNT_ATAVW;

    xnData_[1] = reinterpret_cast<uint64_t>(commParam->sendBuf); // 1 is index of xnData
    xnData_[2] = commParam->wParamExt.sendSizes;                 // 2 is index of xnData
    xnData_[8] = commParam->wParamExt.remoteWinOffset;           // 8 is index of xnData
    uint64_t sliceSize = commParam->count;

    xnData_[3] = commParam->wParamExt.sendOffsets; // 3 is index of xnData
    CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[4]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllReduce(
    __gm__ CommonPrepareParamCcu* commParam)
{
    const uint64_t dataSize = GetHcclDataTypeSize(commParam->dataType);
    xnData_[0] = GetOpId(commParam); // ccu xn0
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t tmpCount = commParam->count / ccuParam_.rankNum;
    uint64_t loopCount = CCU_LOOP_COUNT;
    uint64_t sliceCount = (ccuParam_.rankId == ccuParam_.rankNum - 1) ?
                              (commParam->count - (ccuParam_.rankNum - 1) * tmpCount) :
                              tmpCount;
    uint64_t sliceSize = sliceCount * dataSize;

    uint64_t loopSize = loopCount * CCU_MEMSLICE_SIZE;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / CCU_MEMSLICE_SIZE;
    uint64_t p = sliceSize - m * loopSize - n * CCU_MEMSLICE_SIZE;

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
    auto dataSize = GetHcclDataTypeSize(commParam->dataType);
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
    xnData_[0] = GetOpId(commParam); // ccu xn0
    auto dataSize = GetHcclDataTypeSize(commParam->dataType);
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset; // ccu xn1
    xnData_[2] = (uint64_t)commParam->recvBuf + offset; // ccu xn2

    uint64_t loopCount = CCU_LOOP_COUNT_M2M_RE;
    uint64_t tmpCount = (commParam->count + ccuParam_.rankNum - 1) / ccuParam_.rankNum;
    uint64_t sliceCount = (ccuParam_.rankId == ccuParam_.rankNum - 1) ?
                              (commParam->count - (ccuParam_.rankNum - 1) * tmpCount) :
                              tmpCount;
    uint64_t sliceSize = sliceCount * dataSize;

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

    constexpr uint64_t arScratchSize = 128 * 1024 * 1024; // 与 host 侧 alloc_ctx_res.cc 对齐（双端契约）
    constexpr uint64_t minSliceAlign = 128;
    uint64_t chunkSize = arScratchSize / ccuParam_.rankNum / minSliceAlign * minSliceAlign;
    uint64_t mySliceSize = xnData_[8];
    uint64_t fullChunkCount = mySliceSize == 0 ? 0 : (mySliceSize - 1) / chunkSize;
    uint64_t tailSize = mySliceSize - fullChunkCount * chunkSize;
    uint64_t chunkCount = mySliceSize == 0 ? 0 : fullChunkCount + 1;
    xnData_[15] = chunkSize;
    xnData_[16] = tailSize;
    xnData_[17] = UINT64_MAX - chunkCount;
    CalcGoSize(chunkSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[18]);
    CalcGoSize(tailSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[22]);
    KERNEL_LOG(
        KERNEL_INFO, "AR chunk debug: mySlice=0x%llx, chunk=0x%llx, full=0x%llx, tail=0x%llx, loop=0x%llx\n",
        mySliceSize, chunkSize, fullChunkCount, tailSize, xnData_[17]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllGatherM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    xnData_[KFC_CONCURRENT_AG_OP_ID] = GetOpId(commParam);
    auto dataSize = GetHcclDataTypeSize(commParam->dataType);
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[KFC_CONCURRENT_AG_MESH_INPUT] = (uint64_t)commParam->sendBuf + offset;
    xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT] = (uint64_t)commParam->recvBuf + offset;

    uint64_t loopCount = CCU_LOOP_COUNT_M2M_AG;
    uint64_t sliceCount = commParam->count;
    uint64_t sliceSize = sliceCount * dataSize;

    xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET] =
        ccuParam_.rankId * ((commParam->strideCount == 0) ? sliceSize : (commParam->strideCount * dataSize));
    xnData_[KFC_CONCURRENT_AG_MESH_SLICE_SIZE] = sliceSize;
    CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE * 8, &xnData_[KFC_CONCURRENT_AG_MESH_GO_SIZE_0]);
    xnData_[KFC_CONCURRENT_AG_MESH_CURRENT_RANK_SLICE_INPUT_OFFSET] = 0U;
    xnData_[KFC_CONCURRENT_AG_MESH_REPEAT_NUM_INV] = UINT64_MAX - 1U;
    xnData_[KFC_CONCURRENT_AG_MESH_INPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_MESH_LAST_SLICE_SIZE] = sliceSize;
    xnData_[KFC_CONCURRENT_AG_MESH_INPUT_OUTPUT_EQUAL] =
        (xnData_[KFC_CONCURRENT_AG_MESH_INPUT] ==
         xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT] + xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET]) ?
            1U :
            0U;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForConcurrentAllGatherM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    constexpr uint64_t meshBandwidth = 11U;
    constexpr uint64_t totalBandwidth = 21U;
    constexpr uint64_t splitAlignment = 128U;
    constexpr uint64_t nhrJettyNum = 4U;
    const uint64_t dataTypeSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    const uint64_t totalSize = commParam->count * dataTypeSize;
    const uint64_t repeatOffset = totalSize * ccuParam_.repeatIndex;
    const uint64_t meshSize = (totalSize * meshBandwidth / totalBandwidth / splitAlignment) * splitAlignment;
    const uint64_t nhrSize = totalSize - meshSize;
    const uint64_t nhrSliceSizePerJetty = (nhrSize / nhrJettyNum / splitAlignment) * splitAlignment;
    const uint64_t nhrLastSliceSizePerJetty = nhrSize - nhrSliceSizePerJetty * (nhrJettyNum - 1U);
    const uint64_t outputStride = commParam->strideCount == 0U ? totalSize : commParam->strideCount * dataTypeSize;
    const uint64_t inputBase = reinterpret_cast<uint64_t>(commParam->sendBuf) + repeatOffset;
    const uint64_t outputBase = reinterpret_cast<uint64_t>(commParam->recvBuf) + repeatOffset;

    xnData_[KFC_CONCURRENT_AG_OP_ID] = GetOpId(commParam);
    xnData_[KFC_CONCURRENT_AG_MESH_INPUT] = inputBase;
    xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT] = outputBase;
    xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET] = outputStride * ccuParam_.rankId;
    xnData_[KFC_CONCURRENT_AG_MESH_SLICE_SIZE] = meshSize;
    CalcGoSize(meshSize, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, &xnData_[KFC_CONCURRENT_AG_MESH_GO_SIZE_0]);
    xnData_[KFC_CONCURRENT_AG_MESH_CURRENT_RANK_SLICE_INPUT_OFFSET] = 0U;
    xnData_[KFC_CONCURRENT_AG_MESH_REPEAT_NUM_INV] = UINT64_MAX - 1U;
    xnData_[KFC_CONCURRENT_AG_MESH_INPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_MESH_LAST_SLICE_SIZE] = meshSize;
    xnData_[KFC_CONCURRENT_AG_MESH_INPUT_OUTPUT_EQUAL] =
        (inputBase == outputBase + xnData_[KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET]) ? 1U : 0U;

    xnData_[KFC_CONCURRENT_AG_NHR_INPUT] = inputBase + meshSize;
    xnData_[KFC_CONCURRENT_AG_NHR_OUTPUT] = outputBase + meshSize;
    xnData_[KFC_CONCURRENT_AG_NHR_SLICE_SIZE] = nhrSize;
    xnData_[KFC_CONCURRENT_AG_NHR_SLICE_SIZE_PER_JETTY] = nhrSliceSizePerJetty;
    xnData_[KFC_CONCURRENT_AG_NHR_LAST_SLICE_SIZE_PER_JETTY] = nhrLastSliceSizePerJetty;
    xnData_[KFC_CONCURRENT_AG_NHR_REPEAT_NUM_INV] = UINT64_MAX - 1U;
    xnData_[KFC_CONCURRENT_AG_NHR_INPUT_SLICE_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_NHR_OUTPUT_SLICE_STRIDE] = outputStride;
    xnData_[KFC_CONCURRENT_AG_NHR_INPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_NHR_OUTPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_AG_NHR_INPUT_OUTPUT_EQUAL] =
        (xnData_[KFC_CONCURRENT_AG_NHR_INPUT] ==
         xnData_[KFC_CONCURRENT_AG_NHR_OUTPUT] + outputStride * ccuParam_.rankId) ?
            1U :
            0U;
    CalcGoSize(nhrSize, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, &xnData_[KFC_CONCURRENT_AG_NHR_GO_SIZE_0]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForConcurrentAllToAll(
    __gm__ CommonPrepareParamCcu* commParam)
{
    constexpr uint64_t meshBandwidth = 11U;
    constexpr uint64_t totalBandwidth = 21U;
    constexpr uint64_t splitAlignment = 128U;
    const uint64_t dataTypeSize = GetHcclDataTypeSize(commParam->dataType);
    const uint64_t totalSize = commParam->count * dataTypeSize;
    const uint64_t repeatOffset = totalSize * ccuParam_.repeatIndex;
    const uint64_t meshSize = (totalSize * meshBandwidth / totalBandwidth / splitAlignment) * splitAlignment;
    const uint64_t closSize = totalSize - meshSize;
    const uint64_t strideSize = commParam->strideCount == 0U ? totalSize : commParam->strideCount * dataTypeSize;
    const uint64_t inputBase = reinterpret_cast<uint64_t>(commParam->sendBuf) + repeatOffset;
    const uint64_t outputBase = reinterpret_cast<uint64_t>(commParam->recvBuf) + repeatOffset;

    xnData_[KFC_CONCURRENT_A2A_OP_ID] = GetOpId(commParam);
    xnData_[KFC_CONCURRENT_A2A_MESH_INPUT] = inputBase;
    xnData_[KFC_CONCURRENT_A2A_MESH_OUTPUT] = outputBase;
    xnData_[KFC_CONCURRENT_A2A_MESH_SLICE_SIZE] = meshSize;
    xnData_[KFC_CONCURRENT_A2A_MESH_SRC_STRIDE] = strideSize;
    xnData_[KFC_CONCURRENT_A2A_MESH_SRC_OFFSET] = 0U;
    xnData_[KFC_CONCURRENT_A2A_MESH_DST_OFFSET] = strideSize * ccuParam_.rankId;
    CalcGoSize(meshSize, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, &xnData_[KFC_CONCURRENT_A2A_MESH_GO_SIZE_0]);

    xnData_[KFC_CONCURRENT_A2A_CLOS_INPUT] = inputBase + meshSize;
    xnData_[KFC_CONCURRENT_A2A_CLOS_OUTPUT] = outputBase + meshSize;
    xnData_[KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE] = closSize;
    xnData_[KFC_CONCURRENT_A2A_CLOS_SRC_STRIDE] = strideSize;
    xnData_[KFC_CONCURRENT_A2A_CLOS_SRC_OFFSET] = 0U;
    xnData_[KFC_CONCURRENT_A2A_CLOS_DST_OFFSET] = strideSize * ccuParam_.rankId;
    CalcGoSize(closSize, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, &xnData_[KFC_CONCURRENT_A2A_CLOS_GO_SIZE_0]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForParallelAllGatherM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    // Keep the split identical to InsV2AllGatherParallelExecutor: split by
    // element count so neither half can cut through an element.
    constexpr uint64_t nhrJettyNum = 1U;
    const uint64_t dataTypeSize = GetHcclDataTypeSize(commParam->dataType);
    const uint64_t totalSize = commParam->count * dataTypeSize;
    const uint64_t part0Size = (commParam->count / 2U) * dataTypeSize;
    const uint64_t part1Size = totalSize - part0Size;
    const uint64_t repeatOffset = totalSize * ccuParam_.repeatIndex;
    const uint64_t outputStride = commParam->strideCount == 0U ? totalSize : commParam->strideCount * dataTypeSize;
    const uint64_t inputBase = reinterpret_cast<uint64_t>(commParam->sendBuf) + repeatOffset;
    const uint64_t outputBase = reinterpret_cast<uint64_t>(commParam->recvBuf) + repeatOffset;

    GM_ADDR queueBase = newCcuFlag_ ? reinterpret_cast<GM_ADDR>(hcclNewContext_->xnAddr) : hcclContext_->xnOffset;
    queueBase += static_cast<uint64_t>(ccuParam_.alltoallvCnt) * CCU_MSG_XN_NUM * CCU_XN_DATA_SIZE;

    xnData_[KFC_PARALLEL_AG_OP_ID] = GetOpId(commParam);
    xnData_[KFC_PARALLEL_AG_INPUT] = inputBase;
    xnData_[KFC_PARALLEL_AG_OUTPUT] = outputBase;
    xnData_[KFC_PARALLEL_AG_TOTAL_SIZE] = totalSize;
    xnData_[KFC_PARALLEL_AG_OUTPUT_STRIDE] = outputStride;
    xnData_[KFC_PARALLEL_AG_PART0_SIZE] = part0Size;
    xnData_[KFC_PARALLEL_AG_PART1_SIZE] = part1Size;
    xnData_[KFC_PARALLEL_AG_PART1_OFFSET] = part0Size;
    xnData_[KFC_PARALLEL_AG_MESH_PHASE_DONE_ADDR] =
        reinterpret_cast<uint64_t>(queueBase + KFC_PARALLEL_AG_MESH_PHASE_DONE_STORAGE * CCU_XN_DATA_SIZE);
    xnData_[KFC_PARALLEL_AG_NHR_PHASE_DONE_ADDR] =
        reinterpret_cast<uint64_t>(queueBase + KFC_PARALLEL_AG_NHR_PHASE_DONE_STORAGE * CCU_XN_DATA_SIZE);
    CalcGoSize(part0Size, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, &xnData_[KFC_PARALLEL_AG_PART0_GO_SIZE_0]);
    CalcGoSize(part1Size, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, &xnData_[KFC_PARALLEL_AG_PART1_GO_SIZE_0]);
    xnData_[KFC_PARALLEL_AG_PART0_SLICE_PER_JETTY] = part0Size / nhrJettyNum;
    xnData_[KFC_PARALLEL_AG_PART0_LAST_SLICE_PER_JETTY] = part0Size;
    xnData_[KFC_PARALLEL_AG_PART1_SLICE_PER_JETTY] = part1Size / nhrJettyNum;
    xnData_[KFC_PARALLEL_AG_PART1_LAST_SLICE_PER_JETTY] = part1Size;
    xnData_[KFC_PARALLEL_AG_MESH_PHASE_DONE_STORAGE] = 0U;
    xnData_[KFC_PARALLEL_AG_NHR_PHASE_DONE_STORAGE] = 0U;
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForReduceScatterM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    xnData_[0] = GetOpId(commParam); // ccu xn0
    auto dataSize = GetHcclDataTypeSize(commParam->dataType);
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
    xnData_[5] = 0;
    xnData_[6] = 0;
    xnData_[7] = sliceSize;
    xnData_[8] = UINT64_MAX - 1;
    CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[9]);
    // KFC-specific parameters at [13..23] (homm template ignores these, uses templateDataParams).
    constexpr uint64_t scratchSize = 64 * 1024 * 1024; // 调试: 16MB->64MB，需与host侧alloc_ctx_res.cc对齐
    constexpr uint64_t minSliceAlign = 128;
    uint64_t chunkSize = scratchSize / ccuParam_.rankNum / minSliceAlign * minSliceAlign;
    uint64_t fullChunkCount = sliceSize == 0 ? 0 : (sliceSize - 1) / chunkSize;
    uint64_t chunkCount = sliceSize == 0 ? 0 : fullChunkCount + 1;
    uint64_t tailSize = sliceSize - fullChunkCount * chunkSize;
    xnData_[13] = chunkSize;
    xnData_[14] = UINT64_MAX - chunkCount;
    xnData_[15] = tailSize;
    CalcGoSize(chunkSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[16]);
    CalcGoSize(tailSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[20]);
    KERNEL_LOG(
        KERNEL_INFO, "RS chunk debug: slice=0x%llx, chunk=0x%llx, full=0x%llx, tail=0x%llx, loop=0x%llx\n", sliceSize,
        chunkSize, fullChunkCount, xnData_[15], xnData_[14]);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForReduceScatterPeerOnlyM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    xnData_[0] = GetOpId(commParam);
    const uint64_t dataSize = GetHcclDataTypeSize(commParam->dataType);
    const uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset;
    xnData_[2] = (uint64_t)commParam->recvBuf + offset;
    xnData_[3] = 0;

    const uint64_t sliceSize = commParam->count * dataSize;
    xnData_[4] = ccuParam_.rankId * ((commParam->strideCount == 0) ? sliceSize : (commParam->strideCount * dataSize));
    xnData_[5] = 0;
    xnData_[6] = 0;
    CalcPeerOnlyChunkParams(sliceSize, &xnData_[7], &xnData_[8]);
    KERNEL_LOG(
        KERNEL_INFO,
        "PeerOnly RS prepare: input=0x%llx, output=0x%llx, rankOffset=0x%llx, slice=0x%llx, "
        "chunk=0x%llx, tail=0x%llx, loop=0x%llx\n",
        xnData_[1], xnData_[2], xnData_[4], sliceSize, CCU_MAX_COMM_DATA, xnData_[7], xnData_[8]);
}

// SoleNHRMultiLink（CcuSchedReduceScatterSoleNHRMultiLink）的 AIV prepare：
// 将 hccl InsV2ReduceScatterSoleExecutor::OrchestrateLoop（loopTimes=1 单趟）+
// CcuTempReduceScatterNhrMultiJettyMem2Mem1D::KernelRun 的 host 侧参数计算搬到设备侧。
// 布局见 KfcReduceScatterSoleNhrParamIndex，与 KFC dispatch、kernel 形参逐槽一致。
template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForReduceScatterSoleNhrM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    constexpr uint64_t hcclMinSliceAlign = 128U;
    constexpr uint64_t nhrJettyNum = 1U; // 与 hccl CcuTempReduceScatterNhrMultiJettyMem2Mem1D 的 portNum=1 一致
    const uint64_t dataTypeSize = GetHcclDataTypeSize(commParam->dataType);
    const uint64_t sliceSize = commParam->count * dataTypeSize; // 每 rank 输出份
    const uint64_t repeatOffset = sliceSize * ccuParam_.repeatIndex;
    const uint64_t sliceStride = commParam->strideCount == 0U ? sliceSize : commParam->strideCount * dataTypeSize;
    const uint64_t inputBase = reinterpret_cast<uint64_t>(commParam->sendBuf) + repeatOffset;
    const uint64_t outputBase = reinterpret_cast<uint64_t>(commParam->recvBuf) + repeatOffset;
    // hccl 口径：sliceAlignCount = 128 / dataTypeSize（元素数）；portNum=1 时 sliceOneJettySize 不被消费
    const uint64_t sliceAlignCount = hcclMinSliceAlign / dataTypeSize;
    const uint64_t sliceOneJettySize = sliceSize / nhrJettyNum / sliceAlignCount * sliceAlignCount;
    const uint64_t sliceLastJettySize = sliceSize - (nhrJettyNum - 1U) * sliceOneJettySize;

    xnData_[KFC_RS_SOLE_NHR_OP_ID] = GetOpId(commParam);
    xnData_[KFC_RS_SOLE_NHR_INPUT] = inputBase;
    xnData_[KFC_RS_SOLE_NHR_OUTPUT] = outputBase;
    xnData_[KFC_RS_SOLE_NHR_SLICE_SIZE] = sliceSize;
    xnData_[KFC_RS_SOLE_NHR_INPUT_SLICE_STRIDE] = sliceStride;
    xnData_[KFC_RS_SOLE_NHR_SLICE_ONE_JETTY_SIZE] = sliceOneJettySize;
    xnData_[KFC_RS_SOLE_NHR_SLICE_LAST_JETTY_SIZE] = sliceLastJettySize;
    xnData_[KFC_RS_SOLE_NHR_REPEAT_NUM_INV] = UINT64_MAX - 1U; // sole executor repeatNum=1
    xnData_[KFC_RS_SOLE_NHR_INPUT_REPEAT_STRIDE] = 0U;         // sole executor 双 stride=0
    xnData_[KFC_RS_SOLE_NHR_OUTPUT_REPEAT_STRIDE] = 0U;
    KERNEL_LOG(
        KERNEL_INFO, "RS sole-NHR prepare: slice=0x%llx, stride=0x%llx, lastJetty=0x%llx\n", sliceSize, sliceStride,
        sliceLastJettySize);
}

template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForConcurrentReduceScatterM2M(
    __gm__ CommonPrepareParamCcu* commParam)
{
    // 将 hccl InsReduceScatterConcurrentExecutor::OrchestrateLoop 的 host 侧切分搬到 AIV：
    // CCU_SCHED 带宽比 MESH_BW_SCHED:CLOS_BW_SCHED = 10:12，meshSize 按 128B 对齐下取整。
    constexpr uint64_t meshBandwidth = 10U;
    constexpr uint64_t totalBandwidth = 22U;
    constexpr uint64_t splitAlignment = 128U;
    constexpr uint64_t nhrJettyNum = 1U; // 与 hccl CcuTempReduceScatterNhrMultiJettyMem2Mem1D 的 portNum=1 一致
    const uint64_t dataTypeSize = GetHcclDataTypeSize(commParam->dataType);
    const uint64_t sliceSize = commParam->count * dataTypeSize; // 每 rank 输出份
    const uint64_t repeatOffset = sliceSize * ccuParam_.repeatIndex;
    const uint64_t meshSize = (sliceSize * meshBandwidth / totalBandwidth / splitAlignment) * splitAlignment;
    const uint64_t nhrSize = sliceSize - meshSize;
    const uint64_t sliceStride = commParam->strideCount == 0U ? sliceSize : commParam->strideCount * dataTypeSize;
    const uint64_t inputBase = reinterpret_cast<uint64_t>(commParam->sendBuf) + repeatOffset;
    const uint64_t outputBase = reinterpret_cast<uint64_t>(commParam->recvBuf) + repeatOffset;

    // mission0：Mesh 流（紧凑布局 [1..15]，chunk 循环公式同 SoleMesh；NHR 流不占 scratch，全量给 Mesh 流）。
    xnData_[KFC_CONCURRENT_RS_OP_ID] = GetOpId(commParam);
    xnData_[KFC_CONCURRENT_RS_MESH_INPUT] = inputBase;
    xnData_[KFC_CONCURRENT_RS_MESH_OUTPUT] = outputBase;
    xnData_[KFC_CONCURRENT_RS_MESH_SCRATCH] = ccuParam_.scratchAddr;
    xnData_[KFC_CONCURRENT_RS_MESH_RANK_SLICE_OFFSET] = ccuParam_.rankId * sliceStride;
    constexpr uint64_t scratchSize = 64 * 1024 * 1024; // 与 host 侧 alloc_ctx_res.cc 对齐（双端契约）
    const uint64_t chunkSize = scratchSize / ccuParam_.rankNum / splitAlignment * splitAlignment;
    const uint64_t fullChunkCount = meshSize == 0 ? 0 : (meshSize - 1) / chunkSize;
    const uint64_t chunkCount = meshSize == 0 ? 0 : fullChunkCount + 1;
    const uint64_t meshTailSize = meshSize - fullChunkCount * chunkSize;
    xnData_[KFC_CONCURRENT_RS_MESH_CHUNK_SIZE] = chunkSize;
    xnData_[KFC_CONCURRENT_RS_MESH_CHUNK_LOOP_NUM] = UINT64_MAX - chunkCount;
    xnData_[KFC_CONCURRENT_RS_MESH_TAIL_SIZE] = meshTailSize;
    CalcGoSize(
        chunkSize, CCU_LOOP_COUNT_M2M_RE, CCU_MEMSLICE_SIZE, &xnData_[KFC_CONCURRENT_RS_MESH_FULL_GO_ADDR_OFFSET]);
    CalcGoSize(
        meshTailSize, CCU_LOOP_COUNT_M2M_RE, CCU_MEMSLICE_SIZE, &xnData_[KFC_CONCURRENT_RS_MESH_TAIL_GO_ADDR_OFFSET]);

    // mission1：NHR 流（[16..24]，处理 meshSize 之后的尾段；sliceStride 用于定位各 rank 的输入分片）。
    xnData_[KFC_CONCURRENT_RS_NHR_INPUT] = inputBase + meshSize;
    xnData_[KFC_CONCURRENT_RS_NHR_OUTPUT] = outputBase + meshSize;
    xnData_[KFC_CONCURRENT_RS_NHR_SLICE_SIZE] = nhrSize;
    xnData_[KFC_CONCURRENT_RS_NHR_INPUT_SLICE_STRIDE] = sliceStride;
    xnData_[KFC_CONCURRENT_RS_NHR_SLICE_ONE_JETTY_SIZE] = nhrSize / nhrJettyNum / splitAlignment * splitAlignment;
    xnData_[KFC_CONCURRENT_RS_NHR_SLICE_LAST_JETTY_SIZE] =
        nhrSize - (nhrJettyNum - 1U) * xnData_[KFC_CONCURRENT_RS_NHR_SLICE_ONE_JETTY_SIZE];
    xnData_[KFC_CONCURRENT_RS_NHR_REPEAT_NUM_INV] = UINT64_MAX - 1U;
    xnData_[KFC_CONCURRENT_RS_NHR_INPUT_REPEAT_STRIDE] = 0U;
    xnData_[KFC_CONCURRENT_RS_NHR_OUTPUT_REPEAT_STRIDE] = 0U;
    KERNEL_LOG(
        KERNEL_INFO,
        "RS concur chunk debug: slice=0x%llx, mesh=0x%llx, nhr=0x%llx, chunk=0x%llx, meshLoop=0x%llx, "
        "meshTail=0x%llx\n",
        sliceSize, meshSize, nhrSize, chunkSize, xnData_[KFC_CONCURRENT_RS_MESH_CHUNK_LOOP_NUM], meshTailSize);
}

// ============================================================================
// M2（§12-11 甲路线尾款轮，2026-09-21）：unified Plan 的 device 写入方。
//
// 职责：从 OpResCtx.res[1] 携带的 v3 blob device 地址读 blob → 剥头部 4 槽 →
// 逐段写 xn 消息区（每段一帧 32 槽）→ 追加 INVALID_U64 哨兵帧 → 逐帧写 commit CKE
// 信号。与既有 CcuPrepareFor* 的差别：那些是"单帧参数装配"（xnData_[32] 填值后由
// CcuSendMsg 落盘一帧 + 一个信号），本函数是"多帧批量落盘"——信号粒度是帧不是消息
//（M0-① 裁定）：N 段 + 1 哨兵 = N+1 次 commitCKE 写，只写 xn 不逐帧写信号 =
// server 第 2 轮起在 waitAddr+8 死锁。
//
// 落盘方式：直写 GM（parallel AG 先例 CcuPrepareForParallelAllGatherM2M 的 queueBase
// 同式）+ 自理 flush（每帧 32 槽=256B=2 个 128B 行）。不复用 CcuSendMsg：它写的是
// xnData_[32] 单帧寄存器快照（v3 的段数据在 blob GM 区不在寄存器），且其 flush 按
// ccuUsedXnNum_ 折算——多帧直写更直接，也避免与 v2 消息流的 resourceId 全局游标错位。
//
// 帧几何（M0-② 裁定）：AIV 每帧步进 CCU_MSG_XN_NUM*CCU_XN_DATA_SIZE=512B（CcuSendMsg
// 同式），server 每帧步进 CCU_PARAM_NUM_MAX*8B=512B（server 侧 paramAddr 推进），
// 两侧天然对齐；v3 记录只用前 32 槽（server LoadFuncParamFromMemory 只 Load 32），
// 后 32 槽闲置不写。
//
// blob 格式（kfc_server_message.h M2 常量，镜像 unified plan_serializer.h；R6 头部
// 扩槽 4→7/版本升 4——dataType/outputType/reduceType 进头部 [4..6]，写入方只透传
// 不消费，server 分发不读头部）：
//   [0]=magic "UPL3"、[1]=版本 4、[2]=段数 N、[3]=保留、[4]=dataType、[5]=outputType、
//   [6]=reduceType；[7..] = N×32 槽段记录。
// 校验（M4-①/M4-④ 落点）：magic/版本不符即拒发（不写 xn 区——server 不会收到假帧，
// 本轮通信悬置由 host 侧调用方超时兜底，与 v2 各 prepare 的 ASCENDC_ASSERT 拒发同粒度）；
// 段数越界（0 或 >63）同样拒发。空 Plan 口径（M4-④ 裁定）：拒绝 segNum=0——允许空
// 计划会把"编译产物为空"静默成"零动作通信"吞掉错误，拒绝则逼上游 Compile 期显式化。
// 哨兵帧不属于 blob（第四轮契约），本函数追加。
//
// R6.5 · WP-C1（B-CCU②，2026-09-22）：本函数从"纯拷贝"升级为"拷贝 + 数据槽覆写"
// ——方向 B 的 CCU 侧落地（AICPU 姊妹件 = R6 的 unified_exec/aicpu_consumer.h
// InstantiateSegment 八字段实例化）。动机（B0 注释④病灶）：资源期 count=0（
// PrepareParamForAllGather 显式置 0，真值 exec 期由 commParam 注入），blob 的数据
// 相关槽全是占位值——槽 4(sliceSize)=0、槽 5..8(goSize)=全零、槽 14(inOutEqual)
// 按 0==0 误判 1。落盘前以 exec 期 commParam 现值重算覆盖（11 槽现值覆写 +
// 5 槽常量透传，M0-1 名单）：
//   覆写槽（v2 CcuPrepareForAllGatherM2M :273-296 逐式同源）：
//     1  input       = commParam->sendBuf + count*repeatIndex*dts + blob槽1（合成式：
//                     blob 槽值是段内 inBaseOff 结构量非地址——AG 恒 0 时退化为直替，
//                     但合成式是正确形态，防未来非零 baseOff/切轮帧）
//     2  output      = commParam->recvBuf + count*repeatIndex*dts + blob槽2（同式）
//     3  outputOffset= rankId * (strideCount==0 ? sliceSize : strideCount*dts)
//                     （★strideCount 也是 exec 现值——三分支齐，只按 count 推漏分支）
//     4  sliceSize   = commParam->count * GetHcclDataTypeSize(commParam->dataType)
//     5..8 goSize    = CalcGoSize(覆写后 sliceSize, CCU_LOOP_COUNT_M2M_AG,
//                     CCU_MEMSLICE_SIZE*8)（设备侧 :148 同式）
//     13 lastSliceSize = 覆写后 sliceSize（AG 等分场景；blob 编译期尾片结构按新
//                     count 重算的位置——AG 恒等分，P2 尾片场景在此扩展）
//     14 inOutEqual  = (覆写后槽1 == 覆写后槽2 + 槽3) ? 1 : 0（重判——资源期
//                     count=0 下 blob 槽 14 恒 1 是误判值，透传必错）
//   透传槽（值不依赖 count/dataType/strideCount/ptrs 任一 exec 现值，资源期
//   与 exec 期同值）：0 动作 ID、9 curRankInOff（AG 恒 0）、10 repeatNumInv、
//   11/12 repeatStride。
//   goSize 上界拒收（WP-C2 降级后的唯一真实约束，R6.5 修订一）：覆写后
//   sliceSize > 262144×8192 = 2 GiB → 拒收不落盘（loopSize=8×32768=262144、
//   MaxLoopIterNum=(1<<13)-1=8191，v2 基准 ccu_kernel_alg_base.cc:48 maxSize）。
//   ★比 v2 严：v2 设备侧 CalcGoSize（hccl_ccu_xn_utils.h:148）与本防线前对 2GiB+
//   AG 均静默溢出——此为**新增防线非存量复刻**，防后人按"对齐 v2"名义移除。
//   RS 帧级拒收（R6.5 修订三）：段帧槽 0 != CollectAll → ERROR + return（RS
//   instantiation 属 R7；与 magic/版本/段数拒发同粒度——不落盘不发信号，本轮
//   通信由 host 侧调用方超时兜底，M4-④"宁可显式失败"口径族）。
//   【R7 · WP-1（2026-10-06）：RS 帧级拒收放开——ReduceSlice 帧的 exec 现值实例化】
//   AG 覆写名单不变；RS 帧（槽 0 == KFC_UNIFIED_ACTION_REDUCE_SLICE）按 v2 并发 RS
//   写入方（CcuPrepareForConcurrentReduceScatterM2M :528-575，mission0 Mesh 区间）
//   逐式实例化 RS 槽 1..15：
//     1  input       = commParam->sendBuf + count*repeatIndex*dts + blob槽1（合成式
//                     ——blob 槽值是段内 inBaseOff 结构量；与 AG 槽 1 同式）
//     2  output      = commParam->recvBuf + count*repeatIndex*dts + blob槽2（同式；
//                     ★v2 并发 RS 的 repeatOffset 用 sliceSize（每 rank 输出份）计
//                     而 AG 用 count——RS 场景 commParam->count 即每 rank 输出份
//                     （selector 口径），count×dts == sliceSize，两式在此等价）
//     3  scratch     = ccuParam_.scratchAddr（v2 :547 直替——资源期 blob 槽 3 是
//                     scratchBaseOff=0 占位，无结构量可保留）
//     4  rankSliceOffset = rankId × (strideCount==0 ? sliceSize : strideCount×dts)
//                     （v2 :548 三分支，strideCount 是 exec 现值——整体覆写丢弃
//                     blob 编译期占位值）
//     5  chunkSize   = 64MB / rankNum / 128 × 128（v2 :552-553 双端契约常量，
//                     rankNum 来自 ccuParam_——设备侧自持，非 blob 字段）
//     6  chunkLoopNum= UINT64_MAX - chunkCount（chunkCount = (sliceSize-1)/chunkSize+1，
//                     sliceSize==0 时 0——v2 :554-555 同式）
//     7  tailSize    = sliceSize - fullChunkCount×chunkSize（v2 :556）
//     8..11 fullGo   = CalcGoSize(chunkSize, CCU_LOOP_COUNT_M2M_RE, CCU_MEMSLICE_SIZE)
//                     （★RS config loopCount=16 ≠ AG 的 8——漏配即 goSize 与
//                     LoopGroup 步进错位，kernel :26-30 注释的硬约束）
//     12..15 tailGo  = CalcGoSize(tailSize, 同上)（v2 :561-563 逐式）
//   RS 无透传数据槽（15 槽全部依赖 count/dataType/strideCount/scratch/rankNum 现值
//   ——chunk 参数是 scratchSize/rankNum 的函数，rankNum 变则 chunk 变，无资源期
//   可信值）。未知动作 ID（既非 CollectAll 也非 ReduceSlice）维持帧级拒收
//   （server v3 白名单 {0,1} 对齐——写入方不产出 server 不认识的帧）。
//   ★RS goSize 上界不查 2GiB：RS goSize 按 chunkSize（≤64MB/8=8MB）计算而非
//   sliceSize 全量（v2 无此检查同因——chunk 恒小）；2GiB 防线对 AG 保留。
// ============================================================================
template <const auto& config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForUnifiedPlan(
    __gm__ CommonPrepareParamCcu* commParam)
{
    // R6.5 B-CCU②：commParam 现值消费（count/dataType/strideCount/sendBuf/recvBuf
    // ——exec 期注入的数据真值；AICPU 八字段的 CCU 投影 AG 子集）
    // ① blob 地址与头部校验（res[1] 携带，M0-⑤ 裁定；res[0]=scratch 已被 RS 占用）
    if (!newCcuFlag_ || hcclNewContext_ == nullptr) {
        KERNEL_LOG(KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan requires new ccu context (OpResCtx).");
        return;
    }
    __gm__ uint64_t* blob = reinterpret_cast<__gm__ uint64_t*>(hcclNewContext_->res[1]);
    if (blob == nullptr) {
        KERNEL_LOG(KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan res[1] (plan blob addr) is null.");
        return;
    }
    const uint64_t magic = ReadHBMData(blob);
    if (magic != KFC_UNIFIED_BLOB_MAGIC) {
        KERNEL_LOG(
            KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan bad magic[0x%llx] != UPL3.",
            (unsigned long long)magic);
        return;
    }
    const uint64_t version = ReadHBMData(blob + 1);
    if (version != KFC_UNIFIED_BLOB_VERSION) {
        KERNEL_LOG(
            KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan bad version[%llu] != 4.",
            (unsigned long long)version);
        return;
    }
    const uint64_t segNum = ReadHBMData(blob + 2);
    if (segNum == 0U || segNum > KFC_UNIFIED_MAX_SEGMENTS) {
        KERNEL_LOG(
            KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan invalid segNum[%llu] (need 1..63).",
            (unsigned long long)segNum);
        return;
    }

    // R6.5 B-CCU② · 覆写前置计算（exec 现值，v2 :273-296 同式）：
    //   首帧（帧 0）的动作 ID 决定覆写路径——AG（CollectAll）走数据槽覆写；
    //   其他动作 ID 帧级拒收（R7 起 ReduceSlice 亦受理，未知 ID 仍拒——帧级判据
    //   而非 Plan 级）
    // ★R7 锚点（审查预核登记，随切轮/多帧设计处理）：
    //   ①首帧预检（本段）与帧循环内判据（②段）是**双份动作 ID 检查**——单帧
    //     blob（本轮唯一形态）无害，R7 多帧时二选一（预检可撤或循环判据专职）；
    //   ②槽 3 覆写本轮**丢弃 blob 编译期值**正确（资源期 count=0 → blob 槽 3 恒 0，
    //     无可保留结构量），但 R7 切轮帧若把**轮内偏移**编码进槽 3（帧间基址推进
    //     现在走槽 1/2 的 blob 结构 offset），须改为合成式（rankId×stride + 轮偏移）
    //     而非整体覆写——届时本行是改造点。
    __gm__ uint64_t* firstFrame = blob + KFC_UNIFIED_BLOB_HEADER_SLOTS;
    const uint64_t firstActionId = ReadHBMData(firstFrame + KFC_UNIFIED_ACTION_ID);
    if (firstActionId != KFC_UNIFIED_ACTION_COLLECT_ALL &&
        firstActionId != KFC_UNIFIED_ACTION_REDUCE_SLICE) {
        KERNEL_LOG(
            KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan action[%llu] not in {CollectAll, ReduceSlice} "
            "(unknown action id; refusing, not silently passing through).",
            (unsigned long long)firstActionId);
        return;
    }
    // exec 现值派生量（对全部 AG 段帧统一预计算——单帧 blob 恒一帧，切轮帧 R7 起）
    const uint64_t dts = GetHcclDataTypeSize(commParam->dataType);
    const uint64_t sliceSize = commParam->count * dts;
    // goSize 上界拒收（WP-C2：唯一真实约束；2 GiB = 262144 × 8192）
    constexpr uint64_t kGoSizeLoopLimit = 262144ULL * 8192ULL;   // loopSize × (MaxLoopIterNum+1)
    if (sliceSize > kGoSizeLoopLimit) {
        KERNEL_LOG(
            KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan sliceSize[%llu] > 2GiB goSize loop "
            "limit[%llu] (v2 single-frame capacity; rejecting — this guard is STRICTER than v2, "
            "which silently overflows; do not remove when aligning to v2).",
            (unsigned long long)sliceSize, (unsigned long long)kGoSizeLoopLimit);
        return;
    }
    const uint64_t repeatOffset = commParam->count * ccuParam_.repeatIndex * dts;   // v2 :275 同式
    const uint64_t outputOffset = ccuParam_.rankId *
        ((commParam->strideCount == 0U) ? sliceSize : (commParam->strideCount * dts)); // v2 :283-284 同式
    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForUnifiedPlan exec-instantiation: count[%llu] dts[%llu] "
        "slice[%llu] rank[%u] outOff[%llu] repeatIdx[%u] repeatOff[%llu].",
        (unsigned long long)commParam->count, (unsigned long long)dts,
        (unsigned long long)sliceSize, ccuParam_.rankId, (unsigned long long)outputOffset,
        ccuParam_.repeatIndex, (unsigned long long)repeatOffset);

    // ② 消息区基址（与 CcuPrepareForParallelAllGatherM2M 的 queueBase 同式）+ 逐帧落盘。
    // R6 头部扩槽：segBase 按新头宽（KFC_UNIFIED_BLOB_HEADER_SLOTS=7，R6 裁决一：
    // dataType/outputType/reduceType 进头部 [4..6]，写入方只透传不消费——server 分发
    // 不读头部）剥头；帧推进不变（段记录区仍 N×32 槽，帧几何与本函数头注 M0-② 相同）
    GM_ADDR queueBase = reinterpret_cast<GM_ADDR>(hcclNewContext_->xnAddr);
    __gm__ uint64_t* segBase = blob + KFC_UNIFIED_BLOB_HEADER_SLOTS;
    const uint64_t totalFrames = segNum + 1U;   // 段帧 + 哨兵帧

    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForUnifiedPlan blob[0x%llx] segs[%llu] queueBase[0x%llx] frames[%llu]",
        (unsigned long long)(uint64_t)(blob), (unsigned long long)segNum,
        (unsigned long long)(uint64_t)queueBase, (unsigned long long)totalFrames);

    for (uint64_t frame = 0; frame < totalFrames; ++frame) {
        GM_ADDR frameAddr = queueBase + frame * CCU_MSG_XN_NUM * CCU_XN_DATA_SIZE;
        __gm__ uint64_t* dst = reinterpret_cast<__gm__ uint64_t*>(frameAddr);
        if (frame < segNum) {
            // 段帧：从 blob 逐槽拷 32 个（v3 记录宽度；后 32 槽闲置不写——server 只 Load 前 32）
            __gm__ uint64_t* src = segBase + frame * KFC_UNIFIED_PARAM_NUM_PER_RECORD;
            for (uint32_t i = 0; i < KFC_UNIFIED_PARAM_NUM_PER_RECORD; ++i) {
                dst[i] = ReadHBMData(src + i);
            }
            // R6.5 B-CCU② + R7 WP-1：段帧数据槽覆写（拷贝后、flush/信号前——落盘值=
            // 结构透传槽 + exec 现值覆写槽的合成）。帧级动作 ID 分叉两套覆写：
            //   CollectAll → AG 名单（11 覆写 + 5 透传，M0-1）
            //   ReduceSlice → RS 名单（槽 1..15 全覆写，头注 R7·WP-1 段）
            // 未知 ID 维持帧级拒收（server v3 白名单 {0,1} 对齐）
            const uint64_t frameActionId = dst[KFC_UNIFIED_ACTION_ID];
            if (frameActionId == KFC_UNIFIED_ACTION_REDUCE_SLICE) {
                // R7 · WP-1：RS 帧实例化（布局基准 = v2 并发 RS 写入方
                // CcuPrepareForConcurrentReduceScatterM2M :528-575 的 Mesh 区间槽 1..15；
                // chunk 计数输入取 sole 写入方 CcuPrepareForReduceScatterM2M :455-457 的
                // 全量 sliceSize 口径——并发版按 meshSize 计数系 10:22 双 mission 切分
                // 专属，单段 unified 单 server 单 mission 无切分，全量才正确。
                // commParam->count 在 RS 语义下即每 rank 输出份元素数（selector 口径），
                // sliceSize = count×dts 与 v2 的 sliceSize 同源）
                dst[KFC_UNIFIED_REDUCE_SLICE_INPUT] =
                    (uint64_t)commParam->sendBuf + repeatOffset + dst[KFC_UNIFIED_REDUCE_SLICE_INPUT];
                dst[KFC_UNIFIED_REDUCE_SLICE_OUTPUT] =
                    (uint64_t)commParam->recvBuf + repeatOffset + dst[KFC_UNIFIED_REDUCE_SLICE_OUTPUT];
                // 槽 3：scratch 直替（v2 :547；blob 编译期值是 scratchBaseOff 占位）
                dst[KFC_UNIFIED_REDUCE_SLICE_SCRATCH] = ccuParam_.scratchAddr;
                // 槽 4：rankSliceOffset 三分支（v2 :548；strideCount 是 exec 现值）
                dst[KFC_UNIFIED_REDUCE_SLICE_RANK_SLICE_OFFSET] = ccuParam_.rankId *
                    ((commParam->strideCount == 0U) ? sliceSize : (commParam->strideCount * dts));
                // 槽 5..7：chunk 三元组（chunkSize 公式 v2 :552-553 与 sole :454 同式；
                // chunkCount 输入=全量 sliceSize（sole 语义，见上注）；64MB scratch
                // 双端契约，128B 对齐下取整；chunkCount 倒计数哨兵）
                constexpr uint64_t kRsScratchSize = 64ULL * 1024 * 1024;   // v2 :551（host 侧同值契约）
                constexpr uint64_t kRsSplitAlign = 128ULL;                 // v2 :535 splitAlignment
                const uint64_t rsChunkSize = kRsScratchSize / ccuParam_.rankNum / kRsSplitAlign * kRsSplitAlign;
                const uint64_t rsFullChunkCount = (sliceSize == 0U || rsChunkSize == 0U) ? 0 : (sliceSize - 1) / rsChunkSize;
                const uint64_t rsChunkCount = (sliceSize == 0U || rsChunkSize == 0U) ? 0 : rsFullChunkCount + 1;
                const uint64_t rsTailSize = sliceSize - rsFullChunkCount * rsChunkSize;
                dst[KFC_UNIFIED_REDUCE_SLICE_CHUNK_SIZE] = rsChunkSize;
                dst[KFC_UNIFIED_REDUCE_SLICE_CHUNK_LOOP_NUM] = ~0ULL - rsChunkCount;   // v2 :558 同式
                dst[KFC_UNIFIED_REDUCE_SLICE_TAIL_SIZE] = rsTailSize;
                // 槽 8..15：full/tail 双 goSize（★RS config：loopCount=CCU_LOOP_COUNT_M2M_RE=16
                // 与 AG 的 8 不同源——kernel :26-30 的步进错位硬约束；memSlice=4096 不 ×8，
                // v2 :561-563 逐式）。本地数组中转：CalcGoSize 形参是 uint64_t*（Local
                // Memory 地址空间），dst 是 __gm__ 指针——NPU 编译面（__NPU_ARCH__=3510）
                // 严格拒绝地址空间绑定（MAJOR-1 修复；对齐 v2 写入方 xnData_ 本地数组
                // 风格，devkit CPU 面（ASCENDC_CPU_DEBUG）无地址空间区分故两侧皆过）
                uint64_t rsFullGo[4] = {0, 0, 0, 0};
                uint64_t rsTailGo[4] = {0, 0, 0, 0};
                CalcGoSize(rsChunkSize, CCU_LOOP_COUNT_M2M_RE, CCU_MEMSLICE_SIZE, rsFullGo);
                CalcGoSize(rsTailSize, CCU_LOOP_COUNT_M2M_RE, CCU_MEMSLICE_SIZE, rsTailGo);
                dst[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_ADDR_OFFSET] = rsFullGo[0];
                dst[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_LOOP_PARAM] = rsFullGo[1];
                dst[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_PARALLEL_PARAM] = rsFullGo[2];
                dst[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_RESIDUAL] = rsFullGo[3];
                dst[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_ADDR_OFFSET] = rsTailGo[0];
                dst[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_LOOP_PARAM] = rsTailGo[1];
                dst[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_PARALLEL_PARAM] = rsTailGo[2];
                dst[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_RESIDUAL] = rsTailGo[3];
                KERNEL_LOG(
                    KERNEL_INFO, "ApiClient CcuPrepareForUnifiedPlan RS frame[%llu] instantiated: slice[%llu] "
                    "chunk[%llu] tail[%llu] rankSliceOff[%llu].",
                    (unsigned long long)frame, (unsigned long long)sliceSize,
                    (unsigned long long)rsChunkSize, (unsigned long long)rsTailSize,
                    (unsigned long long)dst[KFC_UNIFIED_REDUCE_SLICE_RANK_SLICE_OFFSET]);
            } else if (frameActionId != KFC_UNIFIED_ACTION_COLLECT_ALL) {
                KERNEL_LOG(
                    KERNEL_ERROR, "ApiClient CcuPrepareForUnifiedPlan frame[%llu] action[%llu] not in "
                    "{CollectAll, ReduceSlice} (unknown action id; refusing).",
                    (unsigned long long)frame, (unsigned long long)frameActionId);
                return;   // 拒发：不落盘剩余帧、不发信号（与头部拒发同粒度）
            } else {
                // ── AG（CollectAll）覆写名单：11 覆写 + 5 透传（R6.5 M0-1，v2 :273-296） ──
                // 槽 1/2：合成式（blob 槽值是段内结构 offset，与 exec 现址相加）
                dst[KFC_UNIFIED_COLLECT_ALL_INPUT] =
                    (uint64_t)commParam->sendBuf + repeatOffset + dst[KFC_UNIFIED_COLLECT_ALL_INPUT];
                dst[KFC_UNIFIED_COLLECT_ALL_OUTPUT] =
                    (uint64_t)commParam->recvBuf + repeatOffset + dst[KFC_UNIFIED_COLLECT_ALL_OUTPUT];
                // 槽 3：rankId × stride 分支（strideCount 是 exec 现值——三分支齐）
                dst[KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET] = outputOffset;
                // 槽 4：sliceSize 现值
                dst[KFC_UNIFIED_COLLECT_ALL_SLICE_SIZE] = sliceSize;
                // 槽 5..8：goSize 重算（覆写后 sliceSize；设备侧 CalcGoSize 同式）。
                // 本地数组中转（R7 MAJOR-1 修复——R6.5 起该调用点在 NPU 编译面
                // （__NPU_ARCH__=3510）即有 __gm__ 指针绑定局部指针形参的地址空间
                // 报错，本修复一并消除；理由同 RS 分支上注）
                uint64_t agGo[4] = {0, 0, 0, 0};
                CalcGoSize(sliceSize, CCU_LOOP_COUNT_M2M_AG, CCU_MEMSLICE_SIZE * 8U, agGo);
                dst[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_0] = agGo[0];
                dst[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_1] = agGo[1];
                dst[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_2] = agGo[2];
                dst[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_3] = agGo[3];
                // 槽 13：lastSliceSize = 覆写后 sliceSize（AG 等分；尾片重算 P2 扩展位）
                dst[KFC_UNIFIED_COLLECT_ALL_LAST_SLICE_SIZE] = sliceSize;
                // 槽 14：inOutEqual 重判（★透传必错：资源期 count=0 下 blob 槽 14 按
                // 0==0 恒 1 是误判值；v2 :292-296 合成式同源）
                dst[KFC_UNIFIED_COLLECT_ALL_INPUT_OUTPUT_EQUAL] =
                    (dst[KFC_UNIFIED_COLLECT_ALL_INPUT] ==
                     dst[KFC_UNIFIED_COLLECT_ALL_OUTPUT] + dst[KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET]) ?
                        1U : 0U;
                // 透传槽（0/9/10/11/12）已由拷贝落值，不覆写
            }
        } else {
            // 哨兵帧：槽 0 = INVALID_U64（server 出口判定 param[0]==INVALID_U64），
            // 其余槽清 0（Finalize 先例只写槽 0；此处全清更干净——server 只读槽 0 判
            // 出口，后续槽不消费，不违背契约）
            for (uint32_t i = 0; i < KFC_UNIFIED_PARAM_NUM_PER_RECORD; ++i) {
                dst[i] = 0ULL;
            }
            dst[KFC_UNIFIED_ACTION_ID] = 0xFFFFFFFFFFFFFFFFULL;   // INVALID_U64 出口哨兵
        }
        // flush：MAJOR-3（M2 审查修正）——cache line 实为 64B（FlushDataCache 只刷新
        // 传入地址所在的 line），v3 帧 32 槽 = 256B = 4 行。镜像 v2 节奏
        // for(i<(32+7)/8)（CcuSendMsg 同折算式，步长 MAX_DCCI_CNT）——4 次覆盖全帧，
        // 槽 16..31（P2 的 RS NHR 24 槽/并发 RS 25 槽在此区间）不再漏落 HBM。
        for (uint32_t i = 0; i < ((KFC_UNIFIED_PARAM_NUM_PER_RECORD + 7U) / 8U); ++i) {
            FlushDataCache(frameAddr + MAX_DCCI_CNT * i);
        }

        // ③ 逐帧 commit 信号（M0-① 裁定：帧粒度；missionNum=1 单 server，
        // GetCommitCkeAddr(frame, 0) 与 server 侧 waitAddr+frame*8 对称）
        ccuMsg_.commitCKEAddr = GetCommitCkeAddr(static_cast<uint8_t>(frame), 0U);
        WriteHBMData(reinterpret_cast<__gm__ uint64_t*>(ccuMsg_.commitCKEAddr), CCU_MSG_CKE_SET_VALUE);
    }

    KERNEL_LOG(
        KERNEL_INFO, "ApiClient CcuPrepareForUnifiedPlan done: [%llu] frames written + signaled.",
        (unsigned long long)totalFrames);
}
} // namespace AscendC

#endif
