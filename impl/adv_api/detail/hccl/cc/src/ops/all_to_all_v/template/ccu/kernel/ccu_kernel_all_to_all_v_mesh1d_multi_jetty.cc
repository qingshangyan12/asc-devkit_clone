/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_all_to_all_v_mesh1d_multi_jetty.h"

namespace mc2_ops_hccl {
namespace {
// 通道 XN 槽位与 CKE 位（照 hccl ccu_kernel_all_to_all_v_mesh1d_multi_jetty.cc）
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
constexpr int CKE_IDX_0 = 0;

// GM ext 区相邻字段间距 8 字节（字段顺序与 AIV 侧 CCUMsgExtMultiJetty 一致，kernel 顺序读取）
constexpr uint64_t EXT_FIELD_STRIDE = 8;
// 整块搬运粒度，与 hccl UB_MAX_TRANS_SIZE 一致（256MB）
constexpr uint64_t A2AV_MJ_MAX_TRANSPORT_SIZE = 256ULL * 1024ULL * 1024ULL;

static CcuResult PreSync(AllToAllVMesh1DMultiJettyContext& ctx)
{
    uint32_t channelIdx = 0;
    for (uint32_t id = 0; id < ctx.rankSize; id++) {
        ccu::Variable tempDst;
        if (id == ctx.rankId) {
            continue;
        }
        tempDst = ctx.output[ctx.rankId];
        tempDst += ctx.sendRecvInfo[id].recvOffset;
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[channelIdx], tempDst, OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[channelIdx], ctx.token[ctx.rankId], TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID));
        channelIdx++;
    }

    uint32_t allBit = (1 << OUTPUT_XN_ID) | (1 << TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit));
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(AllToAllVMesh1DMultiJettyContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID));
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID));
    }
    return CCU_SUCCESS;
}

static CcuResult InitResource(AllToAllVMesh1DMultiJettyContext& ctx)
{
    uint32_t channelIdx = 0;

    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuAllToAllVMesh1DMultiJetty] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }
    HCCL_INFO("[CcuAllToAllVMesh1DMultiJetty] channels.size: [%u]", ctx.channelCount);

    ctx.input.resize(1);
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], OUTPUT_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }

    ctx.src.resize(ctx.rankSize);
    ctx.remoteDst.resize(ctx.rankSize);
    ctx.eventList.resize(ctx.rankSize);

    return CCU_SUCCESS;
}

static CcuResult LoadSendRecvInfo(AllToAllVMesh1DMultiJettyContext& ctx, const ccu::Variable& extAddr)
{
    ctx.sendRecvInfo.resize(ctx.rankSize);
    ccu::Variable extCursor;
    ccu::Variable fieldStride;
    extCursor = extAddr;
    fieldStride = EXT_FIELD_STRIDE;
    for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].sliceSize));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].tailSliceSize));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].lastSliceSize));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].lastTailSliceSize));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].loopNum));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].sendOffset));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].recvOffset));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].tailGoSize.addrOffset));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].tailGoSize.loopParam));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].tailGoSize.parallelParam));
        extCursor += fieldStride;
        CCU_CHK_RET(ccu::Load(extCursor, ctx.sendRecvInfo[rankIdx].tailGoSize.residual));
        extCursor += fieldStride;
    }
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(
    AllToAllVMesh1DMultiJettyContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable srcOffset, ccu::Variable dstOffset, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, ccu::Variable goSize4, ccu::Variable extAddr)
{
    ctx.input[0] = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.srcOffset = srcOffset;
    ctx.dstOffset = dstOffset;
    ctx.xnMaxTransportGoSize.addrOffset = goSize1;
    ctx.xnMaxTransportGoSize.loopParam = goSize2;
    ctx.xnMaxTransportGoSize.parallelParam = goSize3;
    ctx.xnMaxTransportGoSize.residual = goSize4;
    return LoadSendRecvInfo(ctx, extAddr);
}

