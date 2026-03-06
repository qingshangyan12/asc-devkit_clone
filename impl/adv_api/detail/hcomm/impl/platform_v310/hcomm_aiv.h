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
 * \file hcomm_roce.h
 * \brief
 */
#ifndef IMPL_V310_HCOMM_AIV_H
#define IMPL_V310_HCOMM_AIV_H

#include "hcomm_aiv_def.h"
#include "../../common/hcomm_utils.h"
#include "../../common/hcomm_inner_def.h"

namespace AscendC {

__aicore__ inline uint64_t GetDbValue(uint32_t qpn)
{
    RoceDbEntry dbEntry = {0};
    dbEntry.dw0.bs.type = ROCE_SQ_DOORBELL_TYPE;
    dbEntry.dw0.bs.c = 0;
    dbEntry.dw0.bs.n = 0;
    dbEntry.dw0.bs.cntxSize = 1;
    dbEntry.dw0.bs.qpn = qpn;
    dbEntry.dw0.bs.mtuShift = 0;
    dbEntry.dw0.bs.resv = 0;
    dbEntry.dw0.bs.pi = 0;
    dbEntry.dw0.bs.sgidIndex = ROCE_INIT_SQ_DB_SGIT_IDX;
    dbEntry.dw0.bs.cos = 0x7;
    return dbEntry.dw0.value;
}

__aicore__ inline uint32_t HtoNL(uint32_t x)
{
    return ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8)
            | (((x) & 0x000000ff) << 24));
}

template <typename T>
__aicore__ inline void Gm2Ub(const AscendC::LocalTensor<T>& dstLocal, const AscendC::GlobalTensor<T>& srcGlobal, int32_t length)
{
    AscendC::DataCopyExtParams copyParams{1, (uint32_t)(length * sizeof(T)), 0, 0, 0};
    AscendC::DataCopyPadExtParams<T> padParams{0, 0, 0, 0};

    PipeBarrier<PIPE_ALL>();
    AscendC::DataCopyPad(dstLocal, srcGlobal, copyParams, padParams);
    PipeBarrier<PIPE_ALL>();
}

template <typename T>
__aicore__ inline void Ub2Gm(const AscendC::GlobalTensor<T>& dstGlobal, const AscendC::LocalTensor<T>& srcLocal, int32_t length)
{
    AscendC::DataCopyExtParams copyParams{1, (uint32_t)(length * sizeof(T)), 0, 0, 0};
    PipeBarrier<PIPE_ALL>();
    AscendC::DataCopyPad(dstGlobal, srcLocal, copyParams);
    PipeBarrier<PIPE_ALL>();
}

__aicore__ inline HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::HcommImpl()
{
    pipe_.InitBuffer(wqeInBuf, 32);
    wqeItem_ = wqeInBuf.Get<uint8_t>();

    pipe_.InitBuffer(cqeInBuf, 32);
    cqeItem_ = cqeInBuf.Get<uint32_t>();

    pipe_.InitBuffer(sqPIInBuf, 32);
    sqPI_ = sqPIInBuf.Get<uint32_t>();

    pipe_.InitBuffer(sqCIInBuf, 32);
    sqCI_ = sqCIInBuf.Get<uint32_t>();

    pipe_.InitBuffer(cqCIInBuf, 32);
    cqCI_ = cqCIInBuf.Get<uint32_t>();

    pipe_.InitBuffer(dbInBuf, 32);
    doorBell_ = dbInBuf.Get<uint64_t>();
}

__aicore__ inline HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::~HcommImpl() {}

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Operate(ChannelHandle channel,
                                                                                        GM_ADDR dst, GM_ADDR src,
                                                                                        uint64_t len, uint32_t opType)
{
    HcommHandle handleId = ++curHandleId_;
    channelList_[handleId] = channel;
    return handleId;
}

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle
HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Write(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len)
{
    return Operate<true, PIPE_MTE3, PIPE_MTE3>(channel, dst, src, len, 4U);
}

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Read(ChannelHandle channel, GM_ADDR dst,
                                                                                     GM_ADDR src, uint64_t len)
{
    return Operate<true, PIPE_MTE3, PIPE_MTE3>(channel, dst, src, len, 8U);
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Commit(HcommHandle handle)
{
    KERNEL_LOG(KERNEL_INFO, "sqPI_.GetValue: %llu", sqPI_.GetValue(0));
    __gm__ Channel* channel = (__gm__ Channel*)channelList_[handle]; // 获取channel
    KERNEL_LOG(KERNEL_INFO, "Hcomm Commit handle: %u channelHandle:%llu", handle, channel);
    AscendC::GlobalTensor<uint32_t> sqPIGlobalTensor;
    sqPIGlobalTensor.SetGlobalBuffer((__gm__ uint32_t*)channel->sqContextAddr->ctx.rdmaSqContext.headAddr);
    uint32_t xxx = sqPIGlobalTensor.GetValue(0); // 获取当前channel对应的sq pi
    KERNEL_LOG(KERNEL_INFO, "xxx: %llu, headAddr: %llu", xxx, channel->sqContextAddr->ctx.rdmaSqContext.headAddr);
    Gm2Ub(sqPI_, sqPIGlobalTensor, 1);
    KERNEL_LOG(KERNEL_INFO, "sqHead 0: %llu", sqPI_.GetValue(0));
    PipeBarrier<PIPE_ALL>();
    KERNEL_LOG(KERNEL_INFO, "sqHead 1: %llu", sqPI_.GetValue(0));
    sqPI_.SetValue(0, (uint32_t)(0));
    KERNEL_LOG(KERNEL_INFO, "sqHead 2: %llu", sqPI_.GetValue(0));
    PipeBarrier<PIPE_ALL>();
    uint32_t sqHead = sqPI_.GetValue(0); // 获取当前channel对应的sq pi
    KERNEL_LOG(KERNEL_INFO, "sqHead: %llu", sqHead);

    return HCOMM_SUCCESS;
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Wait(HcommHandle handle)
{
    __gm__ Channel* channel = (__gm__ Channel*)channelList_[handle]; // 获取channel
    KERNEL_LOG(KERNEL_INFO, "Hcomm Wait handle: %u channelHandle:%llu", handle, channel);
    auto sqCIAddr = channel->sqContextAddr->ctx.rdmaSqContext.tailAddr;
    KERNEL_LOG(KERNEL_INFO, "sqCIAddr: %p", sqCIAddr);
    KERNEL_LOG(KERNEL_INFO, "sq pi local: %p", (uint64_t)(sqPI_.GetPhyAddr()));
    KERNEL_LOG(KERNEL_INFO, "sq ci local: %p", (uint64_t)(sqCI_.GetPhyAddr()));
    KERNEL_LOG(KERNEL_INFO, "cq ci local: %p", (uint64_t)(cqCI_.GetPhyAddr()));
    KERNEL_LOG(KERNEL_INFO, "doorBell local: %p", (uint64_t)(doorBell_.GetPhyAddr()));

    return HCOMM_SUCCESS;
}
} // namespace AscendC

#endif

