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
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "alg_env_config.h"
#include "all_gather_auto_selector.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "executor_v2_base.h"
#include "gtest/gtest.h"
#include "hccl.h"
#include "op_common.h"
#include "sim_task.h"
#include "sim_task_queue.h"
#include "sim_world.h"

using namespace mc2_ops_hccl;

namespace checker {
namespace {
constexpr char MULTI_JETTY_ALGORITHM_NAME[] = "InsAllGatherParallelMesh1DNHRMultiJetty";
constexpr u32 MULTI_JETTY_RANK_SIZE = 16U;
constexpr u32 MULTI_JETTY_MESH_SIZE = 8U;
constexpr u32 MULTI_JETTY_CLOS_SIZE = 16U;
constexpr u32 MULTI_JETTY_CHANNEL_NUM = 4U;
constexpr u32 MULTI_JETTY_EXPECTED_SLAVE_THREAD_NUM = 15U;
constexpr u32 MULTI_JETTY_EXPECTED_THREAD_NUM = MULTI_JETTY_EXPECTED_SLAVE_THREAD_NUM + 1U;
constexpr u32 MULTI_JETTY_EXPECTED_SCRATCH_MULTIPLE = 16U;
constexpr u32 MULTI_JETTY_AXIS_NUM = 2U;
constexpr u64 MULTI_JETTY_DATA_COUNT = 4ULL * 1024ULL * 1024ULL + 1U;
constexpr u64 MULTI_JETTY_STRIDE_COUNT = MULTI_JETTY_DATA_COUNT + 17U;

using TaskPtr = std::shared_ptr<HcclSim::TaskStub>;
using OutputRange = std::pair<u64, u64>;

struct MultiJettyRankExecution {
    HcclComm comm = nullptr;
    OpParam param{};
    TopoInfoWithNetLayerDetails topo{};
    AlgHierarchyInfoForAllLevel hierarchy{};
    AlgResourceRequest request{};
    std::unique_ptr<InsCollAlgBase> executor;
    std::unique_ptr<AlgResourceCtxSerializable> resCtx;
};

OpParam MakeAicpuParam(CommEngine engine = COMM_ENGINE_AICPU_TS)
{
    OpParam param{};
    param.opType = HCCL_CMD_ALLGATHER;
    param.engine = engine;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    param.DataDes.dataType = HCCL_DATA_TYPE_FP32;
    param.DataDes.count = 1U;
    return param;
}

TopoInfoWithNetLayerDetails MakeConcurrentTopo(u32 rankSize = 4U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    topo.netLayerDetails.localNetInsSizeOfLayer = {rankSize};
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {rankSize};
    return topo;
}

TopoInfoWithNetLayerDetails MakeMultiJettyTopo(u32 userRank = 0U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = userRank;
    topo.userRankSize = MULTI_JETTY_RANK_SIZE;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    topo.netLayerDetails.localNetInsSizeOfLayer = {MULTI_JETTY_MESH_SIZE};
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {MULTI_JETTY_MESH_SIZE};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {MULTI_JETTY_CLOS_SIZE};
    return topo;
}

AlgHierarchyInfoForAllLevel MakeMultiJettyHierarchy(u32 userRank)
{
    const u32 meshBase = userRank / MULTI_JETTY_MESH_SIZE * MULTI_JETTY_MESH_SIZE;
    std::vector<u32> meshRanks;
    std::vector<u32> closRanks;
    for (u32 idx = 0U; idx < MULTI_JETTY_MESH_SIZE; ++idx) {
        meshRanks.push_back(meshBase + idx);
    }
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        closRanks.push_back(rank);
    }
    AlgHierarchyInfoForAllLevel hierarchy;
    hierarchy.infos = {{meshRanks, closRanks}};
    return hierarchy;
}

void AddOutputRange(std::map<u32, std::vector<OutputRange>>& outputRanges, u32 rank, u64 offset, u64 size)
{
    if (size != 0U) {
        outputRanges[rank].emplace_back(offset, offset + size);
    }
}

void CollectOutputRanges(
    const HcclSim::SingleTaskQueue& taskQueue, u32 rank, std::map<u32, std::vector<OutputRange>>& outputRanges)
{
    for (const auto& threadTasks : taskQueue) {
        for (const TaskPtr& task : threadTasks) {
            if (auto* localCopy = dynamic_cast<HcclSim::TaskStubLocalCopy*>(task.get())) {
                const auto& dst = localCopy->GetDstSlice();
                if (dst.GetType() == HcclSim::BufferType::OUTPUT) {
                    AddOutputRange(outputRanges, rank, dst.GetOffset(), dst.GetSize());
                }
            } else if (auto* read = dynamic_cast<HcclSim::TaskStubRead*>(task.get())) {
                const auto& dst = read->GetLocalSlice();
                if (dst.GetType() == HcclSim::BufferType::OUTPUT) {
                    AddOutputRange(outputRanges, rank, dst.GetOffset(), dst.GetSize());
                }
            } else if (auto* write = dynamic_cast<HcclSim::TaskStubWrite*>(task.get())) {
                const auto& dst = write->GetRemoteSlice();
                if (dst.GetType() == HcclSim::BufferType::OUTPUT) {
                    AddOutputRange(outputRanges, write->GetRemoteRank(), dst.GetOffset(), dst.GetSize());
                }
            }
        }
    }
}

bool CoversRange(const std::vector<OutputRange>& ranges, u64 start, u64 end)
{
    std::vector<OutputRange> sortedRanges = ranges;
    std::sort(sortedRanges.begin(), sortedRanges.end());
    u64 coveredEnd = start;
    for (const auto& range : sortedRanges) {
        if (range.second <= coveredEnd || range.first > coveredEnd) {
            continue;
        }
        coveredEnd = range.second;
        if (coveredEnd >= end) {
            return true;
        }
    }
    return coveredEnd >= end;
}

bool IsWithinOutputSlot(const OutputRange& range, u64 strideBytes, u64 dataBytes)
{
    if (range.first >= range.second) {
        return false;
    }
    const u64 slot = range.first / strideBytes;
    return slot < MULTI_JETTY_RANK_SIZE && range.second <= slot * strideBytes + dataBytes;
}

void DestroyCommunicators(std::vector<MultiJettyRankExecution>& executions)
{
    for (auto& execution : executions) {
        if (execution.comm != nullptr) {
            HcclCommDestroy(execution.comm);
            execution.comm = nullptr;
        }
    }
}

class MultiJettyExecutionGuard {
public:
    explicit MultiJettyExecutionGuard(std::vector<MultiJettyRankExecution>& executions) : executions_(executions) {}
    ~MultiJettyExecutionGuard()
    {
        DestroyCommunicators(executions_);
        HcclSim::SimWorld::Global()->Deinit();
    }

private:
    std::vector<MultiJettyRankExecution>& executions_;
};

} // namespace