static CcuResult CalcGroupSrcDst(AllToAllVMesh1DMultiJettyContext& ctx)
{
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        ctx.src[rankIdx].token = ctx.token[rankIdx];
        ctx.src[rankIdx].addr = ctx.input[0];
        ctx.src[rankIdx].addr += ctx.sendRecvInfo[rankIdx].sendOffset;
        ctx.src[rankIdx].addr += ctx.srcOffset;

        if (rankIdx == ctx.rankId) {
            ctx.myDst.token = ctx.token[rankIdx];
            ctx.myDst.addr = ctx.output[rankIdx];
            ctx.myDst.addr += ctx.sendRecvInfo[rankIdx].recvOffset;
            ctx.myDst.addr += ctx.dstOffset;
        } else {
            ctx.remoteDst[rankIdx].token = ctx.token[rankIdx];
            ctx.remoteDst[rankIdx].addr = ctx.output[rankIdx];
            ctx.remoteDst[rankIdx].addr += ctx.dstOffset;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult DoAll2AllVLastBlock(AllToAllVMesh1DMultiJettyContext& ctx, uint32_t rankIdx, uint32_t channelIdx)
{
    for (uint32_t i = 0; i < ctx.jettyNums[rankIdx]; i++) {
        CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum != UINT64_MAX - 1)
        {
            CCU_IF(ctx.sendRecvInfo[rankIdx].lastSliceSize == 0)
            {
                CCU_CHK_RET(ccu::EventRecord(ctx.eventList[rankIdx], 1 << i));
            }
            CCU_ELSE
            {
                ccu::Write(
                    ctx.channels[channelIdx], ctx.remoteDst[rankIdx], ctx.src[rankIdx],
                    ctx.sendRecvInfo[rankIdx].lastSliceSize, ctx.eventList[rankIdx], 1 << i);
                ctx.src[rankIdx].addr += ctx.sendRecvInfo[rankIdx].lastSliceSize;
                ctx.remoteDst[rankIdx].addr += ctx.sendRecvInfo[rankIdx].lastSliceSize;
            }
        }
        CCU_ELSE
        {
            CCU_IF(ctx.sendRecvInfo[rankIdx].lastTailSliceSize == 0)
            {
                CCU_CHK_RET(ccu::EventRecord(ctx.eventList[rankIdx], 1 << i));
            }
            CCU_ELSE
            {
                ccu::Write(
                    ctx.channels[channelIdx], ctx.remoteDst[rankIdx], ctx.src[rankIdx],
                    ctx.sendRecvInfo[rankIdx].lastTailSliceSize, ctx.eventList[rankIdx], 1 << i);
            }
            ctx.completedRankCount += ctx.xnConst1;
        }
        ctx.sendRecvInfo[rankIdx].loopNum += ctx.xnConst1;
    }
    return CCU_SUCCESS;
}

static CcuResult DoAll2AllVBlock(AllToAllVMesh1DMultiJettyContext& ctx, uint32_t rankIdx, uint32_t channelIdx)
{
    for (uint32_t i = 0; i < ctx.jettyNums[rankIdx]; i++) {
        if (i != ctx.jettyNums[rankIdx] - 1) {
            CCU_IF(ctx.sendRecvInfo[rankIdx].sliceSize == 0)
            {
                CCU_CHK_RET(ccu::EventRecord(ctx.eventList[rankIdx], 1 << i));
            }
            CCU_ELSE
            {
                ccu::Write(
                    ctx.channels[channelIdx], ctx.remoteDst[rankIdx], ctx.src[rankIdx],
                    ctx.sendRecvInfo[rankIdx].sliceSize, ctx.eventList[rankIdx], 1 << i);
                ctx.src[rankIdx].addr += ctx.sendRecvInfo[rankIdx].sliceSize;
                ctx.remoteDst[rankIdx].addr += ctx.sendRecvInfo[rankIdx].sliceSize;
            }
        } else {
            CCU_IF(ctx.sendRecvInfo[rankIdx].tailSliceSize == 0)
            {
                CCU_CHK_RET(ccu::EventRecord(ctx.eventList[rankIdx], 1 << i));
            }
            CCU_ELSE
            {
                ccu::Write(
                    ctx.channels[channelIdx], ctx.remoteDst[rankIdx], ctx.src[rankIdx],
                    ctx.sendRecvInfo[rankIdx].tailSliceSize, ctx.eventList[rankIdx], 1 << i);
                ctx.src[rankIdx].addr += ctx.sendRecvInfo[rankIdx].tailSliceSize;
                ctx.remoteDst[rankIdx].addr += ctx.sendRecvInfo[rankIdx].tailSliceSize;
            }
        }
        ctx.sendRecvInfo[rankIdx].loopNum += ctx.xnConst1;
    }
    return CCU_SUCCESS;
}

static CcuResult DoAll2AllVMultiLoop(AllToAllVMesh1DMultiJettyContext& ctx)
{
    HCCL_DEBUG("[CcuAllToAllVMesh1DMultiJetty] alltoallv mesh 1d use GroupCopy start");
    ctx.xnMaxTransportSize = A2AV_MJ_MAX_TRANSPORT_SIZE;
    ctx.completedRankCount = 0;
    ctx.xnConst1 = 1;
    CCU_WHILE(ctx.completedRankCount != static_cast<uint64_t>(ctx.rankSize))
    {
        uint32_t channelIdx = 0;
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx == ctx.rankId) {
                continue;
            }
            CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum == UINT64_MAX)
            {
                CCU_CHK_RET(ccu::EventRecord(ctx.eventList[rankIdx], (1 << ctx.jettyNums[rankIdx]) - 1));
            }
            CCU_ELSE
            {
                CCU_IF(ctx.sendRecvInfo[rankIdx].loopNum != UINT64_MAX - ctx.jettyNums[rankIdx])
                {
                    CCU_CHK_RET(DoAll2AllVBlock(ctx, rankIdx, channelIdx));
                }
                CCU_ELSE { CCU_CHK_RET(DoAll2AllVLastBlock(ctx, rankIdx, channelIdx)); }
            }
            channelIdx++;
        }
        CCU_IF(ctx.sendRecvInfo[ctx.rankId].loopNum == UINT64_MAX)
        {
            CCU_CHK_RET(ccu::EventRecord(ctx.eventList[ctx.rankId], (1 << ctx.jettyNums[ctx.rankId]) - 1));
        }
        CCU_ELSE
        {
            CCU_IF(ctx.sendRecvInfo[ctx.rankId].loopNum == UINT64_MAX - ctx.jettyNums[ctx.rankId])
            {
                CCU_IF(ctx.sendRecvInfo[ctx.rankId].lastTailSliceSize == 0)
                {
                    CCU_CHK_RET(ccu::EventRecord(ctx.eventList[ctx.rankId], (1 << ctx.jettyNums[ctx.rankId]) - 1));
                }
                CCU_ELSE
                {
                    CCU_CHK_RET(GroupCopy(
                        ctx, ctx.myDst, ctx.src[ctx.rankId], ctx.sendRecvInfo[ctx.rankId].tailGoSize, GetCcuVersion()));
                    CCU_CHK_RET(ccu::EventRecord(ctx.eventList[ctx.rankId], (1 << ctx.jettyNums[ctx.rankId]) - 1));
                }
                ctx.completedRankCount += ctx.xnConst1;
            }
            CCU_ELSE
            {
                CCU_IF(ctx.sendRecvInfo[ctx.rankId].tailSliceSize == 0)
                {
                    CCU_CHK_RET(ccu::EventRecord(ctx.eventList[ctx.rankId], (1 << ctx.jettyNums[ctx.rankId]) - 1));
                }
                CCU_ELSE
                {
                    CCU_CHK_RET(
                        GroupCopy(ctx, ctx.myDst, ctx.src[ctx.rankId], ctx.xnMaxTransportGoSize, GetCcuVersion()));
                    CCU_CHK_RET(ccu::EventRecord(ctx.eventList[ctx.rankId], (1 << ctx.jettyNums[ctx.rankId]) - 1));
                    ctx.src[ctx.rankId].addr += ctx.xnMaxTransportSize;
                    ctx.myDst.addr += ctx.xnMaxTransportSize;
                }
            }
            ctx.sendRecvInfo[ctx.rankId].loopNum += ctx.xnConst1;
        }
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            CCU_CHK_RET(ccu::EventWait(ctx.eventList[rankIdx], (1 << ctx.jettyNums[rankIdx]) - 1));
        }
    }
    return CCU_SUCCESS;
}
} // namespace

CcuResult CcuAllToAllVMesh1DMultiJettyKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable srcOffset,
    ccu::Variable dstOffset, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable goSize4,
    ccu::Variable extAddr, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const uint32_t jettyNums[])
{
    HCCL_INFO("[AllToAllVAlgo] AllToAllVMesh1DMultiJetty run");
    AllToAllVMesh1DMultiJettyContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.jettyNums = jettyNums;
    InitCcuKernelCtxBase(ctx);

    HCCL_INFO("[CcuAllToAllVMesh1DMultiJetty] Init, KernelArgs are rankId[%u], rankSize[%u]", rankId, rankSize);

    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(
        ctx, inputAddr, outputAddr, tokenInfo, srcOffset, dstOffset, goSize1, goSize2, goSize3, goSize4, extAddr));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(CalcGroupSrcDst(ctx));
    CCU_CHK_RET(DoAll2AllVMultiLoop(ctx));
    CCU_CHK_RET(PostSync(ctx));

    HCCL_INFO("[CcuAllToAllVMesh1DMultiJetty] AllToAllVMesh1DMultiJetty end");
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
