/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

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
    ccu::Event localEvent;
    ccu::Variable repeatTimeflag;
    std::vector<ccu::Variable> outputSliceOffset;
    ccu::Variable myrankInputSliceOffset;
    ccu::LocalAddr srcMem;
    ccu::RemoteAddr dstMem;
    ccu::Variable constVar1;
    // 分摊拷贝：本轴拷贝区基址不含 die 烘焙偏移（axis1 的 input/output 已 += die0Size，
    // 而拷贝区由 AIV 按双 die 50/50 配平、与 die 数据半区解耦），故独立保存基址寄存器；
    // copySrcMem/copyDstMem 为发射工作寄存器（V1 GroupCopy 内部会就地推进其地址，基址不受
    // 污染）。拷贝引擎按芯片版本选择（mesh1d AG kernel 先例）：V1 走 GroupCopy（goSize 驱动），
    // V2 走单跳 LocalCopy（chunkSize/tailSize 驱动，专用 copyEvent 免与 slice 事件位冲突）
    CcuVersion ccuVersion = CcuVersion::CCU_V1;
    ccu::Event copyEvent;
    ccu::Variable copySrcBase;
    ccu::Variable copyDstBase;
    ccu::Variable copyChunkSize;
    ccu::Variable copyChunkLoopNum;
    ccu::Variable copyTailSize;
    ccu::Variable copyCur;
    GroupOpSizeVars chunkGoSize;
    GroupOpSizeVars tailGoSize;
    ccu::LocalAddr copySrcMem;
    ccu::LocalAddr copyDstMem;
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
// sole-NHR 常量参数不占 xnData 槽，在此内联（prepare 恒定写入，见
// KfcAllGatherSoleNhrParamIndex）：die0LastSize=die0Size、repeatNum=1（REPEAT_NUM_INV=
// UINT64_MAX-1）、三 stride=0；isInputOutputEqual 经拷贝参数表达（in-place 时 AIV 按
// 0 字节计算：CHUNK_LOOP 哨兵满、TAIL_SIZE=0、尾块 goSize 全零，V1/V2 拷贝路径均
// 退化为空），kernel 无需该参数。分摊拷贝参数（copyOffset/chunkSize/chunkLoopNum/
// tailSize/chunkGo/tailGo）已由 dispatch 按 axisId 选档（D0/D1 组）。
void LoadNhr1DArgs(
    KfcAllGatherNhr1DContext& ctx, ccu::Variable input, ccu::Variable output, ccu::Variable token,
    ccu::Variable die0Size, ccu::Variable die1Size, ccu::Variable outputSliceStride, ccu::Variable copyOffset,
    ccu::Variable copyChunkSize, ccu::Variable copyChunkLoopNum, ccu::Variable copyTailSize, ccu::Variable chunkGoSize0,
    ccu::Variable chunkGoSize1, ccu::Variable chunkGoSize2, ccu::Variable chunkGoSize3, ccu::Variable tailGoSize0,
    ccu::Variable tailGoSize1, ccu::Variable tailGoSize2, ccu::Variable tailGoSize3, uint32_t axisId)
{
    const uint32_t localIdx = ctx.channelCount;
    ctx.input = input;
    ctx.output[localIdx] = output;
    ctx.token[localIdx] = token;
    // D4 双 die（hccl axisId 语义）：axis0 处理 [0, die0Size)、axis1 处理 [die0Size, +die1Size)。
    // die0Size/die0LastSize 字段复用为"本 die 档"（下游 IssueSendSlice/GroupCopy 档位选择零改动）；
    // axis1 将 die0Size 基偏移烘焙进 input/output[localIdx]——直发/转发源、拷贝源、以及 PreSync
    // 交换给对端同 die 内核的输出基均随之带偏移（对端 die1 内核写本端 [die0Size,+die1Size) 半区）。
    if (axisId == 0U) {
        ctx.die0Size = die0Size;
        ctx.die0LastSize = die0Size;
    } else {
        ctx.die0Size = die1Size;
        ctx.die0LastSize = die1Size;
        ctx.input += die0Size;
        ctx.output[localIdx] += die0Size;
    }
    ctx.repeatNum = UINT64_MAX - 1ULL;
    ctx.inputSliceStride = 0;
    ctx.outputSliceStride = outputSliceStride;
    ctx.inputRepeatStride = 0;
    ctx.outputRepeatStride = 0;
    // 分摊拷贝：基址取烘焙 die 偏移前的原始参数值 + copyOffset（拷贝区与 die 半区解耦，
    // axis0 的 copyOffset=0）；own slot 偏移待 outputSliceOffset 就绪后在
    // DoRepeatAllGatherNHR 折入 copyDstBase
    ctx.copySrcBase = input;
    ctx.copySrcBase += copyOffset;
    ctx.copyDstBase = output;
    ctx.copyDstBase += copyOffset;
    ctx.copyChunkSize = copyChunkSize;
    ctx.copyChunkLoopNum = copyChunkLoopNum;
    ctx.copyTailSize = copyTailSize;
    ctx.copyCur = 0;
    ctx.chunkGoSize.addrOffset = chunkGoSize0;
    ctx.chunkGoSize.loopParam = chunkGoSize1;
    ctx.chunkGoSize.parallelParam = chunkGoSize2;
    ctx.chunkGoSize.residual = chunkGoSize3;
    ctx.tailGoSize.addrOffset = tailGoSize0;
    ctx.tailGoSize.loopParam = tailGoSize1;
    ctx.tailGoSize.parallelParam = tailGoSize2;
    ctx.tailGoSize.residual = tailGoSize3;
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
    CCU_IF(sliceSize != 0) { CCU_CHK_RET(ccu::Write(sendChannel, dst, src, sliceSize, ctx.localEvent, signalMask)); }
    CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.localEvent, signalMask)); }
    return CCU_SUCCESS;
}

