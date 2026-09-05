/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <algorithm>
#include <cstring>
#include <limits>
#include <map>
#include <numeric>
#include <vector>

#include "gtest/gtest.h"
#include "alg_data_trans_wrapper.h"
#include "hcomm_primitives_dl.h"
#include "ins_temp_reduce_scatter_mesh_1D_meshchunk.h"

using namespace mc2_ops_hccl;

namespace {
enum class Kind { COPY, REDUCE, CHANNEL_RECORD, CHANNEL_WAIT, THREAD_RECORD, THREAD_WAIT };
struct Task {
    Kind kind;
    ThreadHandle thread;
    uint64_t target;
    uint32_t notify;
    void* dst = nullptr;
    const void* src = nullptr;
    uint64_t count = 0;
    HcommDataType dtype = static_cast<HcommDataType>(HCCL_DATA_TYPE_INT8);
};
std::vector<Task> tasks;
std::vector<HcclHcommBatchTransferDesc> descriptors;
bool batchSupported = true;
int32_t batchResult = HCCL_SUCCESS;
int32_t copyResult = HCCL_SUCCESS;
size_t batchCalls = 0;

class MeshChunkTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tasks.clear();
        descriptors.clear();
        batchSupported = true;
        batchResult = HCCL_SUCCESS;
        copyResult = HCCL_SUCCESS;
        batchCalls = 0;
        ASSERT_EQ(InitHcommBatchTransferOnThreadSupported(true), HCCL_SUCCESS);
    }
};

SendRecvReduceInfo MakeTransfer(const std::vector<uint64_t>& counts)
{
    ChannelInfo channel{};
    channel.handle = 2;
    std::vector<DataSlice> src, dst;
    uint64_t offset = 0;
    for (auto count : counts) {
        src.emplace_back(reinterpret_cast<void*>(0x100000), offset, count * sizeof(float), count);
        dst.emplace_back(reinterpret_cast<void*>(0x200000), offset, count * sizeof(float), count);
        offset += count * sizeof(float);
    }
    return {{channel, channel}, {{src, dst}, {src, dst}}, HCCL_DATA_TYPE_FP32, HCCL_REDUCE_SUM};
}

size_t Count(Kind kind, uint32_t notify)
{
    return std::count_if(tasks.begin(), tasks.end(), [=](const Task& t) {
        return t.kind == kind && t.notify == notify;
    });
}
} // namespace

