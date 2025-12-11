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
#include "c_api/c_api_interf_util.h"

class TestDCPreload : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {

void dc_preload_int16_t_Stub(uint64_t* address, int16_t offset)
{
    EXPECT_EQ(address, reinterpret_cast<uint64_t*>(11));
    EXPECT_EQ(offset, static_cast<int16_t>(22));
}

void dc_preload_int64_t_Stub(uint64_t* address, int64_t offset)
{
    EXPECT_EQ(address, reinterpret_cast<uint64_t*>(11));
    EXPECT_EQ(offset, static_cast<int64_t>(22));
}

}

TEST_F(TestDCPreload, asc_data_cache_preload_uint64_t_addr_int64_t_Succ)
{
    uint64_t* address = reinterpret_cast<uint64_t*>(11);
    int64_t offset = 22;
    MOCKER_CPP(dc_preload, void(uint64_t*, int64_t))
        .times(1)
        .will(invoke(dc_preload_int64_t_Stub));
    asc_data_cache_preload(address, offset);
    GlobalMockObject::verify();
}

TEST_F(TestDCPreload, asc_data_cache_preload_uint64_t_addr_int16_t_Succ)
{
    uint64_t* address = reinterpret_cast<uint64_t*>(11);
    int16_t offset = 22;
    MOCKER_CPP(dc_preload, void(uint64_t*, int16_t))
        .times(1)
        .will(invoke(dc_preload_int16_t_Stub));
    asc_data_cache_preload(address, offset);
    GlobalMockObject::verify();
}