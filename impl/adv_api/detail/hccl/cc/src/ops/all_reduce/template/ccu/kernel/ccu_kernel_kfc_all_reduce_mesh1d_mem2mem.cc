/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {

constexpr int AR_M2M_INPUT_XN_ID = 0;
constexpr int AR_M2M_OUTPUT_XN_ID = 1;
constexpr int AR_M2M_TOKEN_XN_ID = 2;
constexpr int AR_M2M_POST_SYNC_CKE_IDX = 0;
constexpr int AR_M2M_INPUT_CKE_IDX = 1;
constexpr int AR_M2M_OUTPUT_CKE_IDX = 2;
constexpr int AR_M2M_TOKEN_CKE_IDX = 3;
constexpr uint16_t AR_M2M_POST_SYNC_BIT = 1U;
constexpr uint16_t AR_M2M_INPUT_BIT = 1U << AR_M2M_INPUT_XN_ID;
constexpr uint16_t AR_M2M_OUTPUT_BIT = 1U << AR_M2M_OUTPUT_XN_ID;
constexpr uint16_t AR_M2M_TOKEN_BIT = 1U << AR_M2M_TOKEN_XN_ID;
constexpr uint32_t AR_M2M_BITS_PER_EVENT = 16;

struct KfcAllReduceMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};
    HcclDataType dataType{HcclDataType::HCCL_DATA_TYPE_RESERVED};
    HcclDataType outputDataType{HcclDataType::HCCL_DATA_TYPE_RESERVED};
    HcclReduceOp reduceOp{HcclReduceOp::HCCL_REDUCE_RESERVED};

    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable scratchAddr;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable mySliceSize;
    ccu::Variable sliceOffset;
    ccu::Variable sliceSize;
    std::vector<ccu::RemoteAddr> reduceSrc;
    std::vector<ccu::LocalAddr> scratch;
    std::vector<ccu::Event> events;
};

static uint16_t GetEventMask(uint32_t rankSize, uint32_t eventIdx)
{
    const uint32_t firstRank = eventIdx * AR_M2M_BITS_PER_EVENT;
    const uint32_t remainingRanks = rankSize - firstRank;
    const uint32_t bitCount = std::min(remainingRanks, AR_M2M_BITS_PER_EVENT);
    return bitCount == AR_M2M_BITS_PER_EVENT ? UINT16_MAX : static_cast<uint16_t>((1U << bitCount) - 1);
}

static CcuResult InitResource(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    if (ctx.rankSize == 0 || ctx.rankId >= ctx.rankSize || ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcAllReduceMesh1DMem2Mem] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; ++peerId) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_M2M_INPUT_XN_ID);
        ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_M2M_OUTPUT_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_M2M_TOKEN_XN_ID);
        ++channelIdx;
    }

    ctx.reduceSrc.resize(ctx.rankSize);
    ctx.scratch.resize(ctx.rankSize);
    ctx.events.resize((ctx.rankSize + AR_M2M_BITS_PER_EVENT - 1) / AR_M2M_BITS_PER_EVENT);
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(
            ccu::WriteVariableWithNotify(
                ctx.channels[i], ctx.input[ctx.rankId], AR_M2M_INPUT_XN_ID, AR_M2M_INPUT_CKE_IDX, AR_M2M_INPUT_BIT));
        CCU_CHK_RET(
            ccu::WriteVariableWithNotify(
                ctx.channels[i], ctx.output[ctx.rankId], AR_M2M_OUTPUT_XN_ID, AR_M2M_OUTPUT_CKE_IDX,
                AR_M2M_OUTPUT_BIT));
        CCU_CHK_RET(
            ccu::WriteVariableWithNotify(
                ctx.channels[i], ctx.token[ctx.rankId], AR_M2M_TOKEN_XN_ID, AR_M2M_TOKEN_CKE_IDX, AR_M2M_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_M2M_INPUT_CKE_IDX, AR_M2M_INPUT_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_M2M_OUTPUT_CKE_IDX, AR_M2M_OUTPUT_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_M2M_TOKEN_CKE_IDX, AR_M2M_TOKEN_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], AR_M2M_POST_SYNC_CKE_IDX, AR_M2M_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_M2M_POST_SYNC_CKE_IDX, AR_M2M_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult ReadInputsToScratch(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    ccu::Variable scratchOffset;
    scratchOffset = 0;
    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; ++rankIdx) {
        ctx.reduceSrc[rankIdx].addr = ctx.input[rankIdx];
        ctx.reduceSrc[rankIdx].addr += ctx.sliceOffset;
        ctx.reduceSrc[rankIdx].token = ctx.token[rankIdx];
        ctx.scratch[rankIdx].addr = ctx.scratchAddr;
        ctx.scratch[rankIdx].addr += scratchOffset;
        ctx.scratch[rankIdx].token = ctx.token[ctx.rankId];
        scratchOffset += ctx.sliceSize;

        const uint32_t eventIdx = rankIdx / AR_M2M_BITS_PER_EVENT;
        const uint16_t rankMask = 1U << (rankIdx % AR_M2M_BITS_PER_EVENT);
        if (rankIdx == ctx.rankId) {
            ccu::LocalAddr localSrc;
            localSrc.addr = ctx.input[rankIdx];
            localSrc.addr += ctx.sliceOffset;
            localSrc.token = ctx.token[rankIdx];
            CCU_CHK_RET(ccu::LocalCopy(
                ctx.scratch[rankIdx], localSrc, ctx.sliceSize, ctx.events[eventIdx], rankMask));
        } else {
            CCU_CHK_RET(
                ccu::Read(
                    ctx.channels[channelIdx], ctx.scratch[rankIdx], ctx.reduceSrc[rankIdx], ctx.sliceSize,
                    ctx.events[eventIdx], rankMask));
            ++channelIdx;
        }
    }
    for (uint32_t eventIdx = 0; eventIdx < ctx.events.size(); ++eventIdx) {
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], GetEventMask(ctx.rankSize, eventIdx)));
    }
    return CCU_SUCCESS;
}

