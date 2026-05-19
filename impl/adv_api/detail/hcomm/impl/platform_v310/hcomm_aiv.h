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
 * \brief Hcomm AIV implementation for V310
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/hcomm/impl/platform_v310/hcomm_aiv.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/activation/simplesoftmax.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCOMM_AIV_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_HCOMM_IMPL_PLATFORM_V310_HCOMM_AIV_H
#define IMPL_ADV_API_DETAIL_HCOMM_IMPL_PLATFORM_V310_HCOMM_AIV_H

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

template <typename T>
__aicore__ inline void Gm2Ub(
    const AscendC::LocalTensor<T>& dstLocal, const AscendC::GlobalTensor<T>& srcGlobal, int32_t length)
{
    AscendC::DataCopyExtParams copyParams{1, (uint32_t)(length * sizeof(T)), 0, 0, 0};
    AscendC::DataCopyPadExtParams<T> padParams{true, 0, (ONE_BLOCK_SIZE - sizeof(T)) / sizeof(T), 0};

    PipeBarrier<PIPE_ALL>();
    AscendC::DataCopyPad(dstLocal, srcGlobal, copyParams, padParams);
    PipeBarrier<PIPE_ALL>();
}

template <typename T>
__aicore__ inline void Ub2Gm(
    const AscendC::GlobalTensor<T>& dstGlobal, const AscendC::LocalTensor<T>& srcLocal, int32_t length)
{
    AscendC::DataCopyExtParams copyParams{1, (uint32_t)(length * sizeof(T)), 0, 0, 0};
    SyncAction<HardEvent::S_MTE3>();
    AscendC::DataCopyPad<T>(dstGlobal, srcLocal, copyParams);
    SyncAction<HardEvent::MTE3_S>();
}

__aicore__ inline HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::KnockDoorBell(__gm__ ChannelEntity* chnlPtr,
    uint32_t sqHead)
{
    GlobalTensor<uint8_t> dbGlobalTensor;
    __gm__ uint8_t* doorBellAddr =
        (__gm__ uint8_t*)(chnlPtr->sqContextAddr->contextInfo.roceSq.dbVa + (sqHead >> 8) & 0xff);
    dbGlobalTensor.SetGlobalBuffer(doorBellAddr);

    uint64_t dbValue = GetDbValue(chnlPtr->sqContextAddr->contextInfo.roceSq.qpn);
    __ubuf__ uint32_t* dbUB = (__ubuf__ uint32_t*)(dbAddr_);
    *(dbUB + 0) = (dbValue & 0xffffffff);
    *(dbUB + 1) = (dbValue >> 32) | ((sqHead >> 8) & 0xff);
#if defined(UT_TEST)
    uint64_t utDbAddr = reinterpret_cast<uint64_t>(dbAddr_) -
        (uint64_t)(GetTPipePtr()->GetBaseAddr(static_cast<int8_t>(TPosition::VECOUT)));
    LocalTensor<uint8_t> dbUBTensor = LocalTensor<uint8_t>(TPosition::VECOUT,
        static_cast<uint32_t>(utDbAddr), HCOMM_MEM_BLOCK_SIZE);
#else
    LocalTensor<uint8_t> dbUBTensor = LocalTensor<uint8_t>(TPosition::VECOUT,
        static_cast<uint32_t>(reinterpret_cast<uint64_t>(dbAddr_)), HCOMM_MEM_BLOCK_SIZE);
#endif
    // 此处想保证db写入为原子操作，穿刺中未验证操作正确性，是否需要换成SetAtomicAdd
    set_atomic_add();
    Ub2Gm<uint8_t>(dbGlobalTensor, dbUBTensor, sizeof(uint64_t));
    SetAtomicNone();
}

