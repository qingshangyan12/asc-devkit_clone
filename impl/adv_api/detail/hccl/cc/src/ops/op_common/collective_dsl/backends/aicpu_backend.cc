// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "aicpu_backend.h"

#include "alg_data_trans_wrapper.h"
#include "log.h"

#include <map>
#include <set>

namespace mc2_ops_hccl {
namespace dsl {
namespace {

void* Base(const Slice& slice, const AicpuMemory& memory)
{
    if (slice.rank == memory.rank) {
        return slice.buffer == Buffer::INPUT ? memory.input : memory.output;
    }
    const std::vector<void*>& remote = slice.buffer == Buffer::INPUT ? memory.remoteInput : memory.remoteOutput;
    return slice.rank < remote.size() ? remote[slice.rank] : nullptr;
}

HcclResult ToDataSlice(const Slice& slice, const AicpuMemory& memory, DataSlice& result)
{
    void* base = Base(slice, memory);
    CHK_PRT_RET(base == nullptr, HCCL_ERROR("[CollectiveDsl] null buffer for rank[%u]", slice.rank), HCCL_E_PTR);
    const uint64_t ownerOffset = slice.buffer == Buffer::OUTPUT ? slice.owner * memory.outputSlotBytes : 0;
    result = DataSlice(base, ownerOffset + slice.offset, slice.bytes);
    return HCCL_SUCCESS;
}

HcclResult ToDataSlices(const std::vector<Slice>& slices, const AicpuMemory& memory, std::vector<DataSlice>& result)
{
    result.clear();
    result.reserve(slices.size());
    for (const Slice& slice : slices) {
        DataSlice data(nullptr, 0, 0);
        CHK_RET(ToDataSlice(slice, memory, data));
        result.push_back(data);
    }
    return HCCL_SUCCESS;
}

const ChannelInfo* GetChannel(const TemplateResource& resource, const Channel& channel)
{
    const auto peer = resource.channels.find(channel.peer);
    if (peer == resource.channels.end() || channel.path >= peer->second.size()) {
        return nullptr;
    }
    return &peer->second[channel.path];
}

} // namespace

HcclResult CalcAicpuResources(
    const Plan& plan, const std::vector<HcclChannelDesc>& channelDescs, AlgResourceRequest& request)
{
    const ResourcePlan need = CalcResources(plan);
    std::map<Rank, uint32_t> available;
    for (const HcclChannelDesc& channel : channelDescs) {
        ++available[channel.remoteRank];
    }
    for (const ChannelNeed& channel : need.channels) {
        CHK_PRT_RET(
            available[channel.peer] <= channel.path,
            HCCL_ERROR("[CollectiveDsl] channel[%u][%u] was not requested", channel.peer, channel.path), HCCL_E_PARA);
    }

    request.channels.push_back(channelDescs);
    // 顺序 backend 只申请主 thread，不需要主从 thread notify。
    request.slaveThreadNum = 0;
    request.notifyNumOnMainThread = 0;
    request.notifyNumPerThread.clear();
    return HCCL_SUCCESS;
}

HcclResult RunAicpu(const Plan& plan, const AicpuMemory& memory, const TemplateResource& resource)
{
    CHK_PRT_RET(resource.threads.empty(), HCCL_ERROR("[CollectiveDsl] no AICPU thread"), HCCL_E_PTR);
    const ThreadHandle thread = resource.threads[0];
    for (const Instruction& ins : plan.instructions) {
        if (ins.op == Op::LOCAL_COPY) {
            DataSlice src(nullptr, 0, 0);
            DataSlice dst(nullptr, 0, 0);
            CHK_RET(ToDataSlice(ins.src, memory, src));
            CHK_RET(ToDataSlice(ins.dst, memory, dst));
            CHK_RET(LocalCopy(thread, src, dst));
            continue;
        }
        if (ins.op != Op::EXCHANGE) {
            // 单 thread 按 Plan 顺序提交，WAIT/COMPLETE 不需要额外 thread notify。
            continue;
        }

        const ChannelInfo* send = GetChannel(resource, ins.send);
        const ChannelInfo* recv = GetChannel(resource, ins.recv);
        CHK_PRT_RET(
            send == nullptr || recv == nullptr, HCCL_ERROR("[CollectiveDsl] acquired channel does not match Plan"),
            HCCL_E_PTR);
        std::vector<DataSlice> txDst;
        std::vector<DataSlice> txSrc;
        std::vector<DataSlice> rxDst;
        std::vector<DataSlice> rxSrc;
        CHK_RET(ToDataSlices(ins.txDst, memory, txDst));
        CHK_RET(ToDataSlices(ins.txSrc, memory, txSrc));
        CHK_RET(ToDataSlices(ins.rxDst, memory, rxDst));
        CHK_RET(ToDataSlices(ins.rxSrc, memory, rxSrc));
        const TxRxChannels channels(*send, *recv);
        const SlicesList txSlices(txSrc, txDst);
        const SlicesList rxSlices(rxSrc, rxDst);
        const TxRxSlicesList slices(txSlices, rxSlices);
        CHK_RET(SendRecvBatchWrite(SendRecvInfo(channels, slices), thread));
    }
    return HCCL_SUCCESS;
}

} // namespace dsl
} // namespace mc2_ops_hccl
