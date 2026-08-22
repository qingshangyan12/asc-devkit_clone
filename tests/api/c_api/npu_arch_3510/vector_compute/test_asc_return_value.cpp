/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
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

// Return-value overloads: the underlying instr is still invoked once, and the
// result register is returned by value.

// ==========unary same-type: mock(dst&, src, mask, Literal)==========
#define TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(class_name, c_api_name, cce_name, data_type, ret_type)             \
                                                                                                               \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {             \
    protected:                                                                                                 \
        void SetUp() {}                                                                                        \
        void TearDown() {}                                                                                     \
    };                                                                                                         \
                                                                                                               \
    namespace {                                                                                                \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                              \
        ret_type& dst, data_type src0, vector_bool mask, Literal mode)                                         \
    {}                                                                                                         \
    }                                                                                                          \
                                                                                                               \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                                    \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                          \
    {                                                                                                          \
        data_type src0;                                                                                        \
        vector_bool mask;                                                                                      \
                                                                                                               \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, vector_bool, Literal))                                 \
            .times(1)                                                                                          \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                            \
                                                                                                               \
        ret_type dst = c_api_name(src0, mask);                                                                 \
        (void)dst;                                                                                             \
        GlobalMockObject::verify();                                                                            \
    }

// ==========reduce_sum/reduce_sum_datablock dst!=src: mock(dst&, src, mask, Literal)==========
#define TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(class_name, c_api_name, cce_name, ret_type, data_type)    \
                                                                                                          \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {        \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                        \
        ret_type& dst, data_type src0, vector_bool mask, Literal mode)                                    \
    {}                                                                                                    \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                               \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                     \
    {                                                                                                     \
        data_type src0;                                                                                   \
        vector_bool mask;                                                                                 \
                                                                                                          \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, vector_bool, Literal))                            \
            .times(1)                                                                                     \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                       \
                                                                                                          \
        ret_type dst = c_api_name(src0, mask);                                                            \
        (void)dst;                                                                                        \
        GlobalMockObject::verify();                                                                       \
    }

// ==========binary same-type: mock(dst&, src0, src1, mask, Literal)==========
#define TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(class_name, c_api_name, cce_name, data_type, ret_type)       \
                                                                                                          \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {        \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                        \
        ret_type& dst, data_type src0, data_type src1, vector_bool mask, Literal mode)                    \
    {}                                                                                                    \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                               \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                     \
    {                                                                                                     \
        data_type src0;                                                                                   \
        data_type src1;                                                                                   \
        vector_bool mask;                                                                                 \
                                                                                                          \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, data_type, vector_bool, Literal))                 \
            .times(1)                                                                                     \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                       \
                                                                                                          \
        ret_type dst = c_api_name(src0, src1, mask);                                                      \
        (void)dst;                                                                                        \
        GlobalMockObject::verify();                                                                       \
    }

// ==========fma: mock(dst&, src0, src1, mask, Literal)==========
#define TEST_VECTOR_COMPUTE_FMA_RET_INSTR(class_name, c_api_name, cce_name, data_type)                    \
                                                                                                          \
    class TestVectorCompute##class_name##_##data_type##_CApi : public testing::Test {                     \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void cce_name##ReturnValue##_##data_type##_Stub(                                                      \
        data_type& dst, data_type src0, data_type src1, vector_bool mask, Literal mode)                   \
    {}                                                                                                    \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestVectorCompute##class_name##_##data_type##_CApi, c_api_name##ReturnValue##_##data_type##_Succ) \
    {                                                                                                     \
        data_type dstSrc;                                                                                 \
        data_type src0;                                                                                   \
        data_type src1;                                                                                   \
        vector_bool mask;                                                                                 \
                                                                                                          \
        MOCKER_CPP(cce_name, void(data_type&, data_type, data_type, vector_bool, Literal))                \
            .times(1)                                                                                     \
            .will(invoke(cce_name##ReturnValue##_##data_type##_Stub));                                    \
                                                                                                          \
        data_type (*func)(data_type, data_type, data_type, vector_bool) = c_api_name;                     \
        data_type dst = func(dstSrc, src0, src1, mask);                                                   \
        (void)dst;                                                                                        \
        GlobalMockObject::verify();                                                                       \
    }

// ==========exp_sub: mock(dst&, src0, src1, mask, int)==========
#define TEST_VECTOR_COMPUTE_EXP_SUB_RET_INSTR(class_name, c_api_name, cce_name, ret_type, data_type) \
                                                                                                     \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {   \
    protected:                                                                                       \
        void SetUp() {}                                                                              \
        void TearDown() {}                                                                           \
    };                                                                                               \
                                                                                                     \
    namespace {                                                                                      \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                   \
        ret_type& dst, data_type src0, data_type src1, vector_bool mask, int32_t mode)               \
    {}                                                                                               \
    }                                                                                                \
                                                                                                     \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                          \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                \
    {                                                                                                \
        data_type src0;                                                                              \
        data_type src1;                                                                              \
        vector_bool mask;                                                                            \
                                                                                                     \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, data_type, vector_bool, int32_t))            \
            .times(1)                                                                                \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                  \
                                                                                                     \
        ret_type dst = c_api_name(src0, src1, mask);                                                 \
        (void)dst;                                                                                   \
        GlobalMockObject::verify();                                                                  \
    }

