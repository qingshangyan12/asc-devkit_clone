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
#include "c_api/c_api_utils_intf.h"

class TestSimdAtomicBcnt : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
int64_t asc_zero_bits_cnt_Stub(uint64_t offset)
{
    return 2;
}
}

TEST_F(TestSimdAtomicBcnt, zero_bits_cnt_Succ)
{
    uint64_t offset = 0;
    MOCKER_CPP(bcnt0, int64_t(uint64_t))
            .times(1)
            .will(invoke(asc_zero_bits_cnt_Stub));

    int64_t val = 2;
    EXPECT_EQ(2, val);
    GlobalMockObject::verify();
}