/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cstdio>
#include <set>
#include <string>
#include <utility>

#include "alg_meta_registry.h"
#include "external_alg_ccu_ext_names.h"
#include "external_alg_parser.h"
#include "external_alg_resolver.h"
#include "external_alg_rules.h"
#include "gtest/gtest.h"
#include "hccl_alloc_ctx_res.h"

using namespace mc2_ops_hccl;

namespace {
Mc2CcTilingInner MakeTiling(const char* algConfig, uint32_t opType = 6U /*ALLGATHER*/)
{
    Mc2CcTilingInner tiling{};
    tiling.opType = opType;
    tiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED);
    (void)snprintf(tiling.algConfig, sizeof(tiling.algConfig), "%s", algConfig);
    return tiling;
}
} // namespace

// 设计不变量 1（spec §14/§21.1）：CCU 候选 ⊆ algorithmMap（三名单一致性：
// sidecar 注册表 / tiling 白名单 / algorithmMap——前两者由 GateA 镜像断言与
// tiling 行为用例分别守护，此处锚定第三份名单）
TEST(ExtAlgFull, CcuCandidatesAreInAlgorithmMap)
{
    for (const AlgMetaRow& row : AlgMetaRegistry::Instance().GetAll()) {
        if (row.meta.engine != AlgEngine::CCU) {
            continue;
        }
        EXPECT_NE(algorithmMap.find(row.registeredName), algorithmMap.end())
            << row.registeredName << " missing in algorithmMap";
    }
}

// 改造点①: 四态识别（P4 兼容性: NONE/LEGACY/BARE_NAME 行为与旧两态强制名接口逐字节等价）
TEST(ExtAlgForced, FourStateClassification)
{
    std::string algName;
    ExternalAlgSpec spec;
    std::string errMsg;

    Mc2CcTilingInner empty = MakeTiling("");
    EXPECT_EQ(GetForcedAlg(&empty, algName, spec, errMsg), ForcedAlgKind::NONE);

    Mc2CcTilingInner legacy = MakeTiling("allgather=level0:fullmesh");
    EXPECT_EQ(GetForcedAlg(&legacy, algName, spec, errMsg), ForcedAlgKind::LEGACY);

    Mc2CcTilingInner bare = MakeTiling("InsAllGatherMesh1D");
    EXPECT_EQ(GetForcedAlg(&bare, algName, spec, errMsg), ForcedAlgKind::BARE_NAME);
    EXPECT_EQ(algName, "InsAllGatherMesh1D");

    Mc2CcTilingInner ext = MakeTiling("parallel[mesh, nhr]");
    EXPECT_EQ(GetForcedAlg(&ext, algName, spec, errMsg), ForcedAlgKind::EXTERNAL);
    EXPECT_TRUE(errMsg.empty());
    EXPECT_EQ(spec.canonical(), "parallel[mesh,nhr]");

    // 解析失败: spec 复位无效 + errMsg 非空（ClassifyForcedAlgConfig 契约）。
    // 复位 spec 的 canonical() 是 "sole[]"（executor 默认 SOLE）而非空串，
    // 失败态断言用默认值判据: layers 空 + orderStrong 复位
    Mc2CcTilingInner bad = MakeTiling("sole[ring]");
    EXPECT_EQ(GetForcedAlg(&bad, algName, spec, errMsg), ForcedAlgKind::EXTERNAL);
    EXPECT_FALSE(errMsg.empty());
    EXPECT_TRUE(spec.layers.empty());
    EXPECT_FALSE(spec.orderStrong);

    EXPECT_EQ(GetForcedAlg(nullptr, algName, spec, errMsg), ForcedAlgKind::NONE);
}

// hccl_mc2.cc 内部自由函数，专属目标已编入该翻译单元，符号可见
bool CheckCcuAlgorithmsRegistered(const void* ccTilingList[], uint32_t tilingNum);

