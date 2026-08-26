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
#include "mc2_type.h"

namespace mc2_ops_hccl {
using namespace hcomm;

constexpr int INPUT_XN_ID = 0;
constexpr int SCRATCH_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;
constexpr uint16_t BIT_NUM_PER_CKE = 16;
constexpr uint16_t REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT = 8;
constexpr uint16_t REDUCE_SCATTER_LOOP_COUNT = 16;
constexpr uint64_t REDUCE_MS_CNT = 8;

struct KfcReduceScatterMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    ccu::Variable output;
    std::vector<ccu::Variable> scratch;
    std::vector<ccu::Variable> token;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable chunkSize;
    ccu::Variable chunkLoopNum;
    ccu::Variable tailSize;
    ccu::Variable currentSliceSize;
    GroupOpSizeVars fullGoSize;
    GroupOpSizeVars tailGoSize;
    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMem;
    std::vector<ccu::Event> events;
    ccu::Variable constVar1;
    std::array<std::vector<ccu::LocalAddr>, 2> loopScratch;
    ccu::LocalAddr loopSrc[2];
    ccu::LocalAddr loopDst[2];
    ccu::Variable loopLen[2];
    ccu::Variable loopLenExp[2];
};

static CcuResult InitResource(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    uint32_t channelIdx = 0;

    if (ctx.channelCount == 0) {
        HCCL_ERROR("[KfcReduceScatterMesh1DMem2Mem] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.scratch.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    ctx.remoteInput.resize(ctx.rankSize);
    ctx.scratchMem.resize(ctx.rankSize);

    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], INPUT_XN_ID);
            ctx.scratch[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], SCRATCH_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }
    ctx.moConfig.msInterleave = REDUCE_MS_CNT;
    ctx.moConfig.loopCount = REDUCE_SCATTER_LOOP_COUNT;
    ctx.moConfig.memSlice = CCU_MS_SIZE;
    ctx.resourceAllocated = false;
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    // Process one chunk at a time; one event group is sufficient and avoids unroll resource pressure.
    ctx.events.resize(eventNum);
    ctx.constVar1 = 1;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable scratch, ccu::Variable inputOffset, ccu::Variable chunkSize,
    ccu::Variable chunkLoopNum, ccu::Variable tailSize, ccu::Variable fullGoAddr, ccu::Variable fullGoLoop,
    ccu::Variable fullGoParallel, ccu::Variable fullGoResidual, ccu::Variable tailGoAddr,
    ccu::Variable tailGoLoop, ccu::Variable tailGoParallel, ccu::Variable tailGoResidual)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.scratch[ctx.rankId] = scratch;
    ctx.currentRankSliceInputOffset = inputOffset;
    ctx.currentRankSliceOutputOffset = 0;
    ctx.chunkSize = chunkSize;
    ctx.chunkLoopNum = chunkLoopNum;
    ctx.tailSize = tailSize;
    ctx.fullGoSize.addrOffset = fullGoAddr;
    ctx.fullGoSize.loopParam = fullGoLoop;
    ctx.fullGoSize.parallelParam = fullGoParallel;
    ctx.fullGoSize.residual = fullGoResidual;
    ctx.tailGoSize.addrOffset = tailGoAddr;
    ctx.tailGoSize.loopParam = tailGoLoop;
    ctx.tailGoSize.parallelParam = tailGoParallel;
    ctx.tailGoSize.residual = tailGoResidual;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(ctx.channels[i], ctx.input[ctx.rankId], INPUT_XN_ID, CKE_IDX_0, 1));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(ctx.channels[i], ctx.scratch[ctx.rankId], SCRATCH_XN_ID, CKE_IDX_0, 2));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(ctx.channels[i], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 4));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, 7);
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID);
    }
    return CCU_SUCCESS;
}

static void InitAddr(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    ccu::Variable scratchOffset;
    scratchOffset = 0;

    // Divide the fixed 16 MiB scratch into rankSize regions and reuse them for every chunk.
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
        scratchOffset += ctx.chunkSize;
        ctx.scratchMem[rankIdx].token = ctx.token[ctx.rankId];

    }
}

static void ReadRound(KfcReduceScatterMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t channelId = 0;
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = unrollIdx * eventNum + rankIdx / BIT_NUM_PER_CKE;
        uint32_t rankMask = 1 << (rankIdx % BIT_NUM_PER_CKE);
        if (rankIdx == ctx.rankId) {
            if (ctx.rankSize <= REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT) {
                ccu::EventRecord(ctx.events[eventIdx], rankMask);
            } else {
                ccu::LocalCopy(
                    ctx.scratchMem[rankIdx], ctx.myInput, ctx.currentSliceSize, ctx.events[eventIdx], rankMask);
            }
        } else {
            ccu::Read(ctx.channels[channelId], ctx.scratchMem[rankIdx], ctx.remoteInput[rankIdx], ctx.currentSliceSize,
                      ctx.events[eventIdx], rankMask);
            channelId++;
        }
    }
}

