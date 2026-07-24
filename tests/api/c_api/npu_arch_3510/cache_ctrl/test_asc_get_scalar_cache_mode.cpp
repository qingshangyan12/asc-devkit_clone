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

struct LoadCacheModeCase {
    uint8_t cacheBits;
    asc_load_l2_cache_mode expected;
};

struct StoreCacheModeCase {
    uint8_t cacheBits;
    asc_store_l2_cache_mode expected;
};

uint64_t g_ctrlValue;

int64_t get_ctrl_stub() { return static_cast<int64_t>(g_ctrlValue); }

class TestGetScalarLoadCacheMode : public testing::TestWithParam<LoadCacheModeCase> {};

TEST_P(TestGetScalarLoadCacheMode, ReturnsDecodedMode)
{
    const LoadCacheModeCase testCase = GetParam();
    constexpr uint8_t startBit = 16;
    g_ctrlValue = static_cast<uint64_t>(testCase.cacheBits) << startBit;

    asc_load_l2_cache_mode cacheMode = static_cast<asc_load_l2_cache_mode>(0xFF);

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));

    asc_get_scalar_cache_mode(cacheMode);

    EXPECT_EQ(testCase.expected, cacheMode);
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_CASE_P(
    LoadModes, TestGetScalarLoadCacheMode,
    testing::Values(
        LoadCacheModeCase{0, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM},
        LoadCacheModeCase{1, asc_load_l2_cache_mode::NORMAL_LAST_VICTIM},
        LoadCacheModeCase{2, asc_load_l2_cache_mode::NORMAL_PERSISTENT},
        LoadCacheModeCase{4, asc_load_l2_cache_mode::NOTALLOC_KEEP},
        LoadCacheModeCase{7, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM}));

class TestGetScalarStoreCacheMode : public testing::TestWithParam<StoreCacheModeCase> {};

TEST_P(TestGetScalarStoreCacheMode, ReturnsDecodedMode)
{
    const StoreCacheModeCase testCase = GetParam();
    constexpr uint8_t startBit = 20;
    g_ctrlValue = static_cast<uint64_t>(testCase.cacheBits) << startBit;

    asc_store_l2_cache_mode cacheMode = static_cast<asc_store_l2_cache_mode>(0xFF);

    MOCKER_CPP(get_ctrl, int64_t()).times(1).will(invoke(get_ctrl_stub));

    asc_get_scalar_cache_mode(cacheMode);

    EXPECT_EQ(testCase.expected, cacheMode);
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_CASE_P(
    StoreModes, TestGetScalarStoreCacheMode,
    testing::Values(
        StoreCacheModeCase{0, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM},
        StoreCacheModeCase{1, asc_store_l2_cache_mode::NORMAL_LAST_VICTIM},
        StoreCacheModeCase{2, asc_store_l2_cache_mode::NORMAL_PERSISTENT},
        StoreCacheModeCase{4, asc_store_l2_cache_mode::NOTALLOC_CLEAN},
        StoreCacheModeCase{7, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM}));

} // namespace
