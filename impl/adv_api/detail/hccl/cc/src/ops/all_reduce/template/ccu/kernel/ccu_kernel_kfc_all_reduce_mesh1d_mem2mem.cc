/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <array>

#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"
#include "ccu_func_dl.hpp"
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {
using namespace hcomm;

#define MAX_LOOP_NUM 2
constexpr int INPUT_XN_ID = 0;
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;
constexpr uint16_t BIT_NUM_PER_CKE = 16;
constexpr uint16_t GROUP_REDUCE_MAX_PIECE_CNT = 8;

struct GroupReduceMem2MemVar {
    ccu::LocalAddr loopDst[MAX_LOOP_NUM];
    ccu::LocalAddr loopSrc[MAX_LOOP_NUM];
    std::array<std::vector<ccu::LocalAddr>, MAX_LOOP_NUM> loopScratch;
    ccu::Variable loopLen[MAX_LOOP_NUM];
    ccu::Variable loopLenExp[MAX_LOOP_NUM];
};

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

static CcuResult CreateReduceLoopV1(
    KfcAllReduceMesh1DMem2MemContext& ctx, GroupReduceMem2MemVar& var, uint32_t size, HcclDataType dataType,
    HcclDataType outputDataType, HcclReduceOp opType)
{
    constexpr uint32_t LOOP_NUM = 16;
    AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, LOOP_NUM);
    if (ctx.IsLoopEntityRegistered("reduce_mesh1d_mem2mem")) {
        return CCU_SUCCESS;
    }
    ctx.CreateLoopEntity("reduce_mesh1d_mem2mem");
    auto& loops = ctx.loopMap["reduce_mesh1d_mem2mem"];

    uint32_t expansionNum = GetReduceExpansionNum(opType, dataType, outputDataType);
    uint32_t usedBufNum = size > expansionNum ? size : expansionNum;

    for (int32_t index = 0; index < MAX_LOOP_NUM; index++) { // 需要实例化2个Loop
        var.loopScratch[index].resize(size);
        uint32_t bufBase = index * ctx.moConfig.msInterleave;
        ccu::Event e = ctx.moRes.completedEvent[index];
        loops.body[index].reset(new ccu::Func([&ctx, index, bufBase, e, size, &var, dataType, outputDataType,
                                               opType]() {
            for (uint32_t i = 0; i < size; i++) {
                if (i == ctx.rankId) {
                    ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase + i], var.loopSrc[index], var.loopLen[index], e, 1 << i);
                } else {
                    ccu::LocalCopy(
                        ctx.moRes.ccuBuf[bufBase + i], var.loopScratch[index][i], var.loopLen[index], e, 1 << i);
                }
            }
            ccu::EventWait(e, (1 << size) - 1);
            if (size > 1) {
                ccu::LocalReduce(
                    &ctx.moRes.ccuBuf[bufBase], size, dataType, outputDataType, opType, var.loopLen[index], e, 1);
                ccu::EventWait(e, 1);
            }

            ccu::LocalCopy(var.loopDst[index], ctx.moRes.ccuBuf[bufBase], var.loopLenExp[index], e, 1);
            ccu::EventWait(e, 1);
        }));

        loops.loops[index].reset(new ccu::Loop(loops.loopParam[index], *loops.body[index]));
    }
    return CCU_SUCCESS;
}

