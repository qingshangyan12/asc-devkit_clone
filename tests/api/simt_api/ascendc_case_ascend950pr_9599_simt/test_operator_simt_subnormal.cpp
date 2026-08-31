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
#include <cmath>
#include <limits>
#include <type_traits>
#include "simt_compiler_stub.h"
#include "simt_api/math_functions.h"

class FloatSubnormalTestsuite : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FloatSubnormalTestsuite, ExpFloat)
{
    float a = 1.17549421e-38f;
    float x = 1.0f;
    float y = -80.0f;
    float subnormalOutputInput = -90.0f;
    float z = 0.0f;
    EXPECT_FLOAT_EQ(std::exp(x), expf(x));
    EXPECT_FLOAT_EQ(std::exp(a), expf(a));
    EXPECT_FLOAT_EQ(std::exp(y), expf(y));
#ifdef __ASC_FTZ__
    EXPECT_FLOAT_EQ(std::exp(subnormalOutputInput), expf(subnormalOutputInput));
#else
    EXPECT_NEAR(std::exp(subnormalOutputInput), expf(subnormalOutputInput), 1e-43f);
#endif
    EXPECT_FLOAT_EQ(std::exp(z), expf(z));
}

TEST_F(FloatSubnormalTestsuite, InternalExpFloat)
{
    volatile float normalInput = -80.0f;
    volatile float subnormalOutputInput = -90.0f;
    EXPECT_FLOAT_EQ(std::exp(static_cast<float>(normalInput)), __internal_expf(normalInput));
#ifdef __ASC_FTZ__
    EXPECT_FLOAT_EQ(std::exp(static_cast<float>(subnormalOutputInput)), __internal_expf(subnormalOutputInput));
#else
    EXPECT_NEAR(std::exp(static_cast<float>(subnormalOutputInput)), __internal_expf(subnormalOutputInput), 1e-43f);
#endif
}

TEST_F(FloatSubnormalTestsuite, SqrtFloat)
{
    float x = 1.0f;
    float y = 1.17549421e-38f;
    float z = 0.0f;
    float normal = 4.0f;
    EXPECT_FLOAT_EQ(std::sqrt(x), sqrtf(x));
#ifdef __ASC_FTZ__
    EXPECT_FLOAT_EQ(std::sqrt(y), sqrtf(y));
#else
    EXPECT_NEAR(std::sqrt(y), sqrtf(y), 1e-24f);
#endif
    EXPECT_FLOAT_EQ(std::sqrt(z), sqrtf(z));
#ifdef __ASC_PREC_SQRT__
    float scaledNormal = 0.25f;
    EXPECT_NEAR(std::sqrt(scaledNormal), sqrtf(scaledNormal), 1e-6f);
    EXPECT_NEAR(std::sqrt(normal), sqrtf(normal), 1e-6f);
    EXPECT_TRUE(std::isinf(sqrtf(ASCRT_INF_F)));
#else
    EXPECT_FLOAT_EQ(std::sqrt(normal), sqrtf(normal));
#endif
}

TEST_F(FloatSubnormalTestsuite, InternalSqrtFloat)
{
    volatile float subnormal = std::numeric_limits<float>::denorm_min();
    volatile float normal = 4.0f;
#ifdef __ASC_PREC_SQRT__
    volatile float zero = 0.0f;
    volatile float inf = ASCRT_INF_F;
    volatile float scaledNormal = 0.25f;
    EXPECT_FLOAT_EQ(0.0f, __internal_sqrtf(zero));
    EXPECT_TRUE(std::isinf(__internal_sqrtf(inf)));
#ifdef __ASC_FTZ__
    EXPECT_FLOAT_EQ(std::sqrt(static_cast<float>(subnormal)), __internal_sqrtf(subnormal));
#else
    EXPECT_NEAR(std::sqrt(static_cast<float>(subnormal)), __internal_sqrtf(subnormal), 1e-24f);
#endif
    EXPECT_NEAR(std::sqrt(static_cast<float>(scaledNormal)), __internal_sqrtf(scaledNormal), 1e-6f);
    EXPECT_NEAR(std::sqrt(static_cast<float>(normal)), __internal_sqrtf(normal), 1e-6f);
#else
#ifdef __ASC_FTZ__
    EXPECT_FLOAT_EQ(std::sqrt(static_cast<float>(subnormal)), __internal_sqrtf(subnormal));
#else
    EXPECT_NEAR(std::sqrt(static_cast<float>(subnormal)), __internal_sqrtf(subnormal), 1e-24f);
#endif
    EXPECT_FLOAT_EQ(std::sqrt(static_cast<float>(normal)), __internal_sqrtf(normal));
#endif
}

