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
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_ms_accum.h"
#include "mc2_type.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// XN/CKE 槽位与 asc-devkit 侧 KFC RS 兄弟内核（mem2mem / peer_only）保持一致：INPUT=0、TOKEN=2、POST=3
constexpr int RS_MS_INPUT_XN_ID = 0;
constexpr int RS_MS_TOKEN_XN_ID = 2;
constexpr int RS_MS_POST_SYNC_ID = 3;
constexpr int RS_MS_CKE_IDX = 0;
// 与 hccl rs-sched-perf 的 MS_ACCUM_REDUCE_SCATTER_LOOP_COUNT 一致：GroupReduce 的 LoopGroup 并行度。
// 必须与宿主 CcuPrepareForReduceScatterTwoShotM2M 里 CalcGoSize(sliceSize, loopCount, ...) 的
// loopCount（16）一致，否则 goSize 与 LoopGroup 步进错位（同 CCU_LOOP_COUNT_M2M_RE 的契约）。
constexpr uint32_t RS_MS_ACCUM_REDUCE_SCATTER_LOOP_COUNT = 16;

struct KfcReduceScatterMesh1DMsAccumContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> token;
    ccu::Variable output;
    ccu::Variable localInputOffset;
    GroupOpSizeVars goSize;
};

static CcuResult InitMsAccumResource(KfcReduceScatterMesh1DMsAccumContext& ctx)
{
    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuReduceScatterMesh1DMsAccum] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], RS_MS_INPUT_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], RS_MS_TOKEN_XN_ID);
        channelIdx++;
    }
    return CCU_SUCCESS;
}

static CcuResult LoadMsAccumArgs(
    KfcReduceScatterMesh1DMsAccumContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable localInputOffset, ccu::Variable goAddrOffset, ccu::Variable goLoopParam,
    ccu::Variable goParallelParam, ccu::Variable goResidual)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.localInputOffset = localInputOffset;
    ctx.goSize.addrOffset = goAddrOffset;
    ctx.goSize.loopParam = goLoopParam;
    ctx.goSize.parallelParam = goParallelParam;
    ctx.goSize.residual = goResidual;
    return CCU_SUCCESS;
}

static CcuResult MsAccumPreSync(KfcReduceScatterMesh1DMsAccumContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.input[ctx.rankId], RS_MS_INPUT_XN_ID, RS_MS_CKE_IDX, 1 << RS_MS_INPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], RS_MS_TOKEN_XN_ID, RS_MS_CKE_IDX, 1 << RS_MS_TOKEN_XN_ID));
    }
    const uint16_t allBit = (1 << RS_MS_INPUT_XN_ID) | (1 << RS_MS_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], RS_MS_CKE_IDX, allBit);
    }
    return CCU_SUCCESS;
}

static CcuResult MsAccumPostSync(KfcReduceScatterMesh1DMsAccumContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], RS_MS_CKE_IDX, 1 << RS_MS_POST_SYNC_ID);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], RS_MS_CKE_IDX, 1 << RS_MS_POST_SYNC_ID);
    }
    return CCU_SUCCESS;
}

// 与 hccl CcuReduceScatterMesh1DMsAccumKernel::DoReduceScatter 一致：本 rank 份作 localSrc，
// 其余 peer 份按 rankIdx 升序（= channel 序）作远端 src，一次 GroupReduce 归并进 output。
// 显式 AllocGoResource(16, 1, ckeNum) 先行配置 LoopGroup 资源（默认并行度 128 不可用），
// 与宿主 goSize 的 loopCount=16 口径配套。
static CcuResult DoMsAccumReduceScatter(KfcReduceScatterMesh1DMsAccumContext& ctx)
{
    std::vector<ccu::RemoteAddr> src;
    ccu::LocalAddr localSrc;
    src.resize(ctx.channelCount);

    ccu::LocalAddr dst;
    dst.addr = ctx.output;
    dst.token = ctx.token[ctx.rankId];

    uint32_t curId = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx != ctx.rankId) {
            src[curId].addr = ctx.input[rankIdx];
            src[curId].addr += ctx.localInputOffset;
            src[curId].token = ctx.token[rankIdx];
            curId++;
        } else {
            localSrc.addr = ctx.input[rankIdx];
            localSrc.addr += ctx.localInputOffset;
            localSrc.token = ctx.token[rankIdx];
        }
    }

    uint32_t ckeNum = (GetCcuVersion() == CcuVersion::CCU_V2) ? CCU_LOOP_CKE_NUM_REDUCE_V2 : 1;
    CCU_CHK_RET(AllocGoResource(
        ctx.moConfig, ctx.moRes, ctx.resourceAllocated, RS_MS_ACCUM_REDUCE_SCATTER_LOOP_COUNT, 1, ckeNum));
    CCU_CHK_RET(GroupReduce(
        ctx, ctx.channels, ctx.channelCount, dst, src, localSrc, ctx.goSize, ctx.dataType, ctx.outputDataType,
        ctx.reduceOp, GetCcuVersion()));
    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMesh1DMsAccumKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable localInputOffset,
    ccu::Variable goAddrOffset, ccu::Variable goLoopParam, ccu::Variable goParallelParam, ccu::Variable goResidual,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType)
{
    KfcReduceScatterMesh1DMsAccumContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputType;
    if (ctx.outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        ctx.outputDataType = dataType;
    }
    ctx.reduceOp = reduceType;
    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;

    HCCL_INFO(
        "[CcuReduceScatterMesh1DMsAccum] run, rankId[%u], rankSize[%u], dataType[%d], outputDataType[%d], "
        "reduceOp[%d]",
        ctx.rankId, ctx.rankSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp);
    CCU_CHK_RET(InitMsAccumResource(ctx));
    CCU_CHK_RET(LoadMsAccumArgs(
        ctx, inputAddr, outputAddr, tokenInfo, localInputOffset, goAddrOffset, goLoopParam, goParallelParam,
        goResidual));

    CCU_CHK_RET(MsAccumPreSync(ctx));
    CCU_CHK_RET(DoMsAccumReduceScatter(ctx));
    CCU_CHK_RET(MsAccumPostSync(ctx));
    HCCL_INFO("[CcuReduceScatterMesh1DMsAccum] end.");
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
