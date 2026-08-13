/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "base.h"
#include "dtype_common.h"
#include "hccl_alloc_ctx_res.h"
#include "hccl_mc2.h"
#include "sim_communicator.h"
#include "sim_world.h"
#include "topo_model.h"

namespace {

extern "C" void ResetCcuKernelRegisterCount();
extern "C" uint32_t GetCcuKernelRegisterCount();

constexpr uint32_t TEST_RANK_SIZE = 8;
constexpr uint32_t TEST_RANK_ID = 0;
constexpr uint8_t CCU_TILING_ENGINE = static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED);
std::vector<void*> g_allocatedPtrs;

void StubCleanup()
{
    ResetCcuKernelRegisterCount();
    for (void* ptr : g_allocatedPtrs) {
        free(ptr);
    }
    g_allocatedPtrs.clear();
    unsetenv("HCCL_OP_EXPANSION_MODE");
}

void SetCcuEngineEnv()
{
    setenv("HCCL_OP_EXPANSION_MODE", "CCU_MS", 1);
}

TopoMeta BuildTopoMeta(uint32_t rankSize)
{
    TopoMeta topoMeta;
    SuperPodMeta superPod;
    ServerMeta server;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        server.push_back(rank);
    }
    superPod.push_back(server);
    topoMeta.push_back(superPod);
    return topoMeta;
}

Mc2CcTilingInner BuildAllToAllVTiling(
    HcclDataType sendType = HCCL_DATA_TYPE_FP16, HcclDataType recvType = HCCL_DATA_TYPE_FP16)
{
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALLV);
    tiling.commEngine = CCU_TILING_ENGINE;
    tiling.srcDataType = static_cast<uint8_t>(sendType);
    tiling.dstDataType = static_cast<uint8_t>(recvType);
    strcpy(tiling.groupName, "test_group");
    strcpy(tiling.algConfig, "CcuAlltoAllVMesh1D");
    return tiling;
}

struct Mc2TilingTestData {
    uint32_t version;
    uint32_t cnt;
    uint32_t offset[MAX_CC_TILING_NUM];
    Mc2ServerCfg serverCfg;
    Mc2CcTilingInner ccTiling[MAX_CC_TILING_NUM];
};

Mc2TilingTestData BuildMc2Tiling(const std::vector<Mc2CcTilingInner>& tilings)
{
    Mc2TilingTestData result{};
    result.version = INIT_TILING_CCU_NEW_VERSION;
    result.cnt = tilings.size();
    const uint32_t baseOffset = offsetof(Mc2TilingTestData, ccTiling);
    for (uint32_t i = 0; i < MAX_CC_TILING_NUM; ++i) {
        result.offset[i] = baseOffset + i * sizeof(Mc2CcTilingInner);
    }
    for (uint32_t i = 0; i < tilings.size(); ++i) {
        result.ccTiling[i] = tilings[i];
    }
    return result;
}

HcclResult CcuSelectAlgForTest(HcclComm comm, void* stream, const std::string topoTag[],
    const void* ccTilingList[], uint32_t tilingNum, OpResCtx& resCtx)
{
    Mc2InitTilingInner initTiling{};
    return CcuSelectAlg(comm, stream, topoTag, ccTilingList, tilingNum, &initTiling, resCtx);
}

class CcuMc2AllToAllVTest : public testing::Test {
protected:
    void SetUp() override
    {
        StubCleanup();
        const TopoMeta topoMeta = BuildTopoMeta(TEST_RANK_SIZE);
        HcclSim::SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
        ASSERT_EQ(aclrtSetDevice(TEST_RANK_ID), ACL_SUCCESS);
        ASSERT_EQ(HcclSim::Sim_HcclCommInitClusterInfo(topoMeta, TEST_RANK_ID, &comm_), HCCL_SUCCESS);
        stream_ = &dummyStream_;
        SetCcuEngineEnv();
    }

    void TearDown() override
    {
        if (comm_ != nullptr) {
            delete static_cast<HcclSim::SimCommunicator*>(comm_);
            comm_ = nullptr;
        }
        HcclSim::SimWorld::Global()->Deinit();
        StubCleanup();
    }