TEST_F(FloatSubnormalTestsuite, InternalPreciseSqrtFloatCommon)
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

TEST_F(FloatSubnormalTestsuite, Log2Float)
{
    volatile float denormMin = std::numeric_limits<float>::denorm_min();
    volatile float minNormal = std::numeric_limits<float>::min();
    volatile float half = 0.5f;
    volatile float normal = 3.5f;
    volatile float zero = 0.0f;
    volatile float negative = -1.0f;
    volatile float inf = ASCRT_INF_F;
    volatile float nan = ASCRT_NAN_F;

    EXPECT_NEAR(std::log2(static_cast<float>(denormMin)), log2f(denormMin), 1e-4f);
    EXPECT_NEAR(std::log2(static_cast<float>(minNormal)), log2f(minNormal), 1e-4f);
    EXPECT_NEAR(std::log2(static_cast<float>(half)), log2f(half), 1e-6f);
    EXPECT_NEAR(std::log2(static_cast<float>(normal)), log2f(normal), 1e-5f);
    EXPECT_TRUE(std::isinf(log2f(zero)));
    EXPECT_LT(log2f(zero), 0.0f);
    EXPECT_TRUE(std::isnan(log2f(negative)));
    EXPECT_TRUE(std::isinf(log2f(inf)));
    EXPECT_GT(log2f(inf), 0.0f);
    EXPECT_TRUE(std::isnan(log2f(nan)));
}

TEST_F(FloatSubnormalTestsuite, InternalLog2PositiveFiniteFloat)
{
    volatile float denormMin = std::numeric_limits<float>::denorm_min();
    volatile float minNormal = std::numeric_limits<float>::min();
    volatile float normal = 1.25f;
    float log2Hi = 0.0f;
    float log2Lo = 0.0f;

    __internal_log2f_positive_finite(denormMin, log2Hi, log2Lo);
    EXPECT_NEAR(std::log2(static_cast<float>(denormMin)), log2Hi + log2Lo, 1e-4f);

    __internal_log2f_positive_finite(minNormal, log2Hi, log2Lo);
    EXPECT_NEAR(std::log2(static_cast<float>(minNormal)), log2Hi + log2Lo, 1e-4f);

    __internal_log2f_positive_finite(normal, log2Hi, log2Lo);
    EXPECT_NEAR(std::log2(static_cast<float>(normal)), log2Hi + log2Lo, 1e-5f);
}

TEST_F(FloatSubnormalTestsuite, Exp2Float)
{
    volatile float negative = -4.25f;
    volatile float positive = 3.5f;
    volatile float zero = 0.0f;
    volatile float overflowInput = 153.0f;
    volatile float underflowInput = -153.0f;
    volatile float positiveInf = ASCRT_INF_F;
    volatile float negativeInf = -ASCRT_INF_F;
    volatile float nan = ASCRT_NAN_F;

    EXPECT_NEAR(std::exp2(static_cast<float>(negative)), exp2f(negative), 1e-7f);
    EXPECT_NEAR(std::exp2(static_cast<float>(positive)), exp2f(positive), 1e-4f);
    EXPECT_FLOAT_EQ(1.0f, exp2f(zero));
    EXPECT_TRUE(std::isinf(exp2f(overflowInput)));
    EXPECT_FLOAT_EQ(0.0f, exp2f(underflowInput));
    EXPECT_TRUE(std::isinf(exp2f(positiveInf)));
    EXPECT_FLOAT_EQ(0.0f, exp2f(negativeInf));
    EXPECT_TRUE(std::isnan(exp2f(nan)));
}

