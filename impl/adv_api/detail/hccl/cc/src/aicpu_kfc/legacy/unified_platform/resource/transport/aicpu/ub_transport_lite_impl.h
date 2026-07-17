/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UB_MEM_TRANSPORT_LITE_H
#define UB_MEM_TRANSPORT_LITE_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "base_transport_lite_impl.h"
#include "notify_lite.h"
#include "task_param.h"
#include "rmt_rma_buf_slice_lite.h"
#include "rma_conn_lite.h"
#include "kernel_param_lite.h"
#include "hcomm_primitives.h"

namespace Hccl {

static const std::unordered_map<HcommReduceOp, ReduceOp> mapHcommReduceOpA5 = {
    {HcommReduceOp::HCOMM_REDUCE_SUM,  ReduceOp::SUM},
    {HcommReduceOp::HCOMM_REDUCE_PROD, ReduceOp::PROD},
    {HcommReduceOp::HCOMM_REDUCE_MAX,  ReduceOp::MAX},
    {HcommReduceOp::HCOMM_REDUCE_MIN,  ReduceOp::MIN},
    {HcommReduceOp::HCOMM_REDUCE_RESERVED, ReduceOp::INVALID}
};

static const std::unordered_map<HcommDataType, DataType> mapHcommDataTypeA5 = {
    {HcommDataType::HCOMM_DATA_TYPE_HIF8,     DataType::HIF8},
    {HcommDataType::HCOMM_DATA_TYPE_FP8E4M3,  DataType::FP8E4M3},
    {HcommDataType::HCOMM_DATA_TYPE_FP8E5M2,  DataType::FP8E5M2},
    {HcommDataType::HCOMM_DATA_TYPE_FP8E8M0,  DataType::FP8E8M0},
    {HcommDataType::HCOMM_DATA_TYPE_INT8,     DataType::INT8},
    {HcommDataType::HCOMM_DATA_TYPE_INT16,    DataType::INT16},
    {HcommDataType::HCOMM_DATA_TYPE_INT32,    DataType::INT32},
    {HcommDataType::HCOMM_DATA_TYPE_INT64,    DataType::INT64},
    {HcommDataType::HCOMM_DATA_TYPE_INT128,   DataType::INT128},
    {HcommDataType::HCOMM_DATA_TYPE_UINT8,    DataType::UINT8},
    {HcommDataType::HCOMM_DATA_TYPE_UINT16,   DataType::UINT16},
    {HcommDataType::HCOMM_DATA_TYPE_UINT32,   DataType::UINT32},
    {HcommDataType::HCOMM_DATA_TYPE_UINT64,   DataType::UINT64},
    {HcommDataType::HCOMM_DATA_TYPE_FP16,     DataType::FP16},
    {HcommDataType::HCOMM_DATA_TYPE_FP32,     DataType::FP32},
    {HcommDataType::HCOMM_DATA_TYPE_FP64,     DataType::FP64},
    {HcommDataType::HCOMM_DATA_TYPE_BFP16,    DataType::BFP16},
    {HcommDataType::HCOMM_DATA_TYPE_RESERVED, DataType::INVALID}
};

static inline u32 GetHcommDataTypeSize(HcommDataType dataType)
{
    switch (dataType) {
        case HCOMM_DATA_TYPE_INT8: return sizeof(s8);
        case HCOMM_DATA_TYPE_INT16: return sizeof(s16);
        case HCOMM_DATA_TYPE_INT32: return sizeof(s32);
        case HCOMM_DATA_TYPE_FP16: return 2;
        case HCOMM_DATA_TYPE_FP32: return sizeof(float);
        case HCOMM_DATA_TYPE_INT64: return sizeof(s64);
        case HCOMM_DATA_TYPE_UINT64: return sizeof(u64);
        case HCOMM_DATA_TYPE_UINT8: return sizeof(u8);
        case HCOMM_DATA_TYPE_UINT16: return sizeof(u16);
        case HCOMM_DATA_TYPE_UINT32: return sizeof(u32);
        case HCOMM_DATA_TYPE_FP64: return 8;
        case HCOMM_DATA_TYPE_BFP16: return 2;
        case HCOMM_DATA_TYPE_INT128: return 16;
        case HCOMM_DATA_TYPE_HIF8: return 1;
        case HCOMM_DATA_TYPE_FP8E4M3: return 1;
        case HCOMM_DATA_TYPE_FP8E5M2: return 1;
        case HCOMM_DATA_TYPE_FP8E8M0: return 1;
        default: return 0;
    }
}

static inline HcclResult CheckReduceHcommDataTypeAndHcommReduceOp(HcommDataType dataType, HcommReduceOp reduceOp)
{
    auto dataTypeIt = mapHcommDataTypeA5.find(dataType);
    if (dataTypeIt == mapHcommDataTypeA5.end() || dataTypeIt->first == HcommDataType::HCOMM_DATA_TYPE_RESERVED) {
        HCCL_ERROR("[%s] type[%u] is not supported.", __func__, dataType);
        return HCCL_E_PARA;
    }

    auto reduceOpIt = mapHcommReduceOpA5.find(reduceOp);
    if (reduceOpIt == mapHcommReduceOpA5.end() || reduceOpIt->first == HcommReduceOp::HCOMM_REDUCE_RESERVED) {
        HCCL_ERROR("[%s] op[%u] is not supported.", __func__, reduceOp);
        return HCCL_E_PARA;
    }

    return HCCL_SUCCESS;
}

static inline HcclResult ParasReduceData(const HcommBatchTransferDesc &transferDesc, uint64_t &len,
                                    HcommDataType &dataType, HcommReduceOp &reduceOp)
{
    len = transferDesc.transferInfo.reduce.count;
    dataType = transferDesc.transferInfo.reduce.dataType;
    reduceOp = transferDesc.transferInfo.reduce.reduceOp;
    auto ret = CheckReduceHcommDataTypeAndHcommReduceOp(dataType, reduceOp);
    CHK_PRT_RET(ret != HCCL_SUCCESS,
        HCCL_ERROR("FAIL at CheckReduceHcommDataTypeAndHcommReduceOp dataType[%d], reduceOp[%d].", dataType, reduceOp), ret);
    return HCCL_SUCCESS;
}

static inline HcclResult ParseData(const HcommBatchTransferDesc &transferDesc, void* &rmt, void* &loc,
                        uint64_t &len, TransferType &tfType, HcommDataType &dataType, HcommReduceOp &reduceOp)
{
    if (transferDesc.transType == HCOMM_TRANSFER_TYPE_WRITE) {
        rmt = transferDesc.transferInfo.write.dst;
        loc = transferDesc.transferInfo.write.src;
        len = transferDesc.transferInfo.write.len;
        tfType = TransferType::WRITE;
    } else if (transferDesc.transType == HCOMM_TRANSFER_TYPE_READ) {
        rmt = transferDesc.transferInfo.read.src;
        loc = transferDesc.transferInfo.read.dst;
        len = transferDesc.transferInfo.read.len;
        tfType = TransferType::READ;
    } else if (transferDesc.transType == HCOMM_TRANSFER_TYPE_WRITE_REDUCE) {
        rmt = transferDesc.transferInfo.reduce.dst;
        loc = transferDesc.transferInfo.reduce.src;
        tfType = TransferType::WRITE_REDUCE;
        CHK_RET(ParasReduceData(transferDesc, len, dataType, reduceOp));
    } else if (transferDesc.transType == HCOMM_TRANSFER_TYPE_READ_REDUCE) {
        rmt = transferDesc.transferInfo.reduce.src;
        loc = transferDesc.transferInfo.reduce.dst;
        tfType = TransferType::READ_REDUCE;
        CHK_RET(ParasReduceData(transferDesc, len, dataType, reduceOp));
    } else {
        HCCL_ERROR("[%s] unsupported transType[%d]", __func__, transferDesc.transType);
        return HCCL_E_NOT_SUPPORT;
    }
    if (reduceOp != HcommReduceOp::HCOMM_REDUCE_RESERVED) {
        len = len * GetHcommDataTypeSize(dataType);
    }
    return HCCL_SUCCESS;
}

class UbTransportLiteImpl : public BaseTransportLiteImpl {
public:
    explicit UbTransportLiteImpl(
        std::vector<char>& uniqueId,
        std::function<void(u32 streamId, u32 taskId, const TaskParam& taskParam)> callback);

