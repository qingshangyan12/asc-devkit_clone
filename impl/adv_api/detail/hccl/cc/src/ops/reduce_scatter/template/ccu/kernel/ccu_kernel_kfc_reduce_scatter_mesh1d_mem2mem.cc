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
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {

constexpr int INPUT_XN_ID = 0;
constexpr int SCRATCH_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;

constexpr uint32_t BIT_NUM_PER_CKE = 16;
constexpr uint32_t REDUCE_MS_CNT = 8;
constexpr uint32_t REDUCE_SCATTER_LOOP_COUNT = 16;
constexpr uint32_t REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT = 8;
constexpr uint32_t RS_UNROLL_NUM = 16;

struct KfcReduceScatterMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> scratch;
    std::vector<ccu::Variable> token;
    ccu::Variable output;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable scratchRepeatStride;
    ccu::Variable repeatNum;
    ccu::Variable flag;
    GroupOpSizeVars goSize;

    std::vector<ccu::Event> events;
    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMem;
    ccu::Variable constVar1;
    ccu::Variable sliceSize;
    ccu::Variable readRepeatNum;
    ccu::Variable waitRepeatNum;
    std::array<std::vector<ccu::LocalAddr>, 2> loopScratch;
    ccu::LocalAddr loopSrc[2];
    ccu::LocalAddr loopDst[2];
    ccu::Variable loopLen[2];
    ccu::Variable loopLenExp[2];
};

static CcuResult InitResource(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    uint32_t channelIdx = 0;
    if (ctx.rankSize == 0 || ctx.rankId >= ctx.rankSize || ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[KfcKernelReduceScatterMesh1DMem2Mem] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    // Keep the V1 LoopGroup configuration identical to hccl. In particular,
    // msInterleave is eight while msPerLoop remains the AllocGoResource
    // default of one, so memSlice stays one CCU memory slice.
    ctx.moConfig.loopCount = REDUCE_SCATTER_LOOP_COUNT;
    ctx.moConfig.msInterleave = REDUCE_MS_CNT;
    ctx.moConfig.memSlice = CCU_MS_SIZE;
    HCCL_INFO(
        "[CCU_RS_KFC_DIAG] InitResource rank[%u/%u], channels[%u], loopCount[%u], msInterleave[%u], memSlice[%llu]",
        ctx.rankId, ctx.rankSize, ctx.channelCount, ctx.moConfig.loopCount, ctx.moConfig.msInterleave,
        static_cast<unsigned long long>(ctx.moConfig.memSlice));

    ctx.input.resize(ctx.rankSize);
    ctx.scratch.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    ctx.remoteInput.resize(ctx.rankSize);
    ctx.scratchMem.resize(ctx.rankSize);

    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], INPUT_XN_ID);
        ctx.scratch[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], SCRATCH_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
        channelIdx++;
    }

    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    ctx.events.resize(RS_UNROLL_NUM * eventNum);
    ctx.constVar1 = 1;
    HCCL_INFO(
        "[CCU_RS_KFC_DIAG] InitResource eventNum[%u], totalEvents[%u], externalScratchSlices[%u]", eventNum,
        RS_UNROLL_NUM * eventNum, ctx.rankSize);
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable scratchRepeatStride,
    ccu::Variable repeatNum, ccu::Variable goSizeAddrOffset, ccu::Variable goSizeLoopParam,
    ccu::Variable goSizeParallelParam, ccu::Variable goSizeResidual)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.scratch[ctx.rankId] = scratch;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = 0;
    ctx.inputRepeatStride = 0;
    ctx.outputRepeatStride = 0;
    ctx.normalSliceSize = normalSliceSize;
    ctx.lastSliceSize = lastSliceSize;
    ctx.scratchRepeatStride = scratchRepeatStride;
    ctx.repeatNum = repeatNum;
    ctx.goSize.addrOffset = goSizeAddrOffset;
    ctx.goSize.loopParam = goSizeLoopParam;
    ctx.goSize.parallelParam = goSizeParallelParam;
    ctx.goSize.residual = goSizeResidual;
    HCCL_INFO(
        "[CCU_RS_KFC_DIAG] LoadArgs rank[%u], compressedParams[normal,last,scratchStride,repeat,go0..3] loaded",
        ctx.rankId);
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    HCCL_DEBUG("[CCU_RS_KFC_DIAG] PreSync begin rank[%u], channels[%u]", ctx.rankId, ctx.channelCount);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.input[ctx.rankId], INPUT_XN_ID, CKE_IDX_0, 1 << INPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.scratch[ctx.rankId], SCRATCH_XN_ID, CKE_IDX_0, 1 << SCRATCH_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID));
    }
    uint32_t allBit = (1U << INPUT_XN_ID) | (1U << SCRATCH_XN_ID) | (1U << TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit));
    }
    HCCL_DEBUG("[CCU_RS_KFC_DIAG] PreSync registered rank[%u]", ctx.rankId);
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    HCCL_DEBUG("[CCU_RS_KFC_DIAG] PostSync begin rank[%u], channels[%u]", ctx.rankId, ctx.channelCount);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID));
    }
    HCCL_DEBUG("[CCU_RS_KFC_DIAG] PostSync registered rank[%u]", ctx.rankId);
    return CCU_SUCCESS;
}

