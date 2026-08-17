/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifdef __ASC_FTZ__
#undef __ASC_FTZ__
#endif
#include <gtest/gtest.h>
#include <type_traits>
#include <cmath>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_api/math_functions.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

#define THREAD_DIM 128

struct FloatSubnormalParams {
    int32_t mode;
};

class FloatSubnormalFtzFalsePrecFalseTestsuite : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FloatSubnormalFtzFalsePrecFalseTestsuite, ExpFloatFtzFalsePrecFalse)
{
    float a = 1.17549421e-38f;
    float x = 1.0f;
    float y = -80.0f;
    float z = 0.0f;
    EXPECT_FLOAT_EQ(std::exp(x), expf(x));
    EXPECT_FLOAT_EQ(std::exp(a), expf(a));
    EXPECT_FLOAT_EQ(std::exp(y), expf(y));
    EXPECT_FLOAT_EQ(std::exp(z), expf(z));
}

TEST_F(FloatSubnormalFtzFalsePrecFalseTestsuite, SqrtFloatFtzFalsePrecFalse)
{
    float x = 1.0f;
    float y = 1.17549421e-38f;
    float z = 0.0f;
    EXPECT_FLOAT_EQ(std::sqrt(x), sqrtf(x));
    EXPECT_FLOAT_EQ(std::sqrt(y), sqrtf(y));
    EXPECT_FLOAT_EQ(std::sqrt(z), sqrtf(z));
}