    UbTransportLiteImpl(std::vector<char>& uniqueId);
    void Init(std::vector<char>& uniqueId);

    ~UbTransportLiteImpl() override;

    std::string Describe() const override;

    Buffer GetRmtBuffer(u32 index) override;

    Eid GetLocEid() const;
    Eid GetRmtEid() const;

    void Post(u32 index, const StreamLite& stream) override;

    void Wait(u32 index, const StreamLite& stream) override;

    void Read(const RmaBufferLite& loc, const Buffer& rmt, const StreamLite& stream) override;

    void Write(const RmaBufferLite& loc, const Buffer& rmt, const StreamLite& stream) override;

    void ReadReduce(
        const RmaBufferLite& loc, const Buffer& rmt, const ReduceIn& reduceIn, const StreamLite& stream) override;

    void WriteReduce(
        const RmaBufferLite& loc, const Buffer& rmt, const ReduceIn& reduceIn, const StreamLite& stream) override;

    void WriteWithNotify(
        const RmaBufferLite& loc, const Buffer& rmt, const WithNotifyIn& withNotify, const StreamLite& stream) override;

    void WriteReduceWithNotify(
        const RmaBufferLite& loc, const Buffer& rmt, const ReduceIn& reduceIn, const WithNotifyIn& withNotify,
        const StreamLite& stream) override;

