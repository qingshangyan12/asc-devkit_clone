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

#include <algorithm>

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

// 双 mission 共享同一组 channel：PreSync/PostSync 的通知位按 missionIndex 错开，
// 否则同一位被两个 mission 各 post 一次、各 wait 一次，wait 消费位后第二个 wait
// 永远等不到（死锁）。hostMissionIndex 为构图期常量，这里用编译期三元选位。
constexpr uint16_t HA2AV_POST_SYNC_BITS[KFC_MAX_MISSION_NUM] = {HA2AV_POST_SYNC_BIT, HA2AV_POST_SYNC_BIT << 1};
constexpr uint16_t HA2AV_BUFFER_BITS[KFC_MAX_MISSION_NUM] = {HA2AV_BUFFER_BIT, HA2AV_BUFFER_BIT << 1};
constexpr uint16_t HA2AV_TOKEN_BITS[KFC_MAX_MISSION_NUM] = {HA2AV_TOKEN_BIT, HA2AV_TOKEN_BIT << 1};

struct KfcHalfAlltoAllVMesh1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable userInAddr;
    ccu::Variable recvOffset;
    // 建图期常量：本端 window 基址
    ccu::Variable localWinAddr;
    // peerBuffer[peerId] 为对端 cclBuffer 首地址；本 rank 槽位由 myBufferAddr 填入
    std::vector<ccu::Variable> peerBuffer;
    std::vector<ccu::Variable> token;
    // 单 mission 模式：读取完整的 2*rankSize 数组（兼容源端双片填充）
    // 前 rankSize 个为 A 片参数，后 rankSize 个为 B 片参数
    std::unique_ptr<ccu::Array<ccu::Variable>> sendSize;
    std::unique_ptr<ccu::Array<ccu::Variable>> sendOffset;
    GroupOpSizeVars goSize;
    std::vector<ccu::Event> writeDoneEvents;

    // 阶段 3-1：双 mission 参数
    ccu::Variable missionNum;
    ccu::Variable missionIndex;
    // 构图期常量（与 kfcServerArgs 运行时值同源）：选本 mission 的通知位
    uint32_t hostMissionIndex = 0;
    // 构图期常量：mission 总数（server 侧 GetKfcServerMissionNum），收尾栅栏的门控
    uint32_t hostMissionNum = 1;
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

    // 单 mission：分配 2*rankSize 数组（兼容源端双片填充模式）
    const uint32_t sliceArgNum = ctx.rankSize * NUM_TWO;
    ctx.sendSize = std::make_unique<ccu::Array<ccu::Variable>>(sliceArgNum);
    ctx.sendOffset = std::make_unique<ccu::Array<ccu::Variable>>(sliceArgNum);

    const uint32_t eventNum = (ctx.rankSize + HA2AV_BITS_PER_EVENT - 1) / HA2AV_BITS_PER_EVENT;
    ctx.writeDoneEvents.resize(eventNum);
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

static CcuResult LoadArgsFromMem(
    KfcHalfAlltoAllVMesh1DContext& ctx, ccu::Variable userInAddr, ccu::Variable sendSizeAddr,
    ccu::Variable sendOffsetAddr, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, ccu::Variable recvOffset, ccu::Variable tokenInfo)
{
    ctx.userInAddr = userInAddr;
    // 本端 window 基址来自建图期的 kernelArg，非运行时 xn 参数
    ctx.peerBuffer[ctx.rankId] = ctx.localWinAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.recvOffset = recvOffset;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;

    // 单 mission 模式：读取完整的 2*rankSize 数组（兼容源端双片填充）
    // 前 rankSize 个为 A 片，后 rankSize 个为 B 片
    // 如果样例按单片模式填充（A 片全量，B 片为 0），也能正确处理
    const uint32_t sliceArgNum = ctx.rankSize * NUM_TWO;
    CCU_CHK_RET(ccu::Load(sendSizeAddr, *ctx.sendSize, sliceArgNum));
    CCU_CHK_RET(ccu::Load(sendOffsetAddr, *ctx.sendOffset, sliceArgNum));
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    // 把本端 cclBuffer 地址与 token 广播给所有对端。
    // 通知位按 missionIndex 错开（双 mission 共享 channel，同一位会死锁）：
    // m0 用 bit1/bit2，m1 用 bit3/bit4（构图期常量选位，非运行时分支）。
    const uint16_t bufferBit = HA2AV_BUFFER_BITS[ctx.hostMissionIndex];
    const uint16_t tokenBit = HA2AV_TOKEN_BITS[ctx.hostMissionIndex];
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.peerBuffer[ctx.rankId], HA2AV_BUFFER_XN_ID, HA2AV_BUFFER_CKE_IDX, bufferBit));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], HA2AV_TOKEN_XN_ID, HA2AV_TOKEN_CKE_IDX, tokenBit));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_BUFFER_CKE_IDX, bufferBit));
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_TOKEN_CKE_IDX, tokenBit));
    }
    return CCU_SUCCESS;
}

