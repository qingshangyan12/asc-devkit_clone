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
 * \file hccl_hcomm_urma.h
 * \brief
 */
#ifndef IMPL_V310_HCOMM_URMA_H
#define IMPL_V310_HCOMM_URMA_H

#include "hcomm_aiv_def.h"
#include "hcomm_inner_def.h"

namespace AscendC {

__aicore__ inline uint32_t HtoNL(uint32_t x)
{
    return ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8)
            | (((x) & 0x000000ff) << 24));
}

#define ROCE_SQ_DOORBELL_TYPE 2
#define ROCE_INIT_SQ_DB_SGIT_IDX 1

constexpr uint64_t UB_FLAG_SIZE = 32;
constexpr uing64_t UB_FLAG_SIZE_5 = UB_FLAG_SIZE * 5;

constexpr uint64_t UB_FLAG_PAD_COUNT = 8;
constexpr uint64_t UB_ADDRESS_PAD_COUNT = 4;

__aicore__ inline Hcomm<COMM_PROTOCOL::ROCE, pipe>::Hcomm()
{
    pipe.InitBuffer(localFlagBuf, UB_FLAG_SIZE_5);
    ubLocalDb = localFlagBuf.GetWithOffset<uint64_t>(UB_FLAG_PAD_COUNT, UB_ADDRESS_PAD_COUNT);
    ubLocalSqCi = localFlagBuf.GetWithOffset<uint32_t>(UB_FLAG_PAD_COUNT, UB_ADDRESS_PAD_COUNT);
    ubLocalSqPi = localFlagBuf.GetWithOffset<uint32_t>(UB_FLAG_PAD_COUNT, UB_ADDRESS_PAD_COUNT);
    ubLocalCq = localFlagBuf.GetWithOffset<uint32_t>(UB_FLAG_PAD_COUNT, UB_ADDRESS_PAD_COUNT);
    ubLocalCqCi = localFlagBuf.GetWithOffset<uint32_t>(UB_FLAG_PAD_COUNT, UB_ADDRESS_PAD_COUNT);
}

template <bool commit, pipe_t pipe>
__aicore__ inline Handle Hcomm<COMM_PROTOCOL::ROCE, pipe> Write(ChannelHandle channel, void* dst, const void* src,
                                                                uint64_t len)
{}

template <bool commit, pipe_t pipe>
__aicore__ inline Handle Hcomm<COMM_PROTOCOL::ROCE, pipe> Read(ChannelHandle channel, void* dst, const void* src,
                                                               uint64_t len)
{}

template <typename T>
__aicore__ inline void Gm2Ub(const LocalTensor<T>& dstLocal, const GlobalTensor<T>& srcGlobal, int32_t length)
{
    DataCopyExtParams copyParams{1, length * sizeof(T), 0, 0, 0};
    DataCopyPadExtParams<uint32_t> padParams;
    padParams.isPad = 0;
    padParams.leftPadding = 0;
    padParams.rightPadding = 0;
    padParams.paddingValue = 0;

    PipeBarrier<PIPE_ALL>();
    DataCopyPad(dstLocal, srcGlobal, copyParams, padParams);
    PipeBarrier<PIPE_ALL>();
}

template <typename T>
__aicore__ inline void Ub2Gm(const GlobalTensor<T>& dstGlobal, const LocalTensor<T>& srcLocal, int32_t length)
{
    DataCopyExtParams copyParams{1, length * sizeof(T), 0, 0, 0};

    PipeBarrier<PIPE_ALL>();
    DataCopyPad(dstGlobal, srcLocal, copyParams);
    PipeBarrier<PIPE_ALL>();
}

