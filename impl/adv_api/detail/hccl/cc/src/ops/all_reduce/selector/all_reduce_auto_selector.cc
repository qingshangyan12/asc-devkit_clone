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
constexpr u32 MAX_RANK_NUM_FOR_CONCURRENT_ALGO = 4;

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
    selectAlgName = "CcuAllReduceMesh1D";
    HCCL_DEBUG("[AllReduceAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllReduceAutoSelector::SelectCcuScheduleLevel0Algo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName,
    const u64 dataSize) const
{
    CHK_PRT_RET(
        opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8,
        HCCL_DEBUG(
            "[AllReduceAutoSelector] dataType[%d] is not supported yet for ccu schedule mode "
            "with ms reduce.",
            opParam.DataDes.dataType),
        SelectorStatus::NOT_MATCH);
    if (topoInfo->level0Topo != Level0Shape::MESH_1D ||
        topoInfo->level0MeshType == Level0MeshType::TWO_DIE_NOT_REGULAR) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] level0Topo[%d], level0MeshType[%d] is not supported for ccu schedule mode.",
            topoInfo->level0Topo, topoInfo->level0MeshType);
        return SelectorStatus::NOT_MATCH;
    }
    double ratio;
    if (topoInfo->userRankSize == 0) {
        HCCL_DEBUG("[AllReduceAutoSelector] the selector userRankSize not set");
        ratio = 1;
    } else {
        ratio = DEFAULT_RANK_SIZE / topoInfo->userRankSize;
    }
    if (dataSize * ratio >= AR_M2M_1D_MAX_DATA_SIZE) {
        HCCL_DEBUG(
            "[AllReduceAutoSelector] dataSize[%llu] * ratio[%f] >= MAX_DATA_SIZE[%llu].",
            static_cast<unsigned long long>(dataSize), ratio,
            static_cast<unsigned long long>(AR_M2M_1D_MAX_DATA_SIZE));
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_REGULAR) {
        HCCL_DEBUG("[AllReduceAutoSelector] TWO_DIE_REGULAR not supported yet for KFC AllReduce.");
        return SelectorStatus::NOT_MATCH;
    }
    selectAlgName = "CcuAllReduceMesh1DMem2Mem";
    HCCL_DEBUG("[AllReduceAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
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
    if (Is64BitDataType(opParam.DataDes.dataType)) {
        HCCL_DEBUG("[AllReduceAutoSelector] ccu_schedule mode not support INT64, UINT64, FP64.");
        return SelectorStatus::NOT_MATCH;
    }
    CHK_PRT_RET(
        IsInputOutputOverlap(opParam) == true,
        HCCL_WARNING("[Algo][AllReduceAutoSelector] ccu_sched does not support inplace allreduce."),
        SelectorStatus::NOT_MATCH);
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    if (topoInfo->topoLevelNums > 1) {
        HCCL_DEBUG("[AllReduceAutoSelector] multi-level topo is not supported for ccu schedule mode.");
        return SelectorStatus::NOT_MATCH;
    }
    return SelectCcuScheduleLevel0Algo(topoInfo, opParam, selectAlgName, dataSize);
}

SelectorStatus AllReduceAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    (void)opParam;
    if (topoInfo->topoLevelNums > 1) {
        selectAlgName = "InsAllReduceNHR";
    } else {
        if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
            selectAlgName = "InsAllReduceMesh1D";
        } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
            selectAlgName = "InsAllReduceNHR";
        } else if (topoInfo->level0Topo == Level0Shape::CLOS) {
            selectAlgName = "InsAllReduceNHR";
        } else {
            HCCL_ERROR("[AllReduceAutoSelector] topo not match");
            return SelectorStatus::NOT_MATCH;
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
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLREDUCE, 18, AllReduceAutoSelector);

} // namespace mc2_ops_hccl