    HcclComm comm_ = nullptr;
    aclrtStream stream_ = nullptr;
    int dummyStream_ = 1;
};

TEST_F(CcuMc2AllToAllVTest, AllocCcuOpResCtxInitializesQueueResources)
{
    OpResCtx resCtx{};
    ASSERT_EQ(AllocCcuOpResCtx(comm_, "alltoallv_ctx", TEST_RANK_SIZE, TEST_RANK_ID, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.rankId, TEST_RANK_ID);
    EXPECT_EQ(resCtx.rankSize, TEST_RANK_SIZE);
    EXPECT_EQ(resCtx.workSpaceSize, Hccl::MC2_WORKSPACE_SIZE);
    EXPECT_NE(resCtx.workSpace, 0U);
    EXPECT_NE(resCtx.xnAddr, 0U);
    EXPECT_NE(resCtx.ckeAddr, 0U);
    EXPECT_NE(resCtx.workSpace, resCtx.xnAddr);
    EXPECT_NE(resCtx.workSpace, resCtx.ckeAddr);
}

TEST_F(CcuMc2AllToAllVTest, SelectsAllToAllVMesh1D)
{
    OpResCtx resCtx{};
    ASSERT_EQ(AllocCcuOpResCtx(comm_, "select_ctx", TEST_RANK_SIZE, TEST_RANK_ID, resCtx), HCCL_SUCCESS);
    Mc2CcTilingInner tiling = BuildAllToAllVTiling();
    const void* tilingList[] = {&tiling};
    std::string tags[] = {"alltoallv"};

    ASSERT_EQ(CcuSelectAlgForTest(comm_, stream_, tags, tilingList, 1, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALLV));
    EXPECT_EQ(resCtx.algorithmType[0], static_cast<uint32_t>(CcuAlltoAllVMesh1D));
    EXPECT_EQ(GetCcuKernelRegisterCount(), 1U);
}

TEST_F(CcuMc2AllToAllVTest, InitializesReduceTypeForKfcKernelQuery)
{
    Mc2CcTilingInner tiling = BuildAllToAllVTiling();
    OpParam opParam{};

    ASSERT_EQ(InitOpParamByTiling(comm_, stream_, "reduce_type", &tiling, opParam), HCCL_SUCCESS);
    EXPECT_EQ(opParam.reduceType, HcclReduceOp::HCCL_REDUCE_SUM);
}

TEST_F(CcuMc2AllToAllVTest, MultipleAllToAllVConfigsFallBackFromMc2Ccu)
{
    Mc2CcTilingInner fp16 = BuildAllToAllVTiling(HCCL_DATA_TYPE_FP16, HCCL_DATA_TYPE_FP16);
    Mc2CcTilingInner fp32 = BuildAllToAllVTiling(HCCL_DATA_TYPE_FP32, HCCL_DATA_TYPE_FP32);
    Mc2TilingTestData tiling = BuildMc2Tiling({fp16, fp32});
    void* opResCtx = nullptr;

    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_E_ALG_NOT_SUPPORTED);
    EXPECT_EQ(opResCtx, nullptr);
    EXPECT_EQ(GetCcuKernelRegisterCount(), 0U);
}

TEST_F(CcuMc2AllToAllVTest, DefaultAlgorithmConfigFallsBackFromMc2Ccu)
{
    Mc2CcTilingInner ccTiling = BuildAllToAllVTiling();
    strcpy(ccTiling.algConfig, "default");
    Mc2TilingTestData tiling = BuildMc2Tiling({ccTiling});
    void* opResCtx = nullptr;

    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_E_ALG_NOT_SUPPORTED);
    EXPECT_EQ(opResCtx, nullptr);
}

TEST_F(CcuMc2AllToAllVTest, AllocatesAllToAllVResourceFromTiling)
{
    Mc2TilingTestData tiling = BuildMc2Tiling({BuildAllToAllVTiling()});
    void* opResCtx = nullptr;
    ASSERT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_SUCCESS);
    ASSERT_NE(opResCtx, nullptr);
    auto* ctx = static_cast<OpResCtx*>(opResCtx);
    EXPECT_EQ(ctx->opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALLV));
    EXPECT_EQ(ctx->algorithmType[0], static_cast<uint32_t>(CcuAlltoAllVMesh1D));
    EXPECT_EQ(ctx->rankId, TEST_RANK_ID);
    EXPECT_EQ(ctx->rankSize, TEST_RANK_SIZE);
}