static void WaitRound(KfcReduceScatterMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t eventId = 0; eventId < eventNum; eventId++) {
        uint32_t rankCount = BIT_NUM_PER_CKE;
        if (eventId == eventNum - 1 && ctx.rankSize % BIT_NUM_PER_CKE != 0) {
            rankCount = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        ccu::EventWait(ctx.events[unrollIdx * eventNum + eventId], (1 << rankCount) - 1);
    }
}

static CcuResult CreateReduceLoopV1(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    constexpr uint32_t loopNum = 16;
    CCU_CHK_RET(AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, loopNum));
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
                if (i == ctx.rankId) {
                    ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase + i], ctx.loopSrc[index], ctx.loopLen[index], loopEvent,
                                   1 << i);
                } else {
                    ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase + i], ctx.loopScratch[index][i], ctx.loopLen[index],
                                   loopEvent, 1 << i);
                }
            }
            ccu::EventWait(loopEvent, (1 << size) - 1);
            if (size > 1) {
                ccu::LocalReduce(&ctx.moRes.ccuBuf[bufBase], size, ctx.dataType, ctx.outputDataType, ctx.reduceOp,
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

static CcuResult ReduceLoopGroupV1(KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr output,
                                   ccu::LocalAddr source, std::vector<ccu::LocalAddr>& scratch,
                                   const GroupOpSizeVars& goSize)
{
    const uint32_t size = scratch.size();
    if (size == 0) {
        return CCU_SUCCESS;
    }
    CCU_CHK_RET(CreateReduceLoopV1(ctx));
    auto& loops = ctx.loopMap["reduceScatterLocalReduce"];
    const uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    ccu::LocalAddr dst = output;
    if (expansionNum != 1) {
        ccu::Variable tmp;
        tmp = GetExpansionParam(expansionNum);
        dst.token = dst.token + tmp;
    }
    CCU_IF(goSize.loopParam != 0)
    {
        ccu::Variable loopParam;
        loopParam = GetLoopParam(0, ctx.moConfig.memSlice * ctx.moConfig.loopCount, 0);
        loopParam += goSize.loopParam;
        for (uint32_t i = 0; i < size; i++) {
            ctx.loopScratch[0][i] = scratch[i];
        }
        ctx.loopSrc[0] = source;
        ctx.loopDst[0] = dst;
        ctx.loopLen[0] = ctx.moConfig.memSlice;
        ctx.loopLenExp[0] = ctx.moConfig.memSlice * expansionNum;
        loops.loopParam[0] = loopParam;
        ccu::Variable paraCfg;
        paraCfg = GetParallelParam(ctx.moConfig.loopCount - 1, 0, 1);
        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);
        std::vector<ccu::Loop> groupLoops{*loops.loops[0]};
        ccu::LoopGroup group(paraCfg, offsetCfg, ctx.moConfig.loopCount, groupLoops);
    }
    CCU_IF(goSize.parallelParam != 0)
    {
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += goSize.addrOffset;
        }
        source.addr += goSize.addrOffset;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += goSize.addrOffset;
        }
        ccu::Variable expandedResidual;
        expandedResidual = 0;
        for (uint32_t i = 0; i < expansionNum; i++) {
            expandedResidual += goSize.residual;
        }
        for (uint32_t i = 0; i < size; i++) {
            ctx.loopScratch[0][i] = scratch[i];
        }
        ctx.loopSrc[0] = source;
        ctx.loopDst[0] = dst;
        ctx.loopLen[0] = goSize.residual;
        ctx.loopLenExp[0] = expandedResidual;
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += goSize.residual;
        }
        source.addr += goSize.residual;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += goSize.residual;
        }
        for (uint32_t i = 0; i < size; i++) {
            ctx.loopScratch[1][i] = scratch[i];
        }
        ctx.loopSrc[1] = source;
        ctx.loopDst[1] = dst;
        ctx.loopLen[1] = ctx.moConfig.memSlice;
        ctx.loopLenExp[1] = ctx.moConfig.memSlice * expansionNum;
        loops.loopParam[0] = 1;
        loops.loopParam[1] = 1;
        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);
        std::vector<ccu::Loop> groupLoops{*loops.loops[0], *loops.loops[1]};
        ccu::LoopGroup group(goSize.parallelParam, offsetCfg, ctx.moConfig.loopCount, groupLoops);
    }
    return CCU_SUCCESS;
}

