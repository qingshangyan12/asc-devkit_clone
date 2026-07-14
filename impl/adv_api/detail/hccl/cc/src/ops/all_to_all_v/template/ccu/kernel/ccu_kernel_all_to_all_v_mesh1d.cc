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
// XN 窗口编号必须与框架分配、与定长 all_to_all / hccl _v 保持一致：
// 0=INPUT, 1=OUTPUT, 2=TOKEN。TOKEN 必须是 2，否则远端寻址凭证读错窗口 -> Local Access Error。
// INPUT 窗口本 kernel 不直接从对端读取（本地 input 由 inputAddr 参数给出），故不定义 INPUT_XN_ID。
constexpr int A2AV_OUTPUT_XN_ID = 1;
constexpr int A2AV_TOKEN_XN_ID = 2;
// 前同步用一个 CKE(=0)，用窗口位区分是哪个窗口到达；后同步用 CKE(=1)。
// 后同步 bit 与 hccl _v 保持一致(1<<1)，不要用定长 sibling 的 1<<5。
constexpr int A2AV_PRE_SYNC_CKE_IDX = 0;
constexpr int A2AV_POST_SYNC_CKE_IDX = 1;
constexpr int A2AV_POST_SYNC_BIT = 1;

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

    // 前同步：把本 rank 为每个对端准备的接收基地址(outputAddr + recvOffset)和 token 写到对端，
    // 通知位用窗口位区分(OUTPUT/TOKEN)，与定长 all_to_all / hccl _v 的同步模型一致。
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

    const uint16_t preAllBit = (1 << A2AV_OUTPUT_XN_ID) | (1 << A2AV_TOKEN_XN_ID);
    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyWait(channels[i], A2AV_PRE_SYNC_CKE_IDX, preAllBit);
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

    // 数据搬运：每个 rank 使用独立的 event 位，末尾统一 EventWait，
    // 与定长 all_to_all / hccl _v 一致；避免复用同一 bit 造成的串行/误同步。
    const uint16_t doneAllBit = (1 << rankSize) - 1;
    channelIdx = 0;
    for (uint32_t rankIdx = 0; rankIdx < rankSize; rankIdx++) {
        const uint32_t extBase = rankIdx * A2AV_EXT_FIELD_NUM;
        CCU_IF(extArgs[extBase + A2AV_SEND_SIZE_IDX] == 0) { // 该 rank 无数据，直接置完成位
            ccu::EventRecord(event, 1 << rankIdx);
        }
        CCU_IF(extArgs[extBase + A2AV_SEND_SIZE_IDX] != 0) {
            if (rankIdx == rankId) {
                ccu::LocalCopy(localDst, src[rankIdx], extArgs[extBase + A2AV_SEND_SIZE_IDX], event, 1 << rankIdx);
            } else {
                ccu::Write(channels[channelIdx], dst[rankIdx], src[rankIdx],
                    extArgs[extBase + A2AV_SEND_SIZE_IDX], event, 1 << rankIdx);
            }
        }
        if (rankIdx != rankId) {
            channelIdx++;
        }
    }
    ccu::EventWait(event, doneAllBit);

    // 后同步
    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyRecord(channels[i], A2AV_POST_SYNC_CKE_IDX, 1 << A2AV_POST_SYNC_BIT);
    }
    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyWait(channels[i], A2AV_POST_SYNC_CKE_IDX, 1 << A2AV_POST_SYNC_BIT);
    }
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
