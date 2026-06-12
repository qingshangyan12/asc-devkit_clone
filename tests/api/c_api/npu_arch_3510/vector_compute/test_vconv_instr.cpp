/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "tests/api/c_api/npu_arch_3510/utils/test_vcvt_instr_utils.h"

// ==========asc_e5m22float==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt, asc_e5m22float, vcvt, vector_float, vector_fp8_e5m2_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v2, asc_e5m22float_v2, vcvt, vector_float, vector_fp8_e5m2_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v3, asc_e5m22float_v3, vcvt, vector_float, vector_fp8_e5m2_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v4, asc_e5m22float_v4, vcvt, vector_float, vector_fp8_e5m2_t, 3);

// ==========asc_int322int64==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt, asc_int322int64, vcvt, vector_int64_t, vector_int32_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v2, asc_int322int64_v2, vcvt, vector_int64_t, vector_int32_t, 1);

// ==========asc_uint82uint16==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt, asc_uint82uint16, vcvt, vector_uint16_t, vector_uint8_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v2, asc_uint82uint16_v2, vcvt, vector_uint16_t, vector_uint8_t, 1);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos3401 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3401(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3401, asc_e5m22float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3401));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3402 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3402(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3402, asc_e5m22float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3402));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3403 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3403(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3403, asc_e5m22float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3403));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3404 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3404(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3404, asc_e5m22float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_PosStub_3404));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos3405 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s64_x_PosStub_3405(vector_int64_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3405, asc_int322int64_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s64_x, void(vector_int64_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s64_x_PosStub_3405));

    asc_int322int64(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3406 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s64_x_PosStub_3406(vector_int64_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3406, asc_int322int64_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s64_x, void(vector_int64_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s64_x_PosStub_3406));

    asc_int322int64(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3407 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u16_x_PosStub_3407(vector_uint16_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3407, asc_uint82uint16_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u16_x, void(vector_uint16_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u16_x_PosStub_3407));

    asc_uint82uint16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3408 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u16_x_PosStub_3408(vector_uint16_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos3408, asc_uint82uint16_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u16_x, void(vector_uint16_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u16_x_PosStub_3408));

    asc_uint82uint16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
