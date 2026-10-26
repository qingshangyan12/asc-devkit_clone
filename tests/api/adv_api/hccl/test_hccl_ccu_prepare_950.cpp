/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>
#define private public
#include "kernel_operator.h"
#undef private

using namespace AscendC;
using namespace HcclApi;

namespace {
class PeerOnlyPrepareTest : public ::testing::Test {
protected:
    using Impl = HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, DEFAULT_CFG>;
    Impl impl;
    CommonPrepareParamCcu handles[2]{};
    HcclCombineOpParam context{};
    Mc2CcTilingInner tiling{};
    void SetUp() override
    {
        context.rankNum = 2;
        context.rankId = 1;
        impl.newCcuFlag_ = false;
        impl.hcclContext_ = &context;
        impl.handleParamGM_ = handles;
        impl.curVersion_ = HcclTilingVersion::NEW_TILING_VERSION;
        tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        tiling.srcDataType = static_cast<uint8_t>(HCCL_DATA_TYPE_INT8);
        tiling.dstDataType = tiling.srcDataType;
        tiling.reduceType = static_cast<uint32_t>(HCCL_REDUCE_MAX);
        impl.ccOpTilingDataTable_[tiling.opType] = reinterpret_cast<uint64_t>(&tiling);
        for (auto& handle : handles) {
            handle.commType.prepareType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
            handle.sendBuf = reinterpret_cast<GM_ADDR>(0x100000);
            handle.recvBuf = reinterpret_cast<GM_ADDR>(0x200000);
            handle.count = 257;
            handle.strideCount = 1024;
        }
    }
};

TEST_F(PeerOnlyPrepareTest, SnapshotSurvivesTilingChangeAndRepeat)
{
    tiling.skipLocalRankCopy = 1;
    impl.InitCcuParam(0);
    tiling.skipLocalRankCopy = 0;
    impl.InitCcuParam(1);
    EXPECT_EQ(handles[0].skipLocalRankCopy, 1);
    EXPECT_EQ(handles[1].skipLocalRankCopy, 0);
    for (uint32_t repeat = 0; repeat < 3; ++repeat) {
        impl.ccuParam_.repeatIndex = repeat;
        for (uint32_t handle = 0; handle < 2; ++handle) {
            impl.CcuPrepareForReduceScatterPeerOnlyM2M(&handles[handle]);
            EXPECT_EQ(impl.xnData_[1], 0x100000U + repeat * 257U);
            EXPECT_EQ(impl.xnData_[2], 0x200000U + repeat * 257U);
            EXPECT_EQ(impl.xnData_[4], 1024U);
            EXPECT_EQ(impl.xnData_[5], handle == 0 ? 1U : 0U);
            EXPECT_EQ(impl.xnData_[7], 257U);
            EXPECT_EQ(impl.xnData_[8], UINT64_MAX - 1);
        }
    }
}

TEST_F(PeerOnlyPrepareTest, MissingTilingResetsToSafeDefault)
{
    handles[0].skipLocalRankCopy = 1;
    impl.curVersion_ = HcclTilingVersion::DEPRECATED_TILING_VERSION;
    impl.InitCcuParam(0);
    EXPECT_EQ(handles[0].skipLocalRankCopy, 0);
}

TEST_F(PeerOnlyPrepareTest, OnlineTilingUsesRelativeOffset)
{
    struct TilingBlock {
        Mc2InitTilingInner init{};
        Mc2CcTilingInner cc{};
    } block;
    block.cc = tiling;
    block.cc.skipLocalRankCopy = 1;
    impl.curVersion_ = HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION;
    impl.tilingBaseAddr_ = reinterpret_cast<uint64_t>(&block);
    impl.ccOpTilingDataTable_[tiling.opType] = offsetof(TilingBlock, cc);
    impl.InitCcuParam(0);
    EXPECT_EQ(handles[0].skipLocalRankCopy, 1);
}

TEST_F(PeerOnlyPrepareTest, RankStrideAndChunkBoundaries)
{
    const uint64_t chunk = 256ULL * 1024 * 1024;
    for (uint32_t rank = 0; rank < 2; ++rank) {
        context.rankId = rank;
        for (uint64_t count : std::vector<uint64_t>{0, chunk, chunk + 128, 2 * chunk}) {
            handles[0].count = count;
            handles[0].strideCount = 0;
            impl.InitCcuParam(0);
            impl.ccuParam_.repeatIndex = 0;
            impl.CcuPrepareForReduceScatterPeerOnlyM2M(&handles[0]);
            EXPECT_EQ(impl.xnData_[4], rank * count);
            EXPECT_EQ(impl.xnData_[5], 0U);
            const uint64_t chunks = count == 0 ? 0 : (count - 1) / chunk + 1;
            EXPECT_EQ(impl.xnData_[8], UINT64_MAX - chunks);
            EXPECT_EQ(impl.xnData_[7], chunks == 0 ? 0 : count - (chunks - 1) * chunk);
        }
    }
}

TEST_F(PeerOnlyPrepareTest, WorkspaceLayoutUnchanged)
{
    EXPECT_EQ(offsetof(CommonPrepareParamCcu, paramExt), offsetof(CommonPrepareParam, paramExt));
    EXPECT_EQ(offsetof(CommonPrepareParamCcu, wParamExt), offsetof(CommonPrepareParam, wParamExt));
    EXPECT_EQ(offsetof(CommonPrepareParamCcu, skipLocalRankCopy), offsetof(CommonPrepareParamCcu, repeat) + 1);
    EXPECT_EQ(sizeof(CommonPrepareParamCcu) % 64, 0U);
}

// concur AG/RS prepare 影子读路径：走 CommonPrepareImpl→InitCcuParam→CcuPrepareForOp 真实链路，
// 验证 tiling 覆盖值（dtype/reduce）进入影子并被算法 prepare 消费
class ConcurPrepareTest : public ::testing::Test {
protected:
    using Impl = HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, DEFAULT_CFG>;
    Impl impl;
    CommonPrepareParamCcu handles[1]{};
    HcclApi::OpResCtx opResCtx{};
    Mc2CcTilingInner tiling{};

