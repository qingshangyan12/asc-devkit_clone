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

TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt, asc_int4x22int16, vcvt_s42s16, vector_int16_t, vector_int4x2_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v2, asc_int4x22int16_v2, vcvt_s42s16, vector_int16_t, vector_int4x2_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v3, asc_int4x22int16_v3, vcvt_s42s16, vector_int16_t, vector_int4x2_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v4, asc_int4x22int16_v4, vcvt_s42s16, vector_int16_t, vector_int4x2_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2501 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_PosStub_2501(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2501, asc_int4x22int16_ASC_DISPERSE_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_PosStub_2501));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2502 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_PosStub_2502(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2502, asc_int4x22int16_ASC_DISPERSE_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_PosStub_2502));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2503 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_PosStub_2503(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2503, asc_int4x22int16_ASC_DISPERSE_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_PosStub_2503));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2504 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_PosStub_2504(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2504, asc_int4x22int16_ASC_DISPERSE_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_PosStub_2504));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
