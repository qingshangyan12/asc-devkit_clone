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
constexpr uint16_t GROUP_REDUCE_MAX_PIECE_CNT = 8;

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
    ccu::Variable sliceSize;
    GroupOpSizeVars goSize;
    ccu::Variable chunkSize;
    ccu::Variable currentSliceSize;
    ccu::Variable tailSize;
    ccu::Variable chunkLoopNum;
    GroupOpSizeVars fullGoSize;
    GroupOpSizeVars tailGoSize;

    std::vector<ccu::Event> events;

    ccu::LocalAddr srcMem;
    ccu::LocalAddr localDstMem;
    ccu::RemoteAddr remoteDstMem;
    std::vector<ccu::RemoteAddr> reduceScatterSrc;
    std::vector<ccu::LocalAddr> reduceScatterDst;

    GroupReduceVar reduceVar;
    GroupBroadcastVar broadcastVar;
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

static CcuResult PairwiseLocalReduce(
    KfcAllReduceMesh1DMem2MemContext& ctx, ccu::LocalAddr myOutput, std::vector<ccu::LocalAddr>& inputVec,
    ccu::Variable sliceSize, HcclDataType dataType, HcclDataType outputDataType, HcclReduceOp opType)
{
    (void)outputDataType;
    ccu::Variable len;

    uint32_t remainPieces = ctx.rankSize;
    while (remainPieces > 1) {
        uint32_t reducePieces = remainPieces / 2;
        uint32_t srcIdx = remainPieces - reducePieces;

        len = sliceSize;
        for (uint32_t i = 0; i < reducePieces - 1; i++) {
            len += sliceSize;
        }

        ccu::LocalReduce(inputVec[0], inputVec[srcIdx], len, dataType, opType, ctx.events[0], 1);
        ccu::EventWait(ctx.events[0], 1);

        remainPieces -= reducePieces;
    }

    ccu::LocalCopy(myOutput, inputVec[0], sliceSize, ctx.events[0], 1);
    ccu::EventWait(ctx.events[0], 1);
    return CCU_SUCCESS;
}

static CcuResult CreateReduceLoopM2M(
    KfcAllReduceMesh1DMem2MemContext& ctx, GroupReduceVar& var, uint32_t channelSize, HcclDataType dataType,
    HcclDataType outputDataType, HcclReduceOp opType)
{
    AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, CCU_M2M_LOCAL_COPY_LOOP_COUNT);
    if (ctx.IsLoopEntityRegistered("reduce_m2m")) {
        return CCU_SUCCESS;
    }
    ctx.CreateLoopEntity("reduce_m2m");
    auto& loops = ctx.loopMap["reduce_m2m"];

    uint32_t size = channelSize + 1;
    uint32_t expansionNum = GetReduceExpansionNum(opType, dataType, outputDataType);
    uint32_t loopNum = 2;

    for (int32_t index = 0; index < loopNum; index++) {
        var.loopRemoteSrc[index].resize(channelSize);

        uint32_t bufBase = index * ctx.moConfig.msInterleave;
        ccu::Event loopEvt = ctx.moRes.completedEvent[index];

        loops.body[index].reset(new ccu::Func([&ctx, index, bufBase, loopEvt, channelSize, size, &var, dataType,
                                               outputDataType, opType]() {
            for (uint32_t i = 0; i < channelSize; i++) {
                ccu::Read(
                    ctx.channels[i], ctx.moRes.ccuBuf[bufBase + i], var.loopRemoteSrc[index][i], var.loopLen[index],
                    loopEvt, 1 << i);
            }

            ccu::LocalCopy(
                ctx.moRes.ccuBuf[bufBase + channelSize], var.loopLocalSrc[index], var.loopLen[index], loopEvt,
                1 << channelSize);
            ccu::EventWait(loopEvt, (1 << size) - 1);

            if (size > 1) {
                ccu::LocalReduce(
                    &ctx.moRes.ccuBuf[bufBase], size, dataType, outputDataType, opType, var.loopLen[index], loopEvt, 1);
                ccu::EventWait(loopEvt, 1);
            }

            ccu::LocalCopy(var.loopDst[index], ctx.moRes.ccuBuf[bufBase], var.loopLenExp[index], loopEvt, 1);
            ccu::EventWait(loopEvt, 1);
        }));

        loops.loops[index].reset(new ccu::Loop(loops.loopParam[index], *loops.body[index]));
    }

    return CCU_SUCCESS;
}

