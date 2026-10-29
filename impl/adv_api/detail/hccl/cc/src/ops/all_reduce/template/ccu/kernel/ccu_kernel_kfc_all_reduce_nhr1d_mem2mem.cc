/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// 与 hccl src/ops/all_reduce/algorithm/template/ccu/kernel/ccu_kernel_all_reduce_nhr1d_mem2mem.cc 逐行对照移植。
// hccl -> KFC 适配：
//  1. CcuKernelArg 单参数 LoadArg → dispatch 显式传参（stepInfoVector/rank2ChannelIdx 经
//     CcuKernelArgKfcServer 的 algArg 同构字段下发，axisId/axisSize 来自 kernelArg）。
//  2. 通道绑定变量（GetResByChannel 结果）以拷贝构造（push_back）保存活绑定——operator= 是
//     寄存器值搬运指令，会在 PreSync 握手完成前执行、快照到对端写入前的旧槽位值
//     （AG SoleNHR kernel InitResource 注释同款结论；hccl 原版用 operator= 赋值，此处修正）。
//  3. hccl 两处 CCU_IF(sliceSize!=0)/CCU_IF(sliceSize==0) 改为 CCU_IF/CCU_ELSE 配对
//     （避免 block MS 资源翻倍），分支语义不变。
//  4. host 期常量分支（axisId==1 的 die0 偏移、axisId 的 sliceSize 选档）保持普通 if
//     （emission 期求值，不产生运行时指令），与 hccl 一致。
//  5. indexMap（rank→channel）查找由 operator[] 改为 find+报错（emission 期防御，缺失键不静默置 0）。
#include "ccu_kernel_kfc_all_reduce_nhr1d_mem2mem.h"

namespace mc2_ops_hccl {
namespace {
// 与 hccl ccu_kernel_all_reduce_nhr1d_mem2mem.cc 的 XN/CKE 位定义一致。
constexpr uint16_t AR_NHR_OUTPUT_XN_ID = 1;
constexpr uint16_t AR_NHR_TOKEN_XN_ID = 2;
constexpr uint16_t AR_NHR_POST_SYNC_BIT = 1U << 3U;
constexpr uint16_t AR_NHR_STEP0_PRE_SYNC_BIT = 1U << 4U;
constexpr uint16_t AR_NHR_STEP0_POST_SYNC_BIT = 1U << 5U;
constexpr uint16_t AR_NHR_STEP1_POST_SYNC_BIT = 1U << 6U;
constexpr uint16_t AR_NHR_CKE_IDX = 0;
constexpr uint16_t AR_NHR_RANK_NUM_PER_CKE = 16; // 本 rank 给远端置位时应当写的 CKE，16 个对端一个 CKE
constexpr u32 AR_NHR_STAG_NUM = 2;               // hccl NHR_NUM：stepInfoVector = RS 前半 + AG 后半

struct KfcAllReduceNhr1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels = nullptr;
    uint32_t channelCount = 0;
    uint32_t rankSize = 0;
    uint32_t rankId = 0;
    uint32_t axisId = 0; // die 维编号：0=die0, 1=die1
    uint32_t axisSize = 0; // 与 hccl ctx.axisSize 对齐保留（kernel 体不消费）
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_RESERVED;
    HcclReduceOp reduceOp = HcclReduceOp::HCCL_REDUCE_SUM;
    const std::vector<KfcNhrStepInfo>* stepInfoVector = nullptr;
    const std::map<uint32_t, uint32_t>* rank2ChannelIdx = nullptr;

    uint32_t localSize = 0;  // 本 rank 通信对端数（== channelCount，hccl ctx.localSize）
    uint32_t myRankIdx = 0;  // 本 rank 槽位下标（== channelCount，hccl ctx.myRankIdx）

    // 载入参数（hccl LoadArgs 同序）
    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable isInputOutputEqual;
    ccu::Variable die0Size;
    ccu::Variable die1Size;
    ccu::Variable die0SliceSize;
    ccu::Variable die1SliceSize;
    ccu::Variable die0LastSliceSize;
    ccu::Variable die1LastSliceSize;