// 对应 hccl DoRepeatSendRecvSlices（串行回退路径；sliceSize 取"本 die 档"——D4 经 LoadNhr1DArgs
// 复用 die0Size/die0LastSize 字段实现，轴语义见其注释）。
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

// 分摊拷贝单块发射（对应 hccl DoAllGatherGroupCopy 的本地 own 分片拷贝，按块拆分）：
// 发射前重算绝对地址——V1 GroupCopy 内部会就地推进 src/dst 地址寄存器（Variable 传值
// 句柄共享），基址寄存器 copySrcBase/copyDstBase/copyCur 独立保存不受污染。
// 拷贝引擎按芯片版本选择（mesh1d AG kernel 先例）：Ascend950(CCU_V1) 走 GroupCopy——
// 满块 goSize 恒命中单 LoopGroup（chunkSize 为 loopSize 整数倍：m≥1、n=p=0），尾块
// goSize 全零时（in-place/无尾块）内部两个 CCU_IF 块均不触发；Ascend960(CCU_V2) 走
// 单跳 LocalCopy——len=0 时经 CCU_IF 跳过，专用 copyEvent 即发即等（mesh1d 非 group
// 路径同式），免与 slice 事件位预算冲突。
CcuResult IssueSpreadCopyChunkAt(
    KfcAllGatherNhr1DContext& ctx, const GroupOpSizeVars& goSize, const ccu::Variable& chunkLen)
{
    ctx.copySrcMem.addr = ctx.copySrcBase;
    ctx.copySrcMem.addr += ctx.copyCur;
    ctx.copyDstMem.addr = ctx.copyDstBase;
    ctx.copyDstMem.addr += ctx.copyCur;
    if (ctx.ccuVersion == CcuVersion::CCU_V1) {
        CCU_CHK_RET(GroupCopy(ctx, ctx.copyDstMem, ctx.copySrcMem, goSize));
    } else {
        CCU_IF(chunkLen != 0)
        {
            CCU_CHK_RET(ccu::LocalCopy(ctx.copyDstMem, ctx.copySrcMem, chunkLen, ctx.copyEvent, 1U));
            CCU_CHK_RET(ccu::EventWait(ctx.copyEvent, 1U));
        }
    }
    ctx.copyCur += ctx.copyChunkSize;
    return CCU_SUCCESS;
}

