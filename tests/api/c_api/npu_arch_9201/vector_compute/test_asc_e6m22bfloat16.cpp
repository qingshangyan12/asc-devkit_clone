/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

namespace {
void E6m2ToBf16Stub(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal) {}
} // namespace

TEST(E6m2ToBf16, PositionOverloads)
{
    vector_bfloat16_t dst;
    vector_fp8_e6m2_t src;
    vector_bool mask;
    MOCKER_CPP(vcvt_e6m22bf16, void(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal))
        .times(2)
        .will(invoke(E6m2ToBf16Stub));

    asc_e6m22bfloat16(dst, src, mask, ASC_POSITION_EVEN);
    asc_e6m22bfloat16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

TEST(E6m2ToBf16, ReturnAndV2Overloads)
{
    vector_fp8_e6m2_t src;
    vector_bool mask;
    MOCKER_CPP(vcvt_e6m22bf16, void(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal))
        .times(3)
        .will(invoke(E6m2ToBf16Stub));

    auto even = asc_e6m22bfloat16(src, mask, ASC_POSITION_EVEN);
    auto odd = asc_e6m22bfloat16(src, mask, ASC_POSITION_ODD);
    vector_bfloat16_t dst;
    asc_e6m22bfloat16_v2(dst, src, mask);
    static_assert(std::is_same<decltype(even), vector_bfloat16_t>::value);
    static_assert(std::is_same<decltype(odd), vector_bfloat16_t>::value);
    (void)even;
    (void)odd;
    GlobalMockObject::verify();
}

namespace {
void AscCastE6m2ToBf16EvenStub(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal part)
{
    EXPECT_EQ(part, PART_EVEN);
}

void AscCastE6m2ToBf16OddStub(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal part)
{
    EXPECT_EQ(part, PART_ODD);
}
} // namespace

TEST(AscCastE6m2ToBf16, Even)
{
    vector_bfloat16_t dst;
    vector_fp8_e6m2_t src;
    vector_bool mask;
    MOCKER_CPP(vcvt_e6m22bf16, void(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal))
        .times(1)
        .will(invoke(AscCastE6m2ToBf16EvenStub));

    asc_cast(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

TEST(AscCastE6m2ToBf16, Odd)
{
    vector_bfloat16_t dst;
    vector_fp8_e6m2_t src;
    vector_bool mask;
    MOCKER_CPP(vcvt_e6m22bf16, void(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal))
        .times(1)
        .will(invoke(AscCastE6m2ToBf16OddStub));

    asc_cast(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
