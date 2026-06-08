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
#include <cstring>
#include <vector>
#include <unordered_map>
#include <cstdlib>

#include "dtype_common.h"
#include "base.h"
#include "hccl_mc2.h"
#include "hccl_alloc_ctx_res.h"
#include "sim_communicator.h"
#include "topo_model.h"
#include "sim_common.h"

namespace {

static u32 g_stubRankSize = 8;
static u32 g_stubRankId = 0;
static DevType g_stubDeviceType = DevType::DEV_TYPE_950;
static std::vector<void*> g_allocatedPtrs;
static std::unordered_map<uint32_t, std::string> g_opTypeToAlgName = {
    {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER), "CcuAllGatherMesh1D"},
    {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE), "CcuAllReduceMesh1D"},
    {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), "CcuReduceScatterMesh1D"},
    {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALL), "CcuAlltoAllMesh1D"},
    {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALLV), "CcuAlltoAllVMesh1D"},
};

static void StubCleanup() {
    for (void* p : g_allocatedPtrs) { free(p); }
    g_allocatedPtrs.clear();
    g_stubRankSize = 8;
    g_stubRankId = 0;
    g_stubDeviceType = DevType::DEV_TYPE_950;
    unsetenv("HCCL_OP_EXPANSION_MODE");
}

static void SetCommEngineEnv(uint8_t commEngine) {
    if (commEngine == COMM_ENGINE_CCU) {
        setenv("HCCL_OP_EXPANSION_MODE", "CCU_MS", 1);
    } else if (commEngine == COMM_ENGINE_AICPU) {
        setenv("HCCL_OP_EXPANSION_MODE", "AI_CPU", 1);
    } else {
        unsetenv("HCCL_OP_EXPANSION_MODE");
    }
}

static TopoMeta BuildTopoMeta(uint32_t rankSize) {
    TopoMeta topoMeta;
    SuperPodMeta superPod;
    ServerMeta server;
    for (uint32_t i = 0; i < rankSize; ++i) {
        server.push_back(i);
    }
    superPod.push_back(server);
    topoMeta.push_back(superPod);
    return topoMeta;
}

struct Mc2TilingTestData {
    uint32_t version;
    uint32_t cnt;
    uint32_t offset[MAX_OP_NUM];
    Mc2ServerCfg serverCfg;
    Mc2CcTilingInner ccTiling[MAX_OP_NUM];
};

static Mc2TilingTestData BuildMc2Tiling(uint32_t tilingNum, uint8_t commEngine,
    const uint32_t opTypes[], const uint8_t srcDataTypes[]) {
    SetCommEngineEnv(commEngine);
    Mc2TilingTestData tiling{};
    tiling.version = MC2_TILING_VERSION;
    tiling.cnt = tilingNum;
    uint32_t baseOffset = offsetof(Mc2TilingTestData, ccTiling);
    for (uint32_t i = 0; i < MAX_OP_NUM; ++i) {
        tiling.offset[i] = baseOffset + i * sizeof(Mc2CcTilingInner);
    }
    for (uint32_t i = 0; i < tilingNum; ++i) {
        tiling.ccTiling[i].commEngine = commEngine;
        tiling.ccTiling[i].opType = opTypes[i];
        tiling.ccTiling[i].srcDataType = srcDataTypes ? srcDataTypes[i] : HCCL_DATA_TYPE_FP16;
        tiling.ccTiling[i].dstDataType = HCCL_DATA_TYPE_FP16;
        tiling.ccTiling[i].reduceType = HCCL_REDUCE_SUM;
        strcpy(tiling.ccTiling[i].groupName, "test_group");
        strcpy(tiling.ccTiling[i].algConfig, "default");
    }
    return tiling;
}

class CcuMc2TestSuite : public testing::Test {
protected:
    void SetUp() override {
        StubCleanup();
        TopoMeta topoMeta = BuildTopoMeta(g_stubRankSize);
        HcclSim::Sim_HcclCommInitClusterInfo(topoMeta, g_stubRankId, &comm_);
        int dummyStream = 1;
        stream_ = &dummyStream;
    }
    void TearDown() override {
        if (comm_ != nullptr) {
            auto simComm = static_cast<HcclSim::SimCommunicator*>(comm_);
            delete simComm;
            comm_ = nullptr;
        }
        StubCleanup();
    }
    HcclComm comm_ = nullptr;
    aclrtStream stream_ = nullptr;
};

