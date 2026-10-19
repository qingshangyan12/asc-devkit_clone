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
#include <numeric>
#include "gtest/gtest.h"
#include "template_utils.h"
#include "reduce_scatter_auto_selector.h"
#include "ins_temp_reduce_scatter_mesh_1D_Z_axis_detour.h"
#include "coll_alg_v2_exec_registry.h"
#include "executor_v2_base.h"

using namespace mc2_ops_hccl;

namespace {
std::vector<ChannelInfo> Channels(std::initializer_list<u32> weights)
{
    std::vector<ChannelInfo> channels;
    for (u32 weight : weights) {
        ChannelInfo channel{};
        channel.portGroupSize = weight;
        channels.push_back(channel);
    }
    return channels;
}

TopoInfoWithNetLayerDetails MeshTopo(u32 ranks)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRankSize = ranks;
    topo.serverNum = 1;
    topo.topoLevelNums = 1;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.netLayerDetails.netLayerNum = 2;
    topo.netLayerDetails.netLayers = {0, 1};
    topo.netLayerDetails.localNetInsSizeOfLayer = {ranks, ranks};
    topo.topoInstDetailsOfLayer.resize(2);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {ranks};
    topo.topoInstDetailsOfLayer[1].rankNumForTopoType[COMM_TOPO_CLOS] = {ranks};
    return topo;
}

std::string Select(
    TopoInfoWithNetLayerDetails& topo, u64 bytes, HcclDataType dtype = HCCL_DATA_TYPE_FP32,
    HcclReduceOp reduce = HCCL_REDUCE_SUM)
{
    OpParam param{};
    param.engine = COMM_ENGINE_AICPU_TS;
    param.DataDes.dataType = dtype;
    param.DataDes.count = bytes / DATATYPE_SIZE_TABLE[dtype];
    param.reduceType = reduce;
    ReduceScatterAutoSelector selector;
    const AutoSelectorBase& base = selector;
    std::string name;
    EXPECT_EQ(base.SelectAicpuAlgo(&topo, param, {}, name), SelectorStatus::MATCH);
    return name;
}
} // namespace

TEST(ReduceScatterConcur, BandwidthSplitConservesCountsAndOffsets)
{
    for (u32 ranks : {2U, 4U, 8U}) {
        for (u64 elemSize : {1ULL, 2ULL, 4ULL, 8ULL}) {
            for (u64 count : {0ULL, 1ULL, 3ULL, 255ULL, 257ULL, 1048577ULL}) {
                for (const auto& channels : {Channels({1}), Channels({1, 8}), Channels({1, 2, 6})}) {
                    std::vector<u64> counts, sizes, offsets;
                    ASSERT_EQ(
                        CalcDataSplitByBandwidthZAxisDetour(
                            count, elemSize, channels, counts, sizes, offsets, 1, channels.size() - 1, ranks),
                        HCCL_SUCCESS);
                    EXPECT_EQ(std::accumulate(counts.begin(), counts.end(), u64{0}), count);
                    u64 offset = 0;
                    for (size_t i = 0; i < counts.size(); ++i) {
                        EXPECT_EQ(offsets[i], offset);
                        EXPECT_EQ(sizes[i], counts[i] * elemSize);
                        offset += sizes[i];
                    }
                    EXPECT_EQ(offset, count * elemSize);
                }
            }
        }
    }
    std::vector<u64> counts, sizes, offsets;
    ASSERT_EQ(
        CalcDataSplitByBandwidthZAxisDetour(9 * 1024, 4, Channels({1, 8}), counts, sizes, offsets, 1, 1, 2),
        HCCL_SUCCESS);
    EXPECT_EQ(counts, (std::vector<u64>{1024, 8192}));
}

TEST(ReduceScatterConcur, RejectsInvalidSplitInputs)
{
    std::vector<u64> counts, sizes, offsets;
    auto split = [&](u64 count, u64 size, std::vector<ChannelInfo> channels, u32 l0, u32 l1, u32 ranks) {
        return CalcDataSplitByBandwidthZAxisDetour(count, size, channels, counts, sizes, offsets, l0, l1, ranks);
    };
    EXPECT_EQ(split(1, 0, Channels({1}), 1, 0, 2), HCCL_E_PARA);
    EXPECT_EQ(split(UINT64_MAX, 8, Channels({1}), 1, 0, 2), HCCL_E_PARA);
    EXPECT_EQ(split(1, 4, Channels({1}), 1, 0, 0), HCCL_E_PARA);
    EXPECT_EQ(split(1, 4, Channels({1, 0}), 1, 1, 2), HCCL_E_PARA);
    EXPECT_EQ(split(1, 4, Channels({1, UINT32_MAX}), 1, 1, 2), HCCL_E_PARA);
    EXPECT_EQ(split(1, 4, Channels({1}), 0, 1, 2), HCCL_E_PARA);
    EXPECT_EQ(split(1, 4, Channels({1}), 1, 1, 2), HCCL_E_PARA);
}

