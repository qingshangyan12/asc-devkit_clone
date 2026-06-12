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

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8, asc_uint322uint8, vcvt, vector_uint8_t, vector_uint32_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_sat, asc_uint322uint8_sat, vcvt, vector_uint8_t, vector_uint32_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_v2, asc_uint322uint8_v2, vcvt, vector_uint8_t, vector_uint32_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_sat_v2, asc_uint322uint8_sat_v2, vcvt, vector_uint8_t, vector_uint32_t, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_v3, asc_uint322uint8_v3, vcvt, vector_uint8_t, vector_uint32_t, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_sat_v3, asc_uint322uint8_sat_v3, vcvt, vector_uint8_t, vector_uint32_t, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_v4, asc_uint322uint8_v4, vcvt, vector_uint8_t, vector_uint32_t, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Uint322uint8_sat_v4, asc_uint322uint8_sat_v4, vcvt, vector_uint8_t, vector_uint32_t, 7);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos3201 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3201(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3201, asc_uint322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3201));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3202 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3202(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3202, asc_uint322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3202));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3203 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3203(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3203, asc_uint322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3203));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3204 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3204(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3204, asc_uint322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3204));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3205 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3205(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3205, asc_uint322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3205));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3206 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3206(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3206, asc_uint322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3206));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3207 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3207(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3207, asc_uint322uint8_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3207));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3208 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_PosStub_3208(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3208, asc_uint322uint8_sat_ASC_DISPERSE_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_PosStub_3208));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