    ccu::Event localEvent;
    std::vector<ccu::Variable> sliceOffset;

    ccu::LocalAddr srcMem;
    ccu::RemoteAddr rmtDstMem;
    ccu::LocalAddr locDstMem;
};

// 对应 hccl ParseKernelArg：rankSize/rankId/axisId/dataType/reduceOp 直接取自显式形参。
CcuResult InitResource(KfcAllReduceNhr1DContext& ctx)
{
    if (ctx.channelCount == 0U || ctx.rankSize == 0U || ctx.rankId >= ctx.rankSize ||
        ctx.rank2ChannelIdx == nullptr || ctx.rank2ChannelIdx->size() != ctx.channelCount ||
        ctx.stepInfoVector == nullptr) {
        HCCL_ERROR(
            "[CcuKernelKfcAllReduceNHR1DMem2Mem] invalid resource, rankSize[%u], rankId[%u], channels[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }
    HCCL_INFO(
        "[CcuKernelKfcAllReduceNHR1DMem2Mem] channels.size: [%u], rankId[%u], axisId[%u], dataType[%d], reduceOp[%d]",
        ctx.channelCount, ctx.rankId, ctx.axisId, static_cast<int>(ctx.dataType), static_cast<int>(ctx.reduceOp));

    ctx.localSize = ctx.channelCount;
    ctx.myRankIdx = ctx.channelCount;
    // 通道绑定变量 push_back 活绑定（拷贝构造共享 handle），本地槽 [localSize] 追加默认 Variable，
    // 值由 LoadNhr1DArgs 从本轮参数帧取得（kernel 启动前已写入，无竞争）。
    for (uint32_t channelIdx = 0; channelIdx < ctx.channelCount; ++channelIdx) {
        ctx.output.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_NHR_OUTPUT_XN_ID));
        ctx.token.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AR_NHR_TOKEN_XN_ID));
    }
    ctx.output.push_back(ccu::Variable());
    ctx.token.push_back(ccu::Variable());

    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

// 对应 hccl LoadArgs：argId 顺序 input/output/token/isInputOutputEqual/die0Size/die1Size/
// die0SliceSize/die1SliceSize/die0LastSliceSize/die1LastSliceSize（token 走 server ctx.token）。
void LoadNhr1DArgs(
    KfcAllReduceNhr1DContext& ctx, ccu::Variable input, ccu::Variable output, ccu::Variable token,
    ccu::Variable isInputOutputEqual, ccu::Variable die0Size, ccu::Variable die1Size, ccu::Variable die0SliceSize,
    ccu::Variable die1SliceSize, ccu::Variable die0LastSliceSize, ccu::Variable die1LastSliceSize)
{
    const uint32_t localIdx = ctx.myRankIdx;
    ctx.input = input;
    ctx.output[localIdx] = output;
    ctx.token[localIdx] = token;
    ctx.isInputOutputEqual = isInputOutputEqual;
    ctx.die0Size = die0Size;
    ctx.die1Size = die1Size;
    ctx.die0SliceSize = die0SliceSize;
    ctx.die1SliceSize = die1SliceSize;
    ctx.die0LastSliceSize = die0LastSliceSize;
    ctx.die1LastSliceSize = die1LastSliceSize;
}

// 对应 hccl PreSync：向所有 peer 交换本 rank 的 output/token xn 变量。
CcuResult PreSync(KfcAllReduceNhr1DContext& ctx)
{
    const uint32_t localIdx = ctx.myRankIdx;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[localIdx], AR_NHR_OUTPUT_XN_ID, AR_NHR_CKE_IDX, 1U << AR_NHR_OUTPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[localIdx], AR_NHR_TOKEN_XN_ID, AR_NHR_CKE_IDX, 1U << AR_NHR_TOKEN_XN_ID));
    }
    const uint16_t allBit = (1U << AR_NHR_OUTPUT_XN_ID) | (1U << AR_NHR_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_NHR_CKE_IDX, allBit));
    }
    return CCU_SUCCESS;
}

