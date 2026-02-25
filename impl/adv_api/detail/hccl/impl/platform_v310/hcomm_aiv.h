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

#define ROCE_SQ_DOORBELL_TYPE 2
#define ROCE_INIT_SQ_DB_SGIT_IDX 1

__aicore__ inline void GetDbValue(const LocalTensor<T>& localTensor, uint32_t qpn)
{
    RoceDbEntry *dbEntry = (RoceDbEntry *)localTensor.GetPhyAddr()
    dbEntry->type = ROCE_SQ_DOORBELL_TYPE;
    dbEntry->c = 0;
    dbEntry->r = 0;
    dbEntry->cntx_size = 1;
    dbEntry->qpn = qpn;
    dbEntry->sub_type = 0;
    dbEntry->resv = 0;
    dbEntry->pi = 0;
    dbEntry->sgid_index = ROCE_INIT_SQ_DB_SGIT_IDX;
    dbEntry->cos = 0x7;
}

__aicore__ inline uint32_t HtoNL(uint32_t x)
{
    return ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8)
            | (((x) & 0x000000ff) << 24));
}

__aicore__ inline Hcomm<COMM_PROTOCOL::ROCE, pipe>:: Hcomm()
{
    pipe.InitBuffer(wqeBuf_, 1, DEFAULT_WQE_ITEM_SIZE);
    pipe.InitBuffer(cqeBuf_, 1, ONE_BLOCK_SIZE);
    pipe.InitBuffer(sqPIBuf_, 1, ONE_BLOCK_SIZE);
    pipe.InitBuffer(sqCIBuf_, 1, ONE_BLOCK_SIZE);
    pipe.InitBuffer(cqCIBuf_, 1, ONE_BLOCK_SIZE);
    pipe.InitBuffer(doorBellBuf_, 1, ONE_BLOCK_SIZE);

    wqeItem_ = wqeBuf_.AllocTensor<uint8_t>();
    cqeItem_ = cqeBuf_.AllocTensor<uint64_t>();
    sqPI_ = sqPIBuf_.AllocTensor<uint32_t>();
    sqCI_ = sqCIBuf_.AllocTensor<uint32_t>();
    cqCI_ = cqCIBuf_.AllocTensor<uint32_t>();
    doorBell_ = doorBellBuf_.AllocTensor<uint32_t>();
}

__aicore__ inline Hcomm<COMM_PROTOCOL::ROCE, pipe>:: ~Hcomm()
{
    wqeBuf_.FreeTensor(wqeItem_);
    wqeBuf_.FreeTensor(cqeItem_);
    wqeBuf_.FreeTensor(sqPI_);
    wqeBuf_.FreeTensor(sqCI_);
    wqeBuf_.FreeTensor(cqCI_);
    wqeBuf_.FreeTensor(doorBell_);
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
    Channel* channel = channelList_[handle]; // 获取channel
    Gm2Ub(sqPI_, channel->SqContextAddr->RdmaSqContext.headAddr, 1);
    uint32_t sqHead = sqPI_.GetValue(static_cast<uint32_t>(0)); // 获取当前channel对应的sq pi

    GlobalTensor<uint64_t> dbGlobalTensor;
    __gm__ uint64_t* doorBellAddr =
        (__gm__ uint64_t*)(channel->SqContextAddr->RdmaSqContext.dbVa + (sqHead >> 8) & 0xff);
    dbGlobalTensor.SetGlobalBuffer(doorBellAddr);

    GetDbValue(doorBell_, channel->SqContextAddr->RdmaSqContext.qpn);
    uint32_t *dbUB = (uint32_t *)(doorBell_.GetPhyAddr());
    *(dbUB + 1) |= ((sqHead >> 8) & 0xff);

    SetAtomicAdd<uint64_t>();
    Ub2Gm(dbGlobalTensor, doorBell_, 1);
    SetAtomicNone();
}

template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<pipe> Wait(Handle handle)
{
    Channel* channel = channelList_[handle]; // 获取channel
    auto sqCIAddr = channel->SqContextAddr->RdmaSqContext.tailAddr;
    GlobalTensor<uint32_t> sqCiGlobalTensor;
    sqCiGlobalTensor.SetGlobalBuffer(sqCIAddr); // 获取Sq CI对应的global tensor

    auto cqCIAddr = channel->CqContextAddr->RdmaCqContext.tailAddr;
    GlobalTensor<uint32_t> cqCIGlobalTensor;
    cqCIGlobalTensor.SetGlobalBuffer(cqCIAddr); // 获取Cq CI对应的global tensor

    uint8_t owner;
    uint32_t cqTail;
    uint32_t cqMask;
    uint32_t cqeSize = channel->CqContextAddr->RdmaCqContext.cqeSize;
    uint32_t cqDepth = channel->CqContextAddr->RdmaCqContext.cqDepth;
    uint32_t cqn = channel->CqContextAddr->RdmaCqContext.cqn;
    __gm__ uint8_t* cqBaseBuf = (__gm__ uint8_t*)(channel->CqContextAddr->cqVa); 

    GlobalTensor<uint32_t> cqGlobalTensor;
    RoceCqEntry* cqeUB = (RoceCqEntry*)(cqeItem_.GetPhyAddr());

    do {
        Gm2Ub(cqCI_, cqCIGlobalTensor, 1);
        cqTail = cqCI_.GetValue(static_cast<uint32_t>(0));

        owner = (HtoNL(cqTail) & (cqDepth + 1)) == 0 ? 0 : 1;
        auto cqeAddr = cqBaseBuf + ((HtoNL(cqTail) & cqDepth) * cqeSize);
        cqGlobalTensor.SetGlobalBuffer(cqeAddr);

        Gm2Ub(cqeItem_, cqGlobalTensor, 8);

        if ((((cqeUB->cqe0) >> 31) & 0x1 == owner) && ((cqeUB->cqe0 & 0xfffff) == cqn)) {
            break;
        }
    } while (1);

    cqTail = HtoNL(HtoNL(cqTail) + 1) cqCI_.SetValue(0, cqTail);
    cqTail += 1;
    cqCI_.SetValue(0, cqTail);

    Ub2Gm(cqCIGlobalTensor, cqCI_, 1); // 更新 Cq CI 到 gm

    Gm2Ub(sqCI_, sqCiGlobalTensor, 1);

    uint32_t sqTail = sqCI_.GetValue(static_cast<uint32_t>(0));
    sqTail += 1;
    sqCI_.SetValue(0, sqTail);

    Ub2Gm(sqCiGlobalTensor, sqCI_, 1); // 更新 Sq CI 到 gm
}
} // namespace AscendC

#endif
