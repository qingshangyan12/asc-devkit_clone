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
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_read_reduce.h"
#include "mc2_type.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// XN/CKE 槽位与 asc-devkit 侧 KFC RS 兄弟内核（mem2mem / peer_only）保持一致：INPUT=0、TOKEN=2、POST=3
constexpr int RS_RR_INPUT_XN_ID = 0;
constexpr int RS_RR_TOKEN_XN_ID = 2;
constexpr int RS_RR_POST_SYNC_ID = 3;
constexpr int RS_RR_CKE_IDX = 0;
// 与 hccl rs-sched-perf 的 READ_REDUCE_MAX_CHUNK_COUNT 一致：chunkCount 上限 = 4*(rankSize-1)，rankSize<=8
constexpr uint32_t RS_RR_MAX_CHUNK_COUNT = 28;

struct KfcReduceScatterMesh1DReadReduceContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};
    uint32_t chunkCount{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> token;
    ccu::Variable output;
    ccu::Variable localInputOffset;
    ccu::Variable sliceSize;
    // chunk 尺寸紧凑编码（宿主下发），kernel 内重建 chunkSizes/chunkOffsets
    ccu::Variable chunkBaseSize;
    ccu::Variable chunkAlign;
    ccu::Variable chunkExtraUnits;
    ccu::Variable chunkTail;
    std::vector<ccu::Variable> chunkSizes;
    std::vector<ccu::Variable> chunkOffsets;
    std::vector<ccu::Event> events;
};

static CcuResult InitReadReduceResource(KfcReduceScatterMesh1DReadReduceContext& ctx)
{
    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuReduceScatterMesh1DReadReducePipelined] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }
    if (ctx.rankSize < 2 || ctx.rankSize > 8) {
        HCCL_ERROR(
            "[CcuReduceScatterMesh1DReadReducePipelined] rankSize[%u] is out of range [2, 8].", ctx.rankSize);
        return CcuResult::CCU_E_PARA;
    }
    if (ctx.chunkCount < ctx.rankSize - 1 || ctx.chunkCount > RS_RR_MAX_CHUNK_COUNT) {
        HCCL_ERROR(
            "[CcuReduceScatterMesh1DReadReducePipelined] chunkCount[%u] is out of range [%u, %u].", ctx.chunkCount,
            ctx.rankSize - 1, RS_RR_MAX_CHUNK_COUNT);
        return CcuResult::CCU_E_PARA;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], RS_RR_INPUT_XN_ID);
        ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], RS_RR_TOKEN_XN_ID);
        channelIdx++;
    }

    ctx.chunkSizes.resize(ctx.chunkCount);
    ctx.chunkOffsets.resize(ctx.chunkCount);
    ctx.events.resize(ctx.chunkCount);
    return CCU_SUCCESS;
}

static CcuResult LoadReadReduceArgs(
    KfcReduceScatterMesh1DReadReduceContext& ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable localInputOffset, ccu::Variable sliceSize, ccu::Variable chunkBaseSize,
    ccu::Variable chunkAlign, ccu::Variable chunkExtraUnits, ccu::Variable chunkTail)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.localInputOffset = localInputOffset;
    ctx.sliceSize = sliceSize;
    ctx.chunkBaseSize = chunkBaseSize;
    ctx.chunkAlign = chunkAlign;
    ctx.chunkExtraUnits = chunkExtraUnits;
    ctx.chunkTail = chunkTail;

    // 紧凑编码重建（与宿主 CcuPrepareForReduceScatterChunkPipelinedM2M / hccl
    // CcuTempReduceScatterMesh1DReadReduce::KernelRun 的切分公式逐值一致）：
    // sizes[i] = base + (i < extraUnits ? align : 0)，末块再加 tail；offsets 为前缀和。
    // "i < extraUnits" 以递减计数器 extraLeft 表达（CCU 条件仅用 !=0，避开 ST/生产
    // Variable 运算符集差异）：前 extraUnits 个 chunk 各加一个 align 并递减。
    ccu::Variable chunkOffset;
    chunkOffset = 0;
    ccu::Variable extraLeft;
    extraLeft = ctx.chunkExtraUnits;
    ccu::Variable one;
    one = 1;
    for (uint32_t i = 0; i < ctx.chunkCount; i++) {
        ctx.chunkSizes[i] = ctx.chunkBaseSize;
        CCU_IF(extraLeft != 0)
        {
            ctx.chunkSizes[i] += ctx.chunkAlign;
            extraLeft = extraLeft - one;
        }
        if (i + 1U == ctx.chunkCount) {
            ctx.chunkSizes[i] += ctx.chunkTail;
        }
        ctx.chunkOffsets[i] = chunkOffset;
        chunkOffset += ctx.chunkSizes[i];
    }
    return CCU_SUCCESS;
}

