/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_all_to_all_v_mesh1d.h"

namespace mc2_ops_hccl {
constexpr int INPUT_XN_ID = 0;
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int CKE_IDX_0 = 0; // pre
constexpr int CKE_IDX_1 = 1; // post
constexpr int CKE_IDX_2 = 2;
constexpr int CONST_ONE = 1;
constexpr int CONST_EIGHT = 8;
constexpr int CONST_NINE = 9;

static CcuResult
LoadAll2allSendRecvInfo(AlltoAllVMesh1DContext& ctx, A2AsingleSendRecvInfo& sendRecvInfo, uint16_t index)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] Load Args from Mem");
    sendRecvInfo.loopNum = UINT64_MAX - 1; // MC2 场景 loop num 默认为 1

    // 要求client端排列内存为[size,send,recv][size,send,recv]...
    ccu::Load(ctx.a2avXnAddr, sendRecvInfo.tailSize);
    ctx.a2avXnAddr += ctx.xnLength;

    // 跳过LoopNum
    ctx.a2avXnAddr += ctx.xnLength;

    ccu::Load(ctx.a2avXnAddr, sendRecvInfo.sendOffset);
    ctx.a2avXnAddr += ctx.xnLength;

    ccu::Load(ctx.a2avXnAddr, sendRecvInfo.recvOffset);
    ctx.a2avXnAddr += ctx.xnLength;
    return CCU_SUCCESS;
}

static CcuResult InitResource(AlltoAllVMesh1DContext& ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] InitResource!");
    const auto* arg = ctx.arg;
    uint32_t channelIdx = 0;
    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuKernelAlltoAllVMesh1D] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    HCCL_INFO("ctx.rankSize = %d, ctx.rankId = %d", ctx.rankSize, ctx.rankId);
    ctx.input.resize(ctx.rankSize);
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) { // 非本地，使用远端Variable
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], INPUT_XN_ID);
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], OUTPUT_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }
    HCCL_INFO("output size: %zu, token size: %zu", ctx.output.size(), ctx.token.size());

    ctx.src.resize(ctx.rankSize);
    ctx.dst.resize(ctx.rankSize);

    // 前同步。交换信息，将本Rank load的in\out等地址信息写到所有对端的对应Variable中，并同步

    //  all2allv 数据搬运
    ctx.xnLength = 8; // xn长度为8byte

    ctx.resourceAllocated = false;

    return CCU_SUCCESS;
}

static CcuResult PreSync(AlltoAllVMesh1DContext& ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PreSync begin!");
    const auto* arg = ctx.arg;
    ccu::Variable tempDst;

    u32 channelIdx = 0;
    for (u32 id = 0; id < ctx.rankSize; id++) {
        if (id == ctx.rankId) {
            continue;
        }
        tempDst = ctx.output[ctx.rankId];
        tempDst += ctx.sendRecvInfo[id].recvOffset;
        // index = 0，传递output信息
        ccu::WriteVariableWithNotify(ctx.channels[channelIdx], tempDst, OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID);
        // index = 1，传递token信息
        ccu::WriteVariableWithNotify(
            ctx.channels[channelIdx], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID);
        channelIdx++;
    }

    uint16_t allBit = 1 << OUTPUT_XN_ID | 1 << TOKEN_XN_ID;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PreSync end");

    return CCU_SUCCESS;
}

static void PostSync(AlltoAllVMesh1DContext& ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PostSync!");
    const auto* arg = ctx.arg;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], CKE_IDX_1, 1 << CONST_ONE);
    }

    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_1, 1 << CONST_ONE);
    }
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PostSync End!");
}

static CcuResult LoadArgs(AlltoAllVMesh1DContext& ctx, ccu::Variable inputAddr,
    ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable srcOffset,
    ccu::Variable dstOffset, ccu::Variable extAddr, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable goSize4)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] LoadArgs!");
    ctx.input[0] = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.srcOffset = srcOffset;
    ctx.dstOffset = dstOffset;
    ctx.a2avXnAddr = extAddr;
    ctx.sendRecvInfo.resize(ctx.rankSize);
    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        uint16_t index = CONST_NINE + CONST_EIGHT * peerId;
        CCU_CHK_RET(LoadAll2allSendRecvInfo(ctx, ctx.sendRecvInfo[peerId], index));
    }
    return CCU_SUCCESS;
}

static void CalcGroupSrcDst(AlltoAllVMesh1DContext& ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] CalcGroupSrcDst!");
    const auto* arg = ctx.arg;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        ctx.src[rankIdx].token = ctx.token[rankIdx];

        // ctx.src[rankIdx] = usrInAddr + sendoffset + srcOffset_
        ctx.src[rankIdx].addr = ctx.input[0];
        ctx.src[rankIdx].addr += ctx.sendRecvInfo[rankIdx].sendOffset;
        ctx.src[rankIdx].addr += ctx.srcOffset;

        // ctx.dst[r] = recvBuf[r] + recvOffset + ctx.dstOffset
        if (rankIdx == ctx.rankId) {
            // 写目的端为本端时需要特殊处理：使用接收基地址 + 块地址offset + 已发送数据量
            ctx.myDst.token = ctx.token[rankIdx];
            ctx.myDst.addr = ctx.output[rankIdx];
            ctx.myDst.addr += ctx.sendRecvInfo[rankIdx].recvOffset;
            ctx.myDst.addr += ctx.dstOffset;
        } else {
            // 对端交换的接收块起始地址 + 已接收的数据偏移
            ctx.dst[rankIdx].token = ctx.token[rankIdx];
            ctx.dst[rankIdx].addr = ctx.output[rankIdx];
            ctx.dst[rankIdx].addr += ctx.dstOffset;
        }
    }
}

