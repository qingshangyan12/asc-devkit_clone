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
#include <gtest/gtest-spi.h> // EXPECT_NONFATAL_FAILURE（golden 空守卫用例）

#include <vector>

#include "aicpu_fixture.h"
#include "alg_param.h"

using namespace HcclSim::AicpuSt;

namespace {
// 链路级冒烟：验证 fixture 全链（selector→资源→AicpuStKernelRunner→Orchestrate→任务图非空）
class AicpuStSmokeFixture : public AicpuStFixture {
protected:
    void PrepareOpParam(
        OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem) override
    {
        (void)rankSize;
        opParam.DataDes.count = scenario.count;
        opParam.DataDes.dataType = scenario.dataType; // tiling 已设，此处冗余兜底
        opParam.inputPtr = mem.inputPtr;
        opParam.outputPtr = mem.outputPtr;
        opParam.inputSize = mem.inputBytes;
        opParam.outputSize = mem.outputBytes;
    }

    std::vector<uint8_t> BuildExpectedOutput(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName) override
    {
        // 冒烟阶段不比对数据（Task 6 由 adapter 提供真实 golden）
        (void)rankSize;
        (void)scenario;
        (void)rank;
        (void)actualAlgName;
        return {};
    }

    // 刻意跳过比对：显式豁免 golden 空守卫（非零场景空 golden 默认会 FAIL）
    bool ExpectOutputCheck() override { return false; }
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

// golden 空守卫：非零场景的 BuildExpectedOutput 意外返回空（adapter bug）必须 FAIL（防假绿）
namespace {
class AicpuStEmptyGoldenFixture : public AicpuStFixture {
protected:
    void PrepareOpParam(
        OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem) override
    {
        (void)rankSize;
        opParam.DataDes.count = scenario.count;
        opParam.DataDes.dataType = scenario.dataType; // tiling 已设，此处冗余兜底
        opParam.inputPtr = mem.inputPtr;
        opParam.outputPtr = mem.outputPtr;
        opParam.inputSize = mem.inputBytes;
        opParam.outputSize = mem.outputBytes;
    }

    std::vector<uint8_t> BuildExpectedOutput(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName) override
    {
        (void)rankSize;
        (void)scenario;
        (void)rank;
        (void)actualAlgName;
        return {}; // 模拟 adapter bug：非零场景返回空 golden
    }
};
} // namespace

TEST_F(AicpuStEmptyGoldenFixture, UnexpectedEmptyGoldenFailsForNonZeroScenario)
{
    AicpuStScenario scenario;
    scenario.topoMeta = TopoMeta{{{{0, 1}}}};
    scenario.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    scenario.dataType = HCCL_DATA_TYPE_INT8;
    scenario.count = 16;
    scenario.expectedAlgName = "InsAllGatherMesh1D";
    EXPECT_NONFATAL_FAILURE(VerifyScenario(scenario), "golden unexpectedly empty");
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

// ===== 能力守卫（直算路径显式拒绝）=====
// 判定条件与产品 LocalReduce wrapper 的直算转发条件对齐：reduceLike（原语经
// IsReduceLikeOp 虚函数声明，此处直接传参）且 64bit 类型（任何 op）或 PROD（任何类型）
// 的场景被拒；非归约原语（传 false，如 AllGather）不受限。
// 详见 aicpu_fixture.cc CheckScenarioSupported 实现处的边界注释与设计文档 §18.2。

// 64bit 归约（产品支持但 sim 直解引用 SIGSEGV）→ 拒绝，reason 说明需真机验证
TEST(AicpuStScenarioGuard, Rejects64BitReduce)
{
    using HcclSim::AicpuSt::AicpuStFixture;
    AicpuStScenario scenario;
    scenario.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    scenario.dataType = HCCL_DATA_TYPE_INT64;
    scenario.reduceType = HCCL_REDUCE_SUM;
    std::string reason;
    EXPECT_FALSE(AicpuStFixture::CheckScenarioSupported(scenario, true, reason));
    EXPECT_NE(reason.find("64bit"), std::string::npos);
    EXPECT_NE(reason.find("real-device"), std::string::npos);
}

// PROD + 常规类型（产品 selector/执行能力错配，真机同样报错）→ 拒绝，reason 说明产品错配
TEST(AicpuStScenarioGuard, RejectsProdWithRegularType)
{
    using HcclSim::AicpuSt::AicpuStFixture;
    AicpuStScenario scenario;
    scenario.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    scenario.dataType = HCCL_DATA_TYPE_INT8;
    scenario.reduceType = HCCL_REDUCE_PROD;
    std::string reason;
    EXPECT_FALSE(AicpuStFixture::CheckScenarioSupported(scenario, true, reason));
    EXPECT_NE(reason.find("PROD"), std::string::npos);
    EXPECT_NE(reason.find("mismatch"), std::string::npos);
}

// 可测域不受误伤：归约系常规组合放行；非归约原语（isReduceLikeOp=false，如 AllGather）
// 即使 64bit 也放行（纯字节搬运走任务路径，不受直算边界影响）。
// 归约系 64bit 的端到端拦截（虚函数声明→守卫→ADD_FAILURE）由原语层用例验证
// （reduce_scatter_testcase.cpp ScenarioGuard_*；framework 的 Smoke 不声明归约语义）
TEST(AicpuStScenarioGuard, AllowsRegularReduceAndNonReduceOps)
{
    using HcclSim::AicpuSt::AicpuStFixture;
    AicpuStScenario regular;
    regular.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    regular.dataType = HCCL_DATA_TYPE_FP16;
    regular.reduceType = HCCL_REDUCE_SUM;
    std::string reason = "untouched";
    EXPECT_TRUE(AicpuStFixture::CheckScenarioSupported(regular, true, reason));
    EXPECT_EQ(reason, "untouched"); // 放行时不写 reason（调用方无需读）

    AicpuStScenario gather64;
    gather64.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    gather64.dataType = HCCL_DATA_TYPE_INT64;
    EXPECT_TRUE(AicpuStFixture::CheckScenarioSupported(gather64, false, reason));
}
