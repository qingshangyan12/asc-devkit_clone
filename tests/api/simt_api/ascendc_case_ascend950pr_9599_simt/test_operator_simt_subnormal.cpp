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
#include <type_traits>
// #include <cmath>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_api/math_functions.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

#ifdef __ASC_FTZ__
#undef __ASC_FTZ__
#endif

#define THREAD_DIM 128

struct FloatSubnormalParams {
    int32_t mode;
};

class FloatSubnormalTestsuite : public testing::Test, public testing::WithParamInterface<FloatSubnormalParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FloatSubnormalTestsuite, FloatSubnormalTestCase_expfloatftzfasle)
{
    float x = 1.0f;
    float y = 2.0f;
    EXPECT_EQ(exp(x), expf(x));
    EXPECT_EQ(exp(y), expf(y));
}

TEST_F(FloatSubnormalTestsuite, FloatSubnormalTestCase_sqrtfloatftzfasle)
{
    float x = 1.0f;
    float y = 2.0f;
    EXPECT_EQ(sqrt(x), sqrtf(x));
    EXPECT_EQ(sqrt(y), sqrtf(y));
}
