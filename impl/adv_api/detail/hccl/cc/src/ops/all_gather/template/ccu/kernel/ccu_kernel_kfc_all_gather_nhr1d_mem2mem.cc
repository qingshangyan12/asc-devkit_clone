/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// 与 hccl src/ops/all_gather/algorithm/template/ccu/kernel/ccu_kernel_all_gather_nhr1d_mem2mem.cc 逐行对照移植。
// hccl -> KFC 适配：
//  1. CcuKernelArg 单参数 + ccu::LoadArg → 显式参数列表（地址/尺寸来自 xnData 队列，token 来自 server ctx）。
//  2. CcuKernelArgAllGatherNHR1D 成员 → Context + dispatch 传参；rankSize/rankId/stepInfoVector/rank2ChannelIdx
//     来自 CcuKernelArgKfcServer（InheritKfcServerKernelArg 透传）。
//  3. 固定 axisId=0（单 die）语义：hccl 的 die1Size/die1LastSize/axisId==1 分支不迁移
//     （KFC 单 mission 约束，见 docs/migration/ccu-sched-allgather-sole-nhr-migration.md 4.2 节）。
//  4. GroupCopy 使用 devkit 4 参版本（无 GetCcuVersion）。
//  5. 步内流水（P0 优化，mesh DoAllGatherWrite/Wait 先例）：slice 数 ≤ 16 位事件预算时
//     本轮所有 slice 先全部发射（独立事件位、分片互不相交），轮末一次批量 EventWait；
//     超预算/空步回退原"发一个等一个"串行结构。
//  6. 本地拷贝与首步写重叠（P3 优化，mesh Phase1/2/3 先例）：首步写非阻塞发射后，
//     GroupCopy 本地拷贝与写并发（源相同只读不冲突、目的不同），EventWait 后再步间同步。
#include "ccu_kernel_kfc_all_gather_nhr1d_mem2mem.h"

namespace mc2_ops_hccl {
namespace {
// 与 hccl ccu_kernel_all_gather_nhr1d_mem2mem.cc 的 XN/CKE 位定义一致。
constexpr uint16_t AG_NHR_OUTPUT_XN_ID = 1;
constexpr uint16_t AG_NHR_TOKEN_XN_ID = 2;
constexpr uint16_t AG_NHR_POST_SYNC_BIT = 1U << 3U;
constexpr uint16_t AG_NHR_STEP_POST_SYNC_BIT = 1U << 5U;
constexpr uint16_t AG_NHR_CKE_IDX = 0;
constexpr uint16_t AG_NHR_BIT_NUM_PER_CKE = 16;

struct KfcAllGatherNhr1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels = nullptr;
    uint32_t channelCount = 0;
    uint32_t rankSize = 0;
    uint32_t rankId = 0;
    const std::vector<KfcNhrStepInfo>* stepInfoVector = nullptr;
    const std::map<uint32_t, uint32_t>* rank2ChannelIdx = nullptr;

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable die0Size;
    ccu::Variable die0LastSize;
    ccu::Variable repeatNum;
    ccu::Variable inputSliceStride;
    ccu::Variable outputSliceStride;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable isInputOutputEqual;
    ccu::Event localEvent;
    ccu::Variable repeatTimeflag;
    std::vector<ccu::Variable> outputSliceOffset;
    ccu::Variable myrankInputSliceOffset;
    ccu::LocalAddr srcMem;
    ccu::RemoteAddr dstMem;
    ccu::LocalAddr localDst;
    ccu::Variable constVar1;
    GroupOpSizeVars goSize;
    ccu::Variable groupCopyRepeatNum;
};

// 对应 hccl ParseKernelArg + InitResource：localSize/myRankIdx == channelCount（每 peer 一条通道）。
// 通道绑定变量必须以拷贝构造（句柄共享）保存：ccu::Variable 的 operator= 是寄存器值搬运指令，
// 会在 PreSync 握手完成前执行，快照到对端写入前的旧槽位值（对端 output 地址/token 陈旧，
// 远端写将落到错误目的或因 token 失配被丢弃）。hccl 原版 InitResource 即用 push_back 活绑定。
CcuResult InitResource(KfcAllGatherNhr1DContext& ctx)
{
    if (ctx.rankSize == 0U || ctx.rankId >= ctx.rankSize || ctx.channelCount == 0U || ctx.rank2ChannelIdx == nullptr ||
        ctx.rank2ChannelIdx->size() != ctx.channelCount || ctx.stepInfoVector == nullptr) {
        HCCL_ERROR(
            "[CcuKernelKfcAllGatherNHR1DMem2Mem] invalid resource, rankSize[%u], rankId[%u], channels[%u]",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CCU_E_INTERNAL;
    }
    for (uint32_t channelIdx = 0; channelIdx < ctx.channelCount; ++channelIdx) {
        ctx.output.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AG_NHR_OUTPUT_XN_ID));
        ctx.token.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], AG_NHR_TOKEN_XN_ID));
    }
    // 本地槽 [channelCount]：值来自本轮参数帧（kernel 启动前已写入），LoadNhr1DArgs 值赋值无竞争。
    ctx.output.push_back(ccu::Variable());
    ctx.token.push_back(ccu::Variable());
    ctx.outputSliceOffset.resize(ctx.rankSize);
    ctx.myrankInputSliceOffset = 0;
    ctx.repeatTimeflag = 0;
    ctx.constVar1 = 1;
    return CCU_SUCCESS;
}

