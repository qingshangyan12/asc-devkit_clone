/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include "kernel_operator.h"
#include "mockcpp/mockcpp.hpp"

using namespace std;
using namespace AscendC;

void ScalarCacheModeLoadKernel()
{
    // test L2Cache read mode: CTRL[19:16]
    // FIRST_VICTIM = 0x0
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::FIRST_VICTIM>();
    int64_t value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0x0);

    // LAST_VICTIM = 0x1
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::LAST_VICTIM>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0x1);

    // PERSISTENT = 0x2
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::PERSISTENT>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0x2);

    // KEEP = 0x4
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::KEEP>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0x4);

    // CLEAN = 0x5
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::CLEAN>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0x5);

    // DROP = 0x6
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::DROP>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0x6);
}

void ScalarCacheModeStoreKernel()
{
    // test L2Cache write mode: CTRL[23:20]
    // FIRST_VICTIM = 0x0
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_STORE, CacheMode::FIRST_VICTIM>();
    int64_t value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0x0);

    // LAST_VICTIM = 0x1
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_STORE, CacheMode::LAST_VICTIM>();
    value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0x1);

    // PERSISTENT = 0x2
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_STORE, CacheMode::PERSISTENT>();
    value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0x2);

    // CLEAN (clean-invalid) = 0x4
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_STORE, CacheMode::CLEAN>();
    value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0x4);
}

void ScalarCacheModeCombinedKernel()
{
    // test read and write mode independence
    // set read mode to LAST_VICTIM (0x1)
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::LAST_VICTIM>();
    // set write mode to PERSISTENT (0x2)
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_STORE, CacheMode::PERSISTENT>();

    int64_t readValue = GetCtrlSpr<16, 19>();
    int64_t writeValue = GetCtrlSpr<20, 23>();
    EXPECT_EQ(readValue, 0x1);
    EXPECT_EQ(writeValue, 0x2);

    // change read mode, verify write mode unchanged
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_LOAD, CacheMode::DROP>();
    readValue = GetCtrlSpr<16, 19>();
    writeValue = GetCtrlSpr<20, 23>();
    EXPECT_EQ(readValue, 0x6);
    EXPECT_EQ(writeValue, 0x2);

    // change write mode, verify read mode unchanged
    SetScalarCacheMode<RWMode::L2_CACHE_CONTROL_FOR_STORE, CacheMode::FIRST_VICTIM>();
    readValue = GetCtrlSpr<16, 19>();
    writeValue = GetCtrlSpr<20, 23>();
    EXPECT_EQ(readValue, 0x6);
    EXPECT_EQ(writeValue, 0x0);
}

struct CtrlSprTestParams {
    void (*calFunc)();
};

class CtrlSprTestsuite : public testing::Test, public testing::WithParamInterface<CtrlSprTestParams> {
protected:
    void SetUp() { AscendC::SetGCoreType(2); }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
        GlobalMockObject::verify();
    }
};

INSTANTIATE_TEST_CASE_P(TEST_CTRL_SPR, CtrlSprTestsuite,
    ::testing::Values(
        CtrlSprTestParams{ScalarCacheModeLoadKernel},
        CtrlSprTestParams{ScalarCacheModeStoreKernel},
        CtrlSprTestParams{ScalarCacheModeCombinedKernel}
    ));

TEST_P(CtrlSprTestsuite, CtrlSprTestCase)
{
    auto param = GetParam();
    param.calFunc();
}