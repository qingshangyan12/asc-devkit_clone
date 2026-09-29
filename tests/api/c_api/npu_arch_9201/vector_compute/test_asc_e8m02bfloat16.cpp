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
void E8m0ToBf16Stub(vector_bfloat16_t&, vector_fp8_e8m0_t, vector_bool, Literal) {}
} // namespace

TEST(E8m0ToBf16, PositionOverloads)
{
    vector_bfloat16_t dst;
    vector_fp8_e8m0_t src;
    vector_bool mask;
    MOCKER_CPP(vcvt_e8m02bf16, void(vector_bfloat16_t&, vector_fp8_e8m0_t, vector_bool, Literal))
        .times(2)
        .will(invoke(E8m0ToBf16Stub));

    asc_e8m02bfloat16(dst, src, mask, ASC_POSITION_EVEN);
    asc_e8m02bfloat16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

TEST(E8m0ToBf16, ReturnAndV2Overloads)
{
    vector_fp8_e8m0_t src;
    vector_bool mask;
    MOCKER_CPP(vcvt_e8m02bf16, void(vector_bfloat16_t&, vector_fp8_e8m0_t, vector_bool, Literal))
        .times(3)
        .will(invoke(E8m0ToBf16Stub));

    auto even = asc_e8m02bfloat16(src, mask, ASC_POSITION_EVEN);
    auto odd = asc_e8m02bfloat16(src, mask, ASC_POSITION_ODD);
    vector_bfloat16_t dst;
    asc_e8m02bfloat16_v2(dst, src, mask);
    static_assert(std::is_same<decltype(even), vector_bfloat16_t>::value);
    static_assert(std::is_same<decltype(odd), vector_bfloat16_t>::value);
    (void)even;
    (void)odd;
    GlobalMockObject::verify();
}
