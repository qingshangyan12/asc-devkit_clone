/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <algorithm>
#include <string>
#include <vector>

#include "alg_meta_registry.h"
#include "coll_alg_v2_exec_registry.h"
#include "external_alg_parser.h"
#include "gtest/gtest.h"

using namespace mc2_ops_hccl;

namespace {
// 合法外部名样本（30 个规范名的代表集 + 空格/保序变体）
// 变体条目（带空格 / [order.strong] 后缀）解析必须成功，且规范化后与对应规范名相等
const struct {
    const char* name;              // 输入（可为带空格/后缀的变体）
    const char* expectedCanonical; // 期望的规范化名（剥空格、去 [order.strong] 后缀）
} LEGAL_EXTERNAL_NAMES[] = {
    {"sole[mesh]", "sole[mesh]"},
    {"sole[nhr]", "sole[nhr]"},
    {"sole[mesh.chunk]", "sole[mesh.chunk]"},
    {"sole[mesh.one_shot]", "sole[mesh.one_shot]"},
    {"sole[mesh.two_shot]", "sole[mesh.two_shot]"},
    {"sole[mesh.two_shot.chunk]", "sole[mesh.two_shot.chunk]"},
    {"sole[mesh.single_channel]", "sole[mesh.single_channel]"},
    {"sole[mesh.muti_channel]", "sole[mesh.muti_channel]"},
    {"sole[nhr.muti_channel]", "sole[nhr.muti_channel]"},
    {"sequence[mesh,nhr]", "sequence[mesh,nhr]"},
    {"sequence[mesh,mesh]", "sequence[mesh,mesh]"},
    {"parallel[mesh,nhr]", "parallel[mesh,nhr]"},
    {"parallel[mesh,nhr.muti_channel]", "parallel[mesh,nhr.muti_channel]"},
    {"concur[mesh,nhr]", "concur[mesh,nhr]"},
    {"concur[mesh,nhr.muti_channel]", "concur[mesh,nhr.muti_channel]"},
    {"concur[mesh,mesh]", "concur[mesh,mesh]"},
    {"pipeline[mesh,nhr]", "pipeline[mesh,nhr]"},
    {"parallel[mesh, nhr]", "parallel[mesh,nhr]"},
    {"sole[mesh] [order.strong]", "sole[mesh]"},
    {"sequence[mesh, nhr, nhr]", "sequence[mesh,nhr,nhr]"},
};
} // namespace

TEST(ExtAlgParser, LegalNamesParseAndCanonicalize)
{
    for (const auto& legal : LEGAL_EXTERNAL_NAMES) {
        ExternalAlgSpec spec;
        std::string errMsg;
        ASSERT_TRUE(ParseExternalAlg(legal.name, spec, errMsg)) << "name=" << legal.name << " err=" << errMsg;
        EXPECT_EQ(spec.canonical(), std::string(legal.expectedCanonical)) << "name=" << legal.name;
    }
}

TEST(ExtAlgParser, SpacesEverywhereAreStripped)
{
    ExternalAlgSpec spec;
    std::string errMsg;
    ASSERT_TRUE(ParseExternalAlg("  parallel [ mesh , nhr . muti_channel ]  [ order.strong ] ", spec, errMsg));
    EXPECT_EQ(spec.canonical(), "parallel[mesh,nhr.muti_channel]");
    EXPECT_TRUE(spec.orderStrong);
    EXPECT_EQ(spec.executor, ExternalExecutor::PARALLEL);
    ASSERT_EQ(spec.layers.size(), 2U);
    EXPECT_EQ(spec.layers[0].atom, "mesh");
    EXPECT_TRUE(spec.layers[0].attrs.empty());
    EXPECT_EQ(spec.layers[1].atom, "nhr");
    ASSERT_EQ(spec.layers[1].attrs.size(), 1U);
    EXPECT_EQ(spec.layers[1].attrs[0], "muti_channel");
}

