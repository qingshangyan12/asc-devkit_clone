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

// ==== 常量：命名/取值对齐 hccl ccu_kernel_all_to_all_v_mesh1d.cc ====
[[maybe_unused]] constexpr int INPUT_XN_ID = 0; // mesh1d 窗口布局：0=INPUT（0305 单发未用，仅备注布局）
constexpr int OUTPUT_XN_ID = 1; //                  1=OUTPUT
constexpr int TOKEN_XN_ID  = 2; //                  2=TOKEN
constexpr int CKE_IDX_0    = 0; // pre  前同步
constexpr int CKE_IDX_1    = 1; // post 后同步
constexpr int CONST_ONE    = 1;

// 0305 ext 内存布局 CCUMsgExt{sendSize, loopNum, sendOffset, recvOffset}，每 rank 4 个 uint64。
constexpr uint32_t EXT_FIELD_NUM  = 4;
constexpr uint32_t EXT_SIZE_IDX   = 0; // sendSize
constexpr uint32_t EXT_SEND_IDX   = 2; // sendOffset
constexpr uint32_t EXT_RECV_IDX   = 3; // recvOffset

// ---------------------------------------------------------------------------
// InitResource：参考 hccl InitResource——取每个对端的 OUTPUT/TOKEN 远端 Variable
//               （0305 单发不取对端 INPUT 窗，src 恒用本卡 input）
// ---------------------------------------------------------------------------
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

    // 本端：直接用入参地址。src 恒为“本卡 input”，故只存一份 ctx.input，不取对端 INPUT 窗
    // （hccl 也未真正使用对端 input 窗，此处不抄其冗余）。
    ctx.input = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;

    // 只取对端的 OUTPUT / TOKEN 远端 Variable（数据要写到对端 output、用对端 token 寻址）。
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

// ---------------------------------------------------------------------------
// LoadArgs：对齐 hccl LoadArgs——恢复本卡对所有 rank 的收发信息 sendRecvInfo
// 0305 从 extAddr 一次 Load rankSize*4 个字段（CCUMsgExt 数组）
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// PreSync：对齐 hccl PreSync——把本卡 output 中为每个对端预留的接收基址 + token
//          交换到对端窗口，notify 到 CKE_0（mask 用 XN 窗位），再逐 channel 等齐
// ---------------------------------------------------------------------------
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
        // ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllVMesh1D] PreSync end");
    return CCU_SUCCESS;
}

// ---------------------------------------------------------------------------
// CalcGroupSrcDst：对齐 hccl CalcGroupSrcDst——算每个 rank 的 src / dst / myDst
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// DoAll2AllV：MC2 单发场景——每个 rank 单次搬完（对齐 hccl loadFromMem 分支的语义，
//             去掉了非 MC2 才需要的多轮分块 CCU_WHILE）
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// PostSync：对齐 hccl PostSync——逐 channel 互发/互等 CKE_1，mask 固定 1<<CONST_ONE
// ---------------------------------------------------------------------------
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
// 主入口 Kernel 函数（对齐 hccl 的调用顺序：InitResource → LoadArgs → PreSync
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
