/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "all_gather_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {
constexpr u64 AG_2D_SMALL_DATA_SIZE = 1024 * 1024;
constexpr u32 MAX_RANK_NUM_FOR_CONCURRENT_ALGO = 4;

SelectorStatus AllGatherAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllGatherAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    HCCL_DEBUG("[AllGatherAutoSelector] CcuSchedAllGatherSoleMesh is not a ccu_ms algorithm.");
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AllGatherAutoSelector::SelectCcuScheduleUBXAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, std::string& selectAlgName, const u64 dataSize) const
{
    if (topoInfo->level0PcieMix) {
        return SelectorStatus::NOT_MATCH;
    }
    bool meshNumEqualsClosNum = false;
    CHK_PRT_RET(
        CheckMeshNumEqualToClosNum(topoInfo, meshNumEqualsClosNum) != HCCL_SUCCESS,
        HCCL_DEBUG("[AllGatherAutoSelector] CheckMeshNumEqualToClosNum failed."), SelectorStatus::NOT_MATCH);
    if (dataSize <= SMALL_COUNT_512KB) {
        selectAlgName = "CcuSchedAllGatherSoleMesh";
        return SelectorStatus::MATCH;
    }
    if (meshNumEqualsClosNum && topoInfo->userRankSize <= MAX_RANK_NUM_FOR_CONCURRENT_ALGO) {
        selectAlgName = "CcuSchedAllGatherConcurMeshNHRMultiLink";
        return SelectorStatus::MATCH;
    }
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AllGatherAutoSelector::SelectCcuScheduleLevel0Algo(
    const TopoInfoWithNetLayerDetails* topoInfo, std::string& selectAlgName, const u64 dataSize) const
{
    if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
        return SelectCcuScheduleUBXAlgo(topoInfo, selectAlgName, dataSize);
    }
    if (topoInfo->level0Topo != Level0Shape::MESH_1D || topoInfo->level0MeshType != Level0MeshType::SINGLE_DIE) {
        HCCL_DEBUG(
            "[AllGatherAutoSelector] level0Topo[%d], level0MeshType[%d] is not supported for ccu schedule mode.",
            topoInfo->level0Topo, topoInfo->level0MeshType);
        return SelectorStatus::NOT_MATCH;
    }

    selectAlgName = "CcuSchedAllGatherSoleMesh";
    HCCL_DEBUG("[AllGatherAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllGatherAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllGatherAutoSelector][%s] start", __func__);
    (void)configAlgMap;
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    if (IsInputOutputOverlap(opParam)) {
        HCCL_WARNING("[Algo][AllGatherAutoSelector] ccu schedule does not support inplace allgather.");
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->topoLevelNums != 1) {
        // Temporary KFC convergence: only a single topology level is enabled for this CCU schedule path.
        HCCL_DEBUG(
            "[AllGatherAutoSelector] topoLevelNums[%u] is not supported for ccu schedule mode.",
            topoInfo->topoLevelNums);
        return SelectorStatus::NOT_MATCH;
    }
    return SelectCcuScheduleLevel0Algo(topoInfo, selectAlgName, dataSize);
}

SelectorStatus AllGatherAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllGatherAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    HCCL_INFO(
        "[AllGatherAutoSelector][SelectAicpuAlgo] topoLevelNums=[%d], deviceNumPerModule=[%d], level0Topo=[%d]",
        topoInfo->topoLevelNums, topoInfo->deviceNumPerModule, topoInfo->level0Topo);
    if (topoInfo->topoLevelNums > 1) {
        // Level1Nhr 已在 CalcTopoShape 中设置（GCD==1 时为 true）
        if (topoInfo->Level1Nhr) {
            selectAlgName = "InsAllGatherNHR";
            HCCL_INFO("[AllGatherAutoSelector] Level1Nhr=true, select [%s]", selectAlgName.c_str());
        } else if (topoInfo->Level0Nhr) {
            selectAlgName = "InsAllGatherNHR"; // 预留给NHRNHR
        } else if (topoInfo->netLayerDetails.localNetInsSizeOfLayer[0] == 1) {
            selectAlgName = "InsAllGatherNHR";
        } else if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
            selectAlgName = "InsAllGatherParallelMesh1DNHR";
        } else {
            HCCL_ERROR("[AllGatherAutoSelector] topo not match");
            return SelectorStatus::NOT_MATCH;
        }
    } else {
        if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
            selectAlgName = "AicpuAllGatherSoleMesh";
        } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
            // UBX机型
            bool isMeshNumEqualToClosNum = false;
            bool isClosNumMultipleOfMeshNum = false;
            CHK_PRT_RET(
                CheckMeshNumEqualToClosNum(topoInfo, isMeshNumEqualToClosNum) != HCCL_SUCCESS,
                HCCL_ERROR("[AllGatherAutoSelector] CheckMeshNumEqualToClosNum failed."), SelectorStatus::NOT_MATCH);
            CHK_PRT_RET(
                CheckClosNumMultipleOfMeshNum(topoInfo, isClosNumMultipleOfMeshNum) != HCCL_SUCCESS,
                HCCL_ERROR("[AllGatherAutoSelector] CheckClosNumMultipleOfMeshNum failed."), SelectorStatus::NOT_MATCH);
            if (isMeshNumEqualToClosNum && (topoInfo->userRankSize <= MAX_RANK_NUM_FOR_CONCURRENT_ALGO)) {
                if (dataSize > SMALL_COUNT_512KB) {
                    selectAlgName = "AicpuAllGatherConcurMeshNHR";
                } else {
                    selectAlgName = "AicpuAllGatherSoleMesh";
                }
            } else if (isClosNumMultipleOfMeshNum && dataSize > SMALL_COUNT_512KB) {
                selectAlgName = "InsAllGatherParallelMesh1DNHRUBX";
            } else {
                selectAlgName = "InsAllGatherNHR";
            }
        } else if (topoInfo->level0Topo == Level0Shape::CLOS) {
            selectAlgName = "InsAllGatherNHR";
        } else {
            HCCL_ERROR("[AllGatherAutoSelector] topo not match");
            return SelectorStatus::NOT_MATCH;
        }
    }
    HCCL_DEBUG("[AllGatherAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllGatherAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllGatherAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    (void)opParam;

    selectAlgName = "AivAllGatherMesh1D";
    HCCL_DEBUG("[AllGatherAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus AllGatherAutoSelector::SelectDPUAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[AllGatherAutoSelector][%s] start, topoInfo topoLevelNums[%u]", __func__, topoInfo->topoLevelNums);
    if (topoInfo->topoLevelNums > 1) {
        if ((topoInfo->netLayerDetails.localNetInsSizeOfLayer[0] == 1) ||
            (topoInfo->level0Topo == Level0Shape::MESH_1D)) {
            selectAlgName = "InsAllGatherMeshNhrDPU";
            HCCL_DEBUG("[AllGatherAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
            return SelectorStatus::MATCH;
        }
    }
    HCCL_DEBUG("[AllGatherAutoSelector][%s] end", __func__);
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLGATHER, 18, AllGatherAutoSelector);

} // namespace mc2_ops_hccl
