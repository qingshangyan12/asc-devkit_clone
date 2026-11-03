/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_all_gather_mesh1d_mem2mem_pc.h"
#include "../ccu_temp_kfc_all_gather_nhr_1D_multi_jetty_mem2mem.h"
#include "ccu_func_dl.hpp"
#include <stdexcept>

namespace mc2_ops_hccl {
namespace pc {

constexpr int AG_M2M_OUTPUT_XN_ID = 1;
constexpr int AG_M2M_TOKEN_XN_ID = 2;
constexpr int AG_M2M_CKE_IDX = 0;
constexpr int AG_M2M_POST_SYNC_ID = 3;
constexpr uint32_t AG_M2M_BITS_PER_EVENT = 16;
constexpr uint16_t AG_M2M_FULL_EVENT_MASK = static_cast<uint16_t>(0xFFFFU);

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
    // GroupCopy uses goSize to describe its chunk loop. LocalCopy ignores
    // these four variables.
    GroupOpSizeVars goSize;
    ccu::LocalAddr srcLocCopy;
    ccu::LocalAddr localDst;
    std::vector<ccu::Event> events;
    ccu::Event localCopyEvent;

    ccu::Variable constVar1;
    ccu::Variable repeatTimeFlag;
    ccu::Variable remoteRepeatTimeFlag;
    ccu::Variable waitRepeatNum;
    ccu::Variable localCopyRepeatNum;
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
    for (uint32_t peerId = 0; peerId < ctx.rankSize; ++peerId) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AG_M2M_OUTPUT_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AG_M2M_TOKEN_XN_ID);
        ++channelIdx;
    }

    const uint32_t eventNumPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    ctx.events.resize(AG_M2M_UNROLL_NUM * eventNumPerIter);
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
    constexpr uint16_t outputBit = static_cast<uint16_t>(1U << AG_M2M_OUTPUT_XN_ID);
    constexpr uint16_t tokenBit = static_cast<uint16_t>(1U << AG_M2M_TOKEN_XN_ID);
    constexpr uint16_t allBits = static_cast<uint16_t>(outputBit | tokenBit);
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[ctx.rankId], AG_M2M_OUTPUT_XN_ID, AG_M2M_CKE_IDX, outputBit));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], AG_M2M_TOKEN_XN_ID, AG_M2M_CKE_IDX, tokenBit));
    }
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_CKE_IDX, allBits));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    constexpr uint16_t postSyncBit = static_cast<uint16_t>(1U << AG_M2M_POST_SYNC_ID);
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], AG_M2M_CKE_IDX, postSyncBit));
    }
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_M2M_CKE_IDX, postSyncBit));
    }
    return CCU_SUCCESS;
}

