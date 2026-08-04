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
#include <cstdlib>
#include <limits>
#include <memory>
#include <new>
#include <thread>
#include <vector>

#include "acl/acl_rt.h"
#include "alg_env_config.h"
#include "alltoall_auto_selector.h"
#include "alltoallv_auto_selector.h"
#include "cann_bridge_route.h"
#include "check_utils.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"
#include "hccl.h"
#include "hccl_verifier.h"
#include "ins_temp_all_to_all_v_mesh_1D.h"
#include "op_common.h"
#include "sim_world.h"

using namespace HcclSim;
using namespace mc2_ops_hccl;

namespace checker {
namespace {
constexpr u64 ALLTOALL_MULTI_CHANNEL_LIMIT = 150ULL * 1024ULL * 1024ULL;
constexpr u64 ALLTOALLV_VECTOR_NUM = 4U;

struct OpParamDeleter {
    void operator()(OpParam* param) const
    {
        if (param != nullptr) {
            param->~OpParam();
            std::free(param);
        }
    }
};

HcclResult ExecuteLocalAlltoAllV(
    const void* sendBuf, const std::vector<u64>& sendCounts, const std::vector<u64>& sendDispls, HcclDataType dataType,
    const void* recvBuf, const std::vector<u64>& recvCounts, const std::vector<u64>& recvDispls, HcclCMDType opType,
    HcclComm comm, aclrtStream stream, const char* forcedAlgName = nullptr)
{
    const u64 rankSize = sendCounts.size();
    CHK_PRT_RET(
        rankSize == 0U || recvCounts.size() != rankSize || sendDispls.size() != rankSize ||
            recvDispls.size() != rankSize,
        HCCL_ERROR("[ExecuteLocalAlltoAllV] invalid vector sizes."), HCCL_E_PARA);

    const u64 varMemSize = ALLTOALLV_VECTOR_NUM * rankSize * sizeof(u64);
    void* storage = std::malloc(sizeof(OpParam) + varMemSize);
    CHK_PTR_NULL(storage);
    std::unique_ptr<OpParam, OpParamDeleter> param(new (storage) OpParam());

    CHK_RET(HcclGetCommName(comm, param->commName));
    const char* tag = opType == HCCL_CMD_ALLTOALL ? "AlltoAll_AICPU_ST" : "AlltoAllV_AICPU_ST";
    CHK_PRT_RET(
        sprintf_s(param->tag, sizeof(param->tag), "%s", tag) <= 0,
        HCCL_ERROR("[ExecuteLocalAlltoAllV] failed to set tag."), HCCL_E_INTERNAL);

    param->stream = stream;
    param->opMode = OpMode::OPBASE;
    param->deviceType = DevType::DEV_TYPE_950;
    param->inputPtr = const_cast<void*>(sendBuf);
    param->outputPtr = const_cast<void*>(recvBuf);
    param->varMemSize = varMemSize;
    param->opType = opType;
    param->all2AllVDataDes.sendType = dataType;
    param->all2AllVDataDes.recvType = dataType;

    u64 maxInputCount = 0U;
    u64 maxOutputCount = 0U;
    for (u64 rank = 0U; rank < rankSize; ++rank) {
        maxInputCount = std::max(maxInputCount, sendDispls[rank] + sendCounts[rank]);
        maxOutputCount = std::max(maxOutputCount, recvDispls[rank] + recvCounts[rank]);
    }
    const u64 dataTypeSize = DATATYPE_SIZE_TABLE[dataType];
    param->inputSize = maxInputCount * dataTypeSize;
    param->outputSize = maxOutputCount * dataTypeSize;

    u64* varData = reinterpret_cast<u64*>(param->varData);
    std::copy(sendCounts.begin(), sendCounts.end(), varData);
    std::copy(recvCounts.begin(), recvCounts.end(), varData + rankSize);
    std::copy(sendDispls.begin(), sendDispls.end(), varData + 2U * rankSize);
    std::copy(recvDispls.begin(), recvDispls.end(), varData + 3U * rankSize);
    param->all2AllVDataDes.sendCounts = varData;
    param->all2AllVDataDes.recvCounts = varData + rankSize;
    param->all2AllVDataDes.sdispls = varData + 2U * rankSize;
    param->all2AllVDataDes.rdispls = varData + 3U * rankSize;

    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    CHK_RET(Selector(comm, *param, topoInfo, algName));
    if (forcedAlgName != nullptr) {
        algName = forcedAlgName;
        CHK_RET(SetOpParamAlgTag(*param, algName));
    }
    CHK_PRT_RET(
        sprintf_s(param->algName, sizeof(param->algName), "%s", algName.c_str()) <= 0,
        HCCL_ERROR("[ExecuteLocalAlltoAllV] failed to set algName."), HCCL_E_INTERNAL);
    CHK_RET(HcclExecOp(comm, *param, topoInfo, algName));
    return HCCL_SUCCESS;
}

void SetAicpuEnvironment()
{
    setenv("HCCL_OP_EXPANSION_MODE", "AI_CPU", 1);
    setenv("HCCL_BUFFSIZE", "200", 1);
    setenv("HCCL_INDEPENDENT_OP", "1", 1);
    setenv("HCCL_ENABLE_OPEN_AICPU", "1", 1);
}

void RunAlltoAll(
    const TopoMeta& topoMeta, u32 rankSize, HcclDataType dataType, u64 count, const char* forcedAlgName = nullptr)
{
    SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
    SetAicpuEnvironment();

    std::vector<std::thread> threads;
    for (u32 rankId = 0U; rankId < rankSize; ++rankId) {
        threads.emplace_back([=]() {
            aclrtSetDevice(rankId);
            aclrtStream stream = nullptr;
            aclrtCreateStream(&stream);
            HcclComm comm = nullptr;
            CHK_RET(HcclCommInitClusterInfo("./ranktable.json", rankId, &comm));

            void* sendBuf = nullptr;
            void* recvBuf = nullptr;
            const u64 bufferSize = count * DATATYPE_SIZE_TABLE[dataType] * rankSize;
            aclrtMalloc(&sendBuf, bufferSize, static_cast<aclrtMemMallocPolicy>(BUFFER_INPUT_MARK));
            aclrtMalloc(&recvBuf, bufferSize, static_cast<aclrtMemMallocPolicy>(BUFFER_OUTPUT_MARK));
            std::vector<u64> counts(rankSize, count);
            std::vector<u64> displs(rankSize, 0U);
            for (u32 peer = 1U; peer < rankSize; ++peer) {
                displs[peer] = displs[peer - 1U] + count;
            }
            CHK_RET(ExecuteLocalAlltoAllV(
                sendBuf, counts, displs, dataType, recvBuf, counts, displs, HCCL_CMD_ALLTOALL, comm, stream,
                forcedAlgName));
            CHK_RET(HcclCommDestroy(comm));
            return HCCL_SUCCESS;
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    auto taskQueues = SimTaskQueue::Global()->GetAllRankTaskQueues();
    EXPECT_EQ(CheckAll2All(taskQueues, rankSize, dataType, count), HCCL_SUCCESS);
    SimWorld::Global()->Deinit();
}

void RunAlltoAllV(
    const TopoMeta& topoMeta, u32 rankSize, HcclDataType dataType, const std::vector<u64>& sendCountMatrix)
{
    SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
    SetAicpuEnvironment();

    std::vector<std::thread> threads;
    for (u32 rankId = 0U; rankId < rankSize; ++rankId) {
        threads.emplace_back([=, &sendCountMatrix]() {
            aclrtSetDevice(rankId);
            aclrtStream stream = nullptr;
            aclrtCreateStream(&stream);
            HcclComm comm = nullptr;
            CHK_RET(HcclCommInitClusterInfo("./ranktable.json", rankId, &comm));

            std::vector<u64> sendCounts(rankSize, 0U);
            std::vector<u64> recvCounts(rankSize, 0U);
            std::vector<u64> sendDispls(rankSize, 0U);
            std::vector<u64> recvDispls(rankSize, 0U);
            u64 sendCount = 0U;
            u64 recvCount = 0U;
            for (u32 peer = 0U; peer < rankSize; ++peer) {
                sendCounts[peer] = sendCountMatrix[rankId * rankSize + peer];
                recvCounts[peer] = sendCountMatrix[peer * rankSize + rankId];
                sendDispls[peer] = sendCount;
                recvDispls[peer] = recvCount;
                sendCount += sendCounts[peer];
                recvCount += recvCounts[peer];
            }

            void* sendBuf = nullptr;
            void* recvBuf = nullptr;
            aclrtMalloc(
                &sendBuf, sendCount * DATATYPE_SIZE_TABLE[dataType],
                static_cast<aclrtMemMallocPolicy>(BUFFER_INPUT_MARK));
            aclrtMalloc(
                &recvBuf, recvCount * DATATYPE_SIZE_TABLE[dataType],
                static_cast<aclrtMemMallocPolicy>(BUFFER_OUTPUT_MARK));
            CHK_RET(ExecuteLocalAlltoAllV(
                sendBuf, sendCounts, sendDispls, dataType, recvBuf, recvCounts, recvDispls, HCCL_CMD_ALLTOALLV, comm,
                stream));
            CHK_RET(HcclCommDestroy(comm));
            return HCCL_SUCCESS;
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    auto taskQueues = SimTaskQueue::Global()->GetAllRankTaskQueues();
    EXPECT_EQ(CheckAll2AllV(taskQueues, rankSize, dataType, sendCountMatrix), HCCL_SUCCESS);
    SimWorld::Global()->Deinit();
}
} // namespace

class ST_ALL_TO_ALL_MESH1D_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }

    void TearDown() override
    {
        unsetenv("HCCL_OP_EXPANSION_MODE");
        unsetenv("HCCL_BUFFSIZE");
        unsetenv("HCCL_INDEPENDENT_OP");
        unsetenv("HCCL_ENABLE_OPEN_AICPU");
    }
};

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, route_uses_local_asc_devkit_algorithms)
{
    EXPECT_TRUE(UseCannBridgeForAicpuOp(COMM_ENGINE_AICPU_TS, HCCL_CMD_ALLREDUCE));
    EXPECT_TRUE(UseCannBridgeForAicpuOp(COMM_ENGINE_AICPU, HCCL_CMD_ALLREDUCE));
    EXPECT_FALSE(UseCannBridgeForAicpuOp(COMM_ENGINE_AICPU_TS, HCCL_CMD_ALLGATHER));
    EXPECT_FALSE(UseCannBridgeForAicpuOp(COMM_ENGINE_AICPU_TS, HCCL_CMD_REDUCE_SCATTER));
    EXPECT_FALSE(UseCannBridgeForAicpuOp(COMM_ENGINE_AICPU_TS, HCCL_CMD_ALLTOALLV));
    EXPECT_FALSE(UseCannBridgeForAicpuOp(COMM_ENGINE_AICPU_TS, HCCL_CMD_ALLTOALL));
    EXPECT_FALSE(UseCannBridgeForAicpuOp(COMM_ENGINE_CPU, HCCL_CMD_ALLREDUCE));
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, selectors_return_local_mesh1d_algorithms)
{
    TopoInfoWithNetLayerDetails topoInfo{};
    topoInfo.userRankSize = 2U;
    topoInfo.serverNum = 1U;
    topoInfo.level0Topo = Level0Shape::MESH_1D;
    OpParam param{};
    param.engine = COMM_ENGINE_AICPU_TS;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;

    std::string algName;
    param.opType = HCCL_CMD_ALLTOALLV;
    AlltoAllVAutoSelector alltoallvSelector;
    EXPECT_EQ(alltoallvSelector.Select(param, &topoInfo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "InsAlltoAllVMesh1D");

    algName.clear();
    param.opType = HCCL_CMD_ALLTOALL;
    AlltoAllAutoSelector alltoallSelector;
    EXPECT_EQ(alltoallSelector.Select(param, &topoInfo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "InsAlltoAllMesh1D");

    algName.clear();
    topoInfo.level0Topo = Level0Shape::MESH_1D_CLOS;
    EXPECT_EQ(alltoallSelector.Select(param, &topoInfo, algName), SelectorStatus::NOT_MATCH);
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, registry_contains_all_three_algorithms)
{
    auto& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLTOALLV, "InsAlltoAllVMesh1D"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLTOALL, "InsAlltoAllMesh1DSingleChannel"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLTOALL, "InsAlltoAllMesh1D"), nullptr);
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, alltoall_multi_channel_threshold_is_strict_and_overflow_safe)
{
    constexpr u64 rankSize = 3U;
    constexpr u64 dataTypeSize = 1U;
    constexpr u64 countAtLimit = ALLTOALL_MULTI_CHANNEL_LIMIT / rankSize;
    EXPECT_FALSE(ShouldUseMultiChannelForAlltoAll(countAtLimit - 1U, dataTypeSize, rankSize));
    EXPECT_FALSE(ShouldUseMultiChannelForAlltoAll(countAtLimit, dataTypeSize, rankSize));
    EXPECT_TRUE(ShouldUseMultiChannelForAlltoAll(countAtLimit + 1U, dataTypeSize, rankSize));
    EXPECT_TRUE(ShouldUseMultiChannelForAlltoAll(std::numeric_limits<u64>::max(), 16U, 18U));
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, alltoall_mesh1d_aicpu_semantics)
{
    const TopoMeta topoMeta{{{0, 1, 2}}};
    RunAlltoAll(topoMeta, 3U, HCCL_DATA_TYPE_INT32, 17U);
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, alltoall_single_channel_token_aicpu_semantics)
{
    const TopoMeta topoMeta{{{0, 1, 2}}};
    RunAlltoAll(topoMeta, 3U, HCCL_DATA_TYPE_INT32, 11U, "InsAlltoAllMesh1DSingleChannel");
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, peer_group_rounds_cover_18_ranks)
{
    const std::vector<u32> firstRound = CalcAlltoallMesh1DCommRanks(0U, 18U, 16U, 0U, 17U);
    const std::vector<u32> secondRound = CalcAlltoallMesh1DCommRanks(0U, 18U, 16U, 1U, 1U);
    EXPECT_EQ(firstRound.size(), 16U);
    EXPECT_EQ(std::find(firstRound.begin(), firstRound.end(), 0U), firstRound.end());
    std::vector<u32> sortedFirstRound = firstRound;
    std::sort(sortedFirstRound.begin(), sortedFirstRound.end());
    EXPECT_EQ(std::unique(sortedFirstRound.begin(), sortedFirstRound.end()), sortedFirstRound.end());
    EXPECT_EQ(
        sortedFirstRound, std::vector<u32>({1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U}));
    EXPECT_EQ(secondRound, std::vector<u32>({9U}));
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, alltoall_mesh1d_max_simulated_rank_semantics)
{
    TopoMeta topoMeta;
    GenTopoMeta(topoMeta, 1, 1, 8);
    RunAlltoAll(topoMeta, 8U, HCCL_DATA_TYPE_INT8, 1U);
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, alltoallv_mesh1d_aicpu_semantics)
{
    const TopoMeta topoMeta{{{0, 1, 2}}};
    const std::vector<u64> sendCountMatrix{3U, 5U, 0U, 7U, 2U, 11U, 13U, 0U, 17U};
    RunAlltoAllV(topoMeta, 3U, HCCL_DATA_TYPE_UINT16, sendCountMatrix);
}
} // namespace checker