// 源端保序方案，当前最大支持 8P 通信域；通道保序方案明确后可放开
static CcuResult PostSync(KfcHalfAlltoAllVMesh1DContext& ctx)
{
    // 远程 Post 同步仅在 mission 0 执行（与 hcomm 原版一致：Mi1 不做远程 Post）。
    // 单 mission 时 missionIndex 必然为 0，保持现有行为。mission 0 的 post 位
    // （bit0）当前无并发使用者：mission 1 不做 Post，preSync 位已按 mission 错开。
    // 使用 CCU_IF...CCU_ELSE 结构，避免在 CCU_IF 内提前 return
    CCU_IF(ctx.missionNum == 2)
    {
        // 双 mission 模式
        CCU_IF(ctx.missionIndex == 0)
        {
            // mission 0 执行远程 Post 同步
            for (uint32_t i = 0; i < ctx.channelCount; i++) {
                CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], HA2AV_POST_SYNC_CKE_IDX, HA2AV_POST_SYNC_BIT));
            }
            for (uint32_t i = 0; i < ctx.channelCount; i++) {
                CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_POST_SYNC_CKE_IDX, HA2AV_POST_SYNC_BIT));
            }
        }
        // mission 1: 什么都不做
    }
    CCU_ELSE
    {
        // 单 mission 模式：执行远程 Post 同步
        for (uint32_t i = 0; i < ctx.channelCount; i++) {
            CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], HA2AV_POST_SYNC_CKE_IDX, HA2AV_POST_SYNC_BIT));
        }
        for (uint32_t i = 0; i < ctx.channelCount; i++) {
            CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], HA2AV_POST_SYNC_CKE_IDX, HA2AV_POST_SYNC_BIT));
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoHalfAlltoAllVWrite(
    KfcHalfAlltoAllVMesh1DContext& ctx, ccu::LocalAddr& localSrc, ccu::LocalAddr& localDst)
{
    ccu::Variable curCount;
    ccu::Variable curOffset;
    uint32_t channelIdx = 0;

    // 阶段 3-1：双 mission 支持
    // missionNum==1 时为单 mission 模式（合并 A+B，兼容现有行为）
    // missionNum==2 时为双 mission 模式：
    //   - missionIndex==0 使用 A 切片（前 rankSize 个元素）
    //   - missionIndex==1 使用 B 切片（后 rankSize 个元素），且接收地址需加 sizeA 偏移

    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        const uint32_t eventIdx = peerId / HA2AV_BITS_PER_EVENT;
        const uint16_t peerBit = 1U << (peerId % HA2AV_BITS_PER_EVENT);

        ccu::Variable sizeA = (*ctx.sendSize)[peerId];
        ccu::Variable sizeB = (*ctx.sendSize)[ctx.rankSize + peerId];
        ccu::Variable offsetA = (*ctx.sendOffset)[peerId];
        ccu::Variable offsetB = (*ctx.sendOffset)[ctx.rankSize + peerId];

        // 根据 mission 模式选择切片
        CCU_IF(ctx.missionNum == 1)
        {
            // 单 mission 模式：合并 A+B 两片的数据量（兼容现有行为）
            curCount = sizeA + sizeB;
            curOffset = offsetA;
        }
        CCU_ELSE
        {
            // 双 mission 模式：根据 missionIndex 选择 A 或 B 切片
            CCU_IF(ctx.missionIndex == 0)
            {
                // mission 0: A 切片
                curCount = sizeA;
                curOffset = offsetA;
            }
            CCU_ELSE
            {
                // mission 1: B 切片
                curCount = sizeB;
                curOffset = offsetB;
            }
        }

        // 源地址：根据选择的 offset
        ccu::LocalAddr src;
        src.addr = ctx.userInAddr;
        src.addr += curOffset;
        src.token = ctx.token[ctx.rankId];

        if (peerId == ctx.rankId) {
            // 本地拷贝：源和目标都在本地
            localSrc.addr = src.addr;
            localSrc.token = src.token;
            localDst.addr = ctx.peerBuffer[ctx.rankId];
            localDst.addr += ctx.recvOffset;

            // 阶段 3-1：mission 1 的接收地址需跳过 mission 0 的数据段
            CCU_IF(ctx.missionNum == 2)
            {
                CCU_IF(ctx.missionIndex != 0) { localDst.addr += sizeA; }
            }

            localDst.token = ctx.token[ctx.rankId];
            CCU_CHK_RET(ccu::EventRecord(ctx.writeDoneEvents[eventIdx], peerBit));
            continue;
        }

        ccu::RemoteAddr dst;
        dst.addr = ctx.peerBuffer[peerId];
        dst.addr += ctx.recvOffset;

        // 阶段 3-1：mission 1 的接收地址需跳过 mission 0 的数据段
        CCU_IF(ctx.missionNum == 2)
        {
            CCU_IF(ctx.missionIndex != 0) { dst.addr += sizeA; }
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

    // 本地拷贝（GroupCopy）仅在 mission 0 执行（对应 hcomm 原版 missionId_==0 分支；
    // 单 mission 时 missionIndex 必然为 0，行为不变）
    CCU_IF(ctx.missionIndex == 0) { CCU_CHK_RET(GroupCopy(ctx, localDst, localSrc, ctx.goSize)); }

    CCU_CHK_RET(DoHalfAlltoAllVWait(ctx));

    // 收尾栅栏：对应 hcomm 原版 MissionSync(0)——两 mission 互等对方 writeDone 全清。
    // bit 按 mission 错开（post/wait 各一次，与 PreSync 同一死锁约束），门控用构图期
    // 常量（CCU_IF 是运行时分支，构图期两分支都会注册 tag）。
    if (ctx.hostMissionNum == KFC_MAX_MISSION_NUM) {
        if (ctx.hostMissionIndex == 0) {
            CCU_CHK_RET(ccu::EventRecord("MiFin_m0", 1U << 0));
            CCU_CHK_RET(ccu::EventWait("MiFin_m1", 1U << 1));
        } else {
            CCU_CHK_RET(ccu::EventRecord("MiFin_m1", 1U << 1));
            CCU_CHK_RET(ccu::EventWait("MiFin_m0", 1U << 0));
        }
    }
    return CCU_SUCCESS;
}

CcuResult CcuKfcHalfAlltoAllVMesh1DKernel(
    ccu::Variable userInAddr, ccu::Variable sendSizeAddr, ccu::Variable sendOffsetAddr, ccu::Variable goSize0,
    ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable recvOffset,
    ccu::Variable tokenInfo, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    uint64_t localWinAddr, ccu::Variable missionNum, ccu::Variable missionIndex, uint32_t hostMissionIndex,
    uint32_t hostMissionNum)
{
    KfcHalfAlltoAllVMesh1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    InitCcuKernelCtxBase(ctx);

    // 阶段 3-1：设置 mission 参数
    ctx.missionNum = missionNum;
    ctx.missionIndex = missionIndex;
    ctx.hostMissionIndex = hostMissionIndex;
    ctx.hostMissionNum = hostMissionNum;

    CCU_CHK_RET(InitResource(ctx));
    ctx.localWinAddr = localWinAddr;
    CCU_CHK_RET(LoadArgsFromMem(
        ctx, userInAddr, sendSizeAddr, sendOffsetAddr, goSize0, goSize1, goSize2, goSize3, recvOffset, tokenInfo));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoHalfAlltoAllV(ctx));
    CCU_CHK_RET(PostSync(ctx));
    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
