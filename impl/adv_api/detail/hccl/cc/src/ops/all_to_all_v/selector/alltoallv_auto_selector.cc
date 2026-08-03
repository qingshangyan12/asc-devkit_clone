/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "alltoallv_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {

// Source-side ordering caps the supported rank count; kept aligned with the template and sub-kernel guards.
constexpr u32 A2AV_MIN_SUPPORTED_RANK_SIZE = 2;
constexpr u32 A2AV_MAX_SUPPORTED_RANK_SIZE = 8;

SelectorStatus AlltoAllVAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AlltoAllVAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;

    if (opParam.opType != HcclCMDType::HCCL_CMD_HALF_ALLTOALLV) {
        HCCL_DEBUG(
            "[AlltoAllVAutoSelector] opType[%u] is not supported yet for ccu_schedule mode.",
            static_cast<u32>(opParam.opType));
        return SelectorStatus::NOT_MATCH;
    }

    if (topoInfo->topoLevelNums != 1 || topoInfo->level0Topo != Level0Shape::MESH_1D ||
        topoInfo->userRankSize < A2AV_MIN_SUPPORTED_RANK_SIZE ||
        topoInfo->userRankSize > A2AV_MAX_SUPPORTED_RANK_SIZE) {
        HCCL_DEBUG(
            "[AlltoAllVAutoSelector] Half alltoallv only supports 2-8 ranks in one 1D mesh. "
            "topoLevelNums[%u], level0Topo[%u], userRankSize[%u].",
            topoInfo->topoLevelNums, static_cast<u32>(topoInfo->level0Topo), topoInfo->userRankSize);
        return SelectorStatus::NOT_MATCH;
    }

    // Must stay identical to the REGISTER_EXEC_V2 algorithm name and OP_TYPE_TO_CCU_1D_ALG_MAP entry.
    selectAlgName = "CcuHalfAll2AllVMesh1D";
    HCCL_DEBUG("[AlltoAllVAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_HALF_ALLTOALLV, 18, AlltoAllVAutoSelector);

} // namespace mc2_ops_hccl
