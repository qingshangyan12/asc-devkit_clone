/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all.h"

#include <vector>

using namespace HcclSim::CcuSt;

namespace {

CcuStScenario MakeScenario(uint32_t rankSize, uint64_t sliceSize)
{
    CcuStScenario scenario;
    scenario.rankSize = rankSize;
    scenario.opType = HcclCMDType::HCCL_CMD_ALLTOALL;
    scenario.sizes.assign(rankSize, std::vector<uint64_t>(rankSize, sliceSize));
    return scenario;
}

} // namespace

TEST_F(CcuStAllToAll, HostSelectorPicksMesh1DAndRuntimeDeliversTwoRankEqualLength)
{
    VerifyScenario(MakeScenario(2, 100));
}

TEST_F(CcuStAllToAll, FourRankEqualLengthTransfersCorrectly)
{
    VerifyScenario(MakeScenario(4, 64));
}

TEST_F(CcuStAllToAll, RuntimeCheckerRejectsOutputCorruption)
{
    CcuStScenario scenario = MakeScenario(2, 80);
    scenario.corruptOutput = true;
    VerifyScenario(scenario);
}

TEST_F(CcuStAllToAll, CheckerRejectsTaskArgumentRebinding)
{
    CcuStScenario scenario = MakeScenario(2, 80);
    scenario.swapTaskArgs = true;
    VerifyScenario(scenario);
}

TEST_F(CcuStAllToAll, LaunchRejectsMissingDynamicArgument)
{
    CcuStScenario scenario = MakeScenario(2, 80);
    scenario.missingTaskArg = true;
    VerifyScenario(scenario);
}
