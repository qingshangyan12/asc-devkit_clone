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
#include <cstdlib>
#include <deque>
#include <map>
#include <numeric>
#include <set>
#include <tuple>
#include <vector>
#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "alg_data_trans_wrapper.h"
#include "ins_temp_all_gather_nhr.h"
#include "mc2_boundary_group_state.h"
#include "ins_v2_all_gather_sole_executor.h"
#include "topo_match_1d.h"
#include "hcomm_primitives_dl.h"
#include "comm_kfc_open_kernel_adapter.h"
#include "task_cache/mc2_aicpu_task_cache_runner.h"
#include "task_cache_test_stub.h"
#include "alg_env_config.h"

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
constexpr uint32_t RANKS = 16;
constexpr uint32_t TURNS = 3;
constexpr uint32_t COUNT = 144;
constexpr uint32_t STRIDE = 148;
bool batchSupported = false;
uint32_t allocatedThreads = 2U;
uint32_t activeChannels = 1U;
uint32_t batchCalls = 0;
using Queues = std::map<ThreadHandle, std::deque<Task>>;
std::map<std::string, Queues> cachedQueues;
std::map<ThreadHandle, size_t> captureOffsets;
uint32_t replayCalls = 0;

// Model the existing HCOMM cache API, not hardware SQE relocation. The real runner
// chooses miss/hit/bypass and the real executor supplies all captured primitives.
int32_t Lookup(const char* tag, bool* hit)
{
    *hit = cachedQueues.count(tag) != 0U;
    return HCCL_SUCCESS;
}
int32_t CaptureStart(const char*, void**, uint64_t*, uint64_t)
{
    captureOffsets.clear();
    for (const auto& q : queues) {
        captureOffsets[q.first] = q.second.size();
    }
    return HCCL_SUCCESS;
}
int32_t CaptureEnd(const char* tag)
{
    auto& graph = cachedQueues[tag];
    for (const auto& q : queues) {
        graph[q.first] = std::deque<Task>(q.second.begin() + captureOffsets[q.first], q.second.end());
    }
    return HCCL_SUCCESS;
}
int32_t Replay(const char* tag, void**, uint64_t*, uint64_t)
{
    ++replayCalls;
    for (const auto& q : cachedQueues.at(tag)) {
        queues[q.first].insert(queues[q.first].end(), q.second.begin(), q.second.end());
    }
    return HCCL_SUCCESS;
}
int32_t Clear(const char* tag)
{
    cachedQueues.erase(tag);
    return HCCL_SUCCESS;
}
struct CacheHooks {
    CacheHooks()
    {
        ResetAlgEnvConfigInitState();
        ResetMc2TaskCacheTestStubState();
        cachedQueues.clear();
        replayCalls = 0U;
        Mc2HcommAicpuTsTaskCacheSetTestFunctions(Lookup, CaptureStart, CaptureEnd, Replay, Clear);
    }
    ~CacheHooks() { Mc2HcommAicpuTsTaskCacheResetTestFunctions(); }
};

