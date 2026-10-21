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
namespace {
constexpr u32 CONCURRENT_RANK_LIMIT = 4U;
constexpr u32 A2AV_CCU_MAX_RANK_SIZE = 64U; // 与 hccl A2AV_CCU_MAX_RANK_SIZE 一致
constexpr u32 TOPO_LEVEL_NUM_3 = 3U;
} // namespace

SelectorStatus AlltoAllVAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)opParam;
    (void)configAlgMap;
    CHK_PRT_RET(
        topoInfo == nullptr, HCCL_ERROR("[AlltoAllVAutoSelector] topoInfo is nullptr."), SelectorStatus::NOT_MATCH);
    if (topoInfo->userRankSize <= 1U) {
        HCCL_WARNING(
            "[AlltoAllVAutoSelector] rankSize[%u] is not supported by local AICPU AllToAllV.", topoInfo->userRankSize);
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->level0Topo == Level0Shape::MESH_1D || topoInfo->level0Topo == Level0Shape::CLOS) {
        selectAlgName = "AicpuAllToAllVSoleMesh";
        HCCL_INFO("[AlltoAllVAutoSelector] Algo match[%s].", selectAlgName.c_str());
        return SelectorStatus::MATCH;
    } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
        // PCIE-SW定制机型，使用mesh1d算法
        if (topoInfo->level0PcieMix) {
            selectAlgName = "AicpuAllToAllVSoleMesh";
            HCCL_INFO("[AlltoAllVAutoSelector][%s] Algo match[%s]", __func__, selectAlgName.c_str());
            return SelectorStatus::MATCH;
        }
        // 对齐 hccl：多级 MESH_1D_CLOS 走 SoleMesh
        if (topoInfo->topoLevelNums > 1U) {
            selectAlgName = "AicpuAllToAllVSoleMesh";
            HCCL_INFO("[AlltoAllVAutoSelector] Algo match[%s].", selectAlgName.c_str());
            return SelectorStatus::MATCH;
        }
    }
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS || topoInfo->topoLevelNums != 1U || topoInfo->level0PcieMix) {
        HCCL_WARNING(
            "[AlltoAllVAutoSelector] rankSize[%u], level0Topo[%u] is not supported by local AICPU AllToAllV.",
            topoInfo->userRankSize, static_cast<u32>(topoInfo->level0Topo));
        return SelectorStatus::NOT_MATCH;
    }
    bool isMeshNumEqualToClosNum = false;
    CHK_PRT_RET(
        CheckMeshNumEqualToClosNum(topoInfo, isMeshNumEqualToClosNum) != HCCL_SUCCESS,
        HCCL_ERROR("[AlltoAllVAutoSelector] failed to compare Mesh/CLOS rank counts."), SelectorStatus::NOT_MATCH);
    CHK_PRT_RET(
        !isMeshNumEqualToClosNum || topoInfo->userRankSize > CONCURRENT_RANK_LIMIT,
        HCCL_WARNING("[AlltoAllVAutoSelector] topology does not satisfy Concurrent constraints."),
        SelectorStatus::NOT_MATCH);
    selectAlgName = "AicpuAllToAllVSoleMeshConcurrent";
    HCCL_INFO("[AlltoAllVAutoSelector] Algo match[%s].", selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

// 对齐 hccl 分支框架：已注册算法 MATCH，未注册算法设名后 NOT_MATCH（报错由框架统一处理）。
SelectorStatus AlltoAllVAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    HCCL_DEBUG("[AlltoAllVAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    if (topoInfo->level2UbRtp) {
        HCCL_INFO("[AlltoAllVAutoSelector][%s] ccu schedule is not supported with level2UbRtp.", __func__);
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->topoLevelNums >= TOPO_LEVEL_NUM_3) {
        HCCL_INFO(
            "[AlltoAllVAutoSelector][%s] ccu schedule is not supported when topoLevelNums >= 3(levelNum[%u]).",
            __func__, topoInfo->topoLevelNums);
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->topoLevelNums > 1U) {
        if (topoInfo->level0Topo != Level0Shape::CLOS) {
            // hccl 侧 int8 判断借 union 重叠读 all2AllDataDes.sendType，此处直接用 all2AllVDataDes 语义等价
            if (opParam.all2AllVDataDes.sendType == HcclDataType::HCCL_DATA_TYPE_INT8) {
                HCCL_WARNING("[Algo][AlltoAllVAutoSelector] int8 is not supported yet for ccu_schedule mode.");
                return SelectorStatus::NOT_MATCH;
            }
            if (topoInfo->userRankSize > A2AV_CCU_MAX_RANK_SIZE) {
                HCCL_WARNING("[Algo][AlltoAllVAutoSelector] rankSize > 64 is not supported yet for ccu_schedule mode.");
                return SelectorStatus::NOT_MATCH;
            }
            if (topoInfo->userRankSize <= CONCURRENT_RANK_LIMIT) {
                selectAlgName = "CcuSchedAllToAllVSoleMesh";
                HCCL_INFO("[AlltoAllVAutoSelector][%s] Algo match[%s].", __func__, selectAlgName.c_str());
                return SelectorStatus::MATCH;
            }
            selectAlgName = "CcuSchedAllToAllVSoleMeshMultiLink";
            HCCL_WARNING(
                "[AlltoAllVAutoSelector][%s] algo[%s] not registered, not supported.", __func__, selectAlgName.c_str());
            return SelectorStatus::NOT_MATCH;
        }
        HCCL_WARNING("[Algo][AlltoAllVAutoSelector] clos is not supported for ccu_schedule mode.");
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_REGULAR) {
            selectAlgName = "CcuSchedAllToAllVSoleMesh2Die";
            HCCL_WARNING(
                "[AlltoAllVAutoSelector][%s] algo[%s] not registered, not supported.", __func__, selectAlgName.c_str());
            return SelectorStatus::NOT_MATCH;
        }
        if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_NOT_REGULAR) {
            HCCL_DEBUG("[AlltoAllVAutoSelector][%s] TWO_DIE_NOT_REGULAR not match", __func__);
            return SelectorStatus::NOT_MATCH;
        }
        selectAlgName = "CcuSchedAllToAllVSoleMesh";
        HCCL_INFO("[AlltoAllVAutoSelector][%s] Algo match[%s].", __func__, selectAlgName.c_str());
        return SelectorStatus::MATCH;
    }
    if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
        // PCIE-SW定制机型，Mesh无法链接全卡时需跨pcie链路，仅全连通1DMESH支持ccu
        if (topoInfo->level0PcieMix) {
            if (IsLayerAllConnetedWithTopo(topoInfo, 0, CommTopo::COMM_TOPO_1DMESH)) {
                selectAlgName = "CcuSchedAllToAllVSoleMesh";
                HCCL_INFO("[AlltoAllVAutoSelector][%s] Algo match[%s].", __func__, selectAlgName.c_str());
                return SelectorStatus::MATCH;
            }
            HCCL_WARNING("[AlltoAllVAutoSelector] pcie mixed topo is not supported yet for ccu schedule mode.");
            return SelectorStatus::NOT_MATCH;
        }
        bool isMeshNumEqualToClosNum = false;
        CHK_PRT_RET(
            CheckMeshNumEqualToClosNum(topoInfo, isMeshNumEqualToClosNum) != HCCL_SUCCESS,
            HCCL_DEBUG("[AlltoAllVAutoSelector] CheckMeshNumEqualToClosNum failed."), SelectorStatus::NOT_MATCH);
        if (isMeshNumEqualToClosNum && topoInfo->userRankSize <= CONCURRENT_RANK_LIMIT) {
            selectAlgName = "CcuSchedAllToAllVSoleMeshConcurrent";
            HCCL_WARNING(
                "[AlltoAllVAutoSelector][%s] algo[%s] not registered, not supported.", __func__, selectAlgName.c_str());
            return SelectorStatus::NOT_MATCH;
        }
        selectAlgName = "CcuSchedAllToAllVSoleMeshMultiJetty";
        HCCL_WARNING(
            "[AlltoAllVAutoSelector][%s] algo[%s] not registered, not supported.", __func__, selectAlgName.c_str());
        return SelectorStatus::NOT_MATCH;
    }
    HCCL_DEBUG("[AlltoAllVAutoSelector] algo is not supported yet for ccu_schedule mode.");
    return SelectorStatus::NOT_MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_ALLTOALLV, 18, AlltoAllVAutoSelector);
} // namespace mc2_ops_hccl