extern "C" bool __wrap_HcommIsSupportHcommBatchTransferOnThread() { return batchSupported; }
extern "C" int32_t __wrap_HcommChannelNotifyRecordOnThread(ThreadHandle t, ChannelHandle c, uint32_t n)
{
    tasks.push_back({Kind::CHANNEL_RECORD, t, c, n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommChannelNotifyWaitOnThread(ThreadHandle t, ChannelHandle c, uint32_t n, uint32_t)
{
    tasks.push_back({Kind::CHANNEL_WAIT, t, c, n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommThreadNotifyRecordOnThread(ThreadHandle t, ThreadHandle dst, uint32_t n)
{
    tasks.push_back({Kind::THREAD_RECORD, t, dst, n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommThreadNotifyWaitOnThread(ThreadHandle t, uint32_t n, uint32_t)
{
    tasks.push_back({Kind::THREAD_WAIT, t, t, n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommLocalCopyOnThread(ThreadHandle t, void* dst, const void* src, uint64_t len)
{
    if (copyResult != HCCL_SUCCESS) {
        return copyResult;
    }
    tasks.push_back({Kind::COPY, t, 0, 0, dst, src, len});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommWriteReduceOnThread(
    ThreadHandle t, ChannelHandle c, void* dst, const void* src, uint64_t count, HcommDataType dtype, HcommReduceOp)
{
    tasks.push_back({Kind::REDUCE, t, c, 0, dst, src, count, dtype});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcclHcommBatchTransferOnThread(
    ThreadHandle t, ChannelHandle c, const HcclHcommBatchTransferDesc* desc, uint32_t num)
{
    ++batchCalls;
    descriptors.insert(descriptors.end(), desc, desc + num);
    if (batchResult != HCCL_SUCCESS) {
        return batchResult;
    }
    for (uint32_t i = 0; i < num; ++i) {
        if (desc[i].transType == HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE_WITH_NOTIFY) {
            const auto& r = desc[i].transferInfo.writeReduceWithNotify;
            __wrap_HcommWriteReduceOnThread(t, c, r.dst, r.src, r.count, r.dataType, r.reduceOp);
            // Model completion notification AFTER the payload, on the same ordered thread.
            __wrap_HcommChannelNotifyRecordOnThread(t, c, r.notifyIdx);
        } else {
            EXPECT_EQ(desc[i].transType, HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE);
            const auto& r = desc[i].transferInfo.reduce;
            __wrap_HcommWriteReduceOnThread(t, c, r.dst, r.src, r.count, r.dataType, r.reduceOp);
        }
    }
    return HCCL_SUCCESS;
}

TEST_F(MeshChunkTest, FusesOnlyLastNonemptyDescriptorAndPreservesFields)
{
    for (const auto& counts : {std::vector<uint64_t>{9}, {3, 7, 0}, {0, 3, 0, 7, 0}}) {
        tasks.clear();
        descriptors.clear();
        ASSERT_EQ(SendRecvBatchWriteReduce(MakeTransfer(counts), 1, true), HCCL_SUCCESS);
        ASSERT_FALSE(descriptors.empty());
        for (size_t i = 0; i + 1 < descriptors.size(); ++i) {
            EXPECT_EQ(descriptors[i].transType, HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE);
        }
        const auto& last = descriptors.back();
        EXPECT_EQ(last.transType, HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE_WITH_NOTIFY);
        const auto& fused = last.transferInfo.writeReduceWithNotify;
        const auto lastCount = *std::find_if(counts.rbegin(), counts.rend(), [](uint64_t n) { return n != 0; });
        const auto offset = (std::accumulate(counts.begin(), counts.end(), uint64_t{0}) - lastCount) * sizeof(float);
        EXPECT_EQ(fused.count, lastCount);
        EXPECT_EQ(reinterpret_cast<uintptr_t>(fused.src), 0x100000U + offset);
        EXPECT_EQ(reinterpret_cast<uintptr_t>(fused.dst), 0x200000U + offset);
        EXPECT_EQ(fused.dataType, static_cast<HcommDataType>(HCCL_DATA_TYPE_FP32));
        EXPECT_EQ(fused.reduceOp, static_cast<HcommReduceOp>(HCCL_REDUCE_SUM));
        EXPECT_EQ(fused.notifyIdx, NOTIFY_IDX_DATA_SIGNAL);
        EXPECT_EQ(Count(Kind::CHANNEL_RECORD, NOTIFY_IDX_ACK), 1U);
        EXPECT_EQ(Count(Kind::CHANNEL_WAIT, NOTIFY_IDX_ACK), 1U);
        EXPECT_EQ(Count(Kind::CHANNEL_RECORD, NOTIFY_IDX_DATA_SIGNAL), 1U);
        EXPECT_EQ(Count(Kind::CHANNEL_WAIT, NOTIFY_IDX_DATA_SIGNAL), 1U);
    }
}

TEST_F(MeshChunkTest, EmptyDataStillSignalsAndDefaultPathDoesNotFuse)
{
    ASSERT_EQ(SendRecvBatchWriteReduce(MakeTransfer({0, 0}), 1, true), HCCL_SUCCESS);
    EXPECT_EQ(batchCalls, 0U);
    EXPECT_EQ(Count(Kind::CHANNEL_RECORD, NOTIFY_IDX_DATA_SIGNAL), 1U);
    ASSERT_EQ(SendRecvBatchWriteReduce(MakeTransfer({2}), 1), HCCL_SUCCESS);
    ASSERT_EQ(descriptors.size(), 1U);
    EXPECT_EQ(descriptors[0].transType, HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE);
}

TEST_F(MeshChunkTest, OldInterfaceFallbackAndBatchErrorNeverResubmit)
{
    batchSupported = false;
    ASSERT_EQ(SendRecvBatchWriteReduce(MakeTransfer({3}), 1, true), HCCL_SUCCESS);
    EXPECT_EQ(batchCalls, 0U);
    EXPECT_EQ(Count(Kind::CHANNEL_RECORD, NOTIFY_IDX_DATA_SIGNAL), 1U);
    tasks.clear();
    batchSupported = true;
    batchResult = HCCL_E_RUNTIME;
    EXPECT_EQ(SendRecvBatchWriteReduce(MakeTransfer({3}), 1, true), HCCL_E_RUNTIME);
    EXPECT_EQ(batchCalls, 1U);
    EXPECT_EQ(Count(Kind::REDUCE, 0), 0U);
    EXPECT_EQ(Count(Kind::CHANNEL_RECORD, NOTIFY_IDX_DATA_SIGNAL), 0U);
}
