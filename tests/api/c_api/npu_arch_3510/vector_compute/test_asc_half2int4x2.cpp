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

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd, asc_half2int4x2_rd, vcvt_f162s4, vector_int4x2_t, vector_half, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_sat, asc_half2int4x2_rd_sat, vcvt_f162s4, vector_int4x2_t, vector_half, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_v2, asc_half2int4x2_rd_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_sat_v2, asc_half2int4x2_rd_sat_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_v3, asc_half2int4x2_rd_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_sat_v3, asc_half2int4x2_rd_sat_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_v4, asc_half2int4x2_rd_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_sat_v4, asc_half2int4x2_rd_sat_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 7);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz, asc_half2int4x2_rz, vcvt_f162s4, vector_int4x2_t, vector_half, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_sat, asc_half2int4x2_rz_sat, vcvt_f162s4, vector_int4x2_t, vector_half, 11);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_v2, asc_half2int4x2_rz_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_sat_v2, asc_half2int4x2_rz_sat_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_v3, asc_half2int4x2_rz_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_sat_v3, asc_half2int4x2_rz_sat_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 15);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_v4, asc_half2int4x2_rz_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rz_sat_v4, asc_half2int4x2_rz_sat_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 17);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn, asc_half2int4x2_rn, vcvt_f162s4, vector_int4x2_t, vector_half, 20);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_sat, asc_half2int4x2_rn_sat, vcvt_f162s4, vector_int4x2_t, vector_half, 21);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_v2, asc_half2int4x2_rn_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 22);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_sat_v2, asc_half2int4x2_rn_sat_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 23);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_v3, asc_half2int4x2_rn_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 24);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_sat_v3, asc_half2int4x2_rn_sat_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 25);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_v4, asc_half2int4x2_rn_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 26);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rn_sat_v4, asc_half2int4x2_rn_sat_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 27);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna, asc_half2int4x2_rna, vcvt_f162s4, vector_int4x2_t, vector_half, 30);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_sat, asc_half2int4x2_rna_sat, vcvt_f162s4, vector_int4x2_t, vector_half, 31);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_v2, asc_half2int4x2_rna_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 32);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_sat_v2, asc_half2int4x2_rna_sat_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 33);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_v3, asc_half2int4x2_rna_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 34);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_sat_v3, asc_half2int4x2_rna_sat_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 35);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_v4, asc_half2int4x2_rna_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 36);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rna_sat_v4, asc_half2int4x2_rna_sat_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 37);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru, asc_half2int4x2_ru, vcvt_f162s4, vector_int4x2_t, vector_half, 40);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_sat, asc_half2int4x2_ru_sat, vcvt_f162s4, vector_int4x2_t, vector_half, 41);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_v2, asc_half2int4x2_ru_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 42);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_sat_v2, asc_half2int4x2_ru_sat_v2, vcvt_f162s4, vector_int4x2_t, vector_half, 43);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_v3, asc_half2int4x2_ru_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 44);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_sat_v3, asc_half2int4x2_ru_sat_v3, vcvt_f162s4, vector_int4x2_t, vector_half, 45);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_v4, asc_half2int4x2_ru_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 46);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_ru_sat_v4, asc_half2int4x2_ru_sat_v4, vcvt_f162s4, vector_int4x2_t, vector_half, 47);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos1201 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1201(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1201, asc_half2int4x2_rd_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1201));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1202 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1202(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1202, asc_half2int4x2_rd_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1202));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1203 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1203(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1203, asc_half2int4x2_rd_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1203));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1204 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1204(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1204, asc_half2int4x2_rd_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1204));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1205 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1205(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1205, asc_half2int4x2_rd_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1205));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1206 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1206(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1206, asc_half2int4x2_rd_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1206));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1207 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1207(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1207, asc_half2int4x2_rd_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1207));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1208 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1208(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1208, asc_half2int4x2_rd_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1208));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1209 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1209(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1209, asc_half2int4x2_rn_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1209));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1210 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1210(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1210, asc_half2int4x2_rn_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1210));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1211 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1211(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1211, asc_half2int4x2_rn_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1211));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1212 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1212(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1212, asc_half2int4x2_rn_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1212));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1213 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1213(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1213, asc_half2int4x2_rn_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1213));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1214 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1214(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1214, asc_half2int4x2_rn_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1214));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1215 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1215(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1215, asc_half2int4x2_rn_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1215));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1216 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1216(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1216, asc_half2int4x2_rn_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1216));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1217 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1217(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1217, asc_half2int4x2_ru_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1217));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1218 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1218(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1218, asc_half2int4x2_ru_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1218));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1219 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1219(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1219, asc_half2int4x2_ru_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1219));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1220 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1220(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1220, asc_half2int4x2_ru_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1220));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1221 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1221(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1221, asc_half2int4x2_ru_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1221));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1222 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1222(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1222, asc_half2int4x2_ru_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1222));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1223 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1223(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1223, asc_half2int4x2_ru_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1223));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1224 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1224(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1224, asc_half2int4x2_ru_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1224));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1225 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1225(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1225, asc_half2int4x2_rz_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1225));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1226 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1226(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1226, asc_half2int4x2_rz_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1226));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1227 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1227(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1227, asc_half2int4x2_rz_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1227));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1228 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1228(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1228, asc_half2int4x2_rz_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1228));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1229 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1229(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1229, asc_half2int4x2_rz_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1229));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1230 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1230(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1230, asc_half2int4x2_rz_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1230));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1231 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1231(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1231, asc_half2int4x2_rz_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1231));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos1232 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_PosStub_1232(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1232, asc_half2int4x2_rz_sat_ASC_DISPERSE_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_PosStub_1232));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
