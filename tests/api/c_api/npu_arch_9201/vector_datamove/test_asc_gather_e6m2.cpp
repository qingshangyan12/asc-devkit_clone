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
void Gather2Stub(vector_fp8_e5m2_t&, __ubuf__ fp8_e5m2_t*, vector_uint16_t, vector_bool) {}
void GatherbMaskStub(vector_fp8_e5m2_t&, __ubuf__ fp8_e5m2_t*, vector_uint32_t, vector_bool) {}
void GatherbStub(vector_fp8_e5m2_t&, __ubuf__ fp8_e5m2_t*, vector_uint32_t) {}
} // namespace

TEST(GatherE6m2, Gather2RefAndReturn)
{
    vector_fp8_e6m2_t dst;
    vector_uint16_t index;
    vector_bool mask;
    auto* src = reinterpret_cast<__ubuf__ fp8_e6m2_t*>(0);
    MOCKER_CPP(vgather2, void(vector_fp8_e5m2_t&, __ubuf__ fp8_e5m2_t*, vector_uint16_t, vector_bool))
        .times(2)
        .will(invoke(Gather2Stub));

    asc_gather(dst, src, index, mask);
    auto result = asc_gather(src, index, mask);
    static_assert(std::is_same<decltype(result), vector_fp8_e6m2_t>::value);
    (void)result;
    GlobalMockObject::verify();
}

TEST(GatherE6m2, GatherbRefAndReturn)
{
    vector_fp8_e6m2_t dst;
    vector_uint32_t index;
    vector_bool mask;
    auto* src = reinterpret_cast<__ubuf__ fp8_e6m2_t*>(0);
    MOCKER_CPP(vgatherb, void(vector_fp8_e5m2_t&, __ubuf__ fp8_e5m2_t*, vector_uint32_t, vector_bool))
        .times(2)
        .will(invoke(GatherbMaskStub));
    MOCKER_CPP(vgatherb, void(vector_fp8_e5m2_t&, __ubuf__ fp8_e5m2_t*, vector_uint32_t))
        .times(2)
        .will(invoke(GatherbStub));

    asc_gather_datablock(dst, src, index, mask);
    auto mask_result = asc_gather_datablock(src, index, mask);
    asc_gather_datablock(dst, src, index);
    auto result = asc_gather_datablock(src, index);
    static_assert(std::is_same<decltype(mask_result), vector_fp8_e6m2_t>::value);
    static_assert(std::is_same<decltype(result), vector_fp8_e6m2_t>::value);
    (void)mask_result;
    (void)result;
    GlobalMockObject::verify();
}
