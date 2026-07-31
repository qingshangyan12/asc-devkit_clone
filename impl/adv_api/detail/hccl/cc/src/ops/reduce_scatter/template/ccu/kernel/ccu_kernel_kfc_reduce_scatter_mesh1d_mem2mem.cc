/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {
using namespace hcomm;

constexpr int INPUT_XN_ID = 0;
constexpr int SCRATCH_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;

constexpr uint16_t BIT_NUM_PER_CKE = 16;
constexpr uint16_t REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT = 8;
constexpr uint32_t RS_UNROLL_NUM = 16;
constexpr uint64_t REDUCE_MS_CNT = 8;
constexpr uint16_t REDUCE_SCATTER_LOOP_COUNT = 16;

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
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable scratchRepeatStride;
    ccu::Variable repeatNum;
    GroupOpSizeVars goSize;
    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMem;

    std::vector<ccu::Event> events;
    ccu::Variable constVar1;
    ccu::Variable sliceSize;
    ccu::Variable readRepeatNum;
    ccu::Variable waitRepeatNum;
    ccu::Variable flag;

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

    ctx.moConfig.loopCount = REDUCE_SCATTER_LOOP_COUNT;
    ctx.moConfig.msInterleave = REDUCE_MS_CNT;
    ctx.moConfig.memSlice = CCU_MS_SIZE;

    ctx.resourceAllocated = false;

    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    ctx.events.resize(RS_UNROLL_NUM * eventNum);

    ctx.constVar1 = 1;

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
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.input[ctx.rankId], INPUT_XN_ID, CKE_IDX_0, 1 << INPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.scratch[ctx.rankId], SCRATCH_XN_ID, CKE_IDX_0, 1 << SCRATCH_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID));
    }
    uint32_t allBit = 1 << INPUT_XN_ID | 1 << SCRATCH_XN_ID | 1 << TOKEN_XN_ID;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
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

static void DoReduceScatterRead(KfcReduceScatterMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t channelId = 0;
    uint32_t numEventsPerIter = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;

    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = unrollIdx * numEventsPerIter + rankIdx / BIT_NUM_PER_CKE;
        uint32_t rankMask = 1 << (rankIdx % BIT_NUM_PER_CKE);

        if (rankIdx == ctx.rankId) {
            if (ctx.rankSize <= REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT) {
                ccu::EventRecord(ctx.events[eventIdx], rankMask);
            } else {
                ccu::LocalCopy(ctx.scratchMem[rankIdx], ctx.myInput, ctx.sliceSize, ctx.events[eventIdx], rankMask);
            }
        } else {
            ccu::Read(ctx.channels[channelId], ctx.scratchMem[rankIdx], ctx.remoteInput[rankIdx], ctx.sliceSize,
                      ctx.events[eventIdx], rankMask);
            channelId++;
        }
    }
}

static void DoReduceScatterWait(KfcReduceScatterMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t numEventsPerIter = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t i = 0; i < numEventsPerIter; i++) {
        uint32_t eventIdx = unrollIdx * numEventsPerIter + i;
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && i == (numEventsPerIter - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        uint32_t allBit = (1 << sigNum) - 1;
        ccu::EventWait(ctx.events[eventIdx], allBit);
    }
}

static CcuResult PairwiseLocalReduce(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr myOutput, std::vector<ccu::LocalAddr>& inputVec,
    ccu::Variable sliceSize)
{
    ccu::Variable len;

    uint32_t remainPieces = ctx.rankSize;
    while (remainPieces > 1) {
        uint32_t reducePieces = remainPieces / 2;
        uint32_t srcIdx = remainPieces - reducePieces;
        uint32_t dstIdx = 0;

        len = sliceSize;
        for (uint32_t i = 0; i < reducePieces - 1; i++) {
            len += sliceSize;
        }

        ccu::LocalReduce(inputVec[dstIdx], inputVec[srcIdx], len, ctx.dataType, ctx.reduceOp, ctx.events[0]);
        ccu::EventWait(ctx.events[0]);

        remainPieces -= reducePieces;
    }

    ccu::LocalCopy(myOutput, inputVec[0], sliceSize, ctx.events[0]);
    ccu::EventWait(ctx.events[0]);

    return CCU_SUCCESS;
}