// 对应 hccl PostSync。
CcuResult PostSync(KfcAllReduceNhr1DContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], AR_NHR_CKE_IDX, AR_NHR_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AR_NHR_CKE_IDX, AR_NHR_POST_SYNC_BIT));
    }
    HCCL_DEBUG("[CcuKernelKfcAllReduceNHR1DMem2Mem] PostSync run finished");
    return CCU_SUCCESS;
}

// 对应 hccl ctx.indexMap[rank]：emission 期 find，缺失键报错（hccl operator[] 缺失时静默置 0）。
CcuResult GetChannelIdxOfRank(const KfcAllReduceNhr1DContext& ctx, u32 rank, u32& channelIdx)
{
    const auto it = ctx.rank2ChannelIdx->find(rank);
    if (it == ctx.rank2ChannelIdx->end()) {
        HCCL_ERROR("[CcuKernelKfcAllReduceNHR1DMem2Mem] rank2ChannelIdx not find rank key [%u]", rank);
        return CcuResult::CCU_E_PARA;
    }
    channelIdx = it->second;
    return CCU_SUCCESS;
}

// 对应 hccl DoWriteReduceSlice：axisId==1 加 die0 基偏移；最后一块 slice 大小可能不一致。
CcuResult DoWriteReduceSlice(
    KfcAllReduceNhr1DContext& ctx, const u32& toRank, ccu::LocalAddr& src, ccu::RemoteAddr& dst,
    const u32& sendSliceIdx, u32 signalIndex)
{
    u32 toRankIdx = 0;
    CCU_CHK_RET(GetChannelIdxOfRank(ctx, toRank, toRankIdx));
    const ChannelHandle sendChannel = ctx.channels[toRankIdx];

    // 添加 die1 偏移
    if (ctx.axisId == 1) {
        src.addr += ctx.die0Size;
        dst.addr += ctx.die0Size;
    }

    // allreduce 切片的最后一块 slice，大小可能不一致
    const bool islastSlice = (sendSliceIdx + 1 == ctx.rankSize);
    ccu::Variable& sliceSize = ctx.axisId == 0 ? (islastSlice ? ctx.die0LastSliceSize : ctx.die0SliceSize) :
                                                (islastSlice ? ctx.die1LastSliceSize : ctx.die1SliceSize);
    const uint16_t signalMask = static_cast<uint16_t>(1U << signalIndex);
    CCU_IF(sliceSize != 0)
    {
        CCU_CHK_RET(
            ccu::WriteReduce(sendChannel, dst, src, sliceSize, ctx.dataType, ctx.reduceOp, ctx.localEvent, signalMask));
    }
    CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.localEvent, signalMask)); }
    return CCU_SUCCESS;
}

