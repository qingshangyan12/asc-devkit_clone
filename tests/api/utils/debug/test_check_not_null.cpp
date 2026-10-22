/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdlib>
#include <type_traits>

#include <gtest/gtest.h>

#include "utils/debug/check_not_null.h"

namespace {

int g_evaluationCount = 0;

int* GetPointer(int* ptr)
{
    ++g_evaluationCount;
    return ptr;
}

TEST(CheckNotNullTest, AcceptsNonNullObjectPointers)
{
    int value = 0;
    const int constValue = 0;

    CHECK_NOT_NULL(&value);
    CHECK_NOT_NULL(&constValue);
}

TEST(CheckNotNullTest, EvaluatesPointerExpressionOnce)
{
    int value = 0;
    g_evaluationCount = 0;

    CHECK_NOT_NULL(GetPointer(&value));

    EXPECT_EQ(g_evaluationCount, 1);
}

TEST(CheckNotNullTest, ExitsWhenPointerIsNull)
{
    int* nullPtr = nullptr;

    EXPECT_EXIT(
        CHECK_NOT_NULL(nullPtr), ::testing::ExitedWithCode(EXIT_FAILURE),
        "CHECK_NOT_NULL failed: pointer 'nullPtr' is null, file=.*test_check_not_null.cpp, line=[0-9]+");
}

static_assert(std::is_convertible_v<int*, const void*>);
static_assert(std::is_convertible_v<const int*, const void*>);

} // namespace