static CcuResult GroupReduceM2M(
    KfcAllReduceMesh1DMem2MemContext& ctx, ccu::LocalAddr dst, std::vector<ccu::RemoteAddr> src,
    ccu::LocalAddr localSrc, GroupOpSizeVars goSize, HcclDataType dataType, HcclDataType outputDataType,
    HcclReduceOp opType)
{
    GroupReduceVar& var = ctx.reduceVar;
    CCU_CHK_RET(CreateReduceLoopM2M(ctx, var, ctx.channelCount, dataType, outputDataType, opType));
    auto& loops = ctx.loopMap["reduce_m2m"];

    uint32_t size = ctx.channelCount + 1;
    uint32_t expansionNum = GetReduceExpansionNum(opType, dataType, outputDataType);
    ccu::Variable sliceSizeExpansion;

    if (expansionNum != 1) {
        ccu::Variable tmp;
        tmp = GetExpansionParam(expansionNum);
        dst.token = dst.token + tmp;
    }

    CCU_IF(goSize.addrOffset != 0)
    {
        ccu::Variable loopParam;
        loopParam = GetLoopParam(0, ctx.moConfig.memSlice * ctx.moConfig.loopCount, 0);
        loopParam = loopParam + goSize.loopParam;

        ccu::Variable sliceSize;
        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        for (uint32_t i = 0; i < ctx.channelCount; ++i) {
            var.loopRemoteSrc[0][i].addr = src[i].addr;
            var.loopRemoteSrc[0][i].token = src[i].token;
        }
        var.loopLocalSrc[0].addr = localSrc.addr;
        var.loopLocalSrc[0].token = localSrc.token;
        var.loopDst[0].addr = dst.addr;
        var.loopDst[0].token = dst.token;
        var.loopLen[0] = sliceSize;
        var.loopLenExp[0] = sliceSizeExpansion;

        ccu::Variable paraCfg;
        paraCfg = GetParallelParam(ctx.moConfig.loopCount - 1, 0, 1);

        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopParam;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0]};
        ccu::LoopGroup group(paraCfg, offsetCfg, 1, grpLoops);
    }

    CCU_IF(goSize.parallelParam != 0)
    {
        for (uint32_t i = 0; i < ctx.channelCount; i++) {
            src[i].addr += goSize.addrOffset;
        }
        localSrc.addr += goSize.addrOffset;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += goSize.addrOffset;
        }

        sliceSizeExpansion = 0;
        for (uint32_t i = 0; i < expansionNum; i++) {
            sliceSizeExpansion = sliceSizeExpansion + goSize.residual;
        }
        for (uint32_t i = 0; i < ctx.channelCount; ++i) {
            var.loopRemoteSrc[0][i].addr = src[i].addr;
            var.loopRemoteSrc[0][i].token = src[i].token;
        }
        var.loopLocalSrc[0].addr = localSrc.addr;
        var.loopLocalSrc[0].token = localSrc.token;
        var.loopDst[0].addr = dst.addr;
        var.loopDst[0].token = dst.token;
        var.loopLen[0] = goSize.residual;
        var.loopLenExp[0] = sliceSizeExpansion;

        for (uint32_t i = 0; i < ctx.channelCount; i++) {
            src[i].addr += goSize.residual;
        }
        localSrc.addr += goSize.residual;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += goSize.residual;
        }

        ccu::Variable sliceSize;
        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        for (uint32_t i = 0; i < ctx.channelCount; ++i) {
            var.loopRemoteSrc[1][i].addr = src[i].addr;
            var.loopRemoteSrc[1][i].token = src[i].token;
        }
        var.loopLocalSrc[1].addr = localSrc.addr;
        var.loopLocalSrc[1].token = localSrc.token;
        var.loopDst[1].addr = dst.addr;
        var.loopDst[1].token = dst.token;
        var.loopLen[1] = sliceSize;
        var.loopLenExp[1] = sliceSizeExpansion;

        ccu::Variable loopCfg0;
        loopCfg0 = GetLoopParam(0, 0, 1);

        ccu::Variable loopCfg1;
        loopCfg1 = GetLoopParam(0, 0, 1);

        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopCfg0;
        loops.loopParam[1] = loopCfg1;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0], *loops.loops[1]};
        uint32_t loopNum = 2;
        ccu::LoopGroup group(goSize.parallelParam, offsetCfg, loopNum, grpLoops);
    }
    return CCU_SUCCESS;
}

