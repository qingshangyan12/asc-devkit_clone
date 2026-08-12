/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "all_to_all_v_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {

SelectorStatus AllToAllVAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)opParam;
    (void)configAlgMap;
    // 仅单层 MESH_1D 支持 CCU HalfAllToAllV
    if (topoInfo->topoLevelNums > 1 || topoInfo->level0Topo != Level0Shape::MESH_1D) {
        HCCL_DEBUG("[AllToAllVAutoSelector] only single level MESH_1D is supported for ccu_ms mode.");
        return SelectorStatus::NOT_MATCH;
    }
    selectAlgName = "CcuHalfAll2AllVMesh1D";
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllToAllVAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)opParam;
    (void)configAlgMap;
    // 仅单层 SINGLE_DIE MESH_1D 支持 CCU schedule 模式
    if (topoInfo->topoLevelNums != 1 || topoInfo->level0Topo != Level0Shape::MESH_1D ||
        topoInfo->level0MeshType != Level0MeshType::SINGLE_DIE) {
        HCCL_DEBUG(
            "[AllToAllVAutoSelector] unsupported KFC topology for ccu schedule mode, levelNum[%u], level0Topo[%d], "
            "meshType[%d].",
            topoInfo->topoLevelNums, topoInfo->level0Topo, topoInfo->level0MeshType);
        return SelectorStatus::NOT_MATCH;
    }
    selectAlgName = "CcuHalfAll2AllVMesh1D";
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllToAllVAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    // AICPU 的 AllToAllV 已由 alltoallv_auto_selector 处理
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] AICPU AllToAllV is handled by alltoallv_auto_selector.", __func__);
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AllToAllVAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] AIV is not supported for AllToAllV.", __func__);
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AllToAllVAutoSelector::SelectDPUAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    HCCL_DEBUG("[AllToAllVAutoSelector][%s] DPU is not supported for AllToAllV.", __func__);
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_HALF_ALLTOALLV, 18, AllToAllVAutoSelector);

} // namespace mc2_ops_hccl
