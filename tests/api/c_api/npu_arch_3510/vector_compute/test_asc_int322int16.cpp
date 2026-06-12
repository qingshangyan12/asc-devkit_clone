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

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(vcvt, asc_int322int16, vcvt, vector_int16_t, vector_int32_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat, asc_int322int16_sat, vcvt, vector_int16_t, vector_int32_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_v2, asc_int322int16_v2, vcvt, vector_int16_t, vector_int32_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat_v2, asc_int322int16_sat_v2, vcvt, vector_int16_t, vector_int32_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2101 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_PosStub_2101(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2101, asc_int322int16_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_PosStub_2101));

    asc_int322int16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2102 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_PosStub_2102(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2102, asc_int322int16_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_PosStub_2102));

    asc_int322int16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2103 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_PosStub_2103(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2103, asc_int322int16_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_PosStub_2103));

    asc_int322int16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos2104 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_PosStub_2104(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2104, asc_int322int16_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_PosStub_2104));

    asc_int322int16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
