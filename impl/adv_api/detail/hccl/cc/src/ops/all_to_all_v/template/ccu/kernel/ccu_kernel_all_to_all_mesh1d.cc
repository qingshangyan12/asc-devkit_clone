/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_all_to_all_mesh1d.h"

namespace mc2_ops_hccl {

constexpr int INPUT_XN_ID  = 0;
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID  = 2;
constexpr int CKE_IDX_0    = 0;
constexpr int CKE_IDX_1    = 1;
constexpr int CKE_IDX_2    = 2;

constexpr uint64_t LOCAL_COPY_MS = 8;
constexpr uint64_t MAX_DMA_TRANS_SIZE = 256ULL * 1024 * 1024;
constexpr uint64_t NEG_256MB = 0xFFFFFFFFF0000000ULL;

static CcuResult ParseKernelArg(AlltoAllMesh1DContext &ctx, CcuKernelArgAlltoAllMesh1D *kernelArg)
{
    return CCU_SUCCESS;
}

static CcuResult InitResource(AlltoAllMesh1DContext &ctx)
{
    const auto *arg = ctx.arg;
    uint32_t channelIdx = 0;

    if (arg->channelCount == 0) {
        HCCL_ERROR("[CcuKernelAlltoAllMesh1D] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    // 按照rank号从小到大遍历channels，遇到本rank就填充本地资源，否则依次取远端资源，要求算法返回的Link同样是按顺序排列的
    ctx.input.resize(arg->rankSize);
    ctx.output.resize(arg->rankSize);
    ctx.token.resize(arg->rankSize);
    for (uint64_t peerId = 0; peerId < arg->rankSize; peerId++) {
        if (peerId != arg->rankId) {
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(arg->channels[channelIdx], INPUT_XN_ID);
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(arg->channels[channelIdx], OUTPUT_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(arg->channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }

    ctx.resourceAllocated = false;

    return CCU_SUCCESS;
}

static CcuResult LoadArgs(AlltoAllMesh1DContext &ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
    ccu::Variable tokenInfo, ccu::Variable sliceSize, ccu::Variable srcStride, ccu::Variable writeLoopCount,
    ccu::Variable dstOffset, ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
    ccu::Variable goSize4)
{
    const auto *arg = ctx.arg;
    ctx.input[arg->rankId] = inputAddr;
    ctx.output[arg->rankId] = outputAddr;
    ctx.token[arg->rankId] = tokenInfo;
    ctx.sliceSize = sliceSize;
    ctx.srcStride = srcStride;
    ctx.writeLoopCount = writeLoopCount;
    ctx.srcOffset = 0;
    ctx.dstOffset = dstOffset;
    ctx.goSize.addrOffset = goSize1;
    ctx.goSize.loopParam = goSize2;
    ctx.goSize.parallelParam = goSize3;
    ctx.goSize.residual = goSize4;
    ctx.srcOffset += ctx.input[arg->rankId];
    return CCU_SUCCESS;
}

static void PreSync(AlltoAllMesh1DContext &ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PreSync begin.");
    const auto *arg = ctx.arg;

    for (uint32_t i = 0; i < arg->channelCount; i++) {
        ccu::WriteVariableWithNotify(arg->channels[i], ctx.output[arg->rankId],
            OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID);
        ccu::WriteVariableWithNotify(arg->channels[i], ctx.token[arg->rankId],
            TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID);
    }

    uint32_t allBit = (1 << OUTPUT_XN_ID) | (1 << TOKEN_XN_ID);
    for (uint32_t i = 0; i < arg->channelCount; i++) {
        ccu::NotifyWait(arg->channels[i], CKE_IDX_0, allBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PreSync end.");
}

static void PostSync(AlltoAllMesh1DContext &ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PostSync begin.");
    const auto *arg = ctx.arg;

    uint16_t postBit = 1 << 5;
    for (uint32_t i = 0; i < arg->channelCount; i++) {
        ccu::NotifyRecord(arg->channels[i], CKE_IDX_1, postBit);
    }
    for (uint32_t i = 0; i < arg->channelCount; i++) {
        ccu::NotifyWait(arg->channels[i], CKE_IDX_1, postBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PostSync end.");
}

static CcuResult DoAlltoAll(AlltoAllMesh1DContext &ctx)
{
    HCCL_INFO("DoAlltoAll Start.");
    const auto *arg = ctx.arg;
    HCCL_INFO("[DoAlltoAll] rankSize[%u], rankId[%u], loadFromMem[%d], channelCount[%u], sliceSize variable set, "
        "srcStride variable set, dstOffset variable set, writeLoopCount variable set",
        arg->rankSize, arg->rankId, arg->loadFromMem, arg->channelCount);
    std::vector<ccu::LocalAddr> src(arg->rankSize);
    std::vector<ccu::RemoteAddr> dst(arg->rankSize);
    ccu::LocalAddr localDst;

    for (uint32_t rankIdx = 0; rankIdx < arg->rankSize; rankIdx++) {
        if (rankIdx != arg->rankId) {
            dst[rankIdx].token = ctx.token[rankIdx];
            dst[rankIdx].addr = ctx.output[rankIdx];
            dst[rankIdx].addr += ctx.dstOffset;
            HCCL_INFO("[DoAlltoAll] remote dst[rankIdx=%u]: output + dstOffset, token set", rankIdx);
        } else {
            localDst.token = ctx.token[rankIdx];
            localDst.addr = ctx.output[rankIdx];
            localDst.addr += ctx.dstOffset;
            HCCL_INFO("[DoAlltoAll] local dst[rankIdx=%u]: output + dstOffset, token set", rankIdx);
        }

        src[rankIdx].addr = ctx.srcOffset;
        src[rankIdx].token = ctx.token[rankIdx];
        for (uint64_t i = 0; i < rankIdx; i++) {
            src[rankIdx].addr += ctx.srcStride;
        }
        HCCL_INFO("[DoAlltoAll] src[rankIdx=%u]: srcOffset + %u * srcStride, token set", rankIdx, rankIdx);
    }

    uint32_t channelId = 0;
    uint16_t allBit = ((1 << arg->rankSize) - 1) & (~(1 << arg->rankId));
    HCCL_INFO("[DoAlltoAll] allBit[0x%x] (rankSize=%u, rankId=%u, exclude self)", allBit, arg->rankSize, arg->rankId);

    ccu::Variable chunkSize;
    chunkSize = MAX_DMA_TRANS_SIZE;
    ccu::Variable neg256MB;
    neg256MB = NEG_256MB;
    ccu::Variable constOne;
    constOne = 1;

    if (arg->loadFromMem) {
        HCCL_INFO("[DoAlltoAll] if branch: loadFromMem=true, local+remote with chunking");
        for(uint64_t r = 0; r < arg->rankSize; r++) {
            if (r == arg->rankId) {
                HCCL_INFO("[DoAlltoAll] LocalCopy for rank[%u], event bit[0x%x]", r, 1 << r);
                ccu::LocalCopy(localDst, src[r], ctx.sliceSize, ctx.event, 1 << r);
            }
            else {
                HCCL_INFO("[DoAlltoAll] Remote Write chunking for rank[%u], channelId[%u]", r, channelId);
                ccu::RemoteAddr curDst = dst[r];
                ccu::LocalAddr curSrc = src[r];

                ccu::Variable loopCount;
                loopCount = ctx.writeLoopCount;
                ccu::Variable remaining;
                remaining = ctx.sliceSize;

                CCU_WHILE(loopCount != UINT64_MAX)
                {
                    ccu::Write(arg->channels[channelId], curDst, curSrc, chunkSize, ctx.event, 0);
                    curDst.addr += chunkSize;
                    curSrc.addr += chunkSize;
                    remaining += neg256MB;
                    loopCount += constOne;
                }

                CCU_IF(remaining != 0)
                {
                    ccu::Write(arg->channels[channelId], curDst, curSrc, remaining, ctx.event, 1 << r);
                }
                CCU_ELSE
                {
                    ccu::EventRecord(ctx.event, 1 << r);
                }
                channelId++;
            }
        }
        HCCL_INFO("[DoAlltoAll] EventWait all ranks, bit[0x%x]", (1 << arg->rankSize) - 1);
        ccu::EventWait(ctx.event, (1 << arg->rankSize) - 1);
    } else {
        HCCL_INFO("[DoAlltoAll] else branch: loadFromMem=false, remote chunking + GroupCopy local");
        for(uint64_t r = 0; r < arg->rankSize; r++) {
            if (r != arg->rankId) {
                HCCL_INFO("[DoAlltoAll] Remote Write chunking for rank[%u], channelId[%u]", r, channelId);
                ccu::RemoteAddr curDst = dst[r];
                ccu::LocalAddr curSrc = src[r];

                ccu::Variable loopCount;
                loopCount = ctx.writeLoopCount;
                ccu::Variable remaining;
                remaining = ctx.sliceSize;

                CCU_WHILE(loopCount != UINT64_MAX)
                {
                    ccu::Write(arg->channels[channelId], curDst, curSrc, chunkSize, ctx.event, 0);
                    curDst.addr += chunkSize;
                    curSrc.addr += chunkSize;
                    remaining += neg256MB;
                    loopCount += constOne;
                }

                CCU_IF(remaining != 0)
                {
                    ccu::Write(arg->channels[channelId], curDst, curSrc, remaining, ctx.event, 1 << r);
                }
                CCU_ELSE
                {
                    ccu::EventRecord(ctx.event, 1 << r);
                }
                channelId++;
            }
        }
        HCCL_INFO("[DoAlltoAll] GroupCopy for local rank[%u]", arg->rankId);
        GroupCopy(ctx, localDst, src[arg->rankId], ctx.goSize);
        HCCL_INFO("[DoAlltoAll] EventWait allBit[0x%x]", allBit);
        ccu::EventWait(ctx.event, allBit);
    }

    return CCU_SUCCESS;
}

// ============================================================================
// 主入口 Kernel 函数
// ============================================================================
CcuResult CcuAlltoAllMesh1DKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
    ccu::Variable sliceSize, ccu::Variable srcStride, ccu::Variable writeLoopCount, ccu::Variable dstOffset,
    ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable goSize4,
    CcuKernelArg &arg)
{
    auto *kernelArg = static_cast<CcuKernelArgAlltoAllMesh1D *>(arg);

    AlltoAllMesh1DContext ctx;
    ctx.arg = kernelArg;
    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;

    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D run");
    CCU_CHK_RET(ParseKernelArg(ctx, kernelArg));
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, sliceSize, srcStride, writeLoopCount, dstOffset,
        goSize1, goSize2, goSize3, goSize4));

    PreSync(ctx);

    CCU_CHK_RET(DoAlltoAll(ctx));

    PostSync(ctx);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D end");

    return CCU_SUCCESS;
}

}// namespace mc2_ops_hccl
