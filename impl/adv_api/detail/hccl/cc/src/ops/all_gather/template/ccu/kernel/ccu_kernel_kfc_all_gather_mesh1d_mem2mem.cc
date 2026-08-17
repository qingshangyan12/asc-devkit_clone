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
constexpr uint16_t AG_M2M_BIT_NUM_PER_CKE = 16;

struct KfcAllGatherMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable tmpRepeatNum;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable isInputOutputEqual;
    GroupOpSizeVars goSize;

    // 地址资源（对齐高性能版）
    ccu::LocalAddr src;               // 指向 input 数据（用于远端 Write）
    ccu::LocalAddr src_loccopy;       // 指向 input 数据（用于本卡 GroupCopy）
    ccu::LocalAddr localDst;          // 指向 output 中本卡位置（GroupCopy 目标）
    std::vector<ccu::RemoteAddr> dst; // 远端 output 位置

    // 每个展开轮次使用独立事件，单个 event 的 16 个 mask bit 对应连续 rank。
    std::vector<ccu::Event> events;

    // 循环控制变量
    ccu::Variable constVar1;
    ccu::Variable repeatTimeflag;
    ccu::Variable waitRepeatNum;
    ccu::Variable groupCopyRepeatNum;
    ccu::Variable sliceSize;
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

    ctx.dst.resize(ctx.rankSize);
    const uint32_t eventNum = (ctx.rankSize + AG_M2M_BIT_NUM_PER_CKE - 1) / AG_M2M_BIT_NUM_PER_CKE;
    ctx.events.resize(AG_M2M_UNROLL_NUM * eventNum);

    ctx.constVar1 = 1;
    ctx.repeatTimeflag = 0;
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcAllGatherMesh1DMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable outputOffset, ccu::Variable sliceSize, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable currentRankSliceInputOffset, ccu::Variable tmpRepeatNum,
    ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride, ccu::Variable lastSliceSize,
    ccu::Variable isInputOutputEqual)
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

static CcuResult DoAllGatherWrite(
    KfcAllGatherMesh1DMem2MemContext& ctx, const ccu::LocalAddr& src, const std::vector<ccu::RemoteAddr>& dst,
    const ccu::Variable& sliceSize, uint32_t unrollIdx)
{
    uint32_t channelId = 0;
    const uint32_t numEventsPerIter = (ctx.rankSize + AG_M2M_BIT_NUM_PER_CKE - 1) / AG_M2M_BIT_NUM_PER_CKE;

    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        const uint32_t eventIdx = unrollIdx * numEventsPerIter + rankIdx / AG_M2M_BIT_NUM_PER_CKE;
        const uint16_t rankMask = 1U << (rankIdx % AG_M2M_BIT_NUM_PER_CKE);
        if (rankIdx == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
        } else {
            CCU_CHK_RET(
                ccu::Write(ctx.channels[channelId], dst[rankIdx], src, sliceSize, ctx.events[eventIdx], rankMask));
            channelId++;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoAllGatherWait(KfcAllGatherMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    const uint32_t numEventsPerIter = (ctx.rankSize + AG_M2M_BIT_NUM_PER_CKE - 1) / AG_M2M_BIT_NUM_PER_CKE;

    for (uint32_t i = 0; i < numEventsPerIter; i++) {
        const uint32_t eventIdx = unrollIdx * numEventsPerIter + i;
        uint16_t eventMask;
        if (i == numEventsPerIter - 1) {
            if (ctx.rankSize % AG_M2M_BIT_NUM_PER_CKE == 0) {
                eventMask = (1U << AG_M2M_BIT_NUM_PER_CKE) - 1;
            } else {
                eventMask = (1U << (ctx.rankSize % AG_M2M_BIT_NUM_PER_CKE)) - 1;
            }
        } else {
            eventMask = (1U << AG_M2M_BIT_NUM_PER_CKE) - 1;
        }
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

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

static CcuResult DoRepeatAllGather(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    CCU_CHK_RET(InitAllGatherAddr(ctx));
    ctx.waitRepeatNum = ctx.tmpRepeatNum;
    ctx.groupCopyRepeatNum = ctx.tmpRepeatNum;

    // Phase 1: 展开下发 Write，使用不同 event，避免逐轮等待。
    CCU_IF(ctx.tmpRepeatNum != UINT64_MAX)
    {
        ctx.tmpRepeatNum += ctx.constVar1;
        CCU_CHK_RET(DoAllGatherWrite(ctx, ctx.src, ctx.dst, ctx.normalSliceSize, 0));
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
            CCU_CHK_RET(DoAllGatherWrite(ctx, ctx.src, ctx.dst, ctx.normalSliceSize, i));
        }
    }

    // Phase 2: 本卡拷贝独立执行，覆盖全部 repeat。
    CCU_CHK_RET(DoAllGatherGroupCopy(ctx));

    // Phase 3: 等待已展开轮次的所有 Write 完成。
    for (uint32_t i = 0; i < AG_M2M_UNROLL_NUM; i++) {
        CCU_IF(ctx.waitRepeatNum != UINT64_MAX)
        {
            ctx.waitRepeatNum += ctx.constVar1;
            CCU_CHK_RET(DoAllGatherWait(ctx, i));
        }
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllGatherMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable outputOffset,
    ccu::Variable sliceSize, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable tmpRepeatNum, ccu::Variable inputRepeatStride,
    ccu::Variable outputRepeatStride, ccu::Variable lastSliceSize, ccu::Variable isInputOutputEqual,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId)
{
    KfcAllGatherMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, outputOffset, sliceSize, goSize0, goSize1, goSize2, goSize3,
        currentRankSliceInputOffset, tmpRepeatNum, inputRepeatStride, outputRepeatStride, lastSliceSize,
        isInputOutputEqual));
    HCCL_INFO("[KFCAG_DEBUG][Kernel] rankId[%u] rankSize[%u] channelCount[%u]", rankId, rankSize, channelCount);
    CCU_CHK_RET(PreSync(ctx));

    ctx.sliceSize = (ctx.rankId == (ctx.rankSize - 1)) ? ctx.lastSliceSize : ctx.normalSliceSize;
    // sliceSize == 0 时不需要执行 AllGather，只需前后同步
    CCU_IF(ctx.sliceSize != 0) { CCU_CHK_RET(DoRepeatAllGather(ctx)); }

    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
