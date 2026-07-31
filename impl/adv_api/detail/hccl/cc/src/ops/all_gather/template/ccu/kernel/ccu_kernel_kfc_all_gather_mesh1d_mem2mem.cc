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
constexpr uint32_t AG_M2M_UNROLL_NUM = 16;

struct KfcAllGatherMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable outputOffset;
    ccu::Variable sliceSize;
    GroupOpSizeVars goSize;
    std::vector<ccu::Event> events;

    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable tmpRepeatNum;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable isInputOutputEqual;

    ccu::LocalAddr src_loccopy;
    ccu::LocalAddr localDst;
    std::vector<ccu::RemoteAddr> dst;
    ccu::LocalAddr src;

    ccu::Variable constVar1;
    ccu::Variable repeatTimeflag;
    ccu::Variable waitRepeatNum;
    ccu::Variable groupCopyRepeatNum;
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

    const uint32_t eventNum = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    ctx.events.resize(AG_M2M_UNROLL_NUM * eventNum);
    ctx.constVar1 = 1;
    ctx.repeatTimeflag = 0;
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}



static CcuResult LoadArgs(
    KfcAllGatherMesh1DMem2MemContext& ctx, 
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable tmpRepeatNum, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable isInputOutputEqual,
    ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3)
{
    ctx.input = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = currentRankSliceOutputOffset;
    ctx.tmpRepeatNum = tmpRepeatNum;
    ctx.inputRepeatStride = inputRepeatStride;
    ctx.outputRepeatStride = outputRepeatStride;
    ctx.normalSliceSize = normalSliceSize;
    ctx.lastSliceSize = lastSliceSize;
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
            ctx.channels[i], ctx.output[ctx.rankId], AG_M2M_OUTPUT_XN_ID,
            AG_M2M_OUTPUT_CKE_IDX, AG_M2M_OUTPUT_BIT));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], AG_M2M_TOKEN_XN_ID,
            AG_M2M_TOKEN_CKE_IDX, AG_M2M_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_OUTPUT_CKE_IDX, AG_M2M_OUTPUT_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_TOKEN_CKE_IDX, AG_M2M_TOKEN_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult InitAllGatherAddr(KfcAllGatherMesh1DMem2MemContext &ctx)
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

static CcuResult DoAllGatherWrite(
    KfcAllGatherMesh1DMem2MemContext& ctx, const ccu::LocalAddr& src,
    const std::vector<ccu::RemoteAddr>& dst, const ccu::Variable &sliceSize, uint32_t unrollIdx)
{
    uint32_t channelId = 0;
    uint32_t numEventsPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;

    for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        uint32_t eventIdx = unrollIdx * numEventsPerIter + rankIdx / AG_M2M_BITS_PER_EVENT;
        uint16_t rankMask = 1 << (rankIdx % AG_M2M_BITS_PER_EVENT);
        if (rankIdx == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
        } else {
            CCU_CHK_RET(ccu::Write(ctx.channels[channelId], dst[rankIdx],
                src, sliceSize, ctx.events[eventIdx], rankMask));
            channelId++;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoAllGatherWait(KfcAllGatherMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    uint32_t numEventsPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;

    for (uint32_t i = 0; i < numEventsPerIter; i++) {
        uint32_t eventIdx = unrollIdx * numEventsPerIter + i;
        uint16_t eventMask;
        if (i == numEventsPerIter - 1) {
            if (ctx.rankSize % AG_M2M_BITS_PER_EVENT == 0) {
                eventMask = (1 << AG_M2M_BITS_PER_EVENT) - 1;
            } else {
                eventMask = (1 << (ctx.rankSize % numEventsPerIter)) - 1;
            }
        } else {
            eventMask = (1 << AG_M2M_BITS_PER_EVENT) - 1;
        }
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

// static CcuResult DoAllGather(KfcAllGatherMesh1DMem2MemContext& ctx)
// {
//     ccu::LocalAddr src;
//     src.addr = ctx.input;
//     src.token = ctx.token[ctx.rankId];

//     ccu::LocalAddr localDst;
//     localDst.addr = ctx.output[ctx.rankId];
//     localDst.addr += ctx.outputOffset;
//     localDst.token = ctx.token[ctx.rankId];

//     std::vector<ccu::RemoteAddr> dst(ctx.rankSize);
//     for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
//         if (rankIdx == ctx.rankId) {
//             continue;
//         }
//         dst[rankIdx].addr = ctx.output[rankIdx];
//         dst[rankIdx].addr += ctx.outputOffset;
//         dst[rankIdx].token = ctx.token[rankIdx];
//     }

//     CCU_CHK_RET(DoAllGatherWrite(ctx, src, dst));
//     CCU_CHK_RET(GroupCopy(ctx, localDst, src, ctx.goSize));
//     CCU_CHK_RET(DoAllGatherWait(ctx));
//     return CCU_SUCCESS;
// }

static CcuResult DoRepeatAllGather(KfcAllGatherMesh1DMem2MemContext& ctx)
{

    CCU_CHK_RET(InitAllGatherAddr(ctx));
    ctx.waitRepeatNum = ctx.tmpRepeatNum;
    ctx.groupCopyRepeatNum = ctx.tmpRepeatNum;

    CCU_IF(ctx.tmpRepeatNum != UINT64_MAX)
    {
        ctx.tmpRepeatNum += ctx.constVar1;
        CCU_CHK_RET(DoAllGatherWrite(ctx, ctx.src, ctx.dst, ctx.normalSliceSize, 0));
    }

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
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable tmpRepeatNum, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable isInputOutputEqual,
    ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId)
{
    KfcAllGatherMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, 
        currentRankSliceInputOffset, currentRankSliceOutputOffset,
        tmpRepeatNum, inputRepeatStride, outputRepeatStride,
        normalSliceSize, lastSliceSize, isInputOutputEqual,
        goSize0, goSize1, goSize2, goSize3));
    CCU_CHK_RET(PreSync(ctx));
    ctx.sliceSize = (rankId == (rankSize - 1)) ? lastSliceSize : normalSliceSize;
    CCU_IF(ctx.sliceSize != 0)
    {
        CCU_CHK_RET(DoRepeatAllGather(ctx));
    }
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
