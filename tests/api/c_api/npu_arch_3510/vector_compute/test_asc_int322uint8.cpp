/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "tests/api/c_api/npu_arch_3510/utils/test_vcvt_instr_utils.h"

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8, asc_int322uint8, vcvt, vector_uint8_t, vector_int32_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_sat, asc_int322uint8_sat, vcvt, vector_uint8_t, vector_int32_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_v2, asc_int322uint8_v2, vcvt, vector_uint8_t, vector_int32_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_sat_v2, asc_int322uint8_sat_v2, vcvt, vector_uint8_t, vector_int32_t, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_v3, asc_int322uint8_v3, vcvt, vector_uint8_t, vector_int32_t, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_sat_v3, asc_int322uint8_sat_v3, vcvt, vector_uint8_t, vector_int32_t, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_v4, asc_int322uint8_v4, vcvt, vector_uint8_t, vector_int32_t, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int322uint8_sat_v4, asc_int322uint8_sat_v4, vcvt, vector_uint8_t, vector_int32_t, 7);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2201 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2201(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2201, asc_int322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2201));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2202 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2202(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2202, asc_int322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2202));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2203 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2203(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2203, asc_int322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2203));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2204 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2204(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2204, asc_int322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2204));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2205 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2205(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2205, asc_int322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2205));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2206 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2206(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2206, asc_int322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2206));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2207 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2207(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2207, asc_int322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2207));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2208 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_PosStub_2208(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2208, asc_int322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_PosStub_2208));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
