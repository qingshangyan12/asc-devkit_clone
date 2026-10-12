/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_fixture.h"

#include <cstdlib>
#include <thread>

#include "acl/acl_rt.h"
#include "aicpu_dag_checker.h"
#include "checker_def.h"
#include "hccl_alloc_ctx_res.h"
#include "hccl_sim_pub.h"
#include "op_common.h"
#include "securec.h"
#include "sim_communicator.h"
#include "sim_npu.h"
#include "sim_task_queue.h"
#include "sim_world.h"
#include "task_graph_executor.h"

namespace HcclSim {
namespace AicpuSt {

uint32_t AicpuStFixture::CountRanks(const TopoMeta& topoMeta)
{
    uint32_t total = 0;
    for (const auto& superPod : topoMeta) {
        for (const auto& server : superPod) {
            total += static_cast<uint32_t>(server.size());
        }
    }
    return total;
}

Mc2CcTilingInner AicpuStFixture::BuildTiling(HcclCMDType opType, HcclDataType dataType, const std::string& algConfig)
{
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(opType);
    tiling.commEngine = static_cast<uint8_t>(mc2_ops_hccl::OpExecuteConfig::AICPU_TS); // 与 CCU ST 的唯一差异
    tiling.srcDataType = static_cast<uint8_t>(dataType);
    tiling.dstDataType = static_cast<uint8_t>(dataType);
    (void)strcpy_s(tiling.groupName, sizeof(tiling.groupName), "test_group");
    (void)strcpy_s(tiling.algConfig, sizeof(tiling.algConfig), algConfig.empty() ? "default" : algConfig.c_str());
    return tiling;
}

// 单 rank 全链路：内存分配 → OpParam 准备 → selector → 资源 → HcclExecOp（内部 launch+Orchestrate）
// RunRankInner 为实际执行体；RunRank 为异常保护包装（见下）——rank 线程内未捕获异常会
// std::terminate 使整个测试进程崩溃、后续用例全丢，违背框架"优雅报告失败"的设计目标，
// 故所有 THROW 点（如 SimNpu::AllocMemory 对超大 len 的 InvalidParamsException）必须在此兜住。
void AicpuStFixture::RunRankInner(
    AicpuStFixture* fixture, const AicpuStScenario& scenario, uint32_t rank, uint32_t rankSize,
    const RankMemLayout& layout, RankResult& result)
{
    if (aclrtSetDevice(static_cast<int32_t>(rank)) != ACL_SUCCESS) {
        result.error = "aclrtSetDevice failed";
        return;
    }
    aclrtStream stream = nullptr;
    if (aclrtCreateStream(&stream) != ACL_SUCCESS) {
        result.error = "aclrtCreateStream failed";
        return;
    }

    RankMem mem;
    mem.inputBytes = layout.inputBytes;
    mem.outputBytes = layout.outputBytes;
    // 零长度场景仍分配 1 字节，保证指针有效（模板 sliceSize==0 时提前返回，不会触碰数据）
    const uint64_t inAlloc = (layout.inputBytes == 0) ? 1 : layout.inputBytes;
    const uint64_t outAlloc = (layout.outputBytes == 0) ? 1 : layout.outputBytes;
    if (aclrtMalloc(&mem.inputPtr, inAlloc, static_cast<aclrtMemMallocPolicy>(BUFFER_INPUT_MARK)) != ACL_SUCCESS) {
        result.error = "aclrtMalloc input failed";
        return;
    }
    if (aclrtMalloc(&mem.outputPtr, outAlloc, static_cast<aclrtMemMallocPolicy>(BUFFER_OUTPUT_MARK)) != ACL_SUCCESS) {
        result.error = "aclrtMalloc output failed";
        return;
    }

    // 填输入数据（直接写 SimNpu 后备内存）
    if (layout.inputBytes > 0) {
        SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(rank);
        uint8_t* base = npu.GetBufferBase(BufferType::INPUT, layout.inputBytes);
        for (uint64_t i = 0; i < layout.inputBytes; ++i) {
            base[i] = AicpuStFixture::MakeInputByte(rank, i);
        }
    }

    HcclComm comm = nullptr;
    if (Sim_HcclCommInitClusterInfo(scenario.topoMeta, rank, &comm) != HCCL_SUCCESS) {
        result.error = "Sim_HcclCommInitClusterInfo failed";
        return;
    }

    const Mc2CcTilingInner tiling = AicpuStFixture::BuildTiling(scenario.opType, scenario.dataType, scenario.algConfig);
    OpParam opParam{};
    if (InitOpParamByTiling(comm, stream, "aicpu_st", &tiling, opParam) != HCCL_SUCCESS) {
        result.error = "InitOpParamByTiling failed";
        (void)HcclCommDestroy(comm);
        return;
    }

    // PrepareOpParam：见 aicpu_fixture.h 注释（count/ptr 必须在 selector 前补齐）
    fixture->PrepareOpParam(opParam, rankSize, scenario, mem);

    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    std::string algName;
    bool forcedAlgAccepted = false;
    if (TryForcedAlgAndPrepareEngine(comm, &tiling, opParam, algName, topoInfo, forcedAlgAccepted) != HCCL_SUCCESS) {
        result.error = "TryForcedAlgAndPrepareEngine failed";
        (void)HcclCommDestroy(comm);
        return;
    }
    if (!forcedAlgAccepted) {
        if (SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo) != HCCL_SUCCESS) {
            result.error = "SelectAlgAndPrepareEngine failed";
            (void)HcclCommDestroy(comm);
            return;
        }
    }
    if (rank == 0 && !scenario.expectedAlgName.empty() && algName != scenario.expectedAlgName) {
        result.error = "algName mismatch: expected[" + scenario.expectedAlgName + "] got[" + algName + "]";
        (void)HcclCommDestroy(comm);
        return;
    }
    result.algName = algName;

