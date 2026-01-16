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
void dcci_Stub(__gm__ void* dst, uint64_t entire)
{
    EXPECT_EQ((__gm__ void*)1, dst);
    EXPECT_EQ(2, entire);
}
}

TEST_F(TestSimdAtomicDcci, dcci_gm_void_ptr_uint64_t_Succ)
{
    __gm__ void* dst = (__gm__ void*)1;
    uint64_t entire = 2;
    MOCKER(dcci, void(__gm__ void*, uint64_t))
        .times(1)
        .will(invoke(dcci_Stub));
    
    asc_dcci(dst, entire);
    GlobalMockObject::verify();
}

TEST_F(TestSimdAtomicDcci, dcci_gm_all_void_ptr_uint64_t_Succ)
{
    __gm__ void* dst = (__gm__ void*)1;
    uint64_t entire = 2;
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(dcci_Stub));
    
    asc_dcci_all(dst, entire);
    GlobalMockObject::verify();
}

TEST_F(TestSimdAtomicDcci, dcci_gm_ub_void_ptr_uint64_t_Succ)
{
    __gm__ void* dst = (__gm__ void*)1;
    uint64_t entire = 2;
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(dcci_Stub));
    
    asc_dcci_ub(dst, entire);
    GlobalMockObject::verify();
}

TEST_F(TestSimdAtomicDcci, dcci_gm_out_void_ptr_uint64_t_Succ)
{
    __gm__ void* dst = (__gm__ void*)1;
    uint64_t entire = 2;
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(dcci_Stub));
    
    asc_dcci_out(dst, entire);
    GlobalMockObject::verify();
}

TEST_F(TestSimdAtomicDcci, dcci_gm_atomic_void_ptr_uint64_t_Succ)
{
    __gm__ void* dst = (__gm__ void*)1;
    uint64_t entire = 2;
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(dcci_Stub));
    
    asc_dcci_atomic(dst, entire);
    GlobalMockObject::verify();
}