TEST(ExtAlgParser, OrderStrongIsParsedButNotInCanonical)
{
    ExternalAlgSpec spec;
    std::string errMsg;
    ASSERT_TRUE(ParseExternalAlg("sole[mesh] [order.strong]", spec, errMsg));
    EXPECT_TRUE(spec.orderStrong);
    EXPECT_EQ(spec.canonical(), "sole[mesh]");
}

TEST(ExtAlgParser, IllegalNamesAreRejectedWithReason)
{
    struct Case {
        const char* name;
        std::string expectedErrFragment;
    };
    const Case cases[] = {
        {"", "empty"},
        {"ring[mesh]", "executor"},
        {"sole[ring]", "atom"},
        {"sole[mesh.bigstep]", "attribute"},
        {"sole[mesh.multi_channel]", "attribute"},
        {"sole[mesh", "missing"},
        {"sole[]", "layer"},
        {"sole[mesh,]", "layer"},
        {"sole[mesh,nhr,nhr,nhr]", "layer"},
        {"sole[mesh] [order.weak]", "suffix"},
        {"parallel[mesh,nhr]]", "suffix"},
        {"sole[mesh.two_shot.chunk.chunk]", ""},
        {"sole[mesh.]", "empty token"}, // 点号结尾
        {"[mesh]", "executor"},         // 空 executor
        {"sole[,,]", "empty layer"},    // 重复逗号
        {"\t", "empty"},                // 纯空白
    };
    for (const Case& c : cases) {
        ExternalAlgSpec spec;
        std::string errMsg;
        const bool ok = ParseExternalAlg(c.name, spec, errMsg);
        if (c.expectedErrFragment.empty()) {
            EXPECT_TRUE(ok) << "name=" << c.name << " err=" << errMsg;
        } else {
            EXPECT_FALSE(ok) << "name=" << c.name;
            EXPECT_NE(errMsg.find(c.expectedErrFragment), std::string::npos) << "name=" << c.name << " err=" << errMsg;
        }
    }
}

TEST(ExtAlgClassify, FourStates)
{
    ExternalAlgSpec spec;
    std::string bareName;
    std::string errMsg;

    EXPECT_EQ(ClassifyForcedAlgConfig("", bareName, spec, errMsg), ForcedAlgKind::NONE);
    EXPECT_EQ(ClassifyForcedAlgConfig("allgather=level0:fullmesh", bareName, spec, errMsg), ForcedAlgKind::LEGACY);
    EXPECT_EQ(ClassifyForcedAlgConfig("InsReduceScatterMesh1D", bareName, spec, errMsg), ForcedAlgKind::BARE_NAME);
    EXPECT_EQ(bareName, "InsReduceScatterMesh1D");
    EXPECT_EQ(ClassifyForcedAlgConfig("parallel[mesh, nhr]", bareName, spec, errMsg), ForcedAlgKind::EXTERNAL);
    EXPECT_TRUE(errMsg.empty());
    EXPECT_EQ(spec.canonical(), "parallel[mesh,nhr]");
    EXPECT_EQ(ClassifyForcedAlgConfig("sole[ring]", bareName, spec, errMsg), ForcedAlgKind::EXTERNAL);
    EXPECT_FALSE(errMsg.empty());
}

TEST(ExtAlgV2Registry, EnumerateRegisteredTags)
{
    auto& registry = CollAlgExecRegistryV2::Instance();
    const std::vector<std::string> agTags = registry.GetRegisteredTags(HcclCMDType::HCCL_CMD_ALLGATHER);
    // 本目标全量编译（-DMC2_CLIENT_ENABLE_CCU=1，无 AICPU_COMPILE 守卫裁剪），AG 应为 12 条：
    // 9 条 AICPU 注册（含 omnipipe）+ 3 条 CCU 注册（CcuSchedAllGatherSoleMesh /
    // CcuSchedAllGatherParallelMeshNHRMultiLink / CcuSchedAllGatherConcurMeshNHRMultiLink）；
    // 此断言同时防止枚举接口漏实现
    EXPECT_FALSE(agTags.empty());
    for (const std::string& tag : agTags) {
        EXPECT_TRUE(registry.IsRegistered(HcclCMDType::HCCL_CMD_ALLGATHER, tag)) << tag;
    }
    // kfc_server/executor 未编入本目标且 AICPU_COMPILE/HCCL_CANN_COMPAT_850 均未定义，
    // 若意外编入将注册 CcuKfcServer——此断言是 GLOB 排除防线的测试侧副本
    EXPECT_TRUE(registry.GetRegisteredTags(HcclCMDType::HCCL_CMD_KFC_SERVER).empty());
}

