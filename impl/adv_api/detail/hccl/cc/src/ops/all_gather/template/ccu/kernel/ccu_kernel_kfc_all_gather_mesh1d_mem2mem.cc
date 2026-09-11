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
#include "kfc_callable_task_registry.h"

namespace mc2_ops_hccl {

constexpr int AG_M2M_OUTPUT_XN_ID = 1;
constexpr int AG_M2M_TOKEN_XN_ID = 2;
constexpr int AG_M2M_CKE_IDX = 0;
constexpr int AG_M2M_POST_SYNC_ID = 3;
constexpr uint32_t AG_M2M_BITS_PER_EVENT = 16;
constexpr uint16_t AG_M2M_FULL_EVENT_MASK = static_cast<uint16_t>(0xFFFFU);

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
    ctx.constVar1 = 1;
    ctx.repeatTimeFlag = 0;
    ctx.remoteRepeatTimeFlag = 0;
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    uint32_t argId = 0;
    CCU_CHK_RET(ccu::LoadArg(ctx.input, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.output[ctx.rankId], argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.token[ctx.rankId], argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.currentRankSliceOutputOffset, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.normalSliceSize, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.addrOffset, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.loopParam, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.parallelParam, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.goSize.residual, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.currentRankSliceInputOffset, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.tmpRepeatNum, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.inputRepeatStride, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.outputRepeatStride, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.lastSliceSize, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.isInputOutputEqual, argId++));
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

static CcuResult DoAllGatherLocalCopy(KfcAllGatherMesh1DMem2MemContext& ctx)
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
                // The KFC subkernel is emitted inside the server's persistent
                // CCU_WHILE. GroupCopy creates a two-loop LoopGroup there,
                // which the KFC server runtime rejects when adding the second
                // loop.
                // Use the direct mem-to-mem primitive already used by other
                // KFC kernels, and wait before reusing its event in the next
                // repeat.
                CCU_CHK_RET(ccu::LocalCopy(ctx.localDst, ctx.srcLocCopy, ctx.sliceSize, ctx.localCopyEvent, 1));
                CCU_CHK_RET(ccu::EventWait(ctx.localCopyEvent, 1));
                ctx.repeatTimeFlag = 1;
            }
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoRepeatAllGather(KfcAllGatherMesh1DMem2MemContext& ctx)
{
    CCU_CHK_RET(InitAllGatherAddr(ctx));
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
        CCU_CHK_RET(DoAllGatherLocalCopy(ctx));

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
    CCU_CHK_RET(LoadArgs(ctx));
    HCCL_INFO(
        "[KFC][AllGather][Kernel] rankId[%u], rankSize[%u], channelCount[%u], repeatBatch[%u], "
        "localCopy[direct]",
        ctx.rankId, ctx.rankSize, ctx.channelCount, AG_M2M_UNROLL_NUM);

    CCU_CHK_RET(PreSync(ctx));
    ctx.sliceSize = (ctx.rankId == ctx.rankSize - 1) ? ctx.lastSliceSize : ctx.normalSliceSize;
    CCU_IF(ctx.sliceSize != 0) { CCU_CHK_RET(DoRepeatAllGather(ctx)); }
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

REGISTER_KFC_CALLABLE_TASK(
    HcclCMDType::HCCL_CMD_ALLGATHER, KfcServerRole::DEFAULT, CcuKernelAllGatherMesh1DMem2Mem,
    CcuKfcAllGatherMesh1DMem2MemKernel);
REGISTER_KFC_CALLABLE_TASK(
    HcclCMDType::HCCL_CMD_ALLGATHER, KfcServerRole::ALL_GATHER_MESH, CcuKernelAllGatherMesh1DMem2Mem,
    CcuKfcAllGatherMesh1DMem2MemKernel);

} // namespace mc2_ops_hccl
