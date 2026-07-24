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

struct GetSaturationFlagCase {
    asc_saturation_mode mode;
    int64_t bit;
    bool bitValue;
    bool expected;
};

uint64_t g_ctrlValue;

int64_t get_ctrl_stub() { return static_cast<int64_t>(g_ctrlValue); }

class TestGetSaturationFlag : public testing::TestWithParam<GetSaturationFlagCase> {};

TEST_P(TestGetSaturationFlag, ReadsExpectedCtrlBit)
{
    const GetSaturationFlagCase testCase = GetParam();
    g_ctrlValue = testCase.bitValue ? (1ULL << testCase.bit) : 0;

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));

    EXPECT_EQ(testCase.expected, asc_get_saturation_flag(testCase.mode));
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_CASE_P(
    SaturationModes, TestGetSaturationFlag,
    testing::Values(
        GetSaturationFlagCase{asc_saturation_mode::FLOAT, 48, false, true},
        GetSaturationFlagCase{asc_saturation_mode::FLOAT, 48, true, false},
        GetSaturationFlagCase{asc_saturation_mode::FLOAT8, 50, false, true},
        GetSaturationFlagCase{asc_saturation_mode::FLOAT8, 50, true, false},
        GetSaturationFlagCase{asc_saturation_mode::INT, 53, false, false},
        GetSaturationFlagCase{asc_saturation_mode::INT, 53, true, true},
        GetSaturationFlagCase{asc_saturation_mode::CAST, 59, false, true},
        GetSaturationFlagCase{asc_saturation_mode::CAST, 59, true, false}));

} // namespace
