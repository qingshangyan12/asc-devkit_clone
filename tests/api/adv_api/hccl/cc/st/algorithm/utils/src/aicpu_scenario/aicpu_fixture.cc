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
#include <fstream>
#include <thread>

#include "acl/acl_rt.h"
#include "aicpu_dag_checker.h"
#include "aicpu_kernel_runner.h"
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
namespace {
// 执行日志 dump（AICPU_ST_DUMP_EXECLOG=<目录> 触发）：每用例一份 ExecEvent 序列。
// 用途：驱动器收编/框架重构时的行为零漂移对照基线（同场景改前改后逐行 diff）
void DumpExecutionLog(const std::vector<ExecEvent>& log)
{
    const char* dir = std::getenv("AICPU_ST_DUMP_EXECLOG");
    if (dir == nullptr || *dir == '\0') {
        return;
    }
    const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
    const std::string name = info == nullptr ? "unknown" : std::string(info->test_suite_name()) + "." + info->name();
    std::ofstream out(std::string(dir) + "/" + name + ".log");
    if (!out.is_open()) {
        return;
    }
    for (const auto& e : log) {
        out << e.seq << " rank=" << e.rank << " stream=" << e.streamId << " task=" << e.taskIndex
            << " type=" << e.type.Describe() << " notify=" << e.notifyId << " sat=" << e.satisfiedBySeq;
        for (const auto& a : e.accesses) {
            out << " acc[" << a.rank << "," << static_cast<int>(a.bufferType) << "," << a.offset << "," << a.size << ","
                << (a.isWrite ? "W" : "R") << "]";
        }
        out << "\n";
    }
}
} // namespace

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

// ═══════════════════════════════════════════════════════════════════════════════════
// 场景能力守卫（直算路径显式拒绝）
//
// 背景：产品 LocalReduce wrapper（alg_data_trans_wrapper.cc:738-744）对
//   "64bit 类型（INT64/UINT64/FP64，任何 reduceOp）或 PROD（任何类型）"
// 的组合不走 Hcomm 原语（任务路径），而转发 AicpuReduce——host/AICPU 上直接解引用
// 数据指针逐元素计算的旁路。真机上 AICPU 与 HBM 统一编址、直访合法；ST 的 buffer
// 是假设备地址（编号，需 GetSlice 解码才得到后备内存指针），直算跳过了解码层，
// 直接解引用必 SIGSEGV。直算没有函数内拦截点（解引用是 load/store 指令，无桩可用），
// 且该路径是产品对"底层归约引擎未实现组合"（PROD/64bit 罕见于梯度聚合负载）的
// CPU 兜底设计，预计长期存在。
//
// 不支持域的两个子情形（原因不同，报错文案区分）：
//  1) 64bit 类型：产品支持该组合，但直算越出 ST 假地址仿真边界（SIGSEGV）——需真机验证；
//  2) PROD + 常规类型：产品 selector 与执行能力错配——selector 照选算法（如
//     InsReduceScatterMesh1D），但 AicpuReduce 仅支持 INT64/UINT64/FP64，
//     执行报 DataType not support（真机同样报错，属产品缺陷，需上游修）。
//
// 守卫在 VerifyScenario 入口（rank 线程启动前）显式拒绝并给出完整原因——优于
// 跑到一半 SIGSEGV/死锁的事后排查。判定条件与产品转发条件严格对齐；
// 非 reduce 系 opType（如 AllGather：无归约，64bit 拷贝走任务路径不受限）不拦。
// 详见 2026-10-10-aicpu-st-design.md §18.2/§18.3。
// ═══════════════════════════════════════════════════════════════════════════════════
bool AicpuStFixture::CheckScenarioSupported(const AicpuStScenario& scenario, bool isReduceLikeOp, std::string& reason)
{
    const bool is64Bit = scenario.dataType == HcclDataType::HCCL_DATA_TYPE_INT64 ||
                         scenario.dataType == HcclDataType::HCCL_DATA_TYPE_UINT64 ||
                         scenario.dataType == HcclDataType::HCCL_DATA_TYPE_FP64;
    const bool isProd = scenario.reduceType == HcclReduceOp::HCCL_REDUCE_PROD;
    if (!isReduceLikeOp || (!is64Bit && !isProd)) {
        return true;
    }
    reason = "unsupported scenario (product direct-compute path, see 2026-10-10-aicpu-st-design.md §18.2): ";
    if (is64Bit) {
        reason += "64bit dataType (" + std::to_string(static_cast<uint32_t>(scenario.dataType)) +
                  ") reduce is routed by product LocalReduce wrapper to AicpuReduce, which computes by "
                  "dereferencing data pointers directly on the processor (valid on real device via unified "
                  "HBM addressing, but ST buffers are encoded pseudo device addresses that only GetSlice "
                  "can decode) -- direct dereference would SIGSEGV. Requires real-device verification.";
    } else {
        reason += "reduceOp PROD with non-64bit dataType (" + std::to_string(static_cast<uint32_t>(scenario.dataType)) +
                  ") is a product-level selector/executor mismatch: selector still picks the algorithm, but "
                  "AicpuReduce only supports INT64/UINT64/FP64 and fails at runtime with 'DataType not "
                  "support' (fails on real device too; needs upstream fix). ST rejects it explicitly.";
    }
    return false;
}

