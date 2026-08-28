/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty_mem2mem.h"

namespace mc2_ops_hccl {

constexpr int A2A_M2M_INPUT_XN_ID = 0;
constexpr int A2A_M2M_OUTPUT_XN_ID = 1;
constexpr int A2A_M2M_TOKEN_XN_ID = 2;
constexpr int A2A_M2M_POST_SYNC_ID = 3;
constexpr int A2A_M2M_CKE_IDX_0 = 0;
constexpr int A2A_M2M_CKE_IDX_1 = 1;
constexpr uint16_t A2A_M2M_OUTPUT_BIT = 1U << A2A_M2M_OUTPUT_XN_ID;
constexpr uint16_t A2A_M2M_TOKEN_BIT = 1U << A2A_M2M_TOKEN_XN_ID;
constexpr uint16_t A2A_M2M_PRE_SYNC_BIT = A2A_M2M_OUTPUT_BIT | A2A_M2M_TOKEN_BIT;
constexpr uint16_t A2A_M2M_POST_SYNC_BIT = 1U << A2A_M2M_POST_SYNC_ID;

struct KfcAllToAllMesh1DMultiJettyMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable input;
    std::vector<ccu::Variable> peerOutput;
    std::vector<ccu::Variable> peerToken;
    ccu::Variable sliceSize;
    ccu::Variable srcStride;
    ccu::Variable srcOffset;
    ccu::Variable dstOffset;
    GroupOpSizeVars goSize;
    std::vector<ccu::Event> eventList;
};

static CcuResult InitResource(KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx)
{
    if (ctx.rankSize == 0 || ctx.rankId >= ctx.rankSize || ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcAllToAllMesh1DMultiJettyMem2Mem] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.peerOutput.resize(ctx.rankSize);
    ctx.peerToken.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.peerOutput[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A2A_M2M_OUTPUT_XN_ID);
        ctx.peerToken[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A2A_M2M_TOKEN_XN_ID);
        channelIdx++;
    }

    ctx.eventList.resize(ctx.rankSize);
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable sliceSize, ccu::Variable srcStride, ccu::Variable srcOffset,
    ccu::Variable dstOffset, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3)
{
    ctx.input = inputAddr;
    ctx.peerOutput[ctx.rankId] = outputAddr;
    ctx.peerToken[ctx.rankId] = tokenInfo;
    ctx.sliceSize = sliceSize;
    ctx.srcStride = srcStride;
    ctx.srcOffset = srcOffset;
    ctx.dstOffset = dstOffset;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.peerOutput[ctx.rankId], A2A_M2M_OUTPUT_XN_ID, A2A_M2M_CKE_IDX_0, A2A_M2M_OUTPUT_BIT));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.peerToken[ctx.rankId], A2A_M2M_TOKEN_XN_ID, A2A_M2M_CKE_IDX_0, A2A_M2M_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2A_M2M_CKE_IDX_0, A2A_M2M_PRE_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

static void CalcAddresses(
    KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx, std::vector<ccu::LocalAddr>& remoteSrc,
    std::vector<ccu::RemoteAddr>& remoteDst, ccu::LocalAddr& localSrc, ccu::LocalAddr& localDst)
{
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx == ctx.rankId) {
            localSrc.addr = ctx.srcOffset;
            for (uint32_t i = 0; i < rankIdx; i++) {
                localSrc.addr += ctx.srcStride;
            }
            localSrc.token = ctx.peerToken[rankIdx];
            localDst.addr = ctx.peerOutput[rankIdx];
            localDst.token = ctx.peerToken[rankIdx];
            localDst.addr += ctx.dstOffset;
        } else {
            remoteSrc[rankIdx].addr = ctx.srcOffset;
            for (uint32_t i = 0; i < rankIdx; i++) {
                remoteSrc[rankIdx].addr += ctx.srcStride;
            }
            remoteSrc[rankIdx].token = ctx.peerToken[rankIdx];
            remoteDst[rankIdx].addr = ctx.peerOutput[rankIdx];
            remoteDst[rankIdx].token = ctx.peerToken[rankIdx];
            remoteDst[rankIdx].addr += ctx.dstOffset;
        }
    }
}

static CcuResult RemoteWrite(
    KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx, std::vector<ccu::LocalAddr>& remoteSrc,
    std::vector<ccu::RemoteAddr>& remoteDst)
{
    uint32_t channelId = 0;
    for (uint32_t r = 0; r < ctx.rankSize; r++) {
        if (r == ctx.rankId) {
            continue;
        }
        CCU_IF(ctx.sliceSize != 0)
        {
            CCU_CHK_RET(ccu::Write(
                ctx.channels[channelId], remoteDst[r], remoteSrc[r], ctx.sliceSize, ctx.eventList[r], 1));
        }
        CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.eventList[r], 1)); }
        channelId++;
    }
    return CCU_SUCCESS;
}

static CcuResult WaitEvents(KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx)
{
    for (uint32_t r = 0; r < ctx.rankSize; r++) {
        CCU_CHK_RET(ccu::EventWait(ctx.eventList[r], 1));
    }
    return CCU_SUCCESS;
}

static CcuResult DoAllToAll(KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx)
{
    ctx.srcOffset += ctx.input;
    std::vector<ccu::LocalAddr> remoteSrc(ctx.rankSize);
    std::vector<ccu::RemoteAddr> remoteDst(ctx.rankSize);
    ccu::LocalAddr localSrc;
    ccu::LocalAddr localDst;

    CalcAddresses(ctx, remoteSrc, remoteDst, localSrc, localDst);
    CCU_CHK_RET(RemoteWrite(ctx, remoteSrc, remoteDst));
    CCU_CHK_RET(GroupCopy(ctx, localDst, localSrc, ctx.goSize));
    CCU_CHK_RET(ccu::EventRecord(ctx.eventList[ctx.rankId], 1));
    CCU_CHK_RET(WaitEvents(ctx));
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcAllToAllMesh1DMultiJettyMem2MemContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], A2A_M2M_CKE_IDX_1, A2A_M2M_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2A_M2M_CKE_IDX_1, A2A_M2M_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllToAllMesh1DMultiJettyMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable sliceSize, ccu::Variable srcStride, ccu::Variable srcOffset,
    ccu::Variable dstOffset, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId)
{
    KfcAllToAllMesh1DMultiJettyMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, sliceSize, srcStride, srcOffset, dstOffset, goSize0, goSize1, goSize2,
        goSize3));
    CCU_CHK_RET(PreSync(ctx));
    CCU_IF(ctx.sliceSize != 0) { CCU_CHK_RET(DoAllToAll(ctx)); }
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
