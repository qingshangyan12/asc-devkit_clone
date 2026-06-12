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

class TestHalf2Float : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
    void vcvt_Stub(vector_float& dst, vector_half src, vector_bool mask, Literal part, Literal mode){}
}

TEST_F(TestHalf2Float, half_to_float_Succ)
{
    vector_float dst;
    vector_half src;
    vector_bool mask;
    MOCKER(vcvt, void(vector_float&, vector_half, vector_bool, Literal, Literal))
        .times(1)
        .will(invoke(vcvt_Stub));

    asc_half2float(dst, src, mask);
    GlobalMockObject::verify();
}

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos1001 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162f32_x_PosStub_1001(vector_float dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos1001, asc_half2float_ASC_POSITION_Succ) {
    vector_float dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162f32_x, void(vector_float, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162f32_x_PosStub_1001));

    asc_half2float(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1002 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162f32_x_PosStub_1002(vector_float dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos1002, asc_half2float_ASC_POSITION_Succ) {
    vector_float dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162f32_x, void(vector_float, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162f32_x_PosStub_1002));

    asc_half2float(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