// 单 rank 全链路：内存分配 → OpParam 准备 → selector → 资源 → AicpuStKernelRunner（kernel run 仿真）
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
        result.error = "HandleSingleRankAndCommMode failed or skipped (single-rank scenario not supported in phase 1)";
        (void)HcclCommDestroy(comm);
        return;
    }

    void* resCtxOut = nullptr;
    if (GetOpParamResCtx(comm, algName, opParam, topoInfo.get(), &resCtxOut) != HCCL_SUCCESS) {
        result.error = "GetOpParamResCtx failed";
        (void)HcclCommDestroy(comm);
        return;
    }

    if (AicpuStKernelRunner::Run(comm, opParam, topoInfo, algName) != HCCL_SUCCESS) {
        result.error = "AicpuStKernelRunner::Run failed";
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

    // 能力守卫：直算路径（64bit/PROD 归约）显式拒绝并说明原因（优于跑到一半 SIGSEGV/死锁）。
    // ADD_FAILURE（非致命）+ return：用例变红且消息完整，后续装配不再执行——
    // 与 FAIL 等效且可被 EXPECT_NONFATAL_FAILURE 捕获测试（同 golden 空守卫先例）。
    // isReduceLikeOp 来自原语级虚函数声明（IsReduceLikeOp），非框架层清单——防原语演化漂移
    std::string unsupportedReason;
    if (!CheckScenarioSupported(scenario, IsReduceLikeOp(), unsupportedReason)) {
        ADD_FAILURE() << unsupportedReason;
        return;
    }

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

    // 集合通信各 rank 必须选同一算法——divergence 即算法缺陷
    for (uint32_t rank = 1; rank < rankSize; ++rank) {
        EXPECT_EQ(results[rank].algName, results[0].algName) << "selector divergence across ranks";
    }
    const std::string& actualAlgName = results[0].algName;

    // golden 构造 + 任务图真执行 + 死锁定位 + DAG 冲突检测 —— 主线程异常保护：
    // 本段内的 THROW（golden 的 op 前置校验 / Execute 的 slice mismatch、非法 op、未实现
    // 任务类型等，多为产品 bug 或场景配置错误的表现——执行器测试 :462-481 已钉死 THROW
    // 契约）若不捕获，Linux gtest 默认不捕异常 → std::terminate 崩掉整个测试进程，后续
    // 用例全丢。与 RunRank 的编排期保护对称（:169-181 rank 线程侧），兑现框架
    // "优雅报告失败"目标：异常转为用例失败 + 异常消息，而非进程崩溃。
    TaskGraphExecutor executor;
    std::string report;
    std::vector<DagConflict> conflicts;
    try {
        // golden 期望：按实际选中算法键控（expectedAlgName 留空模式不影响 golden 选择）
        std::vector<std::vector<uint8_t>> expected(rankSize);
        for (uint32_t rank = 0; rank < rankSize; ++rank) {
            expected[rank] = BuildExpectedOutput(rankSize, scenario, rank, actualAlgName);
        }

        // 执行日志 dump（调试/驱动器对照工具）：环境变量 AICPU_ST_DUMP_EXECLOG=<目录> 时
        // 每用例落一份 ExecEvent 序列——驱动器收编/重构时用于行为零漂移对照
        const ExecResult execRet = executor.Execute(queues, report);
        DumpExecutionLog(executor.ExecutionLog());
        ASSERT_EQ(execRet, ExecResult::SUCCESS) << report;

        // DAG 内存冲突检测（WAW/WAR/RAW 无同步并发）；封顶输出避免刷屏
        conflicts = AicpuDagChecker().Check(executor.ExecutionLog());

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
    } catch (const std::exception& e) {
        ADD_FAILURE() << "execution-stage exception (likely product bug or invalid scenario): " << e.what();
        return;
    } catch (...) {
        ADD_FAILURE() << "execution-stage unknown exception (non-std exception type)";
        return;
    }

    // DAG 冲突报告（检测在 try 内完成，报告在 try 外输出——封顶 5 条避免刷屏）
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
}
} // namespace AicpuSt
} // namespace HcclSim