    void SetUp() override
    {
        impl.newCcuFlag_ = true;
        impl.hcclNewContext_ = &opResCtx;
        impl.handleParamGM_ = handles;
        impl.isInited_ = true;
        impl.workingFlag_ = true;
        impl.curHandleId_ = 0;
        impl.curVersion_ = HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION;
        impl.tilingBaseAddr_ = 0;
        opResCtx.rankSize = 8;
        opResCtx.rankId = 1;
        opResCtx.res[0] = 0x300000; // RS concur 的 scratch
        tiling.srcDataType = static_cast<uint8_t>(HCCL_DATA_TYPE_FP16);
        tiling.dstDataType = tiling.srcDataType;
        tiling.reduceType = static_cast<uint32_t>(HCCL_REDUCE_SUM);
        impl.ccOpTilingDataTable_[static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER)] =
            reinterpret_cast<uint64_t>(&tiling);
        impl.ccOpTilingDataTable_[static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER)] =
            reinterpret_cast<uint64_t>(&tiling);
    }

    CommonPrepareParam MakeParam(HcclCMDType type, uint64_t count)
    {
        CommonPrepareParam param{};
        param.commType.prepareType = type;
        param.sendBuf = reinterpret_cast<GM_ADDR>(0x100000);
        param.recvBuf = reinterpret_cast<GM_ADDR>(0x200000);
        param.count = count;
        param.dataType = HCCL_DATA_TYPE_INT8; // API 值，须被 tiling 的 FP16 覆盖
        param.dstDataType = HCCL_DATA_TYPE_INT8;
        param.op = HCCL_REDUCE_SUM;
        param.strideCount = 0;
        param.repeat = 1U;
        return param;
    }
};

