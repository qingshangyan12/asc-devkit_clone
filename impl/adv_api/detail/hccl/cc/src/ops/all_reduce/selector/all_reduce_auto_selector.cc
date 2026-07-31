/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "all_reduce_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {

constexpr u64 AR_AICPU_1D_SMALL_DATA_SIZE = 8 * 1024 * 1024;

SelectorStatus AllReduceAutoSelector::SelectKfcMesh1D(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    if (topoInfo->topoLevelNums != 1 || topoInfo->level0Topo != Level0Shape::MESH_1D ||
        topoInfo->level0MeshType != Level0MeshType::SINGLE_DIE) {
        return SelectorStatus::NOT_MATCH;
    }
    if (IsInputOutputOverlap(opParam) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD ||
        Is64BitDataType(opParam.DataDes.dataType) || opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8) {
        return SelectorStatus::NOT_MATCH;
    }
    if (opParam.DataDes.outputType != HcclDataType::HCCL_DATA_TYPE_RESERVED &&
        opParam.DataDes.outputType != opParam.DataDes.dataType) {
        return SelectorStatus::NOT_MATCH;
    }
    selectAlgName = "CcuAllReduceMesh1DMem2Mem";
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    return SelectKfcMesh1D(topoInfo, opParam, selectAlgName);
}

SelectorStatus AllReduceAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    return SelectKfcMesh1D(topoInfo, opParam, selectAlgName);
}

SelectorStatus AllReduceAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    if (topoInfo->topoLevelNums != 1 || topoInfo->level0Topo != Level0Shape::MESH_1D) {
        return SelectorStatus::NOT_MATCH;
    }

    const u64 dataSize = opParam.DataDes.count * DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    const bool specialType = Is64BitDataType(opParam.DataDes.dataType) ||
        opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD;

    if ((specialType && dataSize <= AR_AICPU_1D_SMALL_DATA_SIZE) ||
        (!specialType && dataSize <= AR_AICPU_1D_SMALL_DATA_SIZE)) {
        selectAlgName = "InsAllReduceMesh1DOneShot";
        return SelectorStatus::MATCH;
    }
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLREDUCE, 18, AllReduceAutoSelector);

} // namespace mc2_ops_hccl
