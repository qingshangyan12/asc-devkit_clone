/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef EXTERNAL_ALG_CCU_EXT_NAMES_H
#define EXTERNAL_ALG_CCU_EXT_NAMES_H

#include <set>
#include <string>
#include <utility>

#include "external_alg_parser.h"

namespace mc2_ops_hccl {

// 闸门 A 镜像对表：有 CCU 候选的 (opType数值, 规范化外部名) 集合。
// 背景：tiling 库与 ops 库（sidecar 注册表）是不同链接目标，tiling 侧无法读 AlgMetaRegistry，
// 只能镜像。本表由 UT 漂移断言（ExtAlgGateA.MirrorMatchesSidecar）守护——sidecar 加/改 CCU 行
// 而漏改此表时 CI 必红。
// 内容来源：46 行 sidecar 中 engine==CCU 的 12 行归并（opType 数值见 hccl_common.h:
// ALLREDUCE=2 / ALLGATHER=6 / REDUCE_SCATTER=7 / ALLTOALLV=8 / ALLTOALL=10）。
// 边界：本表不随 MC2_CLIENT_ENABLE_CCU 裁剪——裁剪构建下 mc2 侧 CCU sidecar 不注册而本表
// 仍 11 项（当前无害：101 消费点全在 CCU 链，链不存在即无人读）。
inline const std::set<std::pair<uint32_t, std::string>>& CcuExternalNameSet()
{
    static const std::set<std::pair<uint32_t, std::string>> kSet = {
        {6U, "sole[mesh]"},                       // AG: CcuSchedAllGatherSoleMesh
        {6U, "parallel[mesh,nhr.multi_channel]"}, // AG: CcuSchedAllGatherParallelMeshNHRMultiLink
        {6U, "concur[mesh,nhr.multi_channel]"},   // AG: CcuSchedAllGatherConcurMeshNHRMultiLink
        {7U, "sole[mesh]"},                       // RS: CcuSchedReduceScatterSoleMesh(+PeerOnly)
        {7U, "sole[nhr.multi_channel]"},          // RS: CcuSchedReduceScatterSoleNHRMultiLink
        {7U, "concur[mesh,nhr.multi_channel]"},   // RS: CcuSchedReduceScatterConcurMeshNHRMultiLink
        {2U, "sole[mesh]"},                       // AR: CcuSchedAllReduceSoleMesh
        {10U, "sole[mesh]"},                      // A2A: CcuSchedAllToAllSoleMesh
        {10U, "sole[mesh.multi_channel]"},        // A2A: CcuSchedAllToAllMesh1DMultiJetty
        {10U, "concur[mesh,mesh]"},               // A2A: CcuSchedAllToAllSoleMeshConcurrent
        {8U, "sole[mesh]"},                       // A2AV: CcuSchedAllToAllVSoleMesh
    };
    return kSet;
}

} // namespace mc2_ops_hccl
#endif // EXTERNAL_ALG_CCU_EXT_NAMES_H
