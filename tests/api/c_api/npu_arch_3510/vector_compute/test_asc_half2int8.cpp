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

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd, asc_half2int8_rd, vcvt, vector_int8_t, vector_half, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_sat, asc_half2int8_rd_sat, vcvt, vector_int8_t, vector_half, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_v2, asc_half2int8_rd_v2, vcvt, vector_int8_t, vector_half, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_sat_v2, asc_half2int8_rd_sat_v2, vcvt, vector_int8_t, vector_half, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru, asc_half2int8_ru, vcvt, vector_int8_t, vector_half, 19);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_sat, asc_half2int8_ru_sat, vcvt, vector_int8_t, vector_half, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_v2, asc_half2int8_ru_v2, vcvt, vector_int8_t, vector_half, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_sat_v2, asc_half2int8_ru_sat_v2, vcvt, vector_int8_t, vector_half, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz, asc_half2int8_rz, vcvt, vector_int8_t, vector_half, 7);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_sat, asc_half2int8_rz_sat, vcvt, vector_int8_t, vector_half, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_v2, asc_half2int8_rz_v2, vcvt, vector_int8_t, vector_half, 9);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_sat_v2, asc_half2int8_rz_sat_v2, vcvt, vector_int8_t, vector_half, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn, asc_half2int8_rn, vcvt, vector_int8_t, vector_half, 11);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat, asc_half2int8_rn_sat, vcvt, vector_int8_t, vector_half, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_v2, asc_half2int8_rn_v2, vcvt, vector_int8_t, vector_half, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat_v2, asc_half2int8_rn_sat_v2, vcvt, vector_int8_t, vector_half, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna, asc_half2int8_rna, vcvt, vector_int8_t, vector_half, 15);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat, asc_half2int8_rna_sat, vcvt, vector_int8_t, vector_half, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_v2, asc_half2int8_rna_v2, vcvt, vector_int8_t, vector_half, 17);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat_v2, asc_half2int8_rna_sat_v2, vcvt, vector_int8_t, vector_half, 18);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos1301 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1301(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1301, asc_half2int8_rd_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1301));

    asc_half2int8_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1302 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1302(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1302, asc_half2int8_rd_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1302));

    asc_half2int8_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1303 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1303(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1303, asc_half2int8_rd_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1303));

    asc_half2int8_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1304 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1304(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1304, asc_half2int8_rd_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1304));

    asc_half2int8_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1305 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1305(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1305, asc_half2int8_ru_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1305));

    asc_half2int8_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1306 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1306(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1306, asc_half2int8_ru_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1306));

    asc_half2int8_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1307 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1307(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1307, asc_half2int8_ru_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1307));

    asc_half2int8_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1308 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1308(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1308, asc_half2int8_ru_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1308));

    asc_half2int8_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1309 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1309(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1309, asc_half2int8_rz_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1309));

    asc_half2int8_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1310 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1310(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1310, asc_half2int8_rz_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1310));

    asc_half2int8_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1311 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1311(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1311, asc_half2int8_rz_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1311));

    asc_half2int8_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1312 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1312(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1312, asc_half2int8_rz_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1312));

    asc_half2int8_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1313 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1313(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1313, asc_half2int8_rn_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1313));

    asc_half2int8_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1314 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1314(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1314, asc_half2int8_rn_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1314));

    asc_half2int8_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1315 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1315(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1315, asc_half2int8_rn_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1315));

    asc_half2int8_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1316 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_PosStub_1316(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1316, asc_half2int8_rn_sat_ASC_POSITION_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_PosStub_1316));

    asc_half2int8_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
