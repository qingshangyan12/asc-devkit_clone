/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <array>
#include <cstring>
#include <deque>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>
#include "gtest/gtest.h"
#include "alg_data_trans_wrapper.h"
#include "ins_temp_all_gather_nhr.h"
#include "mc2_boundary_group_state.h"

using namespace mc2_ops_hccl;
namespace {
enum class Kind { COPY, RECORD, WAIT, CHECK };
struct Task {
    Kind kind;
    uint64_t owner;
    uint32_t slot;
    void* dst = nullptr;
    const void* src = nullptr;
    uint64_t bytes = 0;
};
std::map<ThreadHandle, std::deque<Task>> queues;
uint32_t entryRecords = 0;
uint32_t internalRecords = 0;
uint32_t payloads = 0;
constexpr uint32_t RANKS = 8;
constexpr uint32_t TURNS = 3;
constexpr uint32_t COUNT = 16;
constexpr uint32_t STRIDE = 19;
uint64_t ChannelMailbox(ChannelHandle c, bool remote)
{
    const uint64_t from = (c - 1U) / RANKS;
    const uint64_t to = (c - 1U) % RANKS;
    return 1000U + (remote ? to * RANKS + from : from * RANKS + to);
}
void Copy(ThreadHandle t, void* dst, const void* src, uint64_t bytes)
{
    queues[t].push_back({Kind::COPY, 0, 0, dst, src, bytes});
    ++payloads;
}
bool Execute(bool slaveFirst)
{
    std::map<std::pair<uint64_t, uint32_t>, bool> signals;
    for (uint32_t pass = 0; pass < 100000; ++pass) {
        bool progress = false;
        bool empty = true;
        // Drain each runnable stream, intentionally producing extreme main/slave skew.
        for (uint32_t index = 0; index < RANKS * 2; ++index) {
            const uint32_t rank = index / 2;
            const bool slave = (index % 2 == 0) == slaveFirst;
            auto& q = queues[rank * 2U + (slave ? 2U : 1U)];
            while (!q.empty()) {
                empty = false;
                const Task t = q.front();
                auto& signal = signals[{t.owner, t.slot}];
                if (t.kind == Kind::WAIT && !signal) {
                    break;
                }
                if (t.kind == Kind::RECORD) {
                    if (signal) {
                        ADD_FAILURE() << "Overwrite of an unconsumed ordinary Notify";
                        return false;
                    }
                    signal = true;
                } else if (t.kind == Kind::WAIT) {
                    signal = false;
                } else if (t.kind == Kind::CHECK) {
                    if (std::memcmp(t.dst, t.src, t.bytes) != 0) {
                        ADD_FAILURE() << "Per-turn completion preceded output readiness";
                        return false;
                    }
                } else {
                    std::memcpy(t.dst, t.src, t.bytes);
                }
                q.pop_front();
                progress = true;
            }
        }
        if (empty) {
            return true;
        }
        if (!progress) {
            ADD_FAILURE() << "Deadlock in captured primitive queues";
            return false;
        }
    }
    return false;
}
} // namespace