static CcuResult DoAll2AllVMultiLoop(AlltoAllVMesh1DContext& ctx)
{
    HCCL_DEBUG("[CcuKernelAlltoAllVMesh1D] alltoallv mesh 1d use GroupCopy start");
    const auto* arg = ctx.arg;
    ctx.xnMaxTransportSize = UB_MAX_TRANS_SIZE;
    ctx.completedRankCount = 0;
    ctx.xnConst1 = 1;
    uint16_t allBit = (1 << ctx.rankSize) - 1;
    CCU_WHILE(ctx.completedRankCount != ctx.rankSize)
    {
        u32 channelId = 0;
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx == ctx.rankId) {
                continue;
            }
            CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum == UINT64_MAX)
            { // 已经完成，直接置位完成信号
                ccu::EventRecord(ctx.event, 1 << rankIdx);
            }
            CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum != UINT64_MAX)
            { // 还没有完成，则继续循环
                CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum == UINT64_MAX - 1)
                { // 最后一轮循环, 发送尾块数据
                    CCU_IF(ctx.sendRecvInfo[rankIdx].tailSize == 0)
                    { // 尾块数据量为 0，则不需要发送尾块数据
                        ccu::EventRecord(ctx.event, 1 << rankIdx);
                    }
                    CCU_IF(ctx.sendRecvInfo[rankIdx].tailSize != 0)
                    { // 尾块数据量不为 0，则需要发送尾块数据
                        ccu::Write(
                            ctx.channels[channelId], ctx.dst[rankIdx], ctx.src[rankIdx],
                            ctx.sendRecvInfo[rankIdx].tailSize, ctx.event, 1 << rankIdx);
                    }
                    ctx.completedRankCount += ctx.xnConst1; // 之后一轮循环完成，更新已完成的rank数
                }
                CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum != UINT64_MAX - 1)
                { // 未完成，则继续循环，发送整块数据
                    ccu::Write(
                        ctx.channels[channelId], ctx.dst[rankIdx], ctx.src[rankIdx], ctx.xnMaxTransportSize, ctx.event,
                        1 << rankIdx);
                    // 更新偏移
                    ctx.src[rankIdx].addr += ctx.xnMaxTransportSize;
                    ctx.dst[rankIdx].addr += ctx.xnMaxTransportSize;
                }
                ctx.sendRecvInfo[rankIdx].loopNum += ctx.xnConst1;
            }
            channelId++;
        }

        CCU_IF(ctx.sendRecvInfo[ctx.rankId].loopNum == UINT64_MAX)
        { // 已经完成，直接置位完成信号
            ccu::EventRecord(ctx.event, 1 << ctx.rankId);
        }

        CCU_IF(ctx.sendRecvInfo[ctx.rankId].loopNum != UINT64_MAX)
        { // 还没有完成，则继续循环
            CCU_IF(ctx.sendRecvInfo[ctx.rankId].loopNum == UINT64_MAX - 1)
            { // 最后一轮循环, 发送尾块数据
                CCU_IF(ctx.sendRecvInfo[ctx.rankId].tailSize == 0)
                { // 尾块数据量为 0，则不需要发送尾块数据
                    ccu::EventRecord(ctx.event, 1 << ctx.rankId);
                }
                CCU_IF(ctx.sendRecvInfo[ctx.rankId].tailSize != 0)
                { // 尾块数据量不为 0，则需要发送尾块数据
                    ccu::LocalCopy(
                        ctx.myDst, ctx.src[ctx.rankId], ctx.sendRecvInfo[ctx.rankId].tailSize, ctx.event,
                        1 << ctx.rankId);
                }
                ctx.completedRankCount += ctx.xnConst1; // 之后一轮循环完成，更新已完成的rank数
            }
            CCU_IF(ctx.sendRecvInfo[ctx.rankId].loopNum != UINT64_MAX - 1)
            { // 未完成，则继续循环，发送整块数据
                ccu::LocalCopy(
                    ctx.myDst, ctx.src[ctx.rankId], ctx.xnMaxTransportSize, ctx.event, 1 << ctx.rankId);
                // 更新偏移
                ctx.src[ctx.rankId].addr += ctx.xnMaxTransportSize;
                ctx.myDst.addr += ctx.xnMaxTransportSize;
            }
            ctx.sendRecvInfo[ctx.rankId].loopNum += ctx.xnConst1;
        }
        // 等待本轮发送完成
        ccu::EventWait(ctx.event, allBit);
    }

    return CCU_SUCCESS;
}

// ============================================================================
// 主入口 Kernel 函数
// ============================================================================
CcuResult CcuAlltoAllVMesh1DKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable srcOffset,
    ccu::Variable dstOffset, ccu::Variable extAddr, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    ccu::Variable goSize4, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId)
{
    AlltoAllVMesh1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.loadFromMem = loadFromMem;
    InitCcuKernelCtxBase(ctx);

    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] AlltoAllMesh1D run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, srcOffset, dstOffset,
                        extAddr, goSize1, goSize2, goSize3, goSize4));

    CCU_CHK_RET(PreSync(ctx));
    CalcGroupSrcDst(ctx);

    CCU_CHK_RET(DoAll2AllVMultiLoop(ctx));

    PostSync(ctx);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D end");

    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
