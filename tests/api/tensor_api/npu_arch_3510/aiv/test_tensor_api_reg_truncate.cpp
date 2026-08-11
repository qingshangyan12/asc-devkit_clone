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

#include "tensor_api/arch/vector/experimental/reg_truncate.h"

namespace {

struct TruncateCall {
    vector_bool mask = 0;
};

TruncateCall g_truncate_call;

void HalfTruncateStub(vector_half&, vector_half, vector_bool mask)
{
    g_truncate_call.mask = mask;
}

void Bfloat16TruncateStub(vector_bfloat16_t&, vector_bfloat16_t, vector_bool mask)
{
    g_truncate_call.mask = mask;
}

void FloatTruncateStub(vector_float&, vector_float, vector_bool mask)
{
    g_truncate_call.mask = mask;
}

template <typename T>
using TruncateResult = decltype(asc::te::trunc(std::declval<asc::te::reg_tensor<T>>()));

static_assert(asc::te::reg_truncate_detail::is_supported<half>::value);
static_assert(asc::te::reg_truncate_detail::is_supported<bfloat16_t>::value);
static_assert(asc::te::reg_truncate_detail::is_supported<float>::value);
static_assert(!asc::te::reg_truncate_detail::is_supported<int32_t>::value);
static_assert(std::is_same_v<TruncateResult<half>, asc::te::reg_tensor<half>>);
static_assert(std::is_same_v<TruncateResult<bfloat16_t>, asc::te::reg_tensor<bfloat16_t>>);
static_assert(std::is_same_v<TruncateResult<float>, asc::te::reg_tensor<float>>);

class TestTensorApiRegTruncate : public testing::Test {
protected:
    void SetUp() override
    {
        g_truncate_call = {};
    }
};

TEST_F(TestTensorApiRegTruncate, DispatchesHalfAndPreservesMask)
{
    constexpr vector_bool mask = 17;
    MOCKER_CPP(asc_trunc, void(vector_half&, vector_half, vector_bool))
        .times(1)
        .will(invoke(HalfTruncateStub));

    asc::te::reg_tensor<half> src {};
    src.mask = mask;
    auto dst = asc::te::trunc(src);

    GlobalMockObject::verify();
    EXPECT_EQ(dst.mask, mask);
    EXPECT_EQ(g_truncate_call.mask, mask);
}

TEST_F(TestTensorApiRegTruncate, DispatchesBfloat16AndPreservesMask)
{
    constexpr vector_bool mask = 23;
    MOCKER_CPP(asc_trunc, void(vector_bfloat16_t&, vector_bfloat16_t, vector_bool))
        .times(1)
        .will(invoke(Bfloat16TruncateStub));

    asc::te::reg_tensor<bfloat16_t> src {};
    src.mask = mask;
    auto dst = asc::te::trunc(src);

    GlobalMockObject::verify();
    EXPECT_EQ(dst.mask, mask);
    EXPECT_EQ(g_truncate_call.mask, mask);
}

TEST_F(TestTensorApiRegTruncate, DispatchesFloatAndPreservesMask)
{
    constexpr vector_bool mask = 31;
    MOCKER_CPP(asc_trunc, void(vector_float&, vector_float, vector_bool))
        .times(1)
        .will(invoke(FloatTruncateStub));

    asc::te::reg_tensor<float> src {};
    src.mask = mask;
    auto dst = asc::te::trunc(src);

    GlobalMockObject::verify();
    EXPECT_EQ(dst.mask, mask);
    EXPECT_EQ(g_truncate_call.mask, mask);
}

} // namespace