TEST_F(CcuMc2TestSuite, AllocCcuOpResCtx_BasicFields) {
    OpResCtx resCtx{};
    u32 rankSize = 8;
    u32 userRank = 3;
    HcclResult ret = AllocCcuOpResCtx(comm_, "test_tag", rankSize, userRank, resCtx);
    EXPECT_EQ(ret, HCCL_SUCCESS);
    EXPECT_EQ(resCtx.rankId, userRank);
    EXPECT_EQ(resCtx.rankSize, rankSize);
    EXPECT_EQ(resCtx.workSpaceSize, Hccl::MC2_WORKSPACE_SIZE);
}

TEST_F(CcuMc2TestSuite, AllocCcuOpResCtx_ThreeAddrsDistinct) {
    OpResCtx resCtx{};
    EXPECT_EQ(AllocCcuOpResCtx(comm_, "tag", 4, 0, resCtx), HCCL_SUCCESS);
    EXPECT_NE(resCtx.workSpace, resCtx.xnAddr);
    EXPECT_NE(resCtx.workSpace, resCtx.ckeAddr);
    EXPECT_NE(resCtx.xnAddr, resCtx.ckeAddr);
}

TEST_F(CcuMc2TestSuite, CcuSelectAlg_AllGather) {
    SetCommEngineEnv(COMM_ENGINE_CCU);
    Mc2CcTilingInner ccTiling{};
    ccTiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER);
    ccTiling.commEngine = COMM_ENGINE_CCU;
    const void* ccTilingList[] = {&ccTiling};
    std::string topoTag[] = {"tag0"};
    OpResCtx resCtx{};
    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTag, ccTilingList, 1, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER));
    EXPECT_EQ(resCtx.algorithmType[0], static_cast<uint32_t>(CcuAllGatherMesh1D));
}

TEST_F(CcuMc2TestSuite, CcuSelectAlg_AllReduce) {
    SetCommEngineEnv(COMM_ENGINE_CCU);
    Mc2CcTilingInner ccTiling{};
    ccTiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE);
    ccTiling.commEngine = COMM_ENGINE_CCU;
    const void* ccTilingList[] = {&ccTiling};
    std::string topoTag[] = {"tag0"};
    OpResCtx resCtx{};
    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTag, ccTilingList, 1, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE));
    EXPECT_EQ(resCtx.algorithmType[0], static_cast<uint32_t>(CcuAllReduceMesh1D));
}

TEST_F(CcuMc2TestSuite, CcuSelectAlg_ReduceScatter) {
    SetCommEngineEnv(COMM_ENGINE_CCU);
    Mc2CcTilingInner ccTiling{};
    ccTiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
    ccTiling.commEngine = COMM_ENGINE_CCU;
    const void* ccTilingList[] = {&ccTiling};
    std::string topoTag[] = {"tag0"};
    OpResCtx resCtx{};
    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTag, ccTilingList, 1, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER));
    EXPECT_EQ(resCtx.algorithmType[0], static_cast<uint32_t>(CcuReduceScatterMesh1D));
}

TEST_F(CcuMc2TestSuite, CcuSelectAlg_AlltoAll) {
    SetCommEngineEnv(COMM_ENGINE_CCU);
    Mc2CcTilingInner ccTiling{};
    ccTiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALL);
    ccTiling.commEngine = COMM_ENGINE_CCU;
    const void* ccTilingList[] = {&ccTiling};
    std::string topoTag[] = {"tag0"};
    OpResCtx resCtx{};
    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTag, ccTilingList, 1, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALL));
    EXPECT_EQ(resCtx.algorithmType[0], static_cast<uint32_t>(CcuAlltoAllMesh1D));
}

TEST_F(CcuMc2TestSuite, CcuSelectAlg_MultipleTilings) {
    SetCommEngineEnv(COMM_ENGINE_CCU);
    Mc2CcTilingInner ccTiling0{};
    ccTiling0.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER);
    ccTiling0.commEngine = COMM_ENGINE_CCU;
    Mc2CcTilingInner ccTiling1{};
    ccTiling1.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE);
    ccTiling1.commEngine = COMM_ENGINE_CCU;
    const void* ccTilingList[] = {&ccTiling0, &ccTiling1};
    std::string topoTag[] = {"tag0", "tag1"};
    OpResCtx resCtx{};
    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTag, ccTilingList, 2, resCtx), HCCL_SUCCESS);
    EXPECT_EQ(resCtx.opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER));
    EXPECT_EQ(resCtx.algorithmType[0], static_cast<uint32_t>(CcuAllGatherMesh1D));
    EXPECT_EQ(resCtx.opType[1], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE));
    EXPECT_EQ(resCtx.algorithmType[1], static_cast<uint32_t>(CcuAllReduceMesh1D));
}

