/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
* \file hccl_ccu_common.h
* \brief CCU common implementation for v0 and v1
*/
#ifndef IMPL_V310_HCCL_CCU_BASE_H
#define IMPL_V310_HCCL_CCU_BASE_H

#include "../../common/hccl_utils.h"

namespace AscendC {

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AllReduce(GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t count,
                    HcclDataType dataType, HcclReduceOp op, uint8_t repeat)
{
    ASCENDC_HCCL_API_ASSERT(op >= HCCL_REDUCE_SUM && op < HCCL_REDUCE_RESERVED, { return INVALID_HANDLE_ID; },
                            "Call AllReduce failed, param HcclReduceOp is %d, invalid.", static_cast<int32_t>(op));
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLREDUCE, sendBuf, recvBuf, count, dataType, dataType,
                                    op, 0, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AllGather(GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t sendCount,
                HcclDataType dataType, uint64_t strideCount, uint8_t repeat)
{
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLGATHER, sendBuf, recvBuf, sendCount, dataType, dataType,
                                    HCCL_REDUCE_RESERVED, strideCount, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::ReduceScatter(GM_ADDR sendBuf, GM_ADDR recvBuf,
                    uint64_t recvCount, HcclDataType dataType, HcclReduceOp op, uint64_t strideCount, uint8_t repeat)
{
    ASCENDC_HCCL_API_ASSERT(op >= HCCL_REDUCE_SUM && op < HCCL_REDUCE_RESERVED, { return INVALID_HANDLE_ID; },
                            "Call ReduceScatter failed, param HcclReduceOp is %d, invalid.", static_cast<int32_t>(op));
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_REDUCE_SCATTER, sendBuf, recvBuf, recvCount,
                                    dataType, dataType, op, strideCount, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AlltoAll(GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t dataCount,
                                    HcclDataType dataType, uint64_t strideCount, uint8_t repeat)
{
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLTOALL, sendBuf, recvBuf, dataCount, dataType, dataType,
                                    HCCL_REDUCE_RESERVED, strideCount, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AlltoAllV(GM_ADDR sendBuf, void *sendCounts, void *sdispls,
                                        HcclDataType sendType, GM_ADDR recvBuf, void *recvCounts, void *rdispls,
                                        HcclDataType recvType, uint8_t repeat)
{
    ASCENDC_HCCL_API_ASSERT(sendType == recvType, { return INVALID_HANDLE_ID; },
                            "Call AlltoAllV failed, param sendType[%d] is not equal to recvType[%d], invalid.",
                            static_cast<int32_t>(sendType), static_cast<int32_t>(recvType));
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLTOALLV, sendBuf, recvBuf, 0U, sendType, recvType,
                                    HCCL_REDUCE_RESERVED, 0U, repeat,
                                    {static_cast<uint64_t *>(sendCounts), static_cast<uint64_t *>(sdispls),
                                        static_cast<uint64_t *>(recvCounts), static_cast<uint64_t *>(rdispls)} });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AlltoAllvWrite(GM_ADDR usrIn, GM_ADDR sendOffsets,
    GM_ADDR sendSizes, uint64_t remoteWinOffset, uint64_t localDataSize)
{
    CommonPrepareParam commonPrepareParam = {HcclCMDType::HCCL_CMD_HALF_ALLTOALLV,
        usrIn, usrIn, localDataSize, HCCL_DATA_TYPE_INT8, HCCL_DATA_TYPE_INT8,
        HCCL_REDUCE_RESERVED, 0, 1, {},
        {reinterpret_cast<uint64_t>(sendOffsets), reinterpret_cast<uint64_t>(sendSizes), remoteWinOffset}
    };
    return CommonPrepareImpl<commit>(commonPrepareParam);
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::FlushDataCacheForCopy(GM_ADDR gmAddr, uint32_t size)
{
    uint32_t offset = 0;
    for (uint32_t i = 0; i < size; i += MAX_DCCI_CNT) {
        FlushDataCache((GM_ADDR)(gmAddr + i));
        offset += MAX_DCCI_CNT;
    }
    FlushDataCache((GM_ADDR)(gmAddr + offset));
}

template<const auto &config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitWorkingFlag()
{
    using T = decltype(config);
    static_assert(std::is_same<T, const HcclServerConfig &>::value);
    KERNEL_LOG(KERNEL_INFO, "Working core type %u id %u.", static_cast<uint8_t>(config.type), config.blockId);
    if constexpr (config.type == CoreType::ON_AIV) {
        workingFlag_ = (g_coreType == AscendC::AIV && GetBlockIdx() == config.blockId);
    } else if constexpr (config.type == CoreType::ON_AIC) {
        workingFlag_ = (g_coreType == AscendC::AIC && GetBlockIdx() == config.blockId);
    } else {
        workingFlag_ = (GetBlockIdx() == config.blockId);
    }
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitInnerBase(GM_ADDR context, HcclTilingVersion version)
{
    ASCENDC_HCCL_API_ASSERT(context != nullptr, { return; }, "Init Hccl failed, context addr is nullptr.");
    hcclContext_ = (__gm__ HcclCombineOpParam *)context;
    uint64_t msgAddr = hcclContext_->workSpace;
    if (msgAddr & 0x1ff) {
        msgAddr = (msgAddr & (~((uint64_t)0x1ff))) + 0x200;
    }
    KERNEL_LOG(KERNEL_INFO, "ApiClient InitInner msgAddr:0x%llx, workSpaceSize:0x%llx", msgAddr, hcclContext_->workSpaceSize);

#ifndef __CCE_KT_TEST__
    ASCENDC_ASSERT((reinterpret_cast<uintptr_t>(hcclContext_->xnOffset) % ALIGN_64_BYTE == 0),
        { KERNEL_LOG(KERNEL_ERROR, "xnAddr is not 64-byte aligned!"); });
#endif

    finishCntGM_ = reinterpret_cast<__gm__ uint64_t*>(msgAddr);
    uint64_t finishCntGMOffset =  msgAddr + MAX_DCCI_CNT;
    handleParamGM_ = reinterpret_cast<__gm__ CommonPrepareParamCcu*>(finishCntGMOffset);
    uint64_t handleParamGMOffset = finishCntGMOffset + (uint64_t)sizeof(CommonPrepareParamCcu) * HCCL_MAX_HANDLE_ID;
    allToAllVParam_ = reinterpret_cast<__gm__ AlltoAllVParamCcu*>(handleParamGMOffset);
    uint64_t allToAllVParamGMOffset = handleParamGMOffset + (uint64_t)sizeof(AlltoAllVParamCcu) * HCCL_MAX_HANDLE_ID;
    ccuParam_.ccuMsgExt = reinterpret_cast<__gm__ CCUMsgExt*>(allToAllVParamGMOffset);

    InitWorkingFlag();
    if (workingFlag_) {
        WriteHBMData(finishCntGM_, uint64_t(0));
    }
    curVersion_ = version;
    curHandleId_ = 0;
    finishNum_ = 0;
    finishNumTemp_ = 0;
    globalCurWaitId_ = 0;
    globalCurResId_ = 0;

    for (uint8_t i = 0; i < CCU_MAX_MSG_NUM; ++i) {
        msgQueueIsAvailable_[i] = true;
    }
    KERNEL_LOG(KERNEL_INFO, "ApiClient InitInner rankId:%d, rankNum:%d, xnAddr:0x%llx, ckeAddr:0x%llx, ccuMsgExt:0x%llx",
                    hcclContext_->rankId, hcclContext_->rankNum, hcclContext_->xnOffset, hcclContext_->ckeOffset,
                    reinterpret_cast<uint64_t>(ccuParam_.ccuMsgExt));
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::Init(GM_ADDR context, __gm__ void *initTiling)
{
    HcclTilingVersion version =
        (initTiling != nullptr ? HcclTilingVersion::NEW_TILING_VERSION : HcclTilingVersion::DEPRECATED_TILING_VERSION);
    InitInner(context, version);
}

template <const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitV2(GM_ADDR context, const void *initTiling)
{
    HcclTilingVersion version = (initTiling != nullptr ? HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION
                                                    : HcclTilingVersion::DEPRECATED_TILING_VERSION);
    InitInner(context, version);
    tilingBaseAddr_ = reinterpret_cast<uint64_t>(initTiling);
}

template<const auto &config>
__aicore__ inline int32_t
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::SetCcTiling(__gm__ void *ccOpTilingData)
{
    ASCENDC_HCCL_API_ASSERT(curVersion_ == HcclTilingVersion::NEW_TILING_VERSION, { return HCCL_FAILED; },
                            "Call SetCcTiling failed, ensure Hccl::InitV1 func has been called successfully!");
    ASCENDC_HCCL_API_ASSERT(ccOpTilingData != nullptr, { return HCCL_FAILED; },
                            "Call SetCcTiling failed, ensure ccOpTilingData is not nullptr");
    const uint32_t opType = (static_cast<__gm__ Mc2CcTilingInner *>(ccOpTilingData))->opType;
    ASCENDC_HCCL_API_ASSERT(opType >= 0 && opType < static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALL),
                            { return HCCL_FAILED; }, "Call SetCcTiling failed, ensure cmdType is valid");
    KERNEL_LOG(KERNEL_INFO, "CmdType = %d, ccOpTilingData = %lu ", opType, reinterpret_cast<uint64_t>(ccOpTilingData));
    ccOpTilingDataTable_[opType] = reinterpret_cast<uint64_t>(ccOpTilingData);
    return HCCL_SUCCESS;
}

template <const auto &config>
__aicore__ inline int32_t HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::SetCcTilingV2(uint64_t offset)
{
    ASCENDC_HCCL_API_ASSERT(curVersion_ == HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION,
                            { return HCCL_FAILED; },
                            "Call SetCcTiling failed, ensure Hccl::InitV2 func has been called successfully!");
    const uint32_t opType = (reinterpret_cast<Mc2CcTilingInner *>(tilingBaseAddr_ + offset))->opType;
    ASCENDC_HCCL_API_ASSERT(opType >= 0 && opType < static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALL),
                            { return HCCL_FAILED; }, "Call SetCcTiling failed, ensure cmdType is valid");
    ccOpTilingDataTable_[opType] = offset;
    return HCCL_SUCCESS;
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForOp(const HcclHandle handleId)
{
    ccuUsedXnNum_ = 8;
    sizeOfXnMsg_ = CCU_XN_DATA_SIZE * ccuUsedXnNum_;
    FlushDataCache(&handleParamGM_[handleId]);
    if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_ALLGATHER) {
        CcuPrepareForAllGather(&handleParamGM_[handleId]);
    } else if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_ALLREDUCE) {
        CcuPrepareForAllReduce(&handleParamGM_[handleId]);
    } else if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_ALLTOALL) {
        CcuPrepareForAllToAll(&handleParamGM_[handleId]);
    } else if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
        FlushDataCache(&allToAllVParam_[handleId]);
        CcuPrepareForAllToAllV(&handleParamGM_[handleId], &allToAllVParam_[handleId]);
    } else if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_HALF_ALLTOALLV) {
        ccuUsedXnNum_ = 9;
        sizeOfXnMsg_ += CCU_XN_DATA_SIZE;
        FlushDataCache(reinterpret_cast<__gm__ uint8_t*>(&handleParamGM_[handleId]) + MAX_DCCI_CNT);
        CcuPrepareForAllToAllVWrite(&handleParamGM_[handleId]);
    } else if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER) {
        CcuPrepareForReduceScatter(&handleParamGM_[handleId]);
    }
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CommitMsgInner(const HcclHandle handleId)
{
    handleNeedCommitCnt_[handleId] += handleRepeatCnt_[handleId];
    for (uint32_t i = 0U; i < handleRepeatCnt_[handleId]; ++i) {
        if (msgQueueIsAvailable_[globalCurResId_] == false) {
            break;
        }
        KERNEL_LOG(KERNEL_INFO, "ApiClient do ccu prepare repeatIdx = %d, globalCurResId_ = %d.", i, globalCurResId_);
        ccuParam_.repeatIndex = i;
        if (workingFlag_) {
            CcuPrepareForOp(handleId);
            CcuSendMsg(globalCurResId_);
        }
        msgQueueIsAvailable_[globalCurResId_] = false;
        handleCommitCnt_[handleId]++;
        globalCurCommitCnt_++;
        globalCurResId_++;
        globalCurResId_ %= CCU_MAX_MSG_NUM;
    }
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitCcuParam(const HcclHandle handleId)
{
    if (needResetDataType_) {
        handleParamGM_[handleId].dataType = ccuDataType_.srcDataType;
        handleParamGM_[handleId].dstDataType = ccuDataType_.dstDataType;
        handleParamGM_[handleId].op = ccuDataType_.op;
    } else  {
        uint64_t ccTiling = 0;
        if (curVersion_ == HcclTilingVersion::NEW_TILING_VERSION) {
            ccTiling = ccOpTilingDataTable_[static_cast<uint32_t>(handleParamGM_[handleId].commType.prepareType)];
        } else if (curVersion_ == HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION) {
            ccTiling = ccOpTilingDataTable_[static_cast<uint32_t>(handleParamGM_[handleId].commType.prepareType)] +
                    tilingBaseAddr_;
        }
        if (ccTiling != 0) {
            __gm__ Mc2CcTilingInner *tilingPtr = reinterpret_cast<__gm__ Mc2CcTilingInner *>(ccTiling);
            handleParamGM_[handleId].dataType = static_cast<HcclDataType>(tilingPtr->srcDataType);
            handleParamGM_[handleId].dstDataType = static_cast<HcclDataType>(tilingPtr->dstDataType);
            handleParamGM_[handleId].op = static_cast<HcclReduceOp>(tilingPtr->reduceType);
        }
    }
    ccuParam_.rankNum = hcclContext_->rankNum;
    ccuParam_.rankId = hcclContext_->rankId;
}

template<const auto &config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitHandleInfo(uint8_t handleId)
{
    handleReqId_[handleId] = 0;
    handleRepeatCnt_[handleId] = 0;
    handleNeedCommitCnt_[handleId] = 0;
    handleCommitCnt_[handleId] = 0;
    handleWaitCnt_[handleId] = 0;
    handleFinishCnt_[handleId] = 0;
}

template<const auto &config>
template<bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CommonPrepareImpl(const CommonPrepareParam &commonPrepareParam)
{
    if (unlikely(commonPrepareParam.repeat == 0U)) {
        return INVALID_HANDLE_ID;
    }
    HcclHandle handleId = curHandleId_;
    ASCENDC_HCCL_API_ASSERT(handleId < HCCL_MAX_HANDLE_ID, {return INVALID_HANDLE_ID;},
        "Call Prepare[%d] failed, Prepare interface call num is[%d], expected less than[%d].",
        static_cast<int32_t>(commonPrepareParam.commType.prepareType), handleId + 1, HCCL_MAX_HANDLE_ID);

    InitHandleInfo(handleId);
    uint64_t reqId = handleId == 0 ? 0 : handleReqId_[handleId - 1] + handleRepeatCnt_[handleId - 1];
    handleReqId_[handleId] = (uint8_t)(reqId % CCU_MAX_MSG_NUM);
    handleRepeatCnt_[handleId] = commonPrepareParam.repeat;

    if (workingFlag_) {
        handleParamGM_[handleId].commType.prepareType = commonPrepareParam.commType.prepareType;
        handleParamGM_[handleId].sendBuf = commonPrepareParam.sendBuf;
        handleParamGM_[handleId].recvBuf = commonPrepareParam.recvBuf;
        handleParamGM_[handleId].count = commonPrepareParam.count;
        handleParamGM_[handleId].dataType = commonPrepareParam.dataType;
        handleParamGM_[handleId].dstDataType = commonPrepareParam.dstDataType;
        handleParamGM_[handleId].op = commonPrepareParam.op;
        handleParamGM_[handleId].strideCount = commonPrepareParam.strideCount;
        InitCcuParam(handleId);
        FlushDataCache(&handleParamGM_[handleId]);

        if (commonPrepareParam.commType.prepareType == HcclCMDType::HCCL_CMD_ALLTOALLV)  {
            for (uint32_t i = 0; i < hcclContext_->rankNum; i++) {
                allToAllVParam_[handleId].sendCounts[i] = commonPrepareParam.paramExt.sendCounts[i];
                allToAllVParam_[handleId].sdispls[i] = commonPrepareParam.paramExt.sdispls[i];
                allToAllVParam_[handleId].recvCounts[i] = commonPrepareParam.paramExt.recvCounts[i];
                allToAllVParam_[handleId].rdispls[i] = commonPrepareParam.paramExt.rdispls[i];
            }
            for (uint32_t j = 0; j < hcclContext_->rankNum; j += MAX_DCCI_CNT / sizeof(uint64_t)) {
                FlushDataCache(&allToAllVParam_[handleId].sendCounts + j);
                FlushDataCache(&allToAllVParam_[handleId].sdispls + j);
                FlushDataCache(&allToAllVParam_[handleId].recvCounts + j);
                FlushDataCache(&allToAllVParam_[handleId].rdispls + j);
            }
        } else if (commonPrepareParam.commType.prepareType == HcclCMDType::HCCL_CMD_HALF_ALLTOALLV) {
            handleParamGM_[handleId].wParamExt.sendOffsets = commonPrepareParam.wParamExt.sendOffsets;
            handleParamGM_[handleId].wParamExt.sendSizes = commonPrepareParam.wParamExt.sendSizes;
            handleParamGM_[handleId].wParamExt.remoteWinOffset = commonPrepareParam.wParamExt.remoteWinOffset;
            FlushDataCache(reinterpret_cast<__gm__ uint8_t*>(&handleParamGM_[handleId]) + MAX_DCCI_CNT);
        }
    }

    if constexpr (commit) {
        CommitMsgInner(handleId);
    }
    curHandleId_++;
    return handleId;
}

template<const auto &config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuSendMsg(uint8_t resourceId)
{
    ASCENDC_HCCL_API_ASSERT(resourceId >= 0 && resourceId < CCU_MAX_MSG_NUM, {return;},
            "ApiClient CcuSendMsg resourceId %d is invalid.", resourceId);
    ccuMsg_.xnAddr = hcclContext_->xnOffset + static_cast<uint64_t>(resourceId) * CCU_MSG_XN_NUM * CCU_XN_DATA_SIZE;
    ccuMsg_.commitCKEAddr = GetCommitCkeAddr(resourceId);

    for (int i = 0; i < ccuUsedXnNum_; i++) {
        *(reinterpret_cast<__gm__ uint64_t*>(ccuMsg_.xnAddr + CCU_XN_DATA_SIZE * i)) = xnData_[i];
    }
    FlushDataCache(ccuMsg_.xnAddr);
    if (ccuUsedXnNum_ > 8) {
        FlushDataCache(ccuMsg_.xnAddr + MAX_DCCI_CNT);
    }
    WriteHBMData(ccuMsg_.commitCKEAddr, CCU_MSG_CKE_SET_VALUE);
}

template<const auto &config>
__aicore__ inline GM_ADDR HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::GetCommitCkeAddr(uint8_t msgId)
{
    return hcclContext_->ckeOffset + static_cast<uint64_t>(msgId) * CCU_CKE_SIZE;
}

template<const auto &config>
__aicore__ inline GM_ADDR HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::GetWaitCkeAddr(uint8_t msgId)
{
    uint64_t offset = static_cast<uint64_t>(msgId) * CCU_CKE_SIZE + static_cast<uint64_t>(CCU_CKE_SIZE) * CCU_MAX_MSG_NUM;
    return hcclContext_->ckeOffset + offset;
}

template<const auto &config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CommitMsg(HcclHandle handleId)
{
    ccuParam_.repeatIndex = handleCommitCnt_[handleId];
    if (workingFlag_) {
        CcuPrepareForOp(handleId);
        CcuSendMsg(globalCurResId_);
    }
    msgQueueIsAvailable_[globalCurResId_] = false;
    handleCommitCnt_[handleId]++;
    globalCurCommitCnt_++;
    globalCurResId_++;
    globalCurResId_ %= CCU_MAX_MSG_NUM;
}

template<const auto &config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::Commit(HcclHandle handleId)
{
    ASCENDC_HCCL_API_ASSERT(isInited_, { return; },
    "Call Commit failed, please ensure Hccl::Init func has been called successfully already!");
    ASCENDC_HCCL_API_ASSERT(handleId > INVALID_HANDLE_ID && handleId < HCCL_MAX_HANDLE_ID, { return; },
        "Call Commit failed, handleId is[%d], expected in range of [0, %d).", handleId, HCCL_MAX_HANDLE_ID);
    handleNeedCommitCnt_[handleId]++;
    if (msgQueueIsAvailable_[globalCurResId_] == false) {
        KERNEL_LOG(KERNEL_ERROR, "Call Commit failed, the th msg is not avaliable! handleId = %d", globalCurResId_, handleId);
        return;
    }
    CommitMsg(handleId);
}

template<const auto &config>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::WaitDone(HcclHandle handleId)
{
    msgQueueIsAvailable_[handleReqId_[handleId] + handleFinishCnt_[handleId]] = true;
    handleFinishCnt_[handleId]++;
    finishNum_++;
    globalCurWaitId_++;
    globalCurWaitId_ %= CCU_MAX_MSG_NUM;

    if (handleCommitCnt_[handleId] < handleNeedCommitCnt_[handleId]) {
        CommitMsg(handleId);
    } else {
        uint32_t nextHandleId = handleId + 1;
        if (nextHandleId < curHandleId_ && (handleCommitCnt_[nextHandleId] < handleNeedCommitCnt_[nextHandleId])) {
            CommitMsg(nextHandleId);
        }
    }
}

template<const auto &config>
template <bool sync>
__aicore__ inline void HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::Finalize()
{
    ASCENDC_HCCL_API_ASSERT(isInited_, { return ; },
            "Call Finalize failed, please ensure Hccl::Init func has been called successfully already!");

    if (workingFlag_) {
        HcclHandle handleId = curHandleId_ - 1;
        while (handleFinishCnt_[handleId] < handleCommitCnt_[handleId]) {
            Wait(handleId);
        }
        ccuMsg_.commitCKEAddr = GetCommitCkeAddr(globalCurWaitId_);
        ccuMsg_.xnAddr = hcclContext_->xnOffset + CCU_MSG_XN_NUM * CCU_XN_DATA_SIZE * globalCurWaitId_;
        *reinterpret_cast<__gm__ uint64_t*>(ccuMsg_.xnAddr) = 0xffffffffffffffff;
        FlushDataCache(ccuMsg_.xnAddr);
        WriteHBMData(ccuMsg_.commitCKEAddr, CCU_MSG_CKE_SET_VALUE);
    }
}

template<const auto &config>
__aicore__ inline bool
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::SetReduceDataTypeAbility(HcclReduceOp op,
    HcclDataType dstDataType, HcclDataType srcDataType)
{
    ASCENDC_HCCL_API_ASSERT(op != HcclReduceOp::HCCL_REDUCE_RESERVED, { return false; },
        "Set Reduce DataType Ability Failed, HcclReduceOp is invalid, reduceOpType = %u.",
        static_cast<uint32_t>(op));
    ASCENDC_HCCL_API_ASSERT(dstDataType != HcclDataType::HCCL_DATA_TYPE_RESERVED, { return false; },
        "Set Reduce DataType Ability Failed, Hccl OutputDataType is invalid, DataType = %u.",
        static_cast<uint32_t>(dstDataType));
    ASCENDC_HCCL_API_ASSERT(srcDataType != HcclDataType::HCCL_DATA_TYPE_RESERVED, { return false; },
        "Set Reduce DataType Ability Failed, Hccl InputDataType is invalid, DataType = %u.",
        static_cast<uint32_t>(srcDataType));
    ccuDataType_.op = op;
    ccuDataType_.dstDataType = dstDataType;
    ccuDataType_.srcDataType = srcDataType;
    needResetDataType_ = true;
    return true;
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllGather(__gm__ CommonPrepareParamCcu *commParam)
{
    xnData_[0] = GetOpId(commParam);
    auto dataSize = DATA_TYPE_MAP[static_cast<uint64_t>(commParam->dataType)];
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset;
    xnData_[2] = (uint64_t)commParam->recvBuf + offset;

    uint64_t sliceCount = commParam->count;
    uint64_t sliceSize = sliceCount * dataSize;
    uint64_t loopSize = CCU_LOOP_COUNT * CCU_MEMSLICE_SIZE;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / CCU_MEMSLICE_SIZE;
    uint64_t p = sliceSize - m * loopSize - n * CCU_MEMSLICE_SIZE;

    xnData_[3] = (commParam->strideCount == 0) ? sliceSize * ccuParam_.rankId :
                (commParam->strideCount * dataSize * ccuParam_.rankId);
    xnData_[4] = loopSize * m;
    xnData_[5] = m;
    CalcLoopGroupParam(xnData_, m, n, p);
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAll(__gm__ CommonPrepareParamCcu *commParam)
{
    uint64_t dataSize = DATA_TYPE_MAP[commParam->dataType];
    xnData_[0] = GetOpId(commParam);
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * dataSize;
    xnData_[1] = (uint64_t)commParam->sendBuf + offset;
    xnData_[2] = (uint64_t)commParam->recvBuf + offset;
    uint64_t sliceSizeAlltoall = commParam->count * dataSize;
    uint64_t strideSize = commParam->strideCount == 0 ? sliceSizeAlltoall : commParam->strideCount * dataSize;
    xnData_[3] = sliceSizeAlltoall;
    xnData_[4] = strideSize;
    xnData_[5] = 0;
    xnData_[6] = strideSize * ccuParam_.rankId;
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllV(
    __gm__ CommonPrepareParamCcu *commParam, __gm__ AlltoAllVParamCcu *allToAllVParam)
{
    xnData_[0] = GetOpId(commParam);
    uint64_t offset = commParam->count * ccuParam_.repeatIndex * DATA_TYPE_MAP[commParam->dataType];
    xnData_[1] = (uint64_t)commParam->sendBuf + offset;
    xnData_[2] = (uint64_t)commParam->recvBuf + offset;
    xnData_[3] = 0;
    xnData_[4] = 0;
    AssembleHcclMsgExtForCCU(ccuParam_, commParam, allToAllVParam);
    xnData_[5] = reinterpret_cast<uint64_t>(ccuParam_.ccuMsgExt) + CCU_MSG_EXT_RANK_OFFSET * ccuParam_.alltoallvCnt++;
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForAllToAllVWrite(__gm__ CommonPrepareParamCcu *commParam)
{
    xnData_[0] = GetOpId(commParam);
    xnData_[1] = reinterpret_cast<uint64_t>(commParam->sendBuf);
    xnData_[2] = commParam->wParamExt.sendSizes;
    xnData_[8] = commParam->wParamExt.remoteWinOffset;
    uint64_t sliceSize = commParam->count;
    uint64_t loopSize = CCU_LOOP_COUNT_ATAVW * CCU_MEMSLICE_SIZE;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / CCU_MEMSLICE_SIZE;
    uint64_t p = sliceSize - m * loopSize - n * CCU_MEMSLICE_SIZE;
    xnData_[3] = commParam->wParamExt.sendOffsets;
    xnData_[4] = loopSize * m;
    xnData_[5] = m;
    CalcLoopGroupParam(xnData_, m, n, p);
}

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::CcuPrepareForReduceScatter(__gm__ CommonPrepareParamCcu *commParam)
{
    CcuPrepareForAllGather(commParam);
}

}
#endif