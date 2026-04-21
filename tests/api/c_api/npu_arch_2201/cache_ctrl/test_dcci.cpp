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
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"
#include "c_api/utils_intf.h"

class TestSimdAtomicDcci : public testing::Test { 
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void dcci_single_Stub(__gm__ void* dst)
{
    EXPECT_EQ((__gm__ void*)3, dst);
}

void dcci_single_ub_Stub(__ubuf__ void* dst)
{
    EXPECT_EQ((__ubuf__ void*)3, dst);
}

void dcci_entire_Stub(__gm__ void* dst)
{
    EXPECT_EQ((__gm__ void*)0, dst);
}

} // namespace

TEST_F(TestSimdAtomicDcci, dcci_gm_single)
{
    __gm__ void* dst = (__gm__ void*)3;
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t)).times(1).will(invoke(dcci_single_Stub));
    asc_dcci_single(dst);
    GlobalMockObject ::verify();
}

TEST_F(TestSimdAtomicDcci, dcci_ub_single)
{
    __ubuf__ void* dst = (__ubuf__ void*)3;
    MOCKER(dcci, void(__ubuf__ void*, uint64_t, uint64_t)).times(1).will(invoke(dcci_single_ub_Stub));
    asc_ub_dcci_single(dst);
    GlobalMockObject ::verify();
}

TEST_F(TestSimdAtomicDcci, dcci_gm_entire_ub_void_ptr_uint64_t_Succ)
{
    MOCKER(dcci, void(__ubuf__ void*, uint64_t, uint64_t)).times(1).will(invoke(dcci_entire_Stub));
    asc_dcci_entire_ub();
    GlobalMockObject ::verify();
}

#define TEST_DCCI(entire, type)                                                          \
TEST_F(TestSimdAtomicDcci, dcci_gm_##entire##_##type##_##void_ptr_uint64_t_Succ)         \
{                                                                                        \
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))                                 \
        .times(1)                                                                        \
        .will(invoke(dcci_entire_Stub));                                                        \
    asc_dcci_##entire##_##type();                                                     \
    GlobalMockObject::verify();                                                          \
}                                                                                        \

TEST_DCCI(entire, all);
TEST_DCCI(entire, out);
TEST_DCCI(entire, atomic);
