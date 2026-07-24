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

constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID  = 2;
constexpr int CKE_IDX_0    = 0;
constexpr int CKE_IDX_1    = 1;

struct KfcAlltoAllMesh1DContext {
    const ChannelHandle *channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable sliceSize;
    ccu::Variable srcStride;
    ccu::Variable srcOffset;
    ccu::Variable dstOffset;
    ccu::Event event;
};

static CcuResult InitResource(KfcAlltoAllMesh1DContext &ctx)
{
    uint32_t channelIdx = 0;
    if (ctx.channelCount == 0) {
        HCCL_ERROR("[KfcAlltoAllMesh1D] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    // 按照rank号从小到大遍历channels，遇到本rank就填充本地资源，否则依次取远端资源，要求算法返回的Link同样是按顺序排列的
    ctx.output.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.output[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], OUTPUT_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(KfcAlltoAllMesh1DContext &ctx, ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                        ccu::Variable sliceSize, ccu::Variable srcStride, ccu::Variable srcOffset,
                        ccu::Variable dstOffset)
{
    ctx.input = inputAddr;
    ctx.output[ctx.rankId] = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.sliceSize = sliceSize;
    ctx.srcStride = srcStride;
    ctx.srcOffset = srcOffset;
    ctx.dstOffset = dstOffset;

    ctx.srcOffset += ctx.input;
    return CCU_SUCCESS;
}

static void PreSync(KfcAlltoAllMesh1DContext &ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::WriteVariableWithNotify(ctx.channels[i], ctx.output[ctx.rankId],
                    OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID);
        ccu::WriteVariableWithNotify(ctx.channels[i], ctx.token[ctx.rankId],
                    TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID);
    }

    uint32_t allBit = (1 << OUTPUT_XN_ID) | (1 << TOKEN_XN_ID);
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
    }
}

static void PostSync(KfcAlltoAllMesh1DContext &ctx)
{
    uint16_t postBit = 1 << 5;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], CKE_IDX_1, postBit);
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_1, postBit);
    }
}

static CcuResult DoAlltoAll(KfcAlltoAllMesh1DContext &ctx)
{
    std::vector<ccu::LocalAddr> src(ctx.rankSize);
    std::vector<ccu::RemoteAddr> dst(ctx.rankSize);
    ccu::LocalAddr localDst;

    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx != ctx.rankId) {
            dst[rankIdx].token = ctx.token[rankIdx];
            dst[rankIdx].addr = ctx.output[rankIdx];
            dst[rankIdx].addr += ctx.dstOffset;
        } else {
            localDst.token = ctx.token[rankIdx];
            localDst.addr = ctx.output[rankIdx];
            localDst.addr += ctx.dstOffset;
        }

        src[rankIdx].addr = ctx.srcOffset;
        // 所有源地址都位于本 rank 的 input buffer，必须使用本 rank token。
        src[rankIdx].token = ctx.token[ctx.rankId];
        for (uint64_t i = 0; i < rankIdx; i++) {
            src[rankIdx].addr += ctx.srcStride;
        }
    }

    uint32_t channelId = 0;
    uint16_t allBit = (1 << ctx.rankSize) - 1;

    // 参照 KFC AllGather/ReduceScatter，直接使用 LocalCopy + Write，避免 GroupCopy
    // 创建 LoopGroup、LoopEntity 和内部 CCU buffer 导致资源超限。
    for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx == ctx.rankId) {
            ccu::LocalCopy(localDst, src[rankIdx], ctx.sliceSize, ctx.event, 1 << rankIdx);
        } else {
            ccu::Write(ctx.channels[channelId], dst[rankIdx], src[rankIdx], ctx.sliceSize,
                       ctx.event, 1 << rankIdx);
            channelId++;
        }
    }
    ccu::EventWait(ctx.event, allBit);

    return CCU_SUCCESS;
}

// ============================================================================
// kfc server调用函数
// ============================================================================
CcuResult CcuAlltoAllMesh1DKernel(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                  ccu::Variable sliceSize, ccu::Variable srcStride, ccu::Variable srcOffset,
                                  ccu::Variable dstOffset, ccu::Variable goSize1, ccu::Variable goSize2,
                                  ccu::Variable goSize3, ccu::Variable goSize4,
                                  const ChannelHandle channels[], uint32_t channelCount,
                                  uint32_t rankSize, uint32_t rankId, bool loadFromMem)
{
    (void)goSize1;
    (void)goSize2;
    (void)goSize3;
    (void)goSize4;
    (void)loadFromMem;

    KfcAlltoAllMesh1DContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, sliceSize, srcStride, srcOffset, dstOffset));
    PreSync(ctx);
    CCU_CHK_RET(DoAlltoAll(ctx));
    PostSync(ctx);
    return CCU_SUCCESS;
}

}// namespace mc2_ops_hccl
