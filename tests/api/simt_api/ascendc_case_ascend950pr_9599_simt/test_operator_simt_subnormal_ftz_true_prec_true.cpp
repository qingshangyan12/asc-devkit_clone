/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __ASC_PREC_SQRT__
#define __ASC_PREC_SQRT__
#endif
#include <gtest/gtest.h>
#include <type_traits>
#include <cmath>
#include <limits>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_api/math_functions.h"

class FloatSubnormalFtzTruePrecTrueTestsuite : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FloatSubnormalFtzTruePrecTrueTestsuite, ExpFloatFtzTruePrecTrue)
{
    float a = 1.17549421e-38f;
    float x = 1.0f;
    float y = -80.0f;
    float subnormalOutputInput = -90.0f;
    float z = 0.0f;
    EXPECT_FLOAT_EQ(std::exp(x), expf(x));
    EXPECT_FLOAT_EQ(std::exp(a), expf(a));
    EXPECT_FLOAT_EQ(std::exp(y), expf(y));
    EXPECT_FLOAT_EQ(std::exp(subnormalOutputInput), expf(subnormalOutputInput));
    EXPECT_FLOAT_EQ(std::exp(z), expf(z));
}

TEST_F(FloatSubnormalFtzTruePrecTrueTestsuite, InternalExpFloatFtzTruePrecTrue)
{
    volatile float normalInput = -80.0f;
    volatile float subnormalOutputInput = -90.0f;
    EXPECT_FLOAT_EQ(std::exp(static_cast<float>(normalInput)), __internal_expf(normalInput));
    EXPECT_FLOAT_EQ(std::exp(static_cast<float>(subnormalOutputInput)), __internal_expf(subnormalOutputInput));
}

TEST_F(FloatSubnormalFtzTruePrecTrueTestsuite, SqrtFloatFtzTruePrecTrue)
{
    float x = 1.0f;
    float y = 1.17549421e-38f;
    float z = 0.0f;
    float scaledNormal = 0.25f;
    float normal = 4.0f;
    EXPECT_FLOAT_EQ(std::sqrt(x), sqrtf(x));
    EXPECT_NEAR(std::sqrt(y), sqrtf(y), 1e-24f);
    EXPECT_FLOAT_EQ(std::sqrt(z), sqrtf(z));
    EXPECT_NEAR(std::sqrt(scaledNormal), sqrtf(scaledNormal), 1e-6f);
    EXPECT_NEAR(std::sqrt(normal), sqrtf(normal), 1e-6f);
    EXPECT_TRUE(std::isinf(sqrtf(ASCRT_INF_F)));
}

TEST_F(FloatSubnormalFtzTruePrecTrueTestsuite, InternalSqrtFloatFtzTruePrecTrue)
{
    volatile float zero = 0.0f;
    volatile float inf = ASCRT_INF_F;
    volatile float subnormal = std::numeric_limits<float>::denorm_min();
    volatile float scaledNormal = 0.25f;
    volatile float normal = 4.0f;
    EXPECT_FLOAT_EQ(0.0f, __internal_sqrtf(zero));
    EXPECT_TRUE(std::isinf(__internal_sqrtf(inf)));
    EXPECT_FLOAT_EQ(0.0f, __internal_sqrtf(subnormal));
    EXPECT_NEAR(std::sqrt(static_cast<float>(scaledNormal)), __internal_sqrtf(scaledNormal), 1e-6f);
    EXPECT_NEAR(std::sqrt(static_cast<float>(normal)), __internal_sqrtf(normal), 1e-6f);
}

TEST_F(FloatSubnormalFtzTruePrecTrueTestsuite, InternalPreciseSqrtFloatCommon)
{
    volatile float zero = 0.0f;
    volatile float inf = ASCRT_INF_F;
    volatile float scaled = 0.25f;
    volatile float normal = 4.0f;
    EXPECT_FLOAT_EQ(0.0f, __internal_sqrtf_precise_impl(zero));
    EXPECT_TRUE(std::isinf(__internal_sqrtf_precise_impl(inf)));
    EXPECT_NEAR(std::sqrt(static_cast<float>(scaled)), __internal_sqrtf_precise_impl(scaled), 1e-6f);
    EXPECT_NEAR(std::sqrt(static_cast<float>(normal)), __internal_sqrtf_precise_impl(normal), 1e-6f);
}