// 对应 hccl LoadArgs：本 rank 的 output/token 从队列参数取得，其余形参直赋。
void LoadNhr1DArgs(
    KfcAllGatherNhr1DContext& ctx, ccu::Variable input, ccu::Variable output, ccu::Variable token,
    ccu::Variable die0Size, ccu::Variable die0LastSize, ccu::Variable repeatNumInv, ccu::Variable inputSliceStride,
    ccu::Variable outputSliceStride, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3)
{
    const uint32_t localIdx = ctx.channelCount;
    ctx.input = input;
    ctx.output[localIdx] = output;
    ctx.token[localIdx] = token;
    ctx.die0Size = die0Size;
    ctx.die0LastSize = die0LastSize;
    ctx.repeatNum = repeatNumInv;
    ctx.inputSliceStride = inputSliceStride;
    ctx.outputSliceStride = outputSliceStride;
    ctx.inputRepeatStride = inputRepeatStride;
    ctx.outputRepeatStride = outputRepeatStride;
    ctx.isInputOutputEqual = isInputOutputEqual;
    ctx.goSize.addrOffset = goSize0;
    ctx.goSize.loopParam = goSize1;
    ctx.goSize.parallelParam = goSize2;
    ctx.goSize.residual = goSize3;
}

// 对应 hccl PreSync：向所有 peer 交换本 rank 的 output/token xn 变量。
CcuResult PreSync(KfcAllGatherNhr1DContext& ctx)
{
    const uint32_t localIdx = ctx.channelCount;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output[localIdx], AG_NHR_OUTPUT_XN_ID, AG_NHR_CKE_IDX, 1U << AG_NHR_OUTPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[localIdx], AG_NHR_TOKEN_XN_ID, AG_NHR_CKE_IDX, 1U << AG_NHR_TOKEN_XN_ID));
    }
    const uint16_t allBit = (1U << AG_NHR_OUTPUT_XN_ID) | (1U << AG_NHR_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_NHR_CKE_IDX, allBit));
    }
    return CCU_SUCCESS;
}

// 对应 hccl PostSync。
CcuResult PostSync(KfcAllGatherNhr1DContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], AG_NHR_CKE_IDX, AG_NHR_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], AG_NHR_CKE_IDX, AG_NHR_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

// 只发射单个 slice 的 Write/占位 EventRecord，不等待；事件位由调用方分配。
// 流水路径步末批量 EventWait；串行路径逐 slice 等待（mesh DoAllGatherWrite/Wait 先例）。
CcuResult IssueSendSlice(
    KfcAllGatherNhr1DContext& ctx, const u32& toRank, ccu::LocalAddr& src, ccu::RemoteAddr& dst, u32 signalIndex,
    bool islastSlice)
{
    const ChannelHandle sendChannel = ctx.channels[ctx.rank2ChannelIdx->at(toRank)];
    ccu::Variable& sliceSize = islastSlice ? ctx.die0LastSize : ctx.die0Size;
    const uint16_t signalMask = 1U << signalIndex;
    CCU_IF(sliceSize != 0)
    {
        CCU_CHK_RET(ccu::Write(sendChannel, dst, src, sliceSize, ctx.localEvent, signalMask));
    }
    CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.localEvent, signalMask)); }
    return CCU_SUCCESS;
}