// 对应 hccl DoReduceScatterNHRSingleStep：step!=0 先做步间就绪同步（PreSync 基址交换刚做过全屏障，
// step0 跳过）；slice 按 16 个一组挂事件位，步末按组等待。
CcuResult DoReduceScatterNHRSingleStep(KfcAllReduceNhr1DContext& ctx, const KfcNhrStepInfo& nhrStepInfo)
{
    u32 toRankIdx = 0;
    u32 fromRankIdx = 0;
    CCU_CHK_RET(GetChannelIdxOfRank(ctx, nhrStepInfo.toRank, toRankIdx));
    CCU_CHK_RET(GetChannelIdxOfRank(ctx, nhrStepInfo.fromRank, fromRankIdx));
    const ChannelHandle sendChannel = ctx.channels[toRankIdx];
    const ChannelHandle recvChannel = ctx.channels[fromRankIdx];
    const std::vector<u32>& sendSliceIdxList = nhrStepInfo.txSliceIdxs;
    const uint32_t localIdx = ctx.myRankIdx;
    ctx.srcMem.token = ctx.token[localIdx];
    ctx.rmtDstMem.token = ctx.token[toRankIdx];

    if (nhrStepInfo.step != 0) {
        // 通知 fromRank，可以写入
        CCU_CHK_RET(ccu::NotifyRecord(recvChannel, AR_NHR_CKE_IDX, AR_NHR_STEP0_PRE_SYNC_BIT));

        // 等待 toRank 通知其可以写入
        CCU_CHK_RET(ccu::NotifyWait(sendChannel, AR_NHR_CKE_IDX, AR_NHR_STEP0_PRE_SYNC_BIT));
    }

    for (u32 i = 0; i < sendSliceIdxList.size(); i++) {
        const u32 sendSliceIdx = sendSliceIdxList[i];

        if (i != 0 && i % AR_NHR_RANK_NUM_PER_CKE == 0) {
            CCU_CHK_RET(ccu::EventWait(ctx.localEvent, static_cast<uint16_t>((1U << AR_NHR_RANK_NUM_PER_CKE) - 1U)));
        }

        if (nhrStepInfo.step == 0) {
            ctx.srcMem.addr = ctx.input;
            ctx.srcMem.addr += ctx.sliceOffset[sendSliceIdx];
        } else {
            ctx.srcMem.addr = ctx.output[localIdx];
            ctx.srcMem.addr += ctx.sliceOffset[sendSliceIdx];
        }

        ctx.rmtDstMem.addr = ctx.output[toRankIdx];
        ctx.rmtDstMem.addr += ctx.sliceOffset[sendSliceIdx];

        CCU_CHK_RET(DoWriteReduceSlice(
            ctx, nhrStepInfo.toRank, ctx.srcMem, ctx.rmtDstMem, sendSliceIdx, i % AR_NHR_RANK_NUM_PER_CKE));
    }
    // 最后一组 slice 可能不足 16 个，也可能正好 16 个；size%16 为 0 时需等待满 16 个 bit
    // 空列表场景下未进入循环，不产生任何 event bit，跳过 EventWait 避免死锁
    if (!sendSliceIdxList.empty()) {
        const u32 lastGroupSize = ((static_cast<u32>(sendSliceIdxList.size()) - 1) % AR_NHR_RANK_NUM_PER_CKE) + 1;
        CCU_CHK_RET(ccu::EventWait(ctx.localEvent, static_cast<uint16_t>((1U << lastGroupSize) - 1U)));
    }

    // 通知 toRank 数据写入完毕
    CCU_CHK_RET(ccu::NotifyRecord(sendChannel, AR_NHR_CKE_IDX, AR_NHR_STEP0_POST_SYNC_BIT));
    // 等待 fromRank 通知数据写入完毕
    CCU_CHK_RET(ccu::NotifyWait(recvChannel, AR_NHR_CKE_IDX, AR_NHR_STEP0_POST_SYNC_BIT));
    HCCL_DEBUG(
        "[DoReduceScatterNHRSingleStep] rank %u step %u, toRank=%u, fromRank=%u, nSlice=%zu", ctx.rankId,
        nhrStepInfo.step, nhrStepInfo.toRank, nhrStepInfo.fromRank, sendSliceIdxList.size());
    return CCU_SUCCESS;
}

// 对应 hccl DoReduceScatterNHR：前半 step 为 reduce-scatter。
CcuResult DoReduceScatterNHR(KfcAllReduceNhr1DContext& ctx)
{
    for (u64 i = 0; i < ctx.stepInfoVector->size() / AR_NHR_STAG_NUM; i++) {
        const KfcNhrStepInfo& nhrStepInfo = (*ctx.stepInfoVector)[i];
        CCU_CHK_RET(DoReduceScatterNHRSingleStep(ctx, nhrStepInfo));
    }
    return CCU_SUCCESS;
}