static void InitReduceScatterAddr(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    ccu::Variable scratchOffset;
    scratchOffset = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx == ctx.rankId) {
            ctx.myInput.addr = ctx.input[rankIdx];
            ctx.myInput.addr += ctx.currentRankSliceInputOffset;
            ctx.myInput.token = ctx.token[rankIdx];
        } else {
            ctx.remoteInput[rankIdx].addr = ctx.input[rankIdx];
            ctx.remoteInput[rankIdx].addr += ctx.currentRankSliceInputOffset;
            ctx.remoteInput[rankIdx].token = ctx.token[rankIdx];
        }
        ctx.scratchMem[rankIdx].addr = ctx.scratch[ctx.rankId];
        ctx.scratchMem[rankIdx].addr += scratchOffset;
        scratchOffset += ctx.sliceSize;
        ctx.scratchMem[rankIdx].token = ctx.token[ctx.rankId];
    }
}

static void ResetReduceScatterAddr(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    ccu::Variable scratchOffset;
    scratchOffset = 0;
    ctx.myInput.addr = ctx.input[ctx.rankId];
    ctx.myInput.addr += ctx.currentRankSliceInputOffset;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        ctx.scratchMem[rankIdx].addr = ctx.scratch[ctx.rankId];
        ctx.scratchMem[rankIdx].addr += scratchOffset;
        scratchOffset += ctx.sliceSize;
    }
}

static CcuResult DoReduceScatterRead(KfcReduceScatterMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t channelId = 0;
    uint32_t numEventsPerIter = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = unrollIdx * numEventsPerIter + rankIdx / BIT_NUM_PER_CKE;
        uint16_t rankMask = static_cast<uint16_t>(1U << (rankIdx % BIT_NUM_PER_CKE));
        if (rankIdx == ctx.rankId) {
            if (ctx.rankSize <= REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT) {
                CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
            } else {
                CCU_CHK_RET(ccu::LocalCopy(
                    ctx.scratchMem[rankIdx], ctx.myInput, ctx.sliceSize, ctx.events[eventIdx], rankMask));
            }
        } else {
            CCU_CHK_RET(ccu::Read(
                ctx.channels[channelId], ctx.scratchMem[rankIdx], ctx.remoteInput[rankIdx], ctx.sliceSize,
                ctx.events[eventIdx], rankMask));
            channelId++;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult PairwiseLocalReduce(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr output,
    std::vector<ccu::LocalAddr>& inputVec)
{
    ccu::Variable length;
    uint32_t remainPieces = ctx.rankSize;
    while (remainPieces > 1) {
        uint32_t reducePieces = remainPieces / 2;
        uint32_t sourceIndex = remainPieces - reducePieces;
        length = ctx.sliceSize;
        for (uint32_t i = 0; i < reducePieces - 1; i++) {
            length += ctx.sliceSize;
        }
        ccu::LocalReduce(
            inputVec[0], inputVec[sourceIndex], length, ctx.dataType, ctx.reduceOp, ctx.events[0]);
        ccu::EventWait(ctx.events[0], 1);
        remainPieces -= reducePieces;
    }
    ccu::LocalCopy(output, inputVec[0], ctx.sliceSize, ctx.events[0], 1);
    ccu::EventWait(ctx.events[0], 1);
    return CCU_SUCCESS;
}

static CcuResult DoReduceScatterWait(KfcReduceScatterMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t numEventsPerIter = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t i = 0; i < numEventsPerIter; i++) {
        uint32_t eventIdx = unrollIdx * numEventsPerIter + i;
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && i == (numEventsPerIter - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        uint16_t allBit = static_cast<uint16_t>((1U << sigNum) - 1U);
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], allBit));
    }
    return CCU_SUCCESS;
}

