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
 * \file hcomm_aiv_urma.h
 * \brief Hcomm AIV URMA implementation for V310
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/hcomm/impl/platform_v310/hcomm_aiv_urma.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCOMM_AIV_URMA_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_HCOMM_IMPL_PLATFORM_V310_HCOMM_AIV_URMA_H
#define IMPL_ADV_API_DETAIL_HCOMM_IMPL_PLATFORM_V310_HCOMM_AIV_URMA_H

#include "hcomm_aiv_urma_def.h"

#include "../../common/hcomm_inner_def.h"
#include "../../common/hcomm_utils.h"

typedef AscendC::HcommUrmaSqeCtx HcommUrmaSqeCtx;
typedef AscendC::HcommUrmaSgeCtx HcommUrmaSgeCtx;
typedef AscendC::HcommUrmaNotifyCtx HcommUrmaNotifyCtx;
typedef AscendC::HcommUrmaJfcCqeCtx HcommUrmaJfcCqeCtx;

namespace AscendC {
__aicore__ inline uint32_t HcommUrmaGetBaseBlockCount(const SqContext& sqCtx)
{
    return sqCtx.contextInfo.ubJfs.sqDepth * AscendC::HCOMM_URMA_MAX_SQE_BB_NUM;
}

__aicore__ inline void HcommUrmaFillNotifyCtx(__ubuf__ HcommUrmaNotifyCtx* notifyCtx,
    const RegedBufferEntity& remoteMemInfo, GM_ADDR notifyAddr, uint64_t notifyVal)
{
    uint32_t tokenId = remoteMemInfo.bufferInfo.rma.protectionInfo.memInfo.ub.tokenId;
    uint32_t tokenValue = remoteMemInfo.bufferInfo.rma.protectionInfo.memInfo.ub.tokenValue;
    uint64_t notifyAddrValue = reinterpret_cast<uint64_t>(notifyAddr);
    notifyCtx->notifyTokenId = (tokenId >> 8) & 0xFFFFFU;
    notifyCtx->notifyTokenValue = tokenValue;
    notifyCtx->notifyAddrL = notifyAddrValue & 0xFFFFFFFFU;
    notifyCtx->notifyAddrH = (notifyAddrValue >> 32) & 0xFFFFFFFFU;
    notifyCtx->notifyDataL = notifyVal & 0xFFFFFFFFU;
    notifyCtx->notifyDataH = (notifyVal >> 32) & 0xFFFFFFFFU;
}

template <HcommUrmaOpCode opCode, auto const &config>
__aicore__ inline void HcommUrmaFillSqeCtx(__ubuf__ HcommUrmaSqeCtx* sqeCtx, __gm__ uint8_t* dst,
    const SqContext& sqCtx, const RegedBufferEntity& remoteMemInfo, uint32_t curHead, GM_ADDR notifyAddr = nullptr,
    uint64_t notifyVal = 0)
{
    sqeCtx->opcode = static_cast<uint32_t>(opCode);
    sqeCtx->flag = (config.odr & 0x7U) | ((config.fence & 0x1U) << 3U) | ((config.se & 0x1U) << 4U) |
        ((config.cqe & 0x1U) << 5U) | ((config.inlineEn & 0x1U) << 6U) | ((config.udfFlag & 0x1U) << 7U);
    sqeCtx->nf = 0;
    sqeCtx->tokenEn = 1;
    sqeCtx->rmtJettyType = 1;
    uint32_t baseBlockCount = HcommUrmaGetBaseBlockCount(sqCtx);
    sqeCtx->owner = (curHead & baseBlockCount) == 0 ? 1 : 0;
    sqeCtx->targetHint = 0;
    sqeCtx->inlineMsgLen = 0;
    sqeCtx->tpId = sqCtx.contextInfo.ubJfs.tpID;
    sqeCtx->sgeNum = 1;
    uint32_t tokenId = remoteMemInfo.bufferInfo.rma.protectionInfo.memInfo.ub.tokenId;
    uint32_t tokenValue = remoteMemInfo.bufferInfo.rma.protectionInfo.memInfo.ub.tokenValue;
    sqeCtx->rmtJettyOrSegId = tokenId >> 8;
    sqeCtx->rmtTokenValue = tokenValue;
    uint64_t remoteAddrValue = reinterpret_cast<uint64_t>(dst);
    sqeCtx->rmtAddrLOrTokenId = remoteAddrValue & 0xFFFFFFFFU;
    sqeCtx->rmtAddrHOrTokenValue = (remoteAddrValue >> 32) & 0xFFFFFFFFU;
    auto rmtEid = reinterpret_cast<const uint64_t*>(sqCtx.contextInfo.ubJfs.remoteEID);
    sqeCtx->rmtEidL = rmtEid[0];
    sqeCtx->rmtEidH = rmtEid[1];
    if constexpr (opCode == HcommUrmaOpCode::WRITE_WITH_NOTIFY) {
        __ubuf__ HcommUrmaNotifyCtx* notifyCtx =
            (__ubuf__ HcommUrmaNotifyCtx*)((__ubuf__ uint8_t*)sqeCtx + sizeof(HcommUrmaSqeCtx));
        HcommUrmaFillNotifyCtx(notifyCtx, remoteMemInfo, notifyAddr, notifyVal);
    }
}

__aicore__ inline void HcommUrmaFillSgeCtx(__ubuf__ HcommUrmaSgeCtx* sgeCtx, uint64_t messageLen, __gm__ uint8_t* src)
{
    sgeCtx->len = static_cast<uint32_t>(messageLen);
    sgeCtx->va = reinterpret_cast<uint64_t>(src);
}

__aicore__ inline void HcommUrmaDumpSgeCtx(__ubuf__ HcommUrmaSqeCtx* sqeCtx, __ubuf__ uint8_t* sgeAddr)
{
    if (sqeCtx == nullptr || sgeAddr == nullptr) {
        KERNEL_LOG(KERNEL_INFO, "Hcomm URMA SGE: nullptr");
        return;
    }
    __ubuf__ HcommUrmaSgeCtx* sgeCtx = (__ubuf__ HcommUrmaSgeCtx*)sgeAddr;
    for (uint32_t i = 0; i < sqeCtx->sgeNum; i++) {
        KERNEL_LOG(KERNEL_INFO, "Hcomm URMA SGE: idx=%u len=%u tokenId=%u va=%llu", i, sgeCtx->len, sgeCtx->tokenId,
            static_cast<unsigned long long>(sgeCtx->va));
        sgeCtx++;
    }
}

__aicore__ inline void HcommUrmaDumpWqeCtx(__ubuf__ HcommUrmaSqeCtx* sqeCtx)
{
    if (sqeCtx == nullptr) {
        KERNEL_LOG(KERNEL_INFO, "Hcomm URMA WQE: nullptr");
        return;
    }
    KERNEL_LOG(KERNEL_INFO,
        "Hcomm URMA WQE: sqeBbIdx=%u flag=%u rsv0=%u nf=%u tokenEn=%u rmtJettyType=%u owner=%u",
        sqeCtx->sqeBbIdx, sqeCtx->flag, sqeCtx->rsv0, sqeCtx->nf, sqeCtx->tokenEn, sqeCtx->rmtJettyType,
        sqeCtx->owner);
    KERNEL_LOG(KERNEL_INFO,
        "Hcomm URMA WQE: targetHint=%u opcode=%u rsv1=%u inlineMsgLen=%u tpId=%u sgeNum=%u",
        sqeCtx->targetHint, sqeCtx->opcode, sqeCtx->rsv1, sqeCtx->inlineMsgLen, sqeCtx->tpId, sqeCtx->sgeNum);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA WQE: rmtJettyOrSegId=%u rsv2=%u rmtTokenValue=%u",
        sqeCtx->rmtJettyOrSegId, sqeCtx->rsv2, sqeCtx->rmtTokenValue);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA WQE: rmtEidL=%llu rmtEidH=%llu",
        static_cast<unsigned long long>(sqeCtx->rmtEidL), static_cast<unsigned long long>(sqeCtx->rmtEidH));
    KERNEL_LOG(KERNEL_INFO,
        "Hcomm URMA WQE: udfType=%u reduceDataType=%u reduceOpcode=%u rsv3=%u rmtAddrLOrTokenId=%u "
        "rmtAddrHOrTokenValue=%u",
        sqeCtx->udfType, sqeCtx->reduceDataType, sqeCtx->reduceOpcode, sqeCtx->rsv3, sqeCtx->rmtAddrLOrTokenId,
        sqeCtx->rmtAddrHOrTokenValue);
    __ubuf__ uint8_t* sgeAddr = (__ubuf__ uint8_t*)sqeCtx + sizeof(HcommUrmaSqeCtx);
    if (sqeCtx->opcode == static_cast<uint32_t>(HcommUrmaOpCode::WRITE_WITH_NOTIFY)) {
        __ubuf__ HcommUrmaNotifyCtx* notifyCtx = (__ubuf__ HcommUrmaNotifyCtx*)sgeAddr;
        KERNEL_LOG(KERNEL_INFO,
            "Hcomm URMA WQE: notifyTokenId=%u notifyTokenValue=%u notifyAddrL=%u notifyAddrH=%u notifyDataL=%u "
            "notifyDataH=%u",
            notifyCtx->notifyTokenId, notifyCtx->notifyTokenValue, notifyCtx->notifyAddrL, notifyCtx->notifyAddrH,
            notifyCtx->notifyDataL, notifyCtx->notifyDataH);
        sgeAddr += sizeof(HcommUrmaNotifyCtx);
    }
    HcommUrmaDumpSgeCtx(sqeCtx, sgeAddr);
}

