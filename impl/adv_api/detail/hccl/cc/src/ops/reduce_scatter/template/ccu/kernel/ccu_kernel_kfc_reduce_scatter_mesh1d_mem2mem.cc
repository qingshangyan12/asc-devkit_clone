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
using namespace hcomm;

constexpr int INPUT_XN_ID = 0;
constexpr int SCRATCH_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;

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
    ccu::Variable chunkCount;
    ccu::Variable currentSliceSize;
    ccu::Variable tailSize;
    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMem;
    ccu::Event event;
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

    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset, ccu::Variable chunkSize,
    ccu::Variable chunkCount, ccu::Variable tailChunkSize)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.scratch[ctx.rankId] = scratch;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = 0;
    // chunk 元数据由 Device Prepare 计算并通过 XN5/6/8 下发：
    // chunkSize 为非尾块最大字节数（scratch/rankSize 对齐 128B），chunkCount 为总块数，tailChunkSize 为尾块字节数
    ctx.chunkSize = chunkSize;
    ctx.chunkCount = chunkCount;
    ctx.tailSize = tailChunkSize;
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

static CcuResult DoReduceScatter(KfcReduceScatterMesh1DMem2MemContext& ctx, ccu::Event event)
{
    uint32_t channelId = 0;

    ccu::LocalAddr myOutput;
    myOutput.addr = ctx.output;
    myOutput.addr += ctx.currentRankSliceOutputOffset;
    myOutput.token = ctx.token[ctx.rankId];

    CCU_IF(ctx.currentSliceSize != 0)
    {
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx == ctx.rankId) {
                ccu::LocalCopy(ctx.scratchMem[rankIdx], ctx.myInput, ctx.currentSliceSize, event, 1 << rankIdx);
            } else {
                ccu::Read(
                    ctx.channels[channelId], ctx.scratchMem[rankIdx], ctx.remoteInput[rankIdx], ctx.currentSliceSize, event, 1 << rankIdx);
                channelId++;
            }
        }

        ccu::EventWait(event, (1 << ctx.rankSize) - 1);

        // Keep pr_4523's hand-written reduction to avoid loop-group block resource expansion in KFC server.
        uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
        ccu::LocalAddr reduceDst = myOutput;
        if (expansionNum != 1) {
            ccu::Variable tmp;
            tmp = GetExpansionParam(expansionNum);
            reduceDst.token = reduceDst.token + tmp;
        }
        ccu::LocalCopy(reduceDst, ctx.scratchMem[0], ctx.currentSliceSize, event, 1);
        ccu::EventWait(event, 1);
        for (uint32_t i = 1; i < ctx.rankSize; i++) {
            ccu::LocalReduce(reduceDst, ctx.scratchMem[i], ctx.currentSliceSize, ctx.dataType, ctx.reduceOp, event, 1);
            ccu::EventWait(event, 1);
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoChunkedReduceScatter(KfcReduceScatterMesh1DMem2MemContext& ctx)
{
    ccu::Variable scratchOffset;
    scratchOffset = 0;

    // scratch 按最大 chunkSize 固定划分（步长不得使用尾块大小），单 chunk 内 rankSize * chunkSize 不超过容量
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

    ccu::Variable chunkLeft;
    chunkLeft = ctx.chunkCount;
    ccu::Variable chunkAdd;
    chunkAdd = 1;
    // 满块循环：每轮搬运 chunkSize 字节，输入/输出地址同步推进，scratch 跨 chunk 复用
    CCU_WHILE(chunkLeft != 0)
    {
        chunkLeft = chunkLeft - chunkAdd;
        ctx.currentSliceSize = ctx.chunkSize;
        CCU_CHK_RET(DoReduceScatter(ctx, ctx.event));
        for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx == ctx.rankId) {
                ctx.myInput.addr += ctx.currentSliceSize;
            } else {
                ctx.remoteInput[rankIdx].addr += ctx.currentSliceSize;
            }
        }
        ctx.output += ctx.currentSliceSize;
    }

    // 尾块：totalSliceSize 被 chunkSize 整除时尾块仍为 chunkSize，零数据时为 0（DoReduceScatter 内部跳过）
    ctx.currentSliceSize = ctx.tailSize;
    CCU_CHK_RET(DoReduceScatter(ctx, ctx.event));
    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable chunkSize, ccu::Variable chunkCount,
    ccu::Variable tailChunkSize, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId, const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
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

    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;

    HCCL_INFO("[CcuKernelReduceScatterMesh1DMem2Mem] ReduceScatterMesh1DMem2Mem run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(
        LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset, chunkSize, chunkCount,
                 tailChunkSize));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoChunkedReduceScatter(ctx));
    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CcuKernelReduceScatterMesh1DMem2Mem] ReduceScatterMesh1DMem2Mem end");

    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