TEST_F(CcuMc2TestSuite, ObtainCommEngine_AllCcu) {
    Mc2CcTilingInner t0{}, t1{};
    t0.commEngine = COMM_ENGINE_CCU;
    t1.commEngine = COMM_ENGINE_CCU;
    const void* list[] = {&t0, &t1};
    uint8_t engine;
    EXPECT_EQ(ObtainCommEngine(list, 2, engine), HCCL_SUCCESS);
    EXPECT_EQ(engine, static_cast<uint8_t>(COMM_ENGINE_CCU));
}

TEST_F(CcuMc2TestSuite, ObtainCommEngine_AllAicpu) {
    Mc2CcTilingInner t0{}, t1{};
    t0.commEngine = COMM_ENGINE_AICPU;
    t1.commEngine = COMM_ENGINE_AICPU;
    const void* list[] = {&t0, &t1};
    uint8_t engine;
    EXPECT_EQ(ObtainCommEngine(list, 2, engine), HCCL_SUCCESS);
    EXPECT_EQ(engine, static_cast<uint8_t>(COMM_ENGINE_AICPU));
}

TEST_F(CcuMc2TestSuite, ObtainCommEngine_MixedFail) {
    Mc2CcTilingInner t0{}, t1{};
    t0.commEngine = COMM_ENGINE_CCU;
    t1.commEngine = COMM_ENGINE_AICPU;
    const void* list[] = {&t0, &t1};
    uint8_t engine;
    EXPECT_EQ(ObtainCommEngine(list, 2, engine), HCCL_E_NOT_SUPPORT);
}

TEST_F(CcuMc2TestSuite, ObtainCommEngine_UnsupportedEngine) {
    Mc2CcTilingInner t0{};
    t0.commEngine = COMM_ENGINE_AIV;
    const void* list[] = {&t0};
    uint8_t engine;
    EXPECT_EQ(ObtainCommEngine(list, 1, engine), HCCL_E_NOT_SUPPORT);
}

TEST_F(CcuMc2TestSuite, algorithmMap_AllEntries) {
    EXPECT_EQ(algorithmMap.at("CcuAllGatherMesh1D"), CcuAllGatherMesh1D);
    EXPECT_EQ(algorithmMap.at("CcuAllGatherMeshMem2Mem1D"), CcuAllGatherMeshMem2Mem1D);
    EXPECT_EQ(algorithmMap.at("CcuAllGatherMesh2D"), CcuAllGatherMesh2D);
    EXPECT_EQ(algorithmMap.at("CcuReduceScatterMesh1D"), CcuReduceScatterMesh1D);
    EXPECT_EQ(algorithmMap.at("CcuReduceScatterMeshMem2Mem1D"), CcuReduceScatterMeshMem2Mem1D);
    EXPECT_EQ(algorithmMap.at("CcuReduceScatterMesh2D"), CcuReduceScatterMesh2D);
    EXPECT_EQ(algorithmMap.at("CcuAllReduceMesh1D"), CcuAllReduceMesh1D);
    EXPECT_EQ(algorithmMap.at("CcuAllReduceMeshMem2Mem1D"), CcuAllReduceMeshMem2Mem1D);
    EXPECT_EQ(algorithmMap.at("CcuAllReduceMesh2DOneShot"), CcuAllReduceMesh2DOneShot);
    EXPECT_EQ(algorithmMap.at("CcuAlltoAllMesh1D"), CcuAlltoAllMesh1D);
    EXPECT_EQ(algorithmMap.at("CcuAlltoAllVMesh1D"), CcuAlltoAllVMesh1D);
    EXPECT_EQ(algorithmMap.at("CcuHalfAll2AllVMesh1D"), CcuHalfAll2AllVMesh1D);
    EXPECT_EQ(algorithmMap.size(), 12U);
}