static CcuResult PairwiseLocalReduceFull(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr myOutput, std::vector<ccu::LocalAddr>& inputVec,
    ccu::Variable sliceSize)
{
    ccu::Variable len;

    // 每轮将数据划分为2组做规约，总规约次数log2(n)
    uint32_t remainPieces = ctx.rankSize;
    while (remainPieces > 1) {
        // 每轮将最后remain/2块，reduce到最前remian/2块
        uint32_t reducePieces = remainPieces / 2;
        uint32_t srcIdx = remainPieces - reducePieces;
        uint32_t dstIdx = 0;

        len = sliceSize;
        for (uint32_t i = 0; i < reducePieces - 1; i++) {
            len = len + sliceSize;
        }

        ccu::LocalReduce(inputVec[dstIdx], inputVec[srcIdx], len, ctx.dataType, ctx.reduceOp, ctx.events[0]);
        ccu::EventWait(ctx.events[0]);

        remainPieces -= reducePieces;
    }

    ccu::LocalCopy(myOutput, inputVec[0], sliceSize, ctx.events[0]);
    ccu::EventWait(ctx.events[0]);

    return CCU_SUCCESS;
}

static CcuResult PairwiseLocalReduceTail(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr myOutput, std::vector<ccu::LocalAddr>& inputVec,
    ccu::Variable sliceSize)
{
    uint32_t remainPieces = ctx.rankSize;
    while (remainPieces > 1) {
        uint32_t reducePieces = remainPieces / 2;
        uint32_t srcIdx = remainPieces - reducePieces;
        for (uint32_t i = 0; i < reducePieces; i++) {
            ccu::LocalReduce(
                inputVec[i], inputVec[srcIdx + i], sliceSize, ctx.dataType, ctx.reduceOp, ctx.events[0]);
            ccu::EventWait(ctx.events[0]);
        }
        remainPieces -= reducePieces;
    }
    ccu::LocalCopy(myOutput, inputVec[0], sliceSize, ctx.events[0]);
    ccu::EventWait(ctx.events[0]);
    return CCU_SUCCESS;
}

static CcuResult ReduceRound(
    KfcReduceScatterMesh1DMem2MemContext& ctx, const GroupOpSizeVars& goSize, bool isTail)
{
    ccu::LocalAddr output;
    output.addr = ctx.output;
    output.addr += ctx.currentRankSliceOutputOffset;
    output.token = ctx.token[ctx.rankId];
    if (ctx.rankSize <= REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT) {
        std::vector<ccu::LocalAddr> scratch = ctx.scratchMem;
        return ReduceLoopGroupV1(ctx, output, ctx.myInput, scratch, goSize);
    }
    if (isTail) {
        return PairwiseLocalReduceTail(ctx, output, ctx.scratchMem, ctx.currentSliceSize);
    }
    return PairwiseLocalReduceFull(ctx, output, ctx.scratchMem, ctx.currentSliceSize);
}

static CcuResult DoChunkedReduceScatter(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    InitAddr(ctx);
    CCU_WHILE(ctx.chunkLoopNum != UINT64_MAX)
    {
        ctx.chunkLoopNum += ctx.constVar1;
        CCU_IF(ctx.chunkLoopNum == UINT64_MAX)
        {
            ctx.currentSliceSize = ctx.tailSize;
            ReadRound(ctx, 0);
            WaitRound(ctx, 0);
            CCU_CHK_RET(ReduceRound(ctx, ctx.tailGoSize, true));
        }
        CCU_IF(ctx.chunkLoopNum != UINT64_MAX)
        {
            ctx.currentSliceSize = ctx.chunkSize;
            ReadRound(ctx, 0);
            WaitRound(ctx, 0);
            CCU_CHK_RET(ReduceRound(ctx, ctx.fullGoSize, false));
        }
        ctx.myInput.addr += ctx.currentSliceSize;
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx != ctx.rankId) {
                ctx.remoteInput[rankIdx].addr += ctx.currentSliceSize;
            }
        }
        ctx.output += ctx.currentSliceSize;
    }
    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable chunkSize, ccu::Variable chunkLoopNum,
    ccu::Variable tailSize,
    ccu::Variable fullGoAddrOffset, ccu::Variable fullGoLoopParam,
    ccu::Variable fullGoParallelParam, ccu::Variable fullGoResidual, ccu::Variable tailGoAddrOffset,
    ccu::Variable tailGoLoopParam, ccu::Variable tailGoParallelParam, ccu::Variable tailGoResidual,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
{
    KfcReduceScatterMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputType == HcclDataType::HCCL_DATA_TYPE_RESERVED ? dataType : outputType;
    ctx.reduceOp = reduceType;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset, chunkSize,
                         chunkLoopNum, tailSize, fullGoAddrOffset, fullGoLoopParam, fullGoParallelParam,
                         fullGoResidual, tailGoAddrOffset, tailGoLoopParam, tailGoParallelParam, tailGoResidual));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoChunkedReduceScatter(ctx));
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
