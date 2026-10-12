/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include <vector>

#include "aicpu_fixture.h"
#include "alg_param.h"

using namespace HcclSim::AicpuSt;

namespace {
// 链路级冒烟：验证 fixture 全链（selector→资源→HcclExecOp→Orchestrate→任务图非空）
class AicpuStSmokeFixture : public AicpuStFixture {
protected:
    void PrepareOpParam(
        OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem) override
    {
        (void)rankSize;
        opParam.DataDes.count = scenario.count;
        opParam.inputPtr = mem.inputPtr;
        opParam.outputPtr = mem.outputPtr;
        opParam.inputSize = mem.inputBytes;
        opParam.outputSize = mem.outputBytes;
    }

    std::vector<uint8_t> BuildExpectedOutput(uint32_t rankSize, const AicpuStScenario& scenario, RankId rank) override
    {
        // 冒烟阶段不比对数据（Task 6 由 adapter 提供真实 golden）
        (void)rankSize;
        (void)scenario;
        (void)rank;
        return {};
    }
};
} // namespace

TEST_F(AicpuStSmokeFixture, ChainSmoke_AllGatherMesh1D_2Rank_Int8)
{
    AicpuStScenario scenario;
    scenario.topoMeta = TopoMeta{{{{0, 1}}}};
    scenario.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    scenario.dataType = HCCL_DATA_TYPE_INT8;
    scenario.count = 16;
    scenario.expectedAlgName = "InsAllGatherMesh1D";
    VerifyScenario(scenario);
}

// 块标签双射性质：不同 (rank, blockIdx) 的标签对必不同——256 对齐块置换可检出
TEST(AicpuStInputPattern, BlockTagsDistinguishAlignedBlocks)
{
    using HcclSim::AicpuSt::AicpuStFixture;
    for (uint32_t ra = 0; ra < 4; ++ra) {
        for (uint32_t rb = 0; rb < 4; ++rb) {
            for (uint64_t ba = 0; ba < 300; ba += 37) {
                for (uint64_t bb = 0; bb < 300; bb += 41) {
                    if (ra == rb && ba == bb) {
                        continue;
                    }
                    const uint64_t tagA = (ba << 8) | 0xFEU;
                    const uint64_t tagB = (bb << 8) | 0xFEU;
                    const bool loDiffers =
                        AicpuStFixture::MakeInputByte(ra, tagA) != AicpuStFixture::MakeInputByte(rb, tagB);
                    const bool hiDiffers =
                        AicpuStFixture::MakeInputByte(ra, tagA + 1) != AicpuStFixture::MakeInputByte(rb, tagB + 1);
                    EXPECT_TRUE(loDiffers || hiDiffers)
                        << "tag collision: (" << ra << "," << ba << ") vs (" << rb << "," << bb << ")";
                }
            }
        }
    }
    // 数据字节保留非 256 倍数位移检测力（周期 256，标签位除外）
    EXPECT_NE(AicpuStFixture::MakeInputByte(0, 3), AicpuStFixture::MakeInputByte(0, 4));
    EXPECT_EQ(AicpuStFixture::MakeInputByte(0, 3), AicpuStFixture::MakeInputByte(0, 3 + 256));
    // 相邻 256 块的标签不同（块乱序可检出）
    EXPECT_NE(AicpuStFixture::MakeInputByte(0, 0xFEU), AicpuStFixture::MakeInputByte(0, 256 + 0xFEU));
}
