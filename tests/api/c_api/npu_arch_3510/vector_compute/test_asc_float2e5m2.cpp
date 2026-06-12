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
//================asc_float2e5m2_rn================
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn, asc_float2e5m2_rn, vcvt, vector_f8e5m2, vector_float, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_v2, asc_float2e5m2_rn_v2, vcvt, vector_f8e5m2, vector_float, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_v3, asc_float2e5m2_rn_v3, vcvt, vector_f8e5m2, vector_float, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_v4, asc_float2e5m2_rn_v4, vcvt, vector_f8e5m2, vector_float, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat, asc_float2e5m2_rn_sat, vcvt, vector_f8e5m2, vector_float, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat_v2, asc_float2e5m2_rn_sat_v2, vcvt, vector_f8e5m2, vector_float, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat_v3, asc_float2e5m2_rn_sat_v3, vcvt, vector_f8e5m2, vector_float, 7);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat_v4, asc_float2e5m2_rn_sat_v4, vcvt, vector_f8e5m2, vector_float, 8);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos701 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_701(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos701, asc_float2e5m2_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_701));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos702 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_702(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos702, asc_float2e5m2_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_702));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos703 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_703(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos703, asc_float2e5m2_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_703));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos704 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_704(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos704, asc_float2e5m2_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_704));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos705 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_705(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos705, asc_float2e5m2_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_705));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos706 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_706(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos706, asc_float2e5m2_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_706));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos707 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_707(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos707, asc_float2e5m2_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_707));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos708 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_PosStub_708(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos708, asc_float2e5m2_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_PosStub_708));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
