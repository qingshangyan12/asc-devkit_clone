/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// A5(Ascend950) 单server CCU_SCHED 2~8卡专用 ReduceScatter kernel（大消息双缓冲流水线版）。
// 自 hccl ccu_kernel_reduce_scatter_mesh1d_a5_mem2mem.cc 移植，适配 asc-devkit KFC server 模型：
// 1. 形参由 ccu::LoadArg 改为显式 ccu::Variable（由 DispatchKfcSubKernel 从 param[] 透传）；
// 2. 规约阶段复用 asc-devkit 的 GroupLocalReduce（本rank槽位直读 input 的优化通过 scratch[rankId]=myInput 注入）；
// 3. A5 专属的双缓冲流水线（read-ahead 重叠 + 有界 scratch）原样保留。
// 计数器采用 UINT64_MAX 编码、先自增后比较；尾 chunk 通过条件切换 readLen 与 goSize(B组) 处理。

#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_a5_mem2mem.h"
#include "mc2_type.h"

namespace mc2_ops_hccl {
using namespace hcomm;

constexpr int A5_INPUT_XN_ID = 0;
constexpr int A5_SCRATCH_XN_ID = 1;
constexpr int A5_TOKEN_XN_ID = 2;
constexpr int A5_POST_SYNC_ID = 3;
constexpr int A5_CKE_IDX_0 = 0;
// 与 hccl_inner_def.h 的 CCU_LOOP_COUNT_M2M_RE 保持一致（16）；本 TU 未 include 该头，故本地定义。
constexpr uint64_t CCU_LOOP_COUNT_M2M_RE = 16;

struct KfcReduceScatterMesh1DA5Mem2MemContext : CcuKernelCtxBase {
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
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable scratchRepeatStride;
    ccu::Variable scratchSlotStride;
    ccu::Variable repeatNum;
    ccu::Variable reducedNum;
    ccu::Variable flag;
    ccu::Variable constVar1;
    ccu::Variable readLen;
    GroupOpSizeVars goSize;
    ccu::Variable goSizeBAddrOffset;
    ccu::Variable goSizeBLoopParam;
    ccu::Variable goSizeBParallelParam;
    ccu::Variable goSizeBResidual;

    ccu::Event eventA;
    ccu::Event eventB;

    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMemA;
    std::vector<ccu::LocalAddr> scratchMemB;
};

static CcuResult InitResource(KfcReduceScatterMesh1DA5Mem2MemContext& ctx)
{
    uint32_t channelIdx = 0;

    if (ctx.channelCount == 0) {
        HCCL_ERROR("[KfcReduceScatterMesh1DA5Mem2Mem] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    if (ctx.rankSize < RS_A5_MIN_RANK_SIZE || ctx.rankSize > RS_A5_MAX_RANK_SIZE) {
        HCCL_ERROR(
            "[KfcReduceScatterMesh1DA5Mem2Mem] rankSize[%u] is out of A5 range [%u, %u]!", ctx.rankSize,
            RS_A5_MIN_RANK_SIZE, RS_A5_MAX_RANK_SIZE);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.scratch.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    ctx.remoteInput.resize(ctx.rankSize);
    ctx.scratchMemA.resize(ctx.rankSize);
    ctx.scratchMemB.resize(ctx.rankSize);

    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A5_INPUT_XN_ID);
            ctx.scratch[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A5_SCRATCH_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A5_TOKEN_XN_ID);
            channelIdx++;
        }
    }

    ctx.constVar1 = 1;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcReduceScatterMesh1DA5Mem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset,
    ccu::Variable currentRankSliceOutputOffset, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable scratchRepeatStride,
    ccu::Variable repeatNum, ccu::Variable goAddrOffset, ccu::Variable goLoopParam, ccu::Variable goParallelParam,
    ccu::Variable goResidual, ccu::Variable goBAddrOffset, ccu::Variable goBLoopParam, ccu::Variable goBParallelParam,
    ccu::Variable goBResidual, ccu::Variable scratchSlotStride)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.scratch[ctx.rankId] = scratch;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = currentRankSliceOutputOffset;
    ctx.inputRepeatStride = inputRepeatStride;
    ctx.outputRepeatStride = outputRepeatStride;
    ctx.normalSliceSize = normalSliceSize;
    ctx.lastSliceSize = lastSliceSize;
    ctx.scratchRepeatStride = scratchRepeatStride;
    ctx.scratchSlotStride = scratchSlotStride;
    ctx.repeatNum = repeatNum;
    ctx.goSize.addrOffset = goAddrOffset;
    ctx.goSize.loopParam = goLoopParam;
    ctx.goSize.parallelParam = goParallelParam;
    ctx.goSize.residual = goResidual;
    ctx.goSizeBAddrOffset = goBAddrOffset;
    ctx.goSizeBLoopParam = goBLoopParam;
    ctx.goSizeBParallelParam = goBParallelParam;
    ctx.goSizeBResidual = goBResidual;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcReduceScatterMesh1DA5Mem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.input[ctx.rankId], A5_INPUT_XN_ID, A5_CKE_IDX_0, 1 << A5_INPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.scratch[ctx.rankId], A5_SCRATCH_XN_ID, A5_CKE_IDX_0, 1 << A5_SCRATCH_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], A5_TOKEN_XN_ID, A5_CKE_IDX_0, 1 << A5_TOKEN_XN_ID));
    }
    uint32_t allBit = 1 << A5_INPUT_XN_ID | 1 << A5_SCRATCH_XN_ID | 1 << A5_TOKEN_XN_ID;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], A5_CKE_IDX_0, allBit);
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcReduceScatterMesh1DA5Mem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], A5_CKE_IDX_0, 1 << A5_POST_SYNC_ID);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], A5_CKE_IDX_0, 1 << A5_POST_SYNC_ID);
    }
    return CCU_SUCCESS;
}