static CcuResult CreateReduceLoop(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    constexpr uint32_t LOOP_NUM_16 = 16;
    AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, LOOP_NUM_16);

    if (ctx.IsLoopEntityRegistered("reduceScatterLocalReduce")) {
        return CCU_SUCCESS;
    }
    ctx.CreateLoopEntity("reduceScatterLocalReduce");
    auto& loops = ctx.loopMap["reduceScatterLocalReduce"];

    uint32_t size = ctx.rankSize;
    uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    constexpr uint32_t LOOP_NUM_2 = 2;
    for (int32_t index = 0; index < static_cast<int32_t>(LOOP_NUM_2); index++) {
        ctx.loopScratch[index].resize(size);

        uint32_t bufBase = index * ctx.moConfig.msInterleave;
        ccu::Event loopEvt = ctx.moRes.completedEvent[index];

        loops.body[index].reset(new ccu::Func(
            [&ctx, index, bufBase, loopEvt, size, expansionNum]() {
                for (uint32_t i = 0; i < size; i++) {
                    if (i == ctx.rankId) {
                        ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase + i], ctx.loopSrc[index], ctx.loopLen[index], loopEvt,
                                       1 << i);
                    } else {
                        ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase + i], ctx.loopScratch[index][i], ctx.loopLen[index],
                                       loopEvt, 1 << i);
                    }
                }
                ccu::EventWait(loopEvt, (1 << size) - 1);

                if (size > 1) {
                    ccu::LocalReduce(&ctx.moRes.ccuBuf[bufBase], size, ctx.dataType, ctx.outputDataType, ctx.reduceOp,
                                     ctx.loopLen[index], loopEvt, 1);
                    ccu::EventWait(loopEvt, 1);
                }

                ccu::LocalCopy(ctx.loopDst[index], ctx.moRes.ccuBuf[bufBase], ctx.loopLenExp[index], loopEvt, 1);
                ccu::EventWait(loopEvt, 1);
            }));

        loops.loops[index].reset(new ccu::Loop(loops.loopParam[index], *loops.body[index]));
    }

    return CCU_SUCCESS;
}

static CcuResult ReduceLoopGroup(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::LocalAddr outDstOrg, ccu::LocalAddr srcOrg,
    std::vector<ccu::LocalAddr>& scratchOrg)
{
    const uint32_t size = scratchOrg.size();

    ccu::LocalAddr dst;
    dst.addr = outDstOrg.addr;
    dst.token = outDstOrg.token;

    ccu::LocalAddr src;
    src.addr = srcOrg.addr;
    src.token = srcOrg.token;

    std::vector<ccu::LocalAddr> scratch;
    for (uint32_t idx = 0; idx < size; idx++) {
        ccu::LocalAddr scratchAddr;
        scratchAddr.addr = scratchOrg[idx].addr;
        scratchAddr.token = scratchOrg[idx].token;
        scratch.push_back(scratchAddr);
    }

    CCU_CHK_RET(CreateReduceLoop(ctx));
    auto& loops = ctx.loopMap["reduceScatterLocalReduce"];

    uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    ccu::Variable sliceSizeExpansion;
    ccu::Variable loopCfg0;
    ccu::Variable loopCfg1;
    ccu::Variable loopParam;
    ccu::Variable paraCfg;
    ccu::Variable offsetCfg;

    if (expansionNum != 1) {
        ccu::Variable tmp = GetExpansionParam(expansionNum);
        dst.token = dst.token + tmp;
    }

    CCU_IF(ctx.goSize.loopParam != 0)
    {
        ccu::Variable sliceSize;
        loopParam = GetLoopParam(0, ctx.moConfig.memSlice * ctx.moConfig.loopCount, 0);
        loopParam = loopParam + ctx.goSize.loopParam;

        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        for (uint32_t i = 0; i < size; i++) {
            ctx.loopScratch[0][i].addr = scratch[i].addr;
            ctx.loopScratch[0][i].token = scratch[i].token;
        }
        ctx.loopSrc[0].addr = src.addr;
        ctx.loopSrc[0].token = src.token;
        ctx.loopDst[0].addr = dst.addr;
        ctx.loopDst[0].token = dst.token;
        ctx.loopLen[0] = sliceSize;
        ctx.loopLenExp[0] = sliceSizeExpansion;
        paraCfg = GetParallelParam(ctx.moConfig.loopCount - 1, 0, 1);
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopParam;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0]};
        ccu::LoopGroup group(paraCfg, offsetCfg, ctx.moConfig.loopCount, grpLoops);
    }

    CCU_IF(ctx.goSize.parallelParam != 0)
    {
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += ctx.goSize.addrOffset;
        }
        src.addr += ctx.goSize.addrOffset;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += ctx.goSize.addrOffset;
        }

        sliceSizeExpansion = 0;
        for (uint32_t i = 0; i < expansionNum; i++) {
            sliceSizeExpansion = sliceSizeExpansion + ctx.goSize.residual;
        }

        for (uint32_t i = 0; i < size; i++) {
            ctx.loopScratch[0][i].addr = scratch[i].addr;
            ctx.loopScratch[0][i].token = scratch[i].token;
        }
        ctx.loopSrc[0].addr = src.addr;
        ctx.loopSrc[0].token = src.token;
        ctx.loopDst[0].addr = dst.addr;
        ctx.loopDst[0].token = dst.token;
        ctx.loopLen[0] = ctx.goSize.residual;
        ctx.loopLenExp[0] = sliceSizeExpansion;

        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += ctx.goSize.residual;
        }
        src.addr += ctx.goSize.residual;
        ccu::Variable sliceSize;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += ctx.goSize.residual;
        }

        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        for (uint32_t i = 0; i < size; i++) {
            ctx.loopScratch[1][i].addr = scratch[i].addr;
            ctx.loopScratch[1][i].token = scratch[i].token;
        }
        ctx.loopSrc[1].addr = src.addr;
        ctx.loopSrc[1].token = src.token;
        ctx.loopDst[1].addr = dst.addr;
        ctx.loopDst[1].token = dst.token;
        ctx.loopLen[1] = sliceSize;
        ctx.loopLenExp[1] = sliceSizeExpansion;
        loopCfg0 = GetLoopParam(0, 0, 1);
        loopCfg1 = GetLoopParam(0, 0, 1);
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopCfg0;
        loops.loopParam[1] = loopCfg1;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0], *loops.loops[1]};
        ccu::LoopGroup group(ctx.goSize.parallelParam, offsetCfg, ctx.moConfig.loopCount, grpLoops);
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
        CCU_CHK_RET(ReduceLoopGroup(ctx, myOutput, ctx.myInput, ctx.scratchMem));
    } else {
        CCU_CHK_RET(PairwiseLocalReduce(ctx, myOutput, ctx.scratchMem, ctx.sliceSize));
    }
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

