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

namespace mc2_ops_hccl {

SelectorStatus AlltoAllAutoSelector::SelectCcuMeshAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    if (topoInfo->topoLevelNums != 1 || topoInfo->level0Topo != Level0Shape::MESH_1D) {
        HCCL_DEBUG(
            "[AlltoAllAutoSelector] iteration-1 CCU path requires single-level Mesh1D, levelNum[%u], topo[%d].",
            topoInfo->topoLevelNums, topoInfo->level0Topo);
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->level0MeshType != Level0MeshType::SINGLE_DIE) {
        HCCL_DEBUG("[AlltoAllAutoSelector] iteration-1 KFC path currently supports only single-die Mesh1D.");
        return SelectorStatus::NOT_MATCH;
    }

    const bool isAllToAll = opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALL;
    const bool isAllToAllV = opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALLV;
    if (!isAllToAll && !isAllToAllV) {
        return SelectorStatus::NOT_MATCH;
    }

    selectAlgName = isAllToAll ? "CcuAlltoAllMesh1D" : "CcuAlltoAllVMesh1D";
    HCCL_INFO("[AlltoAllAutoSelector][%s] Algo match [%s].", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AlltoAllAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    return SelectCcuMeshAlgo(topoInfo, opParam, selectAlgName);
}

SelectorStatus AlltoAllAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    return SelectCcuMeshAlgo(topoInfo, opParam, selectAlgName);
}

SelectorStatus AlltoAllAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    if (opParam.opType != HcclCMDType::HCCL_CMD_ALLTOALLV || topoInfo->topoLevelNums != 1 ||
        topoInfo->level0Topo != Level0Shape::MESH_1D ||
        topoInfo->level0MeshType != Level0MeshType::SINGLE_DIE) {
        HCCL_DEBUG("[AlltoAllAutoSelector] AICPU iteration-1 path supports only single-die AllToAllV Mesh1D.");
        return SelectorStatus::NOT_MATCH;
    }

    selectAlgName = "InsAlltoAllVMesh1D";
    HCCL_INFO("[AlltoAllAutoSelector][%s] Algo match [%s].", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AlltoAllAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AlltoAllAutoSelector::SelectDPUAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLTOALL, 18, AlltoAllAutoSelector);
REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLTOALLV, 18, AlltoAllAutoSelector);

} // namespace mc2_ops_hccl
