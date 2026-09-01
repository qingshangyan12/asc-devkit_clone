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
namespace {
constexpr u64 AR_CCU_MESH_1D_MAX_DATA_SIZE = 16 * 1024 * 1024;
constexpr u64 AR_AICPU_MESH_1D_ONESHOT_MAX_DATA_SIZE = 8 * 1024 * 1024;

bool IsSingleDieMesh1D(const TopoInfoWithNetLayerDetails* topoInfo)
{
    return topoInfo->topoLevelNums == 1 && topoInfo->level0Topo == Level0Shape::MESH_1D &&
           topoInfo->level0MeshType == Level0MeshType::SINGLE_DIE;
}
} // namespace

SelectorStatus AllReduceAutoSelector::SelectCcuMesh1D(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    if (!IsSingleDieMesh1D(topoInfo)) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] KFC AllReduce requires single-level, single-die Mesh1D, "
            "levelNum[%u], level0Topo[%d], meshType[%d].",
            topoInfo->topoLevelNums, topoInfo->level0Topo, topoInfo->level0MeshType);
        return SelectorStatus::NOT_MATCH;
    }
    if (IsInputOutputOverlap(opParam)) {
        HCCL_WARNING("[AllReduceAutoSelector] KFC AllReduce does not support overlapping input and output.");
        return SelectorStatus::NOT_MATCH;
    }
    if (opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8 ||
        Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] unsupported KFC dataType[%d] or reduceType[%d].",
            opParam.DataDes.dataType, opParam.reduceType);
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->userRankSize == 0) {
        HCCL_WARNING("[AllReduceAutoSelector] userRankSize is zero.");
        return SelectorStatus::NOT_MATCH;
    }

    const u64 dataSize = opParam.DataDes.count * DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    const double ratio = static_cast<double>(DEFAULT_RANK_SIZE) / topoInfo->userRankSize;
    if (dataSize * ratio >= AR_CCU_MESH_1D_MAX_DATA_SIZE) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] dataSize[%llu] * ratio[%f] exceeds KFC limit[%llu].", dataSize, ratio,
            AR_CCU_MESH_1D_MAX_DATA_SIZE);
        return SelectorStatus::NOT_MATCH;
    }

    selectAlgName = "CcuAllReduceMesh1DMem2Mem";
    HCCL_INFO("[AllReduceAutoSelector][%s] Algo match [%s].", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    return SelectCcuMesh1D(topoInfo, opParam, selectAlgName);
}

SelectorStatus AllReduceAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    return SelectCcuMesh1D(topoInfo, opParam, selectAlgName);
}

SelectorStatus AllReduceAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    if (!IsSingleDieMesh1D(topoInfo)) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] iteration-1 AICPU AllReduce requires single-level, single-die Mesh1D.");
        return SelectorStatus::NOT_MATCH;
    }

    const u64 dataSize = opParam.DataDes.count * DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    selectAlgName = dataSize <= AR_AICPU_MESH_1D_ONESHOT_MAX_DATA_SIZE ? "AicpuAllReduceSoleMeshOneShot" :
                                                                          "AicpuAllReduceSoleMeshTwoShot";
    HCCL_INFO("[AllReduceAutoSelector][%s] Algo match [%s].", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectDPUAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLREDUCE, 18, AllReduceAutoSelector);

} // namespace mc2_ops_hccl