// 对应 hccl DoRepeatSendRecvSlices（axisId==0 语义：sliceSize 取 die0 档）。
// 串行回退路径：发射单个 slice 并立即等待其完成。
CcuResult DoRepeatSendRecvSlices(
    KfcAllGatherNhr1DContext& ctx, const u32& toRank, ccu::LocalAddr& src, ccu::RemoteAddr& dst, u32 signalIndex,
    bool islastSlice)
{
    const ChannelHandle sendChannel = ctx.channels[ctx.rank2ChannelIdx->at(toRank)];
    ccu::Variable tmpRepeatNum;
    tmpRepeatNum = ctx.repeatNum;
    ctx.repeatTimeflag = 0;

    CCU_WHILE(tmpRepeatNum != UINT64_MAX)
    {
        tmpRepeatNum += ctx.constVar1;
        CCU_IF(ctx.repeatTimeflag == 1)
        {
            src.addr += ctx.inputRepeatStride;
            dst.addr += ctx.outputRepeatStride;
        }
        ccu::Variable& sliceSize = islastSlice ? ctx.die0LastSize : ctx.die0Size;

        const uint16_t signalMask = 1U << signalIndex;
        CCU_IF(sliceSize != 0)
        {
            CCU_CHK_RET(ccu::Write(sendChannel, dst, src, sliceSize, ctx.localEvent, signalMask));
            CCU_CHK_RET(ccu::EventWait(ctx.localEvent, signalMask));
        }
        ctx.repeatTimeflag = 1;
    }

    return CCU_SUCCESS;
}

// 对应 hccl DoRepeatAllGatherNHRSingleStep（末步跳过步间同步与 hccl 一致）。
// 性能优化(P0)：在途 slice 数不超 16 位事件预算时走步内流水——本轮所有 slice 先全部
// 发射（各占独立事件位；各 slice 的 src/dst 分片互不相交，无数据依赖），轮末一次
// EventWait 批量等完成，消除"发一个等一个"的完成等待空转间隙。
// 先例：mesh kernel DoAllGatherWrite/DoAllGatherWait 的 Phase1/Phase2 批量等待。
CcuResult DoRepeatAllGatherNHRSingleStep(KfcAllGatherNhr1DContext& ctx, const KfcNhrStepInfo& nhrStepInfo)
{
    const u32 toRankIdx = ctx.rank2ChannelIdx->at(nhrStepInfo.toRank);
    const u32 fromRankIdx = ctx.rank2ChannelIdx->at(nhrStepInfo.fromRank);
    const ChannelHandle sendChannel = ctx.channels[toRankIdx];
    const ChannelHandle recvChannel = ctx.channels[fromRankIdx];
    const std::vector<u32>& sendSliceIdxList = nhrStepInfo.txSliceIdxs;
    const uint32_t localIdx = ctx.channelCount;

    ctx.srcMem.token = ctx.token[localIdx];
    ctx.dstMem.token = ctx.token[toRankIdx];

    const uint32_t sliceNum = static_cast<uint32_t>(sendSliceIdxList.size());
    if (sliceNum != 0U && sliceNum <= AG_NHR_BIT_NUM_PER_CKE) {
        // 流水路径 repeat 外层、slice 内层：同一 repeat 轮内所有 slice 共享一组事件位，
        // 轮末批量等待清位后再进下一轮；各分片地址 = 基址 + 分片偏移 + 帧偏移。
        const uint16_t waitMask = static_cast<uint16_t>((1U << sliceNum) - 1U);
        ccu::Variable turnOffsetIn;
        ccu::Variable turnOffsetOut;
        turnOffsetIn = 0;
        turnOffsetOut = 0;
        ccu::Variable tmpRepeatNum;
        tmpRepeatNum = ctx.repeatNum;
        ctx.repeatTimeflag = 0;
        CCU_WHILE(tmpRepeatNum != UINT64_MAX)
        {
            tmpRepeatNum += ctx.constVar1;
            for (uint32_t sliceSeq = 0; sliceSeq < sliceNum; ++sliceSeq) {
                const u32 sendSliceIdx = sendSliceIdxList[sliceSeq];
                const bool islastSlice = (sendSliceIdx + 1 == ctx.rankSize);
                if (nhrStepInfo.step == 0) {
                    ctx.srcMem.addr = ctx.input;
                    ctx.srcMem.addr += ctx.myrankInputSliceOffset;
                } else {
                    ctx.srcMem.addr = ctx.output[localIdx];
                    ctx.srcMem.addr += ctx.outputSliceOffset[sendSliceIdx];
                }
                ctx.srcMem.addr += turnOffsetIn;
                ctx.dstMem.addr = ctx.output[toRankIdx];
                ctx.dstMem.addr += ctx.outputSliceOffset[sendSliceIdx];
                ctx.dstMem.addr += turnOffsetOut;
                CCU_CHK_RET(IssueSendSlice(ctx, nhrStepInfo.toRank, ctx.srcMem, ctx.dstMem, sliceSeq, islastSlice));
            }
            CCU_CHK_RET(ccu::EventWait(ctx.localEvent, waitMask));
            turnOffsetIn += ctx.inputRepeatStride;
            turnOffsetOut += ctx.outputRepeatStride;
            ctx.repeatTimeflag = 1;
        }
    } else {
        // 串行回退（slice 数超 16 位事件预算或空步）：保持原"发一个等一个"结构。
        for (u32 i = 0; i < sliceNum; i++) {
            const u32 sendSliceIdx = sendSliceIdxList[i];
            if (nhrStepInfo.step == 0) {
                ctx.srcMem.addr = ctx.input;
                ctx.srcMem.addr += ctx.myrankInputSliceOffset;
            } else {
                ctx.srcMem.addr = ctx.output[localIdx];
                ctx.srcMem.addr += ctx.outputSliceOffset[sendSliceIdx];
            }
            ctx.dstMem.addr = ctx.output[toRankIdx];
            ctx.dstMem.addr += ctx.outputSliceOffset[sendSliceIdx];
            const bool islastSlice = (sendSliceIdx + 1 == ctx.rankSize);
            CCU_CHK_RET(DoRepeatSendRecvSlices(
                ctx, nhrStepInfo.toRank, ctx.srcMem, ctx.dstMem, i % AG_NHR_BIT_NUM_PER_CKE, islastSlice));
        }
    }

    if (nhrStepInfo.step + 1 != ctx.stepInfoVector->size()) {
        CCU_CHK_RET(ccu::NotifyRecord(sendChannel, AG_NHR_CKE_IDX, AG_NHR_STEP_POST_SYNC_BIT));
        CCU_CHK_RET(ccu::NotifyWait(recvChannel, AG_NHR_CKE_IDX, AG_NHR_STEP_POST_SYNC_BIT));
    }

    return CCU_SUCCESS;
}

