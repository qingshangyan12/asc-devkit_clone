/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* for full license see repository root LICENSE file.
*/

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

class TestVcvtPos_1 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_1(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_1, asc_bfloat162e1m2x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_1));

    asc_bfloat162e1m2x2_rd(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_2 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_2(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_2, asc_bfloat162e1m2x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_2));

    asc_bfloat162e1m2x2_rd(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_3 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_3(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_3, asc_bfloat162e1m2x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_3));

    asc_bfloat162e1m2x2_rd(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_4 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_4(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_4, asc_bfloat162e1m2x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_4));

    asc_bfloat162e1m2x2_rd(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_5 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_5(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_5, asc_bfloat162e1m2x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_5));

    asc_bfloat162e1m2x2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_6 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_6(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_6, asc_bfloat162e1m2x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_6));

    asc_bfloat162e1m2x2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_7 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_7(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_7, asc_bfloat162e1m2x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_7));

    asc_bfloat162e1m2x2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_8 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_8(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_8, asc_bfloat162e1m2x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_8));

    asc_bfloat162e1m2x2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_9 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_9(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_9, asc_bfloat162e1m2x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_9));

    asc_bfloat162e1m2x2_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_10 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_10(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_10, asc_bfloat162e1m2x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_10));

    asc_bfloat162e1m2x2_rna(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_11 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_11(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_11, asc_bfloat162e1m2x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_11));

    asc_bfloat162e1m2x2_rna(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_12 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_12(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_12, asc_bfloat162e1m2x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_12));

    asc_bfloat162e1m2x2_rna(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_13 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_13(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_13, asc_bfloat162e1m2x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_13));

    asc_bfloat162e1m2x2_ru(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_14 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_14(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_14, asc_bfloat162e1m2x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_14));

    asc_bfloat162e1m2x2_ru(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_15 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_15(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_15, asc_bfloat162e1m2x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_15));

    asc_bfloat162e1m2x2_ru(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_16 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_16(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_16, asc_bfloat162e1m2x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_16));

    asc_bfloat162e1m2x2_ru(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_17 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_17(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_17, asc_bfloat162e1m2x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_17));

    asc_bfloat162e1m2x2_rz(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_18 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_18(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_18, asc_bfloat162e1m2x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_18));

    asc_bfloat162e1m2x2_rz(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_19 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_19(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_19, asc_bfloat162e1m2x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_19));

    asc_bfloat162e1m2x2_rz(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_20 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_20(vector_fp4x2_e1m2_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_20, asc_bfloat162e1m2x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e1m2_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e1m2x2_x, void(vector_fp4x2_e1m2_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e1m2x2_x_Stub_20));

    asc_bfloat162e1m2x2_rz(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_21 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_21(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_21, asc_bfloat162e2m1x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_21));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_22 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_22(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_22, asc_bfloat162e2m1x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_22));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_23 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_23(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_23, asc_bfloat162e2m1x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_23));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_24 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_24(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_24, asc_bfloat162e2m1x2_rd_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_24));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_25 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_25(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_25, asc_bfloat162e2m1x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_25));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_26 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_26(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_26, asc_bfloat162e2m1x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_26));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_27 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_27(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_27, asc_bfloat162e2m1x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_27));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_28 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_28(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_28, asc_bfloat162e2m1x2_rn_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_28));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_29 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_29(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_29, asc_bfloat162e2m1x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_29));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_30 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_30(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_30, asc_bfloat162e2m1x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_30));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_31 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_31(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_31, asc_bfloat162e2m1x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_31));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_32 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_32(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_32, asc_bfloat162e2m1x2_rna_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_32));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_33 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_33(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_33, asc_bfloat162e2m1x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_33));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_34 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_34(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_34, asc_bfloat162e2m1x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_34));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_35 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_35(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_35, asc_bfloat162e2m1x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_35));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_36 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_36(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_36, asc_bfloat162e2m1x2_ru_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_36));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_37 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_37(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_37, asc_bfloat162e2m1x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_37));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_38 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_38(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_38, asc_bfloat162e2m1x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_38));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_39 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_39(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_39, asc_bfloat162e2m1x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_39));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_40 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_40(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_40, asc_bfloat162e2m1x2_rz_ASC_DISP_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_Stub_40));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_41 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f32_x_Stub_41(vector_float dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_41, asc_bfloat162float_ASC_POSI_Succ) {
    vector_float dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f32_x, void(vector_float, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f32_x_Stub_41));

    asc_bfloat162float(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_42 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f32_x_Stub_42(vector_float dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_42, asc_bfloat162float_ASC_POSI_Succ) {
    vector_float dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f32_x, void(vector_float, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f32_x_Stub_42));

    asc_bfloat162float(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_43 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_43(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_43, asc_bfloat162int32_rd_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_43));

    asc_bfloat162int32_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_44 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_44(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_44, asc_bfloat162int32_rd_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_44));

    asc_bfloat162int32_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_45 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_45(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_45, asc_bfloat162int32_rd_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_45));

    asc_bfloat162int32_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_46 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_46(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_46, asc_bfloat162int32_rd_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_46));

    asc_bfloat162int32_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_47 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_47(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_47, asc_bfloat162int32_rn_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_47));

    asc_bfloat162int32_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_48 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_48(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_48, asc_bfloat162int32_rn_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_48));

    asc_bfloat162int32_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_49 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_49(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_49, asc_bfloat162int32_rn_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_49));

    asc_bfloat162int32_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_50 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_50(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_50, asc_bfloat162int32_rn_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_50));

    asc_bfloat162int32_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_51 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_51(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_51, asc_bfloat162int32_rna_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_51));

    asc_bfloat162int32_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_52 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_52(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_52, asc_bfloat162int32_rna_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_52));

    asc_bfloat162int32_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_53 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_53(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_53, asc_bfloat162int32_rna_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_53));

    asc_bfloat162int32_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_54 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_54(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_54, asc_bfloat162int32_rna_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_54));

    asc_bfloat162int32_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_55 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_55(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_55, asc_bfloat162int32_ru_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_55));

    asc_bfloat162int32_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_56 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_56(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_56, asc_bfloat162int32_ru_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_56));

    asc_bfloat162int32_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_57 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_57(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_57, asc_bfloat162int32_ru_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_57));

    asc_bfloat162int32_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_58 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_58(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_58, asc_bfloat162int32_ru_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_58));

    asc_bfloat162int32_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_59 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_59(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_59, asc_bfloat162int32_rz_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_59));

    asc_bfloat162int32_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_60 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_60(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_60, asc_bfloat162int32_rz_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_60));

    asc_bfloat162int32_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_61 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_61(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_61, asc_bfloat162int32_rz_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_61));

    asc_bfloat162int32_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_62 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_bf162s32_x_Stub_62(vector_int32_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_62, asc_bfloat162int32_rz_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_bf162s32_x, void(vector_int32_t, vector_bfloat16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_bf162s32_x_Stub_62));

    asc_bfloat162int32_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_63 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_63(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_63, asc_e1m2x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_63));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_64 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_64(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_64, asc_e1m2x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_64));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_65 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_65(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_65, asc_e1m2x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_65));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_66 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_66(vector_bfloat16_t dst, vector_fp4x2_e1m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_66, asc_e1m2x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e1m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e1m2x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e1m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e1m2x22bf16_x_Stub_66));

    asc_e1m2x22bfloat16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_67 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_67(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_67, asc_e2m1x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_67));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_68 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_68(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_68, asc_e2m1x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_68));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_69 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_69(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_69, asc_e2m1x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_69));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_70 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_70(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_70, asc_e2m1x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_Stub_70));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_71 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_Stub_71(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_71, asc_e4m32float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_Stub_71));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_72 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_Stub_72(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_72, asc_e4m32float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_Stub_72));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_73 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_Stub_73(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_73, asc_e4m32float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_Stub_73));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_74 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e4m32f32_x_Stub_74(vector_float dst, vector_fp8_e4m3fn_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_74, asc_e4m32float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e4m3fn_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e4m32f32_x, void(vector_float, vector_fp8_e4m3fn_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e4m32f32_x_Stub_74));

    asc_e4m32float(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_75 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_Stub_75(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_75, asc_e5m22float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_Stub_75));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_76 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_Stub_76(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_76, asc_e5m22float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_Stub_76));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_77 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_Stub_77(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_77, asc_e5m22float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_Stub_77));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_78 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f8e5m22f32_x_Stub_78(vector_float dst, vector_fp8_e5m2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_78, asc_e5m22float_ASC_DISP_Succ) {
    vector_float dst;
    vector_fp8_e5m2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f8e5m22f32_x, void(vector_float, vector_fp8_e5m2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f8e5m22f32_x_Stub_78));

    asc_e5m22float(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_79 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_79(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_79, asc_float2bfloat16_rd_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_79));

    asc_float2bfloat16_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_80 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_80(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_80, asc_float2bfloat16_rd_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_80));

    asc_float2bfloat16_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_81 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_81(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_81, asc_float2bfloat16_rd_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_81));

    asc_float2bfloat16_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_82 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_82(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_82, asc_float2bfloat16_rd_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_82));

    asc_float2bfloat16_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_83 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_83(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_83, asc_float2bfloat16_rn_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_83));

    asc_float2bfloat16_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_84 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_84(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_84, asc_float2bfloat16_rn_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_84));

    asc_float2bfloat16_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_85 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_85(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_85, asc_float2bfloat16_rn_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_85));

    asc_float2bfloat16_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_86 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_86(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_86, asc_float2bfloat16_rn_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_86));

    asc_float2bfloat16_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_87 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_87(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_87, asc_float2bfloat16_rna_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_87));

    asc_float2bfloat16_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_88 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_88(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_88, asc_float2bfloat16_rna_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_88));

    asc_float2bfloat16_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_89 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_89(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_89, asc_float2bfloat16_rna_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_89));

    asc_float2bfloat16_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_90 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_90(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_90, asc_float2bfloat16_rna_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_90));

    asc_float2bfloat16_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_91 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_91(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_91, asc_float2bfloat16_ru_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_91));

    asc_float2bfloat16_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_92 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_92(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_92, asc_float2bfloat16_ru_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_92));

    asc_float2bfloat16_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_93 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_93(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_93, asc_float2bfloat16_ru_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_93));

    asc_float2bfloat16_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_94 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_94(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_94, asc_float2bfloat16_ru_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_94));

    asc_float2bfloat16_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_95 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_95(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_95, asc_float2bfloat16_rz_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_95));

    asc_float2bfloat16_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_96 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_96(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_96, asc_float2bfloat16_rz_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_96));

    asc_float2bfloat16_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_97 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_97(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_97, asc_float2bfloat16_rz_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_97));

    asc_float2bfloat16_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_98 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322bf16_x_Stub_98(vector_bfloat16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_98, asc_float2bfloat16_rz_sat_ASC_POSI_Succ) {
    vector_bfloat16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322bf16_x, void(vector_bfloat16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322bf16_x_Stub_98));

    asc_float2bfloat16_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_99 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_99(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_99, asc_float2e4m3_rn_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_99));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_100 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_100(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_100, asc_float2e4m3_rn_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_100));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_101 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_101(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_101, asc_float2e4m3_rn_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_101));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_102 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_102(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_102, asc_float2e4m3_rn_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_102));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_103 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_103(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_103, asc_float2e4m3_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_103));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_104 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_104(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_104, asc_float2e4m3_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_104));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_105 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_105(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_105, asc_float2e4m3_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_105));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_106 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_Stub_106(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_106, asc_float2e4m3_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_Stub_106));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_107 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_107(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_107, asc_float2e5m2_rn_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_107));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_108 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_108(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_108, asc_float2e5m2_rn_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_108));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_109 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_109(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_109, asc_float2e5m2_rn_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_109));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_110 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_110(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_110, asc_float2e5m2_rn_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_110));

    asc_float2e5m2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_111 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_111(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_111, asc_float2e5m2_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_111));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_112 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_112(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_112, asc_float2e5m2_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_112));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_113 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_113(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_113, asc_float2e5m2_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_113));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_114 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e5m2_x_Stub_114(vector_fp8_e5m2_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_114, asc_float2e5m2_rn_sat_ASC_DISP_Succ) {
    vector_fp8_e5m2_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e5m2_x, void(vector_fp8_e5m2_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e5m2_x_Stub_114));

    asc_float2e5m2_rn_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_115 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_115(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_115, asc_float2half_rd_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_115));

    asc_float2half_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_116 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_116(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_116, asc_float2half_rd_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_116));

    asc_float2half_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_117 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_117(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_117, asc_float2half_rd_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_117));

    asc_float2half_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_118 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_118(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_118, asc_float2half_rd_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_118));

    asc_float2half_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_119 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_119(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_119, asc_float2half_rn_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_119));

    asc_float2half_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_120 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_120(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_120, asc_float2half_rn_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_120));

    asc_float2half_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_121 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_121(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_121, asc_float2half_rn_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_121));

    asc_float2half_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_122 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_122(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_122, asc_float2half_rn_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_122));

    asc_float2half_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_123 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_123(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_123, asc_float2half_rna_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_123));

    asc_float2half_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_124 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_124(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_124, asc_float2half_rna_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_124));

    asc_float2half_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_125 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_125(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_125, asc_float2half_rna_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_125));

    asc_float2half_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_126 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_126(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_126, asc_float2half_rna_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_126));

    asc_float2half_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_127 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_127(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_127, asc_float2half_ro_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_127));

    asc_float2half_ro(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_128 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_128(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_128, asc_float2half_ro_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_128));

    asc_float2half_ro(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_129 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_129(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_129, asc_float2half_ro_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_129));

    asc_float2half_ro_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_130 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_130(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_130, asc_float2half_ro_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_130));

    asc_float2half_ro_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_131 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_131(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_131, asc_float2half_ru_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_131));

    asc_float2half_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_132 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_132(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_132, asc_float2half_ru_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_132));

    asc_float2half_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_133 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_133(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_133, asc_float2half_ru_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_133));

    asc_float2half_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_134 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_134(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_134, asc_float2half_ru_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_134));

    asc_float2half_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_135 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_135(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_135, asc_float2half_rz_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_135));

    asc_float2half_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_136 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_136(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_136, asc_float2half_rz_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_136));

    asc_float2half_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_137 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_137(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_137, asc_float2half_rz_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_137));

    asc_float2half_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_138 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f16_x_Stub_138(vector_half dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_138, asc_float2half_rz_sat_ASC_POSI_Succ) {
    vector_half dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f16_x, void(vector_half, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f16_x_Stub_138));

    asc_float2half_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_139 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_139(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_139, asc_float2hif8_rh_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_139));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_140 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_140(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_140, asc_float2hif8_rh_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_140));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_141 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_141(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_141, asc_float2hif8_rh_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_141));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_142 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_142(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_142, asc_float2hif8_rh_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_142));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_143 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_143(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_143, asc_float2hif8_rh_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_143));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_144 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_144(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_144, asc_float2hif8_rh_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_144));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_145 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_145(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_145, asc_float2hif8_rh_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_145));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_146 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_146(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_146, asc_float2hif8_rh_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_146));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_147 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_147(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_147, asc_float2hif8_rna_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_147));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_148 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_148(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_148, asc_float2hif8_rna_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_148));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_149 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_149(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_149, asc_float2hif8_rna_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_149));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_150 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_150(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_150, asc_float2hif8_rna_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_150));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_151 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_151(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_151, asc_float2hif8_rna_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_151));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_152 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_152(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_152, asc_float2hif8_rna_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_152));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_153 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_153(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_153, asc_float2hif8_rna_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_153));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_154 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_Stub_154(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_154, asc_float2hif8_rna_sat_ASC_DISP_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_Stub_154));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_155 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_155(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_155, asc_float2int16_rd_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_155));

    asc_float2int16_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_156 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_156(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_156, asc_float2int16_rd_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_156));

    asc_float2int16_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_157 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_157(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_157, asc_float2int16_rd_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_157));

    asc_float2int16_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_158 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_158(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_158, asc_float2int16_rd_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_158));

    asc_float2int16_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_159 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_159(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_159, asc_float2int16_rn_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_159));

    asc_float2int16_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_160 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_160(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_160, asc_float2int16_rn_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_160));

    asc_float2int16_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_161 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_161(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_161, asc_float2int16_rn_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_161));

    asc_float2int16_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_162 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_162(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_162, asc_float2int16_rn_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_162));

    asc_float2int16_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_163 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_163(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_163, asc_float2int16_rna_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_163));

    asc_float2int16_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_164 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_164(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_164, asc_float2int16_rna_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_164));

    asc_float2int16_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_165 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_165(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_165, asc_float2int16_rna_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_165));

    asc_float2int16_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_166 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_166(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_166, asc_float2int16_rna_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_166));

    asc_float2int16_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_167 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_167(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_167, asc_float2int16_ru_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_167));

    asc_float2int16_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_168 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_168(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_168, asc_float2int16_ru_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_168));

    asc_float2int16_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_169 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_169(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_169, asc_float2int16_ru_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_169));

    asc_float2int16_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_170 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_170(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_170, asc_float2int16_ru_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_170));

    asc_float2int16_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_171 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_171(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_171, asc_float2int16_rz_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_171));

    asc_float2int16_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_172 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_172(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_172, asc_float2int16_rz_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_172));

    asc_float2int16_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_173 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_173(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_173, asc_float2int16_rz_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_173));

    asc_float2int16_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_174 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s16_x_Stub_174(vector_int16_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_174, asc_float2int16_rz_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s16_x, void(vector_int16_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s16_x_Stub_174));

    asc_float2int16_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_175 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_175(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_175, asc_float2int64_rd_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_175));

    asc_float2int64_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_176 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_176(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_176, asc_float2int64_rd_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_176));

    asc_float2int64_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_177 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_177(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_177, asc_float2int64_rd_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_177));

    asc_float2int64_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_178 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_178(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_178, asc_float2int64_rd_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_178));

    asc_float2int64_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_179 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_179(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_179, asc_float2int64_rn_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_179));

    asc_float2int64_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_180 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_180(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_180, asc_float2int64_rn_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_180));

    asc_float2int64_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_181 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_181(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_181, asc_float2int64_rn_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_181));

    asc_float2int64_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_182 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_182(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_182, asc_float2int64_rn_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_182));

    asc_float2int64_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_183 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_183(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_183, asc_float2int64_rna_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_183));

    asc_float2int64_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_184 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_184(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_184, asc_float2int64_rna_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_184));

    asc_float2int64_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_185 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_185(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_185, asc_float2int64_rna_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_185));

    asc_float2int64_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_186 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_186(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_186, asc_float2int64_rna_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_186));

    asc_float2int64_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_187 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_187(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_187, asc_float2int64_ru_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_187));

    asc_float2int64_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_188 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_188(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_188, asc_float2int64_ru_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_188));

    asc_float2int64_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_189 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_189(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_189, asc_float2int64_ru_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_189));

    asc_float2int64_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_190 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_190(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_190, asc_float2int64_ru_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_190));

    asc_float2int64_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_191 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_191(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_191, asc_float2int64_rz_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_191));

    asc_float2int64_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_192 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_192(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_192, asc_float2int64_rz_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_192));

    asc_float2int64_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_193 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_193(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_193, asc_float2int64_rz_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_193));

    asc_float2int64_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_194 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f322s64_x_Stub_194(vector_int64_t dst, vector_float src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_194, asc_float2int64_rz_sat_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f322s64_x, void(vector_int64_t, vector_float, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f322s64_x_Stub_194));

    asc_float2int64_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_195 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162f32_x_Stub_195(vector_float dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_195, asc_half2float_ASC_POSI_Succ) {
    vector_float dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162f32_x, void(vector_float, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162f32_x_Stub_195));

    asc_half2float(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_196 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162f32_x_Stub_196(vector_float dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_196, asc_half2float_ASC_POSI_Succ) {
    vector_float dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162f32_x, void(vector_float, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162f32_x_Stub_196));

    asc_half2float(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_197 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_197(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_197, asc_half2hif8_rh_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_197));

    asc_half2hif8_rh(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_198 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_198(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_198, asc_half2hif8_rh_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_198));

    asc_half2hif8_rh(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_199 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_199(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_199, asc_half2hif8_rh_sat_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_199));

    asc_half2hif8_rh_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_200 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_200(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_200, asc_half2hif8_rh_sat_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_200));

    asc_half2hif8_rh_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_201 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_201(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_201, asc_half2hif8_rna_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_201));

    asc_half2hif8_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_202 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_202(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_202, asc_half2hif8_rna_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_202));

    asc_half2hif8_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_203 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_203(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_203, asc_half2hif8_rna_sat_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_203));

    asc_half2hif8_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_204 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f162hif8_x_Stub_204(vector_hifloat8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_204, asc_half2hif8_rna_sat_ASC_POSI_Succ) {
    vector_hifloat8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f162hif8_x, void(vector_hifloat8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f162hif8_x_Stub_204));

    asc_half2hif8_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_205 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_205(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_205, asc_half2int32_rd_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_205));

    asc_half2int32_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_206 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_206(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_206, asc_half2int32_rd_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_206));

    asc_half2int32_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_207 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_207(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_207, asc_half2int32_rn_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_207));

    asc_half2int32_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_208 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_208(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_208, asc_half2int32_rn_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_208));

    asc_half2int32_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_209 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_209(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_209, asc_half2int32_rna_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_209));

    asc_half2int32_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_210 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_210(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_210, asc_half2int32_rna_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_210));

    asc_half2int32_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_211 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_211(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_211, asc_half2int32_ru_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_211));

    asc_half2int32_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_212 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_212(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_212, asc_half2int32_ru_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_212));

    asc_half2int32_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_213 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_213(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_213, asc_half2int32_rz_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_213));

    asc_half2int32_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_214 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s32_x_Stub_214(vector_int32_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_214, asc_half2int32_rz_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s32_x, void(vector_int32_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s32_x_Stub_214));

    asc_half2int32_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_215 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_215(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_215, asc_half2int4x2_rd_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_215));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_216 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_216(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_216, asc_half2int4x2_rd_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_216));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_217 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_217(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_217, asc_half2int4x2_rd_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_217));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_218 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_218(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_218, asc_half2int4x2_rd_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_218));

    asc_half2int4x2_rd(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_219 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_219(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_219, asc_half2int4x2_rd_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_219));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_220 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_220(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_220, asc_half2int4x2_rd_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_220));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_221 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_221(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_221, asc_half2int4x2_rd_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_221));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_222 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_222(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_222, asc_half2int4x2_rd_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_222));

    asc_half2int4x2_rd_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_223 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_223(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_223, asc_half2int4x2_rn_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_223));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_224 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_224(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_224, asc_half2int4x2_rn_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_224));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_225 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_225(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_225, asc_half2int4x2_rn_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_225));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_226 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_226(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_226, asc_half2int4x2_rn_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_226));

    asc_half2int4x2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_227 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_227(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_227, asc_half2int4x2_rn_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_227));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_228 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_228(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_228, asc_half2int4x2_rn_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_228));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_229 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_229(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_229, asc_half2int4x2_rn_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_229));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_230 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_230(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_230, asc_half2int4x2_rn_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_230));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_231 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_231(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_231, asc_half2int4x2_rna_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_231));

    asc_half2int4x2_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_232 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_232(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_232, asc_half2int4x2_rna_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_232));

    asc_half2int4x2_rna(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_233 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_233(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_233, asc_half2int4x2_rna_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_233));

    asc_half2int4x2_rna(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_234 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_234(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_234, asc_half2int4x2_rna_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_234));

    asc_half2int4x2_rna(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_235 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_235(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_235, asc_half2int4x2_rna_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_235));

    asc_half2int4x2_rna_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_236 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_236(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_236, asc_half2int4x2_rna_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_236));

    asc_half2int4x2_rna_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_237 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_237(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_237, asc_half2int4x2_rna_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_237));

    asc_half2int4x2_rna_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_238 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_238(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_238, asc_half2int4x2_rna_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_238));

    asc_half2int4x2_rna_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_239 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_239(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_239, asc_half2int4x2_ru_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_239));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_240 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_240(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_240, asc_half2int4x2_ru_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_240));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_241 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_241(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_241, asc_half2int4x2_ru_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_241));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_242 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_242(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_242, asc_half2int4x2_ru_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_242));

    asc_half2int4x2_ru(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_243 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_243(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_243, asc_half2int4x2_ru_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_243));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_244 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_244(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_244, asc_half2int4x2_ru_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_244));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_245 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_245(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_245, asc_half2int4x2_ru_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_245));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_246 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_246(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_246, asc_half2int4x2_ru_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_246));

    asc_half2int4x2_ru_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_247 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_247(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_247, asc_half2int4x2_rz_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_247));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_248 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_248(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_248, asc_half2int4x2_rz_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_248));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_249 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_249(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_249, asc_half2int4x2_rz_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_249));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_250 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_250(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_250, asc_half2int4x2_rz_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_250));

    asc_half2int4x2_rz(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_251 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_251(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_251, asc_half2int4x2_rz_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_251));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_252 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_252(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_252, asc_half2int4x2_rz_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_252));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_253 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_253(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_253, asc_half2int4x2_rz_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_253));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_254 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s4x2_x_Stub_254(vector_int4x2_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_254, asc_half2int4x2_rz_sat_ASC_DISP_Succ) {
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s4x2_x, void(vector_int4x2_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s4x2_x_Stub_254));

    asc_half2int4x2_rz_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_255 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_255(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_255, asc_half2int8_rd_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_255));

    asc_half2int8_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_256 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_256(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_256, asc_half2int8_rd_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_256));

    asc_half2int8_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_257 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_257(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_257, asc_half2int8_rd_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_257));

    asc_half2int8_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_258 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_258(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_258, asc_half2int8_rd_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_258));

    asc_half2int8_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_259 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_259(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_259, asc_half2int8_rn_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_259));

    asc_half2int8_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_260 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_260(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_260, asc_half2int8_rn_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_260));

    asc_half2int8_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_261 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_261(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_261, asc_half2int8_rn_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_261));

    asc_half2int8_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_262 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_262(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_262, asc_half2int8_rn_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_262));

    asc_half2int8_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_263 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_263(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_263, asc_half2int8_rna_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_263));

    asc_half2int8_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_264 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_264(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_264, asc_half2int8_rna_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_264));

    asc_half2int8_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_265 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_265(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_265, asc_half2int8_rna_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_265));

    asc_half2int8_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_266 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_266(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_266, asc_half2int8_rna_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_266));

    asc_half2int8_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_267 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_267(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_267, asc_half2int8_ru_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_267));

    asc_half2int8_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_268 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_268(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_268, asc_half2int8_ru_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_268));

    asc_half2int8_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_269 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_269(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_269, asc_half2int8_ru_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_269));

    asc_half2int8_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_270 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_270(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_270, asc_half2int8_ru_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_270));

    asc_half2int8_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_271 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_271(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_271, asc_half2int8_rz_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_271));

    asc_half2int8_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_272 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_272(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_272, asc_half2int8_rz_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_272));

    asc_half2int8_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_273 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_273(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_273, asc_half2int8_rz_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_273));

    asc_half2int8_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_274 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162s8_x_Stub_274(vector_int8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_274, asc_half2int8_rz_sat_ASC_POSI_Succ) {
    vector_int8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162s8_x, void(vector_int8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162s8_x_Stub_274));

    asc_half2int8_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_275 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_275(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_275, asc_half2uint8_rd_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_275));

    asc_half2uint8_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_276 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_276(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_276, asc_half2uint8_rd_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_276));

    asc_half2uint8_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_277 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_277(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_277, asc_half2uint8_rd_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_277));

    asc_half2uint8_rd_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_278 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_278(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_278, asc_half2uint8_rd_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_278));

    asc_half2uint8_rd_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_279 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_279(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_279, asc_half2uint8_rn_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_279));

    asc_half2uint8_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_280 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_280(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_280, asc_half2uint8_rn_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_280));

    asc_half2uint8_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_281 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_281(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_281, asc_half2uint8_rn_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_281));

    asc_half2uint8_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_282 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_282(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_282, asc_half2uint8_rn_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_282));

    asc_half2uint8_rn_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_283 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_283(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_283, asc_half2uint8_rna_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_283));

    asc_half2uint8_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_284 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_284(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_284, asc_half2uint8_rna_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_284));

    asc_half2uint8_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_285 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_285(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_285, asc_half2uint8_rna_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_285));

    asc_half2uint8_rna_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_286 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_286(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_286, asc_half2uint8_rna_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_286));

    asc_half2uint8_rna_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_287 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_287(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_287, asc_half2uint8_ru_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_287));

    asc_half2uint8_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_288 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_288(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_288, asc_half2uint8_ru_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_288));

    asc_half2uint8_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_289 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_289(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_289, asc_half2uint8_ru_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_289));

    asc_half2uint8_ru_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_290 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_290(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_290, asc_half2uint8_ru_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_290));

    asc_half2uint8_ru_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_291 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_291(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_291, asc_half2uint8_rz_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_291));

    asc_half2uint8_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_292 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_292(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_292, asc_half2uint8_rz_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_292));

    asc_half2uint8_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_293 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_293(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_293, asc_half2uint8_rz_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_293));

    asc_half2uint8_rz_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_294 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtfi_f162u8_x_Stub_294(vector_uint8_t dst, vector_half src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_294, asc_half2uint8_rz_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_half src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtfi_f162u8_x, void(vector_uint8_t, vector_half, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtfi_f162u8_x_Stub_294));

    asc_half2uint8_rz_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_295 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_Stub_295(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_295, asc_hif82float_ASC_DISP_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_Stub_295));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_296 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_Stub_296(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_296, asc_hif82float_ASC_DISP_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_Stub_296));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_297 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_Stub_297(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_297, asc_hif82float_ASC_DISP_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_Stub_297));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_298 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f32_x_Stub_298(vector_float dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_298, asc_hif82float_ASC_DISP_Succ) {
    vector_float dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f32_x, void(vector_float, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f32_x_Stub_298));

    asc_hif82float(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_299 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f16_x_Stub_299(vector_half dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_299, asc_hif82half_ASC_POSI_Succ) {
    vector_half dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f16_x, void(vector_half, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f16_x_Stub_299));

    asc_hif82half(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_300 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_hif82f16_x_Stub_300(vector_half dst, vector_hifloat8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_300, asc_hif82half_ASC_POSI_Succ) {
    vector_half dst;
    vector_hifloat8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_hif82f16_x, void(vector_half, vector_hifloat8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_hif82f16_x_Stub_300));

    asc_hif82half(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_301 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s162f32_x_Stub_301(vector_float dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_301, asc_int162float_ASC_POSI_Succ) {
    vector_float dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s162f32_x, void(vector_float, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s162f32_x_Stub_301));

    asc_int162float(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_302 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s162f32_x_Stub_302(vector_float dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_302, asc_int162float_ASC_POSI_Succ) {
    vector_float dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s162f32_x, void(vector_float, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s162f32_x_Stub_302));

    asc_int162float(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_303 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162s32_x_Stub_303(vector_int32_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_303, asc_int162int32_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162s32_x, void(vector_int32_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162s32_x_Stub_303));

    asc_int162int32(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_304 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162s32_x_Stub_304(vector_int32_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_304, asc_int162int32_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162s32_x, void(vector_int32_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162s32_x_Stub_304));

    asc_int162int32(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_305 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u32_x_Stub_305(vector_uint32_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_305, asc_int162uint32_ASC_POSI_Succ) {
    vector_uint32_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u32_x, void(vector_uint32_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u32_x_Stub_305));

    asc_int162uint32(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_306 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u32_x_Stub_306(vector_uint32_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_306, asc_int162uint32_ASC_POSI_Succ) {
    vector_uint32_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u32_x, void(vector_uint32_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u32_x_Stub_306));

    asc_int162uint32(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_307 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_Stub_307(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_307, asc_int162uint8_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_Stub_307));

    asc_int162uint8(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_308 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_Stub_308(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_308, asc_int162uint8_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_Stub_308));

    asc_int162uint8(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_309 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_Stub_309(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_309, asc_int162uint8_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_Stub_309));

    asc_int162uint8_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_310 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s162u8_x_Stub_310(vector_uint8_t dst, vector_int16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_310, asc_int162uint8_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s162u8_x, void(vector_uint8_t, vector_int16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s162u8_x_Stub_310));

    asc_int162uint8_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_311 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_Stub_311(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_311, asc_int322int16_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_Stub_311));

    asc_int322int16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_312 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_Stub_312(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_312, asc_int322int16_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_Stub_312));

    asc_int322int16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_313 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_Stub_313(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_313, asc_int322int16_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_Stub_313));

    asc_int322int16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_314 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s16_x_Stub_314(vector_int16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_314, asc_int322int16_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s16_x, void(vector_int16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s16_x_Stub_314));

    asc_int322int16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_315 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s64_x_Stub_315(vector_int64_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_315, asc_int322int64_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s64_x, void(vector_int64_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s64_x_Stub_315));

    asc_int322int64(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_316 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322s64_x_Stub_316(vector_int64_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_316, asc_int322int64_ASC_POSI_Succ) {
    vector_int64_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322s64_x, void(vector_int64_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322s64_x_Stub_316));

    asc_int322int64(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_317 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_Stub_317(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_317, asc_int322uint16_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_Stub_317));

    asc_int322uint16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_318 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_Stub_318(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_318, asc_int322uint16_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_Stub_318));

    asc_int322uint16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_319 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_Stub_319(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_319, asc_int322uint16_sat_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_Stub_319));

    asc_int322uint16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_320 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u16_x_Stub_320(vector_uint16_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_320, asc_int322uint16_sat_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u16_x, void(vector_uint16_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u16_x_Stub_320));

    asc_int322uint16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_321 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_321(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_321, asc_int322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_321));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_322 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_322(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_322, asc_int322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_322));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_323 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_323(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_323, asc_int322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_323));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_324 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_324(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_324, asc_int322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_324));

    asc_int322uint8(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_325 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_325(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_325, asc_int322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_325));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_326 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_326(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_326, asc_int322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_326));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_327 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_327(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_327, asc_int322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_327));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_328 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s322u8_x_Stub_328(vector_uint8_t dst, vector_int32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_328, asc_int322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_int32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s322u8_x, void(vector_uint8_t, vector_int32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s322u8_x_Stub_328));

    asc_int322uint8_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_329 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_Stub_329(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_329, asc_int4x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_Stub_329));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_330 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_Stub_330(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_330, asc_int4x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_Stub_330));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_331 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_Stub_331(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_331, asc_int4x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_Stub_331));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_332 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22bf16_x_Stub_332(vector_bfloat16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_332, asc_int4x22bfloat16_ASC_DISP_Succ) {
    vector_bfloat16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22bf16_x, void(vector_bfloat16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22bf16_x_Stub_332));

    asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_333 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22f16_x_Stub_333(vector_half dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_333, asc_int4x22half_ASC_DISP_Succ) {
    vector_half dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22f16_x, void(vector_half, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22f16_x_Stub_333));

    asc_int4x22half(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_334 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22f16_x_Stub_334(vector_half dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_334, asc_int4x22half_ASC_DISP_Succ) {
    vector_half dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22f16_x, void(vector_half, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22f16_x_Stub_334));

    asc_int4x22half(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_335 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22f16_x_Stub_335(vector_half dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_335, asc_int4x22half_ASC_DISP_Succ) {
    vector_half dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22f16_x, void(vector_half, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22f16_x_Stub_335));

    asc_int4x22half(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_336 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s4x22f16_x_Stub_336(vector_half dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_336, asc_int4x22half_ASC_DISP_Succ) {
    vector_half dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s4x22f16_x, void(vector_half, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s4x22f16_x_Stub_336));

    asc_int4x22half(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_337 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_Stub_337(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_337, asc_int4x22int16_ASC_DISP_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_Stub_337));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_338 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_Stub_338(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_338, asc_int4x22int16_ASC_DISP_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_Stub_338));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_339 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_Stub_339(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_339, asc_int4x22int16_ASC_DISP_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_Stub_339));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_340 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s4x22s16_x_Stub_340(vector_int16_t dst, vector_int4x2_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_340, asc_int4x22int16_ASC_DISP_Succ) {
    vector_int16_t dst;
    vector_int4x2_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s4x22s16_x, void(vector_int16_t, vector_int4x2_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s4x22s16_x_Stub_340));

    asc_int4x22int16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_341 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_341(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_341, asc_int642float_rd_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_341));

    asc_int642float_rd(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_342 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_342(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_342, asc_int642float_rd_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_342));

    asc_int642float_rd(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_343 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_343(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_343, asc_int642float_rn_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_343));

    asc_int642float_rn(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_344 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_344(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_344, asc_int642float_rn_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_344));

    asc_int642float_rn(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_345 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_345(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_345, asc_int642float_rna_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_345));

    asc_int642float_rna(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_346 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_346(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_346, asc_int642float_rna_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_346));

    asc_int642float_rna(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_347 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_347(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_347, asc_int642float_ru_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_347));

    asc_int642float_ru(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_348 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_348(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_348, asc_int642float_ru_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_348));

    asc_int642float_ru(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_349 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_349(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_349, asc_int642float_rz_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_349));

    asc_int642float_rz(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_350 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s642f32_x_Stub_350(vector_float dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_350, asc_int642float_rz_ASC_POSI_Succ) {
    vector_float dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s642f32_x, void(vector_float, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s642f32_x_Stub_350));

    asc_int642float_rz(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_351 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_Stub_351(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_351, asc_int642int32_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_Stub_351));

    asc_int642int32(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_352 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_Stub_352(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_352, asc_int642int32_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_Stub_352));

    asc_int642int32(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_353 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_Stub_353(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_353, asc_int642int32_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_Stub_353));

    asc_int642int32_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_354 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s642s32_x_Stub_354(vector_int32_t dst, vector_int64_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_354, asc_int642int32_sat_ASC_POSI_Succ) {
    vector_int32_t dst;
    vector_int64_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s642s32_x, void(vector_int32_t, vector_int64_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s642s32_x_Stub_354));

    asc_int642int32_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_355 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s82f16_x_Stub_355(vector_half dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_355, asc_int82half_ASC_POSI_Succ) {
    vector_half dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s82f16_x, void(vector_half, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s82f16_x_Stub_355));

    asc_int82half(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_356 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_s82f16_x_Stub_356(vector_half dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_356, asc_int82half_ASC_POSI_Succ) {
    vector_half dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_s82f16_x, void(vector_half, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_s82f16_x_Stub_356));

    asc_int82half(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_357 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s16_x_Stub_357(vector_int16_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_357, asc_int82int16_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s16_x, void(vector_int16_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s16_x_Stub_357));

    asc_int82int16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_358 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s16_x_Stub_358(vector_int16_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_358, asc_int82int16_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s16_x, void(vector_int16_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s16_x_Stub_358));

    asc_int82int16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_359 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_Stub_359(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_359, asc_int82int32_ASC_DISP_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_Stub_359));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_360 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_Stub_360(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_360, asc_int82int32_ASC_DISP_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_Stub_360));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_361 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_Stub_361(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_361, asc_int82int32_ASC_DISP_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_Stub_361));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_362 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_s82s32_x_Stub_362(vector_int32_t dst, vector_int8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_362, asc_int82int32_ASC_DISP_Succ) {
    vector_int32_t dst;
    vector_int8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_s82s32_x, void(vector_int32_t, vector_int8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_s82s32_x_Stub_362));

    asc_int82int32(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_363 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u162u32_x_Stub_363(vector_uint32_t dst, vector_uint16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_363, asc_uint162uint32_ASC_POSI_Succ) {
    vector_uint32_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u162u32_x, void(vector_uint32_t, vector_uint16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u162u32_x_Stub_363));

    asc_uint162uint32(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_364 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u162u32_x_Stub_364(vector_uint32_t dst, vector_uint16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_364, asc_uint162uint32_ASC_POSI_Succ) {
    vector_uint32_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u162u32_x, void(vector_uint32_t, vector_uint16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u162u32_x_Stub_364));

    asc_uint162uint32(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_365 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u162u8_x_Stub_365(vector_uint8_t dst, vector_uint16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_365, asc_uint162uint8_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u162u8_x, void(vector_uint8_t, vector_uint16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u162u8_x_Stub_365));

    asc_uint162uint8(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_366 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u162u8_x_Stub_366(vector_uint8_t dst, vector_uint16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_366, asc_uint162uint8_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u162u8_x, void(vector_uint8_t, vector_uint16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u162u8_x_Stub_366));

    asc_uint162uint8(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_367 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u162u8_x_Stub_367(vector_uint8_t dst, vector_uint16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_367, asc_uint162uint8_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u162u8_x, void(vector_uint8_t, vector_uint16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u162u8_x_Stub_367));

    asc_uint162uint8_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_368 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u162u8_x_Stub_368(vector_uint8_t dst, vector_uint16_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_368, asc_uint162uint8_sat_ASC_POSI_Succ) {
    vector_uint8_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u162u8_x, void(vector_uint8_t, vector_uint16_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u162u8_x_Stub_368));

    asc_uint162uint8_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_369 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322s16_x_Stub_369(vector_int16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_369, asc_uint322int16_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322s16_x, void(vector_int16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322s16_x_Stub_369));

    asc_uint322int16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_370 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322s16_x_Stub_370(vector_int16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_370, asc_uint322int16_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322s16_x, void(vector_int16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322s16_x_Stub_370));

    asc_uint322int16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_371 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322s16_x_Stub_371(vector_int16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_371, asc_uint322int16_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322s16_x, void(vector_int16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322s16_x_Stub_371));

    asc_uint322int16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_372 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322s16_x_Stub_372(vector_int16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_372, asc_uint322int16_sat_ASC_POSI_Succ) {
    vector_int16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322s16_x, void(vector_int16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322s16_x_Stub_372));

    asc_uint322int16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_373 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_Stub_373(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_373, asc_uint322uint16_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_Stub_373));

    asc_uint322uint16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_374 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_Stub_374(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_374, asc_uint322uint16_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_Stub_374));

    asc_uint322uint16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_375 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_Stub_375(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_375, asc_uint322uint16_sat_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_Stub_375));

    asc_uint322uint16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_376 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_Stub_376(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_376, asc_uint322uint16_sat_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_Stub_376));

    asc_uint322uint16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_377 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_377(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_377, asc_uint322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_377));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_378 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_378(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_378, asc_uint322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_378));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_379 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_379(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_379, asc_uint322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_379));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_380 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_380(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_380, asc_uint322uint8_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_380));

    asc_uint322uint8(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_381 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_381(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_381, asc_uint322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_381));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_382 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_382(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_382, asc_uint322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_382));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_383 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_383(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_383, asc_uint322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_383));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_384 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u8_x_Stub_384(vector_uint8_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_384, asc_uint322uint8_sat_ASC_DISP_Succ) {
    vector_uint8_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u8_x, void(vector_uint8_t, vector_uint32_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u8_x_Stub_384));

    asc_uint322uint8_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_385 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_u82f16_x_Stub_385(vector_half dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_385, asc_uint82half_ASC_POSI_Succ) {
    vector_half dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_u82f16_x, void(vector_half, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_u82f16_x_Stub_385));

    asc_uint82half(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_386 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtif_u82f16_x_Stub_386(vector_half dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_386, asc_uint82half_ASC_POSI_Succ) {
    vector_half dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtif_u82f16_x, void(vector_half, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtif_u82f16_x_Stub_386));

    asc_uint82half(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_387 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u16_x_Stub_387(vector_uint16_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_387, asc_uint82uint16_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u16_x, void(vector_uint16_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u16_x_Stub_387));

    asc_uint82uint16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos_388 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u16_x_Stub_388(vector_uint16_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_388, asc_uint82uint16_ASC_POSI_Succ) {
    vector_uint16_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u16_x, void(vector_uint16_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u16_x_Stub_388));

    asc_uint82uint16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos_389 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_Stub_389(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_389, asc_uint82uint32_ASC_DISP_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_Stub_389));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_390 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_Stub_390(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_390, asc_uint82uint32_ASC_DISP_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_Stub_390));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_391 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_Stub_391(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_391, asc_uint82uint32_ASC_DISP_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_Stub_391));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos_392 : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_Stub_392(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos_392, asc_uint82uint32_ASC_DISP_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_Stub_392));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}
