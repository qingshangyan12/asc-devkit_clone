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
#include <climits>
#include <type_traits>
#include "simt_api/asc_bf16.h"
using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

#define THREAD_DIM 128

struct FloatComputeParams1 {
    int32_t mode;
};

class FloatComputeTestsuite1 : public testing::Test, public testing::WithParamInterface<FloatComputeParams1> {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_absfloat)
{
    float x = 123.0;
    float y = -123.0;
    EXPECT_EQ(static_cast<float>(123.0), fabsf(x));
    EXPECT_EQ(static_cast<float>(123.0), fabsf(y));
    EXPECT_EQ(static_cast<float>(123.0), abs(y));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_nanf)
{
    EXPECT_TRUE(std::isnan(nanf("0x1")));
    EXPECT_TRUE(std::isnan(nanf("123")));
    EXPECT_TRUE(std::isnan(nanf("abc")));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_abshalf)
{
    half x = 123.0;
    half y = -123.0;
    EXPECT_EQ(static_cast<half>(123.0), __habs(x));
    EXPECT_EQ(static_cast<half>(123.0), __habs(y));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_absbfloat16t)
{
    bfloat16_t x = 123.0;
    bfloat16_t y = -123.0;
    EXPECT_EQ(static_cast<bfloat16_t>(123.0), __habs(x));
    EXPECT_EQ(static_cast<bfloat16_t>(123.0), __habs(y));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_fmahalf)
{
    half x = 1.0;
    half y = 2.0;
    half z = 3.0;
    EXPECT_EQ(static_cast<half>(5.0), __hfma(x, y, z));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_fmabfloat16t)
{
    bfloat16_t x = 1.0;
    bfloat16_t y = 2.0;
    bfloat16_t z = 3.0;
    EXPECT_EQ(static_cast<bfloat16_t>(5.0), __hfma(x, y, z));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_maxhalf)
{
    half x = 1.0;
    half y = 2.0;
    EXPECT_EQ(static_cast<half>(2.0), __hmax(x, y));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_maxbfloat16t)
{
    bfloat16_t x = 1.0;
    bfloat16_t y = 2.0;
    EXPECT_EQ(static_cast<bfloat16_t>(2.0), __hmax(x, y));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_minhalf)
{
    half x = 1.0;
    half y = 2.0;
    EXPECT_EQ(static_cast<half>(1.0), __hmin(x, y));
}

TEST_F(FloatComputeTestsuite1, FloatComputeTestCase_minbfloat16t)
{
    bfloat16_t x = 1.0;
    bfloat16_t y = 2.0;
    EXPECT_EQ(static_cast<bfloat16_t>(1.0), __hmin(x, y));
}

TEST_F(FloatComputeTestsuite1, MathIntegerTest)
{
    float src = 0.0f;
    src = 1.0f;

    long long int src_lln_x = -1;
    long long int src_lln_y = 1;
    long long int dst_lln = llmax(src_lln_x, src_lln_y);
    EXPECT_EQ(dst_lln, 1);

    long int x_ln = -1;
    long int dst_ln = labs(x_ln);
    EXPECT_EQ(dst_ln, 1);

    dst_lln = llabs(src_lln_x);
    EXPECT_EQ(dst_lln, 1);

    unsigned long long int src_ulln_x = 2;
    unsigned long long int src_ulln_y = 1;
    unsigned long long int dst_ulln = ullmax(src_ulln_x, src_ulln_y);
    EXPECT_EQ(dst_ulln, 2);

    unsigned int src_ui_x = 2;
    unsigned int src_ui_y = 1;
    unsigned int dst_ui = umax(src_ui_x, src_ui_y);
    EXPECT_EQ(dst_ui, 2);

    src_lln_x = -1;
    src_lln_y = 1;
    dst_lln = llmin(src_lln_x, src_lln_y);
    EXPECT_EQ(dst_lln, -1);

    src_ulln_x = 0;
    src_ulln_y = 1;
    dst_ulln = ullmin(src_ulln_x, src_ulln_y);
    EXPECT_EQ(dst_ulln, 0);

    src_ui_x = 0;
    src_ui_y = 1;
    dst_ui = umin(src_ui_x, src_ui_y);
    EXPECT_EQ(dst_ui, 0);

    EXPECT_EQ(max((unsigned int)2, -1), UINT32_MAX);
    EXPECT_EQ(max(-1, (unsigned int)2), UINT32_MAX);
    EXPECT_EQ(max((unsigned long)2, -1L), ULONG_MAX);
    EXPECT_EQ(max(-1L, (unsigned long int)2), ULONG_MAX);
    EXPECT_EQ(max((unsigned long long int)2, -1LL), ULLONG_MAX);
    EXPECT_EQ(max(-1LL, (unsigned long long int)2), ULLONG_MAX);
    EXPECT_EQ(min((unsigned int)2, -1), 2U);
    EXPECT_EQ(min(-1, (unsigned int)2), 2U);
    EXPECT_EQ(min((unsigned long int)2, -1L), 2UL);
    EXPECT_EQ(min(-1L, (unsigned long int)2), 2UL);
    EXPECT_EQ(min((unsigned long long int)2, -1LL), 2ULL);
    EXPECT_EQ(min(-1LL, (unsigned long long int)2), 2ULL);

    float x_f = 1.0f;
    float y_f = 2.0f;
    float res_div = fdividef(x_f, y_f);
    EXPECT_EQ(res_div, 0.5f);

    x_f = 0.0f;
    y_f = -1.0f;
    float res_after = nextafterf(x_f, y_f);
    EXPECT_EQ(res_after, -1.4013e-45f);

    x_f = 0.0f;
    y_f = 1.0f;
    res_after = nextafterf(x_f, y_f);
    EXPECT_EQ(res_after, 1.4013e-45f);

    x_f = NAN;
    y_f = 1.0f;
    res_after = nextafterf(x_f, y_f);
    EXPECT_TRUE(std::isnan(res_after));

    x_f = -1.0f;
    y_f = 0.0f;
    res_after = copysignf(x_f, y_f);
    EXPECT_EQ(res_after, 1.0f);
}

TEST_F(FloatComputeTestsuite1, HalfMathApiTest)
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
