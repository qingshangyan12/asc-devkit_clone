/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_half_all_to_all_v_mesh1d.h"

namespace mc2_ops_hccl {

constexpr int A2AV_OUTPUT_XN_ID = 1;
constexpr int A2AV_TOKEN_XN_ID = 2;
constexpr int A2AV_POST_SYNC_CKE_IDX = 0;
constexpr int A2AV_OUTPUT_CKE_IDX = 1;
constexpr int A2AV_TOKEN_CKE_IDX = 2;
constexpr uint16_t A2AV_POST_SYNC_BIT = 1U;
constexpr uint16_t A2AV_OUTPUT_BIT = 1U << A2AV_OUTPUT_XN_ID;
constexpr uint16_t A2AV_TOKEN_BIT = 1U << A2AV_TOKEN_XN_ID;
constexpr uint32_t A2AV_BITS_PER_EVENT = 16;
// Source-side ordering caps the supported rank count; kept aligned with the selector guard.
constexpr uint32_t A2AV_MAX_SUPPORTED_RANK_SIZE = 8;
// HBM layout written by the AI core side: sendSizes[rankSize] followed by sendOffsets[rankSize].
constexpr uint32_t A2AV_HALF_NUM = 2;

struct KfcHalfAllToAllVMesh1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable input;
    // cclBuffer[rankId] is the local CCL buffer; the other entries are peer buffers fetched over channels.
    std::vector<ccu::Variable> cclBuffer;
    std::vector<ccu::Variable> token;
    ccu::Variable sendSizesAddr;
    ccu::Variable sendOffsetsAddr;
    ccu::Variable recvOffset;
    GroupOpSizeVars goSize;

    // Per-peer variable-length slice descriptors loaded from HBM.
    std::unique_ptr<ccu::Array<ccu::Variable>> sendSizes;
    std::unique_ptr<ccu::Array<ccu::Variable>> sendOffsets;
    std::vector<ccu::Event> events;
};