// 初始化流水线地址：myInput/remoteInput 指向 chunk0 基址；
// scratchMemA/B[r] = scratch + r*slotStride (+A->B 偏移)，A/B 两组 slot 静态绑定。
static void InitPipelineAddr(KfcReduceScatterMesh1DA5Mem2MemContext& ctx)
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

        ctx.scratchMemA[rankIdx].addr = ctx.scratch[ctx.rankId];
        ctx.scratchMemA[rankIdx].addr += scratchOffset;
        ctx.scratchMemA[rankIdx].token = ctx.token[ctx.rankId];

        ctx.scratchMemB[rankIdx].addr = ctx.scratchMemA[rankIdx].addr;
        ctx.scratchMemB[rankIdx].addr += ctx.scratchRepeatStride;
        ctx.scratchMemB[rankIdx].token = ctx.token[ctx.rankId];

        scratchOffset += ctx.scratchSlotStride;
    }
}

// 发起当前 chunk 的读取：本 rank 直接 EventRecord 占位，远端 rank 各一次非阻塞 Read，
// 信号统一落在指定 buffer 的 event 上。read 长度由 ctx.readLen 承载。
static void IssueChunkReads(
    KfcReduceScatterMesh1DA5Mem2MemContext& ctx, ccu::Event& event, std::vector<ccu::LocalAddr>& scratchSet)
{
    uint32_t channelId = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t rankMask = 1u << rankIdx;
        if (rankIdx == ctx.rankId) {
            ccu::EventRecord(event, rankMask);
        } else {
            ccu::Read(
                ctx.channels[channelId], scratchSet[rankIdx], ctx.remoteInput[rankIdx], ctx.readLen, event, rankMask);
            channelId++;
        }
    }
}

// 远端读基址步进到下一 chunk。
static void StepRemoteInput(KfcReduceScatterMesh1DA5Mem2MemContext& ctx)
{
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx == ctx.rankId) {
            continue;
        }
        ctx.remoteInput[rankIdx].addr += ctx.inputRepeatStride;
    }
}

// 尾 chunk：goSize 切换到 B 组。
static void SwitchToLastChunkGoSize(KfcReduceScatterMesh1DA5Mem2MemContext& ctx)
{
    ctx.goSize.addrOffset = ctx.goSizeBAddrOffset;
    ctx.goSize.loopParam = ctx.goSizeBLoopParam;
    ctx.goSize.parallelParam = ctx.goSizeBParallelParam;
    ctx.goSize.residual = ctx.goSizeBResidual;
}

// 规约当前 chunk：复用 asc-devkit GroupLocalReduce；本rank槽位直读 input 通过 scratch[rankId]=myInput 注入。
// 传入 scratchSet 的副本（GroupLocalReduce 内部会推进地址），保持 scratchMemA/B 基址恒定。
static CcuResult DoReduceScatter(KfcReduceScatterMesh1DA5Mem2MemContext& ctx, std::vector<ccu::LocalAddr>& scratchSet)
{
    ccu::LocalAddr myOutput;
    myOutput.addr = ctx.output;
    myOutput.addr += ctx.currentRankSliceOutputOffset;
    myOutput.token = ctx.token[ctx.rankId];

    std::vector<ccu::LocalAddr> scratch = scratchSet;
    scratch[ctx.rankId] = ctx.myInput;
    CCU_CHK_RET(GroupLocalReduce(ctx, myOutput, scratch, ctx.goSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp));
    return CCU_SUCCESS;
}

