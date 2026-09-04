/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstring>
#include <limits>
#include "mc2_replay_gate_host.h"

#include "gtest/gtest.h"

#include "sim_communicator.h"
#include "sim_thread.h"
#include "sim_world.h"
#include "hccl_alloc_ctx_res.h"
#include "op_common.h"
#include "task_cache_test_stub.h"

namespace mc2_ops_hccl {
extern bool g_rejectDirectAclrtMemcpy;
extern uint32_t g_aclrtMemcpyCallCount;
extern uint32_t g_hcclEngineCtxCopyCallCount;
} // namespace mc2_ops_hccl

namespace checker {
namespace {
using namespace HcclSim;
using namespace mc2_ops_hccl;

class AicpuAllocCtxResTest : public testing::Test {
protected:
    void SetUp() override
    {
        const TopoMeta topoMeta{{{0U}}};
        SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
        ASSERT_EQ(Sim_HcclCommInitClusterInfo(topoMeta, 0U, &comm_), HCCL_SUCCESS);
        ASSERT_NE(comm_, nullptr);
        g_rejectDirectAclrtMemcpy = false;
        g_aclrtMemcpyCallCount = 0U;
        g_hcclEngineCtxCopyCallCount = 0U;
        ResetMc2TaskCacheTestStubState();
        GetMc2TaskCacheTestStubState().emulatePooledThreads = true;
    }

    void TearDown() override
    {
        g_rejectDirectAclrtMemcpy = false;
        g_aclrtMemcpyCallCount = 0U;
        g_hcclEngineCtxCopyCallCount = 0U;
        ResetMc2TaskCacheTestStubState();
        if (comm_ != nullptr) {
            EXPECT_EQ(HcclCommDestroy(comm_), HCCL_SUCCESS);
            comm_ = nullptr;
        }
        SimWorld::Global()->Deinit();
    }

    HcclComm comm_ = nullptr;
};

TEST_F(AicpuAllocCtxResTest, CaptureSafeCopyCoversOpParamAndOpResCtx)
{
    constexpr uint32_t tilingOffset = 64U;
    Mc2InitTilingInner initTiling{};
    initTiling.offset[0] = tilingOffset;

    Mc2CcTilingInner ccTiling{};
    ccTiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::AICPU_TS);
    const void* ccTilingList[MAX_CC_TILING_NUM] = {&ccTiling};

    OpParam opParam{};
    opParam.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    const std::vector<OpParam> opParams{opParam};

    g_rejectDirectAclrtMemcpy = true;
    void* opResCtxPtr = nullptr;
    ASSERT_EQ(
        HcclAllocOpResCtx(comm_, "aicpu_capture_safe_copy", opParams, &initTiling, ccTilingList, &opResCtxPtr),
        HCCL_SUCCESS);

    EXPECT_EQ(g_aclrtMemcpyCallCount, 0U);
    EXPECT_EQ(g_hcclEngineCtxCopyCallCount, 2U);
    ASSERT_NE(opResCtxPtr, nullptr);

    const auto* opResCtx = static_cast<const OpResCtx*>(opResCtxPtr);
    EXPECT_EQ(opResCtx->rankId, 0U);
    EXPECT_EQ(opResCtx->rankSize, 1U);
    EXPECT_EQ(opResCtx->algInfo[0].offset, tilingOffset);
    ASSERT_NE(opResCtx->algInfo[0].opParam, 0U);

