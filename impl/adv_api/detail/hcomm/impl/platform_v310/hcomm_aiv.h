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
__aicore__ inline void Gm2Ub(const AscendC::LocalTensor<T>& dstLocal, const AscendC::GlobalTensor<T>& srcGlobal,
                             int32_t length)
{
    AscendC::DataCopyExtParams copyParams{1, (uint32_t)(length * sizeof(T)), 0, 0, 0};
    AscendC::DataCopyPadExtParams<T> padParams{0, 0, 0, 0};

    PipeBarrier<PIPE_ALL>();
    AscendC::DataCopyPad(dstLocal, srcGlobal, copyParams, padParams);
    PipeBarrier<PIPE_ALL>();
}

template <typename T>
__aicore__ inline void Ub2Gm(const AscendC::GlobalTensor<T>& dstGlobal, const AscendC::LocalTensor<T>& srcLocal,
                             int32_t length)
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
    __gm__ Channel* channelPtr = (__gm__ Channel*)(channel);
    auto sqPIAddr = (__gm__ uint32_t*)(channelPtr->sqContextAddr->ctx.rdmaSqContext.headAddr);
    GlobalTensor<uint32_t> sqPIGlobal;
    sqPIGlobal.SetGlobalBuffer(sqPIAddr);
    Gm2Ub<uint32_t>(sqPI_, sqPIGlobal, 1);
    uint32_t sqHead = sqPI_.GetValue(0);

    auto sqCIAddr = (__gm__ uint32_t*)(channelPtr->sqContextAddr->ctx.rdmaSqContext.tailAddr);
    GlobalTensor<uint32_t> sqCIGlobal;
    sqCIGlobal.SetGlobalBuffer(sqCIAddr);
    uint32_t sqDepth = channelPtr->sqContextAddr->ctx.rdmaSqContext.depth;

    RoceWqeEntry* wqePtr = (RoceWqeEntry*)(wqeItem_.GetPhyAddr());
    uint8_t owner = (sqHead & sqDepth) == 0 ? 0 : 1;
    wqePtr->ctrl.dw0.value = HtoNL(owner << OWNER_OFFSET | 2U << CTRLSL_OFFSET | 1U << CR_OFFSET | 1U << VA_OFFSET
                                   | 4U << TSL_OFFSET | 2U << BDSL_OFFSET);
    wqePtr->ctrl.dw1.value = HtoNL(1U << CL_OFFSET);
    wqePtr->doorbell = 0;
    wqePtr->task.dw0.value = HtoNL(opType << OP_TYPE_OFFSET | 1U << C_OFFSET);
    wqePtr->dataLen = HtoNL(len);
    wqePtr->immeData = 0;
    wqePtr->firstLast = 0;
    wqePtr->nxtEthHdr = 0;
    wqePtr->cmdLen = 0;
    wqePtr->rsvd0 = 0;
    wqePtr->lastExtLen = 0;
    wqePtr->vaHigh32 = HtoNL(((uint64_t)dst + len) >> 32);
    wqePtr->vaLow32 = HtoNL(((uint64_t)dst + len) & 0xffffffff);
    uint32_t rKey = channelPtr->remoteBufferAddr->pti.rdmaMemProtectionInfo.rkey;
    wqePtr->rKey = HtoNL(rKey);
    wqePtr->rsvd1 = 0;
    wqePtr->data.bufAddrHigh32 = HtoNL((uint64_t)src >> 32);
    wqePtr->data.bufAddrLow32 = HtoNL((uint64_t)src & 0xffffffff);
    wqePtr->data.rLen = (uint32_t)len;
    uint32_t lKey = channelPtr->localBufferAddr->pti.rdmaMemProtectionInfo.lkey;
    wqePtr->data.leKey = HtoNL(1 << 31 | lKey);
    __gm__ uint8_t* sqBaseAddr = (__gm__ uint8_t*)(channelPtr->sqContextAddr->ctx.rdmaSqContext.sqVa);
    uint32_t sqPIMask = sqDepth - 1;
    uint32_t wqeSize = channelPtr->sqContextAddr->ctx.rdmaSqContext.wqeSize;
    GlobalTensor<uint8_t> sqGlobal;
    sqGlobal.SetGlobalBuffer((__gm__ uint8_t*)(sqBaseAddr + (sqHead & sqPIMask) * wqeSize));

    PipeBarrier<PIPE_ALL>();
    DataCopy(sqGlobal, wqeItem_, DEFAULT_WQE_ITEM_SIZE);
    PipeBarrier<PIPE_ALL>();

    HcommHandle handleId = ++curHandleId_;
    channelList_[handleId] = channel;
    sqHead++;
    sqPI_.SetValue(0, sqHead);

    if constexpr (commit) {
        Commit<PIPE_MTE3>(handleId);
    }
    Ub2Gm<uint32_t>(sqPIGlobal, sqPI_, 1);
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
    __gm__ Channel* channel = (__gm__ Channel*)channelList_[handle]; // 获取channel
    KERNEL_LOG(KERNEL_INFO, "Hcomm Commit handle: %u channelHandle:%llu", handle, channel);
    AscendC::GlobalTensor<uint32_t> sqPIGlobalTensor;
    sqPIGlobalTensor.SetGlobalBuffer((__gm__ uint32_t*)channel->sqContextAddr->ctx.rdmaSqContext.headAddr);
    uint32_t xxx = sqPIGlobalTensor.GetValue(0); // 获取当前channel对应的sq pi
    KERNEL_LOG(KERNEL_INFO, "xxx: %llu, headAddr: %llu", xxx, channel->sqContextAddr->ctx.rdmaSqContext.headAddr);
    Gm2Ub(sqPI_, sqPIGlobalTensor, 1);
    uint32_t sqHead = sqPI_.GetValue(0); // 获取当前channel对应的sq pi
    KERNEL_LOG(KERNEL_INFO, "sqHead: %llu", sqHead);

    GlobalTensor<uint64_t> dbGlobalTensor;
    __gm__ uint64_t* doorBellAddr =
        (__gm__ uint64_t*)(channel->sqContextAddr->ctx.rdmaSqContext.dbVa + (sqHead >> 8) & 0xff);
    dbGlobalTensor.SetGlobalBuffer((__gm__ uint64_t*)doorBellAddr);

    uint64_t dbValue = GetDbValue(channel->sqContextAddr->ctx.rdmaSqContext.qpn);
    KERNEL_LOG(KERNEL_INFO, "dbValue: %llu", dbValue);
    __ubuf__ uint32_t* dbUB = (__ubuf__ uint32_t*)(doorBell_.GetPhyAddr());
    *(dbUB + 0) = (dbValue & 0xffffffff);
    *(dbUB + 1) = (dbValue >> 32) | ((sqHead >> 8) & 0xff);
    KERNEL_LOG(KERNEL_INFO, "doorBell_0: %llu, doorBell_1: %llu", *(dbUB + 0), *(dbUB + 1));

    set_atomic_add();
    Ub2Gm(dbGlobalTensor, doorBell_, 1);
    set_atomic_none();
    return HCOMM_SUCCESS;
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Wait(HcommHandle handle)
{
    __gm__ Channel* channel = (__gm__ Channel*)channelList_[handle]; // 获取channel
    KERNEL_LOG(KERNEL_INFO, "Hcomm Wait handle: %u channelHandle:%llu", handle, channel);
    auto sqCIAddr = channel->sqContextAddr->ctx.rdmaSqContext.tailAddr;
    GlobalTensor<uint32_t> sqCIGlobalTensor;
    sqCIGlobalTensor.SetGlobalBuffer((__gm__ uint32_t*)sqCIAddr); // 获取Sq CI对应的global tensor
    uint32_t sqCI_0 = sqCIGlobalTensor.GetValue(0); // 获取当前channel对应的sq pi
    KERNEL_LOG(KERNEL_INFO, "sqCI_0: %llu", sqCI_0);

    auto cqCIAddr = channel->cqContextAddr->ctx.rdmaCqContext.tailAddr;
    GlobalTensor<uint32_t> cqCIGlobalTensor;
    cqCIGlobalTensor.SetGlobalBuffer((__gm__ uint32_t*)cqCIAddr); // 获取Cq CI对应的global tensor

    uint8_t owner;
    uint32_t cqTail;
    uint32_t cqMask;
    uint32_t cqeSize = channel->cqContextAddr->ctx.rdmaCqContext.cqeSize;
    uint32_t cqDepth = channel->cqContextAddr->ctx.rdmaCqContext.cqDepth;
    uint32_t cqn = channel->cqContextAddr->ctx.rdmaCqContext.cqn;
    __gm__ uint8_t* cqBaseBuf = (__gm__ uint8_t*)(channel->cqContextAddr->ctx.rdmaCqContext.dbVa);

    GlobalTensor<uint32_t> cqeGlobalTensor;
    __ubuf__ RoceCqeEntry* cqeUB = (__ubuf__ RoceCqeEntry*)(cqeItem_.GetPhyAddr());

    do {
        Gm2Ub(cqCI_, cqCIGlobalTensor, 1);
        cqTail = cqCI_.GetValue(static_cast<uint32_t>(0));

        owner = (HtoNL(cqTail) & (cqDepth + 1)) == 0 ? 0 : 1;
        auto cqeAddr = cqBaseBuf + ((HtoNL(cqTail) & cqDepth) * cqeSize);
        cqeGlobalTensor.SetGlobalBuffer((__gm__ uint32_t*)cqeAddr);

        Gm2Ub(cqeItem_, cqeGlobalTensor, 8);
        break;
    } while (1);

    cqTail = HtoNL(HtoNL(cqTail) + 1);
    cqTail += 1;
    cqCI_.SetValue(0, cqTail);

    Ub2Gm(cqCIGlobalTensor, cqCI_, 1); // 更新 Cq CI 到 gm

    Gm2Ub(sqCI_, sqCIGlobalTensor, 1);

    uint32_t lsqCI_0 = sqCI_.GetValue(0);
    KERNEL_LOG(KERNEL_INFO, "lsqCI_0: %llu", lsqCI_0);
    uint32_t sqTail = sqCI_.GetValue(static_cast<uint32_t>(0));
    sqTail += 1;
    sqCI_.SetValue(0, sqTail);

    uint32_t lsqCI_1 = sqCI_.GetValue(0);
    KERNEL_LOG(KERNEL_INFO, "lsqCI_1: %llu", lsqCI_1);
    Ub2Gm(sqCIGlobalTensor, sqCI_, 1); // 更新 Sq CI 到 gm
    uint32_t sqCI_1 = sqCIGlobalTensor.GetValue(0); // 获取当前channel对应的sq pi
    KERNEL_LOG(KERNEL_INFO, "sqCI_1: %llu", sqCI_1);
    return HCOMM_SUCCESS;
}
} // namespace AscendC

#endif