// ==========exp_sub_half2float: mock(dst&, src0, src1, mask, int) with position overloads==========
#define TEST_VECTOR_COMPUTE_EXP_SUB_H2F_RET_INSTR(class_name, c_api_name, cce_name, ret_type, data_type) \
                                                                                                          \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {        \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                        \
        ret_type& dst, data_type src0, data_type src1, vector_bool mask, int32_t mode)                    \
    {}                                                                                                    \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                               \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                     \
    {                                                                                                     \
        data_type src0;                                                                                   \
        data_type src1;                                                                                   \
        vector_bool mask;                                                                                 \
                                                                                                          \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, data_type, vector_bool, int32_t))                 \
            .times(2)                                                                                     \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                       \
                                                                                                          \
        ret_type dstEven = c_api_name(src0, src1, mask, ASC_POSITION_EVEN);                               \
        (void)dstEven;                                                                                    \
        ret_type dstOdd = c_api_name(src0, src1, mask, ASC_POSITION_ODD);                                 \
        (void)dstOdd;                                                                                     \
        GlobalMockObject::verify();                                                                       \
    }

// ==========select: mock(dst&, src0, src1, mask)==========
#define TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(class_name, c_api_name, cce_name, data_type, ret_type)       \
                                                                                                          \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {        \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                        \
        ret_type& dst, data_type src0, data_type src1, vector_bool mask)                                  \
    {}                                                                                                    \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                               \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                     \
    {                                                                                                     \
        data_type src0;                                                                                   \
        data_type src1;                                                                                   \
        vector_bool mask;                                                                                 \
                                                                                                          \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, data_type, vector_bool))                          \
            .times(1)                                                                                     \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                       \
                                                                                                          \
        ret_type dst = c_api_name(src0, src1, mask);                                                      \
        (void)dst;                                                                                        \
        GlobalMockObject::verify();                                                                       \
    }

