/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty.h"
#include "../ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.h"
#include "ccu_func_dl.hpp"

namespace mc2_ops_hccl {
namespace {
constexpr uint16_t A2A_OUTPUT_XN_ID = 1U;
constexpr uint16_t A2A_TOKEN_XN_ID = 2U;
constexpr uint16_t A2A_POST_SYNC_BIT = static_cast<uint16_t>(1U << 5U);
constexpr uint16_t A2A_CKE_IDX = 0U;

struct KfcAllToAllContext : CcuKernelCtxBase {
    const ChannelHandle* channels = nullptr;
    uint32_t channelCount = 0;
    uint32_t rankSize = 0;
    uint32_t rankId = 0;

    ccu::Variable input;
    ccu::Variable output;
    ccu::Variable token;
    std::vector<ccu::Variable> peerOutput;
    std::vector<ccu::Variable> peerToken;
    ccu::Variable sliceSize;
    ccu::Variable srcStride;
    ccu::Variable srcOffset;
    ccu::Variable dstOffset;
    GroupOpSizeVars goSize;
    ccu::Event event;
    ccu::Event localCopyEvent;
};

struct KfcAllToAllClosContext : KfcAllToAllContext {};

CcuResult InitResource(KfcAllToAllContext& ctx)
{
    if (ctx.channelCount == 0U || ctx.rankSize == 0U || ctx.rankId >= ctx.rankSize) {
        HCCL_ERROR(
            "[CcuKfcAllToAll] invalid resource, channelCount[%u], rankSize[%u], rankId[%u]", ctx.channelCount,
            ctx.rankSize, ctx.rankId);
        return CCU_E_INTERNAL;
    }
    ctx.peerOutput.resize(ctx.rankSize);
    ctx.peerToken.resize(ctx.rankSize);
    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < ctx.rankSize; ++peerId) {
        if (peerId == ctx.rankId) {
            continue;
        }
        ctx.peerOutput[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A2A_OUTPUT_XN_ID);
        ctx.peerToken[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], A2A_TOKEN_XN_ID);
        ++channelIdx;
    }
    ctx.resourceAllocated = false;
    return CCU_SUCCESS;
}

CcuResult LoadArgs(KfcAllToAllContext& ctx, ccu::Array<ccu::Variable>& args)
{
    uint32_t argId = 1;
    ctx.input = args[KfcFuncArgIndex(argId++)];
    ctx.output = args[KfcFuncArgIndex(argId++)];
    ctx.token = args[KFC_FUNC_TOKEN_ARG_INDEX];
    ctx.sliceSize = args[KfcFuncArgIndex(argId++)];
    ctx.srcStride = args[KfcFuncArgIndex(argId++)];
    ctx.srcOffset = args[KfcFuncArgIndex(argId++)];
    ctx.dstOffset = args[KfcFuncArgIndex(argId++)];
    ctx.goSize.addrOffset = args[KfcFuncArgIndex(argId++)];
    ctx.goSize.loopParam = args[KfcFuncArgIndex(argId++)];
    ctx.goSize.parallelParam = args[KfcFuncArgIndex(argId++)];
    ctx.goSize.residual = args[KfcFuncArgIndex(argId++)];
    return CCU_SUCCESS;
}

CcuResult LoadArgs(KfcAllToAllClosContext& ctx, ccu::Array<ccu::Variable>& args)
{
    ctx.input = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_INPUT)];
    ctx.output = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_OUTPUT)];
    ctx.token = args[KFC_FUNC_TOKEN_ARG_INDEX];
    ctx.sliceSize = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE)];
    ctx.srcStride = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_SRC_STRIDE)];
    ctx.srcOffset = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_SRC_OFFSET)];
    ctx.dstOffset = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_DST_OFFSET)];
    ctx.goSize.addrOffset = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_GO_SIZE_0)];
    ctx.goSize.loopParam = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_GO_SIZE_1)];
    ctx.goSize.parallelParam = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_GO_SIZE_2)];
    ctx.goSize.residual = args[KfcFuncArgIndex(KFC_CONCURRENT_A2A_CLOS_GO_SIZE_3)];
    return CCU_SUCCESS;
}

