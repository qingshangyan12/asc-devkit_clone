/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {
using namespace hcomm;

constexpr int INPUT_XN_ID = 0;
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;
constexpr uint16_t BIT_NUM_PER_CKE = 16;

struct KfcAllReduceMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable myScratch;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable sliceOffset;
    ccu::Variable isInputOutputEqual;
    ccu::Variable chunkSize;
    ccu::Variable currentSliceSize;
    ccu::Variable tailSize;
    ccu::Variable chunkLoopNum;

    std::vector<ccu::Event> events;

    ccu::LocalAddr srcMem;
    ccu::LocalAddr localDstMem;
    ccu::RemoteAddr remoteDstMem;
    std::vector<ccu::RemoteAddr> reduceScatterSrc;
    std::vector<ccu::LocalAddr> reduceScatterDst;
};

static CcuResult InitResource(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    uint32_t channelIdx = 0;

    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuAllReduceMeshMem2Mem1D] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }
    HCCL_INFO("[CcuAllReduceMeshMem2Mem1D] channels.size: [%u]", ctx.channelCount);

    ctx.input.resize(ctx.rankSize);
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], INPUT_XN_ID);
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], OUTPUT_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }

    ctx.reduceScatterSrc.resize(ctx.rankSize);
    ctx.reduceScatterDst.resize(ctx.rankSize);
    ctx.events.resize(ctx.rankSize);

    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcAllReduceMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable chunkSize, ccu::Variable tailSize, ccu::Variable chunkLoopNum, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.myScratch = scratch;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = currentRankSliceOutputOffset;
    ctx.chunkSize = chunkSize;
    ctx.tailSize = tailSize;
    ctx.chunkLoopNum = chunkLoopNum;
    ctx.sliceOffset = sliceOffset;
    ctx.isInputOutputEqual = isInputOutputEqual;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::WriteVariableWithNotify(ctx.channels[i], ctx.input[ctx.rankId], INPUT_XN_ID, CKE_IDX_0, 1 << INPUT_XN_ID);
        ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[ctx.rankId], OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID);
        ccu::WriteVariableWithNotify(ctx.channels[i], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID);
    }

    uint32_t allBit = (1 << INPUT_XN_ID) | (1 << OUTPUT_XN_ID) | (1 << TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID);
    }
    HCCL_INFO("[CcuKernelAllReduceMeshMem2Mem1D] AllReduceMeshMem2Mem1D AllReduce GroupWait end");
    return CCU_SUCCESS;
}

static CcuResult BcastLocToRmt(
    KfcAllReduceMesh1DMem2MemContext& ctx, const ccu::Variable& srcAddr, const std::vector<ccu::Variable>& dstAddr)
{
    if (dstAddr.size() != ctx.channelCount + 1) {
        HCCL_ERROR("[BcastLocToRmt] dstAddr.size[%zu] != channels_ size[%zu] + 1", dstAddr.size(), ctx.channelCount);
        return CCU_SUCCESS;
    }
    ctx.srcMem.addr = srcAddr;
    ctx.srcMem.addr += ctx.sliceOffset;
    ctx.srcMem.token = ctx.token[ctx.rankId];

    uint32_t channelIdx = 0;
    for (uint32_t rmtId = 0; rmtId < dstAddr.size(); rmtId++) {
        uint32_t eventIdx = rmtId / BIT_NUM_PER_CKE;
        if (rmtId == ctx.rankId) {
            ccu::EventRecord(ctx.events[eventIdx], 1 << (rmtId % BIT_NUM_PER_CKE));
            continue;
        }
        ctx.remoteDstMem.addr = dstAddr[rmtId];
        ctx.remoteDstMem.addr += ctx.sliceOffset;
        ctx.remoteDstMem.token = ctx.token[rmtId];

        ccu::Write(
            ctx.channels[channelIdx], ctx.remoteDstMem, ctx.srcMem, ctx.currentSliceSize, ctx.events[eventIdx],
            1 << (rmtId % BIT_NUM_PER_CKE));
        channelIdx++;
    }
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && eventIdx == (eventNum - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        ccu::EventWait(ctx.events[eventIdx], (1 << sigNum) - 1);
    }
    return CCU_SUCCESS;
}

static CcuResult DoLocalReduce(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    ccu::LocalAddr reduceDst = ctx.localDstMem;
    if (expansionNum != 1) {
        ccu::Variable tmp;
        tmp = GetExpansionParam(expansionNum);
        reduceDst.token = reduceDst.token + tmp;
    }
    ccu::LocalCopy(reduceDst, ctx.reduceScatterDst[0], ctx.currentSliceSize, ctx.events[0], 1);
    ccu::EventWait(ctx.events[0], 1);
    for (uint32_t i = 1; i < ctx.rankSize; i++) {
        ccu::LocalReduce(
            reduceDst, ctx.reduceScatterDst[i], ctx.currentSliceSize, ctx.dataType, ctx.reduceOp, ctx.events[0], 1);
        ccu::EventWait(ctx.events[0], 1);
    }
    return CCU_SUCCESS;
}

