/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "tests/api/c_api/npu_arch_3510/utils/test_vcvt_instr_utils.h"                                                                                              \

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd, asc_float2int16_rd, vcvt, vector_int16_t, vector_float, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd1, asc_float2int16_rd_sat, vcvt, vector_int16_t, vector_float, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd2, asc_float2int16_rd_v2, vcvt, vector_int16_t, vector_float, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd3, asc_float2int16_rd_sat_v2, vcvt, vector_int16_t, vector_float, 3);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn, asc_float2int16_rn, vcvt, vector_int16_t, vector_float, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn1, asc_float2int16_rn_sat, vcvt, vector_int16_t, vector_float, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn2, asc_float2int16_rn_v2, vcvt, vector_int16_t, vector_float, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn3, asc_float2int16_rn_sat_v2, vcvt, vector_int16_t, vector_float, 7);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna, asc_float2int16_rna, vcvt, vector_int16_t, vector_float, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna1, asc_float2int16_rna_sat, vcvt, vector_int16_t, vector_float, 9);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna2, asc_float2int16_rna_v2, vcvt, vector_int16_t, vector_float, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna3, asc_float2int16_rna_sat_v2, vcvt, vector_int16_t, vector_float, 11);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru, asc_float2int16_ru, vcvt, vector_int16_t, vector_float, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru1, asc_float2int16_ru_sat, vcvt, vector_int16_t, vector_float, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru2, asc_float2int16_ru_v2, vcvt, vector_int16_t, vector_float, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru3, asc_float2int16_ru_sat_v2, vcvt, vector_int16_t, vector_float, 15);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz, asc_float2int16_rz, vcvt, vector_int16_t, vector_float, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz1, asc_float2int16_rz_sat, vcvt, vector_int16_t, vector_float, 17);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz2, asc_float2int16_rz_v2, vcvt, vector_int16_t, vector_float, 18);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz3, asc_float2int16_rz_sat_v2, vcvt, vector_int16_t, vector_float, 19);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos801 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_801(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos801, asc_float2int16_rd_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_801));

    asc_float2int16_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos802 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_802(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos802, asc_float2int16_rd_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_802));

    asc_float2int16_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos803 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_803(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos803, asc_float2int16_rd_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_803));

    asc_float2int16_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos804 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_804(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos804, asc_float2int16_rd_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_804));

    asc_float2int16_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos805 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_805(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos805, asc_float2int16_rn_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_805));

    asc_float2int16_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos806 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_806(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos806, asc_float2int16_rn_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_806));

    asc_float2int16_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos807 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_807(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos807, asc_float2int16_rn_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_807));

    asc_float2int16_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos808 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_808(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos808, asc_float2int16_rn_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_808));

    asc_float2int16_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos809 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_809(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos809, asc_float2int16_ru_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_809));

    asc_float2int16_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos810 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_810(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos810, asc_float2int16_ru_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_810));

    asc_float2int16_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos811 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_811(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos811, asc_float2int16_ru_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_811));

    asc_float2int16_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos812 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_812(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos812, asc_float2int16_ru_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_812));

    asc_float2int16_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos813 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_813(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos813, asc_float2int16_rz_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_813));

    asc_float2int16_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos814 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_814(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos814, asc_float2int16_rz_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_814));

    asc_float2int16_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos815 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_815(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos815, asc_float2int16_rz_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_815));

    asc_float2int16_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos816 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_PosStub_816(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos816, asc_float2int16_rz_sat_ASC_POSITION_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_PosStub_816));

    asc_float2int16_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
