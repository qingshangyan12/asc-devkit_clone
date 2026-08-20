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
#include <vector>
#define private public
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;
using namespace HcclApi;

namespace {
class HcclSuiteAICPU : public testing::Test {
protected:
    virtual void SetUp() { AscendC::SetGCoreType(1); }
    virtual void TearDown() { AscendC::SetGCoreType(0); }
};

constexpr uint32_t RANK_NUM = 4U;
constexpr size_t WORKSPACE_SIZE = sizeof(HcclMsgArea);
constexpr uint32_t REPEAT_TIME_3 = 3U;

OpResCtx GetOpResCtx(const vector<uint8_t>& workSpace)
{
    OpResCtx opResCtx{
        1, reinterpret_cast<uintptr_t>(workSpace.data()), WORKSPACE_SIZE, 0, RANK_NUM, {{100, 0x1000}, {200, 0x2000}},
    };
    return opResCtx;
}

static HcclMsgArea* GetHcclMsgArea(uint8_t* workspaceGM)
{
    uint64_t msgAddr = reinterpret_cast<uintptr_t>(workspaceGM);
    if (msgAddr & 0x1ff) {
        msgAddr = (msgAddr & (~((uint64_t)0x1ff))) + 0x200;
    }
    return reinterpret_cast<HcclMsgArea*>(msgAddr);
}

constexpr uint64_t GetCcuWorkspaceLayoutSize()
{
    return MAX_DCCI_CNT + sizeof(CommonPrepareParamCcu) * static_cast<uint64_t>(HCCL_MAX_HANDLE_ID) +
           sizeof(AlltoAllVParamCcu) * static_cast<uint64_t>(HCCL_MAX_HANDLE_ID) + CCU_MSG_EXT_MAX_OFFSET;
}

uint64_t GetCcuWorkspaceRequiredSize(uint64_t workspaceAddr)
{
    constexpr uint64_t workspaceAlign = 512U;
    const uint64_t alignedAddr = (workspaceAddr + workspaceAlign - 1U) & ~(workspaceAlign - 1U);
    return alignedAddr - workspaceAddr + GetCcuWorkspaceLayoutSize();
}

void InitCcuV2(Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU>& hccl, OpResCtx& context)
{
    Mc2InitTilingInner initTiling{};
    initTiling.version = INIT_TILING_CCU_NEW_VERSION;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&context), &initTiling);
}

TEST_F(HcclSuiteAICPU, InitCcuWorkspaceAcceptsExactCapacityWithAlignmentPadding)
{
    std::vector<uint8_t> workSpace(GetCcuWorkspaceLayoutSize() + 512U);
    OpResCtx context = GetOpResCtx(workSpace);
    context.workspace = reinterpret_cast<uint64_t>(workSpace.data() + 1U);
    context.workspaceSize = GetCcuWorkspaceRequiredSize(context.workspace);

    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    InitCcuV2(hccl, context);

    EXPECT_TRUE(hccl.impl_.isInited_);
    EXPECT_EQ(reinterpret_cast<uint64_t>(hccl.impl_.ccuParam_.ccuMsgExt) + CCU_MSG_EXT_MAX_OFFSET,
              context.workspace + context.workspaceSize);
}

TEST_F(HcclSuiteAICPU, InitCcuWorkspaceRejectsCapacityOneByteShort)
{
    std::vector<uint8_t> workSpace(GetCcuWorkspaceLayoutSize() + 512U);
    OpResCtx context = GetOpResCtx(workSpace);
    context.workspace = reinterpret_cast<uint64_t>(workSpace.data() + 1U);
    context.workspaceSize = GetCcuWorkspaceRequiredSize(context.workspace) - 1U;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    InitCcuV2(hccl, context);

    EXPECT_FALSE(hccl.impl_.isInited_);
}

TEST_F(HcclSuiteAICPU, InitCcuWorkspaceRejectsAddressAlignmentOverflow)
{
    std::vector<uint8_t> workSpace(1U);
    OpResCtx context = GetOpResCtx(workSpace);
    context.workspace = UINT64_MAX - 100U;
    context.workspaceSize = UINT64_MAX;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    InitCcuV2(hccl, context);

    EXPECT_FALSE(hccl.impl_.isInited_);
}

TEST_F(HcclSuiteAICPU, CcuSetCcTilingRejectsInvalidOpType)
{
    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    hccl.impl_.curVersion_ = HcclTilingVersion::NEW_TILING_VERSION;
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALL);

    EXPECT_EQ(hccl.SetCcTiling(reinterpret_cast<__gm__ void*>(&tiling)), HCCL_FAILED);
}

TEST_F(HcclSuiteAICPU, CcuSetCcTilingV2RejectsInvalidOpType)
{
    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALL);
    hccl.impl_.curVersion_ = HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION;
    hccl.impl_.tilingBaseAddr_ = reinterpret_cast<uint64_t>(&tiling);

    EXPECT_EQ(hccl.SetCcTilingV2(0U), HCCL_FAILED);
}