static CcuResult ReduceRmtToLoc(
    KfcAllReduceMesh1DMem2MemContext& ctx, const std::vector<ccu::Variable>& srcAddr, const ccu::Variable& dstAddr)
{
    ccu::Variable scratchOffset;
    if (srcAddr.size() != ctx.channelCount + 1) {
        HCCL_ERROR("[ReduceRmtToLoc] srcAddr.size[%zu] != channels_ size[%zu] +1", srcAddr.size(), ctx.channelCount);
        return CCU_SUCCESS;
    }

    ctx.localDstMem.addr = dstAddr;
    ctx.localDstMem.addr += ctx.sliceOffset;
    ctx.localDstMem.token = ctx.token[ctx.rankId];

    scratchOffset = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        ctx.reduceScatterSrc[rankIdx].addr = srcAddr[rankIdx];
        ctx.reduceScatterSrc[rankIdx].addr += ctx.sliceOffset;
        ctx.reduceScatterSrc[rankIdx].token = ctx.token[rankIdx];

        ctx.reduceScatterDst[rankIdx].addr = ctx.myScratch;
        ctx.reduceScatterDst[rankIdx].addr += scratchOffset;
        scratchOffset += ctx.currentSliceSize;
        ctx.reduceScatterDst[rankIdx].token = ctx.token[ctx.rankId];
    }

    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = rankIdx / BIT_NUM_PER_CKE;
        if (rankIdx == ctx.rankId) {
            ccu::LocalAddr src;
            src.addr = ctx.reduceScatterSrc[rankIdx].addr;
            src.token = ctx.reduceScatterSrc[rankIdx].token;
            ccu::LocalCopy(
                ctx.reduceScatterDst[rankIdx], src, ctx.currentSliceSize, ctx.events[eventIdx],
                1 << (rankIdx % BIT_NUM_PER_CKE));
        } else {
            ccu::Read(
                ctx.channels[channelIdx], ctx.reduceScatterDst[rankIdx], ctx.reduceScatterSrc[rankIdx],
                ctx.currentSliceSize, ctx.events[eventIdx], 1 << (rankIdx % BIT_NUM_PER_CKE));
            channelIdx++;
        }
    }
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t i = 0; i < eventNum; i++) {
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && i == (eventNum - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        ccu::EventWait(ctx.events[i], (1 << sigNum) - 1);
    }
    CCU_CHK_RET(DoLocalReduce(ctx));
    return CCU_SUCCESS;
}

static CcuResult DoRepeatAllReduce(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    ccu::Variable one;
    one = 1;
    CCU_WHILE(ctx.chunkLoopNum != UINT64_MAX)
    {
        ctx.chunkLoopNum += one;
        ctx.currentSliceSize = ctx.chunkSize;
        CCU_CHK_RET(ReduceRmtToLoc(ctx, ctx.input, ctx.output[ctx.rankId]));
        CCU_CHK_RET(BcastLocToRmt(ctx, ctx.output[ctx.rankId], ctx.output));
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            ctx.input[rankIdx] += ctx.currentSliceSize;
            ctx.output[rankIdx] += ctx.currentSliceSize;
        }
    }

    ctx.currentSliceSize = ctx.tailSize;
    CCU_IF(ctx.currentSliceSize != 0)
    {
        CCU_CHK_RET(ReduceRmtToLoc(ctx, ctx.input, ctx.output[ctx.rankId]));
        CCU_CHK_RET(BcastLocToRmt(ctx, ctx.output[ctx.rankId], ctx.output));
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable chunkSize, ccu::Variable tailSize, ccu::Variable chunkLoopNum, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId, const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
{
    KfcAllReduceMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputType;
    if (ctx.outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        ctx.outputDataType = ctx.dataType;
        HCCL_DEBUG("[CcuMSAllReduceSoleMesh] outputDataType is [INVALID], set outputDataType to[%d]", ctx.dataType);
    }
    ctx.reduceOp = reduceType;
    HCCL_INFO(
        "[CcuKernelAllReduceMeshMem2Mem1D] Init, KernelArgs are rankId[%u], rankSize_[%u], dataType[%d], "
        "outputDataType[%d], reduceOp[%d]",
        ctx.rankId, ctx.rankSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp);

    InitCcuKernelCtxBase(ctx);

    HCCL_INFO("[CcuKernelAllReduceMeshMem2Mem1D] AllReduceMeshMem2Mem1D run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset, currentRankSliceOutputOffset,
        chunkSize, tailSize, chunkLoopNum, sliceOffset, isInputOutputEqual));
    CCU_CHK_RET(PreSync(ctx));

    CCU_CHK_RET(DoRepeatAllReduce(ctx));

    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CcuKernelAllReduceMeshMem2Mem1D] AllReduceMeshMem2Mem1D end");

    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
