/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_half_alltoallv_mesh1d.h"

namespace mc2_ops_hccl {

constexpr int HA2AV_BUFFER_XN_ID = 1;
constexpr int HA2AV_TOKEN_XN_ID = 2;
constexpr int HA2AV_POST_SYNC_CKE_IDX = 0;
constexpr int HA2AV_BUFFER_CKE_IDX = 1;
constexpr int HA2AV_TOKEN_CKE_IDX = 2;
constexpr uint16_t HA2AV_POST_SYNC_BIT = 1U;
constexpr uint16_t HA2AV_BUFFER_BIT = 1U << HA2AV_BUFFER_XN_ID;
constexpr uint16_t HA2AV_TOKEN_BIT = 1U << HA2AV_TOKEN_XN_ID;
// 每个 Event 有 16 个 bit，一个 peer 占一个 bit，与 AllGather Mem2Mem 口径一致
constexpr uint32_t HA2AV_BITS_PER_EVENT = 16;

struct KfcHalfAlltoAllVMesh1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable userInAddr;
    ccu::Variable recvOffset;
    ccu::Variable missionId;  // 阶段二新增：0=搬A片, 1=搬B片
    // 建图期常量：本端 window 基址
    ccu::Variable localWinAddr;
    // peerBuffer[peerId] 为对端 cclBuffer 首地址；本 rank 槽位由 myBufferAddr 填入
    std::vector<ccu::Variable> peerBuffer;
    std::vector<ccu::Variable> token;
    // 阶段二：扩大 context，保留 A/B 两组参数（用于 Mission 1 的 chunk 偏移计算）
    std::unique_ptr<ccu::Array<ccu::Variable>> sendSizeA;
    std::unique_ptr<ccu::Array<ccu::Variable>> sendSizeB;
    std::unique_ptr<ccu::Array<ccu::Variable>> sendOffsetA;
    std::unique_ptr<ccu::Array<ccu::Variable>> sendOffsetB;
    GroupOpSizeVars goSize;
    std::vector<ccu::Event> writeDoneEvents;
};

static CcuResult InitResource(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    if (ctx.rankSize == 0 || ctx.rankId >= ctx.rankSize || ctx.channelCount + 1 != ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcHalfAlltoAllVMesh1D] invalid rank/channel, rankSize[%u], rankId[%u], channelCount[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.peerBuffer.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    // 按 rank 号升序遍历 channel，遇到本 rank 跳过，要求框架返回的 channel 同样按序排列
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.peerBuffer[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], HA2AV_BUFFER_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], HA2AV_TOKEN_XN_ID);
        channelIdx++;
    }

    // 阶段二：A/B 各分配独立数组（用于 Mission 1 计算 chunk 偏移）
    ctx.sendSizeA = std::make_unique<ccu::Array<ccu::Variable>>(ctx.rankSize);
    ctx.sendSizeB = std::make_unique<ccu::Array<ccu::Variable>>(ctx.rankSize);
    ctx.sendOffsetA = std::make_unique<ccu::Array<ccu::Variable>>(ctx.rankSize);
    ctx.sendOffsetB = std::make_unique<ccu::Array<ccu::Variable>>(ctx.rankSize);

    const uint32_t eventNum = (ctx.rankSize + HA2AV_BITS_PER_EVENT - 1) / HA2AV_BITS_PER_EVENT;
    ctx.writeDoneEvents.resize(eventNum);
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgsFromMem(
    KfcHalfAlltoAllVMesh1DContext& ctx, ccu::Variable missionId, ccu::Variable userInAddr,
    ccu::Variable sendSizeAddr, ccu::Variable sendOffsetAddr, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable recvOffset, ccu::Variable tokenInfo)
{
    ctx.missionId = missionId;
    ctx.userInAddr = userInAddr;
    // 本端 window 基址来自建图期的 kernelArg，非运行时 xn 参数
    ctx.peerBuffer[ctx.rankId] = ctx.localWinAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.recvOffset = recvOffset;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;

    // 阶段二：始终读取 A/B 两段（Mission 1 需要 sendSizeA 计算 chunk 偏移）
    // A 片从 sendSizeAddr+0 开始，B 片从 sendSizeAddr+(rankSize*8) 开始
    CCU_CHK_RET(ccu::Load(sendSizeAddr, *ctx.sendSizeA, ctx.rankSize));
    ccu::Variable segmentSize = ctx.rankSize * sizeof(uint64_t);
    ccu::Variable sendSizeBAddr = sendSizeAddr + segmentSize;
    CCU_CHK_RET(ccu::Load(sendSizeBAddr, *ctx.sendSizeB, ctx.rankSize));

    CCU_CHK_RET(ccu::Load(sendOffsetAddr, *ctx.sendOffsetA, ctx.rankSize));
    ccu::Variable sendOffsetBAddr = sendOffsetAddr + segmentSize;
    CCU_CHK_RET(ccu::Load(sendOffsetBAddr, *ctx.sendOffsetB, ctx.rankSize));

    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    // 把本端 cclBuffer 地址与 token 广播给所有对端
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.peerBuffer[ctx.rankId], HA2AV_BUFFER_XN_ID, HA2AV_BUFFER_CKE_IDX, HA2AV_BUFFER_BIT));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], HA2AV_TOKEN_XN_ID, HA2AV_TOKEN_CKE_IDX, HA2AV_TOKEN_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_BUFFER_CKE_IDX, HA2AV_BUFFER_BIT));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_TOKEN_CKE_IDX, HA2AV_TOKEN_BIT));
    }
    return CCU_SUCCESS;
}

