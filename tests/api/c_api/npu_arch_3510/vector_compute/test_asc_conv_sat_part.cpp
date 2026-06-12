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

#define TEST_VECTOR_COMPUTE_VCONV_SAT_PART(class_name, c_api_name, cce_name, dst_type, src_type)   \
                                                                                                \
class TestVectorCompute##class_name##_##c_api_name##_##dst_type##src_type##CApi : public testing::Test {          \
protected:                                                                                      \
    void SetUp() {}                                                                             \
    void TearDown() {}                                                                          \
};                                                                                              \
                                                                                                \
namespace {                                                                                     \
void c_api_name##_##dst_type##_##src_type##_Stub(dst_type& dst, src_type src, vector_bool mask,   \
    int round, int sat, int part, int mode) {}                                                  \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorCompute##class_name##_##c_api_name##_##dst_type##src_type##CApi, c_api_name##_##dst_type##_##src_type##_Succ)  \
{                                                                                               \
    dst_type dst;                                                                               \
    src_type src;                                                                               \
    vector_bool mask;                                                                           \
                                                                                                \
    MOCKER_CPP(cce_name, void(dst_type&, src_type, vector_bool, int, int, int, int))            \
        .times(1)                                                                               \
        .will(invoke(c_api_name##_##dst_type##_##src_type##_Stub));                               \
                                                                                                \
    c_api_name(dst, src, mask);                                                                 \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \

// ==========(asc_float2hif8_rh/rna)==========
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_sat, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_sat_v2, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_v2, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_sat_v3, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_v3, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_sat_v4, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rh_v4, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_sat, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_sat_v2, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_v2, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_sat_v3, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_v3, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_sat_v4, vcvt, vector_hifloat8_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2hif8_rna_v4, vcvt, vector_hifloat8_t, vector_float);
// ==========(asc_float2e4m3_rn)==========
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_sat, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_sat_v2, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_v2, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_sat_v3, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_v3, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_sat_v4, vcvt, vector_fp8_e4m3fn_t, vector_float);
TEST_VECTOR_COMPUTE_VCONV_SAT_PART(ASCFLOATHIF, asc_float2e4m3_rn_v4, vcvt, vector_fp8_e4m3fn_t, vector_float);

// ========== Renamed pos-parameter overload tests ==========

class TestVcvtPos401 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_401(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos401, asc_float2e4m3_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_401));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos402 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_402(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos402, asc_float2e4m3_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_402));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos403 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_403(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos403, asc_float2e4m3_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_403));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos404 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_404(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos404, asc_float2e4m3_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_404));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos405 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_405(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos405, asc_float2e4m3_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_405));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos406 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_406(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos406, asc_float2e4m3_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_406));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos407 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_407(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos407, asc_float2e4m3_rn_sat_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_407));

    asc_float2e4m3_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos408 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322f8e4m3_x_PosStub_408(vector_fp8_e4m3fn_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos408, asc_float2e4m3_rn_ASC_DISPERSE_Succ) {
    vector_fp8_e4m3fn_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322f8e4m3_x, void(vector_fp8_e4m3fn_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322f8e4m3_x_PosStub_408));

    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos409 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_409(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos409, asc_float2hif8_rh_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_409));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos410 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_410(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos410, asc_float2hif8_rh_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_410));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos411 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_411(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos411, asc_float2hif8_rh_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_411));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos412 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_412(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos412, asc_float2hif8_rh_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_412));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos413 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_413(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos413, asc_float2hif8_rh_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_413));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos414 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_414(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos414, asc_float2hif8_rh_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_414));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos415 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_415(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos415, asc_float2hif8_rh_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_415));

    asc_float2hif8_rh_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos416 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_416(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos416, asc_float2hif8_rh_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_416));

    asc_float2hif8_rh(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos417 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_417(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos417, asc_float2hif8_rna_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_417));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos418 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_418(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos418, asc_float2hif8_rna_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_418));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos419 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_419(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos419, asc_float2hif8_rna_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_419));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos420 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_420(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos420, asc_float2hif8_rna_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_420));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_SECOND_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos421 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_421(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos421, asc_float2hif8_rna_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_421));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos422 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_422(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos422, asc_float2hif8_rna_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_422));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_THIRD_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos423 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_423(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos423, asc_float2hif8_rna_sat_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_423));

    asc_float2hif8_rna_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

class TestVcvtPos424 : public testing::Test {
protected:
    void SetUp() {} void TearDown() {}
};

namespace {
void __builtin_cce_vcvtff_f322hif8_x_PosStub_424(vector_hifloat8_t dst, vector_float src, vector_bool mask, unsigned long long a0, unsigned long long a1, unsigned long long a2) {}
}

TEST_F(TestVcvtPos424, asc_float2hif8_rna_ASC_DISPERSE_Succ) {
    vector_hifloat8_t dst;
    vector_float src;
    vector_bool mask;

    MOCKER_CPP(__builtin_cce_vcvtff_f322hif8_x, void(vector_hifloat8_t, vector_float, vector_bool, unsigned long long, unsigned long long, unsigned long long))
        .times(1)
        .will(invoke(__builtin_cce_vcvtff_f322hif8_x_PosStub_424));

    asc_float2hif8_rna(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}
