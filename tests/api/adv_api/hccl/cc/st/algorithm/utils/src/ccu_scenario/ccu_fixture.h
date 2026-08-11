/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_ST_FIXTURE_H
#define CCU_ST_FIXTURE_H

#include <gtest/gtest.h>
#include "ccu_kernel_proxy.h"
#include "ccu_scenario.h"
#include "ccu_checker.h"
#include "ccu_dag_checker.h"
#include "ccu_sim_world.h"
#include "sim_common.h"
#include "hccl_alloc_ctx_res.h"

namespace HcclSim {
namespace CcuSt {

struct ScenarioData {
    std::vector<RankMemory> memories;
    std::vector<RankLaunch> launches;
};

class CcuStFixture : public ::testing::Test {
protected:
    void SetUp() override {
        RegisterManager::Global().Reset();
    }

    void TearDown() override {
        RegisterManager::Global().Reset();
    }

    // ===== 通用流程：子类调 VerifyScenario 即可 =====
    void VerifyScenario(const CcuStScenario &scenario);

    // ===== 子类必须实现的 3 个虚函数 =====

    // 构造测试数据（memories + launches），子类填充专属的 config 类型
    virtual ScenarioData BuildScenarioData(const CcuStScenario &scenario,
                                           const std::vector<KernelHandle> &handles) = 0;

    // 静态校验（CheckRegistration + CheckTaskArgs），返回 false 表示校验失败
    virtual CheckResult DoCheckRegistration(const Program &program, uint32_t rank,
                                            uint32_t rankSize) = 0;
    virtual CheckResult DoCheckTaskArgs(const Program &program, uint32_t rank,
                                        const ScenarioData &data) = 0;

    // 运行时校验（期望数据计算 + 逐字节对比）
    virtual CheckResult DoCaptureRuntimeExpectation(const ScenarioData &data) = 0;
    virtual CheckResult DoCheckRuntime(const ScenarioData &data, RuntimeWorld &world) = 0;

    // 反例：交换 taskArgs 的哪两个 index（子类可覆盖，默认交换 [5]和[6]）
    virtual std::pair<size_t, size_t> SwapTaskArgIndices() const { return {5, 6}; }

    // 通用工具函数
    static Mc2CcTilingInner BuildTiling(HcclCMDType opType, HcclDataType dataType,
                                         const std::string &algConfig);
    static TopoMeta BuildTopoMeta(uint32_t rankSize);
    static void CleanupHostEnv(HcclComm &comm);
    static KernelHandle RegisterKernelForRank(uint32_t rank, uint32_t rankSize,
                                               const CcuStScenario &scenario,
                                               const TopoMeta &topoMeta,
                                               const Mc2CcTilingInner &tiling);

private:
    // 保存 scenario 供 DoCheckXxx 使用
    CcuStScenario currentScenario_;
};

} // namespace CcuSt
} // namespace HcclSim

#endif // CCU_ST_FIXTURE_H
