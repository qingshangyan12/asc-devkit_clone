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
#include "../ccu_temp_kfc_all_gather_nhr_1D_multi_jetty_mem2mem.h"
#include "ccu_func_dl.hpp"

namespace mc2_ops_hccl {
namespace {
constexpr uint16_t NHR_OUTPUT_XN_ID = 1U;
constexpr uint16_t NHR_TOKEN_XN_ID = 2U;
constexpr uint16_t NHR_POST_SYNC_BIT = 1U << 3U;
constexpr uint16_t NHR_STEP_PRE_SYNC_BIT = 1U << 4U;
constexpr uint16_t NHR_STEP_POST_SYNC_BIT = 1U << 5U;
constexpr uint16_t NHR_CKE_IDX = 0U;

struct KfcAllGatherNhrContext : CcuKernelCtxBase {
    const ChannelHandle* channels = nullptr;
    uint32_t channelCount = 0;
    uint32_t rankSize = 0;
    uint32_t rankId = 0;
    uint32_t jettyNum = 1;
    const std::vector<KfcNhrStepInfo>* stepInfoVector = nullptr;
    const std::map<uint32_t, uint32_t>* rank2ChannelIdx = nullptr;

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable sliceSize;
    ccu::Variable sliceSizePerJetty;
    ccu::Variable lastSliceSizePerJetty;
    ccu::Variable repeatNumInv;
    ccu::Variable inputSliceStride;
    ccu::Variable outputSliceStride;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable isInputOutputEqual;
    GroupOpSizeVars goSize;
    std::vector<ccu::Variable> outputSliceOffset;
    ccu::Variable myRankInputSliceOffset;
    ccu::Variable repeatFlag;
    ccu::Variable repeatNum;
    ccu::Variable constVar1;
    ccu::Event event;
};

struct KfcParallelAllGatherNhrContext : KfcAllGatherNhrContext {
    uint32_t rankSizeLevel0 = 0;
    uint32_t rankIdxLevel0 = 0;
    ccu::Variable parallelInputBase;
    ccu::Variable parallelOutputBase;
    ccu::Variable parallelToken;
    ccu::Variable outputStride;
    ccu::Variable part0Size;
    ccu::Variable part1Size;
    ccu::Variable part1Offset;
    ccu::Variable meshPhaseDoneAddr;
    ccu::Variable nhrPhaseDoneAddr;
    ccu::Variable part0SliceSizePerJetty;
    ccu::Variable part0LastSliceSizePerJetty;
    ccu::Variable part1SliceSizePerJetty;
    ccu::Variable part1LastSliceSizePerJetty;
    GroupOpSizeVars part0GoSize;
    GroupOpSizeVars part1GoSize;
};

CcuResult InitResource(KfcAllGatherNhrContext& ctx)
{
    if (ctx.rankSize == 0U || ctx.rankId >= ctx.rankSize || ctx.channelCount == 0U || ctx.rank2ChannelIdx == nullptr ||
        ctx.rank2ChannelIdx->size() != ctx.channelCount || ctx.jettyNum == 0U || ctx.jettyNum > 16U) {
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

CcuResult LoadArgs(KfcAllGatherNhrContext& ctx, ccu::Array<ccu::Variable>& args)
{
    const uint32_t localIdx = ctx.channelCount;
    ctx.token[localIdx] = args[2];
    ctx.input = args[KFC_CONCURRENT_AG_NHR_INPUT];
    ctx.output[localIdx] = args[KFC_CONCURRENT_AG_NHR_OUTPUT];
    ctx.sliceSize = args[KFC_CONCURRENT_AG_NHR_SLICE_SIZE];
    ctx.sliceSizePerJetty = args[KFC_CONCURRENT_AG_NHR_SLICE_SIZE_PER_JETTY];
    ctx.lastSliceSizePerJetty = args[KFC_CONCURRENT_AG_NHR_LAST_SLICE_SIZE_PER_JETTY];
    ctx.repeatNumInv = args[KFC_CONCURRENT_AG_NHR_REPEAT_NUM_INV];
    ctx.inputSliceStride = args[KFC_CONCURRENT_AG_NHR_INPUT_SLICE_STRIDE];
    ctx.outputSliceStride = args[KFC_CONCURRENT_AG_NHR_OUTPUT_SLICE_STRIDE];
    ctx.inputRepeatStride = args[KFC_CONCURRENT_AG_NHR_INPUT_REPEAT_STRIDE];
    ctx.outputRepeatStride = args[KFC_CONCURRENT_AG_NHR_OUTPUT_REPEAT_STRIDE];
    ctx.isInputOutputEqual = args[KFC_CONCURRENT_AG_NHR_INPUT_OUTPUT_EQUAL];
    ctx.goSize.addrOffset = args[KFC_CONCURRENT_AG_NHR_GO_SIZE_0];
    ctx.goSize.loopParam = args[KFC_CONCURRENT_AG_NHR_GO_SIZE_1];
    ctx.goSize.parallelParam = args[KFC_CONCURRENT_AG_NHR_GO_SIZE_2];
    ctx.goSize.residual = args[KFC_CONCURRENT_AG_NHR_GO_SIZE_3];
    return CCU_SUCCESS;
}

CcuResult LoadArgs(KfcParallelAllGatherNhrContext& ctx, ccu::Array<ccu::Variable>& args)
{
    ctx.parallelInputBase = args[0];
    ctx.parallelOutputBase = args[1];
    ctx.parallelToken = args[2];
    ctx.outputStride = args[KFC_PARALLEL_AG_OUTPUT_STRIDE];
    ctx.part0Size = args[KFC_PARALLEL_AG_PART0_SIZE];
    ctx.part1Size = args[KFC_PARALLEL_AG_PART1_SIZE];
    ctx.part1Offset = args[KFC_PARALLEL_AG_PART1_OFFSET];
    ctx.meshPhaseDoneAddr = args[KFC_PARALLEL_AG_MESH_PHASE_DONE_ADDR];
    ctx.nhrPhaseDoneAddr = args[KFC_PARALLEL_AG_NHR_PHASE_DONE_ADDR];
    ctx.part0SliceSizePerJetty = args[KFC_PARALLEL_AG_PART0_SLICE_PER_JETTY];
    ctx.part0LastSliceSizePerJetty = args[KFC_PARALLEL_AG_PART0_LAST_SLICE_PER_JETTY];
    ctx.part1SliceSizePerJetty = args[KFC_PARALLEL_AG_PART1_SLICE_PER_JETTY];
    ctx.part1LastSliceSizePerJetty = args[KFC_PARALLEL_AG_PART1_LAST_SLICE_PER_JETTY];
    ctx.part0GoSize.addrOffset = args[KFC_PARALLEL_AG_PART0_GO_SIZE_0];
    ctx.part0GoSize.loopParam = args[KFC_PARALLEL_AG_PART0_GO_SIZE_1];
    ctx.part0GoSize.parallelParam = args[KFC_PARALLEL_AG_PART0_GO_SIZE_2];
    ctx.part0GoSize.residual = args[KFC_PARALLEL_AG_PART0_GO_SIZE_3];
    ctx.part1GoSize.addrOffset = args[KFC_PARALLEL_AG_PART1_GO_SIZE_0];
    ctx.part1GoSize.loopParam = args[KFC_PARALLEL_AG_PART1_GO_SIZE_1];
    ctx.part1GoSize.parallelParam = args[KFC_PARALLEL_AG_PART1_GO_SIZE_2];
    ctx.part1GoSize.residual = args[KFC_PARALLEL_AG_PART1_GO_SIZE_3];
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
        // WriteSlice 已等待并清除 event；本地 copy 每次也立即等待，复用 bit0。
        // 不能用 1<<rankId：NHR 子域可以超过 16 rank，而 event mask 只有 16 位。
        constexpr uint16_t rankMask = 1U;
        CCU_IF(ctx.isInputOutputEqual == 0)
        {
            CCU_CHK_RET(GroupCopy(ctx, dst, src, ctx.goSize, GetCcuVersion()));
            CCU_CHK_RET(ccu::EventRecord(ctx.event, rankMask));
        }
        CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.event, rankMask)); }
        CCU_CHK_RET(ccu::EventWait(ctx.event, rankMask));
        ctx.repeatFlag = 1U;
    }
    return CCU_SUCCESS;
}

