/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>
#include <utility>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"

#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#include "impl/tensor_api/arch/vector/experimental/cast/reg_cast.h"
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS

namespace {

using asc::te::cast_layout;
using asc::te::cast_round_mode;
using asc::te::cast_sat_mode;

struct VcvtCall {
    vector_bool mask = 0;
    Literal round = MODE_ZEROING;
    Literal sat = MODE_ZEROING;
    Literal part = MODE_ZEROING;
    Literal mode = MODE_ZEROING;
};

VcvtCall g_vcvt_call;

void HalfToFloatVcvtStub(
    vector_float&, vector_half, vector_bool mask, Literal part, Literal mode)
{
    g_vcvt_call.mask = mask;
    g_vcvt_call.part = part;
    g_vcvt_call.mode = mode;
}

void FloatToHalfVcvtStub(vector_half&, vector_float, vector_bool mask, Literal round,
    Literal sat, Literal part, Literal mode)
{
    g_vcvt_call.mask = mask;
    g_vcvt_call.round = round;
    g_vcvt_call.sat = sat;
    g_vcvt_call.part = part;
    g_vcvt_call.mode = mode;
}

void Int32ToUint8Stub(vector_uint8_t&, vector_int32_t, vector_bool mask)
{
    g_vcvt_call.mask = mask;
}

class TestTensorApiRegCast : public testing::Test {
protected:
    void SetUp() override
    {
        g_vcvt_call = {};
    }
};

static_assert(asc::te::reg_cast_detail::reg_cast_op<float, half, cast_layout::zero,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);
static_assert(asc::te::reg_cast_detail::reg_cast_op<half, float, cast_layout::one,
    cast_round_mode::ceil, cast_sat_mode::sat>::supported);
static_assert(asc::te::reg_cast_detail::reg_cast_op<uint8_t, int32_t, cast_layout::two,
    cast_round_mode::rint, cast_sat_mode::sat>::supported);
static_assert(!asc::te::reg_cast_detail::reg_cast_op<float, half, cast_layout::two,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);

using HalfReg = asc::te::reg_tensor<half>;
static_assert(std::is_same_v<decltype(asc::te::cast<float>(std::declval<HalfReg>())),
    asc::te::reg_tensor<float>>);

TEST_F(TestTensorApiRegCast, DefaultPolicyDispatchesHalfToFloatAndPreservesMask)
{
    constexpr vector_bool mask = 17;
    MOCKER_CPP(vcvt, void(vector_float&, vector_half, vector_bool, Literal, Literal))
        .times(1)
        .will(invoke(HalfToFloatVcvtStub));

    asc::te::reg_tensor<half> src {};
    src.mask = mask;
    auto dst = asc::te::cast<float>(src);

    GlobalMockObject::verify();
    EXPECT_EQ(dst.mask, mask);
    EXPECT_EQ(g_vcvt_call.mask, mask);
    EXPECT_EQ(g_vcvt_call.part, PART_EVEN);
    EXPECT_EQ(g_vcvt_call.mode, MODE_ZEROING);
}

TEST_F(TestTensorApiRegCast, ExplicitPolicyDispatchesLayoutRoundAndSaturation)
{
    constexpr vector_bool mask = 23;
    MOCKER_CPP(vcvt,
        void(vector_half&, vector_float, vector_bool, Literal, Literal, Literal, Literal))
        .times(1)
        .will(invoke(FloatToHalfVcvtStub));

    asc::te::reg_tensor<float> src {};
    src.mask = mask;
    auto dst = asc::te::cast<half, cast_layout::one, cast_round_mode::ceil,
        cast_sat_mode::sat>(src);

    GlobalMockObject::verify();
    EXPECT_EQ(dst.mask, mask);
    EXPECT_EQ(g_vcvt_call.mask, mask);
    EXPECT_EQ(g_vcvt_call.round, ROUND_C);
    EXPECT_EQ(g_vcvt_call.sat, RS_ENABLE);
    EXPECT_EQ(g_vcvt_call.part, PART_ODD);
    EXPECT_EQ(g_vcvt_call.mode, MODE_ZEROING);
}

TEST_F(TestTensorApiRegCast, FourWayNarrowingDispatchesLayoutTwoSaturatedIntrinsic)
{
    constexpr vector_bool mask = 31;
    MOCKER_CPP(asc_int322uint8_sat_v3,
        void(vector_uint8_t&, vector_int32_t, vector_bool))
        .times(1)
        .will(invoke(Int32ToUint8Stub));

    asc::te::reg_tensor<int32_t> src {};
    src.mask = mask;
    auto dst = asc::te::cast<uint8_t, cast_layout::two, cast_round_mode::rint,
        cast_sat_mode::sat>(src);

    GlobalMockObject::verify();
    EXPECT_EQ(dst.mask, mask);
    EXPECT_EQ(g_vcvt_call.mask, mask);
}

} // namespace