// 对应 hccl DoSendRecvSlice：AG 阶段纯写。
CcuResult DoSendRecvSlice(
    KfcAllReduceNhr1DContext& ctx, const u32& toRank, ccu::LocalAddr& src, ccu::RemoteAddr& dst,
    const u32& sendSliceIdx, u32 signalIndex)
{
    u32 toRankIdx = 0;
    CCU_CHK_RET(GetChannelIdxOfRank(ctx, toRank, toRankIdx));
    const ChannelHandle sendChannel = ctx.channels[toRankIdx];

    // 添加 die1 偏移
    if (ctx.axisId == 1) {
        src.addr += ctx.die0Size;
        dst.addr += ctx.die0Size;
    }

    const bool islastSlice = (sendSliceIdx + 1 == ctx.rankSize);
    ccu::Variable& sliceSize = ctx.axisId == 0 ? (islastSlice ? ctx.die0LastSliceSize : ctx.die0SliceSize) :
                                                (islastSlice ? ctx.die1LastSliceSize : ctx.die1SliceSize);
    const uint16_t signalMask = static_cast<uint16_t>(1U << signalIndex);
    CCU_IF(sliceSize != 0) { CCU_CHK_RET(ccu::Write(sendChannel, dst, src, sliceSize, ctx.localEvent, signalMask)); }
    CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.localEvent, signalMask)); }
    return CCU_SUCCESS;
}

// 对应 hccl DoAllGatherNHRSingleStep：AG 阶段步间同步位 STEP1_POST_SYNC（hccl 原条件
// step+1 != stepInfoVector.size() 对 AG 相位恒成立——AG 相位 step 为 0 基相位内编号，
// 保持逐行一致）。
CcuResult DoAllGatherNHRSingleStep(KfcAllReduceNhr1DContext& ctx, const KfcNhrStepInfo& nhrStepInfo)
{
    u32 toRankIdx = 0;
    u32 fromRankIdx = 0;
    CCU_CHK_RET(GetChannelIdxOfRank(ctx, nhrStepInfo.toRank, toRankIdx));
    CCU_CHK_RET(GetChannelIdxOfRank(ctx, nhrStepInfo.fromRank, fromRankIdx));
    const ChannelHandle sendChannel = ctx.channels[toRankIdx];
    const ChannelHandle recvChannel = ctx.channels[fromRankIdx];
    const std::vector<u32>& sendSliceIdxList = nhrStepInfo.txSliceIdxs;
    const uint32_t localIdx = ctx.myRankIdx;
    ctx.srcMem.token = ctx.token[localIdx];
    ctx.rmtDstMem.token = ctx.token[toRankIdx];

    for (u32 i = 0; i < sendSliceIdxList.size(); i++) {
        const u32 sendSliceIdx = sendSliceIdxList[i];

        if (i != 0 && i % AR_NHR_RANK_NUM_PER_CKE == 0) {
            CCU_CHK_RET(ccu::EventWait(ctx.localEvent, static_cast<uint16_t>((1U << AR_NHR_RANK_NUM_PER_CKE) - 1U)));
        }

        ctx.srcMem.addr = ctx.output[localIdx];
        ctx.srcMem.addr += ctx.sliceOffset[sendSliceIdx];

        ctx.rmtDstMem.addr = ctx.output[toRankIdx];
        ctx.rmtDstMem.addr += ctx.sliceOffset[sendSliceIdx];
        CCU_CHK_RET(DoSendRecvSlice(
            ctx, nhrStepInfo.toRank, ctx.srcMem, ctx.rmtDstMem, sendSliceIdx, i % AR_NHR_RANK_NUM_PER_CKE));
    }
    // 最后一组 slice 可能不足 16 个，也可能正好 16 个；size%16 为 0 时需等待满 16 个 bit
    // 空列表场景下未进入循环，不产生任何 event bit，跳过 EventWait 避免死锁
    if (!sendSliceIdxList.empty()) {
        const u32 lastGroupSize = ((static_cast<u32>(sendSliceIdxList.size()) - 1) % AR_NHR_RANK_NUM_PER_CKE) + 1;
        CCU_CHK_RET(ccu::EventWait(ctx.localEvent, static_cast<uint16_t>((1U << lastGroupSize) - 1U)));
    }

    if (nhrStepInfo.step + 1 != ctx.stepInfoVector->size()) { // 最后一步不需要同步
        // 通知 toRank，写入完毕
        CCU_CHK_RET(ccu::NotifyRecord(sendChannel, AR_NHR_CKE_IDX, AR_NHR_STEP1_POST_SYNC_BIT));
        // 等待 fromRank 通知写入完毕
        CCU_CHK_RET(ccu::NotifyWait(recvChannel, AR_NHR_CKE_IDX, AR_NHR_STEP1_POST_SYNC_BIT));
    }

    HCCL_DEBUG(
        "[DoAllGatherNHRSingleStep] rank %u step %u, toRank=%u, fromRank=%u, nSlice=%zu", ctx.rankId, nhrStepInfo.step,
        nhrStepInfo.toRank, nhrStepInfo.fromRank, sendSliceIdxList.size());
    return CCU_SUCCESS;
}