__aicore__ inline void HcommUrmaDumpCqeCtx(__ubuf__ HcommUrmaJfcCqeCtx* cqeCtx)
{
    if (cqeCtx == nullptr) {
        KERNEL_LOG(KERNEL_INFO, "Hcomm URMA CQE: nullptr");
        return;
    }
    KERNEL_LOG(KERNEL_INFO,
        "Hcomm URMA CQE: sR=%u isJetty=%u owner=%u inlineEn=%u opcode=%u fd=%u substatus=%u status=%u",
        cqeCtx->sR, cqeCtx->isJetty, cqeCtx->owner, cqeCtx->inlineEn, cqeCtx->opcode, cqeCtx->fd,
        cqeCtx->substatus, cqeCtx->status);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA CQE: entryIdx=%u localNumL=%u localNumH=%u rmtIdx=%u tpn=%u",
        cqeCtx->entryIdx, cqeCtx->localNumL, cqeCtx->localNumH, cqeCtx->rmtIdx, cqeCtx->tpn);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA CQE: byteCnt=%u userDataH=%u userDataL=%u", cqeCtx->byteCnt,
        cqeCtx->userDataH, cqeCtx->userDataL);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA CQE: rmtEid=[%u,%u,%u,%u]", cqeCtx->rmtEid[0], cqeCtx->rmtEid[1],
        cqeCtx->rmtEid[2], cqeCtx->rmtEid[3]);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA CQE: dataH=%u dataL=%u inlineData=[%u,%u,%u]", cqeCtx->dataH,
        cqeCtx->dataL, cqeCtx->inlineData[0], cqeCtx->inlineData[1], cqeCtx->inlineData[2]);
}

