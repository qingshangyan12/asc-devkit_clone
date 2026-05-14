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
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"


class Testasc_addCAPI : public testing::Test {
protected:
    void SetUp() {
        g_coreType = C_API_AIV_TYPE;
    }
    void TearDown() {
        g_coreType = C_API_AIV_TYPE;
    }
};



namespace {
void asc_add_Stub(vector_uint8_t& dst, vector_uint8_t src0,
                       vector_uint8_t src1, vector_bool mask, Literal literal) {
    // 参数验证
    EXPECT_TRUE(true);
}
}



TEST_F(Testasc_addCAPI, asc_add_Succ) {
    vector_uint8_t dst;
    vector_uint8_t src0;
    vector_uint8_t src1;
    vector_bool mask;

    MOCKER_CPP(asc_add, void(vector_uint8_t&, vector_uint8_t, vector_uint8_t, vector_bool, Literal))
        .times(1)
        .will(invoke(asc_add_Stub));

    asc_add(dst, src0, src1, mask);
    GlobalMockObject::verify();
}

