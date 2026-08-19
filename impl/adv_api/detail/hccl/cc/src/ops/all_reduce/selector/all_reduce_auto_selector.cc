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
constexpr u64 AR_AICPU_1D_MAX_DATA_SIZE = 32 * 1024 * 1024;
constexpr u64 AR_AICPU_SEQUENCE_DATA_SIZE = 4ULL * 1024 * 1024 * 1024;
constexpr u64 AR_2P_DETOUR_DATA_SIZE = 8 * 1024 * 1024;
}

SelectorStatus AllReduceAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    const u64 dataTypeSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    const u64 dataSize = opParam.DataDes.count * dataTypeSize;
    const bool specialDataOrReduce = Is64BitDataType(opParam.DataDes.dataType) ||
                                     opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD;
    if (specialDataOrReduce || topoInfo == nullptr) {
        return SelectorStatus::NOT_MATCH;
    }

    const double rankRatio = topoInfo->userRankSize == 0
                                 ? 1.0
                                 : DEFAULT_RANK_SIZE / topoInfo->userRankSize / topoInfo->userRankSize;
    const bool overMaxDataSize = dataSize * rankRatio > AR_AICPU_1D_MAX_DATA_SIZE;
    const bool overSequenceDataSize = dataSize > AR_AICPU_SEQUENCE_DATA_SIZE;
    bool match = false;
    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        const bool twoLevel = IsTwoLevelNetLayer(topoInfo);
        match = overMaxDataSize && !(twoLevel && overSequenceDataSize) &&
                !(twoLevel && topoInfo->userRankSize == 2 && dataSize >= AR_2P_DETOUR_DATA_SIZE);
    } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS && topoInfo->level0PcieMix) {
        match = IsLayerAllConnetedWithTopo(topoInfo, 0, CommTopo::COMM_TOPO_1DMESH) && overMaxDataSize;
    }

    if (!match) {
        return SelectorStatus::NOT_MATCH;
    }

    selectAlgName = "AicpuAllReduceSoleMeshChunkTwoShot";
    return SelectorStatus::MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLREDUCE, 18, AllReduceAutoSelector);

} // namespace mc2_ops_hccl
