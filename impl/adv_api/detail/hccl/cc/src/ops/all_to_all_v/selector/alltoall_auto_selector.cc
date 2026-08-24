/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "alltoall_auto_selector.h"

#include "selector_registry.h"
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {

constexpr uint32_t CONCURRENT_RANK_LIMIT = 4;
constexpr uint64_t BIG_DATA_SIZE_LIMIT = 512;
constexpr uint32_t TOPO_LEVEL_NUM_3 = 3;

SelectorStatus AlltoAllAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    CHK_PTR_NULL(topoInfo);
    if (topoInfo->topoLevelNums >= TOPO_LEVEL_NUM_3) {
        return SelectorStatus::NOT_MATCH;
    }
    CHK_PRT_RET(
        IsInputOutputOverlap(opParam) == true,
        HCCL_WARNING("[AlltoAllAutoSelector] ccu schedule does not support inplace alltoall."),
        SelectorStatus::NOT_MATCH);

    if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS && !topoInfo->level0PcieMix) {
        uint32_t dataTypeSize = DATATYPE_SIZE_TABLE[opParam.all2AllVDataDes.sendType];
        u64* sendCounts = reinterpret_cast<u64*>(opParam.all2AllVDataDes.sendCounts);
        CHK_PRT_RET(
            sendCounts == nullptr, HCCL_WARNING("[AlltoAllAutoSelector] sendCounts is nullptr."),
            SelectorStatus::NOT_MATCH);
        uint64_t dataSize = sendCounts[0] * dataTypeSize;
        bool isMeshNumEqualToClosNum = false;
        CHK_PRT_RET(
            CheckMeshNumEqualToClosNum(topoInfo, isMeshNumEqualToClosNum) != HCCL_SUCCESS,
            HCCL_DEBUG("[AlltoAllAutoSelector] CheckMeshNumEqualToClosNum failed."), SelectorStatus::NOT_MATCH);
        if ((isMeshNumEqualToClosNum == true) && (topoInfo->userRankSize <= CONCURRENT_RANK_LIMIT)
            && (dataSize > BIG_DATA_SIZE_LIMIT)) {
            selectAlgName = KFC_CONCURRENT_ALL_TO_ALL_ALG_NAME;
        } else {
            selectAlgName = "CcuSchedAllToAllSoleMeshUBX";
        }
        HCCL_INFO("[AlltoAllAutoSelector] Algo match[%s].", selectAlgName.c_str());
        return SelectorStatus::MATCH;
    }

    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        selectAlgName = "CcuSchedAllToAllSoleMesh";
        HCCL_INFO("[AlltoAllAutoSelector] Algo match[%s].", selectAlgName.c_str());
        return SelectorStatus::MATCH;
    }

    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AlltoAllAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)opParam;
    (void)configAlgMap;
    CHK_PRT_RET(
        topoInfo == nullptr, HCCL_ERROR("[AlltoAllAutoSelector] topoInfo is nullptr."), SelectorStatus::NOT_MATCH);
    if (topoInfo->userRankSize <= 1U ||
        (topoInfo->level0Topo != Level0Shape::MESH_1D && topoInfo->level0Topo != Level0Shape::CLOS)) {
        HCCL_WARNING(
            "[AlltoAllAutoSelector] rankSize[%u], level0Topo[%u] is not supported by local AICPU Mesh1D.",
            topoInfo->userRankSize, static_cast<u32>(topoInfo->level0Topo));
        return SelectorStatus::NOT_MATCH;
    }

    // Host tiling has no runtime count. Select the multi-channel token to allocate a resource upper bound.
    selectAlgName = "InsAlltoAllMesh1D";
    HCCL_INFO("[AlltoAllAutoSelector] Algo match[%s].", selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLTOALL, 18, AlltoAllAutoSelector);
} // namespace mc2_ops_hccl
