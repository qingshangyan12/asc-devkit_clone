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

constexpr int AR_M2M_INPUT_XN_ID = 0;
constexpr int AR_M2M_OUTPUT_XN_ID = 1;
constexpr int AR_M2M_TOKEN_XN_ID = 2;
constexpr int AR_M2M_POST_SYNC_ID = 3;
constexpr int AR_M2M_CKE_IDX_0 = 0;
constexpr uint16_t AR_M2M_BITS_PER_EVENT = 16;

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
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable mySliceSize;
    ccu::Variable sliceOffset;
    ccu::Variable isInputOutputEqual;
    GroupOpSizeVars goSize;
    ccu::Variable sliceSize;
    std::vector<ccu::Event> events;

    ccu::LocalAddr srcMem;
    ccu::LocalAddr localDstMem;
    ccu::RemoteAddr remoteDstMem;
    std::vector<ccu::RemoteAddr> reduceScatterSrc;
    std::vector<ccu::LocalAddr> reduceScatterDst;
};

static CcuResult InitResource(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuKfcAllReduceMesh1DMem2Mem] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }
    HCCL_INFO("[CcuKfcAllReduceMesh1DMem2Mem] channels.size: [%u]", ctx.channelCount);

    ctx.input.resize(ctx.rankSize);
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_M2M_INPUT_XN_ID);
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_M2M_OUTPUT_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_M2M_TOKEN_XN_ID);
            channelIdx++;
        }
    }

    ctx.reduceScatterSrc.resize(ctx.rankSize);
    ctx.reduceScatterDst.resize(ctx.rankSize);
    const uint32_t eventNum = (ctx.rankSize + AR_M2M_BITS_PER_EVENT - 1) / AR_M2M_BITS_PER_EVENT;
    ctx.events.resize(eventNum);
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcAllReduceMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable scratchAddr, ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable mySliceSize, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.myScratch = scratchAddr;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = currentRankSliceOutputOffset;
    ctx.normalSliceSize = normalSliceSize;
    ctx.lastSliceSize = lastSliceSize;
    ctx.mySliceSize = mySliceSize;
    ctx.sliceOffset = sliceOffset;
    ctx.isInputOutputEqual = isInputOutputEqual;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.input[ctx.rankId], AR_M2M_INPUT_XN_ID, AR_M2M_CKE_IDX_0, 1 << AR_M2M_INPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[ctx.rankId], AR_M2M_OUTPUT_XN_ID, AR_M2M_CKE_IDX_0, 1 << AR_M2M_OUTPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], AR_M2M_TOKEN_XN_ID, AR_M2M_CKE_IDX_0, 1 << AR_M2M_TOKEN_XN_ID));
    }

    uint32_t allBit = (1 << AR_M2M_INPUT_XN_ID) | (1 << AR_M2M_OUTPUT_XN_ID) | (1 << AR_M2M_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_M2M_CKE_IDX_0, allBit));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], AR_M2M_CKE_IDX_0, 1 << AR_M2M_POST_SYNC_ID));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_M2M_CKE_IDX_0, 1 << AR_M2M_POST_SYNC_ID));
    }
    return CCU_SUCCESS;
}

static CcuResult ReduceRmtToLoc(
    KfcAllReduceMesh1DMem2MemContext& ctx, const std::vector<ccu::Variable>& srcAddr, const ccu::Variable& dstAddr)
{
    ctx.localDstMem.addr = dstAddr;
    ctx.localDstMem.addr += ctx.sliceOffset;
    ctx.localDstMem.token = ctx.token[ctx.rankId];

    ccu::Variable scratchOffset = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        ctx.reduceScatterSrc[rankIdx].addr = srcAddr[rankIdx];
        ctx.reduceScatterSrc[rankIdx].addr += ctx.sliceOffset;
        ctx.reduceScatterSrc[rankIdx].token = ctx.token[rankIdx];

        ctx.reduceScatterDst[rankIdx].addr = ctx.myScratch;
        ctx.reduceScatterDst[rankIdx].addr += scratchOffset;
        scratchOffset += ctx.sliceSize;
        ctx.reduceScatterDst[rankIdx].token = ctx.token[ctx.rankId];
    }

    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = rankIdx / AR_M2M_BITS_PER_EVENT;
        if (rankIdx == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], 1 << (rankIdx % AR_M2M_BITS_PER_EVENT)));
        } else {
            CCU_CHK_RET(ccu::Read(
                ctx.channels[channelIdx], ctx.reduceScatterDst[rankIdx], ctx.reduceScatterSrc[rankIdx], ctx.sliceSize,
                ctx.events[eventIdx], 1 << (rankIdx % AR_M2M_BITS_PER_EVENT)));
            channelIdx++;
        }
    }
    uint32_t eventNum = (ctx.rankSize + AR_M2M_BITS_PER_EVENT - 1) / AR_M2M_BITS_PER_EVENT;
    for (uint32_t i = 0; i < eventNum; i++) {
        uint32_t sigNum = AR_M2M_BITS_PER_EVENT;
        if (ctx.rankSize % AR_M2M_BITS_PER_EVENT != 0 && i == (eventNum - 1)) {
            sigNum = ctx.rankSize % AR_M2M_BITS_PER_EVENT;
        }
        CCU_CHK_RET(ccu::EventWait(ctx.events[i], (1 << sigNum) - 1));
    }

    uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    ccu::LocalAddr reduceDst = ctx.localDstMem;
    if (expansionNum != 1) {
        ccu::Variable tmp = GetExpansionParam(expansionNum);
        reduceDst.token = reduceDst.token + tmp;
    }
    CCU_CHK_RET(ccu::LocalCopy(reduceDst, ctx.reduceScatterDst[0], ctx.sliceSize, ctx.events[0], 1));
    CCU_CHK_RET(ccu::EventWait(ctx.events[0], 1));
    for (uint32_t i = 1; i < ctx.rankSize; i++) {
        CCU_CHK_RET(ccu::LocalReduce(
            reduceDst, ctx.reduceScatterDst[i], ctx.sliceSize, ctx.dataType, ctx.reduceOp, ctx.events[0], 1));
        CCU_CHK_RET(ccu::EventWait(ctx.events[0], 1));
    }
    return CCU_SUCCESS;
}