void ExpectSameQueues(const Queues& expected)
{
    ASSERT_EQ(queues.size(), expected.size());
    for (const auto& q : expected) {
        const auto& actual = queues.at(q.first);
        ASSERT_EQ(actual.size(), q.second.size());
        for (size_t i = 0; i < actual.size(); ++i) {
            EXPECT_EQ(actual[i].kind, q.second[i].kind);
            EXPECT_EQ(actual[i].owner, q.second[i].owner);
            EXPECT_EQ(actual[i].slot, q.second[i].slot);
            EXPECT_EQ(actual[i].dst, q.second[i].dst);
            EXPECT_EQ(actual[i].src, q.second[i].src);
            EXPECT_EQ(actual[i].bytes, q.second[i].bytes);
        }
    }
}
uint64_t ChannelMailbox(ChannelHandle c, bool remote)
{
    const uint64_t channel = (c - 1U) / (RANKS * RANKS);
    const uint64_t from = ((c - 1U) / RANKS) % RANKS;
    const uint64_t to = (c - 1U) % RANKS;
    return 100000U + channel * RANKS * RANKS + (remote ? to * RANKS + from : from * RANKS + to);
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
        for (uint32_t index = 0; index < RANKS * allocatedThreads; ++index) {
            const uint32_t rank = index / allocatedThreads;
            const uint32_t lane = (index % allocatedThreads + (slaveFirst ? 1U : 0U)) % allocatedThreads;
            auto& q = queues[rank * allocatedThreads + lane + 1U];
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
                if (slaveFirst && lane >= activeChannels) {
                    // Slow post-copy lanes while communication lanes drain fully.
                    break;
                }
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
    entryRecords += n == 0U && (t - 1U) % allocatedThreads == 0U;
    internalRecords += n == 1U && (t - 1U) % allocatedThreads < activeChannels &&
        (dst - 1U) % allocatedThreads >= activeChannels;
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

extern "C" bool __wrap_HcommIsSupportHcommBatchTransferOnThread() { return batchSupported; }
extern "C" int32_t __wrap_HcclHcommBatchTransferOnThread(
    ThreadHandle t, ChannelHandle c, const HcclHcommBatchTransferDesc* desc, uint32_t num)
{
    ++batchCalls;
    for (uint32_t i = 0; i < num; ++i) {
        if (desc[i].transType == HCCL_HCOMM_TRANSFER_TYPE_READ) {
            const auto& d = desc[i].transferInfo.read;
            Copy(t, d.dst, d.src, d.len);
        } else if (desc[i].transType == HCCL_HCOMM_TRANSFER_TYPE_WRITE) {
            const auto& d = desc[i].transferInfo.write;
            Copy(t, d.dst, d.src, d.len);
        } else if (desc[i].transType == HCCL_HCOMM_TRANSFER_TYPE_WRITE_WITH_NOTIFY) {
            const auto& d = desc[i].transferInfo.writeWithNotify;
            Copy(t, d.dst, d.src, d.len);
            __wrap_HcommChannelNotifyRecordOnThread(t, c, d.notifyIdx);
        } else if (desc[i].transType == HCCL_HCOMM_TRANSFER_TYPE_NOTIFY_RECORD) {
            __wrap_HcommChannelNotifyRecordOnThread(t, c, desc[i].transferInfo.notifyRecord.notifyIdx);
        } else {
            ADD_FAILURE() << "Unexpected AllGather descriptor " << desc[i].transType;
            return HCCL_E_PARA;
        }
    }
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

static void RunNhrCase(uint32_t loops, bool batch, HcclDataType dtype, uint32_t elementSize,
    uint32_t channels = 1U, uint32_t allocation = 2U)
{
    allocatedThreads = allocation;
    activeChannels = channels;
    for (const bool enabled : {false, true}) {
        for (const bool slaveFirst : {false, true}) {
            CacheHooks hooks;
            Mc2AicpuTaskCacheManager cache(128U, 65536U);
            Mc2AicpuTaskCacheManager fullCache(1U, 4096U);
            ASSERT_EQ(fullCache.TryReserve(reinterpret_cast<void*>(1U), "occupied"),
                Mc2TaskCacheReserveResult::RESERVED);
            queues.clear();
            entryRecords = internalRecords = payloads = 0U;
            batchSupported = batch;
            batchCalls = 0;
            ASSERT_EQ(InitHcommBatchTransferOnThreadSupported(batch), HCCL_SUCCESS);
            std::array<std::array<int8_t, TURNS * COUNT>, RANKS> input{};
            std::array<std::array<int8_t, TURNS * RANKS * STRIDE>, RANKS> output{};
            std::array<std::array<int8_t, RANKS * 256>, RANKS> scratch{};
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
                    param.DataDes.dataType = dtype;
                    param.DataDes.count = COUNT / elementSize;
                    param.DataDes.strideCount = STRIDE / elementSize;
                    param.opType = HCCL_CMD_ALLGATHER;
                    param.engine = COMM_ENGINE_AICPU_TS;
                    param.opMode = OpMode::OPBASE;
                    param.hcclComm = reinterpret_cast<void*>(rank + 1U);
                    std::strcpy(param.algTag, "boundary-integration");
                    std::strcpy(param.commName, "boundary-test");
                    std::strcpy(param.algName, "AicpuAllGatherSoleNHR");
                    param.inputPtr = input[rank].data() + turn * COUNT;
                    param.outputPtr = output[rank].data() + turn * RANKS * STRIDE;
                    param.inputSize = COUNT;
                    param.outputSize = RANKS * STRIDE;
                    AlgResourceCtxSerializable resource{};
                    resource.topoInfo.userRank = rank;
                    resource.topoInfo.userRankSize = RANKS;
                    resource.topoInfo.isPod = channels > 1U;
                    resource.algHierarchyInfo.infos = {{ranks}};
                    resource.cclMem.addr = scratch[rank].data();
                    resource.cclMem.size = RANKS * (loops == 1U ? 256U : 128U);
                    resource.notifyNumOnMainThread = allocation - 1U;
                    resource.notifyNumPerThread.assign(allocation - 1U, 2U);
                    resource.channels.resize(1);
                    for (uint32_t t = 0; t < allocation; ++t) {
                        resource.threads.push_back(rank * allocation + t + 1U);
                    }
                    for (uint32_t peer = 0; peer < RANKS; ++peer) {
                        if (peer == rank) {
                            continue;
                        }
                        for (uint32_t c = 0; c < allocation / 2U; ++c) {
                            ChannelInfo channel{};
                            channel.isValid = true;
                            channel.remoteRank = peer;
                            channel.handle = c * RANKS * RANKS + rank * RANKS + peer + 1U;
                            channel.portGroupSize = c == 0U ? 3U : 1U;
                            channel.protocol = COMM_PROTOCOL_UBC_CTP;
                            channel.remoteCclMem.addr = scratch[peer].data();
                            channel.remoteCclMem.size = scratch[peer].size();
                            resource.channels[0].push_back(channel);
                        }
                    }
                    mc2_boundary::Config config;
                    config.mode = enabled ? 1U : 0U;
                    config.totalTurns = TURNS;
                    std::vector<uint8_t> bytes(sizeof(param));
                    std::memcpy(bytes.data(), &param, sizeof(param));
                    mc2_boundary::TurnPolicy policy;
                    ASSERT_EQ(PrepareOpenBoundaryPolicy(bytes, resource, config, turn, policy), enabled);
                    const auto before = queues;
                    uint32_t executions = 0U;
                    auto execute = [&]() {
                        ++executions;
                        // Match LaunchOpenOpParamDataImpl: a fresh executor for each
                        // expansion, not an accumulated RestoreChannelMap instance.
                        InsV2AllGatherSoleExecutor<TopoMatch1D, InsTempAllGatherNHR> executor;
                        return executor.OrchestrateBoundary(param, resource, policy);
                    };
                    const auto tag = enabled ? ":mb2:" + std::to_string(policy.skipEntry) + ":" +
                        std::to_string(policy.exitNotify) + ":" + std::to_string(policy.communicationThreads) +
                        ":" + std::to_string(policy.activeThreads) : std::string();
                    // Clear each case so the first call must capture even when disabled
                    // turns share a shape; relocation belongs to HCOMM's separate tests.
                    cachedQueues.clear();
                    ASSERT_EQ(RunWithMc2TaskCache(param, resource, execute, cache, tag), HCCL_SUCCESS);
                    const auto captured = queues;
                    const auto entries = entryRecords;
                    const auto internals = internalRecords;
                    const auto replays = replayCalls;
                    queues = before;
                    ASSERT_EQ(RunWithMc2TaskCache(param, resource, execute, cache, tag), HCCL_SUCCESS);
                    EXPECT_EQ(replayCalls, replays + 1U);
                    EXPECT_EQ(executions, 1U);
                    ExpectSameQueues(captured);
                    queues = before;
                    ASSERT_EQ(RunWithMc2TaskCache(param, resource, execute, fullCache, tag), HCCL_SUCCESS);
                    EXPECT_EQ(executions, 2U);
                    ExpectSameQueues(captured);
                    entryRecords = entries;
                    internalRecords = internals;
                    // CCoreRecord follows the final exit on the main thread.
                    queues[resource.threads[0]].push_back(
                        {Kind::CHECK, 0, 0, param.outputPtr, expected[turn].data(), RANKS * STRIDE});
                }
            }
            EXPECT_EQ(entryRecords, RANKS * (TURNS * loops * (2U * channels - 1U) -
                (enabled ? (TURNS - 1U) * channels : 0U)));
            EXPECT_EQ(internalRecords, RANKS * TURNS * loops * channels);
            ASSERT_GT(payloads, 0U);
            EXPECT_EQ(batchCalls > 0U, batch);
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
    // Capability is immutable for a process, just as on device. CMake runs this
    // executable twice instead of mutating production capability state for a test.
    const char* value = std::getenv("MC2_BOUNDARY_TEST_BATCH");
    const bool batch = value != nullptr && std::strcmp(value, "1") == 0;
    for (const auto dtype : {HCCL_DATA_TYPE_INT8, HCCL_DATA_TYPE_FP16,
                            HCCL_DATA_TYPE_BFP16, HCCL_DATA_TYPE_FP32}) {
        const uint32_t elementSize = dtype == HCCL_DATA_TYPE_INT8 ? 1U :
            (dtype == HCCL_DATA_TYPE_FP32 ? 4U : 2U);
        RunNhrCase(1U, batch, dtype, elementSize);
        RunNhrCase(2U, batch, dtype, elementSize);
        RunNhrCase(1U, batch, dtype, elementSize, 1U, 4U);
        RunNhrCase(1U, batch, dtype, elementSize, 2U, 4U);
        RunNhrCase(2U, batch, dtype, elementSize, 2U, 4U);
    }
}

TEST(Mc2Boundary, RuntimeFallbackReasonsAndPolicyReset)
{
    std::array<uint8_t, 64> input{}, output{}, scratch{};
    OpParam param{};
    param.opType = HCCL_CMD_ALLGATHER;
    param.engine = COMM_ENGINE_AICPU_TS;
    param.opMode = OpMode::OPBASE;
    param.DataDes.dataType = HCCL_DATA_TYPE_INT8;
    param.DataDes.count = 8U;
    param.inputPtr = input.data();
    param.outputPtr = output.data();
    std::strcpy(param.algName, "AicpuAllGatherSoleNHR");
    AlgResourceCtxSerializable res{};
    res.threads = {1U, 2U};
    res.topoInfo.userRankSize = 2U;
    res.cclMem.addr = scratch.data();
    res.cclMem.size = scratch.size();
    res.notifyNumOnMainThread = 2U;
    ChannelInfo channel{};
    channel.isValid = true;
    channel.remoteRank = 1U;
    channel.handle = 1U;
    channel.protocol = COMM_PROTOCOL_UBC_CTP;
    res.channels = {{channel}};
    mc2_boundary::Config config;
    config.mode = 1U;
    config.totalTurns = 3U;
    mc2_boundary::TurnPolicy policy;
    auto prepare = [&]() {
        std::vector<uint8_t> bytes(sizeof(param));
        std::memcpy(bytes.data(), &param, sizeof(param));
        return PrepareOpenBoundaryPolicy(bytes, res, config, 1U, policy);
    };
    ASSERT_TRUE(prepare());
    EXPECT_TRUE(policy.skipEntry);
    EXPECT_EQ(policy.exitNotify, 4U);
    param.outputPtr = input.data();
    EXPECT_FALSE(prepare());
    EXPECT_STREQ(policy.fallbackReason, "buffer_alias_or_invalid_span");
    EXPECT_FALSE(policy.skipEntry);
    EXPECT_EQ(policy.exitNotify, 0U);
    param.outputPtr = output.data();
    ASSERT_TRUE(prepare()); // A later Prepare must recompute policy for its new addresses.
    param.DataDes.count = 0U;
    EXPECT_FALSE(prepare());
    EXPECT_STREQ(policy.fallbackReason, "zero_count");
    param.DataDes.count = 8U;
    param.DataDes.strideCount = 7U;
    EXPECT_FALSE(prepare());
    EXPECT_STREQ(policy.fallbackReason, "invalid_range_or_stride");
    param.DataDes.strideCount = 0U;
    res.notifyNumOnMainThread = 62U;
    EXPECT_FALSE(prepare());
    EXPECT_STREQ(policy.fallbackReason, "notify_budget");
    res.notifyNumOnMainThread = 2U;
    std::strcpy(param.algName, "AicpuAllGatherSoleNHRMultiLink");
    EXPECT_FALSE(prepare());
    EXPECT_STREQ(policy.fallbackReason, "unsupported_algorithm_or_mode");
}

TEST(Mc2Boundary, EffectiveLayoutAndNotifyMatrix)
{
    AlgResourceCtxSerializable res{};
    res.threads = {1U, 2U, 3U, 4U};
    res.channels.resize(1);
    for (uint32_t peer = 1U; peer < 4U; ++peer) {
        for (uint32_t c = 0U; c < 2U; ++c) {
            ChannelInfo channel{};
            channel.remoteRank = peer;
            channel.handle = peer * 2U + c;
            channel.isValid = true;
            channel.protocol = COMM_PROTOCOL_UBC_CTP;
            channel.portGroupSize = c + 1U;
            res.channels[0].push_back(channel);
        }
    }
    mc2_boundary::TurnPolicy policy;
    ASSERT_TRUE(PrepareOpenBoundaryLayout(res, policy));
    EXPECT_EQ(policy.communicationThreads, 1U);
    EXPECT_EQ(policy.activeThreads, 2U);
    res.topoInfo.isPod = true;
    ASSERT_TRUE(PrepareOpenBoundaryLayout(res, policy));
    EXPECT_EQ(policy.communicationThreads, 2U);
    EXPECT_EQ(policy.activeThreads, 4U);
    uint32_t required = 0U;
    EXPECT_TRUE(mc2_boundary::NotifyBudget(4U, 4U, 4U, required));
    EXPECT_EQ(required, 16U);
    EXPECT_TRUE(mc2_boundary::NotifyBudget(4U, 4U, 20U, required));
    EXPECT_EQ(required, 64U);
    EXPECT_FALSE(mc2_boundary::NotifyBudget(4U, 4U, 21U, required));
    EXPECT_FALSE(mc2_boundary::NotifyBudget(4U, UINT32_MAX, UINT32_MAX, required));
    std::set<uint32_t> slots;
    for (uint32_t turn = 0; turn < 4U; ++turn) {
        for (uint32_t slave = 0; slave < 3U; ++slave) {
            EXPECT_TRUE(slots.insert(4U + turn * 3U + slave).second);
        }
    }
    res.threads[3] = res.threads[1];
    EXPECT_FALSE(PrepareOpenBoundaryLayout(res, policy));
    EXPECT_STREQ(policy.fallbackReason, "thread_layout");
    res.threads[3] = 4U;
    res.channels[0].back().portGroupSize = 9U;
    EXPECT_FALSE(PrepareOpenBoundaryLayout(res, policy));
    EXPECT_STREQ(policy.fallbackReason, "channel_protocol_or_partition");
    res.channels[0].pop_back();
    EXPECT_FALSE(PrepareOpenBoundaryLayout(res, policy));
    EXPECT_STREQ(policy.fallbackReason, "uneven_channel_layout");
}

TEST(Mc2Boundary, CommunicationEntryProtectsSlowPostCopyScratch)
{
    allocatedThreads = 4U;
    activeChannels = 2U;
    for (const bool retainEntry : {false, true}) {
        queues.clear();
        uint32_t scratch = 7U;
        uint32_t output = 0U;
        const uint32_t expected = 7U;
        const uint32_t nextInput = 9U;
        // Previous full exit is on main, not on the producer T1.
        queues[1].push_back({Kind::WAIT, 1U, 6U});
        if (retainEntry) {
            queues[1].push_back({Kind::RECORD, 2U, 0U});
            queues[2].push_back({Kind::WAIT, 2U, 0U});
        }
        queues[1].push_back({Kind::CHECK, 0U, 0U, &output, &expected, sizeof(output)});
        queues[2].push_back({Kind::COPY, 0U, 0U, &scratch, &nextInput, sizeof(scratch)});
        queues[4].push_back({Kind::COPY, 0U, 0U, &output, &scratch, sizeof(output)});
        queues[4].push_back({Kind::RECORD, 1U, 6U});
        if (retainEntry) {
            EXPECT_TRUE(Execute(false));
            EXPECT_EQ(output, expected);
        } else {
            EXPECT_NONFATAL_FAILURE(EXPECT_FALSE(Execute(false)), "Per-turn completion preceded output readiness");
        }
    }
}