static CcuResult ReadReducePreSync(KfcReduceScatterMesh1DReadReduceContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.input[ctx.rankId], RS_RR_INPUT_XN_ID, RS_RR_CKE_IDX, 1 << RS_RR_INPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token[ctx.rankId], RS_RR_TOKEN_XN_ID, RS_RR_CKE_IDX, 1 << RS_RR_TOKEN_XN_ID));
    }
    const uint16_t allBit = (1 << RS_RR_INPUT_XN_ID) | (1 << RS_RR_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], RS_RR_CKE_IDX, allBit);
    }
    return CCU_SUCCESS;
}

static CcuResult ReadReducePostSync(KfcReduceScatterMesh1DReadReduceContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], RS_RR_CKE_IDX, 1 << RS_RR_POST_SYNC_ID);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], RS_RR_CKE_IDX, 1 << RS_RR_POST_SYNC_ID);
    }
    return CCU_SUCCESS;
}

// 三段式（hccl "Change to cycle" 终态）：1) seed：本 rank 份额 LocalCopy 进 output；
// 2) wave×chunk 双层循环 ReadReduce：每 wave 内各 chunk 轮转访问不同 peer（均衡通道），
//    per-chunk 事件在下一 wave 复用前等待（无 wave barrier 的软件流水）；
// 3) 尾部批量等待所有 chunk 事件。
static CcuResult DoReadReduceScatter(KfcReduceScatterMesh1DReadReduceContext& ctx)
{
    const uint32_t chunkCount = ctx.chunkCount;
    const uint32_t peerCount = ctx.rankSize - 1;

    ccu::LocalAddr seedSrc;
    seedSrc.addr = ctx.input[ctx.rankId];
    seedSrc.addr += ctx.localInputOffset;
    seedSrc.token = ctx.token[ctx.rankId];

    ccu::LocalAddr seedDst;
    seedDst.addr = ctx.output;
    seedDst.token = ctx.token[ctx.rankId];

    ccu::Event seedEvent;
    CCU_CHK_RET(ccu::LocalCopy(seedDst, seedSrc, ctx.sliceSize, seedEvent, 1));
    CCU_CHK_RET(ccu::EventWait(seedEvent, 1));

    for (uint32_t wave = 0; wave < peerCount; wave++) {
        for (uint32_t chunk = 0; chunk < chunkCount; chunk++) {
            if (wave != 0) {
                CCU_CHK_RET(ccu::EventWait(ctx.events[chunk], 1));
            }

            const uint32_t peerOffset = (chunk + wave) % peerCount + 1;
            const uint32_t peerRank = (ctx.rankId + peerOffset) % ctx.rankSize;
            const uint32_t channelIdx = (peerRank < ctx.rankId) ? peerRank : peerRank - 1;

            ccu::LocalAddr dst;
            dst.addr = ctx.output;
            dst.addr += ctx.chunkOffsets[chunk];
            dst.token = ctx.token[ctx.rankId];

            ccu::RemoteAddr src;
            src.addr = ctx.input[peerRank];
            src.addr += ctx.localInputOffset;
            src.addr += ctx.chunkOffsets[chunk];
            src.token = ctx.token[peerRank];

            CCU_CHK_RET(ccu::ReadReduce(
                ctx.channels[channelIdx], dst, src, ctx.chunkSizes[chunk], ctx.dataType, ctx.reduceOp,
                ctx.events[chunk], 1));
        }
    }

    for (uint32_t chunk = 0; chunk < chunkCount; chunk++) {
        CCU_CHK_RET(ccu::EventWait(ctx.events[chunk], 1));
    }

    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMesh1DReadReducePipelinedKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable localInputOffset,
    ccu::Variable sliceSize, ccu::Variable chunkBaseSize, ccu::Variable chunkAlign, ccu::Variable chunkExtraUnits,
    ccu::Variable chunkTail, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId, uint32_t chunkCount, const HcclDataType& dataType, const HcclDataType& outputType,
    const HcclReduceOp& reduceType)
{
    KfcReduceScatterMesh1DReadReduceContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.chunkCount = chunkCount;
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
        "[CcuReduceScatterMesh1DReadReducePipelined] run, rankId[%u], rankSize[%u], chunkCount[%u], dataType[%d], "
        "outputDataType[%d], reduceOp[%d]",
        ctx.rankId, ctx.rankSize, ctx.chunkCount, ctx.dataType, ctx.outputDataType, ctx.reduceOp);
    CCU_CHK_RET(InitReadReduceResource(ctx));
    CCU_CHK_RET(LoadReadReduceArgs(
        ctx, inputAddr, outputAddr, tokenInfo, localInputOffset, sliceSize, chunkBaseSize, chunkAlign,
        chunkExtraUnits, chunkTail));

    CCU_CHK_RET(ReadReducePreSync(ctx));

    CCU_IF(ctx.sliceSize != 0)
    {
        CCU_CHK_RET(DoReadReduceScatter(ctx));
    }

    CCU_CHK_RET(ReadReducePostSync(ctx));
    HCCL_INFO("[CcuReduceScatterMesh1DReadReducePipelined] end.");
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