CcuResult PreSync(KfcAllToAllContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.output, A2A_OUTPUT_XN_ID, A2A_CKE_IDX, 1U << A2A_OUTPUT_XN_ID));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(
            ctx.channels[i], ctx.token, A2A_TOKEN_XN_ID, A2A_CKE_IDX, 1U << A2A_TOKEN_XN_ID));
    }
    const uint16_t waitMask = (1U << A2A_OUTPUT_XN_ID) | (1U << A2A_TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2A_CKE_IDX, waitMask));
    }
    return CCU_SUCCESS;
}

CcuResult PostSync(KfcAllToAllContext& ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.channels[i], A2A_CKE_IDX, A2A_POST_SYNC_BIT));
    }
    for (uint32_t i = 0; i < ctx.channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.channels[i], A2A_CKE_IDX, A2A_POST_SYNC_BIT));
    }
    return CCU_SUCCESS;
}

CcuResult DoAllToAll(KfcAllToAllContext& ctx)
{
    ctx.srcOffset += ctx.input;

    uint32_t channelId = 0;
    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; ++rankIdx) {
        if (rankIdx == ctx.rankId) {
            continue;
        }
        ccu::LocalAddr src;
        src.addr = ctx.srcOffset;
        for (uint32_t i = 0; i < rankIdx; ++i) {
            src.addr += ctx.srcStride;
        }
        src.token = ctx.token;
        ccu::RemoteAddr dst;
        dst.addr = ctx.peerOutput[rankIdx];
        dst.addr += ctx.dstOffset;
        dst.token = ctx.peerToken[rankIdx];

        const uint16_t rankMask = static_cast<uint16_t>(1U << rankIdx);
        CCU_IF(ctx.sliceSize != 0)
        {
            CCU_CHK_RET(ccu::Write(ctx.channels[channelId], dst, src, ctx.sliceSize, ctx.event, rankMask));
        }
        CCU_ELSE { CCU_CHK_RET(ccu::EventRecord(ctx.event, rankMask)); }
        ++channelId;
    }

    ccu::LocalAddr localSrc;
    localSrc.addr = ctx.srcOffset;
    for (uint32_t i = 0; i < ctx.rankId; ++i) {
        localSrc.addr += ctx.srcStride;
    }
    localSrc.token = ctx.token;
    ccu::LocalAddr localDst;
    localDst.addr = ctx.output;
    localDst.addr += ctx.dstOffset;
    localDst.token = ctx.token;
    CCU_IF(ctx.sliceSize != 0)
    {
        CCU_CHK_RET(ccu::LocalCopy(localDst, localSrc, ctx.sliceSize, ctx.localCopyEvent, 1));
        CCU_CHK_RET(ccu::EventWait(ctx.localCopyEvent, 1));
    }
    CCU_CHK_RET(ccu::EventRecord(ctx.event, static_cast<uint16_t>(1U << ctx.rankId)));

    uint16_t waitMask = 0;
    for (uint32_t r = 0; r < ctx.rankSize; ++r) {
        waitMask |= 1U << r;
    }
    CCU_CHK_RET(ccu::EventWait(ctx.event, waitMask));
    return CCU_SUCCESS;
}
} // namespace

CcuResult CcuKfcAllToAllMesh1DMultiJettyKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgKfcAllToAllMesh1DMultiJetty*>(arg);
    KfcAllToAllContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = static_cast<uint32_t>(kernelArg->rankSize);
    ctx.rankId = kernelArg->rankId;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));

    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadArgs(ctx, args);
            PreSync(ctx);
            DoAllToAll(ctx);
            PostSync(ctx);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_KFC_ALL_TO_ALL_MESH1D_MULTI_JETTY_NAME);
    return CCU_SUCCESS;
}

CcuResult CcuKfcAllToAllClos1DMultiJettyKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<CcuKernelArgKfcAllToAllMesh1DMultiJetty*>(arg);
    KfcAllToAllClosContext ctx;
    ctx.channels = kernelArg->channels;
    ctx.channelCount = kernelArg->channelCount;
    ctx.rankSize = static_cast<uint32_t>(kernelArg->rankSize);
    ctx.rankId = kernelArg->rankId;
    InitCcuKernelCtxBase(ctx);
    CCU_CHK_RET(InitResource(ctx));
    ccu::Func func(
        [&ctx](ccu::Array<ccu::Variable>& args) {
            LoadArgs(ctx, args);
            PreSync(ctx);
            DoAllToAll(ctx);
            PostSync(ctx);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_KFC_ALL_TO_ALL_CLOS1D_MULTI_JETTY_NAME);
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
