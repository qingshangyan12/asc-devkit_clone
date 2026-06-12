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

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rh, asc_half2hif8_rh, vcvt, vector_hifloat8_t, vector_half, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rh_sat, asc_half2hif8_rh_sat, vcvt, vector_hifloat8_t, vector_half, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rh_v2, asc_half2hif8_rh_v2, vcvt, vector_hifloat8_t, vector_half, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rh_sat_v2, asc_half2hif8_rh_sat_v2, vcvt, vector_hifloat8_t, vector_half, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna, asc_half2hif8_rna, vcvt, vector_hifloat8_t, vector_half, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat, asc_half2hif8_rna_sat, vcvt, vector_hifloat8_t, vector_half, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_v2, asc_half2hif8_rna_v2, vcvt, vector_hifloat8_t, vector_half, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat_v2, asc_half2hif8_rna_sat_v2, vcvt, vector_hifloat8_t, vector_half, 7);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos1101 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_PosStub_1101(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1101, asc_half2hif8_rh_ASC_POSITION_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_PosStub_1101));

    asc_half2hif8_rh(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1102 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_PosStub_1102(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1102, asc_half2hif8_rh_sat_ASC_POSITION_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_PosStub_1102));

    asc_half2hif8_rh_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1103 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_PosStub_1103(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1103, asc_half2hif8_rh_ASC_POSITION_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_PosStub_1103));

    asc_half2hif8_rh(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1104 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_PosStub_1104(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1104, asc_half2hif8_rh_sat_ASC_POSITION_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_PosStub_1104));

    asc_half2hif8_rh_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
