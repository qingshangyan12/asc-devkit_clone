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

TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt, asc_int82int16, vcvt, vector_int16_t, vector_int8_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_3(Vcvt_v2, asc_int82int16_v2, vcvt, vector_int16_t, vector_int8_t, 1);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2801 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s16_x_PosStub_2801(vector_int16_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2801, asc_int82int16_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s16_x, void(vector_int16_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s16_x_PosStub_2801));

    asc_int82int16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2802 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s16_x_PosStub_2802(vector_int16_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2802, asc_int82int16_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s16_x, void(vector_int16_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s16_x_PosStub_2802));

    asc_int82int16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
