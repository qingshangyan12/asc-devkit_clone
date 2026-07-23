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

constexpr uint32_t A2AV_EXT_FIELD_NUM = 4;
constexpr uint32_t A2AV_SEND_SIZE_IDX = 0;
constexpr uint32_t A2AV_SEND_OFFSET_IDX = 2;
constexpr uint32_t A2AV_RECV_OFFSET_IDX = 3;
// mesh1d 固定窗口布局：XN 0=INPUT, 1=OUTPUT, 2=TOKEN。
constexpr int A2AV_OUTPUT_XN_ID = 1;
constexpr int A2AV_TOKEN_XN_ID = 2;
// 同步方案对齐 hccl ccu_kernel_all_to_all_v_mesh1d.cc（逐 channel 握手，mask 用 XN 窗位，
// 不依赖 rankSize/rankId 的位累加，避免 AllGather 式 allOtherBit 在 rankSize/rankId 异常时死锁）。
constexpr int A2AV_PRE_SYNC_CKE_IDX = 0;  // 前同步：output/token 都 notify 到 CKE_0
constexpr int A2AV_POST_SYNC_CKE_IDX = 1; // 后同步：CKE_1
constexpr int A2AV_POST_SYNC_BIT = 1;     // 后同步固定 mask 位（与 hccl CONST_ONE 对齐）

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

    std::vector<ccu::Variable> output(rankSize);
    std::vector<ccu::Variable> token(rankSize);
    std::vector<ccu::LocalAddr> src(rankSize);
    std::vector<ccu::RemoteAddr> dst(rankSize);
    ccu::LocalAddr localDst;
    ccu::Event event;

    const uint32_t extArgNum = rankSize * A2AV_EXT_FIELD_NUM;
    ccu::Array<ccu::Variable> extArgs(extArgNum);
    ccu::Load(extAddr, extArgs, extArgNum);

    output[rankId] = outputAddr;
    token[rankId] = tokenInfo;

    uint32_t channelIdx = 0;
    for (uint32_t peerId = 0; peerId < rankSize; peerId++) {
        if (peerId == rankId) {
            continue;
        }
        output[peerId] = ccu::GetResByChannel<ccu::Variable>(channels[channelIdx], A2AV_OUTPUT_XN_ID);
        token[peerId] = ccu::GetResByChannel<ccu::Variable>(channels[channelIdx], A2AV_TOKEN_XN_ID);
        channelIdx++;
    }

    // ==== 前同步（对齐 hccl PreSync）====
    // 本卡向每个对端交换：接收基址(写入对端 OUTPUT 窗) + token(写入对端 TOKEN 窗)，
    // 两次 notify 都打到对端 CKE_0，mask 分别用 1<<OUTPUT_XN_ID / 1<<TOKEN_XN_ID。
    channelIdx = 0;
    for (uint32_t peerId = 0; peerId < rankSize; peerId++) {
        if (peerId == rankId) {
            continue;
        }
        ccu::Variable peerRecvBase;
        peerRecvBase = outputAddr + extArgs[peerId * A2AV_EXT_FIELD_NUM + A2AV_RECV_OFFSET_IDX];
        ccu::WriteVariableWithNotify(channels[channelIdx], peerRecvBase,
            A2AV_OUTPUT_XN_ID, A2AV_PRE_SYNC_CKE_IDX, 1 << A2AV_OUTPUT_XN_ID);
        ccu::WriteVariableWithNotify(channels[channelIdx], tokenInfo,
            A2AV_TOKEN_XN_ID, A2AV_PRE_SYNC_CKE_IDX, 1 << A2AV_TOKEN_XN_ID);
        channelIdx++;
    }

    // 逐 channel 等齐 output+token 两个通知（一次等 CKE_0 上的两位）。
    const uint16_t preSyncBit = (1 << A2AV_OUTPUT_XN_ID) | (1 << A2AV_TOKEN_XN_ID);
    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyWait(channels[i], A2AV_PRE_SYNC_CKE_IDX, preSyncBit);
    }

    for (uint32_t rankIdx = 0; rankIdx < rankSize; rankIdx++) {
        const uint32_t extBase = rankIdx * A2AV_EXT_FIELD_NUM;
        src[rankIdx].addr = inputAddr;
        src[rankIdx].addr += extArgs[extBase + A2AV_SEND_OFFSET_IDX];
        src[rankIdx].addr += srcOffset;
        src[rankIdx].token = token[rankIdx];

        if (rankIdx == rankId) {
            localDst.addr = outputAddr;
            localDst.addr += extArgs[extBase + A2AV_RECV_OFFSET_IDX];
            localDst.addr += dstOffset;
            localDst.token = token[rankIdx];
        } else {
            dst[rankIdx].addr = output[rankIdx];
            dst[rankIdx].addr += dstOffset;
            dst[rankIdx].token = token[rankIdx];
        }
    }

    channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < rankSize; rankIdx++) {
        const uint32_t extBase = rankIdx * A2AV_EXT_FIELD_NUM;
        CCU_IF(extArgs[extBase + A2AV_SEND_SIZE_IDX] != 0) {
            if (rankIdx == rankId) {
                ccu::LocalCopy(localDst, src[rankIdx], extArgs[extBase + A2AV_SEND_SIZE_IDX], event, 1);
                ccu::EventWait(event, 1);
            } else {
                ccu::Write(channels[channelIdx], dst[rankIdx], src[rankIdx],
                    extArgs[extBase + A2AV_SEND_SIZE_IDX], event, 1);
                ccu::EventWait(event, 1);
            }
        }
        if (rankIdx != rankId) {
            channelIdx++;
        }
    }

    // ==== 后同步（对齐 hccl PostSync）====
    // 逐 channel 互发/互等 CKE_1，mask 固定 1<<1，不依赖 rankId 位累加。
    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyRecord(channels[i], A2AV_POST_SYNC_CKE_IDX, 1 << A2AV_POST_SYNC_BIT);
    }
    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyWait(channels[i], A2AV_POST_SYNC_CKE_IDX, 1 << A2AV_POST_SYNC_BIT);
    }
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
