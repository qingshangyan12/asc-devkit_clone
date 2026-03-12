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

class TestVectorDatamoveLoadAlignPlds : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void plds_stub(vector_bool &dst, __ubuf__ uint32_t *src, int32_t offset, Dist dist) {
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<int32_t>(0));
    EXPECT_EQ(dist, static_cast<Literal>(Dist::DIST_NORM));
}

void plds_upsample_stub(vector_bool &dst, __ubuf__ uint32_t *src, int32_t offset, Dist dist) {
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<int32_t>(0));
    EXPECT_EQ(dist, static_cast<Literal>(Dist::DIST_US));
}

void plds_downsample_stub(vector_bool &dst, __ubuf__ uint32_t *src, int32_t offset, Dist dist) {
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<int32_t>(0));
    EXPECT_EQ(dist, static_cast<Literal>(Dist::DIST_DS));
}

void plds_offset_stub(vector_bool &dst, __ubuf__ uint32_t *src, int32_t offset, Dist dist) {
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<int32_t>(33));
    EXPECT_EQ(dist, static_cast<Literal>(Dist::DIST_NORM));
}

void plds_offset_upsample_stub(vector_bool &dst, __ubuf__ uint32_t *src, int32_t offset, Dist dist) {
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<int32_t>(33));
    EXPECT_EQ(dist, static_cast<Literal>(Dist::DIST_US));
}

void plds_offset_downsample_stub(vector_bool &dst, __ubuf__ uint32_t *src, int32_t offset, Dist dist) {
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint32_t *>(22));
    EXPECT_EQ(offset, static_cast<int32_t>(33));
    EXPECT_EQ(dist, static_cast<Literal>(Dist::DIST_DS));
}
}

TEST_F(TestVectorDatamoveLoadAlignPlds, LoadAlignPlds_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    
    MOCKER_CPP(plds, void(vector_bool &, __ubuf__ uint32_t *, int32_t, Dist))
        .times(1)
        .will(invoke(plds_stub));

    asc_loadalign(dst, src);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPlds, LoadAlignPldsUpsample_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    
    MOCKER_CPP(plds, void(vector_bool &, __ubuf__ uint32_t *, int32_t, Dist))
        .times(1)
        .will(invoke(plds_upsample_stub));

    asc_loadalign_upsample(dst, src);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPlds, LoadAlignPldsDownsample_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    
    MOCKER_CPP(plds, void(vector_bool &, __ubuf__ uint32_t *, int32_t, Dist))
        .times(1)
        .will(invoke(plds_downsample_stub));

    asc_loadalign_downsample(dst, src);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPlds, LoadAlignPldsOffset_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    int32_t offset = static_cast<int32_t>(33);
    
    MOCKER_CPP(plds, void(vector_bool &, __ubuf__ uint32_t *, int32_t, Dist))
        .times(1)
        .will(invoke(plds_offset_stub));

    asc_loadalign(dst, src, offset);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPlds, LoadAlignPldsOffsetUpsample_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    int32_t offset = static_cast<int32_t>(33);

    MOCKER_CPP(plds, void(vector_bool &, __ubuf__ uint32_t *, int32_t, Dist))
        .times(1)
        .will(invoke(plds_offset_upsample_stub));

    asc_loadalign_upsample(dst, src, offset);
    GlobalMockObject::verify();
}

TEST_F(TestVectorDatamoveLoadAlignPlds, LoadAlignPldsOffsetDownsample_Succ)
{
    vector_bool dst = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint32_t *src = reinterpret_cast<__ubuf__ uint32_t *>(22);
    int32_t offset = static_cast<int32_t>(33);

    MOCKER_CPP(plds, void(vector_bool &, __ubuf__ uint32_t *, int32_t, Dist))
        .times(1)
        .will(invoke(plds_offset_downsample_stub));

    asc_loadalign_downsample(dst, src, offset);
    GlobalMockObject::verify();
}