    void BatchOneSidedWrite(
        const vector<RmaBufSliceLite>& loc, const vector<RmtRmaBufSliceLite>& rmt, const StreamLite& stream) override;

    void BatchOneSidedRead(
        const vector<RmaBufSliceLite>& loc, const vector<RmtRmaBufSliceLite>& rmt, const StreamLite& stream) override;

    void BatchTransfer(
        const std::vector<RmaBufferLite>& loc, const std::vector<Buffer>& rmt,
        const std::vector<TransferOp>& transferOp, const StreamLite& stream) override;

    HcclResult BuildLocRmaBufferLite(const uintptr_t addr, const size_t size, RmaBufferLite& rmaBufferLite) const;
    HcclResult Fence();

    HcclResult Clean();
    HcclResult Resume(std::vector<char>& uniqueId);

    HcclResult ExecuteBatchTransfer(StreamLite *streamLitePtr, const HcommBatchTransferDesc *transferDescs,
                        uint32_t transferDescNum);

    HcclResult SetAddTaskInfoCallback(
        std::function<HcclResult(u32, u32, const TaskParam&, u64)> callback); // 自定义算子流程上报task的Callback
private:
    u32 notifyNum{0};
    u32 bufferNum{0};
    u32 connNum{0};
    bool fence_{false};

    struct RmtUbBufLite {
        u64 addr;
        u64 size;
        u32 tokenId;
        u32 tokenValue;
        std::string Describe() const { return StringFormat("RmtUbBufLite[addr=0x%llx, size=0x%llx]", addr, size); }
    };

    struct LocUbBufLite {
        u64 addr;
        u64 size;
        u32 tokenId;
        u32 tokenValue;
        std::string Describe() const { return StringFormat("LocUbBufLite[addr=0x%llx, size=0x%llx]", addr, size); }
    };

    std::vector<char> wqeData;    // connection返回的WQE内容
    ConnLiteOperationOut connOut; // connection的输出

    void ClearConnOut();

    using RmtUbBufLiteVec = std::vector<RmtUbBufLite>;
    using LocUbBufLiteVec = std::vector<LocUbBufLite>;
    MAKE_ENUM(RmaUbBufType, NOTIFY, BUFFER)
    RmtUbBufLiteVec rmtNotifyVec;
    RmtUbBufLiteVec rmtBufferVec;
    LocUbBufLiteVec locBufferVec;

    RmtRmaBufSliceLite GetRmtNotifySliceLite(u32 index);
    RmtRmaBufSliceLite GetRmtRmaBufSliceLite(const Buffer& rmtBuf);

    RmaBufSliceLite GetRmaBufSlicelite(const RmaBufferLite& lite) const;
    RmtRmaBufSliceLite GetRmtRmaBufSliceLite(const RmaBufferLite& lite) const;

    std::vector<std::unique_ptr<NotifyLite>> locNotifyVec;