static CcuResult CreateReduceLoopV1(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    // Match hccl V1: memSlice is one CCU MS (4096 bytes). The 8-way value
    // belongs to msInterleave, not msPerLoop.
    CCU_CHK_RET(AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, REDUCE_SCATTER_LOOP_COUNT));
    HCCL_INFO(
        "[CCU_RS_KFC_DIAG] LoopResource allocated rank[%u], eventCount[%u], bufCount[%u]", ctx.rankId,
        ctx.moRes.eventCount, ctx.moRes.bufCount);
    if (ctx.IsLoopEntityRegistered("reduceScatterLocalReduce")) {
        return CCU_SUCCESS;
    }
    ctx.CreateLoopEntity("reduceScatterLocalReduce");
    auto& loops = ctx.loopMap["reduceScatterLocalReduce"];
    const uint32_t size = ctx.rankSize;

    for (uint32_t index = 0; index < 2; index++) {
        ctx.loopScratch[index].resize(size);
        const uint32_t bufBase = index * ctx.moConfig.msInterleave;
        ccu::Event loopEvent = ctx.moRes.completedEvent[index];
        loops.body[index].reset(new ccu::Func([&ctx, index, bufBase, loopEvent, size]() {
            for (uint32_t i = 0; i < size; i++) {
                const uint16_t rankMask = static_cast<uint16_t>(1U << i);
                if (i == ctx.rankId) {
                    ccu::LocalCopy(
                        ctx.moRes.ccuBuf[bufBase + i], ctx.loopSrc[index], ctx.loopLen[index], loopEvent, rankMask);
                } else {
                    ccu::LocalCopy(
                        ctx.moRes.ccuBuf[bufBase + i], ctx.loopScratch[index][i], ctx.loopLen[index], loopEvent,
                        rankMask);
                }
            }
            const uint16_t allRankMask = static_cast<uint16_t>((1U << size) - 1U);
            ccu::EventWait(loopEvent, allRankMask);
            if (size > 1) {
                ccu::LocalReduce(
                    &ctx.moRes.ccuBuf[bufBase], size, ctx.dataType, ctx.outputDataType, ctx.reduceOp,
                    ctx.loopLen[index], loopEvent, 1);
                ccu::EventWait(loopEvent, 1);
            }
            ccu::LocalCopy(ctx.loopDst[index], ctx.moRes.ccuBuf[bufBase], ctx.loopLenExp[index], loopEvent, 1);
            ccu::EventWait(loopEvent, 1);
        }));
        loops.loops[index].reset(new ccu::Loop(loops.loopParam[index], *loops.body[index]));
    }
    return CCU_SUCCESS;
}

static CcuResult ReduceLoopGroupV1(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr output, ccu::LocalAddr source,
    std::vector<ccu::LocalAddr>& scratchOrg)
{
    CCU_CHK_RET(CreateReduceLoopV1(ctx));
    auto& loops = ctx.loopMap["reduceScatterLocalReduce"];
    const uint32_t size = static_cast<uint32_t>(scratchOrg.size());
    const uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);

    ccu::LocalAddr dst;
    dst.addr = output.addr;
    dst.token = output.token;
    if (expansionNum != 1) {
        ccu::Variable expansionParam;
        expansionParam = GetExpansionParam(expansionNum);
        dst.token = dst.token + expansionParam;
    }

    ccu::Variable loopParam;
    ccu::Variable parallelParam;
    ccu::Variable offsetParam;
    ccu::Variable sliceSize;
    ccu::Variable expandedSliceSize;
    ccu::Variable expansionSlice;

    expansionSlice = ctx.moConfig.memSlice;

    // KFC small-message mode: register exactly one LoopGroup. The CCU_IF
    // branches used by the full hccl implementation are intentionally not
    // constructed here because KFC registration executes both branch bodies;
    // reusing the same Loop in both groups violates the hcomm LoopGroup
    // lifetime/ownership contract.
    source.addr += ctx.goSize.addrOffset;
    for (uint32_t i = 0; i < expansionNum; i++) {
        dst.addr += ctx.goSize.addrOffset;
    }
    expandedSliceSize = 0;
    for (uint32_t i = 0; i < expansionNum; i++) {
        expandedSliceSize += ctx.goSize.residual;
    }
    for (uint32_t i = 0; i < size; i++) {
        ctx.loopScratch[0][i].addr = scratchOrg[i].addr;
        ctx.loopScratch[0][i].token = scratchOrg[i].token;
        ctx.loopScratch[0][i].addr += ctx.goSize.addrOffset;
    }
    ctx.loopSrc[0].addr = source.addr;
    ctx.loopSrc[0].token = source.token;
    ctx.loopDst[0].addr = dst.addr;
    ctx.loopDst[0].token = dst.token;
    ctx.loopLen[0] = ctx.goSize.residual;
    ctx.loopLenExp[0] = expandedSliceSize;

    loopParam = GetLoopParam(0, 0, 1);
    offsetParam = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);
    loops.loopParam[0] = loopParam;
    std::vector<ccu::Loop> loopList{*loops.loops[0]};
    ccu::LoopGroup group(ctx.goSize.parallelParam, offsetParam, 1, loopList);
    }
    return CCU_SUCCESS;
}