// 对应 hccl DoAllGatherNHR：后半 step 为 all-gather。
CcuResult DoAllGatherNHR(KfcAllReduceNhr1DContext& ctx)
{
    for (u64 i = ctx.stepInfoVector->size() / AR_NHR_STAG_NUM; i < ctx.stepInfoVector->size(); i++) {
        const KfcNhrStepInfo& nhrStepInfo = (*ctx.stepInfoVector)[i];
        CCU_CHK_RET(DoAllGatherNHRSingleStep(ctx, nhrStepInfo));
    }
    return CCU_SUCCESS;
}

// 对应 hccl GetNonTxSliceIdxs：取 step0 中本 rank 不发送的 slice 集合（host 期纯函数）。
std::vector<u32> GetNonTxSliceIdxs(const KfcAllReduceNhr1DContext& ctx, const std::vector<u32>& txSliceIdxs)
{
    std::vector<bool> isTx(ctx.rankSize, false);
    for (u32 idx : txSliceIdxs) {
        if (idx < ctx.rankSize) {
            isTx[idx] = true;
        }
    }

    std::vector<u32> nonTxSliceIdxs;
    for (u32 idx = 0; idx < ctx.rankSize; ++idx) {
        if (!isTx[idx]) {
            nonTxSliceIdxs.push_back(idx);
        }
    }

    return nonTxSliceIdxs;
}

// 对应 hccl DoLocalCopySlice。
CcuResult DoLocalCopySlice(
    KfcAllReduceNhr1DContext& ctx, ccu::LocalAddr& src, ccu::LocalAddr& dst, const u32& copySliceIdx, u32 signalIndex)
{
    // 添加 die1 偏移
    if (ctx.axisId == 1) {
        src.addr += ctx.die0Size;
        dst.addr += ctx.die0Size;
    }

    const bool islastSlice = (copySliceIdx + 1 == ctx.rankSize);
    ccu::Variable& sliceSize = ctx.axisId == 0 ? (islastSlice ? ctx.die0LastSliceSize : ctx.die0SliceSize) :
                                                (islastSlice ? ctx.die1LastSliceSize : ctx.die1SliceSize);
    const uint16_t signalMask = static_cast<uint16_t>(1U << signalIndex);
    CCU_IF(sliceSize != 0) { CCU_CHK_RET(ccu::LocalCopy(dst, src, sliceSize, ctx.localEvent, signalMask)); }
    CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.localEvent, signalMask)); }
    return CCU_SUCCESS;
}