__aicore__ inline HcommImpl<CommProtocol::URMA, CommEngine::AIV>::HcommImpl()
{
    pipe_.InitBuffer(hcommBuf_, HCOMM_URMA_TMP_BUF_SIZE);
    wqeItem_ = hcommBuf_.Get<uint32_t>();
    cqeItem_ = wqeItem_[HCOMM_URMA_WQE_U32_NUM];
    sqPI_ = cqeItem_[HCOMM_URMA_CQE_U32_NUM];
    sqCI_ = sqPI_[8];
    cqCI_ = sqCI_[8];
    doorBell_ = cqCI_[8];
}

__aicore__ inline HcommImpl<CommProtocol::URMA, CommEngine::AIV>::~HcommImpl() {}

__aicore__ inline void HcommImpl<CommProtocol::URMA, CommEngine::AIV>::PollCqWhenSqOverflow(
    ChannelHandle channel, const SqContext& sqCtx, uint32_t sqHead)
{
    AscendC::GlobalTensor<uint32_t> sqCIGlobal;
    sqCIGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqCtx.contextInfo.ubJfs.tailAddr);
    Gm2Ub(sqCI_, sqCIGlobal, 1);
    uint32_t sqTail = sqCI_.GetValue(0);
    uint32_t baseBlockCount = HcommUrmaGetBaseBlockCount(sqCtx);
    constexpr uint32_t POLL_CQ_THRESHOLD = 10;
    constexpr uint32_t NUM_CQE_PER_POLL_CQ = 100;
    if ((sqHead + POLL_CQ_THRESHOLD) % baseBlockCount == sqTail % baseBlockCount) {
        uint32_t idx = (sqTail + NUM_CQE_PER_POLL_CQ) > sqHead ? sqHead : sqTail + NUM_CQE_PER_POLL_CQ;
        KERNEL_LOG(KERNEL_INFO, "Hcomm URMA SQ overflow sqHead=%u sqTail=%u idx=%u baseBlockCount=%u", sqHead,
            sqTail, idx, baseBlockCount);
        (void)PollCq(channel, idx);
    }
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe, HcommUrmaOpCode opCode, auto const &config>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::URMA, CommEngine::AIV>::PostSend(
    ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len, GM_ADDR notifyAddr, uint64_t notifyVal)
{
    (void)commitPipe;
    (void)reqPipe;

    __gm__ ChannelEntity* channelPtr = (__gm__ ChannelEntity*)channel;
    int32_t remoteIdx =
        HcommFindBufferIdx(channelPtr->remoteBufferAddr, channelPtr->remoteBufferNum, dst, len);
    if (remoteIdx == HCOMM_INVALID_HANDLE_ID) {
        KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA PostSend failed with invalid remote buffer");
        return HCOMM_INVALID_HANDLE_ID;
    }
    int32_t localIdx = HcommFindBufferIdx(channelPtr->localBufferAddr, channelPtr->localBufferNum, src, len);
    if (localIdx == HCOMM_INVALID_HANDLE_ID) {
        KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA PostSend failed with invalid local buffer");
        return HCOMM_INVALID_HANDLE_ID;
    }
    auto sqCtx = channelPtr->sqContextAddr[HCOMM_URMA_DEFAULT_QP_IDX];
    AscendC::GlobalTensor<uint32_t> sqPIGlobal;
    sqPIGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqCtx.contextInfo.ubJfs.headAddr);
    Gm2Ub(sqPI_, sqPIGlobal, 1);
    uint32_t sqHead = sqPI_.GetValue(0);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA PostSend resolved remoteIdx=%d localIdx=%d sqHead=%u sqDepth=%u", remoteIdx,
        localIdx, sqHead, sqCtx.contextInfo.ubJfs.sqDepth);

    // poll cq if send queue is full
    PollCqWhenSqOverflow(channel, sqCtx, sqHead);

    // write SQE
    __ubuf__ HcommUrmaSqeCtx* sqeCtx = (__ubuf__ HcommUrmaSqeCtx*)wqeItem_.GetPhyAddr();
    auto remoteMemInfo = channelPtr->remoteBufferAddr[remoteIdx];
    HcommUrmaFillSqeCtx<opCode, config>(
        sqeCtx, (__gm__ uint8_t*)dst, sqCtx, remoteMemInfo, sqHead, notifyAddr, notifyVal);

    // write SGE
    __ubuf__ uint8_t* sgeAddr = (__ubuf__ uint8_t*)sqeCtx + sizeof(HcommUrmaSqeCtx);
    if constexpr (opCode == HcommUrmaOpCode::WRITE_WITH_NOTIFY) {
        sgeAddr += sizeof(HcommUrmaNotifyCtx);
    }
    __ubuf__ HcommUrmaSgeCtx* sgeCtx = (__ubuf__ HcommUrmaSgeCtx*)sgeAddr;
    HcommUrmaFillSgeCtx(sgeCtx, len, (__gm__ uint8_t*)src);

    // SQE & SGE cache flush
    uint64_t sqBaseAddr = sqCtx.contextInfo.ubJfs.sqVa;
    uint32_t wqeSize = sqCtx.contextInfo.ubJfs.wqeSize;
    uint32_t baseBlockCount = HcommUrmaGetBaseBlockCount(sqCtx);
    __gm__ uint8_t* sqeAddr = (__gm__ uint8_t*)(sqBaseAddr + wqeSize * (sqHead % baseBlockCount));
    AscendC::GlobalTensor<uint32_t> sqeGlobal;
    sqeGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqeAddr);
    PipeBarrier<PIPE_ALL>();
    constexpr uint32_t wqeBbCnt = opCode == HcommUrmaOpCode::WRITE_WITH_NOTIFY ? 2U : 1U;
    DataCopy(sqeGlobal, wqeItem_, wqeSize * wqeBbCnt / 4);
    PipeBarrier<PIPE_ALL>();

    HcommHandle handleId = ++curHandleId_;
    channelList_[handleId] = channel;
    sqHead += wqeBbCnt;
    sqPI_.SetValue(0, sqHead);
    Ub2Gm<uint32_t>(sqPIGlobal, sqPI_, 1);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA PostSend finish handleId=%d newSqHead=%u wqeBbCnt=%u", handleId, sqHead,
        wqeBbCnt);
    if constexpr (commit) {
        Commit(handleId);
    }
    HcommUrmaDumpWqeCtx(sqeCtx);
    return handleId;
}