// 非末步：尚有剩余块（哨兵未满）时发射一块——拷贝引擎占用指令流期间，本步已在途的
// Write 并发推进，拷贝摊入全轮写阴影（原实现整块堆末步，大数据量下拷贝超出写掩盖窗口，
// 直接顶到关键路径）。
CcuResult IssueSpreadCopyStepChunk(KfcAllGatherNhr1DContext& ctx)
{
    CCU_IF(ctx.copyChunkLoopNum != UINT64_MAX)
    {
        ctx.copyChunkLoopNum += ctx.constVar1;
        CCU_CHK_RET(IssueSpreadCopyChunkAt(ctx, ctx.chunkGoSize, ctx.copyChunkSize));
    }
    return CCU_SUCCESS;
}

// 末步/空步兜底：收尾全部剩余块 + 尾块。流序先于 turn-end Store，AIV 等齐双 mission
// turn-end 时整个 own slot 拷贝已落位（双 die 两轴拷贝区不相交、并集为全分片）。
CcuResult DrainSpreadCopy(KfcAllGatherNhr1DContext& ctx)
{
    CCU_WHILE(ctx.copyChunkLoopNum != UINT64_MAX)
    {
        ctx.copyChunkLoopNum += ctx.constVar1;
        CCU_CHK_RET(IssueSpreadCopyChunkAt(ctx, ctx.chunkGoSize, ctx.copyChunkSize));
    }
    CCU_CHK_RET(IssueSpreadCopyChunkAt(ctx, ctx.tailGoSize, ctx.copyTailSize));
    return CCU_SUCCESS;
}

// 对应 hccl DoRepeatAllGatherNHRSingleStep（末步跳过步间同步与 hccl 一致）。
// own slot 在各步均由 input 直发——txSliceIdxs 每步以 own slot 开头
// （见 ccu_temp_kfc_all_gather_nhr_1d_mem2mem.cc GetStepInfo），若走 output 转发会依赖
// 本地拷贝先落位；直发后本地拷贝与各步远端写无数据依赖（拷贝只写 own slot，转发只读
// 其他 slot），按步分摊发射（IssueSpreadCopyStepChunk），末步 DrainSpreadCopy 收尾。
// 性能优化：在途 slice 数不超 16 位事件预算时走步内流水——本轮所有 slice 先全部
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
    const bool isLastStep = (nhrStepInfo.step + 1 == ctx.stepInfoVector->size());

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
                // own slot 由 input 直发（D1）：不依赖本地拷贝落位，拷贝方可按步分摊
                if (nhrStepInfo.step == 0 || sendSliceIdx == ctx.rankId) {
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
            // 分摊拷贝：非末步每步发射一块（拷贝引擎占用指令流期间本批在途 Write 并发
            // 推进，拷贝摊入全轮写阴影）；末步收尾剩余块 + 尾块，先于 EventWait/步同步流序完成
            if (isLastStep) {
                CCU_CHK_RET(DrainSpreadCopy(ctx));
            } else {
                CCU_CHK_RET(IssueSpreadCopyStepChunk(ctx));
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
            if (nhrStepInfo.step == 0 || sendSliceIdx == ctx.rankId) {
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
        // 串行写逐笔等待、已无在途批次，分摊拷贝按步发射仅起拆块作用（耗时等价整块），
        // 末步收尾先于步同步/PostSync 串行执行（正确性等价）
        if (isLastStep) {
            CCU_CHK_RET(DrainSpreadCopy(ctx));
        } else {
            CCU_CHK_RET(IssueSpreadCopyStepChunk(ctx));
        }
    }

    // D3：末步也做步间同步——turn-end 提前到 PostSync 前的前置条件（末步接收分片的到达
    // 保证由此承担，与 hccl 仅末步跳过同步不同）；record 先于 wait 且不阻塞，无死锁。
    CCU_CHK_RET(ccu::NotifyRecord(sendChannel, AG_NHR_CKE_IDX, AG_NHR_STEP_POST_SYNC_BIT));
    CCU_CHK_RET(ccu::NotifyWait(recvChannel, AG_NHR_CKE_IDX, AG_NHR_STEP_POST_SYNC_BIT));

    return CCU_SUCCESS;
}

// 对应 hccl DoRepeatAllGatherNHR（axisId==1 的 die0 偏移经 LoadNhr1DArgs 烘焙进 input/output 基址，
// 本函数零改动——D4）。
// 本地拷贝按步分摊发射（见 DoRepeatAllGatherNHRSingleStep / IssueSpreadCopyStepChunk），
// 本 rank 贡献在各步均由 input 直发，step 循环前不再需要先落位。
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
    // 分摊拷贝基址收尾：own slot 偏移折入目的基址（拷贝区偏移已在 LoadNhr1DArgs 折入）
    ctx.copyDstBase += ctx.outputSliceOffset[ctx.rankId];
    ctx.copySrcMem.token = ctx.token[localIdx];
    ctx.copyDstMem.token = ctx.token[localIdx];

    if (ctx.stepInfoVector->empty()) {
        // 空步骤向量（防御）：退化为仅本地拷贝（一次性收尾全部块 + 尾块）
        CCU_CHK_RET(DrainSpreadCopy(ctx));
        return CCU_SUCCESS;
    }

    for (const KfcNhrStepInfo& nhrStepInfo : *ctx.stepInfoVector) {
        CCU_CHK_RET(DoRepeatAllGatherNHRSingleStep(ctx, nhrStepInfo));
    }

    return CCU_SUCCESS;
}
} // namespace