template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<pipe> Commit(Handle handle)
{
    Channel* channel = getChannelFromHandle(handle); // 获取channel
    Gm2Ub(ubLocalSqPi, channel->SqContextAddr->RdmaSqContext.tailAddr, 1);
    uint32_t sq_head = ubLocalSqPi.GetValue(static_cast<uint32_t>(0)); // 获取当前channel对应的sq pi

    uint64_t doorBellInfo = 0;
    doorBellInfo |= ROCE_SQ_DOORBELL_TYPE << 63UL;
    doorBellInfo |= channel->SqContextAddr->RdmaSqContext.qpn << 32UL;
    doorBellInfo |= ROCE_INIT_SQ_DB_SGIT_IDX << 22UL;
    doorBellInfo |= sq_head;

    __gm__ uint64_t* doorBellAddr =
        (__gm__ uint64_t*)(channel->SqContextAddr->RdmaSqContext.dbVa + (sq_head >> 8) & 0xff);
    ubLocalDb.SetValue(0, doorBellInfo);

    GlobalTensor<uint64_t> dbGlobalTensor;
    dbGlobalTensor.SetGlobalBuffer(doorBellAddr);
    DataCopyExtParams copyParamsDb{1, 1 * sizeof(uint64_t), 0, 0, 0};

    SetAtomicAdd<uint64_t>();
    PipeBarrier<PIPE_ALL>();
    DataCopyPad(dbGlobalTensor, ubLocalDb, copyParamsDb);
    PipeBarrier<PIPE_ALL>();
    SetAtomicNone();
}

template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<pipe> Wait(Handle handle)
{
    Channel* channel = getChannelFromHandle(handle); // 获取channel
    auto sqCiAddr = channel->SqContextAddr->RdmaSqContext.tailAddr;
    GlobalTensor<uint32_t> sqCiGlobalTensor;
    sqCiGlobalTensor.SetGlobalBuffer(sqCiAddr); // 获取Sq ci对应的global tensor

    auto cqCiAddr = channel->CqContextAddr->RdmaCqContext.tailAddr;
    GlobalTensor<uint32_t> cqCiGlobalTensor;
    cqCiGlobalTensor.SetGlobalBuffer(cqCiAddr);

    __gm__ uint8_t* cq_base_buf = (__gm__ uint8_t*)(channel->CqContextAddr->cqVa); // 获取cq地址

    uint8_t owner;
    uint32_t cq_mask;

    uint32_t cqeSize = channel->CqContextAddr->RdmaCqContext.cqeSize;
    GlobalTensor<uint32_t> cqGlobalTensor;
    RoceCqEntry* cqeUB = (RoceCqEntry*)(ubLocalCq.GetPhyAddr());
    do {
        Ub2Gm(cqCiGlobalTensor, ubLocalCqCi, 1);

        uint32_t cq_tail = ubLocalCqCi.GetValue(static_cast<uint32_t>(0));
        owner = (HtoNL(cq_tail) & (channel->cqe_cnt + 1)) == 0 ? 0 : 1;
        cq_mask = qp_info->cqe_cnt;
        auto cqeAddr = cq_base_buf + ((HtoNL(cq_tail) & cq_mask) * channel->cqe_size);
        cqGlobalTensor.SetGlobalBuffer(cqeAddr);

        Gm2Ub(ubLocalCq, cqGlobalTensor, cqeSize);

        if ((((cqeUB->cqe_0) >> 31) & 0x1 == owner) && ((cqeUB->cqe_0 & 0xfffff) == qp_info->qpn)) {
            break;
        }
    } while (1);

    uint32_t cq_tail = ubLocalCqCi.GetValue(static_cast<uint32_t>(0));
    cq_tail = HtoNL(HtoNL(cq_tail) + 1) ubLocalCqCi.SetValue(0, cq_tail);
    cq_tail += 1;
    ubLocalCqCi.SetValue(0, cq_tail);

    Ub2Gm(cqCiGlobalTensor, ubLocalCqCi, 1); // 更新 Cq Ci 到 gm

    Gm2Ub(ubLocalSqCi, sqCiGlobalTensor, 1);

    uint32_t sq_tail = ubLocalSqCi.GetValue(static_cast<uint32_t>(0));
    sq_tail += 1;
    ubLocalSqCi.SetValue(0, sq_tail);

    Ub2Gm(sqCiGlobalTensor, ubLocalSqCi, 1); // 更新 Sq Ci 到 gm
}
} // namespace AscendC

#endif