static CcuResult BcastLocToRmt(
    KfcAllReduceMesh1DMem2MemContext& ctx, const ccu::Variable& srcAddr, const std::vector<ccu::Variable>& dstAddr)
{
    if (dstAddr.size() != ctx.channelCount + 1) {
        HCCL_ERROR("[BcastLocToRmt] dstAddr.size[%zu] != channels_ size[%u] + 1", dstAddr.size(), ctx.channelCount);
        return CCU_SUCCESS;
    }
    ctx.srcMem.addr = srcAddr;
    ctx.srcMem.addr += ctx.sliceOffset;
    ctx.srcMem.token = ctx.token[ctx.rankId];

    uint32_t channelIdx = 0;
    for (uint32_t rmtId = 0; rmtId < dstAddr.size(); rmtId++) {
        uint32_t eventIdx = rmtId / AR_M2M_BITS_PER_EVENT;
        if (rmtId == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], 1 << (rmtId % AR_M2M_BITS_PER_EVENT)));
            continue;
        }
        ctx.remoteDstMem.addr = dstAddr[rmtId];
        ctx.remoteDstMem.addr += ctx.sliceOffset;
        ctx.remoteDstMem.token = ctx.token[rmtId];

        CCU_CHK_RET(ccu::Write(
            ctx.channels[channelIdx], ctx.remoteDstMem, ctx.srcMem, ctx.sliceSize, ctx.events[eventIdx],
            1 << (rmtId % AR_M2M_BITS_PER_EVENT)));
        channelIdx++;
    }
    uint32_t eventNum = (ctx.rankSize + AR_M2M_BITS_PER_EVENT - 1) / AR_M2M_BITS_PER_EVENT;
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        uint32_t sigNum = AR_M2M_BITS_PER_EVENT;
        if (ctx.rankSize % AR_M2M_BITS_PER_EVENT != 0 && eventIdx == (eventNum - 1)) {
            sigNum = ctx.rankSize % AR_M2M_BITS_PER_EVENT;
        }
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], (1 << sigNum) - 1));
    }
    return CCU_SUCCESS;
}

static CcuResult DoAllReduce(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    if (ctx.rankId != ctx.rankSize - 1) {
        ctx.sliceSize = ctx.normalSliceSize;
    } else {
        ctx.sliceSize = ctx.lastSliceSize;
    }
    CCU_CHK_RET(ReduceRmtToLoc(ctx, ctx.input, ctx.output[ctx.rankId]));
    CCU_CHK_RET(BcastLocToRmt(ctx, ctx.output[ctx.rankId], ctx.output));
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratchAddr,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable mySliceSize, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    HcclDataType dataType, HcclDataType outputType, HcclReduceOp reduceOp)
{
    KfcAllReduceMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputType;
    if (ctx.outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        ctx.outputDataType = dataType;
    }
    ctx.reduceOp = reduceOp;
    InitCcuKernelCtxBase(ctx);

    HCCL_INFO("[CcuKfcAllReduceMesh1DMem2Mem] AllReduceMesh1DMem2Mem run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, scratchAddr, currentRankSliceInputOffset, currentRankSliceOutputOffset,
        normalSliceSize, lastSliceSize, mySliceSize, sliceOffset, isInputOutputEqual, goSize0, goSize1, goSize2,
        goSize3));
    CCU_CHK_RET(PreSync(ctx));
    CCU_IF(ctx.mySliceSize != 0) { CCU_CHK_RET(DoAllReduce(ctx)); }
    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CcuKfcAllReduceMesh1DMem2Mem] AllReduceMesh1DMem2Mem end");
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
