/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstddef>
#include <cstring>
#include <thread>
#include <vector>

#include "acl/acl_rt.h"
#include "alg_env_config.h"
#include "check_utils.h"
#include "gtest/gtest.h"
#include "hccl.h"
#include "hccl/hccl_types.h"
#include "hccl_verifier.h"
#include "hccl_alloc_ctx_res.h"
#include "hccl_mc2.h"
#include "sim_world.h"
#include "task_cache_test_stub.h"

using namespace HcclSim;

constexpr uint32_t DATATYPE_SIZE_TABLE_ALL_GATHER_ST[HCCL_DATA_TYPE_RESERVED] = {
    sizeof(int8_t),
    sizeof(int16_t),
    sizeof(int32_t),
    2,
    sizeof(float),
    sizeof(int64_t),
    sizeof(uint64_t),
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
    8,
    2,
    16,
    2,
    1,
    1,
    1,
    1};

constexpr uint8_t AICPU_TILING_ENGINE = static_cast<uint8_t>(mc2_ops_hccl::OpExecuteConfig::AICPU_TS);

struct AllGatherMc2Tiling {
    Mc2InitTilingInner init;
    Mc2CcTilingInner cc;
};

AllGatherMc2Tiling BuildAllGatherMc2Tiling(HcclDataType dataType, const std::string& algName)
{
    AllGatherMc2Tiling tiling{};
    tiling.init.version = INIT_TILING_VERSION;
    tiling.init.mc2HcommCnt = 1U;
    tiling.init.offset[0] = offsetof(AllGatherMc2Tiling, cc);
    tiling.cc.commEngine = AICPU_TILING_ENGINE;
    tiling.cc.srcDataType = static_cast<uint8_t>(dataType);
    tiling.cc.dstDataType = static_cast<uint8_t>(dataType);
    tiling.cc.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER);
    std::strncpy(tiling.cc.groupName, "all_gather_mc2_st", sizeof(tiling.cc.groupName) - 1U);
    if (!algName.empty()) {
        std::strncpy(tiling.cc.algConfig, algName.c_str(), sizeof(tiling.cc.algConfig) - 1U);
    }
    return tiling;
}