TEST(ReduceScatterConcur, SelectorThresholdAndUnsupportedReductions)
{
    constexpr u64 threshold = 4ULL * 1024 * 1024;
    auto topo = MeshTopo(2);
    EXPECT_NE(Select(topo, threshold - 4), "AicpuReduceScatterSoleMeshConcur");
    EXPECT_EQ(Select(topo, threshold), "AicpuReduceScatterSoleMeshConcur");
    EXPECT_EQ(Select(topo, threshold + 4), "AicpuReduceScatterSoleMeshConcur");
    EXPECT_EQ(Select(topo, threshold, HCCL_DATA_TYPE_INT64), "InsReduceScatterMesh1D");
    EXPECT_EQ(Select(topo, threshold, HCCL_DATA_TYPE_FP32, HCCL_REDUCE_PROD), "InsReduceScatterMesh1D");
    topo.netLayerDetails.netLayerNum = 1;
    EXPECT_NE(Select(topo, threshold), "AicpuReduceScatterSoleMeshConcur");
    for (u32 ranks : {4U, 8U}) {
        topo = MeshTopo(ranks);
        EXPECT_NE(Select(topo, threshold), "AicpuReduceScatterSoleMeshConcur");
        for (u64 bytes : {1536ULL * 1024 * 1024 / ranks, 1536ULL * 1024 * 1024 / ranks + 4}) {
            EXPECT_EQ(Select(topo, bytes), "AicpuReduceScatterSoleMeshChunk");
        }
    }
}

TEST(ReduceScatterConcur, RestoringChannelsDoesNotRetainPreviousLayerCounts)
{
    OpParam param{};
    InsTempReduceScatterMesh1DZAxisDetour alg(param, 0, {{0, 1}});
    ASSERT_EQ(alg.SetchannelsPerRank({{1, Channels({1, 8})}}), HCCL_SUCCESS);
    EXPECT_EQ(alg.GetThreadNum(), 3U);
    ASSERT_EQ(alg.SetchannelsPerRank({{1, Channels({1})}}), HCCL_SUCCESS);
    EXPECT_EQ(alg.GetThreadNum(), 2U);
    std::vector<u64> counts, sizes, offsets;
    ASSERT_EQ(alg.CalcDataSplitByPortGroup(257, 4, Channels({1}), counts, sizes, offsets), HCCL_SUCCESS);
    EXPECT_EQ(counts, (std::vector<u64>{257}));
    EXPECT_NE(alg.SetchannelsPerRank({}), HCCL_SUCCESS);
}

TEST(ReduceScatterConcur, OnlyConcurNameRegisteredAndRejectsUnsupportedTopologyAndReduction)
{
    EXPECT_EQ(
        CollAlgExecRegistryV2::Instance().GetAlgExec(HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterMesh1DZAxisDetour"),
        nullptr);
    auto exec =
        CollAlgExecRegistryV2::Instance().GetAlgExec(HCCL_CMD_REDUCE_SCATTER, "AicpuReduceScatterSoleMeshConcur");
    ASSERT_NE(exec, nullptr);
    auto topo = MeshTopo(2);
    OpParam param{};
    param.engine = COMM_ENGINE_AICPU_TS;
    param.DataDes.dataType = HCCL_DATA_TYPE_FP32;
    param.reduceType = HCCL_REDUCE_PROD;
    AlgHierarchyInfoForAllLevel hierarchy{};
    hierarchy.infos = {{{0, 1}}};
    AlgResourceRequest request{};
    EXPECT_EQ(exec->CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_E_NOT_SUPPORT);
    param.reduceType = HCCL_REDUCE_SUM;
    topo.topoLevelNums = 2;
    EXPECT_EQ(exec->CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_E_NOT_SUPPORT);
    topo.topoLevelNums = 1;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.level0PcieMix = false;
    EXPECT_EQ(exec->CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_E_NOT_SUPPORT);
}
