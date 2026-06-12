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

TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt, asc_e1m2x22bfloat16, vcvt, vector_bfloat16_t, vector_f4e1m2x2, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v2, asc_e1m2x22bfloat16_v2, vcvt, vector_bfloat16_t, vector_f4e1m2x2, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v3, asc_e1m2x22bfloat16_v3, vcvt, vector_bfloat16_t, vector_f4e1m2x2, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Vcvt_v4, asc_e1m2x22bfloat16_v4, vcvt, vector_bfloat16_t, vector_f4e1m2x2, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos501 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_501(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos501, asc_e1m2x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_501));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos502 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_502(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos502, asc_e1m2x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_502));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos503 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_503(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos503, asc_e1m2x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_503));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos504 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_504(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos504, asc_e1m2x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_PosStub_504));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