CcuResult LoadNhrArgs(
    KfcAllGatherNhrContext& ctx, ccu::Variable input, ccu::Variable output, ccu::Variable token,
    ccu::Variable sliceSize, ccu::Variable sliceSizePerJetty, ccu::Variable lastSliceSizePerJetty,
    ccu::Variable repeatNumInv, ccu::Variable inputSliceStride, ccu::Variable outputSliceStride,
    ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride, ccu::Variable isInputOutputEqual,
    ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3)
{
    const uint32_t localIdx = ctx.channelCount;
    ctx.input = input;
    ctx.output[localIdx] = output;
    ctx.token[localIdx] = token;
    ctx.sliceSize = sliceSize;
    ctx.sliceSizePerJetty = sliceSizePerJetty;
    ctx.lastSliceSizePerJetty = lastSliceSizePerJetty;
    ctx.repeatNumInv = repeatNumInv;
    ctx.inputSliceStride = inputSliceStride;
    ctx.outputSliceStride = outputSliceStride;
    ctx.inputRepeatStride = inputRepeatStride;
    ctx.outputRepeatStride = outputRepeatStride;
    ctx.isInputOutputEqual = isInputOutputEqual;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;
    ctx.constVar1 = 1U;
    return CCU_SUCCESS;
}

void AddVariableNTimes(ccu::Variable& result, const ccu::Variable& value, uint32_t times)
{
    result = 0U;
    for (uint32_t i = 0; i < times; ++i) {
        result += value;
    }
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
    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadArgs(ctx, args);
            ctx.constVar1 = 1U;
            PreSync(ctx);
            CCU_IF(ctx.sliceSize != 0) { RunNhr(ctx); }
            PostSync(ctx);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_ALL_GATHER_NHR1D_MULTI_JETTY_MEM2MEM_NAME);
    return CCU_SUCCESS;
}