static CcuResult PairwiseLocalReduce(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    ccu::Variable len;
    uint32_t remainPieces = ctx.rankSize;
    while (remainPieces > 1) {
        const uint32_t reducePieces = remainPieces / 2;
        const uint32_t srcIdx = remainPieces - reducePieces;
        len = ctx.sliceSize;
        for (uint32_t i = 1; i < reducePieces; ++i) {
            len += ctx.sliceSize;
        }
        CCU_CHK_RET(
            ccu::LocalReduce(ctx.scratch[0], ctx.scratch[srcIdx], len, ctx.dataType, ctx.reduceOp, ctx.events[0], 1));
        CCU_CHK_RET(ccu::EventWait(ctx.events[0], 1));
        remainPieces -= reducePieces;
    }

    ccu::LocalAddr localOutput;
    localOutput.addr = ctx.output[ctx.rankId];
    localOutput.addr += ctx.sliceOffset;
    localOutput.token = ctx.token[ctx.rankId];
    CCU_CHK_RET(ccu::LocalCopy(localOutput, ctx.scratch[0], ctx.sliceSize, ctx.events[0], 1));
    CCU_CHK_RET(ccu::EventWait(ctx.events[0], 1));
    return CCU_SUCCESS;
}

static CcuResult BroadcastResult(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    ccu::LocalAddr src;
    src.addr = ctx.output[ctx.rankId];
    src.addr += ctx.sliceOffset;
    src.token = ctx.token[ctx.rankId];

    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; ++rankIdx) {
        const uint32_t eventIdx = rankIdx / AR_M2M_BITS_PER_EVENT;
        const uint16_t rankMask = 1U << (rankIdx % AR_M2M_BITS_PER_EVENT);
        if (rankIdx == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
            continue;
        }
        ccu::RemoteAddr dst;
        dst.addr = ctx.output[rankIdx];
        dst.addr += ctx.sliceOffset;
        dst.token = ctx.token[rankIdx];
        CCU_CHK_RET(ccu::Write(ctx.channels[channelIdx], dst, src, ctx.sliceSize, ctx.events[eventIdx], rankMask));
        ++channelIdx;
    }
    for (uint32_t eventIdx = 0; eventIdx < ctx.events.size(); ++eventIdx) {
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], GetEventMask(ctx.rankSize, eventIdx)));
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratchAddr,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable mySliceSize, ccu::Variable sliceOffset,
    HcclDataType dataType, HcclDataType outputDataType, HcclReduceOp reduceOp, const ChannelHandle channels[],
    uint32_t channelCount, uint32_t rankSize, uint32_t rankId)
{
    KfcAllReduceMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED ? dataType : outputDataType;
    ctx.reduceOp = reduceOp;
    ctx.scratchAddr = scratchAddr;
    ctx.normalSliceSize = normalSliceSize;
    ctx.lastSliceSize = lastSliceSize;
    ctx.mySliceSize = mySliceSize;
    ctx.sliceOffset = sliceOffset;
    ctx.input.resize(rankSize);
    ctx.output.resize(rankSize);
    ctx.token.resize(rankSize);
    ctx.input[rankId] = inputAddr;
    ctx.output[rankId] = outputAddr;
    ctx.token[rankId] = tokenInfo;
    ctx.sliceSize = rankId + 1 == rankSize ? lastSliceSize : normalSliceSize;
    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(PreSync(ctx));
    CCU_IF(ctx.mySliceSize != 0)
    {
        CCU_CHK_RET(ReadInputsToScratch(ctx));
        CCU_CHK_RET(PairwiseLocalReduce(ctx));
        CCU_CHK_RET(BroadcastResult(ctx));
    }
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
