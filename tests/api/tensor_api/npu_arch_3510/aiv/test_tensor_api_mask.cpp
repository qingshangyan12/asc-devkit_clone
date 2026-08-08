/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <array>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"

#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#include "impl/tensor_api/experimental/reg/mask_impl.h"
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS

namespace {

struct MaskCalls {
    std::vector<Literal> b8;
    std::vector<Literal> b16;
    std::vector<Literal> b32;
};

MaskCalls g_mask_calls;

vector_bool PsetB8Stub(Literal pattern)
{
    g_mask_calls.b8.push_back(pattern);
    return {};
}

vector_bool PsetB16Stub(Literal pattern)
{
    g_mask_calls.b16.push_back(pattern);
    return {};
}

vector_bool PsetB32Stub(Literal pattern)
{
    g_mask_calls.b32.push_back(pattern);
    return {};
}

template <typename T>
void MakeAllMaskPatterns()
{
    (void)asc::te::make_mask<asc::te::mask_pattern::all, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl1, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl2, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl3, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl4, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl8, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl16, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl32, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl64, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::vl128, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::every3, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::every4, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::half, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::quarter, T>();
    (void)asc::te::make_mask<asc::te::mask_pattern::none, T>();
}

template <std::size_t N>
void ExpectPatterns(const std::vector<Literal>& actual, const std::array<Literal, N>& expected)
{
    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

class TestTensorApiMask : public testing::Test {
protected:
    void SetUp() override
    {
        g_mask_calls = {};
    }
};

static_assert(std::is_same_v<
    decltype(asc::te::make_mask<asc::te::mask_pattern::all, uint8_t>()), asc::te::reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(asc::te::zeros<uint8_t>()), asc::te::reg_tensor<bool>>);

TEST_F(TestTensorApiMask, MakeMaskMapsPatternsAndDispatchesByElementWidth)
{
    MOCKER_CPP(pset_b8, vector_bool(Literal)).times(15).will(invoke(PsetB8Stub));
    MOCKER_CPP(pset_b16, vector_bool(Literal)).times(15).will(invoke(PsetB16Stub));
    MOCKER_CPP(pset_b32, vector_bool(Literal)).times(15).will(invoke(PsetB32Stub));

    MakeAllMaskPatterns<uint8_t>();
    MakeAllMaskPatterns<uint16_t>();
    MakeAllMaskPatterns<uint32_t>();

    GlobalMockObject::verify();

    constexpr std::array<Literal, 15> expected = {
        PAT_ALL, PAT_VL1, PAT_VL2, PAT_VL3, PAT_VL4, PAT_VL8, PAT_VL16, PAT_VL32,
        PAT_VL64, PAT_VL128, PAT_M3, PAT_M4, PAT_H, PAT_Q, PAT_ALLF};
    ExpectPatterns(g_mask_calls.b8, expected);
    ExpectPatterns(g_mask_calls.b16, expected);
    ExpectPatterns(g_mask_calls.b32, expected);
}

TEST_F(TestTensorApiMask, ZerosCreatesAllFalseMaskForEachElementWidth)
{
    MOCKER_CPP(pset_b8, vector_bool(Literal)).times(1).will(invoke(PsetB8Stub));
    MOCKER_CPP(pset_b16, vector_bool(Literal)).times(1).will(invoke(PsetB16Stub));
    MOCKER_CPP(pset_b32, vector_bool(Literal)).times(1).will(invoke(PsetB32Stub));

    (void)asc::te::zeros<uint8_t>();
    (void)asc::te::zeros<uint16_t>();
    (void)asc::te::zeros<uint32_t>();

    GlobalMockObject::verify();

    constexpr std::array<Literal, 1> expected = {PAT_ALLF};
    ExpectPatterns(g_mask_calls.b8, expected);
    ExpectPatterns(g_mask_calls.b16, expected);
    ExpectPatterns(g_mask_calls.b32, expected);
}

} // namespace