TEST(ExtAlgMetaRegistry, RegisterAndQuery)
{
    // 用本目标已注册的 sidecar 行验证（Task 4 之前表为空则此用例先红）
    const AlgMeta* meta =
        AlgMetaRegistry::Instance().Get(HcclCMDType::HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterMesh1D");
    ASSERT_NE(meta, nullptr);
    EXPECT_EQ(meta->engine, AlgEngine::AICPU);
    EXPECT_EQ(std::string(meta->externalName), "sole[mesh]");
}

namespace {
const HcclCMDType COLL_CMDS[] = {
    HcclCMDType::HCCL_CMD_ALLGATHER, HcclCMDType::HCCL_CMD_REDUCE_SCATTER, HcclCMDType::HCCL_CMD_ALLREDUCE,
    HcclCMDType::HCCL_CMD_ALLTOALL,  HcclCMDType::HCCL_CMD_ALLTOALLV,
};
} // namespace

// 设计不变量 1（spec §14）：V2 注册表 ↔ AlgMetaRegistry 双向满射。本目标全量编译所有注册现场
// （15 个 executor 文件，-DMC2_CLIENT_ENABLE_CCU=1 且无 AICPU_COMPILE 守卫裁剪），可见全部
// 46 行 sidecar——按 cmd 分布：AG 12 / RS 16 / AR 8 / A2A 7 / A2AV 3。
TEST(ExtAlgSurjectivity, V2AndMetaRegistryAreBijective)
{
    for (HcclCMDType cmd : COLL_CMDS) {
        std::vector<std::string> v2Tags = CollAlgExecRegistryV2::Instance().GetRegisteredTags(cmd);
        std::vector<std::string> metaNames;
        for (const AlgMetaRow& row : AlgMetaRegistry::Instance().GetAll()) {
            if (row.cmd == cmd) {
                metaNames.push_back(row.registeredName);
            }
        }
        std::sort(v2Tags.begin(), v2Tags.end());
        std::sort(metaNames.begin(), metaNames.end());
        EXPECT_EQ(v2Tags, metaNames) << "cmd=" << static_cast<u32>(cmd);
    }
    // 各 cmd 精确分布锚（AG 12 / RS 16 / AR 8 / A2A 7 / A2AV 3 = 46），失败时定位更直接
    EXPECT_EQ(CollAlgExecRegistryV2::Instance().GetRegisteredTags(HcclCMDType::HCCL_CMD_ALLGATHER).size(), 12U);
    EXPECT_EQ(CollAlgExecRegistryV2::Instance().GetRegisteredTags(HcclCMDType::HCCL_CMD_REDUCE_SCATTER).size(), 16U);
    EXPECT_EQ(CollAlgExecRegistryV2::Instance().GetRegisteredTags(HcclCMDType::HCCL_CMD_ALLREDUCE).size(), 8U);
    EXPECT_EQ(CollAlgExecRegistryV2::Instance().GetRegisteredTags(HcclCMDType::HCCL_CMD_ALLTOALL).size(), 7U);
    EXPECT_EQ(CollAlgExecRegistryV2::Instance().GetRegisteredTags(HcclCMDType::HCCL_CMD_ALLTOALLV).size(), 3U);
    // 本目标全量编译（MC2_CLIENT_ENABLE_CCU=1，无 AICPU_COMPILE）：46 行精确锚
    size_t total = 0;
    for (HcclCMDType cmd : COLL_CMDS) {
        for (const AlgMetaRow& row : AlgMetaRegistry::Instance().GetAll()) {
            if (row.cmd == cmd) {
                total++;
            }
        }
    }
    EXPECT_EQ(total, 46U);
}