// 闸门 B: 外部名按 CCU 候选子集过双名单；AICPU-only 外部名被拒
TEST(ExtAlgGateB, ExternalNameGate)
{
    // sole[mesh] AG 有 CCU 候选 CcuSchedAllGatherSoleMesh（algorithmMap + V2 注册表均在）→ 过
    Mc2CcTilingInner agSoleMesh = MakeTiling("sole[mesh]", 6U);
    const void* goodList[] = {&agSoleMesh};
    EXPECT_TRUE(CheckCcuAlgorithmsRegistered(goodList, 1U));

    // AG pipeline[mesh,nhr] 仅 AICPU 候选 → 拒
    Mc2CcTilingInner agPipeline = MakeTiling("pipeline[mesh,nhr]", 6U);
    const void* badList[] = {&agPipeline};
    EXPECT_FALSE(CheckCcuAlgorithmsRegistered(badList, 1U));

    // 语法错 → 拒
    Mc2CcTilingInner badSyntax = MakeTiling("sole[ring]", 6U);
    const void* badSyntaxList[] = {&badSyntax};
    EXPECT_FALSE(CheckCcuAlgorithmsRegistered(badSyntaxList, 1U));

    // 裸名/空名/legacy 现状语义不变
    Mc2CcTilingInner bare = MakeTiling("CcuSchedAllGatherSoleMesh", 6U);
    const void* bareList[] = {&bare};
    EXPECT_TRUE(CheckCcuAlgorithmsRegistered(bareList, 1U));
    Mc2CcTilingInner empty = MakeTiling("", 6U);
    const void* emptyList[] = {&empty};
    EXPECT_FALSE(CheckCcuAlgorithmsRegistered(emptyList, 1U));
    Mc2CcTilingInner legacy = MakeTiling("allgather=level0:fullmesh", 6U);
    const void* legacyList[] = {&legacy};
    EXPECT_FALSE(CheckCcuAlgorithmsRegistered(legacyList, 1U));
}

// 闸门 A 镜像漂移断言：镜像对表 ↔ sidecar 真值（engine==CCU 的 (opType, canonical外部名) 集合）逐项相等
// 漂移时同步修改 external_alg_ccu_ext_names.h 的 CcuExternalNameSet（sidecar 加/改 CCU 行时）
TEST(ExtAlgGateA, MirrorMatchesSidecar)
{
    std::set<std::pair<uint32_t, std::string>> truth;
    for (const AlgMetaRow& row : AlgMetaRegistry::Instance().GetAll()) {
        if (row.meta.engine != AlgEngine::CCU) {
            continue;
        }
        truth.insert(std::make_pair(static_cast<uint32_t>(row.cmd), std::string(row.meta.externalName)));
    }
    EXPECT_EQ(CcuExternalNameSet(), truth);
}

// 失败路径三要素（设计 §10/§12）：报错必须含 algConfig 原文/候选或定名/原因。
// 闸门 B 行为层：语法错 / AICPU-only 候选 → 拒；errMsg 内容层：外部名原文必在。
TEST(ExtAlgFailure, ErrorMessagesCarryThreeElements)
{
    // 闸门 B 行为断言
    Mc2CcTilingInner rsMeshChunk = MakeTiling("sole[mesh.chunk]", 7U); // RS: AICPU-only 候选
    const void* badList[] = {&rsMeshChunk};
    EXPECT_FALSE(CheckCcuAlgorithmsRegistered(badList, 1U));

    // GetForcedAlg 层 errMsg 三要素：解析失败消息含原文
    Mc2CcTilingInner bad = MakeTiling("sole[ring]", 6U);
    std::string algName;
    ExternalAlgSpec spec;
    std::string errMsg;
    EXPECT_EQ(GetForcedAlg(&bad, algName, spec, errMsg), ForcedAlgKind::EXTERNAL);
    EXPECT_NE(errMsg.find("sole[ring]"), std::string::npos); // 要素1: 原文
    EXPECT_NE(errMsg.find("atom"), std::string::npos);       // 要素3: 原因（词表外原子）

    // resolver 层 errMsg 三要素：候选为空/规则全灭消息含原文与原因分叉
    ExternalAlgSpec okSpec;
    std::string parseErr;
    ASSERT_TRUE(ParseExternalAlg("pipeline[mesh,nhr,nhr]", okSpec, parseErr));
    OpParam op{};
    op.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    op.engine = CommEngine::COMM_ENGINE_AICPU_TS;
    TopoInfoWithNetLayerDetails flat{};
    std::string resolvedName;
    EXPECT_EQ(ResolveExternalAlg(op, &flat, okSpec, resolvedName, errMsg), ResolveStatus::ERROR_INVALID);
    EXPECT_NE(errMsg.find("pipeline[mesh,nhr,nhr]"), std::string::npos);                // 要素1
    EXPECT_NE(errMsg.find("no candidate registered in this build"), std::string::npos); // 要素3 分叉文案

    ASSERT_TRUE(ParseExternalAlg("parallel[mesh,nhr]", okSpec, parseErr));
    EXPECT_EQ(ResolveExternalAlg(op, &flat, okSpec, resolvedName, errMsg), ResolveStatus::ERROR_INVALID);
    EXPECT_NE(errMsg.find("all candidates rejected by rules"), std::string::npos); // 全灭分叉文案
}
