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
#include "ccu_kernel_all_to_all_mesh1d.h"

namespace mc2_ops_hccl {

constexpr int INPUT_XN_ID  = 0;
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID  = 2;
constexpr int CKE_IDX_0    = 0;
constexpr int CKE_IDX_1    = 1;
constexpr int CKE_IDX_2    = 2;

constexpr uint64_t LOCAL_COPY_MS = 8;

static CcuResult ParseKernelArg(AlltoAllMesh1DContext &ctx, CcuKernelArgAlltoAllMesh1D *kernelArg)
{
    // ctx.rankId          = kernelArg->rankId;
    // ctx.rankSize        = kernelArg->rankSize;
    // ctx.channels       = kernelArg->channels;
    return CCU_SUCCESS;
}

static CcuResult InitResource(AlltoAllMesh1DContext &ctx)
{
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] start InitResource.");
    const auto *arg = ctx.arg;
    uint32_t channelIdx = 0;
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] obtain arg, arg == nullptr:[%u].", static_cast<uint32_t>(arg == nullptr));

    if (arg == nullptr) {
        HCCL_ERROR("[CcuKernelAlltoAllMesh1D] InitResource failed, kernel arg is nullptr.");
        return CcuResult::CCU_E_INTERNAL;
    }

    HCCL_INFO("[CcuKernelAlltoAllMesh1D] InitResource arg snapshot: rankSize[%llu], rankId[%u], "
        "channelCount[%u], loadFromMem[%u].",
        static_cast<unsigned long long>(arg->rankSize), arg->rankId, arg->channelCount,
        static_cast<uint32_t>(arg->loadFromMem));

    if (arg->rankSize == 0 || arg->rankSize > CCU_MAX_RANK_SIZE) {
        HCCL_ERROR("[CcuKernelAlltoAllMesh1D] invalid rankSize[%llu], maxRankSize[%llu].",
            static_cast<unsigned long long>(arg->rankSize), static_cast<unsigned long long>(CCU_MAX_RANK_SIZE));
        return CcuResult::CCU_E_INTERNAL;
    }
    if (arg->rankId >= arg->rankSize) {
        HCCL_ERROR("[CcuKernelAlltoAllMesh1D] invalid rankId[%u], rankSize[%llu].",
            arg->rankId, static_cast<unsigned long long>(arg->rankSize));
        return CcuResult::CCU_E_INTERNAL;
    }

    const uint64_t expectedChannelCount = arg->rankSize - 1;
    if (arg->channelCount < expectedChannelCount) {
        HCCL_ERROR("[CcuKernelAlltoAllMesh1D] insufficient channels, channelCount[%u], expected[%llu], "
            "rankSize[%llu], rankId[%u].",
            arg->channelCount, static_cast<unsigned long long>(expectedChannelCount),
            static_cast<unsigned long long>(arg->rankSize), arg->rankId);
        return CcuResult::CCU_E_INTERNAL;
    }
    if (arg->channelCount > expectedChannelCount) {
        HCCL_WARNING("[CcuKernelAlltoAllMesh1D] channelCount[%u] is larger than expected[%llu], "
            "only first expected channels will be used.",
            arg->channelCount, static_cast<unsigned long long>(expectedChannelCount));
    }

    // 按照rank号从小到大遍历channels，遇到本rank就填充本地资源，否则依次取远端资源，要求算法返回的Link同样是按顺序排列的
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] resize ctx.input start, size[%llu].",
        static_cast<unsigned long long>(arg->rankSize));
    ctx.input.resize(arg->rankSize);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] resize ctx.output start, size[%llu].",
        static_cast<unsigned long long>(arg->rankSize));
    ctx.output.resize(arg->rankSize);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] resize ctx.token start, size[%llu].",
        static_cast<unsigned long long>(arg->rankSize));
    ctx.token.resize(arg->rankSize);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] ctx resize finished, inputSize[%zu], outputSize[%zu], tokenSize[%zu].",
        ctx.input.size(), ctx.output.size(), ctx.token.size());
    for (uint64_t peerId = 0; peerId < arg->rankSize; peerId++) {
        HCCL_INFO("[CcuKernelAlltoAllMesh1D] process peerId[%llu/%llu], rankId[%u], channelIdx[%u].",
            static_cast<unsigned long long>(peerId), static_cast<unsigned long long>(arg->rankSize),
            arg->rankId, channelIdx);
        if (peerId != arg->rankId) {
            if (channelIdx >= arg->channelCount) {
                HCCL_ERROR("[CcuKernelAlltoAllMesh1D] channelIdx[%u] out of range, channelCount[%u], "
                    "peerId[%llu], rankId[%u], rankSize[%llu].",
                    channelIdx, arg->channelCount, static_cast<unsigned long long>(peerId), arg->rankId,
                    static_cast<unsigned long long>(arg->rankSize));
                return CcuResult::CCU_E_INTERNAL;
            }

            const auto channel = arg->channels[channelIdx];
            if (channel == 0) {
                HCCL_WARNING("[CcuKernelAlltoAllMesh1D] channel handle is zero, channelIdx[%u], peerId[%llu].",
                    channelIdx, static_cast<unsigned long long>(peerId));
            }
            HCCL_INFO("[CcuKernelAlltoAllMesh1D] get input variable start, channelIdx[%u], peerId[%llu], "
                "resId[%d], channel[0x%llx].",
                channelIdx, static_cast<unsigned long long>(peerId), INPUT_XN_ID,
                static_cast<unsigned long long>(channel));
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(channel, INPUT_XN_ID);
            HCCL_INFO("[CcuKernelAlltoAllMesh1D] get input variable finished, peerId[%llu], handle[0x%llx].",
                static_cast<unsigned long long>(peerId), static_cast<unsigned long long>(ctx.input[peerId].handle));

            HCCL_INFO("[CcuKernelAlltoAllMesh1D] get output variable start, channelIdx[%u], peerId[%llu], "
                "resId[%d], channel[0x%llx].",
                channelIdx, static_cast<unsigned long long>(peerId), OUTPUT_XN_ID,
                static_cast<unsigned long long>(channel));
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(channel, OUTPUT_XN_ID);
            HCCL_INFO("[CcuKernelAlltoAllMesh1D] get output variable finished, peerId[%llu], handle[0x%llx].",
                static_cast<unsigned long long>(peerId), static_cast<unsigned long long>(ctx.output[peerId].handle));

            HCCL_INFO("[CcuKernelAlltoAllMesh1D] get token variable start, channelIdx[%u], peerId[%llu], "
                "resId[%d], channel[0x%llx].",
                channelIdx, static_cast<unsigned long long>(peerId), TOKEN_XN_ID,
                static_cast<unsigned long long>(channel));
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(channel, TOKEN_XN_ID);
            HCCL_INFO("[CcuKernelAlltoAllMesh1D] get token variable finished, peerId[%llu], handle[0x%llx].",
                static_cast<unsigned long long>(peerId), static_cast<unsigned long long>(ctx.token[peerId].handle));
            channelIdx++;
        } else {
            HCCL_INFO("[CcuKernelAlltoAllMesh1D] skip local peerId[%llu], channelIdx[%u].",
                static_cast<unsigned long long>(peerId), channelIdx);
        }
    }
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] InitResource channel traversal finished, usedChannelCount[%u], "
        "expectedChannelCount[%llu].",
        channelIdx, static_cast<unsigned long long>(expectedChannelCount));

    // CCU_CHK_RET(ccu::CreateLoopExecutor(&ctx.enginePool, MAX_RANK_SIZE));

    ctx.resourceAllocated = false;
    // ctx.IsLoopResRegistered    = false;

    return CCU_SUCCESS;
}

