/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include "kernel_operator.h"
#include "include/adv_api/matmul/tiling.h"
#include "impl/adv_api/detail/matmul/utils/matmul_param.h"
#include "impl/adv_api/detail/matmul/resource/l1_manager/l1_manager.h"
#include "impl/adv_api/detail/matmul/stage/copy_cube_out/quant/quant_processor_utils.h"
#include "impl/adv_api/detail/matmul/dfx/dfx_proxy.h"
#include "impl/adv_api/detail/matmul/dfx/handlers/dfx_chain_handler.h"

namespace AscendC {
namespace {
__aicore__ constexpr MatmulConfig GetL1OptimizedConfig()
{
    auto cfg = CFG_NORM;
    cfg.enableL1BankConflictOptimise = true;
    return cfg;
}

struct FakeImpl {};

struct DfxCounterA {
    static uint32_t preCount;
    static uint32_t postRetCount;
    static uint32_t postVoidCount;

    template <typename... Args>
    __aicore__ inline static void PreCall(const Impl::Detail::DfxFuncInfo&, Args&&...)
    {
        ++preCount;
    }

    template <typename RT>
    __aicore__ inline static void PostCall(const Impl::Detail::DfxFuncInfo&, const RT&)
    {
        ++postRetCount;
    }

    __aicore__ inline static void PostCall(const Impl::Detail::DfxFuncInfo&) { ++postVoidCount; }
};

struct DfxCounterB {
    static uint32_t preCount;
    static uint32_t postRetCount;
    static uint32_t postVoidCount;

    template <typename... Args>
    __aicore__ inline static void PreCall(const Impl::Detail::DfxFuncInfo&, Args&&...)
    {
        ++preCount;
    }

    template <typename RT>
    __aicore__ inline static void PostCall(const Impl::Detail::DfxFuncInfo&, const RT&)
    {
        ++postRetCount;
    }

    __aicore__ inline static void PostCall(const Impl::Detail::DfxFuncInfo&) { ++postVoidCount; }
};

uint32_t DfxCounterA::preCount = 0;
uint32_t DfxCounterA::postRetCount = 0;
uint32_t DfxCounterA::postVoidCount = 0;
uint32_t DfxCounterB::preCount = 0;
uint32_t DfxCounterB::postRetCount = 0;
uint32_t DfxCounterB::postVoidCount = 0;

struct DfxProxyModule {
    __aicore__ inline int32_t Add(int32_t lhs, int32_t rhs) { return lhs + rhs; }

    __aicore__ inline void Store(int32_t value) { storedValue = value; }

    int32_t storedValue = 0;
};
} // namespace

TEST(TestMatmulInternalCoverage, L1ManagerTracksUpperAndLowerHalf)
{
    static constexpr MatmulConfig l1Config = GetL1OptimizedConfig();
    Impl::Detail::L1Manager<FakeImpl, l1Config> manager;

    EXPECT_EQ(manager.GetCurrentUpperHalfAddr(), 0U);
    EXPECT_EQ(manager.GetCurrentLowerHalfAddr(), Impl::Detail::L1SIZE / Impl::Detail::L1_SPLIT_NUM);

    manager.SetCurrentAddress(64U, 128U);

    EXPECT_EQ(manager.GetCurrentUpperHalfAddr(), 64U);
    EXPECT_EQ(manager.GetCurrentLowerHalfAddr(), Impl::Detail::L1SIZE / Impl::Detail::L1_SPLIT_NUM + 128U);
}

TEST(TestMatmulInternalCoverage, QuantScenarioCoversC310TypeBranches)
{
    EXPECT_FALSE((Impl::Detail::IsQuantSenario<half, half>()));
    EXPECT_TRUE((Impl::Detail::IsQuantSenario<half, fp8_e5m2_t>()));
    EXPECT_TRUE((Impl::Detail::IsQuantSenario<bfloat16_t, int8_t>()));
    EXPECT_TRUE((Impl::Detail::IsQuantSenario<uint8_t, half>()));
}

TEST(TestMatmulInternalCoverage, DfxChainHandlerDispatchesAllHooks)
{
    DfxCounterA::preCount = 0;
    DfxCounterA::postRetCount = 0;
    DfxCounterA::postVoidCount = 0;
    DfxCounterB::preCount = 0;
    DfxCounterB::postRetCount = 0;
    DfxCounterB::postVoidCount = 0;

    Impl::Detail::DfxFuncInfo info{"module", "func", 1U};
    using Handler = Impl::Detail::DfxChainHandler<DfxCounterA, DfxCounterB>;

    Handler::PreCall(info, 1, 2);
    Handler::PostCall(info, 3);
    Handler::PostCall(info);

    EXPECT_EQ(DfxCounterA::preCount, 1U);
    EXPECT_EQ(DfxCounterA::postRetCount, 1U);
    EXPECT_EQ(DfxCounterA::postVoidCount, 1U);
    EXPECT_EQ(DfxCounterB::preCount, 1U);
    EXPECT_EQ(DfxCounterB::postRetCount, 1U);
    EXPECT_EQ(DfxCounterB::postVoidCount, 1U);
}

TEST(TestMatmulInternalCoverage, DfxProxyCallsReturnAndVoidFunctions)
{
    Impl::Detail::DfxProxy<FakeImpl, DfxProxyModule> proxy;

    EXPECT_EQ(proxy->Add(2, 5), 7);
    proxy->Store(9);

    auto* rawModule = static_cast<DfxProxyModule*>(proxy);
    EXPECT_EQ(rawModule->storedValue, 9);
}
} // namespace AscendC