TEST_F(HcclSuiteAICPU, CcuCommonPrepareRejectsInvalidIndexInputs)
{
    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    CommonPrepareParam param{};
    param.commType.prepareType = HcclCMDType::HCCL_CMD_ALL;
    param.dataType = HCCL_DATA_TYPE_INT8;
    param.repeat = 1U;
    EXPECT_EQ(hccl.impl_.CommonPrepareImpl<false>(param), INVALID_HANDLE_ID);

    param.commType.prepareType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.dataType = static_cast<HcclDataType>(254);
    EXPECT_EQ(hccl.impl_.CommonPrepareImpl<false>(param), INVALID_HANDLE_ID);
}

TEST_F(HcclSuiteAICPU, CcuCommonPrepareRequiresRegisteredTiling)
{
    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    hccl.impl_.curVersion_ = HcclTilingVersion::NEW_TILING_VERSION;
    CommonPrepareParam param{};
    param.commType.prepareType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.dataType = HCCL_DATA_TYPE_INT8;
    param.repeat = 1U;

    EXPECT_EQ(hccl.impl_.CommonPrepareImpl<false>(param), INVALID_HANDLE_ID);
}

TEST_F(HcclSuiteAICPU, CcuAlltoAllVRankCapacityCoversDav3510Limit)
{
    EXPECT_FALSE(IsValidCcuRankNum(0U));
    EXPECT_TRUE(IsValidCcuRankNum(1U));
    EXPECT_TRUE(IsValidCcuRankNum(32U));
    EXPECT_TRUE(IsValidCcuRankNum(33U));
    EXPECT_TRUE(IsValidCcuRankNum(HCCL_CCU_MAX_RANK_NUM));
    EXPECT_FALSE(IsValidCcuRankNum(HCCL_CCU_MAX_RANK_NUM + 1U));
    EXPECT_EQ(sizeof(AlltoAllVParamCcu), sizeof(uint64_t) * HCCL_CCU_MAX_RANK_NUM * 4U);
}

TEST_F(HcclSuiteAICPU, CcuAlltoAllVRejectsRankBeyondDav3510Limit)
{
    Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU> hccl;
    OpResCtx context{};
    context.rankSize = HCCL_CCU_MAX_RANK_NUM + 1U;
    hccl.impl_.newCcuFlag_ = true;
    hccl.impl_.hcclNewContext_ = &context;

    CommonPrepareParam param{};
    param.commType.prepareType = HcclCMDType::HCCL_CMD_ALLTOALLV;
    param.dataType = HCCL_DATA_TYPE_INT8;
    param.repeat = 1U;
    const HcclHandle initialHandleId = hccl.impl_.curHandleId_;

    EXPECT_EQ(hccl.impl_.CommonPrepareImpl<false>(param), INVALID_HANDLE_ID);
    EXPECT_EQ(hccl.impl_.curHandleId_, initialHandleId);
}

class Mc2InitTilingAicpuTest {
    uint32_t version = 0U;
    uint32_t mc2HcommCnt = 0U;
    uint32_t offset[MAX_CC_TILING_NUM] = {0U};
    uint8_t debugMode = 0U;
    uint8_t preparePosition = 0U;
    uint16_t queueNum = 0U;
    uint16_t commBlockNum = 0U;
    uint8_t devType = 0U;
    char reserved[17] = {0U};
};

struct Mc2CcTilingAicpuTest {
    uint8_t skipLocalRankCopy;
    uint8_t skipBufferWindowCopy;
    uint8_t stepSize = 1U;
    uint8_t version;
    char reserved[12];
    char groupName[128];
    char algConfig[128];
    uint32_t opType;
    uint32_t reduceType;
};

class Mc2TilingAicpuTest {
    Mc2InitTilingAicpuTest init;
    Mc2CcTilingAicpuTest cc;
};

TEST_F(HcclSuiteAICPU, AllGather_repeat1_prepare1_commit1_wait1_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLGATHER;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    EXPECT_EQ(hccl.GetRankDim(), 4);
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.AllGather(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, 0, 1);
    EXPECT_EQ(handleId, 0);
    hccl.Commit(handleId);
    EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
}

TEST_F(HcclSuiteAICPU, AllGather_repeat3_prepare1_commit0_wait3_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLGATHER;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.AllGather<true>(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, 0, 3);
    EXPECT_EQ(handleId, 0);
    for (uint8_t i = 0; i < REPEAT_TIME_3; i++) {
        EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
    }
}

TEST_F(HcclSuiteAICPU, AllReduce_repeat1_prepare1_commit1_wait1_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLREDUCE;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.AllReduce(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, HcclReduceOp::HCCL_REDUCE_SUM, 1);
    hccl.Commit(handleId);
    EXPECT_EQ(handleId, 0);
    EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
}

TEST_F(HcclSuiteAICPU, AllReduce_repeat3_prepare1_commit0_wait3_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLREDUCE;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.AllReduce<true>(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, HcclReduceOp::HCCL_REDUCE_SUM, 3);
    EXPECT_EQ(handleId, 0);
    for (uint8_t i = 0; i < REPEAT_TIME_3; i++) {
        EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
    }
}

