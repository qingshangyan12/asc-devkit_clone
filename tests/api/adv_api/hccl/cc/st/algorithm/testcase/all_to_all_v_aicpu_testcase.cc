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
#include <memory>
#include <new>
#include <thread>
#include <vector>

#include "acl/acl_rt.h"
#include "alg_env_config.h"
#include "alltoallv_auto_selector.h"
#include "check_utils.h"
#include "gtest/gtest.h"
#include "hccl.h"
#include "hccl/hccl_types.h"
#include "hccl_verifier.h"
#include "ins_temp_all_to_all_v_mesh_1D.h"
#include "op_common.h"
#include "sim_world.h"

using namespace HcclSim;
using namespace mc2_ops_hccl;

namespace {

TopoMeta MakeMesh1DTopo(u32 rankSize)
{
    ServerMeta devices(rankSize);
    for (u32 rank = 0; rank < rankSize; ++rank) {
        devices[rank] = rank;
    }
    return TopoMeta{SuperPodMeta{devices}};
}

std::vector<u64> MakeDisplsMatrix(const std::vector<u64>& sendCountMatrix, u32 rankSize, bool receive, u64 padding)
{
    std::vector<u64> displs(rankSize * rankSize, 0);
    for (u32 rank = 0; rank < rankSize; ++rank) {
        u64 offset = padding;
        for (u32 peer = 0; peer < rankSize; ++peer) {
            displs[rank * rankSize + peer] = offset;
            const u64 count =
                receive ? sendCountMatrix[peer * rankSize + rank] : sendCountMatrix[rank * rankSize + peer];
            offset += count + padding;
        }
    }
    return displs;
}

u64 CalcBufferCount(const std::vector<u64>& counts, const std::vector<u64>& displs)
{
    u64 bufferCount = 0;
    for (size_t index = 0; index < counts.size(); ++index) {
        bufferCount = std::max(bufferCount, displs[index] + counts[index]);
    }
    return bufferCount;
}

HcclResult RunAllToAllVViaCommonPipeline(
    HcclComm comm, aclrtStream stream, void* sendBuf, void* recvBuf, HcclDataType dataType,
    const std::vector<u64>& sendCounts, const std::vector<u64>& recvCounts, const std::vector<u64>& sendDispls,
    const std::vector<u64>& recvDispls)
{
    const u32 rankSize = sendCounts.size();
    const u64 varMemSize = ALL_TO_ALL_V_VECTOR_NUM * static_cast<u64>(rankSize) * sizeof(u64);
    std::unique_ptr<u8[]> paramStorage(new (std::nothrow) u8[sizeof(OpParam) + varMemSize]);
    CHK_PTR_NULL(paramStorage);
    OpParam* rawParam = new (paramStorage.get()) OpParam();
    auto param = std::unique_ptr<OpParam, void (*)(OpParam*)>(rawParam, [](OpParam* ptr) { ptr->~OpParam(); });

    CHK_RET(HcclGetCommName(comm, param->commName));
    const int tagRet = sprintf_s(param->tag, sizeof(param->tag), "AllToAllV_Aicpu_Test");
    CHK_PRT_RET(tagRet <= 0, HCCL_ERROR("Failed to set AllToAllV test tag."), HCCL_E_INTERNAL);
    param->stream = stream;
    param->opMode = OpMode::OPBASE;
    param->inputPtr = sendBuf;
    param->outputPtr = recvBuf;
    param->inputSize = CalcBufferCount(sendCounts, sendDispls) * SIZE_TABLE[dataType];
    param->outputSize = CalcBufferCount(recvCounts, recvDispls) * SIZE_TABLE[dataType];
    param->hcclComm = comm;
    param->deviceType = DevType::DEV_TYPE_950;
    param->opType = HcclCMDType::HCCL_CMD_ALLTOALLV;
    param->varMemSize = varMemSize;
    param->all2AllVDataDes.sendType = dataType;
    param->all2AllVDataDes.recvType = dataType;

    u64* varData = reinterpret_cast<u64*>(param->varData);
    std::copy(sendCounts.begin(), sendCounts.end(), varData);
    std::copy(recvCounts.begin(), recvCounts.end(), varData + rankSize);
    std::copy(sendDispls.begin(), sendDispls.end(), varData + 2 * rankSize);
    std::copy(recvDispls.begin(), recvDispls.end(), varData + 3 * rankSize);
    param->all2AllVDataDes.sendCounts = varData;
    param->all2AllVDataDes.recvCounts = varData + rankSize;
    param->all2AllVDataDes.sdispls = varData + 2 * rankSize;
    param->all2AllVDataDes.rdispls = varData + 3 * rankSize;

    std::string algName;
    std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    CHK_RET(Selector(comm, *param, topoInfo, algName));
    CHK_RET(HcclExecOp(comm, *param, topoInfo, algName));
    return HCCL_SUCCESS;
}

class AllToAllVAicpuTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ResetAlgEnvConfigInitState();
        setenv("HCCL_OP_EXPANSION_MODE", "AI_CPU", 1);
        setenv("HCCL_BUFFSIZE", "200", 1);
        setenv("HCCL_INDEPENDENT_OP", "1", 1);
        setenv("HCCL_ENABLE_OPEN_AICPU", "1", 1);
    }

    void TearDown() override
    {
        unsetenv("HCCL_OP_EXPANSION_MODE");
        unsetenv("HCCL_BUFFSIZE");
        unsetenv("HCCL_INDEPENDENT_OP");
        unsetenv("HCCL_ENABLE_OPEN_AICPU");
    }

    void Run(u32 rankSize, HcclDataType dataType, const std::vector<u64>& sendCountMatrix, u64 displacementPadding = 0)
    {
        ASSERT_EQ(sendCountMatrix.size(), static_cast<size_t>(rankSize) * rankSize);
        const TopoMeta topoMeta = MakeMesh1DTopo(rankSize);
        const std::vector<u64> sendDisplsMatrix =
            MakeDisplsMatrix(sendCountMatrix, rankSize, false, displacementPadding);
        const std::vector<u64> recvDisplsMatrix =
            MakeDisplsMatrix(sendCountMatrix, rankSize, true, displacementPadding);
        SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);

        std::vector<HcclResult> results(rankSize, HCCL_E_INTERNAL);
        std::vector<std::thread> threads;
        for (u32 rankId = 0; rankId < rankSize; ++rankId) {
            threads.emplace_back([&, rankId]() {
                results[rankId] = [&]() -> HcclResult {
                    if (aclrtSetDevice(rankId) != ACL_SUCCESS) {
                        return HCCL_E_INTERNAL;
                    }
                    aclrtStream stream = nullptr;
                    if (aclrtCreateStream(&stream) != ACL_SUCCESS) {
                        return HCCL_E_INTERNAL;
                    }
                    HcclComm comm = nullptr;
                    CHK_RET(HcclCommInitClusterInfo("./ranktable.json", rankId, &comm));

                    std::vector<u64> sendCounts(rankSize, 0);
                    std::vector<u64> recvCounts(rankSize, 0);
                    std::vector<u64> sendDispls(rankSize, 0);
                    std::vector<u64> recvDispls(rankSize, 0);
                    for (u32 peer = 0; peer < rankSize; ++peer) {
                        sendCounts[peer] = sendCountMatrix[rankId * rankSize + peer];
                        recvCounts[peer] = sendCountMatrix[peer * rankSize + rankId];
                        sendDispls[peer] = sendDisplsMatrix[rankId * rankSize + peer];
                        recvDispls[peer] = recvDisplsMatrix[rankId * rankSize + peer];
                    }

                    const u64 dataTypeSize = SIZE_TABLE[dataType];
                    void* sendBuf = nullptr;
                    void* recvBuf = nullptr;
                    if (aclrtMalloc(
                            &sendBuf, CalcBufferCount(sendCounts, sendDispls) * dataTypeSize,
                            static_cast<aclrtMemMallocPolicy>(BUFFER_INPUT_MARK)) != ACL_SUCCESS ||
                        aclrtMalloc(
                            &recvBuf, CalcBufferCount(recvCounts, recvDispls) * dataTypeSize,
                            static_cast<aclrtMemMallocPolicy>(BUFFER_OUTPUT_MARK)) != ACL_SUCCESS) {
                        return HCCL_E_INTERNAL;
                    }
                    CHK_RET(RunAllToAllVViaCommonPipeline(
                        comm, stream, sendBuf, recvBuf, dataType, sendCounts, recvCounts, sendDispls, recvDispls));
                    CHK_RET(HcclCommDestroy(comm));
                    return HCCL_SUCCESS;
                }();
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
        for (HcclResult result : results) {
            EXPECT_EQ(result, HCCL_SUCCESS);
        }

        const bool hasData =
            std::any_of(sendCountMatrix.begin(), sendCountMatrix.end(), [](u64 count) { return count != 0; });
        if (hasData) {
            auto taskQueues = SimTaskQueue::Global()->GetAllRankTaskQueues();
            EXPECT_EQ(
                CheckAll2AllV(taskQueues, rankSize, dataType, sendCountMatrix, sendDisplsMatrix, recvDisplsMatrix),
                HCCL_SUCCESS);
        }
        SimWorld::Global()->Deinit();
    }
};

TEST_F(AllToAllVAicpuTest, TwoRankAllZeroInt8) { Run(2, HCCL_DATA_TYPE_INT8, std::vector<u64>(4, 0)); }

TEST_F(AllToAllVAicpuTest, ThreeRankSelfOnlyInt16) { Run(3, HCCL_DATA_TYPE_INT16, {1, 0, 0, 0, 2, 0, 0, 0, 3}); }

TEST_F(AllToAllVAicpuTest, FourRankSparseAsymmetricInt32WithDisplacementGaps)
{
    Run(4, HCCL_DATA_TYPE_INT32, {1, 3, 0, 2, 0, 2, 5, 0, 7, 0, 3, 1, 2, 4, 0, 4}, 1);
}

TEST_F(AllToAllVAicpuTest, TwoRankInt8MultiLoop)
{
    constexpr u64 countOverCclBuffer = 201ULL * 1024 * 1024;
    Run(2, HCCL_DATA_TYPE_UINT8, {1, countOverCclBuffer, 17, 2});
}

TEST_F(AllToAllVAicpuTest, TwoRankInt64) { Run(2, HCCL_DATA_TYPE_INT64, {1, 3, 2, 4}); }

TEST_F(AllToAllVAicpuTest, SeventeenRankUsesSingleCommunicationRound)
{
    constexpr u32 rankSize = 17;
    OpParam param;
    std::vector<u32> ranks(rankSize, 0);
    for (u32 rank = 0; rank < rankSize; ++rank) {
        ranks[rank] = rank;
    }
    InsTempAlltoAllVMesh1D algTemplate(param, 0, {ranks});
    EXPECT_EQ(algTemplate.CalcScratchMultiple(mc2_ops_hccl::BufferType::INPUT, mc2_ops_hccl::BufferType::OUTPUT), 16);
    EXPECT_EQ(algTemplate.CalcCommLoopsForTest(), 1);
    std::vector<u32> commRanks;
    algTemplate.CalcCommRankSetForOneLoopForTest(0, rankSize - 1, commRanks);
    EXPECT_EQ(commRanks.size(), 16);
}

TEST_F(AllToAllVAicpuTest, EighteenRankUsesSecondCommunicationRound)
{
    constexpr u32 rankSize = 18;
    OpParam param;
    std::vector<u32> ranks(rankSize, 0);
    for (u32 rank = 0; rank < rankSize; ++rank) {
        ranks[rank] = rank;
    }
    InsTempAlltoAllVMesh1D algTemplate(param, 0, {ranks});
    EXPECT_EQ(algTemplate.CalcScratchMultiple(mc2_ops_hccl::BufferType::INPUT, mc2_ops_hccl::BufferType::OUTPUT), 16);
    EXPECT_EQ(algTemplate.CalcCommLoopsForTest(), 2);
    std::vector<u32> firstRoundRanks;
    std::vector<u32> secondRoundRanks;
    algTemplate.CalcCommRankSetForOneLoopForTest(0, rankSize - 1, firstRoundRanks);
    algTemplate.CalcCommRankSetForOneLoopForTest(1, 1, secondRoundRanks);
    EXPECT_EQ(firstRoundRanks.size(), 16);
    EXPECT_EQ(secondRoundRanks.size(), 1);
}

TEST_F(AllToAllVAicpuTest, SelectorRejectsUnsupportedBoundaries)
{
    AlltoAllVAutoSelector selector;
    OpParam opParam;
    opParam.opType = HcclCMDType::HCCL_CMD_ALLTOALLV;
    opParam.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    TopoInfoWithNetLayerDetails topoInfo;
    topoInfo.serverNum = 1;
    topoInfo.userRankSize = 1;
    topoInfo.level0Topo = Level0Shape::MESH_1D;
    std::string algName;
    EXPECT_EQ(selector.Select(opParam, &topoInfo, algName), SelectorStatus::NOT_MATCH);

    topoInfo.userRankSize = 2;
    topoInfo.level0Topo = Level0Shape::MESH_1D_CLOS;
    EXPECT_EQ(selector.Select(opParam, &topoInfo, algName), SelectorStatus::NOT_MATCH);
}

} // namespace