CcuResult CcuKfcParallelAllGatherNHR1DMultiJettyMem2MemKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgKfcAllGatherNHR1DMultiJettyMem2Mem*>(arg);
    const auto* parallelArg = static_cast<const KfcParallelAllGatherArg*>(kernelArg->algArg.get());
    KfcParallelAllGatherNhrContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = parallelArg->rankSizeLevel1;
    ctx.rankId = parallelArg->rankIdxLevel1;
    ctx.rankSizeLevel0 = parallelArg->rankSizeLevel0;
    ctx.rankIdxLevel0 = parallelArg->rankIdxLevel0;
    ctx.jettyNum = parallelArg->jettyNum;
    ctx.stepInfoVector = &parallelArg->stepInfoVector;
    ctx.rank2ChannelIdx = &parallelArg->rank2ChannelIdx;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));
    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadArgs(ctx, args);
            ccu::Variable zero;
            ccu::Variable one;
            ccu::Variable level1Stride;
            ccu::Variable phase0Input;
            ccu::Variable phase0Output;
            ccu::Variable phase1RepeatNumInv;
            ccu::Variable phase0RankOffset;
            ccu::Variable phase0RepeatNumInv;
            zero = 0U;
            one = 1U;
            AddVariableNTimes(level1Stride, ctx.outputStride, ctx.rankSizeLevel0);
            phase0Input = ctx.parallelInputBase + ctx.part1Offset;
            AddVariableNTimes(phase0RankOffset, ctx.outputStride, ctx.rankIdxLevel0);
            phase0Output = ctx.parallelOutputBase + phase0RankOffset;
            phase0Output += ctx.part1Offset;
            phase1RepeatNumInv = UINT64_MAX - ctx.rankSizeLevel0;
            phase0RepeatNumInv = UINT64_MAX - 1U;

            LoadNhrArgs(
                ctx, phase0Input, phase0Output, ctx.parallelToken, ctx.part1Size, ctx.part1SliceSizePerJetty,
                ctx.part1LastSliceSizePerJetty, phase0RepeatNumInv, zero, level1Stride, zero, zero, zero,
                ctx.part1GoSize.addrOffset, ctx.part1GoSize.loopParam, ctx.part1GoSize.parallelParam,
                ctx.part1GoSize.residual);
            PreSync(ctx);
            CCU_IF(ctx.part1Size != 0) { RunNhr(ctx); }
            PostSync(ctx);

            ccu::Store(ctx.nhrPhaseDoneAddr, one);
            ccu::Variable meshDone;
            meshDone = 0U;
            CCU_WHILE(meshDone != 1U) { ccu::Load(ctx.meshPhaseDoneAddr, meshDone); }

            LoadNhrArgs(
                ctx, ctx.parallelOutputBase, ctx.parallelOutputBase, ctx.parallelToken, ctx.part0Size,
                ctx.part0SliceSizePerJetty, ctx.part0LastSliceSizePerJetty, phase1RepeatNumInv, level1Stride,
                level1Stride, ctx.outputStride, ctx.outputStride, one, ctx.part0GoSize.addrOffset,
                ctx.part0GoSize.loopParam, ctx.part0GoSize.parallelParam, ctx.part0GoSize.residual);
            PreSync(ctx);
            CCU_IF(ctx.part0Size != 0) { RunNhr(ctx); }
            PostSync(ctx);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_PARALLEL_ALL_GATHER_NHR1D_MULTI_JETTY_MEM2MEM_NAME);
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
