/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software; you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "all_reduce_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {
constexpr u64 AR_M2M_1D_MAX_DATA_SIZE = 8 * 1024 * 1024;

SelectorStatus AllReduceAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    if (topoInfo->topoLevelNums > 1 || topoInfo->level0Topo != Level0Shape::MESH_1D) {
        HCCL_DEBUG("[AllReduceAutoSelector] only single level MESH_1D is supported for ccu_ms mode.");
        return SelectorStatus::NOT_MATCH;
    }
    CHK_PRT_RET(
        opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8,
        HCCL_WARNING(
            "[AllReduceAutoSelector] dataType[%d] is not supported yet for ccu_ms mode.", opParam.DataDes.dataType),
        SelectorStatus::NOT_MATCH);
    CHK_PRT_RET(
        opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD,
        HCCL_WARNING(
            "[AllReduceAutoSelector] ReduceOp[%d] is not supported yet for ccu_ms mode.", opParam.reduceType),
        SelectorStatus::NOT_MATCH);
    if (Is64BitDataType(opParam.DataDes.dataType)) {
        HCCL_WARNING("[AllReduceAutoSelector] ccu_ms mode not support INT64, UINT64, FP64.");
        return SelectorStatus::NOT_MATCH;
    }
    if (IsInputOutputOverlap(opParam)) {
        HCCL_WARNING("[Algo][AllReduceAutoSelector] ccu_ms does not support inplace allreduce.");
        return SelectorStatus::NOT_MATCH;
    }
    selectAlgName = "CcuAllReduceMesh1DMem2Mem";
    HCCL_DEBUG("[AllReduceAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectCcuScheduleLevel0Algo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName,
    const u64 dataSize) const
{
    if (topoInfo->level0Topo != Level0Shape::MESH_1D ||
        topoInfo->level0MeshType != Level0MeshType::SINGLE_DIE) {
        HCCL_WARNING(
            "[AllReduceAutoSelector] unsupported KFC topology, level0Topo[%d], meshType[%d].",
            topoInfo->level0Topo, topoInfo->level0MeshType);
        return SelectorStatus::NOT_MATCH;
    }
    double ratio;
    if (topoInfo->userRankSize == 0) {
        HCCL_WARNING("[AllReduceAutoSelector]the selector is not set topoInfo->userRankSize]");
        ratio = 1;
    } else {
        ratio = DEFAULT_RANK_SIZE / topoInfo->userRankSize;
    }
    if (dataSize * ratio >= AR_M2M_1D_MAX_DATA_SIZE) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] dataSize[%lu] * ratio[%f] >= MAX_DATA_SIZE[%lu].", dataSize, ratio,
            AR_M2M_1D_MAX_DATA_SIZE);
        return SelectorStatus::NOT_MATCH;
    }
    selectAlgName = "CcuAllReduceMesh1DMem2Mem";
    HCCL_DEBUG("[AllReduceAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start", __func__);
    (void)configAlgMap;
    CHK_PRT_RET(
        opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD,
        HCCL_WARNING(
            "[AllReduceAutoSelector] ReduceOp[%d] is not supported yet for ccu schedule mode.", opParam.reduceType),
        SelectorStatus::NOT_MATCH);
    CHK_PRT_RET(
        opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8,
        HCCL_WARNING(
            "[AllReduceAutoSelector] dataType[%d] is not supported by KFC AllReduce.", opParam.DataDes.dataType),
        SelectorStatus::NOT_MATCH);
    if (Is64BitDataType(opParam.DataDes.dataType)) {
        HCCL_WARNING("[AllReduceAutoSelector] KFC AllReduce does not support INT64, UINT64 or FP64.");
        return SelectorStatus::NOT_MATCH;
    }
    if (IsInputOutputOverlap(opParam)) {
        HCCL_WARNING("[Algo][AllReduceAutoSelector] ccu schedule does not support inplace allreduce.");
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->topoLevelNums > 1) {
        HCCL_DEBUG("[AllReduceAutoSelector] multi-level topo is not supported for ccu schedule mode.");
        return SelectorStatus::NOT_MATCH;
    }
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    return SelectCcuScheduleLevel0Algo(topoInfo, opParam, selectAlgName, dataSize);
}

SelectorStatus AllReduceAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    (void)opParam;
    if (topoInfo->topoLevelNums > 1) {
        selectAlgName = "InsAllReduceNHR";
    } else {
        if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
            selectAlgName = "InsAllReduceMesh1DOneShot";
        } else {
            selectAlgName = "InsAllReduceNHR";
        }
    }
    HCCL_DEBUG("[AllReduceAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    (void)opParam;
    selectAlgName = "AivAllReduceMesh1D";
    HCCL_DEBUG("[AllReduceAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectDPUAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    (void)opParam;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLREDUCE, 18, AllReduceAutoSelector);

} // namespace mc2_ops_hccl
