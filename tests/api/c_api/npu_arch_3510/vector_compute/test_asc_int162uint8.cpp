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

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt, asc_int162uint8, vcvt, vector_uint8_t, vector_int16_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat, asc_int162uint8_sat, vcvt, vector_uint8_t, vector_int16_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_v2, asc_int162uint8_v2, vcvt, vector_uint8_t, vector_int16_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat_v2, asc_int162uint8_sat_v2, vcvt, vector_uint8_t, vector_int16_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2001 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_PosStub_2001(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2001, asc_int162uint8_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_PosStub_2001));

    asc_int162uint8(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2002 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_PosStub_2002(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2002, asc_int162uint8_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_PosStub_2002));

    asc_int162uint8_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2003 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_PosStub_2003(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2003, asc_int162uint8_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_PosStub_2003));

    asc_int162uint8(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos2004 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_PosStub_2004(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2004, asc_int162uint8_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_PosStub_2004));

    asc_int162uint8_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
