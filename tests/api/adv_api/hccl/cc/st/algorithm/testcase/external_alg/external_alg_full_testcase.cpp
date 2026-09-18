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
#include <string>

#include "external_alg_parser.h"
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

// 改造点①: 四态识别（P4 兼容性: NONE/LEGACY/BARE_NAME 行为与旧 GetForcedAlgName 逐字节等价）
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