// ==========scalar: mock(dst&, src, scalar, mask, Literal)==========
#define TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(class_name, c_api_name, cce_name, data_type, scalar_type, ret_type) \
                                                                                                                  \
    class TestVectorCompute##class_name##_##data_type##_##scalar_type##_CApi : public testing::Test {            \
    protected:                                                                                                    \
        void SetUp() {}                                                                                           \
        void TearDown() {}                                                                                        \
    };                                                                                                            \
                                                                                                                  \
    namespace {                                                                                                   \
    void cce_name##ReturnValue##_##data_type##_##scalar_type##_Stub(                                             \
        ret_type& dst, data_type src0, scalar_type src1, vector_bool mask, Literal mode)                          \
    {}                                                                                                            \
    }                                                                                                             \
                                                                                                                  \
    TEST_F(TestVectorCompute##class_name##_##data_type##_##scalar_type##_CApi,                                    \
           c_api_name##ReturnValue##_##data_type##_##scalar_type##_Succ)                                          \
    {                                                                                                             \
        data_type src0;                                                                                           \
        scalar_type src1;                                                                                         \
        vector_bool mask;                                                                                         \
                                                                                                                  \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, scalar_type, vector_bool, Literal))                       \
            .times(1)                                                                                             \
            .will(invoke(cce_name##ReturnValue##_##data_type##_##scalar_type##_Stub));                            \
                                                                                                                  \
        ret_type dst = c_api_name(src0, src1, mask);                                                              \
        (void)dst;                                                                                                \
        GlobalMockObject::verify();                                                                               \
    }

// ==========compare scalar: mock(vector_bool&, src, scalar, mask)==========
#define TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(class_name, c_api_name, cce_name, data_type, scalar_type)       \
                                                                                                           \
    class TestVectorCompute##class_name##_##data_type##_##scalar_type##_CApi : public testing::Test {      \
    protected:                                                                                             \
        void SetUp() {}                                                                                    \
        void TearDown() {}                                                                                 \
    };                                                                                                     \
                                                                                                           \
    namespace {                                                                                            \
    void cce_name##ReturnValue##_##data_type##_##scalar_type##_Stub(                                      \
        vector_bool& dst, data_type src0, scalar_type src1, vector_bool mask)                              \
    {}                                                                                                     \
    }                                                                                                      \
                                                                                                           \
    TEST_F(TestVectorCompute##class_name##_##data_type##_##scalar_type##_CApi,                             \
           c_api_name##ReturnValue##_##data_type##_##scalar_type##_Succ)                                   \
    {                                                                                                      \
        data_type src0;                                                                                    \
        scalar_type src1;                                                                                  \
        vector_bool mask;                                                                                  \
                                                                                                           \
        MOCKER_CPP(cce_name, void(vector_bool&, data_type, scalar_type, vector_bool))                      \
            .times(1)                                                                                      \
            .will(invoke(cce_name##ReturnValue##_##data_type##_##scalar_type##_Stub));                     \
                                                                                                           \
        vector_bool dst = c_api_name(src0, src1, mask);                                                    \
        (void)dst;                                                                                         \
        GlobalMockObject::verify();                                                                        \
    }

// ==========vcvt position-based, 5 args(part, mode): mock(dst&, src, mask, int, int)==========
#define TEST_VECTOR_COMPUTE_VCVT_RET_PART_INSTR(class_name, c_api_name, cce_name, ret_type, data_type) \
                                                                                                      \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {    \
    protected:                                                                                        \
        void SetUp() {}                                                                               \
        void TearDown() {}                                                                            \
    };                                                                                                \
                                                                                                      \
    namespace {                                                                                       \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                    \
        ret_type& dst, data_type src0, vector_bool mask, int32_t part, int32_t mode)                  \
    {}                                                                                                \
    }                                                                                                 \
                                                                                                      \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                           \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                 \
    {                                                                                                 \
        data_type src0;                                                                               \
        vector_bool mask;                                                                             \
                                                                                                      \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, vector_bool, int32_t, int32_t))               \
            .times(2)                                                                                 \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                   \
                                                                                                      \
        ret_type dstEven = c_api_name(src0, mask, ASC_POSITION_EVEN);                                 \
        (void)dstEven;                                                                                \
        ret_type dstOdd = c_api_name(src0, mask, ASC_POSITION_ODD);                                   \
        (void)dstOdd;                                                                                 \
        GlobalMockObject::verify();                                                                   \
    }

// ==========vcvt position-based, 6 args(sat, part, mode): mock(dst&, src, mask, int, int, int)==========
#define TEST_VECTOR_COMPUTE_VCVT_RET_SAT_INSTR(class_name, c_api_name, cce_name, ret_type, data_type) \
                                                                                                      \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {    \
    protected:                                                                                        \
        void SetUp() {}                                                                               \
        void TearDown() {}                                                                            \
    };                                                                                                \
                                                                                                      \
    namespace {                                                                                       \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                    \
        ret_type& dst, data_type src0, vector_bool mask, int32_t sat, int32_t part, int32_t mode)     \
    {}                                                                                                \
    }                                                                                                 \
                                                                                                      \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                           \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                 \
    {                                                                                                 \
        data_type src0;                                                                               \
        vector_bool mask;                                                                             \
                                                                                                      \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, vector_bool, int32_t, int32_t, int32_t))      \
            .times(2)                                                                                 \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                   \
                                                                                                      \
        ret_type dstEven = c_api_name(src0, mask, ASC_POSITION_EVEN);                                 \
        (void)dstEven;                                                                                \
        ret_type dstOdd = c_api_name(src0, mask, ASC_POSITION_ODD);                                   \
        (void)dstOdd;                                                                                 \
        GlobalMockObject::verify();                                                                   \
    }

