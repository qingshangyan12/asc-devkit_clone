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
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

class TestVectorDatamoveLoadAlignPld : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void pld_stub(vector_bool &dst, __ubuf__ uint32_t *src, iter_reg offset, Literal dist) {
    EXPECT_EQ(dst, static_cast<vector_bool>(11));
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<iter_reg>(33));
    EXPECT_EQ(dist, static_cast<Literal>(Literal::NORM));
}
}

TEST_F(TestVectorDatamoveLoadAlignPld, LoadAlignPld_Succ)
{
    vector_bool dst = static_cast<vector_bool>(11);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    iter_reg offset = static_cast<iter_reg>(33);;

    MOCKER_CPP(pld, void(vector_bool &, __ubuf__ uint32_t *, iter_reg, Literal))       \
        .times(1)                                                                               \
        .will(invoke(pld_stub));

    asc_loadalign(dst, src, offset);
    GlobalMockObject::verify();
}