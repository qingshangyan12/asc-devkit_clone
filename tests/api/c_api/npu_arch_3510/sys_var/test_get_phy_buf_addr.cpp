/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
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

class TestGetPhyBufAddr : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {   
uint64_t get_imm_stub(uint64_t offset)
{
    return 0;
}
uint64_t get_imm_stub1(uint64_t offset0, uint64_t offset1)
{
    return 1;
}
uint64_t get_imm_stub2(uint64_t offset0, uint64_t offset1, uint64_t offset2)
{
    return 2;
}
uint64_t get_imm_stub3(uint64_t offset0, uint64_t offset1, uint64_t offset2, uint64_t offset3)
{
    return 3;
}
}

TEST_F(TestGetPhyBufAddr, asc_get_phy_buf_addr_one_param_Succ)
{
    MOCKER_CPP(get_imm, uint64_t(uint64_t))
            .times(1)
            .will(invoke(get_imm_stub));

    int64_t val = asc_get_phy_buf_addr(0);
    EXPECT_EQ(0, val);
    GlobalMockObject::verify();
}

TEST_F(TestGetPhyBufAddr, asc_get_phy_buf_addr_two_param_Succ)
{
    MOCKER_CPP(get_imm, uint64_t(uint64_t, uint64_t))
        .times(1)
        .will(invoke(get_imm_stub1));

    int64_t val = asc_get_phy_buf_addr(0, 1);
    EXPECT_EQ(1, val);
    GlobalMockObject::verify();
}

TEST_F(TestGetPhyBufAddr, asc_get_phy_buf_addr_three_param_Succ)
{
    MOCKER_CPP(get_imm, uint64_t(uint64_t, uint64_t, uint64_t))
        .times(1)
        .will(invoke(get_imm_stub2));

    int64_t val = asc_get_phy_buf_addr(0, 1, 2);
    EXPECT_EQ(2, val);
    GlobalMockObject::verify();
}

TEST_F(TestGetPhyBufAddr, asc_get_phy_buf_addr_four_param_Succ)
{
    MOCKER_CPP(get_imm, uint64_t(uint64_t, uint64_t, uint64_t, uint64_t))
        .times(1)
        .will(invoke(get_imm_stub3));

    int64_t val = asc_get_phy_buf_addr(0, 1, 2, 3);
    EXPECT_EQ(3, val);
    GlobalMockObject::verify();
}
