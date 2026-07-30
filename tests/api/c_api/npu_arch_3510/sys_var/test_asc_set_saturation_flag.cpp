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

static_assert(std::is_same<std::underlying_type<asc_saturation_mode>::type, uint8_t>::value);
static_assert(static_cast<uint8_t>(asc_saturation_mode::FLOAT) == 0);
static_assert(static_cast<uint8_t>(asc_saturation_mode::FLOAT8) == 1);
static_assert(static_cast<uint8_t>(asc_saturation_mode::INT) == 2);
static_assert(static_cast<uint8_t>(asc_saturation_mode::CAST) == 3);

namespace {

struct SaturationFlagCase {
    asc_saturation_mode mode;
    int64_t bit;
    bool enable;
    bool setBit;
};

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

class TestSetSaturationFlag : public testing::TestWithParam<SaturationFlagCase> {};

TEST_P(TestSetSaturationFlag, SetsExpectedCtrlBit)
{
    const SaturationFlagCase testCase = GetParam();
    g_expectedBit = testCase.bit;
    g_ctrlValue = testCase.setBit ? 0 : (1ULL << testCase.bit);
    g_expectedCtrlValue = testCase.setBit ? (1ULL << testCase.bit) : 0;

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));
    if (testCase.setBit) {
        MOCKER_CPP(sbitset1, uint64_t(uint64_t, int64_t)).times(1).will(invoke(set_ctrl_bit_stub));
    } else {
        MOCKER_CPP(sbitset0, uint64_t(uint64_t, int64_t)).times(1).will(invoke(clear_ctrl_bit_stub));
    }
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(1).will(invoke(set_ctrl_stub));

    asc_set_saturation_flag(testCase.mode, testCase.enable);
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_CASE_P(
    SaturationModes, TestSetSaturationFlag,
    testing::Values(
        SaturationFlagCase{asc_saturation_mode::FLOAT, 48, true, false},
        SaturationFlagCase{asc_saturation_mode::FLOAT, 48, false, true},
        SaturationFlagCase{asc_saturation_mode::FLOAT8, 50, true, false},
        SaturationFlagCase{asc_saturation_mode::FLOAT8, 50, false, true},
        SaturationFlagCase{asc_saturation_mode::INT, 53, true, true},
        SaturationFlagCase{asc_saturation_mode::INT, 53, false, false},
        SaturationFlagCase{asc_saturation_mode::CAST, 59, true, false},
        SaturationFlagCase{asc_saturation_mode::CAST, 59, false, true}));

} // namespace
