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

SelectorStatus AllReduceAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    HCCL_DEBUG("[AllReduceAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);

    if (topoInfo->topoLevelNums != 1) {
        HCCL_WARNING(
            "[AllReduceAutoSelector] InsAllReduceMesh1DOneShot only supports a single network level, actual[%u].",
            topoInfo->topoLevelNums);
        return SelectorStatus::NOT_MATCH;
    }

    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        const u64 dataSize = opParam.DataDes.count * DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
        if (dataSize <= AR_AICPU_1D_SMALL_DATA_SIZE) {
            selectAlgName = "InsAllReduceMesh1DOneShot";
            HCCL_INFO("[AllReduceAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
            return SelectorStatus::MATCH;
        }
    } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS && topoInfo->level0PcieMix &&
        IsLayerAllConnetedWithTopo(topoInfo, 0, CommTopo::COMM_TOPO_1DMESH)) {
        const u64 dataSize = opParam.DataDes.count * DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
        if (dataSize <= AR_AICPU_1D_SMALL_DATA_SIZE) {
            selectAlgName = "InsAllReduceMesh1DOneShot";
            HCCL_INFO("[AllReduceAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
            return SelectorStatus::MATCH;
        }
    }

    HCCL_WARNING("[AllReduceAutoSelector] topology[%u] or dataSize does not match OneShot.",
        topoInfo->level0Topo);
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLREDUCE, 18, AllReduceAutoSelector);

} // namespace mc2_ops_hccl
