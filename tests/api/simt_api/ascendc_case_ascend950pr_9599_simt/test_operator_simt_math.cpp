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
#include <type_traits>
#include <cmath>
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;


struct FloatComputeParams {
 int32_t mode;
};

class FloatComputeTestsuite : public testing::Test, public testing::WithParamInterface<FloatComputeParams> {
protected:
 void SetUp() {}
 void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(FloatComputeTestCase, FloatComputeTestsuite,
 ::testing::Values(FloatComputeParams {6},
 FloatComputeParams {7},
 FloatComputeParams {8},
 FloatComputeParams {9},
 FloatComputeParams {10},
 FloatComputeParams {11},
 FloatComputeParams {12},
 FloatComputeParams {13},
 FloatComputeParams {14},
 FloatComputeParams {15},
 FloatComputeParams {16},
 FloatComputeParams {17},
 FloatComputeParams {18}
 	               ));

TEST_F(FloatComputeTestsuite, FloatComputeTestCaseAbs)
{
     half x = 123.0;
     half y = -123.0;
     EXPECT_EQ(static_cast<half>(123.0), Simt::Abs(x));
     EXPECT_EQ(static_cast<half>(123.0), Simt::Abs(y));
}

TEST_F(FloatComputeTestsuite, FloatComputeTestCaseFma)
{
     half x = 1.0;
     half y = 2.0;
     half z = 3.0;
     EXPECT_EQ(static_cast<half>(5.0), Simt::Fma(x, y, z));
}

TEST_F(FloatComputeTestsuite, FloatComputeTestCaseCover)
{
    int32_t x = 1;
    int32_t y = 2;
    int32_t z = Simt::Max(x, y);
    EXPECT_EQ(z, y);
    z = Simt::Min(x, y);
    EXPECT_EQ(z, x);
    float f_1 = 1.0f;
    float f_2 = 2.0f;
    float f_3 = Simt::Fdim(f_1, f_2);
    f_3 = Simt::RemQuo(f_1, f_2, &z);
    f_3 = Simt::Mod(f_1, f_2);
    f_3 = Simt::Remainder(f_1, f_2);
    f_3 = Simt::CopySign(f_1, f_2);
    f_3 = Simt::NearbyInt(f_1);
    f_3 = Simt::NextAfter(f_1, f_2);
    f_3 = Simt::ScaLbn(f_1, z);
}