static CcuResult CreateBroadcastLoopM2M(
    KfcAllReduceMesh1DMem2MemContext& ctx, GroupBroadcastVar& var, uint32_t channelSize)
{
    AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, CCU_M2M_LOCAL_COPY_LOOP_COUNT);
    if (ctx.IsLoopEntityRegistered("broadcast_m2m")) {
        return CCU_SUCCESS;
    }
    ctx.CreateLoopEntity("broadcast_m2m");
    auto& loops = ctx.loopMap["broadcast_m2m"];

    uint32_t loopNum = 2;
    for (int32_t index = 0; index < loopNum; index++) {
        var.loopRemoteDst[index].resize(channelSize);

        uint32_t bufBase = index * ctx.moConfig.msInterleave;
        ccu::Event loopEvt = ctx.moRes.completedEvent[index];

        loops.body[index].reset(new ccu::Func([&ctx, index, bufBase, loopEvt, channelSize, &var]() {
            ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase], var.loopSrc[index], var.loopLen[index], loopEvt, 1);
            ccu::EventWait(loopEvt, 1);

            for (uint32_t i = 0; i < channelSize; i++) {
                ccu::Write(
                    ctx.channels[i], var.loopRemoteDst[index][i], ctx.moRes.ccuBuf[bufBase], var.loopLen[index], loopEvt,
                    1 << i);
            }

            ccu::LocalCopy(
                var.loopLocalDst[index], ctx.moRes.ccuBuf[bufBase], var.loopLen[index], loopEvt, 1 << channelSize);

            ccu::EventWait(loopEvt, (1 << (channelSize + 1)) - 1);
        }));

        loops.loops[index].reset(new ccu::Loop(loops.loopParam[index], *loops.body[index]));
    }

    return CCU_SUCCESS;
}