// ==========vcvt round-based, 5 args(round, mode): mock(dst&, src, mask, int, int)==========
#define TEST_VECTOR_COMPUTE_VCVT_RET_RND_INSTR(class_name, c_api_name, cce_name, ret_type, data_type) \
                                                                                                      \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {    \
    protected:                                                                                        \
        void SetUp() {}                                                                               \
        void TearDown() {}                                                                            \
    };                                                                                                \
                                                                                                      \
    namespace {                                                                                       \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                    \
        ret_type& dst, data_type src0, vector_bool mask, int32_t round, int32_t mode)                 \
    {}                                                                                                \
    }                                                                                                 \
                                                                                                      \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                           \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                 \
    {                                                                                                 \
        data_type src0;                                                                               \
        vector_bool mask;                                                                             \
                                                                                                      \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, vector_bool, int32_t, int32_t))               \
            .times(1)                                                                                 \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                   \
                                                                                                      \
        ret_type dst = c_api_name(src0, mask);                                                        \
        (void)dst;                                                                                    \
        GlobalMockObject::verify();                                                                   \
    }

// ==========vcvt round+sat+part, 7 args(round, sat, part, mode)==========
#define TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(class_name, c_api_name, cce_name, ret_type, data_type) \
                                                                                                           \
    class TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi : public testing::Test {         \
    protected:                                                                                             \
        void SetUp() {}                                                                                    \
        void TearDown() {}                                                                                 \
    };                                                                                                     \
                                                                                                           \
    namespace {                                                                                            \
    void cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub(                                         \
        ret_type& dst, data_type src0, vector_bool mask, int32_t round, int32_t sat, int32_t part,         \
        int32_t mode)                                                                                      \
    {}                                                                                                     \
    }                                                                                                      \
                                                                                                           \
    TEST_F(TestVectorCompute##class_name##_##c_api_name##_##data_type##_##ret_type##_CApi,                                \
           c_api_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Succ)                                      \
    {                                                                                                      \
        data_type src0;                                                                                    \
        vector_bool mask;                                                                                  \
                                                                                                           \
        MOCKER_CPP(cce_name, void(ret_type&, data_type, vector_bool, int32_t, int32_t, int32_t, int32_t))  \
            .times(2)                                                                                      \
            .will(invoke(cce_name##ReturnValue##_##c_api_name##_##data_type##_##ret_type##_Stub));                        \
                                                                                                           \
        ret_type dstEven = c_api_name(src0, mask, ASC_POSITION_EVEN);                                      \
        (void)dstEven;                                                                                     \
        ret_type dstOdd = c_api_name(src0, mask, ASC_POSITION_ODD);                                        \
        (void)dstOdd;                                                                                      \
        GlobalMockObject::verify();                                                                        \
    }

// ==========asc_abs==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Abs, asc_abs, vabs, vector_int8_t, vector_int8_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Abs, asc_abs, vabs, vector_int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Abs, asc_abs, vabs, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Abs, asc_abs, vabs, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Abs, asc_abs, vabs, vector_float, vector_float);

// ==========asc_ln==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vln, asc_ln, vln, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vln, asc_ln, vln, vector_float, vector_float);

// ==========asc_relu==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vrelu, asc_relu, vrelu, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vrelu, asc_relu, vrelu, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vrelu, asc_relu, vrelu, vector_float, vector_float);

// ==========asc_reduce_max==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmax, asc_reduce_max, vcmax, vector_int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmax, asc_reduce_max, vcmax, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmax, asc_reduce_max, vcmax, vector_uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmax, asc_reduce_max, vcmax, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmax, asc_reduce_max, vcmax, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmax, asc_reduce_max, vcmax, vector_float, vector_float);

// ==========asc_reduce_min==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmin, asc_reduce_min, vcmin, vector_int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmin, asc_reduce_min, vcmin, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmin, asc_reduce_min, vcmin, vector_uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmin, asc_reduce_min, vcmin, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmin, asc_reduce_min, vcmin, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcmin, asc_reduce_min, vcmin, vector_float, vector_float);

// ==========asc_reduce_max_datablock==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmax, asc_reduce_max_datablock, vcgmax, vector_int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmax, asc_reduce_max_datablock, vcgmax, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmax, asc_reduce_max_datablock, vcgmax, vector_uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmax, asc_reduce_max_datablock, vcgmax, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmax, asc_reduce_max_datablock, vcgmax, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmax, asc_reduce_max_datablock, vcgmax, vector_float, vector_float);

// ==========asc_reduce_min_datablock==========
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmin, asc_reduce_min_datablock, vcgmin, vector_int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmin, asc_reduce_min_datablock, vcgmin, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmin, asc_reduce_min_datablock, vcgmin, vector_uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmin, asc_reduce_min_datablock, vcgmin, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmin, asc_reduce_min_datablock, vcgmin, vector_half, vector_half);
TEST_VECTOR_COMPUTE_UNARY_RET_INSTR(Vcgmin, asc_reduce_min_datablock, vcgmin, vector_float, vector_float);

// ==========asc_reduce_sum==========
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcadd, asc_reduce_sum, vcadd, vector_int32_t, vector_int16_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcadd, asc_reduce_sum, vcadd, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcadd, asc_reduce_sum, vcadd, vector_uint32_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcadd, asc_reduce_sum, vcadd, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcadd, asc_reduce_sum, vcadd, vector_half, vector_half);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcadd, asc_reduce_sum, vcadd, vector_float, vector_float);