HcclResult SimulateMc2AllGatherLaunch(
    void* opResCtxPtr, aclrtStream stream, HcclComm comm, void* sendBuf, void* recvBuf, u64 sendCount,
    HcclDataType dataType, u32 rankSize)
{
    OpResCtx opResCtx{};
    aclError aclRet = aclrtMemcpy(
        &opResCtx, sizeof(opResCtx), opResCtxPtr, sizeof(opResCtx), ACL_MEMCPY_DEVICE_TO_HOST);
    CHK_RET(aclRet == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);
    CHK_PRT_RET(
        opResCtx.algInfo[0].opParam == 0U, HCCL_ERROR("[AllGatherMc2ST] opParam is null."), HCCL_E_PTR);

    mc2_ops_hccl::OpParam opParam{};
    aclRet = aclrtMemcpy(
        &opParam, sizeof(opParam), reinterpret_cast<void*>(opResCtx.algInfo[0].opParam), sizeof(opParam),
        ACL_MEMCPY_DEVICE_TO_HOST);
    CHK_RET(aclRet == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);
    CHK_PRT_RET(
        opParam.opThread == 0U, HCCL_ERROR("[AllGatherMc2ST] operator stream thread is null."), HCCL_E_PTR);
    const u64 dataTypeSize = DATATYPE_SIZE_TABLE_ALL_GATHER_ST[dataType];
    opParam.hcclComm = comm;
    opParam.stream = stream;
    opParam.inputPtr = sendBuf;
    opParam.outputPtr = recvBuf;
    opParam.inputSize = sendCount * dataTypeSize;
    opParam.outputSize = opParam.inputSize * rankSize;
    opParam.DataDes.count = sendCount;
    opParam.DataDes.dataType = dataType;
    opParam.DataDes.outputType = dataType;
    opParam.DataDes.strideCount = sendCount;

    ThreadHandle mainThread = 0U;
    u32 notifyNumOnMainThread = 0U;
    CHK_RET(mc2_ops_hccl::GetMainThreadInfo(comm, opParam, mainThread, notifyNumOnMainThread));
    CHK_PRT_RET(
        notifyNumOnMainThread == 0U, HCCL_ERROR("[AllGatherMc2ST] main thread has no notify."), HCCL_E_INTERNAL);
    ThreadHandle exportedMainThread = 0U;
    CHK_RET(HcclThreadExportToCommEngine(
        comm, 1U, &mainThread, COMM_ENGINE_CPU_TS, &exportedMainThread));

    // 模拟MC2算子kernel向AICPU server提交任务前的启动同步。
    CHK_RET(static_cast<HcclResult>(HcommThreadNotifyRecordOnThread(
        opParam.opThread, exportedMainThread, notifyNumOnMainThread - 1U)));

    aclrtArgsHandle argsHandle = nullptr;
    aclrtParamHandle paramHandle = nullptr;
    aclRet = aclrtKernelArgsInit(nullptr, &argsHandle);
    CHK_RET(aclRet == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);
    aclRet = aclrtKernelArgsAppend(argsHandle, &opParam, sizeof(opParam), &paramHandle);
    CHK_RET(aclRet == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);
    aclRet = aclrtLaunchKernelWithConfig(nullptr, 1U, stream, nullptr, argsHandle, nullptr);
    CHK_RET(aclRet == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);

    // 模拟MC2算子kernel等待AICPU server完成本次通信任务。
    CHK_RET(static_cast<HcclResult>(HcommThreadNotifyWaitOnThread(opParam.opThread, 0U, CUSTOM_TIMEOUT)));
    return HCCL_SUCCESS;
}

class ST_ALL_GATHER_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
    void TearDown() override
    {
        unsetenv("HCCL_OP_EXPANSION_MODE");
        unsetenv("HCCL_ENABLE_OPEN_AICPU");
        unsetenv("HCCL_ST_UBX_CONCURRENT_TOPO");
        unsetenv("HCCL_ST_USE_OPEN_EXECUTOR");
    }

    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};
u32 AnalyseRankSize(const TopoMeta& topoInfo)
{
    u32 rankSize = 0;
    for (const auto& superPod : topoInfo) {
        for (const auto& podIdx : superPod) {
            rankSize += podIdx.size();
        }
    }
    return rankSize;
}