    const auto* copiedOpParam = reinterpret_cast<const OpParam*>(opResCtx->algInfo[0].opParam);
    EXPECT_EQ(copiedOpParam->opType, HcclCMDType::HCCL_CMD_ALLGATHER);
}


OpParam GateParam(const char* tag = "allreduce-pooled-gate-ut")
{
    OpParam param{};
    param.opType = HCCL_CMD_ALLREDUCE;
    param.engine = COMM_ENGINE_AICPU_TS;
    param.opMode = OpMode::OPBASE;
    std::strncpy(param.algTag, tag, sizeof(param.algTag) - 1U);
    return param;
}

AlgResourceRequest GateRequest()
{
    AlgResourceRequest request;
    request.notifyNumOnMainThread = 2U;
    request.slaveThreadNum = 2U;
    request.notifyNumPerThread = {4U, 3U};
    return request;
}

HcclResult AcquireGate(HcclComm comm, OpParam& param, AlgResourceRequest& request,
    std::unique_ptr<AlgResourceCtxSerializable>& resCtx)
{
    resCtx->notifyNumOnMainThread = request.notifyNumOnMainThread;
    resCtx->notifyNumPerThread = request.notifyNumPerThread;
    resCtx->slaveThreadNum = request.slaveThreadNum;
    return HcclGetThread(comm, param, request, resCtx, &param.deviceOrderThread);
}

TEST_F(AicpuAllocCtxResTest, AllReduceUsesOnePooledPrefixAndReservesGateNotify)
{
    auto param = GateParam();
    auto request = GateRequest();
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    ASSERT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_SUCCESS);
    const auto& state = GetMc2TaskCacheTestStubState();
    EXPECT_EQ(state.aicpuThreadAcquireCalls, 1U);
    EXPECT_EQ(state.lastThreadNum, 4U);
    EXPECT_EQ(state.lastThreadNotifyNum, 6U);
    ASSERT_EQ(resCtx->threads.size(), 3U);
    EXPECT_EQ(param.deviceOrderThread, state.pooledThreads[3]);
    EXPECT_NE(param.deviceOrderThread, resCtx->unfoldThread);
    for (ThreadHandle thread : resCtx->threads) {
        EXPECT_NE(thread, param.deviceOrderThread);
        EXPECT_EQ(reinterpret_cast<SimHcclThread*>(thread)->GetNotifyNum(), 6U);
    }
    ThreadHandle main = 0;
    uint32_t notifyNum = 0;
    ASSERT_EQ(GetMainThreadInfo(comm_, param, main, notifyNum), HCCL_SUCCESS);
    EXPECT_EQ(main, resCtx->threads.front());
    EXPECT_EQ(notifyNum, 5U);
}

TEST_F(AicpuAllocCtxResTest, DifferentAlgorithmsReusePoolWithoutWithinOperationAliasing)
{
    auto first = GateParam("small-alg");
    auto request = GateRequest();
    auto firstCtx = std::make_unique<AlgResourceCtxSerializable>();
    ASSERT_EQ(AcquireGate(comm_, first, request, firstCtx), HCCL_SUCCESS);
    auto second = GateParam("large-alg");
    request.slaveThreadNum = 4;
    request.notifyNumPerThread = {4, 4, 4, 4};
    auto secondCtx = std::make_unique<AlgResourceCtxSerializable>();
    ASSERT_EQ(AcquireGate(comm_, second, request, secondCtx), HCCL_SUCCESS);
    EXPECT_EQ(secondCtx->threads[0], firstCtx->threads[0]);
    EXPECT_EQ(secondCtx->threads[3], first.deviceOrderThread);
    EXPECT_NE(second.deviceOrderThread, first.deviceOrderThread);
    EXPECT_EQ(second.deviceOrderThread, GetMc2TaskCacheTestStubState().pooledThreads[5]);
    for (auto thread : secondCtx->threads) {
        EXPECT_NE(thread, second.deviceOrderThread);
    }
}

TEST_F(AicpuAllocCtxResTest, ReuseRestoresGateBeforeHcclGetAlgResEarlyReturn)
{
    auto param = GateParam();
    auto request = GateRequest();
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    ASSERT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_SUCCESS);
    void* resource = nullptr;
    ASSERT_EQ(HcclEngineCtxCreate(comm_, param.algTag, COMM_ENGINE_AICPU_TS, 64, &resource), HCCL_SUCCESS);
    ASSERT_EQ(PublishMc2ReplayGateHostContext(comm_, param, *resCtx, resource, 64), HCCL_SUCCESS);
    const auto release = param.deviceOrderThread;
    param.deviceOrderThread = 0;
    std::unique_ptr<InsCollAlgBase> executor;
    TopoInfoWithNetLayerDetails topo{};
    bool reused = false;
    void* restoredResource = nullptr;
    ASSERT_EQ(HcclGetAlgRes(comm_, param, executor, &topo, resCtx, &restoredResource, reused), HCCL_SUCCESS);
    EXPECT_TRUE(reused);
    EXPECT_EQ(restoredResource, resource);
    EXPECT_EQ(param.deviceOrderThread, release);
    EXPECT_EQ(GetMc2TaskCacheTestStubState().aicpuThreadAcquireCalls, 1U);
}

TEST_F(AicpuAllocCtxResTest, MissingSidecarRejectsResourceReuse)
{
    auto param = GateParam();
    void* resource = nullptr;
    ASSERT_EQ(HcclEngineCtxCreate(comm_, param.algTag, COMM_ENGINE_AICPU_TS, 64, &resource), HCCL_SUCCESS);
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    std::unique_ptr<InsCollAlgBase> executor;
    TopoInfoWithNetLayerDetails topo{};
    bool reused = false;
    void* restored = nullptr;
    EXPECT_EQ(HcclGetAlgRes(comm_, param, executor, &topo, resCtx, &restored, reused), HCCL_E_INTERNAL);
    EXPECT_FALSE(reused);
    EXPECT_EQ(param.deviceOrderThread, 0U);
}