TEST_F(CcuMc2TestSuite, AlgorithmType_EnumValues) {
    EXPECT_EQ(CcuAllGatherMesh1D, 0);
    EXPECT_EQ(CcuAllGatherMeshMem2Mem1D, 1);
    EXPECT_EQ(CcuAllGatherMesh2D, 2);
    EXPECT_EQ(CcuReduceScatterMesh1D, 50);
    EXPECT_EQ(CcuReduceScatterMeshMem2Mem1D, 51);
    EXPECT_EQ(CcuReduceScatterMesh2D, 52);
    EXPECT_EQ(CcuAllReduceMesh1D, 100);
    EXPECT_EQ(CcuAllReduceMeshMem2Mem1D, 101);
    EXPECT_EQ(CcuAllReduceMesh2DOneShot, 102);
    EXPECT_EQ(CcuAlltoAllMesh1D, 150);
    EXPECT_EQ(CcuAlltoAllVMesh1D, 151);
    EXPECT_EQ(CcuHalfAll2AllVMesh1D, 152);
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_CcuPath) {
    uint32_t opTypes[] = {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER)};
    Mc2TilingTestData tiling = BuildMc2Tiling(1, COMM_ENGINE_CCU, opTypes, nullptr);
    void* opResCtx = nullptr;
    HcclResult ret = HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx);
    EXPECT_EQ(ret, HCCL_SUCCESS);
    ASSERT_NE(opResCtx, nullptr);
    OpResCtx* ctx = static_cast<OpResCtx*>(opResCtx);
    EXPECT_EQ(ctx->rankId, g_stubRankId);
    EXPECT_EQ(ctx->rankSize, g_stubRankSize);
    EXPECT_EQ(ctx->workSpaceSize, Hccl::MC2_WORKSPACE_SIZE);
    EXPECT_NE(ctx->workSpace, 0U);
    EXPECT_NE(ctx->xnAddr, 0U);
    EXPECT_NE(ctx->ckeAddr, 0U);
    EXPECT_EQ(ctx->opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER));
    EXPECT_EQ(ctx->algorithmType[0], static_cast<uint32_t>(CcuAllGatherMesh1D));
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_AicpuPath) {
    uint32_t opTypes[] = {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER)};
    Mc2TilingTestData tiling = BuildMc2Tiling(1, COMM_ENGINE_AICPU, opTypes, nullptr);
    void* opResCtx = nullptr;
    HcclResult ret = HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx);
    EXPECT_EQ(ret, HCCL_SUCCESS);
    ASSERT_NE(opResCtx, nullptr);
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_MultipleOps) {
    uint32_t opTypes[] = {
        static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER),
        static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE)
    };
    Mc2TilingTestData tiling = BuildMc2Tiling(2, COMM_ENGINE_CCU, opTypes, nullptr);
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_SUCCESS);
    ASSERT_NE(opResCtx, nullptr);
    OpResCtx* ctx = static_cast<OpResCtx*>(opResCtx);
    EXPECT_EQ(ctx->opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER));
    EXPECT_EQ(ctx->opType[1], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE));
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_NullComm) {
    uint32_t opTypes[] = {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER)};
    Mc2TilingTestData tiling = BuildMc2Tiling(1, COMM_ENGINE_CCU, opTypes, nullptr);
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(nullptr, stream_, &tiling, &opResCtx), HCCL_E_PTR);
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_NullStream) {
    uint32_t opTypes[] = {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER)};
    Mc2TilingTestData tiling = BuildMc2Tiling(1, COMM_ENGINE_CCU, opTypes, nullptr);
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, nullptr, &tiling, &opResCtx), HCCL_E_PTR);
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_NullTiling) {
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, nullptr, &opResCtx), HCCL_E_PTR);
}

TEST_F(CcuMc2TestSuite, CcuSelectAlg_AicpuAlgNotSupported) {
    // tiling标记为CCU引擎，但环境变量设为AICPU模式，
    // 导致SelectAlgAndPrepareEngine选出AICPU算法名（如"InsAllGatherMesh"），
    // 该算法名不在CCU的algorithmMap中，CcuSelectAlg应返回HCCL_E_NOT_SUPPORT
    SetCommEngineEnv(COMM_ENGINE_AICPU);
    Mc2CcTilingInner ccTiling{};
    ccTiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER);
    ccTiling.commEngine = COMM_ENGINE_CCU;
    const void* ccTilingList[] = {&ccTiling};
    std::string topoTag[] = {"tag0"};
    OpResCtx resCtx{};
    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTag, ccTilingList, 1, resCtx), HCCL_E_NOT_SUPPORT);
}

