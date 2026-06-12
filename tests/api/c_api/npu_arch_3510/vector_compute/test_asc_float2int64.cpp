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

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd, asc_float2int64_rd, vcvt, vector_int64_t, vector_float, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd1, asc_float2int64_rd_sat, vcvt, vector_int64_t, vector_float, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd2, asc_float2int64_rd_v2, vcvt, vector_int64_t, vector_float, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd3, asc_float2int64_rd_sat_v2, vcvt, vector_int64_t, vector_float, 3);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn, asc_float2int64_rn, vcvt, vector_int64_t, vector_float, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn1, asc_float2int64_rn_sat, vcvt, vector_int64_t, vector_float, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn2, asc_float2int64_rn_v2, vcvt, vector_int64_t, vector_float, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn3, asc_float2int64_rn_sat_v2, vcvt, vector_int64_t, vector_float, 7);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna, asc_float2int64_rna, vcvt, vector_int64_t, vector_float, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna1, asc_float2int64_rna_sat, vcvt, vector_int64_t, vector_float, 9);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna2, asc_float2int64_rna_v2, vcvt, vector_int64_t, vector_float, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna3, asc_float2int64_rna_sat_v2, vcvt, vector_int64_t, vector_float, 11);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru, asc_float2int64_ru, vcvt, vector_int64_t, vector_float, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru1, asc_float2int64_ru_sat, vcvt, vector_int64_t, vector_float, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru2, asc_float2int64_ru_v2, vcvt, vector_int64_t, vector_float, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru3, asc_float2int64_ru_sat_v2, vcvt, vector_int64_t, vector_float, 15);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz, asc_float2int64_rz, vcvt, vector_int64_t, vector_float, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz1, asc_float2int64_rz_sat, vcvt, vector_int64_t, vector_float, 17);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz2, asc_float2int64_rz_v2, vcvt, vector_int64_t, vector_float, 18);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz3, asc_float2int64_rz_sat_v2, vcvt, vector_int64_t, vector_float, 19);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos901 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_901(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos901, asc_float2int64_rd_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_901));

    asc_float2int64_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos902 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_902(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos902, asc_float2int64_rd_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_902));

    asc_float2int64_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos903 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_903(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos903, asc_float2int64_rd_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_903));

    asc_float2int64_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos904 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_904(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos904, asc_float2int64_rd_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_904));

    asc_float2int64_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos905 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_905(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos905, asc_float2int64_rn_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_905));

    asc_float2int64_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos906 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_906(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos906, asc_float2int64_rn_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_906));

    asc_float2int64_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos907 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_907(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos907, asc_float2int64_rn_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_907));

    asc_float2int64_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos908 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_908(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos908, asc_float2int64_rn_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_908));

    asc_float2int64_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos909 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_909(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos909, asc_float2int64_ru_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_909));

    asc_float2int64_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos910 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_910(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos910, asc_float2int64_ru_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_910));

    asc_float2int64_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos911 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_911(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos911, asc_float2int64_ru_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_911));

    asc_float2int64_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos912 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_912(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos912, asc_float2int64_ru_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_912));

    asc_float2int64_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos913 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_913(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos913, asc_float2int64_rz_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_913));

    asc_float2int64_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos914 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_914(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos914, asc_float2int64_rz_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_914));

    asc_float2int64_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos915 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_915(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos915, asc_float2int64_rz_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_915));

    asc_float2int64_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos916 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_PosStub_916(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos916, asc_float2int64_rz_sat_ASC_POSITION_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_PosStub_916));

    asc_float2int64_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