__aicore__ inline uint32_t HcommImpl<CommProtocol::URMA, CommEngine::AIV>::PollCq(
    ChannelHandle channel, uint32_t expectIdx)
{
    if (expectIdx == 0) {
        return HCOMM_SUCCESS;
    }
    __gm__ ChannelEntity* channelPtr = (__gm__ ChannelEntity*)channel;
    auto cqCtx = channelPtr->cqContextAddr[HCOMM_URMA_DEFAULT_QP_IDX];
    AscendC::GlobalTensor<uint32_t> cqCIGlobal;
    cqCIGlobal.SetGlobalBuffer((__gm__ uint32_t*)cqCtx.contextInfo.ubJfc.tailAddr);
    Gm2Ub(cqCI_, cqCIGlobal, 1);
    uint32_t curTail = cqCI_.GetValue(0);

    uint64_t cqBaseAddr = cqCtx.contextInfo.ubJfc.scqVa;
    uint32_t cqeSize = cqCtx.contextInfo.ubJfc.cqeSize;
    uint32_t cqDepth = cqCtx.contextInfo.ubJfc.cqDepth;
    __ubuf__ HcommUrmaJfcCqeCtx* cqeUb = (__ubuf__ HcommUrmaJfcCqeCtx*)cqeItem_.GetPhyAddr();
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA PollCq enter expectIdx=%u curTail=%u cqDepth=%u", expectIdx, curTail, cqDepth);
#if defined(UT_TEST)
    curTail = expectIdx;
#else
    while (curTail != expectIdx) {
        __gm__ uint8_t* cqeAddr = (__gm__ uint8_t*)(cqBaseAddr + cqeSize * (curTail & (cqDepth - 1)));
        AscendC::GlobalTensor<uint32_t> cqeGlobal;
        cqeGlobal.SetGlobalBuffer((__gm__ uint32_t*)cqeAddr);
        bool validOwner = (curTail / cqDepth) & 1;
        uint32_t times = 0;
        while ((validOwner ^ cqeUb->owner) == 0 && times < HCOMM_URMA_MAX_RETRY_TIMES) {
            PipeBarrier<PIPE_ALL>();
            DataCopy(cqeItem_, cqeGlobal, cqeSize / 4);
            PipeBarrier<PIPE_ALL>();
            times++;
        }
        if (times >= HCOMM_URMA_MAX_RETRY_TIMES) {
            KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Poll CQ timeout curTail=%u expectIdx=%u", curTail, expectIdx);
            HcommUrmaDumpCqeCtx(cqeUb);
            return 0xFFU;
        }
        // check CQE status
        uint8_t status = cqeUb->status & 0xFFU;
        uint8_t subStatus = cqeUb->substatus & 0xFFU;
        constexpr uint8_t statusShift = 8;
        if (status != 0 || subStatus != 0) {
            KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA CQE failed status=%u subStatus=%u", status, subStatus);
            HcommUrmaDumpCqeCtx(cqeUb);
            return (status << statusShift) | subStatus;
        }
        curTail++;
    }
#endif

    // update CQ tail
    cqCI_.SetValue(0, curTail);
    Ub2Gm<uint32_t>(cqCIGlobal, cqCI_, 1);

    // ring CQ doorbell
    AscendC::GlobalTensor<uint32_t> cqDbGlobal;
    cqDbGlobal.SetGlobalBuffer((__gm__ uint32_t*)cqCtx.contextInfo.ubJfc.dbVa);
    doorBell_.SetValue(0, curTail & 0xFFFFFFU);
    Ub2Gm<uint32_t>(cqDbGlobal, doorBell_, 1);

    // update WQ tail
    AscendC::GlobalTensor<uint32_t> sqCIGlobal;
    auto sqCtx = channelPtr->sqContextAddr[HCOMM_URMA_DEFAULT_QP_IDX];
    sqCIGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqCtx.contextInfo.ubJfs.tailAddr);
    sqCI_.SetValue(0, curTail);
    Ub2Gm<uint32_t>(sqCIGlobal, sqCI_, 1);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA PollCq finish expectIdx=%u curTail=%u", expectIdx, curTail);
    return HCOMM_SUCCESS;
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe, auto const &config>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::URMA, CommEngine::AIV>::WriteNbi(
    ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len)
{
    return PostSend<commit, commitPipe, reqPipe, HcommUrmaOpCode::WRITE, config>(channel, dst, src, len);
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe, auto const &config>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::URMA, CommEngine::AIV>::ReadNbi(
    ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len)
{
    return PostSend<commit, commitPipe, reqPipe, HcommUrmaOpCode::READ, config>(channel, src, dst, len);
}

template <bool commit, pipe_t commitPipe, pipe_t reqPipe, auto const &config>
__aicore__ inline HcommHandle HcommImpl<CommProtocol::URMA, CommEngine::AIV>::WriteWithNotifyNbi(
    ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len, GM_ADDR notifyAddr, uint64_t notifyVal)
{
    return PostSend<commit, commitPipe, reqPipe, HcommUrmaOpCode::WRITE_WITH_NOTIFY, config>(
        channel, dst, src, len, notifyAddr, notifyVal);
}

template <bool isWait>
__aicore__ inline bool HcommImpl<CommProtocol::URMA, CommEngine::AIV>::JudgeHandleId(HcommHandle handleId)
{
    // 判断handleId是否合法
    if (handleId < 0 || handleId >= static_cast<HcommHandle>(HCOMM_MAX_HANDLE_ID)) {
        KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Judge HandleId failed with invalid handleId");
        return false;
    }
    if (channelList_[handleId] == 0) {
        KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Judge HandleId failed without valid channel");
        return false;
    }
    if constexpr (isWait) {
        // Wait操作，拒绝对未进行Commit或者已进行Wait的handleId进行操作
        if (!handleCommitList_[handleId]) {
            KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Wait failed without Commit");
            return false;
        }
        if (handleWaitList_[handleId]) {
            KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Wait failed with already Wait");
            return false;
        }
    } else {
        // Commit操作，拒绝对已进行Commit的handleId进行操作
        if (handleCommitList_[handleId]) {
            KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Commit failed with already Commit");
            return false;
        }
    }
    return true;
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<CommProtocol::URMA, CommEngine::AIV>::Commit(HcommHandle handleId)
{
    (void)pipe;
    if (!JudgeHandleId<false>(handleId)) {
        return HCOMM_FAILED;
    }
    ChannelHandle channel = channelList_[handleId];
    __gm__ ChannelEntity* channelPtr = (__gm__ ChannelEntity*)channel;
    auto sqCtx = channelPtr->sqContextAddr[HCOMM_URMA_DEFAULT_QP_IDX];
    AscendC::GlobalTensor<uint32_t> sqPIGlobal;
    sqPIGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqCtx.contextInfo.ubJfs.headAddr);
    Gm2Ub(sqPI_, sqPIGlobal, 1);
    uint32_t sqHead = sqPI_.GetValue(0);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA Commit enter handleId=%d sqHead=%u", handleId, sqHead);

    AscendC::GlobalTensor<uint32_t> dbGlobal;
    dbGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqCtx.contextInfo.ubJfs.dbVa);
    doorBell_.SetValue(0, sqHead);
    // TODO: set_atomic_add();
    Ub2Gm<uint32_t>(dbGlobal, doorBell_, 1);
    // TODO: SetAtomicNone();

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
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA Commit finish handleId=%d sqHead=%u", handleId, sqHead);
    return HCOMM_SUCCESS;
}

