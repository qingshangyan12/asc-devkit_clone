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
<<<<<<< HEAD
void pld_stub(vector_bool& dst, __ubuf__ uint32_t* src, iter_reg offset, Literal dist)
=======
void pld_stub(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset, Literal dist)
>>>>>>> upstream/master
{
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t*>(22));
    EXPECT_EQ(dist, static_cast<Literal>(Literal::NORM));
}

<<<<<<< HEAD
void pld_upsample_stub(vector_bool& dst, __ubuf__ uint32_t* src, iter_reg offset, Literal dist)
=======
void pld_upsample_stub(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset, Literal dist)
>>>>>>> upstream/master
{
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t*>(22));
    EXPECT_EQ(dist, static_cast<Literal>(Literal::US));
}

<<<<<<< HEAD
void pld_downsample_stub(vector_bool& dst, __ubuf__ uint32_t* src, iter_reg offset, Literal dist)
=======
void pld_downsample_stub(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset, Literal dist)
>>>>>>> upstream/master
{
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t*>(22));
    EXPECT_EQ(dist, static_cast<Literal>(Literal::DS));
}
} // namespace

TEST_F(TestVectorDatamoveLoadAlignPld, LoadAlignPld_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t* src = reinterpret_cast<__ubuf__ uint32_t*>(22);
<<<<<<< HEAD
    iter_reg offset = asc_create_iter_reg_b32(64);

    MOCKER_CPP(pld, void(vector_bool&, __ubuf__ uint32_t*, iter_reg, Literal)).times(1).will(invoke(pld_stub));
=======
    addr_reg offset = asc_update_addr_reg_b32(64);

    MOCKER_CPP(pld, void(vector_bool&, __ubuf__ uint32_t*, addr_reg, Literal)).times(1).will(invoke(pld_stub));
>>>>>>> upstream/master

    asc_loadalign(dst, src, offset);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPld, LoadAlignPldUpsample_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t* src = reinterpret_cast<__ubuf__ uint32_t*>(22);
<<<<<<< HEAD
    iter_reg offset = asc_create_iter_reg_b32(64);

    MOCKER_CPP(pld, void(vector_bool&, __ubuf__ uint32_t*, iter_reg, Literal)).times(1).will(invoke(pld_upsample_stub));
=======
    addr_reg offset = asc_update_addr_reg_b32(64);

    MOCKER_CPP(pld, void(vector_bool&, __ubuf__ uint32_t*, addr_reg, Literal)).times(1).will(invoke(pld_upsample_stub));
>>>>>>> upstream/master

    asc_loadalign_upsample(dst, src, offset);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPld, LoadAlignPldDownsample_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t* src = reinterpret_cast<__ubuf__ uint32_t*>(22);
<<<<<<< HEAD
    iter_reg offset = asc_create_iter_reg_b32(64);

    MOCKER_CPP(pld, void(vector_bool&, __ubuf__ uint32_t*, iter_reg, Literal))
=======
    addr_reg offset = asc_update_addr_reg_b32(64);

    MOCKER_CPP(pld, void(vector_bool&, __ubuf__ uint32_t*, addr_reg, Literal))
>>>>>>> upstream/master
        .times(1)
        .will(invoke(pld_downsample_stub));

    asc_loadalign_downsample(dst, src, offset);
    GlobalMockObject::verify();
}