TEST_F(FloatSubnormalTestsuite, InternalExp2ReconstructFloat)
{
    EXPECT_NEAR(std::exp2(2.25f), __internal_exp2f_reconstruct(2.0f, 0.25f), 1e-5f);
    EXPECT_NEAR(std::exp2(-3.5f), __internal_exp2f_reconstruct(-3.0f, -0.5f), 1e-7f);
    EXPECT_FLOAT_EQ(1.0f, __internal_exp2f_finite(0.0f));
    EXPECT_TRUE(std::isinf(__internal_exp2f_reconstruct(153.0f, 0.0f)));
    EXPECT_FLOAT_EQ(0.0f, __internal_exp2f_reconstruct(-153.0f, 0.0f));
}

TEST_F(FloatSubnormalTestsuite, PowFloat)
{
    volatile float denormMin = std::numeric_limits<float>::denorm_min();
    volatile float negativeZero = -0.0f;
    volatile float negativeBase = -2.0f;
    volatile float nonIntegerExponent = 2.5f;
    volatile float positiveInf = ASCRT_INF_F;
    volatile float negativeInf = -ASCRT_INF_F;
    volatile float nan = ASCRT_NAN_F;

    EXPECT_NEAR(std::pow(2.0f, -3.0f), powf(2.0f, -3.0f), 1e-6f);
    EXPECT_NEAR(std::pow(1.00097656f, 7.25f), powf(1.00097656f, 7.25f), 1e-6f);
    EXPECT_FLOAT_EQ(1.0f, powf(nan, 0.0f));
    EXPECT_FLOAT_EQ(1.0f, powf(1.0f, nan));
    EXPECT_TRUE(std::isnan(powf(nan, 2.0f)));
    EXPECT_FLOAT_EQ(-8.0f, powf(negativeBase, 3.0f));
    EXPECT_FLOAT_EQ(16.0f, powf(negativeBase, 4.0f));
    EXPECT_TRUE(std::isnan(powf(negativeBase, nonIntegerExponent)));
    EXPECT_TRUE(std::isinf(powf(negativeZero, -3.0f)));
    EXPECT_TRUE(std::signbit(powf(negativeZero, -3.0f)));
    EXPECT_FLOAT_EQ(0.0f, powf(negativeZero, 2.0f));
    EXPECT_FLOAT_EQ(1.0f, powf(-1.0f, positiveInf));
    EXPECT_FLOAT_EQ(0.0f, powf(0.5f, positiveInf));
    EXPECT_TRUE(std::isinf(powf(0.5f, negativeInf)));
    EXPECT_NEAR(std::pow(static_cast<float>(denormMin), 0.5f), powf(denormMin, 0.5f), 1e-22f);
}

TEST_F(FloatSubnormalTestsuite, CopySignFloat)
{
    float negativeZero = copysignf(0.0f, -1.0f);
    float positiveZero = copysignf(-0.0f, 1.0f);
    float negativeValue = copysignf(2.0f, -0.0f);
    float positiveValue = copysignf(-2.0f, 0.0f);

    EXPECT_FLOAT_EQ(0.0f, negativeZero);
    EXPECT_TRUE(std::signbit(negativeZero));
    EXPECT_FLOAT_EQ(0.0f, positiveZero);
    EXPECT_FALSE(std::signbit(positiveZero));
    EXPECT_FLOAT_EQ(-2.0f, negativeValue);
    EXPECT_TRUE(std::signbit(negativeValue));
    EXPECT_FLOAT_EQ(2.0f, positiveValue);
    EXPECT_FALSE(std::signbit(positiveValue));
    EXPECT_TRUE(std::signbit(__internal_with_sign_bit(1.0f, -0.0f)));
}