static CcuResult InitResource(KfcHalfAllToAllVMesh1DContext& ctx)
{
    if (ctx.rankSize == 0 || ctx.rankSize > A2AV_MAX_SUPPORTED_RANK_SIZE || ctx.rankId >= ctx.rankSize ||
        ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcHalfAllToAllVMesh1D] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.cclBuffer.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.cclBuffer[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A2AV_OUTPUT_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A2AV_TOKEN_XN_ID);
        channelIdx++;
    }

    // Contiguous XN blocks so the descriptors can be bulk-loaded with a single ccu::Load each.
    ctx.sendSizes = std::make_unique<ccu::Array<ccu::Variable>>(ctx.rankSize);
    ctx.sendOffsets = std::make_unique<ccu::Array<ccu::Variable>>(ctx.rankSize);

    const uint32_t eventNum = (ctx.rankSize + A2AV_BITS_PER_EVENT - 1) / A2AV_BITS_PER_EVENT;
    ctx.events.resize(eventNum);
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcHalfAllToAllVMesh1DContext& ctx, ccu::Variable inputAddr, ccu::Variable cclBufferAddr, ccu::Variable tokenInfo,
    ccu::Variable sendSizesAddr, ccu::Variable sendOffsetsAddr, ccu::Variable recvOffset, ccu::Variable goSize0,
    ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3)
{
    ctx.input = inputAddr;
    ctx.cclBuffer[ctx.rankId] = cclBufferAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.sendSizesAddr = sendSizesAddr;
    ctx.sendOffsetsAddr = sendOffsetsAddr;
    ctx.recvOffset = recvOffset;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;

    // The variable-length descriptors live in HBM; pull all rankSize entries in one shot per array.
    CCU_CHK_RET(ccu::Load(ctx.sendSizesAddr, *ctx.sendSizes, ctx.rankSize));
    CCU_CHK_RET(ccu::Load(ctx.sendOffsetsAddr, *ctx.sendOffsets, ctx.rankSize));
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcHalfAllToAllVMesh1DContext& ctx)
{
    // Publish the local CCL buffer base (already shifted by recvOffset) and its token to every peer.
    ccu::Variable localDst;
    localDst = ctx.cclBuffer[ctx.rankId];
    localDst += ctx.recvOffset;

    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], localDst, A2AV_OUTPUT_XN_ID, A2AV_OUTPUT_CKE_IDX, A2AV_OUTPUT_BIT));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], A2AV_TOKEN_XN_ID, A2AV_TOKEN_CKE_IDX, A2AV_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2AV_OUTPUT_CKE_IDX, A2AV_OUTPUT_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2AV_TOKEN_CKE_IDX, A2AV_TOKEN_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcHalfAllToAllVMesh1DContext& ctx)
{
    // Source-side ordering: every rank announces completion and waits for all peers.
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], A2AV_POST_SYNC_CKE_IDX, A2AV_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2AV_POST_SYNC_CKE_IDX, A2AV_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAllToAllWait(KfcHalfAllToAllVMesh1DContext& ctx)
{
    const uint32_t eventNum = ctx.events.size();
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        const uint32_t remainingRanks = ctx.rankSize - eventIdx * A2AV_BITS_PER_EVENT;
        const uint32_t bitCount = std::min(remainingRanks, A2AV_BITS_PER_EVENT);
        const uint16_t eventMask = bitCount == A2AV_BITS_PER_EVENT ? static_cast<uint16_t>(UINT16_MAX) :
                                                                    static_cast<uint16_t>((1U << bitCount) - 1);
        CCU_CHK_RET(ccu::EventWait(ctx.events[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAllToAll(KfcHalfAllToAllVMesh1DContext& ctx)
{
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        const uint32_t eventIdx = peerId / A2AV_BITS_PER_EVENT;
        const uint16_t rankMask = 1U << (peerId % A2AV_BITS_PER_EVENT);

        // src = userIn + sendOffsets[peerId]; every peer reads a different slice of the local input.
        ccu::LocalAddr src;
        src.addr = ctx.input;
        src.addr += (*ctx.sendOffsets)[peerId];
        src.token = ctx.token[ctx.rankId];

        if (peerId == ctx.rankId) {
            // Local slice never crosses a channel: GroupCopy handles the chunked move.
            ccu::LocalAddr localDst;
            localDst.addr = ctx.cclBuffer[ctx.rankId];
            localDst.addr += ctx.recvOffset;
            localDst.token = ctx.token[ctx.rankId];
            CCU_CHK_RET(GroupCopy(ctx, localDst, src, ctx.goSize));
            CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask));
            continue;
        }

        ccu::RemoteAddr dst;
        dst.addr = ctx.cclBuffer[peerId];
        dst.token = ctx.token[peerId];

        // Zero-length slices are legal in AllToAllV; skip the transfer but still satisfy the event.
        CCU_IF((*ctx.sendSizes)[peerId] != 0)
        {
            CCU_CHK_RET(ccu::Write(
                ctx.channels[channelIdx], dst, src, (*ctx.sendSizes)[peerId], ctx.events[eventIdx], rankMask));
        }
        CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.events[eventIdx], rankMask)); }
        channelIdx++;
    }

    CCU_CHK_RET(DoHalfAllToAllWait(ctx));
    return CCU_SUCCESS;
}

CcuResult CcuKfcHalfAllToAllVMesh1DKernel(
    ccu::Variable inputAddr, ccu::Variable cclBufferAddr, ccu::Variable tokenInfo, ccu::Variable sendSizesAddr,
    ccu::Variable sendOffsetsAddr, ccu::Variable recvOffset, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, const ChannelHandle channels[], uint32_t channelCount,
    uint32_t rankSize, uint32_t rankId)
{
    KfcHalfAllToAllVMesh1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, cclBufferAddr, tokenInfo, sendSizesAddr, sendOffsetsAddr, recvOffset, goSize0, goSize1,
        goSize2, goSize3));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoHalfAllToAll(ctx));
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