    if (FillOpParamAlgName(opParam, algName) != HCCL_SUCCESS) {
        result.error = "FillOpParamAlgName failed";
        (void)HcclCommDestroy(comm);
        return;
    }

    bool skipGetRes = false;
    if (HandleSingleRankAndCommMode(comm, opParam, skipGetRes) != HCCL_SUCCESS || skipGetRes) {
        result.error = "HandleSingleRankAndCommMode failed or skipped";
        (void)HcclCommDestroy(comm);
        return;
    }

    void* resCtxOut = nullptr;
    if (GetOpParamResCtx(comm, algName, opParam, topoInfo.get(), &resCtxOut) != HCCL_SUCCESS) {
        result.error = "GetOpParamResCtx failed";
        (void)HcclCommDestroy(comm);
        return;
    }

    ResPackGraphMode resPack{};
    if (HcclExecOp(comm, opParam, topoInfo, algName, resPack) != HCCL_SUCCESS) {
        result.error = "HcclExecOp failed";
        (void)HcclCommDestroy(comm);
        return;
    }

    (void)HcclCommDestroy(comm);
    result.ok = true;
}

// 异常保护包装：任何 THROW/异常转为 result.error 优雅报告。
// comm 泄漏兜底：若异常发生在 comm 创建之后的未包裹 THROW 点，comm 由 SimWorld::Deinit 统一回收。
void AicpuStFixture::RunRank(
    AicpuStFixture* fixture, const AicpuStScenario& scenario, uint32_t rank, uint32_t rankSize,
    const RankMemLayout& layout, RankResult& result)
{
    try {
        RunRankInner(fixture, scenario, rank, rankSize, layout, result);
    } catch (const std::exception& e) {
        result.error = std::string("exception: ") + e.what();
        result.ok = false;
    } catch (...) {
        result.error = "unknown exception in rank thread";
        result.ok = false;
    }
}

void AicpuStFixture::VerifyScenario(const AicpuStScenario& scenario)
{
    const uint32_t rankSize = CountRanks(scenario.topoMeta);
    ASSERT_GE(rankSize, 1U);
    // 守卫：dataType 越界（≥ RESERVED）会使后续 DATATYPE_SIZE_TABLE 索引越界读，先检查枚举界
    ASSERT_LT(static_cast<uint32_t>(scenario.dataType), HCCL_DATA_TYPE_RESERVED)
        << "invalid dataType in scenario, value=" << static_cast<uint32_t>(scenario.dataType);

    SimTaskQueue::Global()->Clear();

    // RAII：保证断言失败也能清理
    struct ScopeGuard {
        ~ScopeGuard()
        {
            SimWorld::Global()->Deinit();
            unsetenv("HCCL_OP_EXPANSION_MODE");
            unsetenv("HCCL_ENABLE_OPEN_AICPU");
        }
    } guard;

    setenv("HCCL_OP_EXPANSION_MODE", "AI_CPU", 1);
    setenv("HCCL_ENABLE_OPEN_AICPU", "1", 1);
    SimWorld::Global()->Init(scenario.topoMeta, scenario.devType);

    const RankMemLayout layout = BuildMemLayout(rankSize, scenario);

    std::vector<RankResult> results(rankSize);
    std::vector<std::thread> threads;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        threads.emplace_back([this, &scenario, rank, rankSize, &layout, &results]() {
            RunRank(this, scenario, rank, rankSize, layout, results[rank]);
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        ASSERT_TRUE(results[rank].ok) << "rank " << rank << " failed: " << results[rank].error;
    }

    // 静态检查：任务图非空（零长度场景除外；per-rank——每 rank 名下至少一个队列有任务）
    AllRankTaskQueues& queues = SimTaskQueue::Global()->GetAllRankTaskQueues();
    if (layout.inputBytes > 0) {
        for (uint32_t rank = 0; rank < rankSize; ++rank) {
            const NpuPos pos = SimWorld::Global()->GetNpuPosByRankId(rank);
            auto podIt = queues.find(pos.superpodId);
            ASSERT_TRUE(podIt != queues.end()) << "rank " << rank << " has no task queue";
            auto serIt = podIt->second.find(pos.serverId);
            ASSERT_TRUE(serIt != podIt->second.end()) << "rank " << rank << " has no task queue";
            auto phyIt = serIt->second.find(pos.phyId);
            ASSERT_TRUE(phyIt != serIt->second.end()) << "rank " << rank << " has no task queue";
            size_t rankTasks = 0;
            for (const auto& stream : phyIt->second) {
                rankTasks += stream.size();
            }
            EXPECT_GT(rankTasks, 0U) << "rank " << rank << " enqueued no tasks";
        }
    }

    // golden 期望
    std::vector<std::vector<uint8_t>> expected(rankSize);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        expected[rank] = BuildExpectedOutput(rankSize, scenario, rank);
    }

    // 任务图真执行 + 死锁定位
    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(queues, report), ExecResult::SUCCESS) << report;