TEST_F(ConcurPrepareTest, ConcurrentAllGatherUsesShadowWithTilingOverride)
{
    opResCtx.opType[0] = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER);
    opResCtx.algorithmType[0] = static_cast<uint32_t>(AlgorithmType::CcuSchedAllGatherConcurMeshNHRMultiLink);

    const HcclHandle handle = impl.CommonPrepareImpl<false>(MakeParam(HcclCMDType::HCCL_CMD_ALLGATHER, 1000));
    ASSERT_EQ(handle, 0);
    // tiling 覆盖须同时落到 GM 与影子（FP16=2 字节决定后续全部切分）
    EXPECT_EQ(handles[0].dataType, HCCL_DATA_TYPE_FP16);
    EXPECT_EQ(impl.handleParamLocal_[0].dataType, HCCL_DATA_TYPE_FP16);

    impl.ccuParam_.repeatIndex = 0;
    impl.CcuPrepareForOp(0);

    // FP16: totalSize=2000, mesh=(2000*11/21/128)*128=1024, nhr=976, perJetty=128, last=592
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_MESH_INPUT], 0x100000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_MESH_OUTPUT], 0x200000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET], 2000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_MESH_SLICE_SIZE], 1024U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_NHR_INPUT], 0x100000U + 1024U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_NHR_OUTPUT], 0x200000U + 1024U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_NHR_SLICE_SIZE], 976U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_NHR_SLICE_SIZE_PER_JETTY], 128U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_NHR_LAST_SLICE_SIZE_PER_JETTY], 592U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_NHR_INPUT_OUTPUT_EQUAL], 0U);
    const uint64_t opId = impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_AG_OP_ID];
    EXPECT_NE(opId, 0xffffffffffffffffU);
    EXPECT_EQ(opId & AscendC::COMM_TYPE_MASK, static_cast<uint64_t>(HcclCMDType::HCCL_CMD_ALLGATHER));
    EXPECT_EQ((opId >> AscendC::DATA_TYPE_SHIFT) & AscendC::DATA_TYPE_MASK, static_cast<uint64_t>(HCCL_DATA_TYPE_FP16));
    EXPECT_EQ(impl.ccuUsedXnNum_, HcclKfcProtocol::KFC_CONCURRENT_AG_PARAM_NUM);
    EXPECT_EQ(impl.handleMissionNum_[0], 2U);
}

TEST_F(ConcurPrepareTest, ConcurrentReduceScatterUsesShadowWithTilingOverride)
{
    opResCtx.opType[0] = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
    opResCtx.algorithmType[0] = static_cast<uint32_t>(AlgorithmType::CcuSchedReduceScatterConcurMeshNHRMultiLink);

    const HcclHandle handle = impl.CommonPrepareImpl<false>(MakeParam(HcclCMDType::HCCL_CMD_REDUCE_SCATTER, 1000));
    ASSERT_EQ(handle, 0);
    EXPECT_EQ(impl.handleParamLocal_[0].dataType, HCCL_DATA_TYPE_FP16);

    impl.ccuParam_.repeatIndex = 0;
    impl.CcuPrepareForOp(0);

    // FP16: sliceSize=2000, mesh=(2000*10/22/128)*128=896, nhr=1104; chunk=64M/8, 单 chunk 尾块=896
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_INPUT], 0x100000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_OUTPUT], 0x200000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_SCRATCH], 0x300000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_RANK_SLICE_OFFSET], 2000U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_CHUNK_SIZE], 8388608U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_CHUNK_LOOP_NUM], UINT64_MAX - 1U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_MESH_TAIL_SIZE], 896U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_NHR_INPUT], 0x100000U + 896U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_NHR_SLICE_SIZE], 1104U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_NHR_SLICE_ONE_JETTY_SIZE], 1024U);
    EXPECT_EQ(impl.xnData_[HcclKfcProtocol::KFC_CONCURRENT_RS_NHR_SLICE_LAST_JETTY_SIZE], 1104U);
    EXPECT_EQ(impl.ccuUsedXnNum_, HcclKfcProtocol::KFC_CONCURRENT_RS_PARAM_NUM);
    EXPECT_EQ(impl.handleMissionNum_[0], 2U);
}
} // namespace