template <pipe_t pipe>
__aicore__ inline int32_t HcommImpl<CommProtocol::URMA, CommEngine::AIV>::Wait(HcommHandle handleId)
{
    (void)pipe;
    if (!JudgeHandleId<true>(handleId)) {
        return HCOMM_FAILED;
    }
    ChannelHandle channel = channelList_[handleId];
    __gm__ ChannelEntity* channelPtr = (__gm__ ChannelEntity*)channel;
    auto sqCtx = channelPtr->sqContextAddr[HCOMM_URMA_DEFAULT_QP_IDX];
    AscendC::GlobalTensor<uint32_t> sqPIGlobal;
    sqPIGlobal.SetGlobalBuffer((__gm__ uint32_t*)sqCtx.contextInfo.ubJfs.headAddr);
    Gm2Ub(sqPI_, sqPIGlobal, 1);
    uint32_t sqHead = sqPI_.GetValue(0);
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA Wait enter handleId=%d sqHead=%u", handleId, sqHead);
    uint32_t ret = PollCq(channel, sqHead);
    if (ret != HCOMM_SUCCESS) {
        KERNEL_LOG(KERNEL_ERROR, "Hcomm URMA Wait failed handleId=%d pollRet=%u", handleId, ret);
        return HCOMM_FAILED;
    }
    for (HcommHandle i = handleId; i >= 0; i--) {
        if (channelList_[i] == channel) {
            if (!handleWaitList_[i]) {
                handleWaitList_[i] = true;
            } else {
                continue;
            }
        }
    }
    KERNEL_LOG(KERNEL_INFO, "Hcomm URMA Wait finish handleId=%d sqHead=%u", handleId, sqHead);
    return HCOMM_SUCCESS;
}

} // namespace AscendC

#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCOMM_AIV_URMA_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCOMM_AIV_URMA_H__
#endif
