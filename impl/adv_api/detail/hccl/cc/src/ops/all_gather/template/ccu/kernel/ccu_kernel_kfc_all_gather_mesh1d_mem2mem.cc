/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_all_gather_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {

constexpr int AG_M2M_OUTPUT_XN_ID = 1;
constexpr int AG_M2M_TOKEN_XN_ID = 2;
constexpr int AG_M2M_POST_SYNC_CKE_IDX = 0;
constexpr int AG_M2M_OUTPUT_CKE_IDX = 1;
constexpr int AG_M2M_TOKEN_CKE_IDX = 2;
constexpr uint16_t AG_M2M_POST_SYNC_BIT = 1U;
constexpr uint16_t AG_M2M_OUTPUT_BIT = 1U << AG_M2M_OUTPUT_XN_ID;
constexpr uint16_t AG_M2M_TOKEN_BIT = 1U << AG_M2M_TOKEN_XN_ID;
constexpr uint32_t AG_M2M_BITS_PER_EVENT = 16;

struct KfcAllGatherMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
<<<<<<< HEAD
    ccu::Variable outputOffset;
    ccu::Variable sliceSize;
=======
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable tmpRepeatNum;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
>>>>>>> f5610b3dd (test)
    ccu::Variable isInputOutputEqual;
    GroupOpSizeVars goSize;
    ccu::LocalAddr src_loccopy;
    ccu::LocalAddr localDst;
    std::vector<ccu::Event> events;

    // 3 阶段 unroll 设计新增变量（与 hccl 独立仓保持一致）
    ccu::Variable constVar1;
    ccu::Variable repeatTimeflag;
    ccu::Variable waitRepeatNum;
    ccu::Variable groupCopyRepeatNum;
    ccu::Variable sliceSize;
    ccu::LocalAddr src;
    std::vector<ccu::RemoteAddr> dst;
};

static CcuResult InitResource(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    if (ctx.rankSize == 0 || ctx.rankId >= ctx.rankSize || ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcAllGatherMesh1DMem2Mem] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AG_M2M_OUTPUT_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AG_M2M_TOKEN_XN_ID);
        channelIdx++;
    }

    // unroll 后每个 repeat 迭代需要独立的一组 event，event 之间通过位掩码错开
    const uint32_t eventNumPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    ctx.events.resize(AG_M2M_UNROLL_NUM * eventNumPerIter);

    ctx.constVar1 = 1;
    ctx.repeatTimeflag = 0;
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcAllGatherMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable outputOffset, ccu::Variable sliceSize, ccu::Variable goSize0, ccu::Variable goSize1,
<<<<<<< HEAD
    ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable isInputOutputEqual)
=======
    ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable currentRankSliceInputOffset, ccu::Variable tmpRepeatNum,
    ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride, ccu::Variable lastSliceSize,
    ccu::Variable isInputOutputEqual)
>>>>>>> f5610b3dd (test)
{
    ctx.input = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.currentRankSliceOutputOffset = outputOffset;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.normalSliceSize = sliceSize;
    ctx.lastSliceSize = lastSliceSize;
    ctx.tmpRepeatNum = tmpRepeatNum;
    ctx.inputRepeatStride = inputRepeatStride;
    ctx.outputRepeatStride = outputRepeatStride;
    ctx.isInputOutputEqual = isInputOutputEqual;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;
    ctx.isInputOutputEqual = isInputOutputEqual;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[ctx.rankId], AG_M2M_OUTPUT_XN_ID, AG_M2M_OUTPUT_CKE_IDX, AG_M2M_OUTPUT_BIT));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], AG_M2M_TOKEN_XN_ID, AG_M2M_TOKEN_CKE_IDX, AG_M2M_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_OUTPUT_CKE_IDX, AG_M2M_OUTPUT_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_TOKEN_CKE_IDX, AG_M2M_TOKEN_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], AG_M2M_POST_SYNC_CKE_IDX, AG_M2M_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_POST_SYNC_CKE_IDX, AG_M2M_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult InitAllGatherAddr(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    ctx.src.addr = ctx.input;
    ctx.src.addr += ctx.currentRankSliceInputOffset;
    ctx.src.token = ctx.token[ctx.rankId];

    ctx.src_loccopy.addr = ctx.input;
    ctx.src_loccopy.addr += ctx.currentRankSliceInputOffset;
    ctx.src_loccopy.token = ctx.token[ctx.rankId];

    ctx.dst.resize(ctx.rankSize);
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx == ctx.rankId) {
            ctx.localDst.addr = ctx.output[ctx.rankId];
            ctx.localDst.addr += ctx.currentRankSliceOutputOffset;
            ctx.localDst.token = ctx.token[ctx.rankId];
        } else {
            ctx.dst[rankIdx].addr = ctx.output[rankIdx];
            ctx.dst[rankIdx].addr += ctx.currentRankSliceOutputOffset;
            ctx.dst[rankIdx].token = ctx.token[rankIdx];
        }
    }
    return CCU_SUCCESS;
}

// Phase 1 的单个 unroll 迭代：将所有远端 WriteNb 下发（非阻塞，事件按位错开），本 rank 仅记录本地完成事件
static CcuResult DoAllGatherWrite(
    KfcAllGatherMesh1DMem2MemContext& ctx, const ccu::LocalAddr& src, const std::vector<ccu::RemoteAddr>& dst,
    uint32_t unrollIdx)
{
    const uint32_t eventNumPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        const uint32_t eventIdx = unrollIdx * eventNumPerIter + rankIdx / AG_M2M_BITS_PER_EVENT;
        const uint16_t rankMask = 1U << (rankIdx % AG_M2M_BITS_PER_EVENT);
        if (rankIdx == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
            continue;
        }
        CCU_CHK_RET(ccu::Write(
            ctx.channels[channelIdx], dst[rankIdx], src, ctx.normalSliceSize, ctx.events[eventIdx], rankMask));
        channelIdx++;
    }
    return CCU_SUCCESS;
}

