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

constexpr uint64_t UB_FLAG_SIZE = 32;
constexpr uing64_t UB_FLAG_SIZE_5 = UB_FLAG_SIZE * 5;

constexpr uint64_t UB_FLAG_PAD_COUNT = 8;
constexpr uint64_t UB_ADDRESS_PAD_COUNT = 4;

__aicore__ inline Hcomm<COMM_PROTOCOL::ROCE, pipe>::Hcomm()
{
    pipe.InitBuffer(localFlagBuf, UB_FLAG_SIZE_5);
    // 后面的参数是啥意思
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

struct {
    uint32_t type : 5;
    uint32_t cos : 3;
    uint32_t c : 1;
    uint32_t n : 1;
    uint32_t cntx_size : 2;
    uint32_t qpn : 20;
    uint32_t queue_id : 4;
    uint32_t sgid_index : 7;
    uint32_t mtu_shift : 3;
    uint32_t vxlan : 1;
    uint32_t xrcvld : 1;
    uint32_t resv : 8;
    uint32_t pi : 8;
} RoceDbEntry;

struct {
    uint32_t cqe_0;
    uint32_t cqe_1;
    uint32_t cqe_2;
    uint32_t cqe_3;
    uint32_t cqe_4;
    uint32_t cqe_5;
    uint32_t cqe_6;
    uint32_t cqe_7;
} RoceCqEntry;

#define ROCE_SQ_DOORBELL_TYPE 2
#define ROCE_INIT_SQ_DB_SGIT_IDX 1

template <typename T>
__aicore__ inline void Gm2Ub(const LocalTensor<T>& dstLocal, __gm__ T* srcGlobal, int32_t length)
{
    GlobalTensor<T> SrcGlobalTensor;
    SrcGlobalTensor.SetGlobalBuffer(srcGlobal);
    DataCopyExtParams copyParams{1, length * sizeof(T), 0, 0, 0};
    DataCopyPadExtParams<uint32_t> padParams;
    padParams.isPad = 0;
    padParams.leftPadding = 0;
    padParams.rightPadding = 0;
    padParams.paddingValue = 0;

    PipeBarrier<PIPE_ALL>();
    DataCopyPad(dstLocal, SrcGlobalTensor, copyParams, padParams);
}

template <typename T>
__aicore__ inline void Ub2Gm(__gm__ T* dstGlobal, const LocalTensor<T>& srcLocal, int32_t length)
{
    GlobalTensor<T> DstGlobalTensor;
    DstGlobalTensor.SetGlobalBuffer(dstGlobal);
    DataCopyExtParams copyParams{1, length * sizeof(T), 0, 0, 0};

    PipeBarrier<PIPE_ALL>();
    DataCopyPad(DstGlobalTensor, srcLocal, copyParams);
}

template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<pipe> Commit(Handle handle)
{
    Channel* channel = getChannelFromHandle(handle);         // 获取channel
    __ubuf__ int32_t* sq_head = getHeadFromChannel(channel); // 获取当前channel维护的临时pi的地址
    uint64_t doorBellInfo = 0;
    doorBellInfo |= ROCE_SQ_DOORBELL_TYPE;
    doorBellInfo |= channel.sqContext.qpn << 12UL;
    doorBellInfo |= ROCE_INIT_SQ_DB_SGIT_IDX << 36UL;
    doorBellInfo |= (*sq_head) << 56UL;

    __gm__ uint64_t* doorBellAddr = (__gm__ uint64_t*)(channel->SqContext.RdmaSqContext.dbVa + (*sq_head >> 8) & 0xff);
    PipeBarrier<PIPE_ALL>();
    ubLocalDb.SetValue(0, doorBellInfo);

    GlobalTensor<uint64_t> DBGlobalTensor;
    DBGlobalTensor.SetGlobalBuffer(doorBellAddr);
    DataCopyExtParams copyParamsDb{1, 1 * sizeof(uint64_t), 0, 0, 0};
    PipeBarrier<PIPE_ALL>();

    SetAtomicAdd<uint64_t>();
    PipeBarrier<PIPE_ALL>();
    DataCopyPad(DBGlobalTensor, ubLocalDb, copyParamsDb);
    PipeBarrier<PIPE_ALL>();
    SetAtomicNone();

    ubLocalSqCi.SetValue(0, (uint32_t)(*sq_head));
    GlobalTensor<uint32_t> SqPiGlobalTensor;
    SqPiGlobalTensor.SetGlobalBuffer(channel->sq_pi_addr);
    DataCopyExtParams copyParamsSqPi{1, 1 * sizeof(uint32_t), 0, 0, 0};
    PipeBarrier<PIPE_ALL>();

    DataCopyPad(SqPiGlobalTensor, ubLocalSqCi, copyParamsSqPi);

    SetFlag<MTE3_MTE2>(EVENT_ID1);
    WaitFlag<MTE3_MTE2>(EVENT_ID1);
    PipeBarrier<PIPE_ALL>();
}

// 如果没有按顺序进行Wait，是否会产生影响？
// 每次 cq 的
template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<pipe> Wait(Handle handle)
{
    Channel* channel = getChannelFromHandle(handle);                   // 获取channel
    __gm__ uint8_t* cq_base_buf = (__gm__ uint8_t*)(channel->cq_addr); // 获取cq地址

    uint8_t owner;
    uint32_t cq_mask;

    GlobalTensor<uint32_t> CqCiGlobalTensor;
    SqPiGlobalTensor.SetGlobalBuffer(channel->cq_ci_addr);
    DataCopyExtParams copyParamsCqCi{1, 1 * sizeof(uint32_t), 0, 0, 0};
    DataCopyPadExtParams<uint32_t> padParamsCqCi;
    padParamsCqCi.isPad = 0;
    padParamsCqCi.leftPadding = 0;
    padParamsCqCi.rightPadding = 0;
    padParamsCqCi.paddingValue = 0;

    GlobalTensor<uint32_t> CqGlobalTensor;
    DataCopyExtParams copyParamsCq{1, channel->cqe_size * sizeof(uint32_t), 0, 0, 0};
    DataCopyPadExtParams<uint32_t> padParamsCq;
    padParamsCq.isPad = 0;
    padParamsCq.leftPadding = 0;
    padParamsCq.rightPadding = 0;
    padParamsCq.paddingValue = 0;

    do {
        pipe_barrier(PIPE_ALL);
        DataCopyPad(ubLocalCqCi, CqCiGlobalTensor, copyParamsCqCi, padParamsCqCi);
        SetFlag<MTE2_MTE3>(EVENT_ID2);
        Waitlag<MTE2_MTE3>(EVENT_ID2);
        PipeBarrier<PIPE_ALL>();

        uint32_t cq_tail = ubLocalSqCi.GetValue(static_cast<uint32_t>(0));

        owner = (HtoNL(cq_tail) & (channel->cqe_cnt + 1)) == 0 ? 0 : 1;
        cq_mask = qp_info->cqe_cnt;
        PipeBarrier<PIPE_ALL>();

        CqGlobalTensor.SetGlobalBuffer(
            (__gm__ int32_t*)(cq_base_buf + ((HtoNL(cq_tail) & cq_mask) * channel->cqe_size)));
        DataCopyPad(ubLocalCq, CqGlobalTensor, copyParamsCq, padParamsCq);

        SetFlag<MTE2_MTE3>(EVENT_ID2);
        Waitlag<MTE2_MTE3>(EVENT_ID2);
        PipeBarrier<PIPE_ALL>();
        uint32_t cqe_0 = ubLocalCq.GetValue(static_cast<uint32_t>(0));
        if ((((cqe_0) >> 31) & 0x1 == owner) && ((cqe_0 & 0xfffff) == qp_info->qpn)) {
            break;
        }
    } while (1);

    uint32_t cq_tail = ubLocalCqCi.GetValue(static_cast<uint32_t>(0));
    cq_tail = HtoNL(HtoNL(cq_tail) + 1) ubLocalCqCi.SetValue(0, cq_tail);
    cq_tail += 1;
    ubLocalCqCi.SetValue(0, sq_tail);

    PipeBarrier<PIPE_ALL>();
    DataCopyPad(CqCiGlobalTensor, ubLocalCqCi, copyParamsCqCi);

    GlobalTensor<uint32_t> SqCiGlobalTensor;
    SqCiGlobalTensor.SetGlobalBuffer(channel->sq_pi_addr);
    DataCopyExtParams copyParamsSqCi{1, 1 * sizeof(uint32_t), 0, 0, 0};
    DataCopyPadExtParams<uint32_t> padParams;
    padParams.isPad = 0;
    padParams.leftPadding = 0;
    padParams.rightPadding = 0;
    padParams.paddingValue = 0;

    SetFlag<MTE3_MTE2>(EVENT_ID1);
    Waitlag<MTE3_MTE2>(EVENT_ID1);
    PipeBarrier<PIPE_ALL>();

    DataCopyPad(ubLocalSqCi, SqCiGlobalTensor, copyParamsSqCi, padParams);

    SetFlag<MTE2_MTE3>(EVENT_ID2);
    Waitlag<MTE2_MTE3>(EVENT_ID2);
    PipeBarrier<PIPE_ALL>();
    uint32_t sq_tail = ubLocalSqCi.GetValue(static_cast<uint32_t>(0));
    sq_tail += 1;
    ubLocalSqCi.SetValue(0, sq_tail);
    PipeBarrier<PIPE_ALL>();
    DataCopyPad(SqCiGlobalTensor, ubLocalSqCi, copyParamsSqCi);

    SetFlag<MTE3_MTE2>(EVENT_ID1);
    WaitFlag<MTE3_MTE2>(EVENT_ID1);
    PipeBarrier<PIPE_ALL>();
}
} // namespace AscendC

#endif