static CcuResult GroupBroadcastM2M(
    KfcAllReduceMesh1DMem2MemContext& ctx, std::vector<ccu::RemoteAddr> dst, ccu::LocalAddr localDst,
    ccu::LocalAddr src, GroupOpSizeVars goSize)
{
    GroupBroadcastVar& var = ctx.broadcastVar;
    CCU_CHK_RET(CreateBroadcastLoopM2M(ctx, var, ctx.channelCount));
    auto& loops = ctx.loopMap["broadcast_m2m"];

    CCU_IF(goSize.loopParam != 0)
    {
        ccu::Variable loopParam;
        loopParam = GetLoopParam(0, ctx.moConfig.memSlice * ctx.moConfig.loopCount, 0);
        loopParam = loopParam + goSize.loopParam;

        ccu::Variable sliceSize;
        sliceSize = ctx.moConfig.memSlice;

        var.loopSrc[0].addr = src.addr;
        var.loopSrc[0].token = src.token;
        var.loopLocalDst[0].addr = localDst.addr;
        var.loopLocalDst[0].token = localDst.token;
        for (uint32_t i = 0; i < ctx.channelCount; ++i) {
            var.loopRemoteDst[0][i].addr = dst[i].addr;
            var.loopRemoteDst[0][i].token = dst[i].token;
        }
        var.loopLen[0] = sliceSize;

        ccu::Variable paraCfg;
        paraCfg = GetParallelParam(ctx.moConfig.loopCount - 1, 0, 1);

        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopParam;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0]};
        ccu::LoopGroup group(paraCfg, offsetCfg, 1, grpLoops);
    }

    CCU_IF(goSize.parallelParam != 0)
    {
        src.addr += goSize.addrOffset;
        localDst.addr += goSize.addrOffset;
        for (uint32_t i = 0; i < ctx.channelCount; i++) {
            dst[i].addr += goSize.addrOffset;
        }

        var.loopSrc[0].addr = src.addr;
        var.loopSrc[0].token = src.token;
        var.loopLocalDst[0].addr = localDst.addr;
        var.loopLocalDst[0].token = localDst.token;
        for (uint32_t i = 0; i < ctx.channelCount; ++i) {
            var.loopRemoteDst[0][i].addr = dst[i].addr;
            var.loopRemoteDst[0][i].token = dst[i].token;
        }
        var.loopLen[0] = goSize.residual;

        src.addr += goSize.residual;
        localDst.addr += goSize.residual;
        for (uint32_t i = 0; i < ctx.channelCount; i++) {
            dst[i].addr += goSize.residual;
        }

        ccu::Variable sliceSize;
        sliceSize = ctx.moConfig.memSlice;

        var.loopSrc[1].addr = src.addr;
        var.loopSrc[1].token = src.token;
        var.loopLocalDst[1].addr = localDst.addr;
        var.loopLocalDst[1].token = localDst.token;
        for (uint32_t i = 0; i < ctx.channelCount; ++i) {
            var.loopRemoteDst[1][i].addr = dst[i].addr;
            var.loopRemoteDst[1][i].token = dst[i].token;
        }
        var.loopLen[1] = sliceSize;

        ccu::Variable loopCfg0;
        loopCfg0 = GetLoopParam(0, 0, 1);

        ccu::Variable loopCfg1;
        loopCfg1 = GetLoopParam(0, 0, 1);

        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopCfg0;
        loops.loopParam[1] = loopCfg1;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0], *loops.loops[1]};
        uint32_t loopNum = 2;
        ccu::LoopGroup group(goSize.parallelParam, offsetCfg, loopNum, grpLoops);
    }
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcAllReduceMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable mySliceSize, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, ccu::Variable chunkSize, ccu::Variable tailSize, ccu::Variable chunkLoopNum,
    ccu::Variable fullGoSize0, ccu::Variable fullGoSize1, ccu::Variable fullGoSize2, ccu::Variable fullGoSize3,
    ccu::Variable tailGoSize0, ccu::Variable tailGoSize1, ccu::Variable tailGoSize2, ccu::Variable tailGoSize3)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.myScratch = scratch;
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
    ctx.chunkSize = chunkSize;
    ctx.tailSize = tailSize;
    ctx.chunkLoopNum = chunkLoopNum;
    ctx.fullGoSize.addrOffset = fullGoSize0;
    ctx.fullGoSize.loopParam = fullGoSize1;
    ctx.fullGoSize.parallelParam = fullGoSize2;
    ctx.fullGoSize.residual = fullGoSize3;
    ctx.tailGoSize.addrOffset = tailGoSize0;
    ctx.tailGoSize.loopParam = tailGoSize1;
    ctx.tailGoSize.parallelParam = tailGoSize2;
    ctx.tailGoSize.residual = tailGoSize3;
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

static CcuResult ReduceRmtToLocChunking(
    KfcAllReduceMesh1DMem2MemContext& ctx, const std::vector<ccu::Variable>& srcAddr, const ccu::Variable& dstAddr)
{
    if (srcAddr.size() != ctx.channelCount + 1) {
        HCCL_ERROR(
            "[ReduceRmtToLocChunking] srcAddr.size[%zu] != channels_ size[%zu] +1", srcAddr.size(), ctx.channelCount);
        return CCU_SUCCESS;
    }

    ctx.localDstMem.addr = dstAddr;
    ctx.localDstMem.addr += ctx.sliceOffset;
    ctx.localDstMem.token = ctx.token[ctx.rankId];

    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        ctx.reduceScatterSrc[rankIdx].addr = srcAddr[rankIdx];
        ctx.reduceScatterSrc[rankIdx].addr += ctx.sliceOffset;
        ctx.reduceScatterSrc[rankIdx].token = ctx.token[rankIdx];
    }

    if (ctx.rankSize <= GROUP_REDUCE_MAX_PIECE_CNT) {
        std::vector<ccu::RemoteAddr> remoteSrc;
        remoteSrc.reserve(ctx.channelCount);
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx != ctx.rankId) {
                remoteSrc.push_back(ctx.reduceScatterSrc[rankIdx]);
            }
        }

        ccu::LocalAddr localSrc;
        localSrc.addr = ctx.reduceScatterSrc[ctx.rankId].addr;
        localSrc.token = ctx.reduceScatterSrc[ctx.rankId].token;

        CCU_CHK_RET(GroupReduceM2M(
            ctx, ctx.localDstMem, remoteSrc, localSrc, ctx.goSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp));
    } else {
        ccu::Variable scratchOffset;
        scratchOffset = 0;
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
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
        CCU_CHK_RET(PairwiseLocalReduce(
            ctx, ctx.localDstMem, ctx.reduceScatterDst, ctx.currentSliceSize, ctx.dataType, ctx.outputDataType,
            ctx.reduceOp));
    }
    return CCU_SUCCESS;
}

