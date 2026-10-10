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
