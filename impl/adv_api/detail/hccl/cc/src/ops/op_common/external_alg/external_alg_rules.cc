/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "external_alg_rules.h"

#include "auto_selector_base.h"
#include "hccl_rank_graph.h"
#include "log.h"

namespace mc2_ops_hccl {

// 零侵入复用 AutoSelectorBase 的 IsTwoLevelNetLayer / IsLayerAllConnetedWithTopo
// （const 成员函数，类无状态，单实例即可；spec 开放项 3 起步方案，抽自由函数随改名批次）
namespace {
const AutoSelectorBase kRuleSelectorBase;

// hccl RS selector:30 / :23（reduce_scatter_auto_selector.cc）同名阈值
constexpr u64 RS_2P_DETOUR_DATA_SIZE = 4ULL * 1024 * 1024;                 // 4MB
constexpr u64 RS_1D_TWO_LEVEL_DATA_SIZE_THRESHOLD = 1536ULL * 1024 * 1024; // 1536MB
} // namespace

// A1 蓝本（hccl 对齐，用户拍板）:
//   hccl src/ops/reduce_scatter/selector/reduce_scatter_auto_selector.cc:483-485
//   hccl src/ops/all_gather/selector/all_gather_auto_selector.cc:338
//   growthring/notes/meetings/algname.md §1.6
// devkit RS:343-345 现状缺 2P 分支，按 hccl 补全。
bool CondZAxis(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam)
{
    if (topo == nullptr) {
        return false;
    }
    if (!kRuleSelectorBase.IsTwoLevelNetLayer(topo)) {
        return false;
    }
    const u64 dataSize = opParam.DataDes.count * DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    if (topo->userRankSize == 2U && dataSize >= RS_2P_DETOUR_DATA_SIZE) {
        return true;
    }
    return dataSize * topo->userRankSize > RS_1D_TWO_LEVEL_DATA_SIZE_THRESHOLD;
}

// A2 蓝本: devkit kernel ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem_peer_only.cc:38-44
//   （rankSize==2 且 channelCount==1）。选型期用 HcclRankGraphGetLinks 查对端链路数
//   （hccl_rank_graph.h:244）。查询前提不满足时保守不命中，裸名候选兜底。
bool CondPeerOnly(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam)
{
    if (topo == nullptr || topo->userRankSize != 2U) {
        return false;
    }
    if (opParam.hcclComm == nullptr || topo->netLayerDetails.netLayers.empty()) {
        return false;
    }
    CommLink* links = nullptr;
    uint32_t linkNum = 0;
    const uint32_t peerRank = (topo->userRank == 0U) ? 1U : 0U;
    const HcclResult ret = HcclRankGraphGetLinks(
        opParam.hcclComm, topo->netLayerDetails.netLayers[0], topo->userRank, peerRank, &links, &linkNum);
    if (ret != HCCL_SUCCESS) {
        HCCL_INFO(
            "[MC2_EXT_ALG] COND_PEER_ONLY query links failed, ret[%d], fallback to bare candidate.",
            static_cast<int>(ret));
        return false;
    }
    return linkNum == 1U;
}

// A3/A4 蓝本: hccl all_reduce_auto_selector.cc:427-434 / reduce_scatter_auto_selector.cc:377
//   （devkit RS:230 相同）。soft_reduce 优先级语义由"特化候选在前 + 裸名兜底"自动成立。
bool CondSpecialDt(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam)
{
    (void)topo;
    return Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD;
}

// B 族蓝本: hccl all_gather_auto_selector.cc:84/:369-370（devkit AG:176-183 相同）
bool FlagPcieMix(const TopoInfoWithNetLayerDetails* topo)
{
    if (topo == nullptr) {
        return false;
    }
    return topo->level0Topo == Level0Shape::MESH_1D_CLOS && topo->level0PcieMix &&
           !kRuleSelectorBase.IsLayerAllConnetedWithTopo(topo, 0U, CommTopo::COMM_TOPO_1DMESH);
}

// 蓝本: hccl all_gather_auto_selector.cc:451（isUBX；devkit AG:184+ 同构）
bool FlagUbx(const TopoInfoWithNetLayerDetails* topo)
{
    if (topo == nullptr) {
        return false;
    }
    return topo->level0Topo == Level0Shape::MESH_1D_CLOS && !topo->level0PcieMix;
}

// 蓝本: devkit AG:163-164（hccl AG:305 分支结构同构）
bool FlagMultiLevel(const TopoInfoWithNetLayerDetails* topo)
{
    if (topo == nullptr) {
        return false;
    }
    return topo->topoLevelNums > 1U && topo->level0Topo == Level0Shape::MESH_1D;
}

// B4 蓝本: TopoMatch1D 语义（topo_match_1d.cc:19-57，两仓同构）。A2A 裸名专用。
bool FlagFlat1D(const TopoInfoWithNetLayerDetails* topo)
{
    if (topo == nullptr) {
        return false;
    }
    return topo->level0Topo != Level0Shape::MESH_1D_CLOS;
}

// ① 蓝本: auto_selector_base.cc:25-65（HostDPUOnly 切 COMM_ENGINE_CPU 只留 *DPU；CCU_SCHED 留 CCU；
// AICPU/AICPU_TS 留 AICPU）
bool EngineMatch(AlgEngine candEngine, const OpParam& opParam)
{
    switch (candEngine) {
        case AlgEngine::DPU:
            return opParam.engine == CommEngine::COMM_ENGINE_CPU;
        case AlgEngine::CCU:
            return opParam.engine == CommEngine::COMM_ENGINE_CCU;
        case AlgEngine::AICPU:
        default:
            return opParam.engine == CommEngine::COMM_ENGINE_AICPU ||
                   opParam.engine == CommEngine::COMM_ENGINE_AICPU_TS;
    }
}

const VariantCond VARIANT_COND_TABLE[VARIANT_COND_COUNT] = {
    CondZAxis,     // COND_Z_AXIS
    CondPeerOnly,  // COND_PEER_ONLY
    CondSpecialDt, // COND_SPECIAL_DT
};

const MachineFlag MACHINE_FLAG_TABLE[MACHINE_FLAG_COUNT] = {
    FlagPcieMix,    // FLAG_PCIE_MIX
    FlagUbx,        // FLAG_UBX
    FlagMultiLevel, // FLAG_MULTI_LEVEL
    FlagFlat1D,     // FLAG_FLAT_1D
};

const char* VariantCondName(int id)
{
    static const char* NAMES[VARIANT_COND_COUNT] = {"COND_Z_AXIS", "COND_PEER_ONLY", "COND_SPECIAL_DT"};
    return (id >= 0 && id < VARIANT_COND_COUNT) ? NAMES[id] : "n/a";
}

const char* MachineFlagName(int id)
{
    static const char* NAMES[MACHINE_FLAG_COUNT] = {"FLAG_PCIE_MIX", "FLAG_UBX", "FLAG_MULTI_LEVEL", "FLAG_FLAT_1D"};
    return (id >= 0 && id < MACHINE_FLAG_COUNT) ? NAMES[id] : "n/a";
}
} // namespace mc2_ops_hccl