class ST_ALL_GATHER_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_GATHER_AICPU_TEST, aicpu_all_gather_does_not_use_cann_bridge)
{
    for (CommEngine engine : {COMM_ENGINE_AICPU_TS, COMM_ENGINE_AICPU}) {
        EXPECT_FALSE(UseCannBridge(MakeAicpuParam(engine)));
    }
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(COMM_ENGINE_CPU)));
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, local_registry_contains_all_gather_algorithms)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLGATHER, "InsAllGatherMesh1D"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLGATHER, "AicpuAllGatherSoleNHR"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLGATHER, "AicpuAllGatherConcurMeshNHR"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLGATHER, "InsAllGatherParallelMesh1DNHRMultiJetty"), nullptr);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, aicpu_selector_obeys_count_and_rank_boundaries)
{
    constexpr u64 countAtLimit = SMALL_COUNT_512KB / sizeof(float);
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeConcurrentTopo();
    OpParam param = MakeAicpuParam();
    std::string algName;

    param.DataDes.count = countAtLimit;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "InsAllGatherMesh1D");

    param = MakeAicpuParam();
    param.DataDes.count = countAtLimit + 1U;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllGatherConcurMeshNHR");

    topo = MakeConcurrentTopo(5U);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllGatherSoleNHR");
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, concurrent_executor_calculates_resources_with_host_all_gather_param)
{
    constexpr u32 rankSize = 4U;
    OpParam param = MakeAicpuParam();
    param.DataDes.count = 513U;
    param.algName[0] = '\0';
    ASSERT_GT(std::snprintf(param.algName, sizeof(param.algName), "%s", "AicpuAllGatherConcurMeshNHR"), 0);

    TopoInfoWithNetLayerDetails topo = MakeConcurrentTopo(rankSize);
    AlgHierarchyInfoForAllLevel hierarchy;
    hierarchy.infos = {{{0U, 1U, 2U, 3U}, {0U, 1U, 2U, 3U}}};
    std::unique_ptr<InsCollAlgBase> executor =
        CollAlgExecRegistryV2::Instance().GetAlgExec(HCCL_CMD_ALLGATHER, param.algName);
    ASSERT_NE(executor, nullptr);

    AlgResourceRequest request;
    ASSERT_EQ(executor->CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_SUCCESS);
    EXPECT_EQ(request.slaveThreadNum, 4U);
    EXPECT_EQ(request.notifyNumOnMainThread, 3U);
    EXPECT_EQ(request.notifyNumPerThread, std::vector<u32>({1U, 1U, 2U, 2U}));
    ASSERT_EQ(request.channels.size(), 1U);
    EXPECT_EQ(request.channels[0].size(), 6U);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, multijetty_selector_uses_algorithm_for_uneven_mesh_clos_topology)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMultiJettyTopo();
    OpParam param = MakeAicpuParam();
    param.DataDes.count = SMALL_COUNT_512KB / sizeof(float) + 1U;
    std::string algName;

    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "InsAllGatherParallelMesh1DNHRMultiJetty");
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, multijetty_executor_calculates_multichannel_resources)
{
    OpParam param = MakeAicpuParam();
    ASSERT_GT(std::snprintf(param.algName, sizeof(param.algName), "%s", MULTI_JETTY_ALGORITHM_NAME), 0);

    const TopoInfoWithNetLayerDetails topo = MakeMultiJettyTopo();
    const AlgHierarchyInfoForAllLevel hierarchy = MakeMultiJettyHierarchy(0U);
    std::unique_ptr<InsCollAlgBase> executor =
        CollAlgExecRegistryV2::Instance().GetAlgExec(HCCL_CMD_ALLGATHER, param.algName);
    ASSERT_NE(executor, nullptr);

    AlgResourceRequest request;
    ASSERT_EQ(executor->CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_SUCCESS);
    EXPECT_EQ(request.slaveThreadNum, MULTI_JETTY_EXPECTED_SLAVE_THREAD_NUM);
    EXPECT_EQ(request.notifyNumOnMainThread, 2U);
    EXPECT_EQ(
        request.notifyNumPerThread, std::vector<u32>({7U, 1U, 1U, 1U, 1U, 1U, 1U, 8U, 2U, 2U, 2U, 2U, 2U, 2U, 2U}));
    ASSERT_EQ(request.channels.size(), 2U);
    EXPECT_EQ(request.channels[0].size(), MULTI_JETTY_MESH_SIZE - 1U);
    EXPECT_EQ(request.channels[1].size(), MULTI_JETTY_CHANNEL_NUM);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, multijetty_executor_orchestrates_multiround_strided_all_gather)
{
    const TopoMeta topoMeta{{{0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U}}};
    std::vector<MultiJettyRankExecution> executions(MULTI_JETTY_RANK_SIZE);
    MultiJettyExecutionGuard guard(executions);
    HcclSim::SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);

    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    bool setupSucceeded = true;
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        auto& execution = executions[rank];
        execution.param = MakeAicpuParam();
        execution.param.userRank = rank;
        execution.param.DataDes.count = MULTI_JETTY_DATA_COUNT;
        execution.param.DataDes.strideCount = MULTI_JETTY_STRIDE_COUNT;
        execution.param.inputSize = MULTI_JETTY_DATA_COUNT * sizeof(float);
        execution.param.outputSize = MULTI_JETTY_RANK_SIZE * MULTI_JETTY_STRIDE_COUNT * sizeof(float);
        if (std::snprintf(execution.param.algName, sizeof(execution.param.algName), "%s", MULTI_JETTY_ALGORITHM_NAME) <=
                0 ||
            HcclCommInitClusterInfo("", rank, &execution.comm) != HCCL_SUCCESS) {
            setupSucceeded = false;
            break;
        }
        execution.topo = MakeMultiJettyTopo(rank);
        execution.hierarchy = MakeMultiJettyHierarchy(rank);
        execution.executor = registry.GetAlgExec(HCCL_CMD_ALLGATHER, execution.param.algName);
        if (execution.executor == nullptr || execution.executor->CalcRes(
                                                 execution.comm, execution.param, &execution.topo, execution.hierarchy,
                                                 execution.request) != HCCL_SUCCESS) {
            setupSucceeded = false;
            break;
        }
    }
    ASSERT_TRUE(setupSucceeded);

    std::vector<HcclResult> threadResults(MULTI_JETTY_RANK_SIZE, HCCL_E_INTERNAL);
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        auto& execution = executions[rank];
        execution.resCtx = std::make_unique<AlgResourceCtxSerializable>();
        threadResults[rank] = HcclGetThread(execution.comm, execution.param, execution.request, execution.resCtx);
    }

    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        ASSERT_EQ(threadResults[rank], HCCL_SUCCESS);
    }

    std::vector<HcclResult> channelResults(MULTI_JETTY_RANK_SIZE, HCCL_E_INTERNAL);
    std::vector<std::thread> channelThreads;
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        channelThreads.emplace_back([&, rank]() {
            auto& execution = executions[rank];
            channelResults[rank] = HcclGetChannel(execution.comm, execution.param, execution.request, execution.resCtx);
        });
    }
    for (auto& thread : channelThreads) {
        thread.join();
    }
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        ASSERT_EQ(channelResults[rank], HCCL_SUCCESS);
        ASSERT_NE(executions[rank].resCtx, nullptr);
        EXPECT_EQ(executions[rank].resCtx->threads.size(), MULTI_JETTY_EXPECTED_THREAD_NUM);
        ASSERT_EQ(executions[rank].resCtx->channels.size(), MULTI_JETTY_AXIS_NUM);
        EXPECT_EQ(executions[rank].resCtx->channels[0].size(), MULTI_JETTY_MESH_SIZE - 1U);
        EXPECT_EQ(executions[rank].resCtx->channels[1].size(), MULTI_JETTY_CHANNEL_NUM);
        executions[rank].resCtx->algHierarchyInfo = executions[rank].hierarchy;
        executions[rank].resCtx->topoInfo = executions[rank].topo;
        HcclSim::SimNpu& npu = HcclSim::SimWorld::Global()->GetSimNpuByRankId(rank);
        executions[rank].param.inputPtr =
            reinterpret_cast<void*>(npu.AllocMemory(HcclSim::BufferType::INPUT, executions[rank].param.inputSize));
        executions[rank].param.outputPtr =
            reinterpret_cast<void*>(npu.AllocMemory(HcclSim::BufferType::OUTPUT, executions[rank].param.outputSize));
    }

    const u64 singleLoopCapacity =
        executions[0].resCtx->cclMem.size / MULTI_JETTY_EXPECTED_SCRATCH_MULTIPLE / sizeof(float);
    EXPECT_GT(MULTI_JETTY_DATA_COUNT, singleLoopCapacity);
    EXPECT_EQ(MULTI_JETTY_DATA_COUNT % MULTI_JETTY_AXIS_NUM, 1U);
    EXPECT_GT(MULTI_JETTY_STRIDE_COUNT, MULTI_JETTY_DATA_COUNT);

    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        EXPECT_EQ(
            executions[rank].executor->Orchestrate(executions[rank].param, *executions[rank].resCtx), HCCL_SUCCESS);
    }

    auto& taskQueues = HcclSim::SimTaskQueue::Global()->GetAllRankTaskQueues();
    ASSERT_FALSE(taskQueues.empty());
    const auto& rankQueues = taskQueues.at(0U).at(0U);
    std::map<u32, std::vector<OutputRange>> outputRanges;
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        ASSERT_TRUE(rankQueues.count(rank) != 0U);
        CollectOutputRanges(rankQueues.at(rank), rank, outputRanges);
    }

    const u64 dataBytes = MULTI_JETTY_DATA_COUNT * sizeof(float);
    const u64 strideBytes = MULTI_JETTY_STRIDE_COUNT * sizeof(float);
    for (u32 rank = 0U; rank < MULTI_JETTY_RANK_SIZE; ++rank) {
        ASSERT_TRUE(outputRanges.count(rank) != 0U);
        for (const OutputRange& range : outputRanges.at(rank)) {
            EXPECT_TRUE(IsWithinOutputSlot(range, strideBytes, dataBytes)) << "rank=" << rank;
        }
        for (u32 sourceRank = 0U; sourceRank < MULTI_JETTY_RANK_SIZE; ++sourceRank) {
            const u64 outputOffset = sourceRank * strideBytes;
            EXPECT_TRUE(CoversRange(outputRanges.at(rank), outputOffset, outputOffset + dataBytes))
                << "rank=" << rank << " sourceRank=" << sourceRank;
        }
    }
}

} // namespace checker