// 流水线半体（A/B 缓冲各发射一次）：
// 1) 若有下一 chunk：设置 readLen（统一/尾）后向对侧 buffer 发起非阻塞读并步进远端基址；
// 2) 计入当前 chunk；若为尾 chunk 则切换 goSize 到 B 组；
// 3) 等待当前 buffer 的 event（全部 rank 位）；
// 4) 非首轮则步进 myInput/output；
// 5) 从当前 buffer 规约当前 chunk。
static CcuResult PipelineHalfBody(
    KfcReduceScatterMesh1DA5Mem2MemContext& ctx, ccu::Event& waitEvent, std::vector<ccu::LocalAddr>& reduceSet,
    ccu::Event& issueEvent, std::vector<ccu::LocalAddr>& issueSet, uint32_t rankBits)
{
    CCU_IF(ctx.repeatNum != static_cast<uint64_t>(UINT64_MAX))
    {
        ctx.repeatNum += ctx.constVar1;
        ctx.readLen = ctx.normalSliceSize;
        CCU_IF(ctx.repeatNum == static_cast<uint64_t>(UINT64_MAX)) { ctx.readLen = ctx.lastSliceSize; }
        IssueChunkReads(ctx, issueEvent, issueSet);
        StepRemoteInput(ctx);
    }

    ctx.reducedNum += ctx.constVar1;
    CCU_IF(ctx.reducedNum == static_cast<uint64_t>(UINT64_MAX)) { SwitchToLastChunkGoSize(ctx); }

    ccu::EventWait(waitEvent, rankBits);

    CCU_IF(ctx.flag != 0)
    {
        ctx.myInput.addr += ctx.inputRepeatStride;
        ctx.output += ctx.outputRepeatStride;
    }

    CCU_CHK_RET(DoReduceScatter(ctx, reduceSet));
    ctx.flag = 1;
    return CCU_SUCCESS;
}

static CcuResult DoPipelineReduceScatter(KfcReduceScatterMesh1DA5Mem2MemContext& ctx, uint32_t rankBits)
{
    InitPipelineAddr(ctx);

    ctx.reducedNum = ctx.repeatNum;
    ctx.flag = 0;

    // Prologue：发起 chunk0 的读取 -> buffer A / event A
    CCU_IF(ctx.repeatNum != static_cast<uint64_t>(UINT64_MAX))
    {
        ctx.repeatNum += ctx.constVar1;
        ctx.readLen = ctx.normalSliceSize;
        CCU_IF(ctx.repeatNum == static_cast<uint64_t>(UINT64_MAX)) { ctx.readLen = ctx.lastSliceSize; }
        IssueChunkReads(ctx, ctx.eventA, ctx.scratchMemA);
        StepRemoteInput(ctx);
    }

    CCU_WHILE(ctx.reducedNum != static_cast<uint64_t>(UINT64_MAX))
    {
        // 半体A：issue -> B/evB，wait evA，reduce from A
        CCU_CHK_RET(PipelineHalfBody(ctx, ctx.eventA, ctx.scratchMemA, ctx.eventB, ctx.scratchMemB, rankBits));
        // 半体B：整体受 guard 保护（K 为奇数时半体A已消费最后一个 chunk）
        CCU_IF(ctx.reducedNum != static_cast<uint64_t>(UINT64_MAX))
        {
            CCU_CHK_RET(PipelineHalfBody(ctx, ctx.eventB, ctx.scratchMemB, ctx.eventA, ctx.scratchMemA, rankBits));
        }
    }

    return CCU_SUCCESS;
}

CcuResult CcuKfcReduceScatterMesh1DA5Mem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride, ccu::Variable normalSliceSize,
    ccu::Variable lastSliceSize, ccu::Variable scratchRepeatStride, ccu::Variable repeatNum, ccu::Variable goAddrOffset,
    ccu::Variable goLoopParam, ccu::Variable goParallelParam, ccu::Variable goResidual, ccu::Variable goBAddrOffset,
    ccu::Variable goBLoopParam, ccu::Variable goBParallelParam, ccu::Variable goBResidual,
    ccu::Variable scratchSlotStride, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId, const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
{
    KfcReduceScatterMesh1DA5Mem2MemContext ctx;
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
        "[CcuKernelKfcReduceScatterMesh1DA5Mem2Mem] Init, KernelArgs are rankId[%u], rankSize[%u], dataType[%d], "
        "outputDataType[%d], reduceOp[%d]",
        ctx.rankId, ctx.rankSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp);

    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;
    ctx.moConfig.msInterleave = CCU_MS_INTERLEAVE;
    ctx.moConfig.loopCount = CCU_LOOP_COUNT_M2M_RE;
    ctx.moConfig.memSlice = CCU_MS_SIZE;

    HCCL_INFO("[KFC CcuKernelKfcReduceScatterMesh1DA5Mem2Mem] ReduceScatterMesh1DA5Mem2Mem run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset, currentRankSliceOutputOffset,
        inputRepeatStride, outputRepeatStride, normalSliceSize, lastSliceSize, scratchRepeatStride, repeatNum,
        goAddrOffset, goLoopParam, goParallelParam, goResidual, goBAddrOffset, goBLoopParam, goBParallelParam,
        goBResidual, scratchSlotStride));
    CCU_CHK_RET(PreSync(ctx));

    uint32_t rankBits = (1u << ctx.rankSize) - 1u;
    // chunk == 0时不需要 read/wait/reduce，只需前后同步
    CCU_IF(ctx.normalSliceSize != 0) { CCU_CHK_RET(DoPipelineReduceScatter(ctx, rankBits)); }

    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[KFC CcuKernelKfcReduceScatterMesh1DA5Mem2Mem] ReduceScatterMesh1DA5Mem2Mem end");

    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
