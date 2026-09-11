/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_kfc_all_gather_nhr1d_multi_jetty_mem2mem.h"
#include "kfc_callable_task_registry.h"

namespace mc2_ops_hccl {
namespace {
constexpr uint16_t NHR_OUTPUT_XN_ID = 1U;
constexpr uint16_t NHR_TOKEN_XN_ID = 2U;
constexpr uint16_t NHR_POST_SYNC_BIT = 1U << 3U;
constexpr uint16_t NHR_STEP_PRE_SYNC_BIT = 1U << 4U;
constexpr uint16_t NHR_STEP_POST_SYNC_BIT = 1U << 5U;
constexpr uint16_t NHR_CKE_IDX = 0U;

CcuResult InitResource(KfcAllGatherNhrContext& ctx)
{
    if (ctx.rankSize == 0U || ctx.rankId >= ctx.rankSize || ctx.channelCount == 0U || ctx.rank2ChannelIdx == nullptr ||
        ctx.rank2ChannelIdx->size() != ctx.channelCount || ctx.jettyNum == 0U) {
        HCCL_ERROR(
            "[CcuKfcAllGatherNHR] invalid resource, rankSize[%u], rankId[%u], channels[%u], jetty[%u]", ctx.rankSize,
            ctx.rankId, ctx.channelCount, ctx.jettyNum);
        return CCU_E_INTERNAL;
    }
    ctx.output.resize(ctx.channelCount + 1U);
    ctx.token.resize(ctx.channelCount + 1U);
    for (uint32_t channelIdx = 0; channelIdx < ctx.channelCount; ++channelIdx) {
        ctx.output[channelIdx] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], NHR_OUTPUT_XN_ID);
        ctx.token[channelIdx] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], NHR_TOKEN_XN_ID);
    }
    ctx.outputSliceOffset.resize(ctx.rankSize);
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

CcuResult LoadArgs(KfcAllGatherNhrContext& ctx)
{
    const uint32_t localIdx = ctx.channelCount;
    CCU_CHK_RET(ccu::LoadArg(ctx.token[localIdx], 2));
    uint32_t argId = KFC_CONCURRENT_AG_NHR_INPUT;
    CCU_CHK_RET(ccu::LoadArg(ctx.input, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.output[localIdx], argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.sliceSize, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.sliceSizePerJetty, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.lastSliceSizePerJetty, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.repeatNumInv, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.inputSliceStride, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.outputSliceStride, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.inputRepeatStride, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.outputRepeatStride, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.isInputOutputEqual, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.addrOffset, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.loopParam, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.parallelParam, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.residual, argId++));
    return CCU_SUCCESS;
}

CcuResult PreSync(KfcAllGatherNhrContext& ctx)
{
    const uint32_t localIdx = ctx.channelCount;
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[localIdx], NHR_OUTPUT_XN_ID, NHR_CKE_IDX, 1U << NHR_OUTPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[localIdx], NHR_TOKEN_XN_ID, NHR_CKE_IDX, 1U << NHR_TOKEN_XN_ID));
    }
    const uint16_t waitMask = (1U << NHR_OUTPUT_XN_ID) | (1U << NHR_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], NHR_CKE_IDX, waitMask));
    }
    return CCU_SUCCESS;
}

CcuResult PostSync(KfcAllGatherNhrContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], NHR_CKE_IDX, NHR_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], NHR_CKE_IDX, NHR_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

CcuResult WriteSlice(KfcAllGatherNhrContext& ctx, uint32_t toRank, ccu::LocalAddr& source, ccu::RemoteAddr& destination)
{
    const ChannelHandle channel = ctx.channels[ctx.rank2ChannelIdx->at(toRank)];
    ccu::LocalAddr src = source;
    ccu::RemoteAddr dst = destination;
    CCU_IF(ctx.sliceSizePerJetty != 0)
    {
        for (uint32_t jetty = 0; jetty + 1U < ctx.jettyNum; ++jetty) {
            const uint16_t mask = 1U << jetty;
            CCU_CHK_RET(ccu::Write(channel, dst, src, ctx.sliceSizePerJetty, ctx.event, mask));
            src.addr += ctx.sliceSizePerJetty;
            dst.addr += ctx.sliceSizePerJetty;
        }
    }
    CCU_ELSE
    {
        for (uint32_t jetty = 0; jetty + 1U < ctx.jettyNum; ++jetty) {
            CCU_CHK_RET(ccu::EventRecord(ctx.event, 1U << jetty));
        }
    }
    const uint16_t lastMask = 1U << (ctx.jettyNum - 1U);
    CCU_IF(ctx.lastSliceSizePerJetty != 0)
    {
        CCU_CHK_RET(ccu::Write(channel, dst, src, ctx.lastSliceSizePerJetty, ctx.event, lastMask));
    }
    CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.event, lastMask)); }
    CCU_CHK_RET(ccu::EventWait(ctx.event, static_cast<uint16_t>((1U << ctx.jettyNum) - 1U)));
    return CCU_SUCCESS;
}

