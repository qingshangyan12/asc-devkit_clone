/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file be in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_ST_FIXTURE_H
#define CCU_ST_FIXTURE_H

#include <gtest/gtest.h>
#include "ccu_kernel_proxy.h"
#include "ccu_st_scenario.h"
#include "ccu_checker.h"
#include "ccu_dag_checker.h"
#include "ccu_sim_world.h"

namespace HcclSim {
namespace CcuSt {

class CcuStFixture : public ::testing::Test {
protected:
    void SetUp() override {
        RegisterManager::Global().Reset();
    }

    void TearDown() override {
        RegisterManager::Global().Reset();
    }

    // Common utility: Program structure validation
    CheckResult CheckProgramStructure(const Program &program) {
        return CcuProgramChecker().Check(program);
    }

    // Common utility: taskArgs ABI validation
    CheckResult CheckAbi(const Program &program, const std::vector<uint64_t> &taskArgs) {
        return CcuTaskArgChecker().Check(program, taskArgs);
    }

    // Common utility: RuntimeWorld execution + DAG conflict detection
    CheckResult ExecuteAndCheckDag(const std::vector<RankLaunch> &launches,
                                   RuntimeWorld &world) {
        Result launchResult = world.Launch(launches);
        if (launchResult != Result::SUCCESS) {
            return CheckResult{launchResult, world.LastError()};
        }
        return CcuDagChecker().Check(world.ExecutionLogData());
    }
};

} // namespace CcuSt
} // namespace HcclSim

#endif // CCU_ST_FIXTURE_H