// 对应 hccl LocalCopySlices：累计 sliceOffset；input != output 时将 step0 不发送的 slice
// 从 input 拷到本 rank 的 output。
CcuResult LocalCopySlices(KfcAllReduceNhr1DContext& ctx)
{
    ccu::Variable tmpSliceOffset;
    tmpSliceOffset = 0;

    for (u64 i = 0; i < ctx.rankSize; i++) {
        ccu::Variable offset;
        ctx.sliceOffset.push_back(offset);
        ctx.sliceOffset[i] = tmpSliceOffset;
        tmpSliceOffset += ctx.axisId == 0 ? ctx.die0SliceSize : ctx.die1SliceSize;
    }

    const uint32_t localIdx = ctx.myRankIdx;
    // 当 input == output 时，不需要拷贝
    CCU_IF(ctx.isInputOutputEqual == 0)
    {
        // 将 step0 中不需要写的 slice，拷贝到本 rank 的 output 中
        const KfcNhrStepInfo& nhrStepInfo = (*ctx.stepInfoVector)[0];
        const std::vector<u32> nonTxSliceIdxList = GetNonTxSliceIdxs(ctx, nhrStepInfo.txSliceIdxs);
        for (u32 i = 0; i < nonTxSliceIdxList.size(); i++) {
            const u32 nonTxSliceIdx = nonTxSliceIdxList[i];

            if (i != 0 && i % AR_NHR_RANK_NUM_PER_CKE == 0) {
                CCU_CHK_RET(
                    ccu::EventWait(ctx.localEvent, static_cast<uint16_t>((1U << AR_NHR_RANK_NUM_PER_CKE) - 1U)));
            }

            ctx.srcMem.addr = ctx.input;
            ctx.srcMem.addr += ctx.sliceOffset[nonTxSliceIdx];
            ctx.srcMem.token = ctx.token[localIdx];

            ctx.locDstMem.addr = ctx.output[localIdx];
            ctx.locDstMem.addr += ctx.sliceOffset[nonTxSliceIdx];
            ctx.locDstMem.token = ctx.token[localIdx];
            CCU_CHK_RET(DoLocalCopySlice(ctx, ctx.srcMem, ctx.locDstMem, nonTxSliceIdx, i % AR_NHR_RANK_NUM_PER_CKE));
        }
        // 最后一组 slice 可能不足 16 个，也可能正好 16 个；size%16 为 0 时需等待满 16 个 bit
        // 空列表场景下未进入循环，不产生任何 event bit，跳过 EventWait 避免死锁
        if (!nonTxSliceIdxList.empty()) {
            const u32 lastGroupSize = ((static_cast<u32>(nonTxSliceIdxList.size()) - 1) % AR_NHR_RANK_NUM_PER_CKE) + 1;
            CCU_CHK_RET(ccu::EventWait(ctx.localEvent, static_cast<uint16_t>((1U << lastGroupSize) - 1U)));
        }
    }
    return CCU_SUCCESS;
}
} // namespace

// 对应 hccl CcuAllReduceNHR1DKernel：LocalCopySlices → PreSync → RS → AG → PostSync
// （LocalCopySlices 仅触碰本地 input/output，先于 PreSync 执行与 hccl 一致）。
CcuResult CcuKernelKfcAllReduceNHR1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable isInputOutputEqual,
    ccu::Variable die0Size, ccu::Variable die1Size, ccu::Variable die0SliceSize, ccu::Variable die1SliceSize,
    ccu::Variable die0LastSliceSize, ccu::Variable die1LastSliceSize, const ChannelHandle channels[],
    uint32_t channelCount, uint32_t rankSize, uint32_t rankId, uint32_t axisId, uint32_t axisSize,
    const HcclDataType& dataType, const HcclReduceOp& reduceType, const std::vector<KfcNhrStepInfo>& stepInfoVector,
    const std::map<uint32_t, uint32_t>& rank2ChannelIdx)
{
    (void)axisSize; // hccl ctx.axisSize 同样不参与 kernel 体计算，仅随 kernelArg 传递
    KfcAllReduceNhr1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.axisId = axisId;
    ctx.axisSize = axisSize;
    ctx.dataType = dataType;
    ctx.reduceOp = reduceType;
    ctx.stepInfoVector = &stepInfoVector;
    ctx.rank2ChannelIdx = &rank2ChannelIdx;
    InitCcuKernelCtxBase(ctx);

    HCCL_INFO("[CcuKernelKfcAllReduceNHR1DMem2Mem] AllReduceNHR1D run");
    CCU_CHK_RET(InitResource(ctx));
    LoadNhr1DArgs(
        ctx, inputAddr, outputAddr, tokenInfo, isInputOutputEqual, die0Size, die1Size, die0SliceSize, die1SliceSize,
        die0LastSliceSize, die1LastSliceSize);
    CCU_CHK_RET(LocalCopySlices(ctx));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoReduceScatterNHR(ctx));
    CCU_CHK_RET(DoAllGatherNHR(ctx));
    CCU_CHK_RET(PostSync(ctx));

    HCCL_INFO("[CcuKernelKfcAllReduceNHR1DMem2Mem] AllReduceNHR1D end");
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
