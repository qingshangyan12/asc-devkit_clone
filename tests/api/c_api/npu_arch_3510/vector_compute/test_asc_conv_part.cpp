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

#define TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(class_name, c_api_name, cce_name, dst_type, src_type)   \
                                                                                                \
class TestVectorCompute##c_api_name##dst_type##src_type##CApi : public testing::Test {          \
protected:                                                                                      \
    void SetUp() {}                                                                             \
    void TearDown() {}                                                                          \
};                                                                                              \
                                                                                                \
namespace {                                                                                     \
void c_api_name##cce_name##_##dst_type##_##src_type##_Stub(dst_type& dst, src_type src,         \
    vector_bool mask, int part, int mode) {}                                                    \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorCompute##c_api_name##dst_type##src_type##CApi, c_api_name##_##dst_type##_##src_type##_Succ)  \
{                                                                                               \
    dst_type dst;                                                                               \
    src_type src;                                                                               \
    vector_bool mask;                                                                           \
                                                                                                \
    MOCKER_CPP(cce_name, void(dst_type&, src_type, vector_bool, int, int))                      \
        .times(1)                                                                               \
        .will(invoke(c_api_name##cce_name##_##dst_type##_##src_type##_Stub));                   \
                                                                                                \
    c_api_name(dst, src, mask);                                                                 \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \

// ==========asc_uint82uint32==========
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVSAT, asc_uint82uint32, vcvt, vector_uint32_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVSAT, asc_uint82uint32_v2, vcvt, vector_uint32_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVSAT, asc_uint82uint32_v3, vcvt, vector_uint32_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVSAT, asc_uint82uint32_v4, vcvt, vector_uint32_t, vector_uint8_t);

// ==========asc_e2m1x22bfloat16==========
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVPART, asc_e2m1x22bfloat16, vcvt, vector_bfloat16_t, vector_fp4x2_e2m1_t);
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVPART, asc_e2m1x22bfloat16_v2, vcvt, vector_bfloat16_t, vector_fp4x2_e2m1_t);
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVPART, asc_e2m1x22bfloat16_v3, vcvt, vector_bfloat16_t, vector_fp4x2_e2m1_t);
TEST_VECTOR_COMPUTE_VCONV_PART_INSTR_PART(CONVPART, asc_e2m1x22bfloat16_v4, vcvt, vector_bfloat16_t, vector_fp4x2_e2m1_t);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos201 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_201(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos201, asc_e2m1x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_201));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos202 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_202(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos202, asc_e2m1x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_202));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos203 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_203(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos203, asc_e2m1x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_203));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos204 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_204(vector_bfloat16_t dst, vector_fp4x2_e2m1_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos204, asc_e2m1x22bfloat16_ASC_DISPERSE_Succ) {
    vector_bfloat16_t dst;
    vector_fp4x2_e2m1_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f4e2m1x22bf16_x, void(vector_bfloat16_t, vector_fp4x2_e2m1_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f4e2m1x22bf16_x_PosStub_204));

    asc_e2m1x22bfloat16(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos205 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_PosStub_205(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos205, asc_uint82uint32_ASC_DISPERSE_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_PosStub_205));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos206 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_PosStub_206(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos206, asc_uint82uint32_ASC_DISPERSE_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_PosStub_206));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos207 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_PosStub_207(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos207, asc_uint82uint32_ASC_DISPERSE_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_PosStub_207));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos208 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtii_u82u32_x_PosStub_208(vector_uint32_t dst, vector_uint8_t src, vector_bool mask, unsigned long long a0) {}
}

TEST_F(TestVcvtPos208, asc_uint82uint32_ASC_DISPERSE_Succ) {
    vector_uint32_t dst;
    vector_uint8_t src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtii_u82u32_x, void(vector_uint32_t, vector_uint8_t, vector_bool, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtii_u82u32_x_PosStub_208));

    asc_uint82uint32(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