static CcuResult ReduceLoopGroupV1(KfcAllReduceMesh1DMem2MemContext& ctx, ccu::LocalAddr srcOrg)
{
    const uint32_t size = ctx.reduceScatterDst.size();

    ccu::LocalAddr dst;
    dst.addr = ctx.localDstMem.addr;
    dst.token = ctx.localDstMem.token;

    ccu::LocalAddr src;
    src.addr = srcOrg.addr;
    src.token = srcOrg.token;

    std::vector<ccu::LocalAddr> scratch;
    scratch.resize(size);
    for (uint32_t idx = 0; idx < size; idx++) {
        scratch[idx].addr = ctx.reduceScatterDst[idx].addr;
        scratch[idx].token = ctx.token[ctx.rankId];
    }
    GroupReduceMem2MemVar var;
    ccu::Variable tmp;
    ccu::Variable loopParam;
    ccu::Variable sliceSize;
    ccu::Variable paraCfg;
    ccu::Variable offsetCfg;
    ccu::Variable loopCfg0;
    ccu::Variable loopCfg1;
    CCU_CHK_RET(CreateReduceLoopV1(ctx, var, size, ctx.dataType, ctx.outputDataType, ctx.reduceOp));
    auto& loops = ctx.loopMap["reduce_mesh1d_mem2mem"];

    uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    ccu::Variable sliceSizeExpansion;

    if (expansionNum != 1) {
        tmp = GetExpansionParam(expansionNum);
        dst.token = dst.token + tmp;
    }

    // m部分
    CCU_IF(ctx.goSize.loopParam != 0) // goSize1
    {
        loopParam = GetLoopParam(0, ctx.moConfig.memSlice * ctx.moConfig.loopCount, 0);
        loopParam = loopParam + ctx.goSize.loopParam;
        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        for (uint32_t i = 0; i < size; ++i) {
            var.loopScratch[0][i].addr = scratch[i].addr;
            var.loopScratch[0][i].token = scratch[i].token;
        }
        var.loopSrc[0].addr = src.addr;
        var.loopSrc[0].token = src.token;
        var.loopDst[0].addr = dst.addr;
        var.loopDst[0].token = dst.token;
        var.loopLen[0] = sliceSize;
        var.loopLenExp[0] = sliceSizeExpansion;

        paraCfg = GetParallelParam(ctx.moConfig.loopCount - 1, 0, 1);

        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopParam;
        std::vector<ccu::Loop> grpLoops{*loops.loops[0]};
        ccu::LoopGroup group(paraCfg, offsetCfg, ctx.moConfig.loopCount, grpLoops);
    }

    CCU_IF(ctx.goSize.parallelParam != 0) // goSize2
    {
        // p部分，加m的偏移
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += ctx.goSize.addrOffset;
        }
        src.addr += ctx.goSize.addrOffset; // goSize0
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += ctx.goSize.addrOffset;
        }

        sliceSizeExpansion = 0;
        for (uint32_t i = 0; i < expansionNum; i++) {
            sliceSizeExpansion += ctx.goSize.residual; // goSize3
        }

        for (uint32_t i = 0; i < size; ++i) {
            var.loopScratch[0][i].addr = scratch[i].addr;
            var.loopScratch[0][i].token = scratch[i].token;
        }
        var.loopSrc[0].addr = src.addr;
        var.loopSrc[0].token = src.token;
        var.loopDst[0].addr = dst.addr;
        var.loopDst[0].token = dst.token;
        var.loopLen[0] = ctx.goSize.residual;
        var.loopLenExp[0] = sliceSizeExpansion;

        // n部分，再加p的偏移
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += ctx.goSize.residual;
        }
        src.addr += ctx.goSize.residual;
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += ctx.goSize.residual;
        }
        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        for (uint32_t i = 0; i < size; ++i) {
            var.loopScratch[1][i].addr = scratch[i].addr;
            var.loopScratch[1][i].token = scratch[i].token;
        }
        var.loopSrc[1].addr = src.addr;
        var.loopSrc[1].token = src.token;
        var.loopDst[1].addr = dst.addr;
        var.loopDst[1].token = dst.token;
        var.loopLen[1] = sliceSize;
        var.loopLenExp[1] = sliceSizeExpansion;
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

static CcuResult ReduceLoopGroup(KfcAllReduceMesh1DMem2MemContext& ctx, ccu::LocalAddr srcOrg)
{
    HCCL_INFO("select ReduceLoopGroupV1");
    return ReduceLoopGroupV1(ctx, srcOrg);
}

