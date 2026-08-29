/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <limits>
#include <map>
#include <vector>

#include "gtest/gtest.h"

#include "ins_temp_reduce_scatter_nhr.h"
#include "template_utils.h"

namespace mc2_ops_hccl {
extern uint32_t g_calcChannelRequestNhrCallCount;
extern uint32_t g_calcChannelRequestNhrMultiJettyCallCount;
extern bool g_lastNhrMultiJettyIsolation;
} // namespace mc2_ops_hccl

namespace {
using namespace mc2_ops_hccl;

std::map<u32, std::vector<ChannelInfo>> MakeParallelChannels(
    const std::vector<u32>& portGroups, const std::vector<u32>& dieIds = {})
{
    std::vector<ChannelInfo> channels(portGroups.size());
    for (size_t i = 0; i < portGroups.size(); ++i) {
        channels[i].portGroupSize = portGroups[i];
        if (i < dieIds.size()) {
            channels[i].dieId = dieIds[i];
        }
    }
    return {{1U, channels}};
}

TopoInfoWithNetLayerDetails MakeReduceScatterTopo(bool fullMesh, bool pcieMix = false)
{
    TopoInfoWithNetLayerDetails topoInfo{};
    topoInfo.userRank = 0U;
    topoInfo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topoInfo.level0PcieMix = pcieMix;
    topoInfo.netLayerDetails.localNetInsSizeOfLayer = {8U};
    TopoInstDetails detail{};
    detail.rankNumForTopoType[CommTopo::COMM_TOPO_1DMESH] = {fullMesh ? 8U : 4U};
    topoInfo.topoInstDetailsOfLayer = {detail};
    return topoInfo;
}

class A5ParallelSplitTest : public testing::Test {};

TEST_F(A5ParallelSplitTest, MissingTopologyUsesConfiguredFallback)
{
    const std::map<u32, std::vector<ChannelInfo>> empty;
    EXPECT_DOUBLE_EQ(
        CalcParallelDataSplitRatio(8U, 2U, empty, empty, ParallelDataSplitType::ALL_GATHER, 0.375), 0.375);
}

TEST_F(A5ParallelSplitTest, InvalidFallbackIsNormalized)
{
    const std::map<u32, std::vector<ChannelInfo>> empty;
    EXPECT_DOUBLE_EQ(
        CalcParallelDataSplitRatio(
            8U, 2U, empty, empty, ParallelDataSplitType::ALL_GATHER,
            std::numeric_limits<double>::quiet_NaN()),
        0.5);
}

TEST_F(A5ParallelSplitTest, AllGatherRatioIsQuantizedAndCappedAtHalf)
{
    const auto intra = MakeParallelChannels({1U, 1U, 1U, 1U, 1U, 1U, 1U});
    const auto inter = MakeParallelChannels({1U});
    EXPECT_DOUBLE_EQ(
        CalcParallelDataSplitRatio(8U, 2U, intra, inter, ParallelDataSplitType::ALL_GATHER, 0.5), 0.5);
}

TEST_F(A5ParallelSplitTest, PodLinksUseEffectiveHalfPortGroup)
{
    const auto intra = MakeParallelChannels({1U, 1U, 1U, 1U, 1U, 1U, 1U});
    const auto inter = MakeParallelChannels({4U, 4U}, {0U, 1U});
    EXPECT_DOUBLE_EQ(
        CalcParallelDataSplitRatio(8U, 2U, intra, inter, ParallelDataSplitType::ALL_GATHER, 0.5), 0.5);
}

TEST_F(A5ParallelSplitTest, NonPodRatioRoundsToNearestEighth)
{
    const auto intra = MakeParallelChannels({1U, 1U, 1U, 1U, 1U, 1U, 1U});
    const auto inter = MakeParallelChannels({16U});
    EXPECT_DOUBLE_EQ(
        CalcParallelDataSplitRatio(8U, 2U, intra, inter, ParallelDataSplitType::ALL_GATHER, 0.5), 0.25);
}

class A5ReduceScatterNhrResourceTest : public testing::Test {
protected:
    void SetUp() override
    {
        g_calcChannelRequestNhrCallCount = 0U;
        g_calcChannelRequestNhrMultiJettyCallCount = 0U;
        g_lastNhrMultiJettyIsolation = false;
    }