__aicore__ inline HcommHandle HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::Init(
    __ubuf__ uint8_t *buff, uint32_t len)
{
    ASCEND_HCCL_API_ASSERT(len >= ROCE_UB_BUF_SIZE, {return;}, "Call Init failed, ensure len is larger than 256.");
    __ubuf__ uint8_t *alignedAddr = AlignAddrTo32Bytes(buff);
    wqeAddr_ = alignedAddr;
    cqeAddr_ = alignedAddr + ROCE_CQE_POS;
    dbAddr_ = alignedAddr + ROCE_DB_POS;
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::PostSend(
    ChannelPtr channel, GM_ADDR dst, GM_ADDR src, uint64_t len, uint32_t opType)
{
    __gm__ ChannelEntity* chnlPtr = (__gm__ ChannelEntity*)(channel);
    auto sqPIAddr = chnlPtr->sqContextAddr->contextInfo.roceSq.headAddr;
    CacheWriteThrough((__gm__ uint8_t*)(sqPIAddr), sizeof(uint32_t));
    uint32_t sqHead = *(__gm__ uint32_t*)(sqPIAddr);
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: opType = %u, sqHead = %u\n", opType, sqHead);

    uint32_t sqDepth = chnlPtr->sqContextAddr->contextInfo.roceSq.depth;
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: sqDepth = %u\n", sqDepth);

    auto sqCIAddr = chnlPtr->sqContextAddr->contextInfo.roceSq.tailAddr;
#if !defined(UT_TEST)
    while (1) {
        CacheWriteThrough((__gm__ uint8_t*)(sqCIAddr), sizeof(uint32_t));
        uint32_t sqTail = *(__gm__ uint32_t*)(sqCIAddr);
        KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: sqTail = %u\n", sqTail);
        if (sqHead - sqTail < sqDepth - 1) {
            if (curHandleId_ >= 0) {
                Wait(curHandleId_);
            } else {
                KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: sq is full, but no wqe in sq.\n");
                return curHandleId_;
            }
        }
    }
#endif
    __ubuf__ RoceWqeEntry* wqePtr = (__ubuf__ RoceWqeEntry*)(wqeAddr_);
    uint8_t owner = (sqHead & sqDepth) == 0 ? 0 : 1;
    wqePtr->ctrl.dw0.value = HtoNL(
        owner << HCOMM_WQE_OWNER_OFFSET | 2U << HCOMM_WQE_CTRLSL_OFFSET | 1U << HCOMM_WQE_CR_OFFSET |
        1U << HCOMM_WQE_VA_OFFSET | 4U << HCOMM_WQE_TSL_OFFSET | 2U << HCOMM_WQE_BDSL_OFFSET);
    wqePtr->ctrl.dw1.value = HtoNL(1U << HCOMM_WQE_CL_OFFSET);
    wqePtr->doorbell = 0;
    wqePtr->task.dw0.value = HtoNL(opType << HCOMM_WQE_OP_TYPE_OFFSET | 1U << HCOMM_WQE_C_OFFSET);
    wqePtr->dataLen = HtoNL(len);
    wqePtr->immeData = 0;
    wqePtr->firstLast = 0;
    wqePtr->nxtEthHdr = 0;
    wqePtr->cmdLen = 0;
    wqePtr->rsvd0 = 0;
    wqePtr->lastExtLen = 0;
    wqePtr->vaHigh32 = HtoNL(((uint64_t)dst + len) >> 32);
    wqePtr->vaLow32 = HtoNL(((uint64_t)dst + len) & 0xffffffff);
    uint32_t rKey = chnlPtr->remoteBufferAddr.bufferInfo.rma.protectionInfo.memInfo.roce.rkey;
    wqePtr->rKey = HtoNL(rKey);
    wqePtr->rsvd1 = 0;
    wqePtr->data.bufAddrHigh32 = HtoNL((uint64_t)src >> 32);
    wqePtr->data.bufAddrLow32 = HtoNL((uint64_t)src & 0xffffffff);
    wqePtr->data.rLen = (uint32_t)len;
    uint32_t lKey = chnlPtr->localBufferAddr.bufferInfo.rma.protectionInfo.memInfo.roce.lkey;
    wqePtr->data.leKey = HtoNL(1 << 31 | lKey);
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: make wqe ok");

    __gm__ uint8_t* sqBaseAddr = (__gm__ uint8_t*)(chnlPtr->sqContextAddr->contextInfo.roceSq.sqVa);
    uint32_t sqPIMask = sqDepth - 1;
    uint32_t wqeSize = chnlPtr->sqContextAddr->contextInfo.roceSq.wqeSize;
    GlobalTensor<uint8_t> sqGlobal;
    auto sqAddr = sqBaseAddr + (sqHead & sqPIMask) * wqeSize;
    sqGlobal.SetGlobalBuffer((__gm__ uint8_t*)(sqAddr));

#if defined(UT_TEST)
    uint64_t utWqeAddr = reinterpret_cast<uint64_t>(wqeAddr_) -
        (uint64_t)(GetTPipePtr()->GetBaseAddr(static_cast<int8_t>(TPosition::VECOUT)));
    LocalTensor<uint8_t> wqeUB = LocalTensor<uint8_t>(TPosition::VECOUT,
        static_cast<uint32_t>(utWqeAddr), sizeof(RoceWqeEntry));
#else
    LocalTensor<uint8_t> wqeUB = LocalTensor<uint8_t>(TPosition::VECOUT,
        static_cast<uint32_t>(reinterpret_cast<uint64_t>(wqeAddr_)), sizeof(RoceWqeEntry));
#endif

    SyncAction<HardEvent::S_MTE3>();
    DataCopy(sqGlobal, wqeUB, sizeof(RoceWqeEntry));
    SyncAction<HardEvent::MTE3_S>();
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: set wqe to qp ok\n");

    HcommHandle handleId = ++curHandleId_;
    channelList_[handleId] = channel;
    sqHead++;
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: update PI in stack, sqHead = %u handleId = %d\n", sqHead, handleId);

    if constexpr (commit) {
        KnockDoorBell(chnlPtr, sqHead);
        handleCommitList_[handleId] = true;
        KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: Commit ok, handleId = %d\n", handleId);
    }
    *(__gm__ uint32_t*)(sqPIAddr) = sqHead;
    CacheWriteThrough((__gm__ uint8_t*)(sqPIAddr), sizeof(uint32_t));
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: update PI to GM sqHead = %u handleId = %d", sqHead, handleId);
    return handleId;
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::Write(
    ChannelPtr channel, GM_ADDR dst, GM_ADDR src, uint64_t len)
{
    return PostSend<commit, commitPipe, reqPipe>(channel, dst, src, len, static_cast<uint32_t>(HCOMM_OP_TYPE::WRITE));
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::Read(
    ChannelPtr channel, GM_ADDR dst, GM_ADDR src, uint64_t len)
{
    return PostSend<commit, commitPipe, reqPipe>(channel, dst, src, len, static_cast<uint32_t>(HCOMM_OP_TYPE::READ));
}

template <bool isWait>
__aicore__ inline bool HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::JudgeHandleId(HcommHandle handleId)
{
    // 判断handleId是否合法
    if (handleId < 0 || handleId >= 1024) {
        KERNEL_LOG(KERNEL_ERROR, "Judge HandleId failed without invalid handleId\n");
        return false;
    }
    if (channelList_[handleId] == 0) {
        KERNEL_LOG(KERNEL_ERROR, "Judge HandleId failed without valid channel\n");
        return false;
    }
    if constexpr (isWait) {
        // Wait操作，拒绝对未进行Commit或者已进行Wait的handleId进行操作
        if (!handleCommitList_[handleId]) {
            KERNEL_LOG(KERNEL_ERROR, "Wait Judge HandleId failed without Commit");
            return false;
        }
    } else {
        // Commit操作，拒绝对已进行Commit的handleId进行操作
        if (handleCommitList_[handleId]) {
            KERNEL_LOG(KERNEL_ERROR, "Commit Judge HandleId failed with already Commit");
            return false;
        }
    }
    return true;
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::Commit(HcommHandle handleId)
{
    KERNEL_LOG(KERNEL_INFO, "Hcomm Commit: opType = Commit, handleId = %u\n", handleId);
    if (!JudgeHandleId<false>(handleId)) {
        return HCOMM_FAILED;
    }
    ChannelPtr channel = channelList_[handleId];
    __gm__ ChannelEntity* chnlPtr = (__gm__ ChannelEntity*)(channel);
    auto sqPIAddr = chnlPtr->sqContextAddr->contextInfo.roceSq.headAddr;
    CacheWriteThrough((__gm__ uint8_t*)(sqPIAddr), sizeof(uint32_t));
    uint32_t sqHead = *(__gm__ uint32_t*)(sqPIAddr);
    KERNEL_LOG(KERNEL_INFO, "Hcomm PostSend: opType = %u, sqHead = %u\n", opType, sqHead);

    KnockDoorBell(chnlPtr, sqHead);

    // 从当前handleId向前遍历更新handleCommitList_
    for (HcommHandle i = handleId; i >= 0; i--) {
        if (channelList_[i] == channel) {
            if (!handleCommitList_[i]) {
                handleCommitList_[i] = true;
            } else {
                continue;
            }
        }
    }
    return HCOMM_SUCCESS;
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<CommProtocol::ROCE, CommEngine::AIV>::Wait(HcommHandle handleId)
{
    KERNEL_LOG(KERNEL_INFO, "Hcomm Wait: opType = Wait, handleId = %u\n", handleId);
    if (handleWaitList_[handleId]) {
        return HCOMM_SUCCESS;
    }
    if (!JudgeHandleId<true>(handleId)) {
        return HCOMM_FAILED;
    }
    ChannelPtr channel = channelList_[handleId];
    __gm__ ChannelEntity* chnlPtr = (__gm__ ChannelEntity*)(channel); // 获取channel
    auto sqCIAddr = chnlPtr->sqContextAddr->contextInfo.roceSq.tailAddr;
    auto cqCIAddr = chnlPtr->cqContextAddr->contextInfo.roceCq.tailAddr;
    uint32_t cqeSize = chnlPtr->cqContextAddr->contextInfo.roceCq.cqeSize;
    uint32_t cqDepth = chnlPtr->cqContextAddr->contextInfo.roceCq.cqDepth;
    uint32_t cqn = chnlPtr->cqContextAddr->contextInfo.roceCq.cqn;
    uint32_t cqTail;
#if defined(UT_TEST)
    uint64_t utCqeAddr = reinterpret_cast<uint64_t>(cqeAddr_) -
        (uint64_t)(GetTPipePtr()->GetBaseAddr(static_cast<int8_t>(TPosition::VECOUT)));
    LocalTensor<uint8_t> cqeUB = LocalTensor<uint8_t>(TPosition::VECOUT,
        static_cast<uint32_t>(utCqeAddr), sizeof(RoceCqeEntry));
#else
    LocalTensor<uint8_t> cqeUB = LocalTensor<uint8_t>(TPosition::VECOUT,
        static_cast<uint32_t>(reinterpret_cast<uint64_t>(cqeAddr_)), sizeof(RoceCqeEntry));
#endif
    __ubuf__ RoceCqeEntry* cqeUBPtr = (__ubuf__ RoceCqeEntry*)(cqeAddr_);
    CacheWriteThrough((__gm__ uint8_t*)(cqCIAddr), sizeof(uint32_t));
    cqTail = *(__gm__ uint32_t*)(cqCIAddr);
    __gm__ uint8_t* cqBaseBuf = (__gm__ uint8_t*)(chnlPtr->cqContextAddr->contextInfo.roceCq.dbVa);
    auto cqeAddr = cqBaseBuf + ((HtoNL(cqTail) & cqDepth) * cqeSize);
    AscendC::GlobalTensor<uint32_t> cqeGlobalTensor;
    cqeGlobalTensor.SetGlobalBuffer((__gm__ uint8_t*)cqeAddr);
    do {
        SyncAction<HardEvent::S_MTE2>();
        DataCopy(cqeUB, cqeGlobalTensor, sizeof(RoceCqeEntry));
        SyncAction<HardEvent::MTE2_S>();
#if defined(UT_TEST)
        break;
#else
        uint8_t owner = (HtoNL(cqTail) & (cqDepth + 1)) == 0 ? 0 : 1;
        if (((((cqeUBPtr->cqe0) >> 31) & 0x1) == owner) && ((cqeUBPtr->cqe0 & 0xfffff) == cqn)) {
            break;
        }
#endif
    } while (1);

    cqTail = HtoNL(HtoNL(cqTail) + 1);
    CacheWriteThrough((__gm__ uint8_t*)(cqCIAddr), sizeof(uint32_t));
    CacheWriteThrough((__gm__ uint8_t*)(sqCIAddr), sizeof(uint32_t));
    uint32_t sqTail = *(__gm__ uint32_t*)(sqCIAddr);
    sqTail += 1;
    *(__gm__ uint32_t*)(sqCIAddr) = sqTail;
    CacheWriteThrough((__gm__ uint8_t*)(sqCIAddr), sizeof(uint32_t));

    // 从当前handleId向前遍历处理handleWaitList_
    for (HcommHandle i = handleId; i >= 0; i--) {
        if (channelList_[i] == channel) {
            if (!handleWaitList_[i]) {
                handleWaitList_[i] = true;
            } else {
                continue;
            }
        }
    }
    return HCOMM_SUCCESS;
}
} // namespace AscendC

#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCOMM_AIV_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCOMM_AIV_H__
#endif
