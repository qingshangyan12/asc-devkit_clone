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
#include <type_traits>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

static_assert(std::is_same<std::underlying_type<asc_override_strategy>::type, uint8_t>::value);
static_assert(static_cast<uint8_t>(asc_override_strategy::USE_API) == 0);
static_assert(static_cast<uint8_t>(asc_override_strategy::USE_GLOBAL) == 1);

namespace {

constexpr int64_t SATURATION_STRATEGY_BIT = 60;
uint64_t g_ctrlValue;
uint64_t g_expectedCtrlValue;
int64_t g_expectedBit;

int64_t get_ctrl_stub() { return static_cast<int64_t>(g_ctrlValue); }

uint64_t clear_ctrl_bit_stub(uint64_t value, int64_t bit)
{
    EXPECT_EQ(g_ctrlValue, value);
    EXPECT_EQ(g_expectedBit, bit);
    return value & ~(1ULL << bit);
}

uint64_t set_ctrl_bit_stub(uint64_t value, int64_t bit)
{
    EXPECT_EQ(g_ctrlValue, value);
    EXPECT_EQ(g_expectedBit, bit);
    return value | (1ULL << bit);
}

void set_ctrl_stub(uint64_t value) { EXPECT_EQ(g_expectedCtrlValue, value); }

class TestSetSaturationStrategy : public testing::Test {};

TEST_F(TestSetSaturationStrategy, SetUseApiClearsStrategyBit)
{
    g_expectedBit = SATURATION_STRATEGY_BIT;
    g_ctrlValue = 1ULL << SATURATION_STRATEGY_BIT;
    g_expectedCtrlValue = 0;

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));
    MOCKER_CPP(sbitset0, uint64_t(uint64_t, int64_t)).times(1).will(invoke(clear_ctrl_bit_stub));
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(1).will(invoke(set_ctrl_stub));

    asc_set_saturation_strategy(asc_override_strategy::USE_API);
    GlobalMockObject::verify();
}

TEST_F(TestSetSaturationStrategy, SetUseGlobalSetsStrategyBit)
{
    g_expectedBit = SATURATION_STRATEGY_BIT;
    g_ctrlValue = 0;
    g_expectedCtrlValue = 1ULL << SATURATION_STRATEGY_BIT;

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));
    MOCKER_CPP(sbitset1, uint64_t(uint64_t, int64_t)).times(1).will(invoke(set_ctrl_bit_stub));
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(1).will(invoke(set_ctrl_stub));

    asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
    GlobalMockObject::verify();
}

} // namespace