    static InsTempReduceScatterNHR MakeTemplate(const std::vector<u32>& ranks = {0U, 1U})
    {
        OpParam param{};
        return InsTempReduceScatterNHR(param, 0U, {ranks});
    }

    static OpParam MakeParam(u64 count)
    {
        OpParam param{};
        param.engine = CommEngine::COMM_ENGINE_AICPU_TS;
        param.DataDes.count = count;
        param.DataDes.dataType = HcclDataType::HCCL_DATA_TYPE_FP16;
        return param;
    }
};

TEST_F(A5ReduceScatterNhrResourceTest, FullMeshDisablesIsolation)
{
    auto nhr = MakeTemplate();
    auto topo = MakeReduceScatterTopo(true);
    auto param = MakeParam(32U * 1024U * 1024U);
    AlgResourceRequest request{};
    ASSERT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_SUCCESS);
    EXPECT_EQ(g_calcChannelRequestNhrMultiJettyCallCount, 1U);
    EXPECT_FALSE(g_lastNhrMultiJettyIsolation);
}

TEST_F(A5ReduceScatterNhrResourceTest, DataAt512KiBBoundaryDisablesIsolation)
{
    auto nhr = MakeTemplate();
    auto topo = MakeReduceScatterTopo(false);
    auto param = MakeParam(SMALL_SIZE_512KB / 2U);
    AlgResourceRequest request{};
    ASSERT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_SUCCESS);
    EXPECT_FALSE(g_lastNhrMultiJettyIsolation);
}

TEST_F(A5ReduceScatterNhrResourceTest, DataAbove512KiBEnablesIsolation)
{
    auto nhr = MakeTemplate();
    auto topo = MakeReduceScatterTopo(false);
    auto param = MakeParam(SMALL_SIZE_512KB / 2U + 1U);
    AlgResourceRequest request{};
    ASSERT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_SUCCESS);
    EXPECT_TRUE(g_lastNhrMultiJettyIsolation);
}

TEST_F(A5ReduceScatterNhrResourceTest, PcieMixUsesLegacyNhrRequest)
{
    auto nhr = MakeTemplate();
    auto topo = MakeReduceScatterTopo(false, true);
    auto param = MakeParam(32U * 1024U * 1024U);
    AlgResourceRequest request{};
    ASSERT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_SUCCESS);
    EXPECT_EQ(g_calcChannelRequestNhrMultiJettyCallCount, 0U);
    EXPECT_EQ(g_calcChannelRequestNhrCallCount, 1U);
}

TEST_F(A5ReduceScatterNhrResourceTest, InvalidDataTypeIsRejectedBeforeResourceRequest)
{
    auto nhr = MakeTemplate();
    auto topo = MakeReduceScatterTopo(false);
    auto param = MakeParam(1U);
    param.DataDes.dataType = HcclDataType::HCCL_DATA_TYPE_RESERVED;
    AlgResourceRequest request{};
    EXPECT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_E_PARA);
    EXPECT_EQ(g_calcChannelRequestNhrCallCount + g_calcChannelRequestNhrMultiJettyCallCount, 0U);
}

TEST_F(A5ReduceScatterNhrResourceTest, DataSizeOverflowIsRejectedBeforeResourceRequest)
{
    auto nhr = MakeTemplate();
    auto topo = MakeReduceScatterTopo(false);
    auto param = MakeParam(std::numeric_limits<u64>::max());
    AlgResourceRequest request{};
    EXPECT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_E_PARA);
    EXPECT_EQ(g_calcChannelRequestNhrCallCount + g_calcChannelRequestNhrMultiJettyCallCount, 0U);
}

TEST_F(A5ReduceScatterNhrResourceTest, EmptyPeerSetIsRejectedAfterChannelSelection)
{
    auto nhr = MakeTemplate({0U});
    auto topo = MakeReduceScatterTopo(false);
    auto param = MakeParam(1U);
    AlgResourceRequest request{};
    EXPECT_EQ(nhr.CalcRes(nullptr, param, &topo, request), HCCL_E_UNAVAIL);
}

} // namespace
