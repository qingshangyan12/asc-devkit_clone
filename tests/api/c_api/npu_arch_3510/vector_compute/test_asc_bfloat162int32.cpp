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

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rn, asc_bfloat162int32_rn, vcvt, vector_int32_t, vector_bfloat16_t, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rn_sat, asc_bfloat162int32_rn_sat, vcvt, vector_int32_t, vector_bfloat16_t, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rn_v2, asc_bfloat162int32_rn_v2, vcvt, vector_int32_t, vector_bfloat16_t, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rn_sat_v2, asc_bfloat162int32_rn_sat_v2, vcvt, vector_int32_t, vector_bfloat16_t, 3);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rna, asc_bfloat162int32_rna, vcvt, vector_int32_t, vector_bfloat16_t, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rna_sat, asc_bfloat162int32_rna_sat, vcvt, vector_int32_t, vector_bfloat16_t, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rna_v2, asc_bfloat162int32_rna_v2, vcvt, vector_int32_t, vector_bfloat16_t, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rna_sat_v2, asc_bfloat162int32_rna_sat_v2, vcvt, vector_int32_t, vector_bfloat16_t, 7);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rd, asc_bfloat162int32_rd, vcvt, vector_int32_t, vector_bfloat16_t, 8);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rd_sat, asc_bfloat162int32_rd_sat, vcvt, vector_int32_t, vector_bfloat16_t, 9);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rd_v2, asc_bfloat162int32_rd_v2, vcvt, vector_int32_t, vector_bfloat16_t, 10);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rd_sat_v2, asc_bfloat162int32_rd_sat_v2, vcvt, vector_int32_t, vector_bfloat16_t, 11);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_ru, asc_bfloat162int32_ru, vcvt, vector_int32_t, vector_bfloat16_t, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_ru_sat, asc_bfloat162int32_ru_sat, vcvt, vector_int32_t, vector_bfloat16_t, 13);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_ru_v2, asc_bfloat162int32_ru_v2, vcvt, vector_int32_t, vector_bfloat16_t, 14);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_ru_sat_v2, asc_bfloat162int32_ru_sat_v2, vcvt, vector_int32_t, vector_bfloat16_t, 15);

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rz, asc_bfloat162int32_rz, vcvt, vector_int32_t, vector_bfloat16_t, 16);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rz_sat, asc_bfloat162int32_rz_sat, vcvt, vector_int32_t, vector_bfloat16_t, 17);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rz_v2, asc_bfloat162int32_rz_v2, vcvt, vector_int32_t, vector_bfloat16_t, 18);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(Bfloat162int32_rz_sat_v2, asc_bfloat162int32_rz_sat_v2, vcvt, vector_int32_t, vector_bfloat16_t, 19);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos101 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_101(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos101, asc_bfloat162int32_rd_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_101));

    asc_bfloat162int32_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos102 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_102(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos102, asc_bfloat162int32_rd_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_102));

    asc_bfloat162int32_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos103 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_103(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos103, asc_bfloat162int32_rd_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_103));

    asc_bfloat162int32_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos104 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_104(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos104, asc_bfloat162int32_rd_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_104));

    asc_bfloat162int32_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos105 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_105(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos105, asc_bfloat162int32_rn_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_105));

    asc_bfloat162int32_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos106 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_106(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos106, asc_bfloat162int32_rn_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_106));

    asc_bfloat162int32_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos107 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_107(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos107, asc_bfloat162int32_rn_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_107));

    asc_bfloat162int32_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos108 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_108(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos108, asc_bfloat162int32_rn_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_108));

    asc_bfloat162int32_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos109 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_109(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos109, asc_bfloat162int32_ru_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_109));

    asc_bfloat162int32_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos110 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_110(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos110, asc_bfloat162int32_ru_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_110));

    asc_bfloat162int32_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos111 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_111(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos111, asc_bfloat162int32_ru_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_111));

    asc_bfloat162int32_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos112 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_112(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos112, asc_bfloat162int32_ru_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_112));

    asc_bfloat162int32_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos113 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_113(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos113, asc_bfloat162int32_rz_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_113));

    asc_bfloat162int32_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos114 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_114(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos114, asc_bfloat162int32_rz_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_114));

    asc_bfloat162int32_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos115 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_115(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos115, asc_bfloat162int32_rz_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_115));

    asc_bfloat162int32_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos116 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_PosStub_116(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos116, asc_bfloat162int32_rz_sat_ASC_POSITION_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_PosStub_116));

    asc_bfloat162int32_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