extern "C" int32_t __wrap_HcommThreadNotifyRecordOnThread(ThreadHandle t, ThreadHandle dst, uint32_t n)
{
    queues[t].push_back({Kind::RECORD, dst, n});
    entryRecords += n == 0U && t % 2U == 1U;
    internalRecords += n == 1U;
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommThreadNotifyWaitOnThread(ThreadHandle t, uint32_t n, uint32_t)
{
    queues[t].push_back({Kind::WAIT, t, n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommChannelNotifyRecordOnThread(ThreadHandle t, ChannelHandle c, uint32_t n)
{
    queues[t].push_back({Kind::RECORD, ChannelMailbox(c, true), n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommChannelNotifyWaitOnThread(ThreadHandle t, ChannelHandle c, uint32_t n, uint32_t)
{
    queues[t].push_back({Kind::WAIT, ChannelMailbox(c, false), n});
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommLocalCopyOnThread(ThreadHandle t, void* dst, const void* src, uint64_t bytes)
{
    Copy(t, dst, src, bytes);
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommReadOnThread(ThreadHandle t, ChannelHandle, void* dst, const void* src, uint64_t bytes)
{
    Copy(t, dst, src, bytes);
    return HCCL_SUCCESS;
}
extern "C" int32_t __wrap_HcommWriteOnThread(ThreadHandle t, ChannelHandle, void* dst, const void* src, uint64_t bytes)
{
    Copy(t, dst, src, bytes);
    return HCCL_SUCCESS;
}

TEST(Mc2Boundary, WireRoundTripAndMalformedExtension)
{
    mc2_boundary::Config config;
    config.mode = 1U;
    config.groupId = 7U;
    config.totalTurns = 3U;
    std::array<uint8_t, 232> bytes{};
    mc2_boundary::Config restored;
    ASSERT_TRUE(mc2_boundary::Decode(bytes.data(), bytes.size(), restored));
    EXPECT_EQ(restored.mode, 0U);
    mc2_boundary::Encode(bytes.data() + 1U, config);
    ASSERT_TRUE(mc2_boundary::Decode(bytes.data() + 1U, bytes.size() - 1U, restored, true));
    EXPECT_EQ(restored.groupId, 7U);
    EXPECT_EQ(restored.totalTurns, 3U);
    bytes[5] = 99U;
    EXPECT_FALSE(mc2_boundary::Decode(bytes.data() + 1U, bytes.size() - 1U, restored, true));
}

TEST(Mc2Boundary, CrossPrepareRepeatAndFinalizationContract)
{
    mc2_boundary::Config config;
    config.mode = 1U;
    config.groupId = 7U;
    config.totalTurns = 3U;
    mc2_boundary::GroupState group;
    ASSERT_TRUE(group.Begin(config, 2));
    EXPECT_FALSE(group.Complete());
    ASSERT_TRUE(group.StartTurn());
    group.CommitTurn();
    ASSERT_TRUE(group.StartTurn());
    group.CommitTurn();
    EXPECT_EQ(group.Turn(), 2U);
    EXPECT_FALSE(group.Begin(mc2_boundary::Config{}, 1));
    auto wrong = config;
    wrong.totalTurns = 4;
    EXPECT_FALSE(group.Begin(wrong, 1));
    EXPECT_FALSE(group.Begin(config, 2));
    ASSERT_TRUE(group.Begin(config, 1));
    ASSERT_TRUE(group.StartTurn());
    EXPECT_FALSE(group.Complete());
    group.CommitTurn();
    EXPECT_TRUE(group.Complete());
    EXPECT_FALSE(group.Begin(config, 1));
}

TEST(Mc2Boundary, FailedSubmissionCannotFinalize)
{
    mc2_boundary::Config config;
    config.mode = 1U;
    config.totalTurns = 1U;
    mc2_boundary::GroupState group;
    ASSERT_TRUE(group.Begin(config, 1));
    ASSERT_TRUE(group.StartTurn());
    EXPECT_FALSE(group.Complete());
    EXPECT_FALSE(group.Begin(config, 1));
}

static void RunNhrCase(uint32_t loops)
{
    for (const bool enabled : {false, true}) {
        for (const bool slaveFirst : {false, true}) {
            queues.clear();
            entryRecords = internalRecords = payloads = 0U;
            ASSERT_EQ(InitHcommBatchTransferOnThreadSupported(false), HCCL_SUCCESS);
            std::array<std::array<int8_t, TURNS * COUNT>, RANKS> input{};
            std::array<std::array<int8_t, TURNS * RANKS * STRIDE>, RANKS> output{};
            std::array<std::array<int8_t, RANKS * COUNT>, RANKS> scratch{};
            std::array<std::array<int8_t, RANKS * STRIDE>, TURNS> expected{};
            std::vector<u32> ranks(RANKS);
            std::iota(ranks.begin(), ranks.end(), 0U);
            for (uint32_t rank = 0; rank < RANKS; ++rank) {
                output[rank].fill(-1);
                for (uint32_t i = 0; i < input[rank].size(); ++i) {
                    input[rank][i] = static_cast<int8_t>((rank * 11U + i) % 127U);
                }
            }
            for (uint32_t turn = 0; turn < TURNS; ++turn) {
                expected[turn].fill(-1);
                for (uint32_t peer = 0; peer < RANKS; ++peer) {
                    std::memcpy(expected[turn].data() + peer * STRIDE,
                        input[peer].data() + turn * COUNT, COUNT);
                }
                for (uint32_t rank = 0; rank < RANKS; ++rank) {
                    OpParam param{};
                    param.DataDes.dataType = HCCL_DATA_TYPE_INT8;
                    TemplateResource resource{};
                    resource.threads = {rank * 2U + 1U, rank * 2U + 2U};
                    for (uint32_t peer = 0; peer < RANKS; ++peer) {
                        if (peer == rank) {
                            continue;
                        }
                        ChannelInfo channel{};
                        channel.remoteRank = peer;
                        channel.handle = rank * RANKS + peer + 1U;
                        channel.protocol = COMM_PROTOCOL_UBC_CTP;
                        channel.remoteCclMem.addr = scratch[peer].data();
                        channel.remoteCclMem.size = scratch[peer].size();
                        resource.channels[peer] = {channel};
                    }
                    TemplateDataParams data{};
                    data.buffInfo.inputPtr = input[rank].data() + turn * COUNT;
                    data.buffInfo.outputPtr = output[rank].data() + turn * RANKS * STRIDE;
                    data.buffInfo.hcclBuff.addr = scratch[rank].data();
                    data.buffInfo.hcclBuff.size = scratch[rank].size();
                    data.buffInfo.inBuffType = BufferType::INPUT;
                    data.buffInfo.outBuffType = BufferType::OUTPUT;
                    data.buffInfo.hcclBuffType = BufferType::HCCL_BUFFER;
                    data.count = data.sliceSize = data.tailSize = COUNT;
                    data.outputSliceStride = STRIDE;
                    data.repeatNum = 1U;
                    InsTempAllGatherNHR nhr(param, rank, {ranks});
                    mc2_boundary::TurnPolicy policy;
                    policy.enabled = enabled;
                    policy.skipEntry = enabled && turn != 0U;
                    policy.exitNotify = 3U + turn;
                    for (uint32_t loop = 0; loop < loops; ++loop) {
                        // The two-loop case includes a smaller tail with a different scratch layout.
                        data.buffInfo.inBuffBaseOff = loop == 0U ? 0U : 12U;
                        data.buffInfo.outBuffBaseOff = data.buffInfo.inBuffBaseOff;
                        data.count = data.sliceSize = data.tailSize =
                            loops == 1U ? COUNT : (loop == 0U ? 12U : 4U);
                        auto loopPolicy = policy;
                        loopPolicy.skipEntry = policy.skipEntry && loop == 0U;
                        loopPolicy.enabled = policy.enabled && loop + 1U == loops;
                        ASSERT_EQ(nhr.KernelRunBoundary(param, data, resource, loopPolicy), HCCL_SUCCESS);
                    }
                    // CCoreRecord follows the final exit on the main thread.
                    queues[resource.threads[0]].push_back(
                        {Kind::CHECK, 0, 0, data.buffInfo.outputPtr, expected[turn].data(), RANKS * STRIDE});
                }
            }
            EXPECT_EQ(entryRecords, RANKS * (enabled ? 1U + TURNS * (loops - 1U) : TURNS * loops));
            EXPECT_EQ(internalRecords, RANKS * TURNS * loops);
            ASSERT_GT(payloads, 0U);
            ASSERT_TRUE(Execute(slaveFirst));
            for (uint32_t rank = 0; rank < RANKS; ++rank) {
                for (uint32_t turn = 0; turn < TURNS; ++turn) {
                    for (uint32_t peer = 0; peer < RANKS; ++peer) {
                        for (uint32_t i = 0; i < STRIDE; ++i) {
                            const int8_t expected = i < COUNT ? input[peer][turn * COUNT + i] : -1;
                            EXPECT_EQ(output[rank][turn * RANKS * STRIDE + peer * STRIDE + i], expected);
                        }
                    }
                }
            }
        }
    }
}

TEST(Mc2Boundary, CapturedNhrQueuesPreserveDataWithFastAndSlowCopyThreads)
{
    RunNhrCase(1U);
    RunNhrCase(2U);
}