// ==========asc_reduce_sum_datablock==========
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcgadd, asc_reduce_sum_datablock, vcgadd, vector_int32_t, vector_int16_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcgadd, asc_reduce_sum_datablock, vcgadd, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcgadd, asc_reduce_sum_datablock, vcgadd, vector_uint32_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcgadd, asc_reduce_sum_datablock, vcgadd, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcgadd, asc_reduce_sum_datablock, vcgadd, vector_half, vector_half);
TEST_VECTOR_COMPUTE_RET_REDUCE_SUM_INSTR(Vcgadd, asc_reduce_sum_datablock, vcgadd, vector_float, vector_float);

// ==========asc_abs_sub==========
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(Vabsdif, asc_abs_sub, vabsdif, vector_half, vector_half);
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(Vabsdif, asc_abs_sub, vabsdif, vector_float, vector_float);

// ==========asc_fma==========
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_half);
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_float);
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_int16_t);
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_uint16_t);
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_int32_t);
TEST_VECTOR_COMPUTE_FMA_RET_INSTR(Vmula, asc_fma, vmula, vector_uint32_t);

// ==========asc_exp_sub==========
TEST_VECTOR_COMPUTE_EXP_SUB_RET_INSTR(Vexpdif, asc_exp_sub, vexpdif, vector_float, vector_float);

// ==========asc_exp_sub_half2float==========
TEST_VECTOR_COMPUTE_EXP_SUB_H2F_RET_INSTR(Vexpdif, asc_exp_sub_half2float, vexpdif, vector_float, vector_half);

// ==========asc_select==========
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Psel, asc_select, psel, vector_bool, vector_bool);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_uint8_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_int8_t, vector_int8_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_fp8_e4m3fn_t, vector_fp8_e4m3fn_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_fp8_e5m2_t, vector_fp8_e5m2_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_half, vector_half);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_bfloat16_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_SELECT_RET_INSTR(Vsel, asc_select, vsel, vector_float, vector_float);

class TestVectorComputeVsel_hifloat8_CApi : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void vselReturnValue_vector_hifloat8_Stub(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{}
}

TEST_F(TestVectorComputeVsel_hifloat8_CApi, asc_selectReturnValue_vector_hifloat8_Succ)
{
    vector_hifloat8_t src0;
    vector_hifloat8_t src1;
    vector_bool mask;

    MOCKER_CPP(vsel, void(vector_uint8_t&, vector_uint8_t, vector_uint8_t, vector_bool))
        .times(1)
        .will(invoke(vselReturnValue_vector_hifloat8_Stub));

    vector_hifloat8_t dst = asc_select(src0, src1, mask);
    (void)dst;
    GlobalMockObject::verify();
}

// ==========asc_add_scalar==========
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_int8_t, int8_t, vector_int8_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_uint8_t, uint8_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_int16_t, int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_uint16_t, uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_half, half, vector_half);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_bfloat16_t, bfloat16_t, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_int32_t, int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_uint32_t, uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vadds, asc_add_scalar, vadds, vector_float, float, vector_float);

// ==========asc_mul_scalar==========
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmuls, asc_mul_scalar, vmuls, vector_int16_t, int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmuls, asc_mul_scalar, vmuls, vector_uint16_t, uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmuls, asc_mul_scalar, vmuls, vector_half, half, vector_half);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmuls, asc_mul_scalar, vmuls, vector_int32_t, int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmuls, asc_mul_scalar, vmuls, vector_uint32_t, uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmuls, asc_mul_scalar, vmuls, vector_float, float, vector_float);