TEST_F(CcuMc2TestSuite, HcclAllocComResourceByTiling_UnsupportedEngine) {
    uint32_t opTypes[] = {static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER)};
    Mc2TilingTestData tiling = BuildMc2Tiling(1, COMM_ENGINE_AIV, opTypes, nullptr);
    void* opResCtx = nullptr;
    EXPECT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &opResCtx), HCCL_E_NOT_SUPPORT);
}

TEST_F(CcuMc2TestSuite, GetCcuOpParamResCtx_AlltoAllMesh1D_KfcServerArgs) {
    SetCommEngineEnv(COMM_ENGINE_CCU);

    // 1. 分配CCU资源，获取opResCtx（含xnAddr、ckeAddr）
    OpResCtx opResCtx{};
    ASSERT_EQ(AllocCcuOpResCtx(comm_, "ctx_tag", g_stubRankSize, g_stubRankId, opResCtx), HCCL_SUCCESS);
    ASSERT_NE(opResCtx.xnAddr, 0U);
    ASSERT_NE(opResCtx.ckeAddr, 0U);

    // 2. 构造OpParam，模拟CcuSelectAlg中的准备流程
    Mc2CcTilingInner ccTiling{};
    ccTiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALL);
    ccTiling.commEngine = COMM_ENGINE_CCU;
    ccTiling.srcDataType = HCCL_DATA_TYPE_FP16;
    ccTiling.dstDataType = HCCL_DATA_TYPE_FP16;
    ccTiling.reduceType = HCCL_REDUCE_SUM;

    OpParam opParam{};
    ASSERT_EQ(InitOpParamByTiling(comm_, stream_, "alltoall_tag", &ccTiling, opParam), HCCL_SUCCESS);

    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    ASSERT_EQ(SelectAlgAndPrepareEngine(comm_, opParam, algName, topoInfo), HCCL_SUCCESS);
    ASSERT_EQ(algName, "CcuAlltoAllMesh1D");
    ASSERT_EQ(opParam.engine, COMM_ENGINE_CCU);

    int result = sprintf_s(opParam.algName, sizeof(opParam.algName), "%s", algName.c_str());
    ASSERT_GT(result, 0);

    bool skipGetRes = false;
    ASSERT_EQ(HandleSingleRankAndCommMode(comm_, opParam, skipGetRes), HCCL_SUCCESS);
    ASSERT_FALSE(skipGetRes);

    // 3. 调用GetCcuOpParamResCtx
    void *resCtxOut = nullptr;
    EXPECT_EQ(GetCcuOpParamResCtx(comm_, algName, opParam, topoInfo.get(), opResCtx, &resCtxOut), HCCL_SUCCESS);
    EXPECT_NE(resCtxOut, nullptr);
    EXPECT_EQ(opParam.resCtx, resCtxOut);

    // 4. 通过反序列化resCtxOut还原resourceCtx，验证kfcServerArgs各字段
    ASSERT_NE(opParam.ctxSize, 0U);
    auto *resCtxRaw = static_cast<char *>(resCtxOut);
    std::vector<char> seq(resCtxRaw, resCtxRaw + opParam.ctxSize);
    mc2_ops_hccl::AlgResourceCtxSerializable resourceCtx;
    resourceCtx.DeSerialize(seq);

    // kfcServerArgs应包含6个uint64_t元素（xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token）
    // 前填5字段在AcquireAlgResources中完成，kfcServerArgSize=6预留token位置
    // 第6字段token在GetAlgResCcu内的HcclAllocAlgResourceCcu中填充
    ASSERT_EQ(resourceCtx.kfcServerArgs.size(), 6U);
    EXPECT_EQ(resourceCtx.kfcServerArgSize, 6U);
    
    // xnAddr
    EXPECT_EQ(resourceCtx.kfcServerArgs[0], opResCtx.xnAddr);
    // ckeAddr
    EXPECT_EQ(resourceCtx.kfcServerArgs[1], opResCtx.ckeAddr);
    // dieNum = 1
    EXPECT_EQ(resourceCtx.kfcServerArgs[2], 1U);
    // missionNum = 1
    EXPECT_EQ(resourceCtx.kfcServerArgs[3], 1U);
    // missionIndex = 0
    EXPECT_EQ(resourceCtx.kfcServerArgs[4], 0U);
    // token (由GetTokenFromBuffInfo获取，模拟环境无实际buffer故为0)
    EXPECT_EQ(resourceCtx.kfcServerArgs[5], 0U);

    free(resCtxOut);
}
}