    // N秒快恢需要清理的两个资源
    std::vector<std::vector<char>> connUniqueIdVec;
    std::vector<RmaConnLite*> connVec;

    std::function<void(u32 streamId, u32 taskId, const TaskParam& taskParam)> callback_{nullptr};

    std::function<HcclResult(u32, u32, const TaskParam&, u64)> newCallback_{nullptr};

    void ProfilingProcess(
        const RmaBufSliceLite& loc, const RmtRmaBufSliceLite& rmt, const StreamLite& stream, DmaOp dmaOp, u32 taskId);

    void ReduceProfilingProcess(
        const RmaBufSliceLite& loc, const RmtRmaBufSliceLite& rmt, const ReduceIn& reduceIn, const StreamLite& stream,
        u32 taskId);

    void ParseLocNotifyVec(std::vector<char>& data);

    void ParseRmtBufferVec(std::vector<char>& data, RmtUbBufLiteVec& vec, RmaUbBufType rmtType) const;

    void ParseLocBufferVec(std::vector<char>& data, LocUbBufLiteVec& vec, RmaUbBufType rmtType) const;

    void ParseConnVec(std::vector<char>& data);

    void BuildUbDbSendTask(const StreamLite& stream, const UbJettyLiteId& jettyLiteId, u32 pi);

    void BuildNotifyWaitTask(const StreamLite& stream, u32 notifyId);

    void CheckConnVec(const std::string& desc);

    void SetFenceConfig(SqeConfigLite& cfg);
};

inline HcclResult UbTransportLiteImpl::ExecuteBatchTransfer(StreamLite *streamLitePtr,
    const HcommBatchTransferDesc *transferDescs, uint32_t transferDescNum)
{
    std::vector<RmaBufferLite> locSlices;
    std::vector<Buffer> rmtSlices;
    std::vector<BaseTransportLiteImpl::TransferOp> transferOps;

    locSlices.reserve(transferDescNum);
    rmtSlices.reserve(transferDescNum);
    transferOps.reserve(transferDescNum);

    for (uint32_t i = 0; i < transferDescNum; i++) {
        RmaBufferLite locRmaBuf;
        void *rmt = nullptr;
        void *loc = nullptr;
        uint64_t len = 0;
        TransferType tfType;
        HcommDataType dataType{HcommDataType::HCOMM_DATA_TYPE_RESERVED};
        HcommReduceOp reduceOp{HcommReduceOp::HCOMM_REDUCE_RESERVED};
        HcclResult ret = ParseData(transferDescs[i], rmt, loc, len, tfType, dataType, reduceOp);
        CHK_PRT_RET(ret != HCCL_SUCCESS,
            HCCL_ERROR("[%s] FAIL at ParseData for index %u.",  __func__, i), ret);
        CHK_PTR_NULL(rmt);
        CHK_PTR_NULL(loc);

        ret = BuildLocRmaBufferLite(reinterpret_cast<uintptr_t>(loc), len, locRmaBuf);
        CHK_PRT_RET(ret != HCCL_SUCCESS,
            HCCL_ERROR("[%s] FAIL at BuildLocRmaBufferLite for index %u. rmt[0x%llx], loc[0x%llx], len[0x%llx], tfType[%u], dataType[%d], reduceOp[%d].",
            __func__, i, rmt, loc, len, tfType, dataType, reduceOp), ret);
        locSlices.push_back(locRmaBuf);

        const Buffer rmtBuf{reinterpret_cast<uintptr_t>(rmt), len};
        rmtSlices.push_back(rmtBuf);

        ReduceIn reduceIn{mapHcommDataTypeA5.at(dataType), mapHcommReduceOpA5.at(reduceOp)};
        transferOps.push_back(BaseTransportLiteImpl::TransferOp{tfType, reduceIn});

        HCCL_DEBUG("[%s] Prepared transfer op for index %u. rmt[0x%llx], loc[0x%llx], len[0x%llx], tfType[%u], dataType[%d], reduceOp[%d].",
            __func__, i, rmt, loc, len, tfType, dataType, reduceOp);
    }
    EXECEPTION_CATCH(BatchTransfer(locSlices, rmtSlices, transferOps, *streamLitePtr), return HCCL_E_INTERNAL);
    return HCCL_SUCCESS;
}

} // namespace Hccl
#endif