TEST_F(AicpuAllocCtxResTest, InvalidSidecarRejectsReuse)
{
    auto param = GateParam();
    auto request = GateRequest();
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    ASSERT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_SUCCESS);
    void* resource = nullptr;
    ASSERT_EQ(HcclEngineCtxCreate(comm_, param.algTag, COMM_ENGINE_AICPU_TS, 64, &resource), HCCL_SUCCESS);
    ASSERT_EQ(PublishMc2ReplayGateHostContext(comm_, param, *resCtx, resource, 64), HCCL_SUCCESS);
    std::string tag;
    ASSERT_EQ(Mc2ReplayGateContextTag(param, tag), HCCL_SUCCESS);
    void* storage = nullptr;
    uint64_t size = 0;
    ASSERT_EQ(HcclEngineCtxGet(comm_, tag.c_str(), COMM_ENGINE_CPU_TS, &storage, &size), HCCL_SUCCESS);
    auto* record = static_cast<Mc2ReplayGateHostContext*>(storage);
    const auto original = *record;
    for (unsigned fault = 0; fault < 7; ++fault) {
        *record = original;
        switch (fault) {
            case 0: record->version++; break;
            case 1: record->valid = 0; break;
            case 2: record->releaseThread = 0; break;
            case 3: record->resourceAddress++; break;
            case 4: record->resourceSize++; break;
            case 5: record->mainThread++; break;
            case 6: record->notifyIdx++; break;
        }
        EXPECT_EQ(RestoreMc2ReplayGateHostContext(comm_, param, resource, 64), HCCL_E_INTERNAL);
        EXPECT_EQ(param.deviceOrderThread, 0U);
    }
    *record = original;
    EXPECT_EQ(RestoreMc2ReplayGateHostContext(comm_, param, resource, 64), HCCL_SUCCESS);
    EXPECT_EQ(param.deviceOrderThread, original.releaseThread);
}

TEST_F(AicpuAllocCtxResTest, AcquisitionFailureDoesNotDisableGateSilently)
{
    auto param = GateParam();
    auto request = GateRequest();
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    GetMc2TaskCacheTestStubState().threadAcquireRet = HCCL_E_UNAVAIL;
    EXPECT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_E_UNAVAIL);
    EXPECT_EQ(param.deviceOrderThread, 0U);
    EXPECT_TRUE(resCtx->threads.empty());
}

TEST_F(AicpuAllocCtxResTest, InvalidHandlesAreRejectedBeforePublication)
{
    for (unsigned fault = 0; fault < 3; ++fault) {
        auto& state = GetMc2TaskCacheTestStubState();
        state.returnNullThread = fault == 0;
        state.returnDuplicateThread = fault == 1;
        state.aliasUnfoldThread = fault == 2;
        auto param = GateParam();
        auto request = GateRequest();
        auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
        EXPECT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_E_INTERNAL);
        EXPECT_EQ(param.deviceOrderThread, 0U);
        EXPECT_TRUE(resCtx->threads.empty());
    }
}

TEST_F(AicpuAllocCtxResTest, OverflowIsRejectedBeforeAnyThreadAllocation)
{
    auto param = GateParam();
    auto request = GateRequest();
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    request.slaveThreadNum = std::numeric_limits<uint32_t>::max() - 1U;
    EXPECT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_E_PARA);
    request = GateRequest();
    request.notifyNumOnMainThread = std::numeric_limits<uint32_t>::max() - 1U;
    EXPECT_EQ(AcquireGate(comm_, param, request, resCtx), HCCL_E_PARA);
    EXPECT_EQ(GetMc2TaskCacheTestStubState().aicpuThreadAcquireCalls, 0U);
}

TEST_F(AicpuAllocCtxResTest, NonAllReduceKeepsOneFrameworkNotify)
{
    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.engine = CommEngine::COMM_ENGINE_AICPU_TS;
    param.deviceOrderThread = static_cast<ThreadHandle>(0xdeadU);
    std::strncpy(param.algTag, "allgather-notify-ut", sizeof(param.algTag) - 1U);

    AlgResourceRequest request;
    request.notifyNumOnMainThread = 2U;
    request.slaveThreadNum = 1U;
    request.notifyNumPerThread = {4U};
    auto resCtx = std::make_unique<AlgResourceCtxSerializable>();
    ASSERT_EQ(HcclGetThread(comm_, param, request, resCtx), HCCL_SUCCESS);
    ASSERT_EQ(resCtx->threads.size(), 2U);
    for (ThreadHandle thread : resCtx->threads) {
        auto* simThread = reinterpret_cast<SimHcclThread*>(thread);
        ASSERT_NE(simThread, nullptr);
        EXPECT_EQ(simThread->GetNotifyNum(), 5U);
    }
}

} // namespace
} // namespace checker