static CcuResult DoReduceScatter(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    ccu::LocalAddr myOutput;
    myOutput.addr = ctx.output;
    myOutput.addr += ctx.currentRankSliceOutputOffset;
    myOutput.token = ctx.token[ctx.rankId];

    if (ctx.rankSize <= REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT) {
        HCCL_INFO("[CCU_RS_KFC_DIAG] Reduce path LoopGroupV1 rank[%u], rankSize[%u]", ctx.rankId, ctx.rankSize);
        CCU_CHK_RET(ReduceLoopGroupV1(ctx, myOutput, ctx.myInput, ctx.scratchMem));
    } else {
        HCCL_INFO("[CCU_RS_KFC_DIAG] Reduce path Pairwise rank[%u], rankSize[%u]", ctx.rankId, ctx.rankSize);
        CCU_CHK_RET(PairwiseLocalReduce(ctx, myOutput, ctx.scratchMem));
    }
    return CCU_SUCCESS;
}

static CcuResult DoRepeatReduceScatter(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    HCCL_INFO("[CCU_RS_KFC_DIAG] one-round pipeline begin rank[%u]", ctx.rankId);
    InitReduceScatterAddr(ctx);
    ctx.waitRepeatNum = ctx.repeatNum;
    ctx.readRepeatNum = ctx.repeatNum;

    CCU_IF(ctx.repeatNum != UINT64_MAX)
    {
        ctx.repeatNum += ctx.constVar1;
        CCU_CHK_RET(DoReduceScatterRead(ctx, 0));
    }

    // KFC currently supplies one round from the sole executor. Keep the
    // address protocol explicit so a future multi-round path can add the
    // repeat stride without changing the first-round data flow.
    CCU_IF(ctx.waitRepeatNum != UINT64_MAX)
    {
        ctx.waitRepeatNum += ctx.constVar1;
        CCU_CHK_RET(DoReduceScatterWait(ctx, 0));
    }

    ResetReduceScatterAddr(ctx);
    CCU_CHK_RET(DoReduceScatter(ctx));
    HCCL_INFO("[CCU_RS_KFC_DIAG] one-round pipeline registered rank[%u]", ctx.rankId);
    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable normalSliceSize, ccu::Variable lastSliceSize,
    ccu::Variable scratchRepeatStride, ccu::Variable repeatNum, ccu::Variable goSizeAddrOffset,
    ccu::Variable goSizeLoopParam, ccu::Variable goSizeParallelParam, ccu::Variable goSizeResidual,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
{
    KfcReduceScatterMesh1DMem2MemContext ctx;
    InitCcuKernelCtxBase(ctx);
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputType;
    if (ctx.outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        ctx.outputDataType = dataType;
    }
    ctx.reduceOp = reduceType;
    ctx.flag = 0;
    ctx.constVar1 = 1;

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(
        LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset, normalSliceSize,
            lastSliceSize, scratchRepeatStride, repeatNum, goSizeAddrOffset, goSizeLoopParam, goSizeParallelParam,
            goSizeResidual));

    if (rankId == (rankSize - 1)) {
        ctx.sliceSize = ctx.lastSliceSize;
    } else {
        ctx.sliceSize = ctx.normalSliceSize;
    }
    HCCL_INFO(
        "[CCU_RS_KFC_DIAG] Kernel start rank[%u/%u], channels[%u], dataType[%u], outputType[%u], reduceOp[%u], "
        "effectiveOutputType[%u], expansionNum[%u]",
        rankId, rankSize, channelCount, static_cast<uint32_t>(dataType), static_cast<uint32_t>(outputType),
        static_cast<uint32_t>(reduceType), static_cast<uint32_t>(ctx.outputDataType),
        GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType));
    CCU_CHK_RET(PreSync(ctx));
    // Match hccl: zero-sized slices only participate in the surrounding
    // synchronization and must not register zero-length CCU operations.
    CCU_IF(ctx.sliceSize != 0)
    {
        CCU_CHK_RET(DoRepeatReduceScatter(ctx));
    }
    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CCU_RS_KFC_DIAG] Kernel end rank[%u]", rankId);
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
