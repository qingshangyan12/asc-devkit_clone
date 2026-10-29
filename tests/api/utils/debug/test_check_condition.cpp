/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>

#include <gtest/gtest.h>

#include "utils/debug/check_condition.h"

namespace {

int g_evaluationCount = 0;

int* GetPointer(int* ptr)
{
    ++g_evaluationCount;
    return ptr;
}

TEST(CheckConditionTest, AcceptsTrueCondition)
{
    int value = 0;
    const int constValue = 0;

    ASC_HOST_CHECK_CONDITION(&value != nullptr, "value is null");
    ASC_HOST_CHECK_CONDITION(&constValue != nullptr, "constValue is null");
}

TEST(CheckConditionTest, EvaluatesConditionOnce)
{
    int value = 0;
    g_evaluationCount = 0;

    ASC_HOST_CHECK_CONDITION(GetPointer(&value) != nullptr, "value is null");

    EXPECT_EQ(g_evaluationCount, 1);
}

TEST(CheckConditionTest, AssertsWhenConditionIsFalse)
{
    int* nullPtr = nullptr;

    EXPECT_DEATH(
        ASC_HOST_CHECK_CONDITION(nullPtr != nullptr, "nullPtr is nullptr."),
        "\\[ERROR\\] ASC_HOST_CHECK_CONDITION failed: nullPtr is nullptr\\., file=.*test_check_condition.cpp, "
        "line=[0-9]+");
}

TEST(CheckConditionTest, AssertsWithDefaultMessageWhenMessageIsNull)
{
    EXPECT_DEATH(
        ASC_HOST_CHECK_CONDITION(false, nullptr),
        "\\[ERROR\\] ASC_HOST_CHECK_CONDITION failed: No violation message provided\\., "
        "file=.*test_check_condition.cpp, "
        "line=[0-9]+");
}

static_assert(std::is_convertible_v<int*, const void*>);
static_assert(std::is_convertible_v<const int*, const void*>);

} // namespace