static CcuResult DoReduceScatterReduce(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    CCU_IF(ctx.readRepeatNum != UINT64_MAX)
    {
        ctx.flag = 0;
        CCU_WHILE(ctx.readRepeatNum != UINT64_MAX)
        {
            ctx.readRepeatNum += ctx.constVar1;
            CCU_IF(ctx.flag != 0)
            {
                for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
                    ctx.scratchMem[rankIdx].addr += ctx.scratchRepeatStride;
                }
                ctx.myInput.addr += ctx.inputRepeatStride;
                ctx.output += ctx.outputRepeatStride;
            }
            CCU_CHK_RET(DoReduceScatter(ctx));
            ctx.flag = 1;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoRepeatReduceScatter(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    InitReduceScatterAddr(ctx);

    ctx.waitRepeatNum = ctx.repeatNum;
    ctx.readRepeatNum = ctx.repeatNum;

    CCU_IF(ctx.repeatNum != UINT64_MAX)
    {
        ctx.repeatNum += ctx.constVar1;
        DoReduceScatterRead(ctx, 0);
    }

    for (uint32_t i = 1; i < RS_UNROLL_NUM; i++) {
        CCU_IF(ctx.repeatNum != UINT64_MAX)
        {
            ctx.repeatNum += ctx.constVar1;
            for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
                if (rankIdx == ctx.rankId) {
                    ctx.myInput.addr += ctx.inputRepeatStride;
                } else {
                    ctx.remoteInput[rankIdx].addr += ctx.inputRepeatStride;
                }
                ctx.scratchMem[rankIdx].addr += ctx.scratchRepeatStride;
            }
            DoReduceScatterRead(ctx, i);
        }
    }

    for (uint32_t i = 0; i < RS_UNROLL_NUM; i++) {
        CCU_IF(ctx.waitRepeatNum != UINT64_MAX)
        {
            ctx.waitRepeatNum += ctx.constVar1;
            DoReduceScatterWait(ctx, i);
        }
    }

    ResetReduceScatterAddr(ctx);

    CCU_CHK_RET(DoReduceScatterReduce(ctx));

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
    HCCL_INFO(
        "[CcuKernelReduceScatterMesh1DMem2Mem] Init, KernelArgs are rankId[%u], rankSize_[%u], dataType[%d], "
        "outputDataType[%d], reduceOp[%d]",
        ctx.rankId, ctx.rankSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp);

    InitCcuKernelCtxBase(ctx);

    HCCL_INFO("[CcuKernelReduceScatterMesh1DMem2Mem] ReduceScatterMesh1DMem2Mem run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset, normalSliceSize,
                         lastSliceSize, scratchRepeatStride, repeatNum, goSizeAddrOffset, goSizeLoopParam,
                         goSizeParallelParam, goSizeResidual));
    CCU_CHK_RET(PreSync(ctx));

    ctx.sliceSize = (ctx.rankId == (ctx.rankSize - 1)) ? ctx.lastSliceSize : ctx.normalSliceSize;
    CCU_IF(ctx.sliceSize != 0)
    {
        CCU_CHK_RET(DoRepeatReduceScatter(ctx));
    }

    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CcuKernelReduceScatterMesh1DMem2Mem] ReduceScatterMesh1DMem2Mem end");

    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