TEST_F(CcuMc2AllToAllVTest, KfcServerArgsUseAllocatedQueueAndToken)
{
    void* cclBuffer = nullptr;
    uint64_t cclBufferSize = 0;
    ASSERT_EQ(HcclGetHcclBuffer(comm_, &cclBuffer, &cclBufferSize), HCCL_SUCCESS);

    OpResCtx opResCtx{};
    ASSERT_EQ(AllocCcuOpResCtx(comm_, "args_ctx", TEST_RANK_SIZE, TEST_RANK_ID, opResCtx), HCCL_SUCCESS);
    Mc2CcTilingInner tiling = BuildAllToAllVTiling();
    OpParam opParam{};
    ASSERT_EQ(InitOpParamByTiling(comm_, stream_, "args_tag", &tiling, opParam), HCCL_SUCCESS);

    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    ASSERT_EQ(SelectAlgAndPrepareEngine(comm_, opParam, algName, topoInfo), HCCL_SUCCESS);
    ASSERT_EQ(algName, "CcuAlltoAllVMesh1D");
    ASSERT_GT(sprintf_s(opParam.algName, sizeof(opParam.algName), "%s", algName.c_str()), 0);

    bool skipGetRes = false;
    ASSERT_EQ(HandleSingleRankAndCommMode(comm_, opParam, skipGetRes), HCCL_SUCCESS);
    ASSERT_FALSE(skipGetRes);
    void* resCtxOut = nullptr;
    ASSERT_EQ(GetCcuOpParamResCtx(comm_, algName, opParam, topoInfo.get(), opResCtx, &resCtxOut), HCCL_SUCCESS);
    ASSERT_NE(resCtxOut, nullptr);

    auto* raw = static_cast<char*>(resCtxOut);
    std::vector<char> sequence(raw, raw + opParam.ctxSize);
    mc2_ops_hccl::AlgResourceCtxSerializable resourceCtx;
    resourceCtx.DeSerialize(sequence);
    ASSERT_EQ(resourceCtx.kfcServerArgs.size(), 6U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[0], opResCtx.xnAddr);
    EXPECT_EQ(resourceCtx.kfcServerArgs[1], opResCtx.ckeAddr);
    EXPECT_EQ(resourceCtx.kfcServerArgs[2], 1U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[3], 1U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[4], 0U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[5], reinterpret_cast<uint64_t>(cclBuffer) + 0x1234U);
}

TEST_F(CcuMc2AllToAllVTest, RejectsCcuMsEngine)
{
    Mc2CcTilingInner ccTiling = BuildAllToAllVTiling();
    ccTiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::CCU_MS);
    const void* list[] = {&ccTiling};
    uint8_t engine = 0;
    EXPECT_EQ(ObtainCommEngine(list, 1, engine), HCCL_E_NOT_SUPPORT);

    Mc2TilingTestData tiling = BuildMc2Tiling({ccTiling});
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_E_NOT_SUPPORT);
    EXPECT_EQ(opResCtx, nullptr);
}

TEST_F(CcuMc2AllToAllVTest, UnknownAlgorithmConfigFallsBackFromMc2Ccu)
{
    Mc2CcTilingInner ccTiling = BuildAllToAllVTiling();
    strcpy(ccTiling.algConfig, "CcuUnknownAllToAllV");
    Mc2TilingTestData tiling = BuildMc2Tiling({ccTiling});
    void* opResCtx = nullptr;

    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_E_ALG_NOT_SUPPORTED);
    EXPECT_EQ(opResCtx, nullptr);
}

TEST_F(CcuMc2AllToAllVTest, ValidatesNullInputs)
{
    Mc2TilingTestData tiling = BuildMc2Tiling({BuildAllToAllVTiling()});
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(nullptr, stream_, &tiling, &opResCtx), HCCL_E_PTR);
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, nullptr, &tiling, &opResCtx), HCCL_E_PTR);
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, nullptr, &opResCtx), HCCL_E_PTR);
}

} // namespace
