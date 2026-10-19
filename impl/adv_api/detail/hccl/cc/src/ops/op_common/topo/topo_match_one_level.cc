/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See the License for the specific language governing permissions and limitations under the License.
 */
#include "topo_match_one_level.h"

#include <algorithm>

namespace mc2_ops_hccl {
namespace {
constexpr u32 INVALID_TOPO_LEVEL_POSITION = 0xffffffffU;

bool LevelEncompasses(const PhysicalLevelInfo& upper, const PhysicalLevelInfo& lower)
{
    return std::includes(upper.localRanks.begin(), upper.localRanks.end(), lower.localRanks.begin(),
        lower.localRanks.end());
}

u32 PickMeshConcurLevel(
    const std::vector<PhysicalLevelInfo>& physicalLevels, const std::vector<u32>& effectiveIndices, u32 userRankSize)
{
    for (u32 position = 0; position < effectiveIndices.size(); ++position) {
        const PhysicalLevelInfo& level = physicalLevels[effectiveIndices[position]];
        if (level.localRanks.size() == userRankSize && level.topoType == CommTopo::COMM_TOPO_1DMESH) {
            return position;
        }
    }
    return INVALID_TOPO_LEVEL_POSITION;
}

u32 FindUpperEncompassingLevel(
    const std::vector<PhysicalLevelInfo>& physicalLevels, const std::vector<u32>& effectiveIndices,
    u32 meshPosition)
{
    const PhysicalLevelInfo& meshLevel = physicalLevels[effectiveIndices[meshPosition]];
    for (u32 position = meshPosition + 1U; position < effectiveIndices.size(); ++position) {
        if (LevelEncompasses(physicalLevels[effectiveIndices[position]], meshLevel)) {
            return position;
        }
    }
    return INVALID_TOPO_LEVEL_POSITION;
}

std::vector<u32> CollectEffectiveIndices(
    const std::vector<PhysicalLevelInfo>& physicalLevels, OpExecuteConfig engine)
{
    const bool requireHost = (engine == OpExecuteConfig::HOSTCPU || engine == OpExecuteConfig::HOSTCPU_TS);
    std::vector<u32> effectiveIndices;
    for (u32 index = 0; index < physicalLevels.size(); ++index) {
        const PhysicalLevelInfo& level = physicalLevels[index];
        if (!level.hasTopoInst) {
            continue;
        }
        if (requireHost && level.locType != EndpointLocType::ENDPOINT_LOC_TYPE_HOST) {
            continue;
        }
        if (!requireHost && level.locType == EndpointLocType::ENDPOINT_LOC_TYPE_HOST) {
            continue;
        }
        effectiveIndices.push_back(index);
    }
    return effectiveIndices;
}

std::string FormatPhysicalIndices(const std::vector<PhysicalLevelIndex>& indices)
{
    std::string result = "{";
    for (size_t i = 0; i < indices.size(); ++i) {
        if (i != 0) {
            result += ",";
        }
        result += std::to_string(static_cast<u32>(indices[i]));
    }
    return result + "}";
}
} // namespace

TopoMatchOneLevel::TopoMatchOneLevel(OpExecuteConfig engine) : engine_(engine) {}

HcclResult TopoMatchOneLevel::MatchTopo(
    HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    (void)comm;
    if (topoInfo == nullptr || topoInfo->physicalLevels.empty() || topoInfo->userRankSize == 0) {
        HCCL_WARNING(
            "[TopoMatchOneLevel] invalid input. physicalLevels.size[%zu], userRankSize[%u].",
            topoInfo == nullptr ? 0U : topoInfo->physicalLevels.size(),
            topoInfo == nullptr ? 0U : topoInfo->userRankSize);
        return HcclResult::HCCL_E_INTERNAL;
    }

    const std::vector<PhysicalLevelInfo>& physicalLevels = topoInfo->physicalLevels;
    const std::vector<u32> effectiveIndices = CollectEffectiveIndices(physicalLevels, engine_);
    if (effectiveIndices.empty()) {
        HCCL_INFO("[TopoMatchOneLevel] Rank [%u], no valid layer after engine filter.", topoInfo->userRank);
        return HcclResult::HCCL_E_NOT_SUPPORT;
    }

    const u32 meshPosition = PickMeshConcurLevel(physicalLevels, effectiveIndices, topoInfo->userRankSize);
    if (meshPosition == INVALID_TOPO_LEVEL_POSITION) {
        HCCL_INFO("[TopoMatchOneLevel] Rank [%u], no full-rank 1DMESH layer for MeshConcur.", topoInfo->userRank);
        return HcclResult::HCCL_E_NOT_SUPPORT;
    }

    const u32 upperPosition = FindUpperEncompassingLevel(physicalLevels, effectiveIndices, meshPosition);
    if (upperPosition == INVALID_TOPO_LEVEL_POSITION) {
        HCCL_INFO("[TopoMatchOneLevel] Rank [%u], MeshConcur has no upper encompassing layer.", topoInfo->userRank);
        return HcclResult::HCCL_E_NOT_SUPPORT;
    }

    const u32 meshIndex = effectiveIndices[meshPosition];
    algHierarchyInfo.infos.clear();
    algHierarchyInfo.infos.resize(1U);
    algHierarchyInfo.infos[0].push_back(physicalLevels[meshIndex].localRanks);

    algHierarchyInfo.physicalIdxForAlgoLevels.clear();
    algHierarchyInfo.physicalIdxForAlgoLevels.resize(1U);
    algHierarchyInfo.physicalIdxForAlgoLevels[0] = {
        static_cast<PhysicalLevelIndex>(meshIndex),
        static_cast<PhysicalLevelIndex>(effectiveIndices[upperPosition])
    };
    HCCL_INFO(
        "[TopoMatchOneLevel] Rank [%u], userRankSize [%u], physicalIdxForAlgoLevels: [%s].", topoInfo->userRank,
        topoInfo->userRankSize, FormatPhysicalIndices(algHierarchyInfo.physicalIdxForAlgoLevels[0]).c_str());
    return HcclResult::HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