void RunAllGatherAicpuA5(
    const TopoMeta& topoInfo, const u64& sendCount, const HcclDataType& dataType,
    const std::string& expectedAlgName = "", bool expectMultiKernel = false, bool useOpenExecutor = false,
    bool checkTaskGraph = true)
{
    if (!expectedAlgName.empty()) {
        setenv("HCCL_ST_UBX_CONCURRENT_TOPO", "1", 1);
    }
    ResetAicpuLaunchAlgNames();
    ResetOpenExecutorLaunchCount();
    if (useOpenExecutor) {
        setenv("HCCL_ST_USE_OPEN_EXECUTOR", "1", 1);
    }
    // 仿真模型初始化
    SimWorld::Global()->Init(topoInfo, DevType::DEV_TYPE_950);

    // 设置展开模式为HOST_TS
    setenv("HCCL_OP_EXPANSION_MODE", "AI_CPU", 1);
    setenv("HCCL_INDEPENDENT_OP", "1", 1);
    setenv("HCCL_ENABLE_OPEN_AICPU", "1", 1);

    const u32 dataTypeSize = DATATYPE_SIZE_TABLE_ALL_GATHER_ST[dataType];
    auto rankSize = AnalyseRankSize(topoInfo);
    // 每个rank从算子tiling进入MC2资源分配，并模拟device侧MC2 AICPU kernel launch。
    std::vector<HcclResult> rankResults(rankSize, HCCL_E_INTERNAL);
    std::vector<std::thread> threads;
    for (auto rankId = 0; rankId < rankSize; ++rankId) {
        threads.emplace_back([=, &rankResults]() {
            auto runRank = [&]() -> HcclResult {
                // 1.SetDevice
                CHK_RET(aclrtSetDevice(rankId) == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);

                // 2.创建流
                aclrtStream stream = nullptr;
                CHK_RET(aclrtCreateStream(&stream) == ACL_SUCCESS ? HCCL_SUCCESS : HCCL_E_RUNTIME);

                // 3.初始化通信域
                HcclComm comm = nullptr;
                CHK_RET(HcclCommInitClusterInfo("./ranktable.json", rankId, &comm));

                void* sendBuf = nullptr;
                void* recvBuf = nullptr;
                u64 sendBufSize = sendCount * dataTypeSize; // 数据量转化为字节数
                u64 recvBufSize = sendCount * dataTypeSize * rankSize;
                // 打桩实现，仿真运行需标记内存是INPUT和OUTPUT
                aclrtMalloc(&sendBuf, sendBufSize, static_cast<aclrtMemMallocPolicy>(BUFFER_INPUT_MARK));
                aclrtMalloc(&recvBuf, recvBufSize, static_cast<aclrtMemMallocPolicy>(BUFFER_OUTPUT_MARK));

                // 4.从Mc2InitTiling/Mc2CcTiling分配资源，不经过已删除的AllGather host op入口。
                AllGatherMc2Tiling tiling = BuildAllGatherMc2Tiling(dataType, expectedAlgName);
                void* opResCtx = nullptr;
                CHK_RET(HcclAllocComResourceByTiling(comm, stream, &tiling, &opResCtx));

                // 5.模拟算子kernel通过MC2 context发起AllGather数据调用。
                CHK_RET(SimulateMc2AllGatherLaunch(
                    opResCtx, stream, comm, sendBuf, recvBuf, sendCount, dataType, rankSize));

                // 6.销毁通信域
                CHK_RET(HcclCommDestroy(comm));
                return HCCL_SUCCESS;
            };
            rankResults[rankId] = runRank();
        });
    }

    // 等待多线程执行完成
    for (auto& thread : threads) {
        thread.join();
    }
    for (HcclResult result : rankResults) {
        ASSERT_EQ(result, HCCL_SUCCESS);
    }

    // 结果成图校验
    auto taskQueues = SimTaskQueue::Global()->GetAllRankTaskQueues();
    if (checkTaskGraph) {
        HcclResult res = CheckAllGather(taskQueues, rankSize, dataType, sendCount);
        EXPECT_TRUE(res == HCCL_SUCCESS);
    }
    if (!expectedAlgName.empty()) {
        const auto algNames = GetAicpuLaunchAlgNames();
        ASSERT_EQ(algNames.size(), rankSize);
        for (const auto& algName : algNames) {
            EXPECT_EQ(algName, expectedAlgName);
        }
    }
    if (expectMultiKernel) {
        for (const auto& pod : taskQueues) {
            for (const auto& server : pod.second) {
                for (const auto& rank : server.second) {
                    size_t activeQueueNum = 0;
                    for (const auto& queue : rank.second) {
                        activeQueueNum += queue.empty() ? 0U : 1U;
                    }
                    EXPECT_GE(activeQueueNum, 4U);
                }
            }
        }
    }
    EXPECT_EQ(GetOpenExecutorLaunchCount(), useOpenExecutor ? rankSize : 0U);

    // 资源清理
    SimWorld::Global()->Deinit();
    unsetenv("HCCL_ST_UBX_CONCURRENT_TOPO");
    unsetenv("HCCL_ST_USE_OPEN_EXECUTOR");
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_ubx_4rank_512kb_selects_sole_mesh_test)
{
    TopoMeta topoMeta{{{0, 1, 2, 3}}};
    RunAllGatherAicpuA5(
        topoMeta, 512U * 1024U, HcclDataType::HCCL_DATA_TYPE_UINT8, "AicpuAllGatherSoleMesh");
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_ubx_4rank_concurrent_mesh_nhr_e2e_test)
{
    TopoMeta topoMeta{{{0, 1, 2, 3}}};
    RunAllGatherAicpuA5(
        topoMeta, 600U * 1024U, HcclDataType::HCCL_DATA_TYPE_UINT8, "AicpuAllGatherConcurMeshNHR", true);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_ubx_4rank_concurrent_mc2_open_executor_e2e_test)
{
    TopoMeta topoMeta{{{0, 1, 2, 3}}};
    // The persistent KFC server's initial thread waits are outside this test's captured task window.
    RunAllGatherAicpuA5(
        topoMeta, 600U * 1024U, HcclDataType::HCCL_DATA_TYPE_UINT8, "AicpuAllGatherConcurMeshNHR", true, true,
        false);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_mesh_1d_2rank_int64_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                               // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_INT64; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_mesh_1d_3rank_int64_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100; // 1GB单卡数据量，scratch单rank分区为100M（200M/2）, 有24M尾块
    auto dataType = HcclDataType::HCCL_DATA_TYPE_INT64; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_mesh_1d_4rank_int32_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2, 3}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                               // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_INT32; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_mesh_1d_6rank_int16_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2, 3, 5}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                               // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_INT16; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_mesh_1d_8rank_int8_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2, 3, 4, 5, 6, 7}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                              // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_INT8; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_nhr_2rank_fp64_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0}, {0}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                              // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_FP64; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_nhr_3rank_fp32_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0}, {0}, {0}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                              // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_FP32; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_nhr_4rank_fp16_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0}, {0}, {0}, {0}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                              // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_FP16; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_nhr_8rank_bfp16_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                               // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_BFP16; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_nhr_6rank_fp8e5m2_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0}, {0}, {0}, {0}, {0}, {0}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100;                                 // 单卡数据量
    auto dataType = HcclDataType::HCCL_DATA_TYPE_FP8E5M2; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_nhr_6rank_fp8e8m0_big_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0}, {0}, {0}, {0}, {0}, {0}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 1024 * 1024 * 1024; // 单卡数据量

    auto dataType = HcclDataType::HCCL_DATA_TYPE_FP8E8M0; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_parallel_nhr_mesh1d_2x2rank_small_data_test)
{
    // 1GB较大数据量（多loop），同时包含尾块，数据宽度8B
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1}, {0, 1}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100; // 单卡数据量

    auto dataType = HcclDataType::HCCL_DATA_TYPE_UINT64; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_parallel_nhr_mesh1d_2x3rank_small_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2}, {0, 1, 2}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 100; // 单卡数据量

    auto dataType = HcclDataType::HCCL_DATA_TYPE_UINT32; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_parallel_nhr_mesh1d_2x3rank_big_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2}, {0, 1, 2}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 300 * 1024 * 1024; // 单卡数据量

    auto dataType = HcclDataType::HCCL_DATA_TYPE_UINT16; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_AICPU_TEST, st_all_gather_a5_aicpu_parallel_nhr_mesh1d_3x3rank_big_data_test)
{
    // 仿真模型初始化
    TopoMeta topoMeta{{{0, 1, 2}, {0, 1, 2}, {0, 1, 2}}}; // 三维数组指定超节点-Server-Device信息

    // 算子执行参数设置
    auto sendCount = 300 * 1024 * 1024; // 单卡数据量

    auto dataType = HcclDataType::HCCL_DATA_TYPE_UINT8; // 数据类型
    RunAllGatherAicpuA5(topoMeta, sendCount, dataType);
}