static CcuResult InitAllGatherAddr(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    ctx.src.addr = ctx.input;
    ctx.src.addr += ctx.currentRankSliceInputOffset;
    ctx.src.token = ctx.token[ctx.rankId];

    ctx.srcLocCopy.addr = ctx.input;
    ctx.srcLocCopy.addr += ctx.currentRankSliceInputOffset;
    ctx.srcLocCopy.token = ctx.token[ctx.rankId];

    ctx.dst.resize(ctx.rankSize);
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; ++rankIdx) {
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
    const uint32_t eventNumPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; ++rankIdx) {
        const uint32_t eventIdx = unrollIdx * eventNumPerIter + rankIdx / AG_M2M_BITS_PER_EVENT;
        const uint16_t rankMask = static_cast<uint16_t>(1U << (rankIdx % AG_M2M_BITS_PER_EVENT));
        if (rankIdx == ctx.rankId) {
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
        } else {
            CCU_CHK_RET(
                ccu::Write(ctx.channels[channelIdx], dst[rankIdx], src, sliceSize, ctx.events[eventIdx], rankMask));
            ++channelIdx;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoAllGatherWait(KfcAllGatherMesh1DMem2MemContext& ctx, uint32_t unrollIdx)
{
    const uint32_t eventNumPerIter = (ctx.rankSize + AG_M2M_BITS_PER_EVENT - 1) / AG_M2M_BITS_PER_EVENT;
    for (uint32_t eventOffset = 0; eventOffset < eventNumPerIter; ++eventOffset) {
        const uint32_t eventIdx = unrollIdx * eventNumPerIter + eventOffset;
        const uint32_t remainingRanks = ctx.rankSize - eventOffset * AG_M2M_BITS_PER_EVENT;
        const uint16_t eventMask = remainingRanks >= AG_M2M_BITS_PER_EVENT ?
                                       AG_M2M_FULL_EVENT_MASK :
                                       static_cast<uint16_t>((1U << remainingRanks) - 1U);
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

static CcuResult DoAllGatherGroupCopy(KfcAllGatherMesh1DMem2MemContext& ctx, bool useGroupCopy)
{
    CCU_IF(ctx.isInputOutputEqual == 0)
    {
        for (uint32_t unrollIdx = 0; unrollIdx < AG_M2M_UNROLL_NUM; ++unrollIdx) {
            CCU_IF(ctx.localCopyRepeatNum != UINT64_MAX)
            {
                ctx.localCopyRepeatNum += ctx.constVar1;
                CCU_IF(ctx.repeatTimeFlag != 0)
                {
                    ctx.localDst.addr += ctx.outputRepeatStride;
                    ctx.srcLocCopy.addr += ctx.inputRepeatStride;
                }
                if (useGroupCopy) {
                    HCCL_INFO("[KFC][AllGather][Kernel]Run groupCopy for allgather.");
                    CCU_CHK_RET(GroupCopy(ctx, ctx.localDst, ctx.srcLocCopy, ctx.goSize, GetCcuVersion()));
                } else {
                    CCU_CHK_RET(ccu::LocalCopy(ctx.localDst, ctx.srcLocCopy, ctx.sliceSize, ctx.localCopyEvent, 1));
                    CCU_CHK_RET(ccu::EventWait(ctx.localCopyEvent, 1));
                }
                ctx.repeatTimeFlag = 1;
            }
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoRepeatAllGather(KfcAllGatherMesh1DMem2MemContext& ctx, bool useGroupCopy)
{
    CCU_CHK_RET(InitAllGatherAddr(ctx));
    // Cross-kernel calls enter the named Func directly and skip the container's prologue.
    ctx.constVar1 = 1;
    ctx.repeatTimeFlag = 0;
    ctx.remoteRepeatTimeFlag = 0;

    // One event set contains AG_M2M_UNROLL_NUM groups.  A queue entry may
    // carry more repeats, so consume it in batches and reuse the event set
    // only after every group in the current batch has been waited on.
    CCU_WHILE(ctx.tmpRepeatNum != UINT64_MAX)
    {
        ctx.waitRepeatNum = ctx.tmpRepeatNum;
        ctx.localCopyRepeatNum = ctx.tmpRepeatNum;

        // Phase 1: issue at most one event group per unrolled repeat.
        for (uint32_t unrollIdx = 0; unrollIdx < AG_M2M_UNROLL_NUM; ++unrollIdx) {
            CCU_IF(ctx.tmpRepeatNum != UINT64_MAX)
            {
                CCU_IF(ctx.remoteRepeatTimeFlag != 0)
                {
                    ctx.src.addr += ctx.inputRepeatStride;
                    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; ++rankIdx) {
                        if (rankIdx != ctx.rankId) {
                            ctx.dst[rankIdx].addr += ctx.outputRepeatStride;
                        }
                    }
                }
                ctx.tmpRepeatNum += ctx.constVar1;
                CCU_CHK_RET(DoAllGatherWrite(ctx, ctx.src, ctx.dst, ctx.normalSliceSize, unrollIdx));
                ctx.remoteRepeatTimeFlag = 1;
            }
        }

        // Phase 2: copy this rank's slice locally while remote writes are in flight.
        CCU_CHK_RET(DoAllGatherGroupCopy(ctx, useGroupCopy));

        // Phase 3: wait before the corresponding event group is reused by
        // the next batch.
        for (uint32_t unrollIdx = 0; unrollIdx < AG_M2M_UNROLL_NUM; ++unrollIdx) {
            CCU_IF(ctx.waitRepeatNum != UINT64_MAX)
            {
                ctx.waitRepeatNum += ctx.constVar1;
                CCU_CHK_RET(DoAllGatherWait(ctx, unrollIdx));
            }
        }
    }

    return CCU_SUCCESS;
}

static void AddVariableNTimes(ccu::Variable& result, const ccu::Variable& value, uint32_t times)
{
    result = 0U;
    for (uint32_t i = 0; i < times; ++i) {
        result += value;
    }
}

CcuResult CcuKfcAllGatherMesh1DMem2MemKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgKfcAllGatherMesh1DMem2Mem*>(arg);
    KfcAllGatherMesh1DMem2MemContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = static_cast<uint32_t>(kernelArg->rankSize);
    ctx.rankId = kernelArg->rankId;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));
    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadArgs(
                ctx, args[1], args[2], args[KFC_FUNC_TOKEN_ARG_INDEX], args[4], args[5], args[6], args[7], args[8],
                args[9], args[10], args[11], args[12], args[13], args[14], args[15]);
            HCCL_INFO(
                "[KFC][AllGather][Kernel] rankId[%u], rankSize[%u], channelCount[%u], repeatBatch[%u], "
                "localCopy[groupCopy]",
                ctx.rankId, ctx.rankSize, ctx.channelCount, AG_M2M_UNROLL_NUM);
            PreSync(ctx);
            ctx.sliceSize = (ctx.rankId == ctx.rankSize - 1) ? ctx.lastSliceSize : ctx.normalSliceSize;
            CCU_IF(ctx.sliceSize != 0) { DoRepeatAllGather(ctx, true); }
            PostSync(ctx);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_ALL_GATHER_MESH1D_MEM2MEM_NAME);
    return CCU_SUCCESS;
}

} // namespace pc
} // namespace mc2_ops_hccl