TEST_F(HcclSuiteAICPU, ReduceScatter_repeat1_prepare1_commit1_wait1_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_REDUCE_SCATTER;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.ReduceScatter(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, HcclReduceOp::HCCL_REDUCE_SUM, 0, 1);
    hccl.Commit(handleId);
    EXPECT_EQ(handleId, 0);
    EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
}

TEST_F(HcclSuiteAICPU, ReduceScatter_repeat3_prepare1_commit0_wait3_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_REDUCE_SCATTER;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.ReduceScatter<true>(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, HcclReduceOp::HCCL_REDUCE_SUM, 0, 3);
    EXPECT_EQ(handleId, 0);
    for (uint8_t i = 0; i < REPEAT_TIME_3; i++) {
        EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
    }
}

TEST_F(HcclSuiteAICPU, AlltoAll_repeat1_prepare1_commit1_wait1_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLTOALL;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.AlltoAll(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, 0, 1);
    hccl.Commit(handleId);
    EXPECT_EQ(handleId, 0);
    EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
}

TEST_F(HcclSuiteAICPU, AlltoAll_repeat3_prepare1_commit0_wait3_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLTOALL;

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    HcclHandle handleId = hccl.AlltoAll<true>(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), reinterpret_cast<__gm__ uint8_t*>(0x4321), 100,
        HcclDataType::HCCL_DATA_TYPE_INT8, 0, 3);
    EXPECT_EQ(handleId, 0);
    for (uint8_t i = 0; i < REPEAT_TIME_3; i++) {
        EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
    }
}

TEST_F(HcclSuiteAICPU, AlltoAllV_repeat1_prepare1_commit1_wait1_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLTOALLV;

    uint64_t sendCounts[RANK_NUM] = {0};
    uint64_t sDisplacements[RANK_NUM] = {0};
    uint64_t recvCounts[RANK_NUM] = {0};
    uint64_t rDisplacements[RANK_NUM] = {0};

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    if (hccl.GetRankId() == 0) {
        sendCounts[0] = 3;
        sendCounts[1] = 3;
        sendCounts[2] = 3;
        sendCounts[3] = 3;
        sDisplacements[1] = 3;
        sDisplacements[2] = 6;
        sDisplacements[3] = 9;
        recvCounts[0] = 3;
        recvCounts[1] = 2;
        recvCounts[2] = 1;
        recvCounts[3] = 3;
        rDisplacements[1] = 3;
        rDisplacements[2] = 5;
        rDisplacements[3] = 6;
    }
    HcclHandle handleId = hccl.AlltoAllV(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), sendCounts, sDisplacements, HcclDataType::HCCL_DATA_TYPE_INT8,
        reinterpret_cast<__gm__ uint8_t*>(0x4321), recvCounts, rDisplacements, HcclDataType::HCCL_DATA_TYPE_INT8);
    hccl.Commit(handleId);
    EXPECT_EQ(handleId, 0);
    EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
}

TEST_F(HcclSuiteAICPU, AlltoAllV_repeat3_prepare1_commit0_wait3_success)
{
    std::vector<uint8_t> workSpace(WORKSPACE_SIZE + 1024);
    HcclMsgArea* hcclMsgArea = GetHcclMsgArea(workSpace.data());
    OpResCtx opResCtx = GetOpResCtx(workSpace);
    Mc2TilingAicpuTest tilingData;
    tilingData.cc.opType = (uint32_t)HcclCMDType::HCCL_CMD_ALLTOALLV;

    uint32_t sendCounts[RANK_NUM] = {0};
    uint32_t sDisplacements[RANK_NUM] = {0};
    uint32_t recvCounts[RANK_NUM] = {0};
    uint32_t rDisplacements[RANK_NUM] = {0};

    Hccl<HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2(reinterpret_cast<GM_ADDR>(&opResCtx), static_cast<const void*>(&tilingData));
    auto ret = hccl.SetCcTilingV2(sizeof(Mc2InitTilingAicpuTest));
    EXPECT_EQ(ret, 0);
    if (hccl.GetRankId() == 0) {
        sendCounts[0] = 3;
        sendCounts[1] = 3;
        sendCounts[2] = 3;
        sendCounts[3] = 3;
        sDisplacements[1] = 3;
        sDisplacements[2] = 6;
        sDisplacements[3] = 9;
        recvCounts[0] = 3;
        recvCounts[1] = 2;
        recvCounts[2] = 1;
        recvCounts[3] = 3;
        rDisplacements[1] = 3;
        rDisplacements[2] = 5;
        rDisplacements[3] = 6;
    }
    HcclHandle handleId = hccl.AlltoAllV<true>(
        reinterpret_cast<__gm__ uint8_t*>(0x1234), sendCounts, sDisplacements, HcclDataType::HCCL_DATA_TYPE_INT8,
        reinterpret_cast<__gm__ uint8_t*>(0x4321), recvCounts, rDisplacements, HcclDataType::HCCL_DATA_TYPE_INT8, 3);
    EXPECT_EQ(handleId, 0);
    for (uint8_t i = 0; i < REPEAT_TIME_3; i++) {
        EXPECT_EQ(hccl.Wait(handleId), HCCL_SUCCESS);
    }
}
} // namespace
