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

TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Int82half, asc_int82half, vcvt, vector_half, vector_int8_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_0(Int82half_v2, asc_int82half_v2, vcvt, vector_half, vector_int8_t, 1);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2701 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s82f16_x_PosStub_2701(vector_half dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2701, asc_int82half_ASC_POSITION_Succ) {
    vector_half dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s82f16_x, void(vector_half, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s82f16_x_PosStub_2701));

    asc_int82half(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2702 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s82f16_x_PosStub_2702(vector_half dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos2702, asc_int82half_ASC_POSITION_Succ) {
    vector_half dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s82f16_x, void(vector_half, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s82f16_x_PosStub_2702));

    asc_int82half(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
