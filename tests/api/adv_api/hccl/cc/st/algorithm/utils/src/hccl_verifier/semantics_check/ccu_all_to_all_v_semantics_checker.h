/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_ALL_TO_ALL_V_SEMANTICS_CHECKER_H
#define HCCL_CCU_ALL_TO_ALL_V_SEMANTICS_CHECKER_H

#include "ccu_all_to_all_v_mesh1d_adapter.h"
#include "ccu_checker.h"

namespace HcclSim {
namespace CcuSt {

struct AllToAllVRuntimeExpectation {
    std::vector<std::vector<uint8_t>> outputs;
};

class CcuAllToAllVSemanticsChecker {
public:
    CheckResult CheckRegistration(const Program &program, const AllToAllVKernelConfig &config) const;
    CheckResult CheckTaskArgs(const Program &program, const std::vector<uint64_t> &taskArgs,
        const AllToAllVLaunchConfig &config) const;
    CheckResult CaptureRuntimeExpectation(const std::vector<RankMemory> &memories,
        const std::vector<AllToAllVLaunchConfig> &configs, AllToAllVRuntimeExpectation &expectation) const;
    CheckResult CheckRuntime(const AllToAllVRuntimeExpectation &expectation,
        const std::vector<RankMemory> &memories, const RuntimeWorld &world) const;
};

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_ALL_TO_ALL_V_SEMANTICS_CHECKER_H