// ==========asc_max_scalar==========
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_int8_t, int8_t, vector_int8_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_int16_t, int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_int32_t, int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_uint8_t, uint8_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_uint16_t, uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_uint32_t, uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_half, half, vector_half);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_float, float, vector_float);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmaxs, asc_max_scalar, vmaxs, vector_bfloat16_t, bfloat16_t, vector_bfloat16_t);

// ==========asc_min_scalar==========
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_int8_t, int8_t, vector_int8_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_int16_t, int16_t, vector_int16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_int32_t, int32_t, vector_int32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_uint8_t, uint8_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_uint16_t, uint16_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_uint32_t, uint32_t, vector_uint32_t);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_half, half, vector_half);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_float, float, vector_float);
TEST_VECTOR_COMPUTE_SCALAR_RET_INSTR(Vmins, asc_min_scalar, vmins, vector_bfloat16_t, bfloat16_t, vector_bfloat16_t);

// ==========asc_ge_scalar==========
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_int8_t, int8_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_uint8_t, uint8_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_int16_t, int16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_uint16_t, uint16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_int32_t, int32_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_uint32_t, uint32_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_half, half);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_bfloat16_t, bfloat16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ge, asc_ge_scalar, vcmps_ge, vector_float, float);

// ==========asc_gt_scalar==========
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_int8_t, int8_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_uint8_t, uint8_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_int16_t, int16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_uint16_t, uint16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_int32_t, int32_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_uint32_t, uint32_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_half, half);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_bfloat16_t, bfloat16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_gt, asc_gt_scalar, vcmps_gt, vector_float, float);

// ==========asc_ne_scalar==========
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_int8_t, int8_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_uint8_t, uint8_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_int16_t, int16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_uint16_t, uint16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_int32_t, int32_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_uint32_t, uint32_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_half, half);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_bfloat16_t, bfloat16_t);
TEST_VECTOR_COMPUTE_VCMPS_RET_INSTR(Vcmps_ne, asc_ne_scalar, vcmps_ne, vector_float, float);

// ==========asc_uint82uint16==========
TEST_VECTOR_COMPUTE_VCVT_RET_PART_INSTR(Vcvt, asc_uint82uint16, vcvt, vector_uint16_t, vector_uint8_t);

// ==========asc_int322int64==========
TEST_VECTOR_COMPUTE_VCVT_RET_PART_INSTR(Vcvt, asc_int322int64, vcvt, vector_int64_t, vector_int32_t);

// ==========asc_int322int16 / asc_int322int16_sat==========
TEST_VECTOR_COMPUTE_VCVT_RET_SAT_INSTR(Vcvt, asc_int322int16, vcvt, vector_int16_t, vector_int32_t);
TEST_VECTOR_COMPUTE_VCVT_RET_SAT_INSTR(Vcvt, asc_int322int16_sat, vcvt, vector_int16_t, vector_int32_t);

// ==========asc_int322uint16 / asc_int322uint16_sat==========
TEST_VECTOR_COMPUTE_VCVT_RET_SAT_INSTR(Vcvt, asc_int322uint16, vcvt, vector_uint16_t, vector_int32_t);
TEST_VECTOR_COMPUTE_VCVT_RET_SAT_INSTR(Vcvt, asc_int322uint16_sat, vcvt, vector_uint16_t, vector_int32_t);

// ==========asc_int322float==========
TEST_VECTOR_COMPUTE_VCVT_RET_RND_INSTR(Vcvt, asc_int322float_rd, vcvt, vector_float, vector_int32_t);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_INSTR(Vcvt, asc_int322float_ru, vcvt, vector_float, vector_int32_t);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_INSTR(Vcvt, asc_int322float_rz, vcvt, vector_float, vector_int32_t);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_INSTR(Vcvt, asc_int322float_rn, vcvt, vector_float, vector_int32_t);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_INSTR(Vcvt, asc_int322float_rna, vcvt, vector_float, vector_int32_t);

// ==========asc_float2bfloat16==========
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rd, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rd_sat, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rn, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rn_sat, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rna, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rna_sat, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_ru, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_ru_sat, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rz, vcvt, vector_bfloat16_t, vector_float);
TEST_VECTOR_COMPUTE_VCVT_RET_RND_SAT_INSTR(Vcvt, asc_float2bfloat16_rz_sat, vcvt, vector_bfloat16_t, vector_float);