    // DAG 内存冲突检测（WAW/WAR/RAW 无同步并发）；封顶输出避免刷屏
    const std::vector<DagConflict> conflicts = AicpuDagChecker().Check(executor.ExecutionLog());
    constexpr size_t maxReported = 5;
    for (size_t i = 0; i < conflicts.size(); ++i) {
        if (i < maxReported) {
            ADD_FAILURE() << "DAG conflict [" << conflicts[i].kind << "]: A(" << conflicts[i].detailA << ") vs B("
                          << conflicts[i].detailB << ")";
        }
    }
    if (conflicts.size() > maxReported) {
        ADD_FAILURE() << "DAG conflicts total " << conflicts.size() << ", first " << maxReported << " reported";
    }

    // golden 空守卫：非零场景意外为空则 FAIL（防 adapter bug 假绿；可经 ExpectOutputCheck 豁免）
    if (layout.outputBytes > 0 && ExpectOutputCheck()) {
        for (uint32_t rank = 0; rank < rankSize; ++rank) {
            if (expected[rank].empty()) {
                ADD_FAILURE() << "rank " << rank
                              << " golden unexpectedly empty for non-zero scenario (adapter bug? override "
                                 "ExpectOutputCheck to skip by design)";
                break; // 只报首个，避免 N rank 刷屏
            }
        }
    }

    // 逐字节比对（期望为空=零长度场景或已豁免场景，跳过）
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        if (expected[rank].empty()) {
            continue;
        }
        SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(rank);
        const uint8_t* out = npu.GetBufferBase(BufferType::OUTPUT, layout.outputBytes);
        ASSERT_EQ(layout.outputBytes, expected[rank].size()) << "rank " << rank << " expected output size mismatch";
        for (uint64_t i = 0; i < layout.outputBytes; ++i) {
            ASSERT_EQ(out[i], expected[rank][i]) << "rank " << rank << " output mismatch at byte " << i;
        }
    }
}

} // namespace AicpuSt
} // namespace HcclSim