static CcuResult LoadArgs(KfcAllReduceMesh1DMem2MemContext& ctx, ccu::Array<ccu::Variable>& args)
{
    uint32_t argId = 0;
    ctx.input[ctx.rankId] = args[argId++];
    ctx.output[ctx.rankId] = args[argId++];
    ctx.token[ctx.rankId] = args[argId++];
    ctx.myScratch = args[argId++];
    ctx.currentRankSliceInputOffset = args[argId++];
    ctx.currentRankSliceOutputOffset = args[argId++];
    ctx.normalSliceSize = args[argId++];
    ctx.lastSliceSize = args[argId++];
    ctx.mySliceSize = args[argId++];
    ctx.sliceOffset = args[argId++];
    ctx.isInputOutputEqual = args[argId++];
    ctx.goSize.addrOffset = args[argId++];
    ctx.goSize.loopParam = args[argId++];
    ctx.goSize.parallelParam = args[argId++];
    ctx.goSize.residual = args[argId++];
    ctx.chunkSize = args[argId++];
    ctx.tailSize = args[argId++];
    ctx.chunkLoopNum = args[argId++];
    ctx.fullGoSize.addrOffset = args[argId++];
    ctx.fullGoSize.loopParam = args[argId++];
    ctx.fullGoSize.parallelParam = args[argId++];
    ctx.fullGoSize.residual = args[argId++];
    ctx.tailGoSize.addrOffset = args[argId++];
    ctx.tailGoSize.loopParam = args[argId++];
    ctx.tailGoSize.parallelParam = args[argId++];
    ctx.tailGoSize.residual = args[argId++];
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
            ctx.channels[channelIdx], ctx.remoteDstMem, ctx.srcMem, ctx.sliceSize, ctx.events[eventIdx],
            1 << (rmtId % BIT_NUM_PER_CKE));
        channelIdx++;
    }
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && eventIdx == (eventNum - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        ccu::EventWait(ctx.events[eventIdx], static_cast<uint16_t>((1 << sigNum) - 1));
    }
    return CCU_SUCCESS;
}

static CcuResult DoLocalReduce(KfcAllReduceMesh1DMem2MemContext& ctx)
{
    if (ctx.rankSize <= GROUP_REDUCE_MAX_PIECE_CNT) {
        ccu::LocalAddr srcLoc;
        srcLoc.addr = ctx.reduceScatterSrc[ctx.rankId].addr;
        srcLoc.token = ctx.reduceScatterSrc[ctx.rankId].token;
        CCU_CHK_RET(ReduceLoopGroup(ctx, srcLoc));
    } else {
        CCU_CHK_RET(PairwiseLocalReduce(
            ctx, ctx.localDstMem, ctx.reduceScatterDst, ctx.sliceSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp));
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
        scratchOffset += ctx.sliceSize;
        ctx.reduceScatterDst[rankIdx].token = ctx.token[ctx.rankId];
    }

    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = rankIdx / BIT_NUM_PER_CKE;
        if (rankIdx == ctx.rankId) {
            if (ctx.rankSize <= GROUP_REDUCE_MAX_PIECE_CNT) {
                ccu::EventRecord(ctx.events[eventIdx], 1 << (rankIdx % BIT_NUM_PER_CKE));
            } else {
                ccu::LocalAddr src;
                src.addr = ctx.reduceScatterSrc[rankIdx].addr;
                src.token = ctx.reduceScatterSrc[rankIdx].token;
                ccu::LocalCopy(
                    ctx.reduceScatterDst[rankIdx], src, ctx.sliceSize, ctx.events[eventIdx],
                    1 << (rankIdx % BIT_NUM_PER_CKE));
            }
        } else {
            ccu::Read(
                ctx.channels[channelIdx], ctx.reduceScatterDst[rankIdx], ctx.reduceScatterSrc[rankIdx], ctx.sliceSize,
                ctx.events[eventIdx], 1 << (rankIdx % BIT_NUM_PER_CKE));
            channelIdx++;
        }
    }
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t i = 0; i < eventNum; i++) {
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && i == (eventNum - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        ccu::EventWait(ctx.events[i], static_cast<uint16_t>((1 << sigNum) - 1));
    }
    CCU_CHK_RET(DoLocalReduce(ctx));
    return CCU_SUCCESS;
}