CcuResult CcuKernelKfcAllGatherNHR1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable die0Size,
    ccu::Variable die1Size, ccu::Variable outputSliceStride, ccu::Variable copyOffset, ccu::Variable copyChunkSize,
    ccu::Variable copyChunkLoopNum, ccu::Variable copyTailSize, ccu::Variable chunkGoSize0, ccu::Variable chunkGoSize1,
    ccu::Variable chunkGoSize2, ccu::Variable chunkGoSize3, ccu::Variable tailGoSize0, ccu::Variable tailGoSize1,
    ccu::Variable tailGoSize2, ccu::Variable tailGoSize3, const ChannelHandle channels[], uint32_t channelCount,
    uint32_t rankSize, uint32_t rankId, uint32_t axisId, const std::vector<KfcNhrStepInfo>& stepInfoVector,
    const std::map<uint32_t, uint32_t>& rank2ChannelIdx, ccu::Variable turnEndAddr)
{
    KfcAllGatherNhr1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.stepInfoVector = &stepInfoVector;
    ctx.rank2ChannelIdx = &rank2ChannelIdx;
    // 按芯片版本选择本地拷贝方式（mesh1d AG kernel 先例）：Ascend950(CCU_V1) 使用
    // GroupCopy，Ascend960(CCU_V2) 使用单跳 LocalCopy
    ctx.ccuVersion = GetCcuVersion();
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));

    LoadNhr1DArgs(
        ctx, inputAddr, outputAddr, tokenInfo, die0Size, die1Size, outputSliceStride, copyOffset, copyChunkSize,
        copyChunkLoopNum, copyTailSize, chunkGoSize0, chunkGoSize1, chunkGoSize2, chunkGoSize3, tailGoSize0,
        tailGoSize1, tailGoSize2, tailGoSize3, axisId);

    CCU_CHK_RET(PreSync(ctx));

    CCU_CHK_RET(DoRepeatAllGatherNHR(ctx));

    // D3：末步同步完成后本 mission 半区输出已完整（各步接收分片到达 + 分摊拷贝全块流序落位 +
    // 本 rank 写 EventWait；双 die 两 mission 数据不相交，AIV 等齐两个 turn-end 即全量完整），
    // 在 PostSync 前写 turn-end，AIV 提前返回；PostSync 仍在 server 指令流中先于下一轮
    // PreSync 执行，跨轮 XN 保护不变。
    CCU_CHK_RET(ccu::Store(turnEndAddr, ctx.constVar1));

    CCU_CHK_RET(PostSync(ctx));

    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
