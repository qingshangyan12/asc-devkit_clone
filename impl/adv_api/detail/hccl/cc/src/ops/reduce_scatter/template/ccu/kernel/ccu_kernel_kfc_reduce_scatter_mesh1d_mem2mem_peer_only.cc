/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem_peer_only.h"
#include "../ccu_temp_kfc_reduce_scatter_mesh_1D_mem2mem.h"
#include "ccu_func_dl.hpp"
#include "kfc_server_protocol.h"
#include "mc2_type.h"

namespace mc2_ops_hccl {
using namespace hcomm;

constexpr int PEER_ONLY_INPUT_XN_ID = 0;
constexpr int PEER_ONLY_TOKEN_XN_ID = 2;
constexpr int PEER_ONLY_POST_SYNC_ID = 3;
constexpr int PEER_ONLY_CKE_IDX = 0;

struct KfcReduceScatterMesh1DMem2MemPeerOnlyContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};
    HcclDataType dataType;
    HcclReduceOp reduceOp;
    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> token;
    ccu::Variable output;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable skipLocalRankCopy;
    ccu::Variable tailSize;
    ccu::Variable chunkLoopNum;
    ccu::Event event;
};

static CcuResult InitPeerOnlyResource(KfcReduceScatterMesh1DMem2MemPeerOnlyContext& ctx)
{
    if (ctx.rankSize != 2U || ctx.rankId >= ctx.rankSize || ctx.channelCount != 1U) {
        HCCL_ERROR(
            "[CcuSchedReduceScatterSoleMeshPeerOnly] requires TP2 and one peer channel, rankSize[%u], "
            "rankId[%u], channelCount[%u].",
            ctx.rankSize, ctx.rankId, ctx.channelCount);
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    const uint32_t peerRank = ctx.rankId ^ 1U;
    ctx.input[peerRank] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[0], PEER_ONLY_INPUT_XN_ID);
    ctx.token[peerRank] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[0], PEER_ONLY_TOKEN_XN_ID);
    return CCU_SUCCESS;
}

static CcuResult LoadPeerOnlyArgs(
    KfcReduceScatterMesh1DMem2MemPeerOnlyContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable currentRankSliceInputOffset, ccu::Variable skipLocalRankCopy,
    ccu::Variable tailSize, ccu::Variable chunkLoopNum)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.output = outputAddr;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.skipLocalRankCopy = skipLocalRankCopy;
    ctx.tailSize = tailSize;
    ctx.chunkLoopNum = chunkLoopNum;
    return CCU_SUCCESS;
}

static CcuResult PeerOnlyPreSync(KfcReduceScatterMesh1DMem2MemPeerOnlyContext& ctx)
{
    CCU_CHK_RET(ccu::WriteVariableWithNotify(
        ctx.channels[0], ctx.input[ctx.rankId], PEER_ONLY_INPUT_XN_ID, PEER_ONLY_CKE_IDX, 1U << PEER_ONLY_INPUT_XN_ID));
    CCU_CHK_RET(ccu::WriteVariableWithNotify(
        ctx.channels[0], ctx.token[ctx.rankId], PEER_ONLY_TOKEN_XN_ID, PEER_ONLY_CKE_IDX, 1U << PEER_ONLY_TOKEN_XN_ID));
    constexpr uint32_t allBit = (1U << PEER_ONLY_INPUT_XN_ID) | (1U << PEER_ONLY_TOKEN_XN_ID);
    ccu::NotifyWait(ctx.channels[0], PEER_ONLY_CKE_IDX, allBit);
    return CCU_SUCCESS;
}

static CcuResult PeerOnlyPostSync(KfcReduceScatterMesh1DMem2MemPeerOnlyContext& ctx)
{
    ccu::NotifyRecord(ctx.channels[0], PEER_ONLY_CKE_IDX, 1U << PEER_ONLY_POST_SYNC_ID);
    ccu::NotifyWait(ctx.channels[0], PEER_ONLY_CKE_IDX, 1U << PEER_ONLY_POST_SYNC_ID);
    return CCU_SUCCESS;
}

static CcuResult DoPeerOnlyReduceScatter(KfcReduceScatterMesh1DMem2MemPeerOnlyContext& ctx)
{
    const uint32_t peerRank = ctx.rankId ^ 1U;
    ccu::RemoteAddr remoteInput;
    remoteInput.addr = ctx.input[peerRank];
    remoteInput.addr += ctx.currentRankSliceInputOffset;
    remoteInput.token = ctx.token[peerRank];

    ccu::LocalAddr output;
    output.addr = ctx.output;
    output.token = ctx.token[ctx.rankId];

    ccu::LocalAddr localInput;
    localInput.addr = ctx.input[ctx.rankId];
    localInput.addr += ctx.currentRankSliceInputOffset;
    localInput.token = ctx.token[ctx.rankId];

    ccu::Variable chunkSize{};
    chunkSize = UB_MAX_TRANS_SIZE;
    ccu::Variable chunkLoopNumAdd{};
    chunkLoopNumAdd = 1;
    CCU_WHILE(ctx.chunkLoopNum != UINT64_MAX)
    {
        CCU_IF(ctx.chunkLoopNum == UINT64_MAX - 1) { chunkSize = ctx.tailSize; }
        ctx.chunkLoopNum += chunkLoopNumAdd;
        CCU_IF(chunkSize != 0)
        {
            // MMRS skips the copy only after Matmul has supplied the local contribution.
            CCU_IF(ctx.skipLocalRankCopy == 0)
            {
                CCU_CHK_RET(ccu::LocalCopy(output, localInput, chunkSize, ctx.event, 1U << ctx.rankId));
                ccu::EventWait(ctx.event, 1U << ctx.rankId);
            }
            ccu::ReadReduce(
                ctx.channels[0], output, remoteInput, chunkSize, ctx.dataType, ctx.reduceOp, ctx.event, 1U << peerRank);
            ccu::EventWait(ctx.event, 1U << peerRank);
        }
        remoteInput.addr += chunkSize;
        localInput.addr += chunkSize;
        output.addr += chunkSize;
    }
    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMeshMem2Mem1DPeerOnlyKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgKfcReduceScatterMesh1DMem2Mem*>(arg);
    KfcReduceScatterMesh1DMem2MemPeerOnlyContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = static_cast<uint32_t>(kernelArg->rankSize);
    ctx.rankId = kernelArg->rankId;
    ctx.dataType = kernelArg->opParam.DataDes.dataType;
    ctx.reduceOp = kernelArg->opParam.reduceType;
    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    HCCL_INFO(
        "[CcuSchedReduceScatterSoleMeshPeerOnly] run, rankId[%u], rankSize[%u], dataType[%d], reduceOp[%d].",
        ctx.rankId, ctx.rankSize, ctx.dataType, ctx.reduceOp);
    CCU_CHK_RET(InitPeerOnlyResource(ctx));
    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadPeerOnlyArgs(ctx, args[0], args[1], args[2], args[4], args[5], args[7], args[8]);
            PeerOnlyPreSync(ctx);
            DoPeerOnlyReduceScatter(ctx);
            PeerOnlyPostSync(ctx);
            HCCL_INFO("[CcuSchedReduceScatterSoleMeshPeerOnly] end.");
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_REDUCE_SCATTER_PEER_ONLY_KERNEL_NAME);
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
