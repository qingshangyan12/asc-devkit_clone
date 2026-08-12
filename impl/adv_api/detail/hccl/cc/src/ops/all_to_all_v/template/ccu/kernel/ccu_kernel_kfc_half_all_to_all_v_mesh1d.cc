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

struct KfcHalfAllToAllVMesh1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable userInAddr;
    ccu::Variable sendSizesAddr;
    ccu::Variable sendOffsetsAddr;
    ccu::Variable recvOffset;
    GroupOpSizeVars goSize;

    std::vector<ccu::Variable> cclBuffer;
    std::vector<ccu::Variable> token;
    std::vector<ccu::Event> writeDoneEvents;

    std::vector<ccu::Variable> sendSize;
    std::vector<ccu::Variable> sendOffset;
};

static CcuResult InitResource(KfcHalfAllToAllVMesh1DContext& ctx)
{
    if (ctx.rankSize == 0 || ctx.rankId >= ctx.rankSize || ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcHalfAll2AllVMesh1D] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
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

    const uint32_t eventNum = (ctx.rankSize + A2AV_BITS_PER_EVENT - 1) / A2AV_BITS_PER_EVENT;
    ctx.writeDoneEvents.resize(eventNum);

    ctx.sendSize.resize(ctx.rankSize);
    ctx.sendOffset.resize(ctx.rankSize);

    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcHalfAllToAllVMesh1DContext& ctx, ccu::Variable userInAddr, ccu::Variable cclBufferAddr,
    ccu::Variable sendSizesAddr, ccu::Variable sendOffsetsAddr, ccu::Variable recvOffset,
    ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    ccu::Variable tokenInfo)
{
    ctx.userInAddr = userInAddr;
    ctx.cclBuffer[ctx.rankId] = cclBufferAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.sendSizesAddr = sendSizesAddr;
    ctx.sendOffsetsAddr = sendOffsetsAddr;
    ctx.recvOffset = recvOffset;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;
    return CCU_SUCCESS;
}

static CcuResult LoadSendParamsFromMem(KfcHalfAllToAllVMesh1DContext& ctx)
{
    ccu::Variable elemSize;
    elemSize = 8U;

    ccu::Variable curSizesAddr = ctx.sendSizesAddr;
    ccu::Variable curOffsetsAddr = ctx.sendOffsetsAddr;

    for (uint32_t i = 0; i < ctx.rankSize; i++) {
        CCU_CHK_RET(ccu::Load(curSizesAddr, ctx.sendSize[i]));
        CCU_CHK_RET(ccu::Load(curOffsetsAddr, ctx.sendOffset[i]));
        curSizesAddr += elemSize;
        curOffsetsAddr += elemSize;
    }
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcHalfAllToAllVMesh1DContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.cclBuffer[ctx.rankId], A2AV_OUTPUT_XN_ID, A2AV_OUTPUT_CKE_IDX, A2AV_OUTPUT_BIT));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], A2AV_TOKEN_XN_ID, A2AV_TOKEN_CKE_IDX, A2AV_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2AV_OUTPUT_CKE_IDX, A2AV_OUTPUT_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2AV_TOKEN_CKE_IDX, A2AV_TOKEN_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAllToAllVWrite(KfcHalfAllToAllVMesh1DContext& ctx)
{
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        const uint32_t eventIdx = peerId / A2AV_BITS_PER_EVENT;
        const uint16_t peerMask = 1U << (peerId % A2AV_BITS_PER_EVENT);

        ccu::LocalAddr src;
        src.addr = ctx.userInAddr;
        src.addr += ctx.sendOffset[peerId];
        src.token = ctx.token[ctx.rankId];

        if (peerId == ctx.rankId) {
            ccu::LocalAddr localDst;
            localDst.addr = ctx.cclBuffer[ctx.rankId];
            localDst.addr += ctx.recvOffset;
            localDst.token = ctx.token[ctx.rankId];
            CCU_CHK_RET(GroupCopy(ctx, localDst, src, ctx.goSize));
            CCU_CHK_RET(ccu::EventRecord(ctx.writeDoneEvents[eventIdx], peerMask));
            continue;
        }

        ccu::RemoteAddr dst;
        dst.addr = ctx.cclBuffer[peerId];
        dst.addr += ctx.recvOffset;
        dst.token = ctx.token[peerId];

        CCU_IF(ctx.sendSize[peerId] != 0)
        {
            CCU_CHK_RET(ccu::Write(
                ctx.channels[channelIdx], dst, src, ctx.sendSize[peerId], ctx.writeDoneEvents[eventIdx], peerMask));
        }
        CCU_IF(ctx.sendSize[peerId] == 0)
        {
            CCU_CHK_RET(ccu::EventRecord(ctx.writeDoneEvents[eventIdx], peerMask));
        }
        channelIdx++;
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAllToAllVWait(KfcHalfAllToAllVMesh1DContext& ctx)
{
    const uint32_t eventNum = ctx.writeDoneEvents.size();
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        const uint32_t remainingRanks = ctx.rankSize - eventIdx * A2AV_BITS_PER_EVENT;
        const uint32_t bitCount = std::min(remainingRanks, A2AV_BITS_PER_EVENT);
        const uint16_t eventMask = bitCount == A2AV_BITS_PER_EVENT ? static_cast<uint16_t>(UINT16_MAX) :
                                                                     static_cast<uint16_t>((1U << bitCount) - 1);
        CCU_CHK_RET(ccu::EventWait(ctx.writeDoneEvents[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcHalfAllToAllVMesh1DContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], A2AV_POST_SYNC_CKE_IDX, A2AV_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2AV_POST_SYNC_CKE_IDX, A2AV_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcHalfAll2AllVMesh1DKernel(
    ccu::Variable userInAddr, ccu::Variable cclBufferAddr, ccu::Variable sendSizesAddr,
    ccu::Variable sendOffsetsAddr, ccu::Variable recvOffset,
    ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    ccu::Variable tokenInfo)
{
    KfcHalfAllToAllVMesh1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, userInAddr, cclBufferAddr, sendSizesAddr, sendOffsetsAddr, recvOffset,
        goSize0, goSize1, goSize2, goSize3, tokenInfo));
    CCU_CHK_RET(LoadSendParamsFromMem(ctx));
    CCU_CHK_RET(PreSync(ctx));

    CCU_IF(goSize2 != 0)
    {
        CCU_CHK_RET(DoHalfAllToAllVWrite(ctx));
        CCU_CHK_RET(DoHalfAllToAllVWait(ctx));
    }

    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