CcuResult RunStep(KfcAllGatherNhrContext& ctx, const KfcNhrStepInfo& stepInfo)
{
    const uint32_t toChannelIdx = ctx.rank2ChannelIdx->at(stepInfo.toRank);
    const uint32_t fromChannelIdx = ctx.rank2ChannelIdx->at(stepInfo.fromRank);
    const ChannelHandle sendChannel = ctx.channels[toChannelIdx];
    const ChannelHandle recvChannel = ctx.channels[fromChannelIdx];
    const uint32_t localIdx = ctx.channelCount;

    CCU_CHK_RET(ccu::NotifyRecord(recvChannel, NHR_CKE_IDX, NHR_STEP_PRE_SYNC_BIT));
    CCU_CHK_RET(ccu::NotifyWait(sendChannel, NHR_CKE_IDX, NHR_STEP_PRE_SYNC_BIT));
    for (uint32_t sliceIdx : stepInfo.txSliceIdxs) {
        ccu::LocalAddr src;
        if (sliceIdx == ctx.rankId) {
            src.addr = ctx.input;
            src.addr += ctx.myRankInputSliceOffset;
        } else {
            src.addr = ctx.output[localIdx];
            src.addr += ctx.outputSliceOffset[sliceIdx];
        }
        src.token = ctx.token[localIdx];
        ccu::RemoteAddr dst;
        dst.addr = ctx.output[toChannelIdx];
        dst.addr += ctx.outputSliceOffset[sliceIdx];
        dst.token = ctx.token[toChannelIdx];

        ctx.repeatFlag = 0;
        ctx.repeatNum = ctx.repeatNumInv;
        CCU_WHILE(ctx.repeatNum != UINT64_MAX)
        {
            ctx.repeatNum += ctx.constVar1;
            CCU_IF(ctx.repeatFlag != 0)
            {
                src.addr += ctx.inputRepeatStride;
                dst.addr += ctx.outputRepeatStride;
            }
            CCU_CHK_RET(WriteSlice(ctx, stepInfo.toRank, src, dst));
            ctx.repeatFlag = 1U;
        }
    }
    CCU_CHK_RET(ccu::NotifyRecord(sendChannel, NHR_CKE_IDX, NHR_STEP_POST_SYNC_BIT));
    CCU_CHK_RET(ccu::NotifyWait(recvChannel, NHR_CKE_IDX, NHR_STEP_POST_SYNC_BIT));
    return CCU_SUCCESS;
}

CcuResult RunNhr(KfcAllGatherNhrContext& ctx)
{
    ccu::Variable sliceOffset;
    sliceOffset = 0;
    ctx.myRankInputSliceOffset = 0;
    for (uint32_t rank = 0; rank < ctx.rankId; ++rank) {
        ctx.myRankInputSliceOffset += ctx.inputSliceStride;
    }
    for (uint32_t rank = 0; rank < ctx.rankSize; ++rank) {
        ctx.outputSliceOffset[rank] = sliceOffset;
        sliceOffset += ctx.outputSliceStride;
    }
    for (const auto& stepInfo : *ctx.stepInfoVector) {
        CCU_CHK_RET(RunStep(ctx, stepInfo));
    }

    const uint32_t localIdx = ctx.channelCount;
    ccu::LocalAddr src;
    src.addr = ctx.input;
    src.addr += ctx.myRankInputSliceOffset;
    src.token = ctx.token[localIdx];
    ccu::LocalAddr dst;
    dst.addr = ctx.output[localIdx];
    dst.addr += ctx.outputSliceOffset[ctx.rankId];
    dst.token = ctx.token[localIdx];
    ctx.repeatFlag = 0;
    ctx.repeatNum = ctx.repeatNumInv;
    CCU_WHILE(ctx.repeatNum != UINT64_MAX)
    {
        ctx.repeatNum += ctx.constVar1;
        CCU_IF(ctx.repeatFlag != 0)
        {
            src.addr += ctx.inputRepeatStride;
            dst.addr += ctx.outputRepeatStride;
        }
        const uint16_t rankMask = 1U << ctx.rankId;
        CCU_IF(ctx.isInputOutputEqual == 0)
        {
            CCU_CHK_RET(GroupCopy(ctx, dst, src, ctx.goSize));
            CCU_CHK_RET(ccu::EventRecord(ctx.event, rankMask));
        }
        CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.event, rankMask)); }
        CCU_CHK_RET(ccu::EventWait(ctx.event, rankMask));
        ctx.repeatFlag = 1U;
    }
    return CCU_SUCCESS;
}
} // namespace

CcuResult CcuKfcAllGatherNHR1DMultiJettyMem2MemKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgKfcAllGatherNHR1DMultiJettyMem2Mem*>(arg);
    KfcAllGatherNhrContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = static_cast<uint32_t>(kernelArg->rankSize);
    ctx.rankId = kernelArg->rankId;
    ctx.jettyNum = kernelArg->jettyNum;
    ctx.stepInfoVector = &kernelArg->stepInfoVector;
    ctx.rank2ChannelIdx = &kernelArg->rank2ChannelIdx;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx));
    ctx.constVar1 = 1U;

    CCU_CHK_RET(PreSync(ctx));
    CCU_IF(ctx.sliceSize != 0) { CCU_CHK_RET(RunNhr(ctx)); }
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

REGISTER_KFC_CALLABLE_TASK(
    HcclCMDType::HCCL_CMD_ALLGATHER, KfcServerRole::ALL_GATHER_NHR, CcuKernelAllGatherNHR1DMultiJettyMem2Mem,
    CcuKfcAllGatherNHR1DMultiJettyMem2MemKernel);

} // namespace mc2_ops_hccl
