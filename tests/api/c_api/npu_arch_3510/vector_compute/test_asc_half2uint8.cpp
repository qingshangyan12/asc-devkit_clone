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

//================asc_half2uint8_rd================
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd, asc_half2uint8_rd, vcvt, vector_uint8_t, vector_half, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_sat, asc_half2uint8_rd_sat, vcvt, vector_uint8_t, vector_half, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_v2, asc_half2uint8_rd_v2, vcvt, vector_uint8_t, vector_half, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rd_sat_v2, asc_half2uint8_rd_sat_v2, vcvt, vector_uint8_t, vector_half, 3);

//================asc_half2uint8_rn================
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn, asc_half2uint8_rn, vcvt, vector_uint8_t, vector_half, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat, asc_half2uint8_rn_sat, vcvt, vector_uint8_t, vector_half, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_v2, asc_half2uint8_rn_v2, vcvt, vector_uint8_t, vector_half, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rn_sat_v2, asc_half2uint8_rn_sat_v2, vcvt, vector_uint8_t, vector_half, 7);

//================asc_half2uint8_rna================
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna, asc_half2uint8_rna, vcvt, vector_uint8_t, vector_half, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat, asc_half2uint8_rna_sat, vcvt, vector_uint8_t, vector_half, 9);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_v2, asc_half2uint8_rna_v2, vcvt, vector_uint8_t, vector_half, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rna_sat_v2, asc_half2uint8_rna_sat_v2, vcvt, vector_uint8_t, vector_half, 11);

//================asc_half2uint8_ru================
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru, asc_half2uint8_ru, vcvt, vector_uint8_t, vector_half, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_sat, asc_half2uint8_ru_sat, vcvt, vector_uint8_t, vector_half, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_v2, asc_half2uint8_ru_v2, vcvt, vector_uint8_t, vector_half, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_ru_sat_v2, asc_half2uint8_ru_sat_v2, vcvt, vector_uint8_t, vector_half, 15);

//================asc_half2uint8_rn================
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz, asc_half2uint8_rz, vcvt, vector_uint8_t, vector_half, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_sat, asc_half2uint8_rz_sat, vcvt, vector_uint8_t, vector_half, 17);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_v2, asc_half2uint8_rz_v2, vcvt, vector_uint8_t, vector_half, 18);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Vcvt_rz_sat_v2, asc_half2uint8_rz_sat_v2, vcvt, vector_uint8_t, vector_half, 19);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos1401 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1401(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1401, asc_half2uint8_rd_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1401));

    asc_half2uint8_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1402 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1402(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1402, asc_half2uint8_rd_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1402));

    asc_half2uint8_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1403 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1403(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1403, asc_half2uint8_rd_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1403));

    asc_half2uint8_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1404 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1404(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1404, asc_half2uint8_rd_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1404));

    asc_half2uint8_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1405 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1405(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1405, asc_half2uint8_rn_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1405));

    asc_half2uint8_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1406 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1406(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1406, asc_half2uint8_rn_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1406));

    asc_half2uint8_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1407 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1407(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1407, asc_half2uint8_rn_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1407));

    asc_half2uint8_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1408 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1408(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1408, asc_half2uint8_rn_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1408));

    asc_half2uint8_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1409 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1409(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1409, asc_half2uint8_ru_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1409));

    asc_half2uint8_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1410 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1410(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1410, asc_half2uint8_ru_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1410));

    asc_half2uint8_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1411 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1411(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1411, asc_half2uint8_ru_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1411));

    asc_half2uint8_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1412 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1412(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1412, asc_half2uint8_ru_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1412));

    asc_half2uint8_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1413 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1413(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1413, asc_half2uint8_rz_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1413));

    asc_half2uint8_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1414 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1414(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1414, asc_half2uint8_rz_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1414));

    asc_half2uint8_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos1415 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1415(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1415, asc_half2uint8_rz_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1415));

    asc_half2uint8_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos1416 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_PosStub_1416(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos1416, asc_half2uint8_rz_sat_ASC_POSITION_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_PosStub_1416));

    asc_half2uint8_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