static CcuResult DoRepeatAllReduce(KfcAllReduceMesh1DMem2MemContext& ctx)
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

static CcuResult ReduceRmtToLocChunking(
    KfcAllReduceMesh1DMem2MemContext& ctx, const std::vector<ccu::Variable>& srcAddr, const ccu::Variable& dstAddr)
{
    ccu::Variable scratchOffset;
    if (srcAddr.size() != ctx.channelCount + 1) {
        HCCL_ERROR(
            "[ReduceRmtToLocChunking] srcAddr.size[%zu] != channels_ size[%zu] +1", srcAddr.size(), ctx.channelCount);
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
            if (ctx.rankSize <= GROUP_REDUCE_MAX_PIECE_CNT) {
                ccu::EventRecord(ctx.events[eventIdx], 1 << (rankIdx % BIT_NUM_PER_CKE));
            } else {
                ccu::LocalAddr src;
                src.addr = ctx.reduceScatterSrc[rankIdx].addr;
                src.token = ctx.reduceScatterSrc[rankIdx].token;
                ccu::LocalCopy(
                    ctx.reduceScatterDst[rankIdx], src, ctx.currentSliceSize, ctx.events[eventIdx],
                    1 << (rankIdx % BIT_NUM_PER_CKE));
            }
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
        ccu::EventWait(ctx.events[i], static_cast<uint16_t>((1 << sigNum) - 1));
    }
    if (ctx.rankSize <= GROUP_REDUCE_MAX_PIECE_CNT) {
        std::vector<ccu::LocalAddr> scratch = ctx.reduceScatterDst;
        scratch[ctx.rankId].addr = ctx.reduceScatterSrc[ctx.rankId].addr;
        scratch[ctx.rankId].token = ctx.reduceScatterSrc[ctx.rankId].token;
        CCU_CHK_RET(GroupLocalReduce(
            ctx, ctx.localDstMem, scratch, ctx.goSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp,
            GetCcuVersion()));
    } else {
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
            static_cast<uint16_t>(1 << (rmtId % BIT_NUM_PER_CKE)));
        channelIdx++;
    }
    uint32_t eventNum = (ctx.rankSize + BIT_NUM_PER_CKE - 1) / BIT_NUM_PER_CKE;
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        uint32_t sigNum = BIT_NUM_PER_CKE;
        if (ctx.rankSize % BIT_NUM_PER_CKE != 0 && eventIdx == (eventNum - 1)) {
            sigNum = ctx.rankSize % BIT_NUM_PER_CKE;
        }
        ccu::EventWait(ctx.events[eventIdx], static_cast<uint16_t>((1 << sigNum) - 1));
    }
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

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgAllReduceKfc*>(arg);
    KfcAllReduceMesh1DMem2MemContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = static_cast<uint32_t>(kernelArg->rankSize);
    ctx.rankId = kernelArg->rankId;
    ctx.dataType = kernelArg->opParam.DataDes.dataType;
    ctx.outputDataType = kernelArg->opParam.DataDes.outputType;
    if (ctx.outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        ctx.outputDataType = ctx.dataType;
    }
    ctx.reduceOp = kernelArg->opParam.reduceType;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));
    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadArgs(ctx, args);
            PreSync(ctx);
            CCU_IF(ctx.mySliceSize != 0) { DoRepeatAllReduceChunking(ctx); }
            PostSync(ctx);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_ALL_REDUCE_MESH1D_MEM2MEM_NAME);
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
