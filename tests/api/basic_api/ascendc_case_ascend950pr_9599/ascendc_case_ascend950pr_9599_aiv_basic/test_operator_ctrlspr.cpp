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
#include "mockcpp/mockcpp.hpp"

using namespace std;
using namespace AscendC;

void ScalarCacheModeReadKernel()
{
    // test L2Cache read mode: CTRL[19:16]
    // CACHE_MODE_NORMAL -> 0b0000 (0)
    SetScalarCacheMode<CacheRwMode::READ, CacheMode::CACHE_MODE_NORMAL>();
    int64_t value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0b0000);

    // CACHE_MODE_LAST -> 0b0001 (1)
    SetScalarCacheMode<CacheRwMode::READ, CacheMode::CACHE_MODE_LAST>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0b0001);

    // CACHE_MODE_PERSISTENT -> 0b0010 (2)
    SetScalarCacheMode<CacheRwMode::READ, CacheMode::CACHE_MODE_PERSISTENT>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0b0010);

    // CACHE_MODE_DISABLE -> 0b0100 (4)
    SetScalarCacheMode<CacheRwMode::READ, CacheMode::CACHE_MODE_DISABLE>();
    value = GetCtrlSpr<16, 19>();
    EXPECT_EQ(value, 0b0100);
}

void ScalarCacheModeWriteKernel()
{
    // test L2Cache write mode: CTRL[23:20]
    // CACHE_MODE_NORMAL -> 0b0000 (0)
    SetScalarCacheMode<CacheRwMode::WRITE, CacheMode::CACHE_MODE_NORMAL>();
    int64_t value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0b0000);

    // CACHE_MODE_LAST -> 0b0001 (1)
    SetScalarCacheMode<CacheRwMode::WRITE, CacheMode::CACHE_MODE_LAST>();
    value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0b0001);

    // CACHE_MODE_PERSISTENT -> 0b0010 (2)
    SetScalarCacheMode<CacheRwMode::WRITE, CacheMode::CACHE_MODE_PERSISTENT>();
    value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0b0010);

    // CACHE_MODE_DISABLE -> 0b0100 (4)
    SetScalarCacheMode<CacheRwMode::WRITE, CacheMode::CACHE_MODE_DISABLE>();
    value = GetCtrlSpr<20, 23>();
    EXPECT_EQ(value, 0b0100);
}

void ScalarCacheModeCombinedKernel()
{
    // test read and write mode independence
    SetScalarCacheMode<CacheRwMode::READ, CacheMode::CACHE_MODE_LAST>();
    SetScalarCacheMode<CacheRwMode::WRITE, CacheMode::CACHE_MODE_PERSISTENT>();

    int64_t readValue = GetCtrlSpr<16, 19>();
    int64_t writeValue = GetCtrlSpr<20, 23>();
    EXPECT_EQ(readValue, 0b0001);
    EXPECT_EQ(writeValue, 0b0010);

    // change read mode, verify write mode unchanged
    SetScalarCacheMode<CacheRwMode::READ, CacheMode::CACHE_MODE_DISABLE>();
    readValue = GetCtrlSpr<16, 19>();
    writeValue = GetCtrlSpr<20, 23>();
    EXPECT_EQ(readValue, 0b0100);
    EXPECT_EQ(writeValue, 0b0010);

    // change write mode, verify read mode unchanged
    SetScalarCacheMode<CacheRwMode::WRITE, CacheMode::CACHE_MODE_NORMAL>();
    readValue = GetCtrlSpr<16, 19>();
    writeValue = GetCtrlSpr<20, 23>();
    EXPECT_EQ(readValue, 0b0100);
    EXPECT_EQ(writeValue, 0b0000);
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

INSTANTIATE_TEST_CASE_P(
    TEST_CTRL_SPR, CtrlSprTestsuite,
    ::testing::Values(
        CtrlSprTestParams{ScalarCacheModeReadKernel}, CtrlSprTestParams{ScalarCacheModeWriteKernel},
        CtrlSprTestParams{ScalarCacheModeCombinedKernel}));

TEST_P(CtrlSprTestsuite, CtrlSprTestCase)
{
    auto param = GetParam();
    param.calFunc();
}