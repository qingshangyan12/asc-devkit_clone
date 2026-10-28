/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include <climits>
#include <type_traits>
#include "simt_api/asc_fp16.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

struct FP16MathParams {
    int32_t mode;
};

class FP16MathTestsuite : public testing::Test, public testing::WithParamInterface<FP16MathParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FP16MathTestsuite, ExtendedMathApiTest)
{
    half x = 1.0;
    half y = 2.0;
    half nanValue = __ushort_as_half(0x7e00U);
    half2 x2 = {x, y};
    half2 y2 = {y, x};
    half2 z2 = {half(3.0), half(4.0)};

    EXPECT_TRUE(__heq(x, x));
    EXPECT_TRUE(__hne(x, y));
    EXPECT_TRUE(__hle(x, y));
    EXPECT_TRUE(__hge(y, x));
    EXPECT_TRUE(__hlt(x, y));
    EXPECT_TRUE(__hgt(y, x));
    EXPECT_TRUE(__hequ(x, x));
    EXPECT_TRUE(__hneu(x, y));
    EXPECT_TRUE(__hleu(x, y));
    EXPECT_TRUE(__hgeu(y, x));
    EXPECT_TRUE(__hltu(x, y));
    EXPECT_TRUE(__hgtu(y, x));
    EXPECT_TRUE(__hbeqx2(x2, x2));
    EXPECT_TRUE(__hbnex2(x2, y2));
    EXPECT_TRUE(__hblex2(x2, y2));
    EXPECT_TRUE(__hbgex2(y2, x2));
    EXPECT_TRUE(__hbltx2(x2, y2));
    EXPECT_TRUE(__hbgtx2(y2, x2));
    EXPECT_TRUE(__hbequx2(x2, x2));
    EXPECT_TRUE(__hbneux2(x2, y2));
    EXPECT_TRUE(__hbleux2(x2, y2));
    EXPECT_TRUE(__hbgeux2(y2, x2));
    EXPECT_TRUE(__hbltux2(x2, y2));
    EXPECT_TRUE(__hbgtux2(y2, x2));

    EXPECT_TRUE(__heqx2(x2, x2).x);
    EXPECT_TRUE(__hnex2(x2, y2).x);
    EXPECT_TRUE(__hlex2(x2, y2).x);
    EXPECT_TRUE(__hgex2(y2, x2).x);
    EXPECT_TRUE(__hltx2(x2, y2).x);
    EXPECT_TRUE(__hgtx2(y2, x2).x);
    EXPECT_TRUE(__hequx2(x2, x2).x);
    EXPECT_TRUE(__hneux2(x2, y2).x);
    EXPECT_TRUE(__hleux2(x2, y2).x);
    EXPECT_TRUE(__hgeux2(y2, x2).x);
    EXPECT_TRUE(__hltux2(x2, y2).x);
    EXPECT_TRUE(__hgtux2(y2, x2).x);
    EXPECT_EQ(0x0101U, __heqx2_mask(x2, x2));
    EXPECT_EQ(0x0101U, __hnex2_mask(x2, y2));
    EXPECT_EQ(0x0101U, __hlex2_mask(x2, y2));
    EXPECT_EQ(0x0101U, __hgex2_mask(y2, x2));
    EXPECT_EQ(0x0101U, __hltx2_mask(x2, y2));
    EXPECT_EQ(0x0101U, __hgtx2_mask(y2, x2));
    EXPECT_EQ(0x0101U, __hequx2_mask(x2, x2));
    EXPECT_EQ(0x0101U, __hneux2_mask(x2, y2));
    EXPECT_EQ(0x0101U, __hleux2_mask(x2, y2));
    EXPECT_EQ(0x0101U, __hgeux2_mask(y2, x2));
    EXPECT_EQ(0x0101U, __hltux2_mask(x2, y2));
    EXPECT_EQ(0x0101U, __hgtux2_mask(y2, x2));
    EXPECT_TRUE(__isnanx2(half2{nanValue, x}).x);

    half2 add = __haddx2(x2, y2);
    half2 sub = __hsubx2(y2, x2);
    half2 mul = __hmulx2(x2, y2);
    half2 div = __hdivx2(y2, x2);
    half2 abs = __habsx2(half2{-x, -y});
    half2 neg = __hnegx2(x2);
    EXPECT_EQ(half(3.0), add.x);
    EXPECT_EQ(half(1.0), sub.x);
    EXPECT_EQ(half(2.0), mul.x);
    EXPECT_EQ(half(2.0), div.x);
    EXPECT_EQ(x, abs.x);
    EXPECT_EQ(-x, neg.x);
    EXPECT_EQ(__hmax_nan(nanValue, x), x);
    EXPECT_EQ(__hmin_nan(nanValue, x), x);
    EXPECT_EQ(__hmaxx2_nan(half2{nanValue, y}, x2).y, y);
    EXPECT_EQ(__hminx2_nan(half2{nanValue, y}, x2).y, y);
    EXPECT_EQ(half(5.0), __hfma_relu(x, y, x));
    EXPECT_EQ(half(0.0), __hfma_relu(-x, y, x));
    EXPECT_EQ(half2(3.0).x, __hfmax2(x2, y2, z2).x);
    EXPECT_EQ(half2(3.0).x, __hfmax2_relu(-x2, y2, z2).x);
    EXPECT_EQ(__hcmadd(x2, y2, z2).x, __hadd(__hmul(x, y), half(3.0)));
    EXPECT_EQ(__hadd(x, y), half(3.0));
    EXPECT_EQ(__hsub(x, y), half(-1.0));
    EXPECT_EQ(__hmul(x, y), half(2.0));
    EXPECT_EQ(__hdiv(y, x), half(2.0));
    EXPECT_EQ(__hneg(x), half(-1.0));

    float2 floatResult = __half22float2(x2);
    EXPECT_FLOAT_EQ(1.0f, floatResult.x);
    EXPECT_FLOAT_EQ(2.0f, floatResult.y);
    float2 floatInput = {1.0f, 2.0f};
    half2 halfResult = __float22half2_rz(floatInput);
    EXPECT_EQ(x2.x, halfResult.x);
    EXPECT_EQ(x2.y, halfResult.y);
}