static CcuResult BcastLocToRmtChunking(
    KfcAllReduceMesh1DMem2MemContext& ctx, const ccu::Variable& srcAddr, const std::vector<ccu::Variable>& dstAddr)
{
    if (dstAddr.size() != ctx.channelCount + 1) {
        HCCL_ERROR(
            "[BcastLocToRmtChunking] dstAddr.size[%zu] != channels_ size[%zu] + 1", dstAddr.size(), ctx.channelCount);
        return CCU_SUCCESS;
    }

    ccu::LocalAddr src;
    src.addr = srcAddr;
    src.addr += ctx.sliceOffset;
    src.token = ctx.token[ctx.rankId];

    std::vector<ccu::RemoteAddr> remoteDst;
    remoteDst.reserve(ctx.channelCount);
    for (uint32_t rmtId = 0; rmtId < dstAddr.size(); rmtId++) {
        if (rmtId == ctx.rankId) {
            continue;
        }
        ccu::RemoteAddr rmt;
        rmt.addr = dstAddr[rmtId];
        rmt.addr += ctx.sliceOffset;
        rmt.token = ctx.token[rmtId];
        remoteDst.push_back(rmt);
    }

    CCU_CHK_RET(GroupBroadcastM2M(ctx, remoteDst, src, src, ctx.goSize));
    return CCU_SUCCESS;
}

static CcuResult DoRepeatAllReduceChunking(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    ccu::Variable one;
    one = 1;
    CCU_WHILE(ctx.chunkLoopNum != static_cast<uint64_t>(UINT64_MAX))
    {
        ctx.currentSliceSize = ctx.chunkSize;
        ctx.goSize = ctx.fullGoSize;
        CCU_IF(ctx.chunkLoopNum == static_cast<uint64_t>(UINT64_MAX - 1))
        {
            ctx.currentSliceSize = ctx.tailSize;
            ctx.goSize = ctx.tailGoSize;
        }
        ctx.chunkLoopNum += one;
        CCU_CHK_RET(ReduceRmtToLocChunking(ctx, ctx.input, ctx.output[ctx.rankId]));
        CCU_CHK_RET(BcastLocToRmtChunking(ctx, ctx.output[ctx.rankId], ctx.output));
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            ctx.input[rankIdx] += ctx.currentSliceSize;
            ctx.output[rankIdx] += ctx.currentSliceSize;
        }
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable mySliceSize, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, ccu::Variable chunkSize, ccu::Variable tailSize, ccu::Variable chunkLoopNum,
    ccu::Variable fullGoSize0, ccu::Variable fullGoSize1, ccu::Variable fullGoSize2, ccu::Variable fullGoSize3,
    ccu::Variable tailGoSize0, ccu::Variable tailGoSize1, ccu::Variable tailGoSize2, ccu::Variable tailGoSize3,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
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
        normalSliceSize, lastSliceSize, mySliceSize, sliceOffset, isInputOutputEqual, goSize0, goSize1, goSize2,
        goSize3, chunkSize, tailSize, chunkLoopNum, fullGoSize0, fullGoSize1, fullGoSize2, fullGoSize3, tailGoSize0,
        tailGoSize1, tailGoSize2, tailGoSize3));
    CCU_CHK_RET(PreSync(ctx));

    CCU_IF(ctx.mySliceSize != 0) { CCU_CHK_RET(DoRepeatAllReduceChunking(ctx)); }

    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CcuKernelAllReduceMeshMem2Mem1D] AllReduceMeshMem2Mem1D end");

    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
