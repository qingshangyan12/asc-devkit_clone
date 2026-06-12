/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "tests/api/c_api/npu_arch_3510/utils/test_vcvt_instr_utils.h"

// ==========asc_float2bfloat16==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd, asc_float2bfloat16_rd, vcvt, vector_bfloat16_t, vector_float, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_sat, asc_float2bfloat16_rd_sat, vcvt, vector_bfloat16_t, vector_float, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_v2, asc_float2bfloat16_rd_v2, vcvt, vector_bfloat16_t, vector_float, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_sat_v2, asc_float2bfloat16_rd_sat_v2, vcvt, vector_bfloat16_t, vector_float, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn, asc_float2bfloat16_rn, vcvt, vector_bfloat16_t, vector_float, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat, asc_float2bfloat16_rn_sat, vcvt, vector_bfloat16_t, vector_float, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_v2, asc_float2bfloat16_rn_v2, vcvt, vector_bfloat16_t, vector_float, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat_v2, asc_float2bfloat16_rn_sat_v2, vcvt, vector_bfloat16_t, vector_float, 7);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna, asc_float2bfloat16_rna, vcvt, vector_bfloat16_t, vector_float, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat, asc_float2bfloat16_rna_sat, vcvt, vector_bfloat16_t, vector_float, 9);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_v2, asc_float2bfloat16_rna_v2, vcvt, vector_bfloat16_t, vector_float, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat_v2, asc_float2bfloat16_rna_sat_v2, vcvt, vector_bfloat16_t, vector_float, 11);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru, asc_float2bfloat16_ru, vcvt, vector_bfloat16_t, vector_float, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_sat, asc_float2bfloat16_ru_sat, vcvt, vector_bfloat16_t, vector_float, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_v2, asc_float2bfloat16_ru_v2, vcvt, vector_bfloat16_t, vector_float, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_sat_v2, asc_float2bfloat16_ru_sat_v2, vcvt, vector_bfloat16_t, vector_float, 15);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz, asc_float2bfloat16_rz, vcvt, vector_bfloat16_t, vector_float, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_sat, asc_float2bfloat16_rz_sat, vcvt, vector_bfloat16_t, vector_float, 17);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_v2, asc_float2bfloat16_rz_v2, vcvt, vector_bfloat16_t, vector_float, 18);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_sat_v2, asc_float2bfloat16_rz_sat_v2, vcvt, vector_bfloat16_t, vector_float, 19);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos3601 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3601(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3601, asc_float2bfloat16_rd_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3601));

    asc_float2bfloat16_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3602 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3602(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3602, asc_float2bfloat16_rd_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3602));

    asc_float2bfloat16_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3603 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3603(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3603, asc_float2bfloat16_rd_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3603));

    asc_float2bfloat16_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3604 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3604(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3604, asc_float2bfloat16_rd_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3604));

    asc_float2bfloat16_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3605 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3605(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3605, asc_float2bfloat16_rn_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3605));

    asc_float2bfloat16_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3606 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3606(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3606, asc_float2bfloat16_rn_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3606));

    asc_float2bfloat16_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3607 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3607(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3607, asc_float2bfloat16_rn_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3607));

    asc_float2bfloat16_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3608 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3608(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3608, asc_float2bfloat16_rn_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3608));

    asc_float2bfloat16_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3609 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3609(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3609, asc_float2bfloat16_rna_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3609));

    asc_float2bfloat16_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3610 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3610(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3610, asc_float2bfloat16_rna_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3610));

    asc_float2bfloat16_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3611 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3611(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3611, asc_float2bfloat16_rna_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3611));

    asc_float2bfloat16_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3612 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3612(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3612, asc_float2bfloat16_rna_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3612));

    asc_float2bfloat16_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3613 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3613(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3613, asc_float2bfloat16_ru_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3613));

    asc_float2bfloat16_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3614 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3614(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3614, asc_float2bfloat16_ru_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3614));

    asc_float2bfloat16_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3615 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3615(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3615, asc_float2bfloat16_ru_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3615));

    asc_float2bfloat16_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3616 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3616(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3616, asc_float2bfloat16_ru_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3616));

    asc_float2bfloat16_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3617 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3617(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3617, asc_float2bfloat16_rz_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3617));

    asc_float2bfloat16_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3618 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3618(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3618, asc_float2bfloat16_rz_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3618));

    asc_float2bfloat16_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3619 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3619(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3619, asc_float2bfloat16_rz_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3619));

    asc_float2bfloat16_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3620 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_PosStub_3620(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos3620, asc_float2bfloat16_rz_sat_ASC_POSITION_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_PosStub_3620));

    asc_float2bfloat16_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
