/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <thread>
#include <vector>

#include "acl/acl_rt.h"
#include "alg_env_config.h"
#include "check_utils.h"
#include "gtest/gtest.h"
#include "hccl.h"
#include "hccl/hccl_types.h"
#include "hccl_verifier.h"
#include "sim_world.h"

using namespace HcclSim;
using namespace mc2_ops_hccl;

namespace {

void RunReduceScatterMeshChunk(
    const TopoMeta& topoMeta, u64 recvCount, HcclDataType dataType, size_t dataTypeSize, HcclReduceOp reduceOp)
{
    u32 rankSize = 0;
    for (const auto& server : topoMeta[0]) {
        rankSize += server.size();
    }

    SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
    setenv("HCCL_OP_EXPANSION_MODE", "AI_CPU", 1);
    setenv("ENABLE_HOSTDPU_FOR_LLT", "1", 1);
    setenv("HCCL_INDEPENDENT_OP", "1", 1);
    setenv("HCCL_ENABLE_OPEN_AICPU", "1", 1);

    std::vector<std::thread> threads;
    for (u32 rankId = 0; rankId < rankSize; ++rankId) {
        threads.emplace_back([=]() {
            aclrtSetDevice(rankId);
            aclrtStream stream = nullptr;
            aclrtCreateStream(&stream);

            HcclComm comm = nullptr;
            CHK_RET(HcclCommInitClusterInfo("./ranktable.json", rankId, &comm));
            void* sendBuf = nullptr;
            void* recvBuf = nullptr;
            aclrtMalloc(
                &sendBuf, recvCount * dataTypeSize * rankSize, static_cast<aclrtMemMallocPolicy>(BUFFER_INPUT_MARK));
            aclrtMalloc(&recvBuf, recvCount * dataTypeSize, static_cast<aclrtMemMallocPolicy>(BUFFER_OUTPUT_MARK));
            CHK_RET(HcclReduceScatter(sendBuf, recvBuf, recvCount, dataType, reduceOp, comm, stream));
            CHK_RET(HcclCommDestroy(comm));
            return HCCL_SUCCESS;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto taskQueues = SimTaskQueue::Global()->GetAllRankTaskQueues();
    EXPECT_EQ(CheckReduceScatter(taskQueues, rankSize, dataType, recvCount, reduceOp), HCCL_SUCCESS);
    SimWorld::Global()->Deinit();
}

} // namespace

class ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }

    void TearDown() override
    {
        unsetenv("HCCL_OP_EXPANSION_MODE");
        unsetenv("ENABLE_HOSTDPU_FOR_LLT");
        unsetenv("HCCL_INDEPENDENT_OP");
        unsetenv("HCCL_ENABLE_OPEN_AICPU");
    }
};

TEST_F(ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST, meshchunk_3rank_fp32_sum)
{
    TopoMeta topoMeta{{{0, 1, 2}}};
    RunReduceScatterMeshChunk(
        topoMeta, 2 * 1024 * 1024, HcclDataType::HCCL_DATA_TYPE_FP32, sizeof(float), HcclReduceOp::HCCL_REDUCE_SUM);
}

TEST_F(ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST, meshchunk_8rank_int8_min)
{
    TopoMeta topoMeta{{{0, 1, 2, 3, 4, 5, 6, 7}}};
    RunReduceScatterMeshChunk(
        topoMeta, 16 * 1024 * 1024 + 1, HcclDataType::HCCL_DATA_TYPE_INT8, sizeof(int8_t),
        HcclReduceOp::HCCL_REDUCE_MIN);
}
