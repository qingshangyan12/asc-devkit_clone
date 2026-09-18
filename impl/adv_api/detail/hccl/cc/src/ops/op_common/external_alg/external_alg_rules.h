/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef EXTERNAL_ALG_RULES_H
#define EXTERNAL_ALG_RULES_H

#include "alg_meta_registry.h" // AlgEngine
#include "alg_param.h"         // OpParam / TopoInfoWithNetLayerDetails

namespace mc2_ops_hccl {

// ② 变体条件槽位 ID（AlgMeta::variantCondId 取值）。-1 = 无条件。
enum VariantCondId {
    COND_NONE = -1,
    COND_Z_AXIS = 0, // A1: 两层网 + (2P大数据 ∥ 总量超阈值) -> ZAxisDetour 候选
    COND_PEER_ONLY,  // A2: rankSize==2 且对端仅 1 条链路 -> PeerOnly 候选
    COND_SPECIAL_DT, // A3/A4: 64bit dtype ∥ PROD -> soft_reduce(AicpuReduce) 候选
    VARIANT_COND_COUNT
};

// ②' 机型 flag 槽位 ID（AlgMeta::machineFlagId 取值）。-1 = 无机型要求。
enum MachineFlagId {
    FLAG_NONE = -1,
    FLAG_PCIE_MIX = 0, // B 族: MESH_1D_CLOS + pcieMix + level0 非全互联
    FLAG_UBX,          // B 族: MESH_1D_CLOS 且非 pcieMix
    FLAG_MULTI_LEVEL,  // B 族: 多层 + level0Topo==MESH_1D
    FLAG_FLAT_1D,      // B4: level0Topo != MESH_1D_CLOS（A2A 裸名专用）
    MACHINE_FLAG_COUNT
};

// ---- ② 变体条件谓词（蓝本对齐 hccl，见 .cc 逐条注释）----
using VariantCond = bool (*)(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam);
// ---- ②' 机型 flag 谓词 ----
using MachineFlag = bool (*)(const TopoInfoWithNetLayerDetails* topo);

bool CondZAxis(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam);
bool CondPeerOnly(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam);
bool CondSpecialDt(const TopoInfoWithNetLayerDetails* topo, const OpParam& opParam);
bool FlagPcieMix(const TopoInfoWithNetLayerDetails* topo);
bool FlagUbx(const TopoInfoWithNetLayerDetails* topo);
bool FlagMultiLevel(const TopoInfoWithNetLayerDetails* topo);
bool FlagFlat1D(const TopoInfoWithNetLayerDetails* topo);

// ① 引擎匹配（蓝本 auto_selector_base.cc:25-65 引擎分叉语义）
bool EngineMatch(AlgEngine candEngine, const OpParam& opParam);

// enum 下标函数指针表（规则两层结构：候选只带 ID，ID->函数绑定集中于此）
extern const VariantCond VARIANT_COND_TABLE[VARIANT_COND_COUNT];
extern const MachineFlag MACHINE_FLAG_TABLE[MACHINE_FLAG_COUNT];
const char* VariantCondName(int id); // 日志可打印规则名
const char* MachineFlagName(int id);

} // namespace mc2_ops_hccl
#endif // EXTERNAL_ALG_RULES_H