// Phase 3 的单个 unroll 迭代：批量等待该迭代对应 event 组的全部位
static CcuResult DoAllGatherWait(KfcAllGatherMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    const uint32_t eventNumPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    for (uint32_t i = 0; i < eventNumPerIter; i++) {
        const uint32_t eventIdx = unrollIdx * eventNumPerIter + i;
        const uint32_t remainingRanks = ctx.rankSize - i * AG_M2M_BITS_PER_EVENT;
        const uint32_t bitCount = std::min(remainingRanks, AG_M2M_BITS_PER_EVENT);
        const uint16_t eventMask = bitCount == AG_M2M_BITS_PER_EVENT ? static_cast<uint16_t>(UINT16_MAX) :
                                                                       static_cast<uint16_t>((1U << bitCount) - 1);
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

// Phase 2：使用 CCU_WHILE 完成本 rank 的本地拷贝（input slice -> output 对应位置）
static CcuResult DoAllGatherGroupCopy(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    CCU_IF(ctx.isInputOutputEqual == 0)
    {
        CCU_IF(ctx.groupCopyRepeatNum != UINT64_MAX)
        {
            ctx.repeatTimeflag = 0;
            CCU_WHILE(ctx.groupCopyRepeatNum != UINT64_MAX)
            {
                ctx.groupCopyRepeatNum += ctx.constVar1;
                CCU_IF(ctx.repeatTimeflag != 0)
                {
                    ctx.localDst.addr += ctx.outputRepeatStride;
                    ctx.src_loccopy.addr += ctx.inputRepeatStride;
                }
                CCU_CHK_RET(GroupCopy(ctx, ctx.localDst, ctx.src_loccopy, ctx.goSize));
                ctx.repeatTimeflag = 1;
            }
        }
    }
    return CCU_SUCCESS;
}

// 三阶段 unroll 高性能版：Phase1 批量下发 WriteNb -> Phase2 GroupCopy -> Phase3 批量 WaitEvent
static CcuResult DoRepeatAllGather(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    CCU_CHK_RET(InitAllGatherAddr(ctx));
    ctx.waitRepeatNum = ctx.tmpRepeatNum;
    ctx.groupCopyRepeatNum = ctx.tmpRepeatNum;

    // Phase 1: 先下发所有 WriteNb（非阻塞，事件错开），不包含 GroupCopy
    CCU_IF(ctx.tmpRepeatNum != UINT64_MAX)
    {
        ctx.tmpRepeatNum += ctx.constVar1;
        CCU_CHK_RET(DoAllGatherWrite(ctx, ctx.src, ctx.dst, 0));
    }

    for (uint32_t i = 1; i < AG_M2M_UNROLL_NUM; i++) {
        CCU_IF(ctx.tmpRepeatNum != UINT64_MAX)
        {
            ctx.tmpRepeatNum += ctx.constVar1;
            ctx.src.addr += ctx.inputRepeatStride;
            for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
                if (rankIdx != ctx.rankId) {
                    ctx.dst[rankIdx].addr += ctx.outputRepeatStride;
                }
            }
            CCU_CHK_RET(DoAllGatherWrite(ctx, ctx.src, ctx.dst, i));
        }
    }

    // Phase 2: GroupCopy 使用 CCU_WHILE
    CCU_CHK_RET(DoAllGatherGroupCopy(ctx));

    // Phase 3: 批量 WaitEvent
    for (uint32_t i = 0; i < AG_M2M_UNROLL_NUM; i++) {
        CCU_IF(ctx.waitRepeatNum != UINT64_MAX)
        {
            ctx.waitRepeatNum += ctx.constVar1;
            CCU_CHK_RET(DoAllGatherWait(ctx, i));
        }
    }

<<<<<<< HEAD
    CCU_CHK_RET(DoAllGatherWrite(ctx, src, dst));
    CCU_IF(ctx.isInputOutputEqual == 0) { CCU_CHK_RET(GroupCopy(ctx, localDst, src, ctx.goSize)); }
    CCU_CHK_RET(DoAllGatherWait(ctx));
=======
>>>>>>> f5610b3dd (test)
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllGatherMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable outputOffset,
    ccu::Variable sliceSize, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
<<<<<<< HEAD
    ccu::Variable isInputOutputEqual, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId)
=======
    ccu::Variable currentRankSliceInputOffset, ccu::Variable tmpRepeatNum, ccu::Variable inputRepeatStride,
    ccu::Variable outputRepeatStride, ccu::Variable lastSliceSize, ccu::Variable isInputOutputEqual,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId)
>>>>>>> f5610b3dd (test)
{
    KfcAllGatherMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    CCU_CHK_RET(InitResource(ctx));
<<<<<<< HEAD
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, outputOffset, sliceSize, goSize0, goSize1, goSize2, goSize3,
=======
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, outputOffset, sliceSize, goSize0, goSize1, goSize2,
        goSize3, currentRankSliceInputOffset, tmpRepeatNum, inputRepeatStride, outputRepeatStride, lastSliceSize,
>>>>>>> f5610b3dd (test)
        isInputOutputEqual));
    CCU_CHK_RET(PreSync(ctx));

    ctx.sliceSize = (ctx.rankId == (ctx.rankSize - 1)) ? ctx.lastSliceSize : ctx.normalSliceSize;
    // sliceSize == 0 时不需要执行 AllGather，只需前后同步
    CCU_IF(ctx.sliceSize != 0) { CCU_CHK_RET(DoRepeatAllGather(ctx)); }

    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
