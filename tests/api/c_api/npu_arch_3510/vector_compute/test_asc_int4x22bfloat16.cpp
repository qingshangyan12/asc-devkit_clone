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

TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt, asc_int4x22bfloat16, vcvt_s42bf16, vector_bfloat16_t, vector_int4x2_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v2, asc_int4x22bfloat16_v2, vcvt_s42bf16, vector_bfloat16_t, vector_int4x2_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v3, asc_int4x22bfloat16_v3, vcvt_s42bf16, vector_bfloat16_t, vector_int4x2_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v4, asc_int4x22bfloat16_v4, vcvt_s42bf16, vector_bfloat16_t, vector_int4x2_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2301 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_PosStub_2301(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2301, asc_int4x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_PosStub_2301));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2302 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_PosStub_2302(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2302, asc_int4x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_PosStub_2302));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2303 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_PosStub_2303(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2303, asc_int4x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_PosStub_2303));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2304 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_PosStub_2304(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2304, asc_int4x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_PosStub_2304));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
