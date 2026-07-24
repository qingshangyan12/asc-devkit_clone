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

struct SetLoadCacheModeCase {
    asc_load_l2_cache_mode mode;
    uint64_t cacheBits;
};

struct SetStoreCacheModeCase {
    asc_store_l2_cache_mode mode;
    uint64_t cacheBits;
};

uint64_t g_ctrlValue;
uint64_t g_expectedCtrlValue;

int64_t get_ctrl_stub() { return static_cast<int64_t>(g_ctrlValue); }

void set_ctrl_stub(uint64_t value) { EXPECT_EQ(g_expectedCtrlValue, value); }

class TestSetScalarLoadCacheMode : public testing::TestWithParam<SetLoadCacheModeCase> {};

TEST_P(TestSetScalarLoadCacheMode, WritesExpectedCtrlBits)
{
    const SetLoadCacheModeCase testCase = GetParam();
    constexpr uint8_t startBit = 16;
    uint64_t mask = ~(uint64_t(0xF) << startBit);
    g_ctrlValue = 0xFFFFFFFFFFFFFFFF;
    g_expectedCtrlValue = (g_ctrlValue & mask) | (testCase.cacheBits << startBit);

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(1).will(invoke(set_ctrl_stub));

    asc_set_scalar_cache_mode(testCase.mode);
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_CASE_P(
    SetLoadModes, TestSetScalarLoadCacheMode,
    testing::Values(
        SetLoadCacheModeCase{asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 0},
        SetLoadCacheModeCase{asc_load_l2_cache_mode::NORMAL_LAST_VICTIM, 1},
        SetLoadCacheModeCase{asc_load_l2_cache_mode::NORMAL_PERSISTENT, 2},
        SetLoadCacheModeCase{asc_load_l2_cache_mode::NOTALLOC_KEEP, 4}));

class TestSetScalarLoadCacheModeNoOp : public testing::Test {};

TEST_F(TestSetScalarLoadCacheModeNoOp, DoesNotModifyCtrlForNotAllocClean)
{
    MOCKER_CPP(get_ctrl, int64_t()).times(0);
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(0);
    asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NOTALLOC_CLEAN);
    GlobalMockObject::verify();
}

TEST_F(TestSetScalarLoadCacheModeNoOp, DoesNotModifyCtrlForNotAllocDrop)
{
    MOCKER_CPP(get_ctrl, int64_t()).times(0);
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(0);
    asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NOTALLOC_DROP);
    GlobalMockObject::verify();
}

class TestSetScalarStoreCacheMode : public testing::TestWithParam<SetStoreCacheModeCase> {};

TEST_P(TestSetScalarStoreCacheMode, WritesExpectedCtrlBits)
{
    const SetStoreCacheModeCase testCase = GetParam();
    constexpr uint8_t startBit = 20;
    uint64_t mask = ~(uint64_t(0xF) << startBit);
    g_ctrlValue = 0xFFFFFFFFFFFFFFFF;
    g_expectedCtrlValue = (g_ctrlValue & mask) | (testCase.cacheBits << startBit);

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(1).will(invoke(set_ctrl_stub));

    asc_set_scalar_cache_mode(testCase.mode);
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_CASE_P(
    SetStoreModes, TestSetScalarStoreCacheMode,
    testing::Values(
        SetStoreCacheModeCase{asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, 0},
        SetStoreCacheModeCase{asc_store_l2_cache_mode::NORMAL_LAST_VICTIM, 1},
        SetStoreCacheModeCase{asc_store_l2_cache_mode::NORMAL_PERSISTENT, 2},
        SetStoreCacheModeCase{asc_store_l2_cache_mode::NOTALLOC_CLEAN, 4}));

} // namespace
