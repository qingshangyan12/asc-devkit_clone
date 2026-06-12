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

// ==========asc_half2int32==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rd, asc_half2int32_rd, vcvt, vector_int32_t, vector_half, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rd_v2, asc_half2int32_rd_v2, vcvt, vector_int32_t, vector_half, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rn, asc_half2int32_rn, vcvt, vector_int32_t, vector_half, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rn_v2, asc_half2int32_rn_v2, vcvt, vector_int32_t, vector_half, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rna, asc_half2int32_rna, vcvt, vector_int32_t, vector_half, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rna_v2, asc_half2int32_rna_v2, vcvt, vector_int32_t, vector_half, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_ru, asc_half2int32_ru, vcvt, vector_int32_t, vector_half, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_ru_v2, asc_half2int32_ru_v2, vcvt, vector_int32_t, vector_half, 7);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rz, asc_half2int32_rz, vcvt, vector_int32_t, vector_half, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_rz_v2, asc_half2int32_rz_v2, vcvt, vector_int32_t, vector_half, 9);

// ==========asc_int322uint16==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt, asc_int322uint16, vcvt, vector_uint16_t, vector_int32_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat, asc_int322uint16_sat, vcvt, vector_uint16_t, vector_int32_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_v2, asc_int322uint16_v2, vcvt, vector_uint16_t, vector_int32_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat_v2, asc_int322uint16_sat_v2, vcvt, vector_uint16_t, vector_int32_t, 3);

// ==========asc_int642int32==========
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt, asc_int642int32, vcvt, vector_int32_t, vector_int64_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat, asc_int642int32_sat, vcvt, vector_int32_t, vector_int64_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_v2, asc_int642int32_v2, vcvt, vector_int32_t, vector_int64_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_4(Vcvt_sat_v2, asc_int642int32_sat_v2, vcvt, vector_int32_t, vector_int64_t, 3);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos3501 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3501(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3501, asc_half2int32_rd_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3501));

    asc_half2int32_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3502 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3502(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3502, asc_half2int32_rd_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3502));

    asc_half2int32_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3503 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3503(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3503, asc_half2int32_rn_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3503));

    asc_half2int32_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3504 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3504(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3504, asc_half2int32_rn_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3504));

    asc_half2int32_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3505 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3505(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3505, asc_half2int32_rna_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3505));

    asc_half2int32_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3506 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3506(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3506, asc_half2int32_rna_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3506));

    asc_half2int32_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3507 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3507(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3507, asc_half2int32_ru_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3507));

    asc_half2int32_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3508 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3508(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3508, asc_half2int32_ru_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3508));

    asc_half2int32_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3509 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3509(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3509, asc_half2int32_rz_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3509));

    asc_half2int32_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3510 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_PosStub_3510(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3510, asc_half2int32_rz_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_PosStub_3510));

    asc_half2int32_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3511 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_PosStub_3511(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3511, asc_int322uint16_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_PosStub_3511));

    asc_int322uint16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3512 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_PosStub_3512(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3512, asc_int322uint16_sat_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_PosStub_3512));

    asc_int322uint16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3513 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_PosStub_3513(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3513, asc_int322uint16_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_PosStub_3513));

    asc_int322uint16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3514 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_PosStub_3514(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3514, asc_int322uint16_sat_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_PosStub_3514));

    asc_int322uint16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3515 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_PosStub_3515(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3515, asc_int642int32_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_PosStub_3515));

    asc_int642int32(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3516 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_PosStub_3516(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3516, asc_int642int32_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_PosStub_3516));

    asc_int642int32_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos3517 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_PosStub_3517(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3517, asc_int642int32_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_PosStub_3517));

    asc_int642int32(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos3518 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_PosStub_3518(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos3518, asc_int642int32_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_PosStub_3518));

    asc_int642int32_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
