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

TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Int82int32, asc_int82int32, vcvt, vector_int32_t, vector_int8_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Int82int32_v2, asc_int82int32_v2, vcvt, vector_int32_t, vector_int8_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Int82int32_v3, asc_int82int32_v3, vcvt, vector_int32_t, vector_int8_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Int82int32_v4, asc_int82int32_v4, vcvt, vector_int32_t, vector_int8_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2901 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_PosStub_2901(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2901, asc_int82int32_ASC_DISPERSE_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_PosStub_2901));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2902 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_PosStub_2902(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2902, asc_int82int32_ASC_DISPERSE_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_PosStub_2902));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2903 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_PosStub_2903(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2903, asc_int82int32_ASC_DISPERSE_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_PosStub_2903));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos2904 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_PosStub_2904(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2904, asc_int82int32_ASC_DISPERSE_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_PosStub_2904));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
