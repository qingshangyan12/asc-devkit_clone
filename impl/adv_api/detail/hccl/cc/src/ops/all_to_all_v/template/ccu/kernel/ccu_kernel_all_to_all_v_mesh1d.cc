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

[[maybe_unused]] constexpr int INPUT_XN_ID = 0; // mesh1d 窗口布局：0=INPUT
constexpr int OUTPUT_XN_ID = 1; //                  1=OUTPUT
constexpr int TOKEN_XN_ID  = 2; //                  2=TOKEN
constexpr int CKE_IDX_0 = 0; // 前同步：output 通知
constexpr int CKE_IDX_1  = 1; // 前同步：token 通知
constexpr int CONST_ONE          = 1;

// ext 内存布局 CCUMsgExt{sendSize, loopNum, sendOffset, recvOffset}，每 rank 4 个 uint64。
constexpr uint32_t EXT_FIELD_NUM  = 4;
constexpr uint32_t EXT_SIZE_IDX   = 0; // sendSize
constexpr uint32_t EXT_SEND_IDX   = 2; // sendOffset
constexpr uint32_t EXT_RECV_IDX   = 3; // recvOffset

static CcuResult InitResource(AlltoAllVMesh1DContext &ctx, ccu::Variable inputAddr,
                              ccu::Variable outputAddr, ccu::Variable tokenInfo)
{
    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuKernelAlltoAllVMesh1D] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    ctx.src.resize(ctx.rankSize);
    ctx.dst.resize(ctx.rankSize);

    ctx.input = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;

    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], OUTPUT_XN_ID);
        ctx.token[peerId]  = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
        channelIdx++;
    }
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(AlltoAllVMesh1DContext &ctx)
{
    const uint32_t extArgNum = ctx.rankSize * EXT_FIELD_NUM;
    ccu::Array<ccu::Variable> extArgs(extArgNum);
    ccu::Load(ctx.extAddr, extArgs, extArgNum);

    ctx.sendRecvInfo.resize(ctx.rankSize);
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        const uint32_t base = peerId * EXT_FIELD_NUM;
        // 跳过 CCUMsgExt.loopNum(idx=1)，0305 单发不需要
        ctx.sendRecvInfo[peerId].sendSize   = extArgs[base + EXT_SIZE_IDX];
        ctx.sendRecvInfo[peerId].sendOffset = extArgs[base + EXT_SEND_IDX];
        ctx.sendRecvInfo[peerId].recvOffset = extArgs[base + EXT_RECV_IDX];
    }
    return CCU_SUCCESS;
}

static CcuResult PreSync(AlltoAllVMesh1DContext &ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PreSync begain!");
    ccu::Variable tempDst;

    uint32_t channelIdx = 0;
    for (uint32_t id = 0; id < ctx.rankSize; id++) {
        if (id == ctx.rankId) {
            continue;
        }
        tempDst = ctx.output[ctx.rankId];
        tempDst += ctx.sendRecvInfo[id].recvOffset;
        // index = 0，传递 output 信息
        ccu::WriteVariableWithNotify(ctx.channels[channelIdx], tempDst, OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID);
        // index = 1，传递 token 信息
        ccu::WriteVariableWithNotify(ctx.channels[channelIdx], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID);
        channelIdx++;
    }

    uint16_t allBit = 1 << OUTPUT_XN_ID | 1 << TOKEN_XN_ID;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PreSync end");
    return CCU_SUCCESS;
}

static void CalcGroupSrcDst(AlltoAllVMesh1DContext &ctx)
{
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        // src[rankIdx] = 本卡 input + 发往该 rank 的 sendOffset + srcOffset
        ctx.src[rankIdx].token = ctx.token[rankIdx];
        ctx.src[rankIdx].addr = ctx.input;
        ctx.src[rankIdx].addr += ctx.sendRecvInfo[rankIdx].sendOffset;
        ctx.src[rankIdx].addr += ctx.srcOffset;

        if (rankIdx == ctx.rankId) {
            // 本端接收：本卡 output + 本卡 recvOffset + dstOffset
            ctx.myDst.token = ctx.token[rankIdx];
            ctx.myDst.addr = ctx.output[rankIdx];
            ctx.myDst.addr += ctx.sendRecvInfo[rankIdx].recvOffset;
            ctx.myDst.addr += ctx.dstOffset;
        } else {
            // 对端接收：PreSync 交换来的接收基址(已含 recvOffset) + dstOffset
            ctx.dst[rankIdx].token = ctx.token[rankIdx];
            ctx.dst[rankIdx].addr = ctx.output[rankIdx];
            ctx.dst[rankIdx].addr += ctx.dstOffset;
        }
    }
}

static CcuResult DoAll2AllV(AlltoAllVMesh1DContext &ctx)
{
    uint32_t channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        CCU_IF(ctx.sendRecvInfo[rankIdx].sendSize != 0) {
            if (rankIdx == ctx.rankId) {
                ccu::LocalCopy(ctx.myDst, ctx.src[rankIdx], ctx.sendRecvInfo[rankIdx].sendSize, ctx.event, 1);
                ccu::EventWait(ctx.event, 1);
            } else {
                ccu::Write(ctx.channels[channelIdx], ctx.dst[rankIdx], ctx.src[rankIdx],
                    ctx.sendRecvInfo[rankIdx].sendSize, ctx.event, 1);
                ccu::EventWait(ctx.event, 1);
            }
        }
        if (rankIdx != ctx.rankId) {
            channelIdx++;
        }
    }
    return CCU_SUCCESS;
}

static void PostSync(AlltoAllVMesh1DContext &ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PostSync!");
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], CKE_IDX_1, 1 << CONST_ONE);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_1, 1 << CONST_ONE);
    }
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PostSync End!");
}

// ============================================================================
// 主入口 Kernel 函数（调用顺序：InitResource → LoadArgs → PreSync
//                       → CalcGroupSrcDst → DoAll2AllV → PostSync）
// ============================================================================
CcuResult CcuAlltoAllVMesh1DKernel(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                   ccu::Variable srcOffset, ccu::Variable dstOffset, ccu::Variable extAddr,
                                   ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
                                   ccu::Variable goSize4, const ChannelHandle channels[], uint32_t channelCount,
                                   uint32_t rankSize, uint32_t rankId)
{
    (void)goSize1;
    (void)goSize2;
    (void)goSize3;
    (void)goSize4;

    AlltoAllVMesh1DContext ctx;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.srcOffset = srcOffset;
    ctx.dstOffset = dstOffset;
    ctx.extAddr = extAddr;

    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] AlltoAllVMesh1D run");
    CCU_CHK_RET(InitResource(ctx, inputAddr, outputAddr, tokenInfo));
    CCU_CHK_RET(LoadArgs(ctx));
    CCU_CHK_RET(PreSync(ctx));
    CalcGroupSrcDst(ctx);
    CCU_CHK_RET(DoAll2AllV(ctx));
    PostSync(ctx);
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] AlltoAllVMesh1D end");
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
