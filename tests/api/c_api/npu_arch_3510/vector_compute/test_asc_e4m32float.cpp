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

TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt, asc_e4m32float, vcvt, vector_float, vector_fp8_e4m3fn_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v2, asc_e4m32float_v2, vcvt, vector_float, vector_fp8_e4m3fn_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v3, asc_e4m32float_v3, vcvt, vector_float, vector_fp8_e4m3fn_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v4, asc_e4m32float_v4, vcvt, vector_float, vector_fp8_e4m3fn_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos601 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_PosStub_601(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos601, asc_e4m32float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_PosStub_601));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos602 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_PosStub_602(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos602, asc_e4m32float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_PosStub_602));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos603 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_PosStub_603(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos603, asc_e4m32float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_PosStub_603));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos604 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_PosStub_604(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos604, asc_e4m32float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_PosStub_604));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
