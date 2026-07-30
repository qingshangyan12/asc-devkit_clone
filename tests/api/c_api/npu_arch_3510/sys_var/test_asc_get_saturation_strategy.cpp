/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

namespace {

constexpr int64_t SATURATION_STRATEGY_BIT = 60;
uint64_t g_ctrlValue;

int64_t get_ctrl_stub() { return static_cast<int64_t>(g_ctrlValue); }

class TestGetSaturationStrategy : public testing::Test {};

TEST_F(TestGetSaturationStrategy, GetStrategyReturnsUseApiWhenBitIsClear)
{
    g_ctrlValue = 0;
    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));

    EXPECT_EQ(asc_override_strategy::USE_API, asc_get_saturation_strategy());
    GlobalMockObject::verify();
}

TEST_F(TestGetSaturationStrategy, GetStrategyReturnsUseGlobalWhenBitIsSet)
{
    g_ctrlValue = 1ULL << SATURATION_STRATEGY_BIT;
    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));

    EXPECT_EQ(asc_override_strategy::USE_GLOBAL, asc_get_saturation_strategy());
    GlobalMockObject::verify();
}

} // namespace
