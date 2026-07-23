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
constexpr int A2AV_OUTPUT_XN_ID = 1;
constexpr int A2AV_TOKEN_XN_ID = 2;
constexpr int A2AV_POST_SYNC_CKE_IDX = 0;
constexpr int A2AV_OUTPUT_CKE_IDX = 1;
constexpr int A2AV_TOKEN_CKE_IDX = 2;

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

    const uint16_t selfBit = 1 << rankId;
    const uint16_t allOtherBit = ((1 << rankSize) - 1) & (~selfBit);
    channelIdx = 0;
    for (uint32_t peerId = 0; peerId < rankSize; peerId++) {
        if (peerId == rankId) {
            continue;
        }
        ccu::Variable peerRecvBase;
        peerRecvBase = outputAddr + extArgs[peerId * A2AV_EXT_FIELD_NUM + A2AV_RECV_OFFSET_IDX];
        ccu::WriteVariableWithNotify(channels[channelIdx], peerRecvBase,
            A2AV_OUTPUT_XN_ID, A2AV_OUTPUT_CKE_IDX, selfBit);
        ccu::WriteVariableWithNotify(channels[channelIdx], tokenInfo,
            A2AV_TOKEN_XN_ID, A2AV_TOKEN_CKE_IDX, selfBit);
        channelIdx++;
    }

    // 【定位step3】怀疑 PreSync 死锁时，注释掉下面这两行 NotifyWait
    // for (uint32_t i = 0; i < channelCount; i++) {
    //     ccu::NotifyWait(channels[i], A2AV_OUTPUT_CKE_IDX, allOtherBit);
    //     ccu::NotifyWait(channels[i], A2AV_TOKEN_CKE_IDX, allOtherBit);
    // }

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
                // 【定位step2】怀疑传输死锁时，注释掉下面这行 EventWait
                // ccu::EventWait(event, 1);
            } else {
                ccu::Write(channels[channelIdx], dst[rankIdx], src[rankIdx],
                    extArgs[extBase + A2AV_SEND_SIZE_IDX], event, 1);
                // 【定位step2】怀疑传输死锁时，注释掉下面这行 EventWait
                // ccu::EventWait(event, 1);
            }
        }
        if (rankIdx != rankId) {
            channelIdx++;
        }
    }

    for (uint32_t i = 0; i < channelCount; i++) {
        ccu::NotifyRecord(channels[i], A2AV_POST_SYNC_CKE_IDX, selfBit);
    }
    // 【定位step1】怀疑 PostSync 死锁时，注释掉下面这三行 NotifyWait
    // for (uint32_t i = 0; i < channelCount; i++) {
    //     ccu::NotifyWait(channels[i], A2AV_POST_SYNC_CKE_IDX, allOtherBit);
    // }
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
