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

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rn, asc_int642float_rn, vcvt, vector_float, vector_int64_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rn_v2, asc_int642float_rn_v2, vcvt, vector_float, vector_int64_t, 1);

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rna, asc_int642float_rna, vcvt, vector_float, vector_int64_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rna_v2, asc_int642float_rna_v2, vcvt, vector_float, vector_int64_t, 3);

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rd, asc_int642float_rd, vcvt, vector_float, vector_int64_t, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rd_v2, asc_int642float_rd_v2, vcvt, vector_float, vector_int64_t, 5);

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_ru, asc_int642float_ru, vcvt, vector_float, vector_int64_t, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_ru_v2, asc_int642float_ru_v2, vcvt, vector_float, vector_int64_t, 7);

TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rz, asc_int642float_rz, vcvt, vector_float, vector_int64_t, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Int642float_rz_v2, asc_int642float_rz_v2, vcvt, vector_float, vector_int64_t, 9);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos2601 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2601(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2601, asc_int642float_rd_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2601));

    asc_int642float_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2602 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2602(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2602, asc_int642float_rd_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2602));

    asc_int642float_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos2603 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2603(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2603, asc_int642float_rn_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2603));

    asc_int642float_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2604 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2604(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2604, asc_int642float_rn_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2604));

    asc_int642float_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos2605 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2605(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2605, asc_int642float_ru_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2605));

    asc_int642float_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2606 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2606(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2606, asc_int642float_ru_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2606));

    asc_int642float_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos2607 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2607(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2607, asc_int642float_rz_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2607));

    asc_int642float_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos2608 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_PosStub_2608(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos2608, asc_int642float_rz_ASC_POSITION_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_PosStub_2608));

    asc_int642float_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