// 源端保序方案，当前最大支持 8P 通信域；通道保序方案明确后可放开
static CcuResult PostSync(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], HA2AV_POST_SYNC_CKE_IDX, HA2AV_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_POST_SYNC_CKE_IDX, HA2AV_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAlltoAllVWrite(
    KfcHalfAlltoAllVMesh1DContext& ctx, ccu::LocalAddr& localSrc, ccu::LocalAddr& localDst)
{
    ccu::Variable curCount;
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        const uint32_t eventIdx = peerId / HA2AV_BITS_PER_EVENT;
        const uint16_t peerBit = 1U << (peerId % HA2AV_BITS_PER_EVENT);

        // 阶段二：根据 missionId 选择 A 或 B 片参数
        ccu::Variable curOffset;
        CCU_IF(ctx.missionId == 0)
        {
            curCount = (*ctx.sendSizeA)[peerId];
            curOffset = (*ctx.sendOffsetA)[peerId];
        }
        CCU_IF(ctx.missionId == 1)
        {
            curCount = (*ctx.sendSizeB)[peerId];
            curOffset = (*ctx.sendOffsetB)[peerId];
        }

        ccu::LocalAddr src;
        src.addr = ctx.userInAddr;
        src.addr += curOffset;
        src.token = ctx.token[ctx.rankId];

        if (peerId == ctx.rankId) {
            // 本地拷贝只在 Mission 0 执行（避免重复写）
            CCU_IF(ctx.missionId == 0)
            {
                localSrc.addr = src.addr;
                localSrc.token = src.token;
                localDst.addr = ctx.peerBuffer[ctx.rankId];
                localDst.addr += ctx.recvOffset;
                localDst.token = ctx.token[ctx.rankId];
            }
            CCU_CHK_RET(ccu::EventRecord(ctx.writeDoneEvents[eventIdx], peerBit));
            continue;
        }

        ccu::RemoteAddr dst;
        dst.addr = ctx.peerBuffer[peerId];
        dst.addr += ctx.recvOffset;

        // Mission 1 需要加 chunk 偏移（跳过 A 片已写入的部分）
        CCU_IF(ctx.missionId == 1)
        {
            dst.addr += (*ctx.sendSizeA)[peerId];
        }

        dst.token = ctx.token[peerId];

        // 变长场景空分片是常态，count==0 时只置位不发数据
        CCU_IF(curCount != 0)
        {
            CCU_CHK_RET(
                ccu::Write(ctx.channels[channelIdx], dst, src, curCount, ctx.writeDoneEvents[eventIdx], peerBit));
        }
        CCU_IF(curCount == 0) { CCU_CHK_RET(ccu::EventRecord(ctx.writeDoneEvents[eventIdx], peerBit)); }
        channelIdx++;
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAlltoAllVWait(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    const uint32_t eventNum = ctx.writeDoneEvents.size();
    for (uint32_t eventIdx = 0; eventIdx < eventNum; eventIdx++) {
        const uint32_t remainingRanks = ctx.rankSize - eventIdx * HA2AV_BITS_PER_EVENT;
        const uint32_t bitCount = std::min(remainingRanks, HA2AV_BITS_PER_EVENT);
        const uint16_t eventMask = bitCount == HA2AV_BITS_PER_EVENT ? static_cast<uint16_t>(UINT16_MAX) :
                                                                      static_cast<uint16_t>((1U << bitCount) - 1);
        CCU_CHK_RET(ccu::EventWait(ctx.writeDoneEvents[eventIdx], eventMask));
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAlltoAllV(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    ccu::LocalAddr localSrc;
    ccu::LocalAddr localDst;
    CCU_CHK_RET(DoHalfAlltoAllVWrite(ctx, localSrc, localDst));
    // 阶段二：本地拷贝只在 Mission 0 执行（DoHalfAlltoAllVWrite 已根据 missionId 填充地址）
    CCU_IF(ctx.missionId == 0)
    {
        CCU_CHK_RET(GroupCopy(ctx, localDst, localSrc, ctx.goSize));
    }
    CCU_CHK_RET(DoHalfAlltoAllVWait(ctx));
    return CCU_SUCCESS;
}

CcuResult CcuKfcHalfAlltoAllVMesh1DKernel(
    ccu::Variable missionId, ccu::Variable userInAddr, ccu::Variable sendSizeAddr, ccu::Variable sendOffsetAddr,
    ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    ccu::Variable recvOffset, ccu::Variable tokenInfo, const ChannelHandle channels[], uint32_t channelCount,
    uint32_t rankSize, uint32_t rankId, uint64_t localWinAddr)
{
    KfcHalfAlltoAllVMesh1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    CCU_CHK_RET(InitResource(ctx));
    ctx.localWinAddr = localWinAddr;
    CCU_CHK_RET(LoadArgsFromMem(
        ctx, missionId, userInAddr, sendSizeAddr, sendOffsetAddr, goSize0, goSize1, goSize2, goSize3, recvOffset,
        tokenInfo));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoHalfAlltoAllV(ctx));
    // 阶段二：PostSync 只在 Mission 0 执行（避免重复同步）
    CCU_IF(missionId == 0)
    {
        CCU_CHK_RET(PostSync(ctx));
    }
    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
