/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License).
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

namespace {
void StorealignFp8e6m2Stub(
    vector_fp8_e6m2_t data, __ubuf__ fp8_e6m2_t* dst, int32_t offset, Literal dist, vector_bool mask)
{}
} // namespace

TEST(StorealignFp8e6m2, Succ)
{
    __ubuf__ fp8_e6m2_t* dst = nullptr;
    vector_fp8_e6m2_t src;
    vector_bool mask;

    MOCKER_CPP(vsts, void(vector_fp8_e6m2_t, __ubuf__ fp8_e6m2_t*, int32_t, Literal, vector_bool))
        .times(1)
        .will(invoke(StorealignFp8e6m2Stub));

    asc_storealign(dst, src, mask);
    GlobalMockObject::verify();
}
