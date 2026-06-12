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

TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Hif82float, asc_hif82float, vcvt, vector_float, vector_hifloat8_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Hif82float_v2, asc_hif82float_v2, vcvt, vector_float, vector_hifloat8_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Hif82float_v3, asc_hif82float_v3, vcvt, vector_float, vector_hifloat8_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Hif82float_v4, asc_hif82float_v4, vcvt, vector_float, vector_hifloat8_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos1501 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_PosStub_1501(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos1501, asc_hif82float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_PosStub_1501));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1502 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_PosStub_1502(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos1502, asc_hif82float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_PosStub_1502));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1503 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_PosStub_1503(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos1503, asc_hif82float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_PosStub_1503));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1504 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_PosStub_1504(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos1504, asc_hif82float_ASC_DISPERSE_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_PosStub_1504));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
