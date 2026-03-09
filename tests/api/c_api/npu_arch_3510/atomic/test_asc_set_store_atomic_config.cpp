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

__aicore__ inline void st_atomic_stub(int8_t value, __gm__ int8_t* addr) {
    EXPECT_EQ(value, static_cast<uint8_t>(11));
    EXPECT_EQ(addr, reinterpret_cast<__gm__ int8_t *>(22));
}

class TEST_ST_ATOMIC : public testing::Test {
protected:
    void SetUp() {
        g_coreType = C_API_AIC_TYPE;
    }
    void TearDown() {
        g_coreType = C_API_AIV_TYPE;
    }
};

TEST_F(TEST_ST_ATOMIC, TEST_ST_ATOMIC)
{
    MOCKER_CPP(st_atomic, void(int8_t, __gm__ int8_t*))
            .times(1)
            .will(invoke(st_atomic_stub));

    int8_t value = static_cast<int8_t>(11);
    __gm__ int8_t *addr = reinterpret_cast<__gm__ int8_t *>(22);

    asc_set_store_atomic_config(value, addr);
    GlobalMockObject::verify();
}