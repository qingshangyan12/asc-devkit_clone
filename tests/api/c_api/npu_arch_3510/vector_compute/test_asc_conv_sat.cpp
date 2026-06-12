/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

#define TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(class_name, c_api_name, cce_name, dst_type, src_type)   \
                                                                                                \
class TestVectorCompute##c_api_name##dst_type##src_type##CApi : public testing::Test {          \
protected:                                                                                      \
    void SetUp() {}                                                                             \
    void TearDown() {}                                                                          \
};                                                                                              \
                                                                                                \
namespace {                                                                                     \
void c_api_name##cce_name##_##dst_type##_##src_type##_Stub(dst_type& dst, src_type src,         \
    vector_bool mask, int round, int part, int mode) {}                                          \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorCompute##c_api_name##dst_type##src_type##CApi, c_api_name##_##dst_type##_##src_type##_Succ)  \
{                                                                                               \
    dst_type dst;                                                                               \
    src_type src;                                                                               \
    vector_bool mask;                                                                           \
                                                                                                \
    MOCKER_CPP(cce_name, void(dst_type&, src_type, vector_bool, int, int, int))                 \
        .times(1)                                                                               \
        .will(invoke(c_api_name##cce_name##_##dst_type##_##src_type##_Stub));                   \
                                                                                                \
    c_api_name(dst, src, mask);                                                                 \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \

// ==========asc_float2int32_rd/ru/rz/rn/rna)==========
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rd, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rd_sat, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_ru, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_ru_sat, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rz, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rz_sat, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rn, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rn_sat, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rna, vcvt, vector_int32_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_float2int32_rna_sat, vcvt, vector_int32_t, vector_float);

// ==========asc_bfloat162e2m1x2_rd/ru/rz/rn/rna)==========
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rd, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rd_v2, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rd_v3, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rd_v4, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_ru, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_ru_v2, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_ru_v3, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_ru_v4, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rz, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rz_v2, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rz_v3, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rz_v4, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rn, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rn_v2, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rn_v3, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rn_v4, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rna, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rna_v2, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rna_v3, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_bfloat162e2m1x2_rna_v4, vcvt, vector_fp4x2_e2m1_t, vector_bfloat16_t);

// ==========asc_uint322uint16==========
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_uint322uint16_sat, vcvt, vector_uint16_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_uint322uint16, vcvt, vector_uint16_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_uint322uint16_sat_v2, vcvt, vector_uint16_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_VCONV_SAT_INSTR(CONVSAT, asc_uint322uint16_v2, vcvt, vector_uint16_t, vector_uint32_t);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos301 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_301(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos301, asc_bfloat162e2m1x2_rd_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_301));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos302 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_302(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos302, asc_bfloat162e2m1x2_rd_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_302));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos303 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_303(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos303, asc_bfloat162e2m1x2_rd_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_303));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos304 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_304(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos304, asc_bfloat162e2m1x2_rd_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_304));

    asc_bfloat162e2m1x2_rd(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos305 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_305(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos305, asc_bfloat162e2m1x2_rn_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_305));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos306 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_306(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos306, asc_bfloat162e2m1x2_rn_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_306));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos307 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_307(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos307, asc_bfloat162e2m1x2_rn_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_307));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos308 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_308(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos308, asc_bfloat162e2m1x2_rn_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_308));

    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos309 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_309(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos309, asc_bfloat162e2m1x2_rna_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_309));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos310 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_310(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos310, asc_bfloat162e2m1x2_rna_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_310));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos311 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_311(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos311, asc_bfloat162e2m1x2_rna_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_311));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos312 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_312(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos312, asc_bfloat162e2m1x2_rna_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_312));

    asc_bfloat162e2m1x2_rna(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos313 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_313(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos313, asc_bfloat162e2m1x2_ru_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_313));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos314 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_314(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos314, asc_bfloat162e2m1x2_ru_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_314));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos315 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_315(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos315, asc_bfloat162e2m1x2_ru_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_315));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos316 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_316(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos316, asc_bfloat162e2m1x2_ru_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_316));

    asc_bfloat162e2m1x2_ru(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos317 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_317(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos317, asc_bfloat162e2m1x2_rz_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_317));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos318 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_318(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos318, asc_bfloat162e2m1x2_rz_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_318));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos319 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_319(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos319, asc_bfloat162e2m1x2_rz_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_319));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos320 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_320(vector_fp4x2_e2m1_t dst, vector_bfloat16_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos320, asc_bfloat162e2m1x2_rz_ASC_DISPERSE_Succ) {
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_bf162f4e2m1x2_x, void(vector_fp4x2_e2m1_t, vector_bfloat16_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_bf162f4e2m1x2_x_PosStub_320));

    asc_bfloat162e2m1x2_rz(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos321 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_PosStub_321(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos321, asc_uint322uint16_sat_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_PosStub_321));

    asc_uint322uint16_sat(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos322 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_PosStub_322(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos322, asc_uint322uint16_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_PosStub_322));

    asc_uint322uint16(dst, src, mask, ASC_POSITION_EVEN);
    GlobalMockObject::verify();
}

class TestVcvtPos323 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_PosStub_323(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos323, asc_uint322uint16_sat_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_PosStub_323));

    asc_uint322uint16_sat(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

class TestVcvtPos324 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u322u16_x_PosStub_324(vector_uint16_t dst, vector_uint32_t src, vector_bool mask, unsigned long long a0, unsigned long long a1) {}
}

TEST_F(TestVcvtPos324, asc_uint322uint16_ASC_POSITION_Succ) {
    vector_uint16_t dst;
    vector_uint32_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u322u16_x, void(vector_uint16_t, vector_uint32_t, vector_bool, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u322u16_x_PosStub_324));

    asc_uint322uint16(dst, src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}
