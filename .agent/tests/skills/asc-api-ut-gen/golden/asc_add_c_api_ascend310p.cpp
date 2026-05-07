/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement.
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

