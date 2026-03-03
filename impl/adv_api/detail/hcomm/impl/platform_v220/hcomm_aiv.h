/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hcomm_aiv.h
 * \brief
 */
#ifndef IMPL_V220_HCOMM_AIV_H
#define IMPL_V220_HCOMM_AIV_H

#include "../../common/hcomm_utils.h"
#include "../../common/hcomm_inner_def.h"

namespace AscendC {

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE_AIV, COMM_PROTOCOL::ROCE>::Write(ChannelHandle channelHandle,
                                                                                     GM_ADDR dst, GM_ADDR src,
                                                                                     uint64_t len)
{
    HcommHandle handleId = curHandleId_++;
    doorBell(channelHandle, dst, src, len);
    return handleId;
}

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE_AIV, COMM_PROTOCOL::ROCE>::Read(ChannelHandle channelHandle,
                                                                                    GM_ADDR dst, GM_ADDR src,
                                                                                    uint64_t len)
{
    HcommHandle handleId = curHandleId_++;
    doorBell(channelHandle, dst, src, len);
    return handleId;
}

__aicore__ inline void HcommImpl<COMM_PROTOCOL::ROCE>::doorBell(ChannelHandle channelHandle, GM_ADDR dst, GM_ADDR src,
                                                                uint64_t len)
{
    __gm__ Channel* channelPtr = (__gm__ Channel*)channelHandle;

    auto qpNum = channelPtr->sqContextAddr->cxt.rdmaSqContext.qpn;
    auto sqBaseAddr = channelPtr->sqContextAddr->cxt.rdmaSqContext.sqVa;
    auto wqeSize = channelPtr->sqContextAddr->cxt.rdmaSqContext.wqeSize;

    auto curHardwareHead = channelPtr->sqContextAddr->cxt.rdmaSqContext.headAddr;
    FlushGmDataCache(reinterpret_cast<__gm__ uint8_t*>(curHardwareHead));
    uint64_t curHead = *(__gm__ uint32_t*)(curHardwareHead);

    auto curHardwareTailAddr = channelPtr->sqContextAddr->cxt.rdmaSqContext.tailAddr;
    uint64_t shift = 15U;
    auto QP_DEPTH = channelPtr->sqContextAddr->cxt.rdmaSqContext.depth;

    // Make sure we don't overflow the SQ in an infinite loop - no need to mitigate endless loop as the host
    // will timeout and kill the kernel, same as all2all krenel if it fails to complete (e.g. in case of link loss)
    while (1) {
        FlushGmDataCache(reinterpret_cast<__gm__ uint8_t*>(curHardwareTailAddr));
        if ((curHead - *(__gm__ uint32_t*)(curHardwareTailAddr)) < QP_DEPTH - 1) {
            break;
        }
        int64_t systemCycleAfter = AscendC::GetSystemCycle(); // add this line to solve slow poll CQ issue
    }

    __gm__ uint8_t* wqeAddr = (__gm__ uint8_t*)(sqBaseAddr + wqeSize * (curHead % QP_DEPTH));

    // Write the WQE to GM
    uint64_t ownBit = (curHead >> shift) & 1U;
    uint32_t byte_4 = 3U;                     // [0:4] opcode=0x3(RDMA_WRITE)
    byte_4 |= ((~ownBit) << 7U) & (1U << 7U); // [7] owner_bit
    byte_4 |= 1U << 8U;                       // [8:8] IBV_SEND_SIGNALED

    *(__gm__ uint32_t*)(wqeAddr) = byte_4;         // Control set by local parameter see above lines
    *(__gm__ uint32_t*)(wqeAddr + 4) = len;        // message size
    *(__gm__ uint32_t*)(wqeAddr + 8) = 0;          // immtdata is always 0 till we provide poll CQ flow in AIV
    *(__gm__ uint32_t*)(wqeAddr + 12) = 1U << 24U; // [120:127] num_sge = 1
    *(__gm__ uint32_t*)(wqeAddr + 16) = 0;         // [128:151] start_sge_idx = 0;
    *(__gm__ uint32_t*)(wqeAddr + 20) = channelPtr->remoteBufferAddr->pti.rdmaMemProtectionInfo.rkey;
    *(__gm__ uint64_t*)(wqeAddr + 24) = (uint64_t)dst; // destination VA

    __gm__ uint8_t* sgeAddr = wqeAddr + 32;
    *(__gm__ uint32_t*)(sgeAddr) = len;
    *(__gm__ uint32_t*)(sgeAddr + sizeof(uint32_t)) = channelPtr->localBufferAddr->pti.rdmaMemProtectionInfo.lkey;
    *(__gm__ uint64_t*)(sgeAddr + 2 * sizeof(uint32_t)) = (uint64_t)src; // src VA addr memory registered by RNIC

    FlushGmDataCache(wqeAddr);

    curHead++;

    uint64_t doorBellInfo = 0;
    doorBellInfo |= channelPtr->sqContextAddr->cxt.rdmaSqContext.qpn; // [0:23] DB_TAG (qp_num)
    doorBellInfo |= 0UL << 24UL;                                      // [24:27] DB_CMD = HNS_ROCE_V2_SQ_DB (0)
    doorBellInfo |= (curHead % 65536UL) << 32UL;                      // [32:47] DB_PI = sq.head
    doorBellInfo |= (uint64_t)(channelPtr->sqContextAddr->cxt.rdmaSqContext.sl) << 48UL; // [48:50] DB_SL = qp.sl

    __gm__ uint64_t* doorBellAddr = (__gm__ uint64_t*)(channelPtr->sqContextAddr->cxt.rdmaSqContext.dbVa);

    ubLocal_.SetValue(0, doorBellInfo);
    AscendC::GlobalTensor<uint64_t> DBGlobalTensor;
    DBGlobalTensor.SetGlobalBuffer(doorBellAddr);
    AscendC::DataCopyExtParams copyParams{1, 1 * sizeof(uint64_t), 0, 0, 0};
    AscendC::DataCopyPad(DBGlobalTensor, ubLocal_, copyParams);

    ubLocalHead_.SetValue(0, (uint32_t)curHead);
    AscendC::GlobalTensor<uint32_t> HeadGlobalTensor;
    HeadGlobalTensor.SetGlobalBuffer((__gm__ uint32_t*)curHardwareHead);
    AscendC::DataCopyExtParams copyParamsHead{1, 1 * sizeof(uint32_t), 0, 0, 0};
    AscendC::DataCopyPad(HeadGlobalTensor, ubLocalHead_, copyParamsHead);
}

} // namespace AscendC

#endif // IMPL_V220_HCOMM_AIV_H
