/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all_v.h"

#include <vector>

using namespace HcclSim::CcuSt;

namespace {

CcuStScenario MakeScenario(uint32_t rankSize, const std::vector<std::vector<uint64_t>> &sizes)
{
    CcuStScenario scenario;
    scenario.rankSize = rankSize;
    scenario.sizes = sizes;
    return scenario;
}

} // namespace

TEST_F(CcuStAllToAllV, HostSelectorPicksMesh1DAndRuntimeDeliversTwoRankAsymmetric)
{
    VerifyScenario(MakeScenario(2, {{19, 37}, {11, 23}}));
}

TEST_F(CcuStAllToAllV, FourRankSupportsZeroLengthPeers)
{
    VerifyScenario(MakeScenario(4, {{5, 0, 17, 9}, {3, 7, 0, 11}, {0, 13, 19, 2}, {23, 0, 6, 15}}));
}

TEST_F(CcuStAllToAllV, RuntimeCheckerRejectsOutputCorruption)
{
    CcuStScenario scenario = MakeScenario(2, {{8, 12}, {16, 20}});
    scenario.corruptOutput = true;
    VerifyScenario(scenario);
}

TEST_F(CcuStAllToAllV, CheckerRejectsTaskArgumentRebinding)
{
    CcuStScenario scenario = MakeScenario(2, {{7, 9}, {11, 13}});
    scenario.swapTaskArgs = true;
    VerifyScenario(scenario);
}

TEST_F(CcuStAllToAllV, LaunchRejectsMissingDynamicArgument)
{
    CcuStScenario scenario = MakeScenario(2, {{7, 9}, {11, 13}});
    scenario.missingTaskArg = true;
    VerifyScenario(scenario);
}

TEST_F(CcuStAllToAllV, LaunchRejectsOutOfRangeParameterTable)
{
    CcuStScenario scenario = MakeScenario(2, {{7, 9}, {11, 13}});
    scenario.outOfRangeParam = true;
    VerifyScenario(scenario);
}