static CcuResult LoadArgs(AlltoAllMesh1DContext &ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                  ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                  ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                  ccu::Variable goSize3, ccu::Variable goSize4)
{
    const auto *arg = ctx.arg;

    ctx.input[arg->rankId] = inputAddr;
    ctx.output[arg->rankId] = outputAddr;
    ctx.token[arg->rankId] = tokenInfo;
    ctx.sliceSize = sliceSize;
    ctx.srcStride = srcStride;
    ctx.srcOffset = srcOffset;
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
    std::vector<ccu::LocalAddr> src(arg->rankSize);
    std::vector<ccu::RemoteAddr> dst(arg->rankSize);
    ccu::LocalAddr localDst;

    for (uint32_t rankIdx = 0; rankIdx < arg->rankSize; rankIdx++) {
        if (rankIdx != arg->rankId) {
            dst[rankIdx].token = ctx.token[rankIdx];
            dst[rankIdx].addr = ctx.output[rankIdx];
            dst[rankIdx].addr += ctx.dstOffset;
        } else {
            localDst.token = ctx.token[rankIdx];
            localDst.addr = ctx.output[rankIdx];
            localDst.addr += ctx.dstOffset;
        }

        src[rankIdx].addr = ctx.srcOffset;
        src[rankIdx].token = ctx.token[rankIdx];
        for (uint64_t i = 0; i < rankIdx; i++) {
            src[rankIdx].addr += ctx.srcStride;
        }
    }

    uint32_t channelId = 0;
    uint16_t allBit = ((1 << arg->rankSize) - 1) & (~(1 << arg->rankId)); // 仅rankid位为0，其他位为1，代表远端准备好了

    if (arg->loadFromMem) {
        for(uint64_t r = 0; r < arg->rankSize; r++) {
            if (r == arg->rankId) {
                ccu::LocalCopy(localDst, src[r], ctx.sliceSize, ctx.event, 1 << r);
            }
            else {
                ccu::Write(arg->channels[channelId], dst[r], src[r], ctx.sliceSize, ctx.event, 1 << r);
                channelId++;
            }
        }
        // 等读完所有对端
        ccu::EventWait(ctx.event, (1 << arg->rankSize) - 1);
    } else {
        for(uint64_t r = 0; r < arg->rankSize; r++) {
            if (r != arg->rankId) {
                ccu::Write(arg->channels[channelId], dst[r], src[r], ctx.sliceSize, ctx.event, 1 << r);
                channelId++;
            }
        }
        GroupCopy(ctx, localDst, src[arg->rankId], ctx.goSize);
        ccu::EventWait(ctx.event, allBit);
    }

    return CCU_SUCCESS;
}

// ============================================================================
// kfc server调用函数
// ============================================================================
CcuResult CcuAlltoAllMesh1DKernel(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                  ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                  ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                  ccu::Variable goSize3, ccu::Variable goSize4, CcuKernelArg &arg)
{
    auto *kernelArg = static_cast<CcuKernelArgAlltoAllMesh1D *>(arg);

    AlltoAllMesh1DContext ctx;
    ctx.arg = kernelArg;
    ctx.resourceAllocated = false;
    // ctx.IsLoopResRegistered = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;

    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D run");
    CCU_CHK_RET(ParseKernelArg(ctx, kernelArg));
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] finish ParseKernelArg!");
    CCU_CHK_RET(InitResource(ctx));
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] finish InitResource!");
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, srcStride, srcOffset, dstOffset, sliceSize,
                                  goSize1, goSize2, goSize3, goSize4));
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] finish LoadArgs!");

    PreSync(ctx);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] finish PreSync!");

    CCU_CHK_RET(DoAlltoAll(ctx));
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] finish DoAlltoAll!");


    PostSync(ctx);
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] finish PostSync!");
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D end");

    return CCU_SUCCESS;
}

}// namespace mc2_ops_hccl