// 对应 hccl DoAllGatherGroupCopy：isInputOutputEqual==0 时本地拷贝自身分片。
CcuResult DoAllGatherGroupCopy(KfcAllGatherNhr1DContext& ctx)
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
                    ctx.srcMem.addr += ctx.inputRepeatStride;
                }
                CCU_CHK_RET(GroupCopy(ctx, ctx.localDst, ctx.srcMem, ctx.goSize));
                ctx.repeatTimeflag = 1;
            }
        }
    }
    return CCU_SUCCESS;
}

// 对应 hccl DoRepeatAllGatherNHR（axisId==1 的 die0 偏移分支不迁移）。
// 性能优化(P3)：本地拷贝与首步远端写重叠——mesh kernel Phase1/Phase2/Phase3 先例。
// 首步(step0)写源 = input（与本地拷贝同源、只读不冲突），目的 = peer output（与本地
// 拷贝目的 output[localIdx] 不同，不冲突），二者完全独立可并发。CCU 指令流：
//   Issue step0 Write(非阻塞) → GroupCopy(本地拷贝) → EventWait(等写完成) → StepSync
// 本地拷贝 ~6.5us 被 step0 写 ~6.1us 完全覆盖，节省关键路径 ~6us。
// 首步 nSlices 恒为 1（NHR 算法性质：deltaRank 最大 → nSlices 最小），事件位仅需 bit0。
CcuResult DoRepeatAllGatherNHR(KfcAllGatherNhr1DContext& ctx)
{
    ccu::Variable tmpSliceOffset;
    tmpSliceOffset = 0;

    for (u64 i = 0; i < ctx.rankId; i++) {
        ctx.myrankInputSliceOffset += ctx.inputSliceStride;
    }

    for (u64 i = 0; i < ctx.rankSize; i++) {
        ctx.outputSliceOffset[i] = tmpSliceOffset;
        tmpSliceOffset += ctx.outputSliceStride;
    }

    const uint32_t localIdx = ctx.channelCount;
    ctx.srcMem.addr = ctx.input;
    ctx.srcMem.addr += ctx.myrankInputSliceOffset;
    ctx.srcMem.token = ctx.token[localIdx];
    ctx.dstMem.addr = ctx.output[localIdx];
    ctx.dstMem.addr += ctx.outputSliceOffset[ctx.rankId];
    ctx.dstMem.token = ctx.token[localIdx];
    ctx.localDst.addr = ctx.output[localIdx];
    ctx.localDst.addr += ctx.outputSliceOffset[ctx.rankId];
    ctx.localDst.token = ctx.token[localIdx];
    ctx.groupCopyRepeatNum = ctx.repeatNum;

    // P3：首步写发射 + 本地拷贝重叠
    if (!ctx.stepInfoVector->empty()) {
        const KfcNhrStepInfo& firstStep = (*ctx.stepInfoVector)[0];
        const u32 toRankIdx = ctx.rank2ChannelIdx->at(firstStep.toRank);
        const u32 fromRankIdx = ctx.rank2ChannelIdx->at(firstStep.fromRank);
        const ChannelHandle sendChannel = ctx.channels[toRankIdx];
        const ChannelHandle recvChannel = ctx.channels[fromRankIdx];

        ctx.dstMem.token = ctx.token[toRankIdx];
        // step0 的源 = input（srcMem 已指向正确位置，IssueSendSlice 按值捕获不冲突）
        for (u32 i = 0; i < firstStep.txSliceIdxs.size(); i++) {
            const u32 sendSliceIdx = firstStep.txSliceIdxs[i];
            ctx.dstMem.addr = ctx.output[toRankIdx];
            ctx.dstMem.addr += ctx.outputSliceOffset[sendSliceIdx];
            const bool islastSlice = (sendSliceIdx + 1 == ctx.rankSize);
            CCU_CHK_RET(IssueSendSlice(ctx, firstStep.toRank, ctx.srcMem, ctx.dstMem, i, islastSlice));
        }
        const uint32_t firstSliceNum = static_cast<uint32_t>(firstStep.txSliceIdxs.size());
        const uint16_t firstWaitMask = static_cast<uint16_t>((1U << firstSliceNum) - 1U);

        // 本地拷贝与首步写并发执行（GroupCopy 引擎与 XN 写引擎独立）
        CCU_CHK_RET(DoAllGatherGroupCopy(ctx));

        // 等首步写完成
        CCU_CHK_RET(ccu::EventWait(ctx.localEvent, firstWaitMask));

        // 首步步间同步（若非末步）
        if (firstStep.step + 1 != ctx.stepInfoVector->size()) {
            CCU_CHK_RET(ccu::NotifyRecord(sendChannel, AG_NHR_CKE_IDX, AG_NHR_STEP_POST_SYNC_BIT));
            CCU_CHK_RET(ccu::NotifyWait(recvChannel, AG_NHR_CKE_IDX, AG_NHR_STEP_POST_SYNC_BIT));
        }

        // 后续步骤正常处理（P0 流水化已在 DoRepeatAllGatherNHRSingleStep 内）
        for (size_t stepIdx = 1; stepIdx < ctx.stepInfoVector->size(); stepIdx++) {
            CCU_CHK_RET(DoRepeatAllGatherNHRSingleStep(ctx, (*ctx.stepInfoVector)[stepIdx]));
        }
    } else {
        // 空步骤向量（防御）：退化为仅本地拷贝
        CCU_CHK_RET(DoAllGatherGroupCopy(ctx));
    }

    return CCU_SUCCESS;
}
} // namespace

CcuResult CcuKernelKfcAllGatherNHR1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable die0Size,
    ccu::Variable die0LastSize, ccu::Variable repeatNumInv, ccu::Variable inputSliceStride,
    ccu::Variable outputSliceStride, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const std::vector<KfcNhrStepInfo>& stepInfoVector, const std::map<uint32_t, uint32_t>& rank2ChannelIdx)
{
    KfcAllGatherNhr1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.stepInfoVector = &stepInfoVector;
    ctx.rank2ChannelIdx = &rank2ChannelIdx;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));

    LoadNhr1DArgs(
        ctx, inputAddr, outputAddr, tokenInfo, die0Size, die0LastSize, repeatNumInv, inputSliceStride,
        outputSliceStride, inputRepeatStride, outputRepeatStride, isInputOutputEqual, goSize0, goSize1, goSize2,
        goSize3);

    CCU_CHK_RET(PreSync(ctx));

    CCU_CHK_RET(DoRepeatAllGatherNHR(ctx));

    CCU_CHK_RET(PostSync(ctx));

    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
