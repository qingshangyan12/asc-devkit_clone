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
#include "c_api/asc_simd.h"

#define TEST_DEINTLV_EVEN(data_type)                                                                        \
                                                                                                            \
    class TestDeintlvEven##data_type##CApi : public testing::Test {                                         \
    protected:                                                                                              \
        void SetUp() {}                                                                                     \
        void TearDown() {}                                                                                  \
    };                                                                                                      \
                                                                                                            \
    namespace {                                                                                             \
    void vdintlvv2##_##data_type##_EvenStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                       \
                                                                                                            \
    TEST_F(TestDeintlvEven##data_type##CApi, asc_deintlv_even_##data_type##_Succ)                           \
    {                                                                                                       \
        data_type dst;                                                                                      \
        data_type src0;                                                                                     \
        data_type src1;                                                                                     \
                                                                                                            \
        MOCKER_CPP(vdintlvv2, void(data_type&, data_type, data_type, Literal))                              \
            .times(1)                                                                                       \
            .will(invoke(vdintlvv2##_##data_type##_EvenStub));                                              \
                                                                                                            \
        asc_deintlv_even(dst, src0, src1);                                                                  \
        GlobalMockObject::verify();                                                                         \
    }

#define TEST_DEINTLV_ODD(data_type)                                                                        \
                                                                                                           \
    class TestDeintlvOdd##data_type##CApi : public testing::Test {                                         \
    protected:                                                                                             \
        void SetUp() {}                                                                                    \
        void TearDown() {}                                                                                 \
    };                                                                                                     \
                                                                                                           \
    namespace {                                                                                            \
    void vdintlvv2##_##data_type##_OddStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                      \
                                                                                                           \
    TEST_F(TestDeintlvOdd##data_type##CApi, asc_deintlv_odd_##data_type##_Succ)                            \
    {                                                                                                      \
        data_type dst;                                                                                     \
        data_type src0;                                                                                    \
        data_type src1;                                                                                    \
                                                                                                           \
        MOCKER_CPP(vdintlvv2, void(data_type&, data_type, data_type, Literal))                             \
            .times(1)                                                                                      \
            .will(invoke(vdintlvv2##_##data_type##_OddStub));                                              \
                                                                                                           \
        asc_deintlv_odd(dst, src0, src1);                                                                  \
        GlobalMockObject::verify();                                                                        \
    }

#define TEST_DEINTLV_EVEN_RETURN(data_type)                                                                    \
                                                                                                               \
    namespace {                                                                                                \
    void vdintlvv2##_##data_type##_EvenRetStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                          \
                                                                                                               \
    TEST(TestDeintlvEvenReturn##data_type##CApi, asc_deintlv_even_##data_type##_RetSucc)                       \
    {                                                                                                          \
        data_type src0;                                                                                        \
        data_type src1;                                                                                        \
                                                                                                               \
        MOCKER_CPP(vdintlvv2, void(data_type&, data_type, data_type, Literal))                                 \
            .times(1)                                                                                          \
            .will(invoke(vdintlvv2##_##data_type##_EvenRetStub));                                              \
                                                                                                               \
        data_type result = asc_deintlv_even(src0, src1);                                                       \
        (void)result;                                                                                          \
        GlobalMockObject::verify();                                                                            \
    }

#define TEST_DEINTLV_ODD_RETURN(data_type)                                                                    \
                                                                                                              \
    namespace {                                                                                               \
    void vdintlvv2##_##data_type##_OddRetStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                         \
                                                                                                              \
    TEST(TestDeintlvOddReturn##data_type##CApi, asc_deintlv_odd_##data_type##_RetSucc)                        \
    {                                                                                                         \
        data_type src0;                                                                                       \
        data_type src1;                                                                                       \
                                                                                                              \
        MOCKER_CPP(vdintlvv2, void(data_type&, data_type, data_type, Literal))                                \
            .times(1)                                                                                         \
            .will(invoke(vdintlvv2##_##data_type##_OddRetStub));                                              \
                                                                                                              \
        data_type result = asc_deintlv_odd(src0, src1);                                                       \
        (void)result;                                                                                         \
        GlobalMockObject::verify();                                                                           \
    }

TEST_DEINTLV_EVEN(vector_uint8_t);
TEST_DEINTLV_EVEN(vector_int8_t);
TEST_DEINTLV_EVEN(vector_uint16_t);
TEST_DEINTLV_EVEN(vector_int16_t);
TEST_DEINTLV_EVEN(vector_uint32_t);
TEST_DEINTLV_EVEN(vector_int32_t);
TEST_DEINTLV_EVEN(vector_half);
TEST_DEINTLV_EVEN(vector_float);

TEST_DEINTLV_ODD(vector_uint8_t);
TEST_DEINTLV_ODD(vector_int8_t);
TEST_DEINTLV_ODD(vector_uint16_t);
TEST_DEINTLV_ODD(vector_int16_t);
TEST_DEINTLV_ODD(vector_uint32_t);
TEST_DEINTLV_ODD(vector_int32_t);
TEST_DEINTLV_ODD(vector_half);
TEST_DEINTLV_ODD(vector_float);

TEST_DEINTLV_EVEN_RETURN(vector_uint8_t);
TEST_DEINTLV_EVEN_RETURN(vector_int8_t);
TEST_DEINTLV_EVEN_RETURN(vector_uint16_t);
TEST_DEINTLV_EVEN_RETURN(vector_int16_t);
TEST_DEINTLV_EVEN_RETURN(vector_uint32_t);
TEST_DEINTLV_EVEN_RETURN(vector_int32_t);
TEST_DEINTLV_EVEN_RETURN(vector_half);
TEST_DEINTLV_EVEN_RETURN(vector_float);

TEST_DEINTLV_ODD_RETURN(vector_uint8_t);
TEST_DEINTLV_ODD_RETURN(vector_int8_t);
TEST_DEINTLV_ODD_RETURN(vector_uint16_t);
TEST_DEINTLV_ODD_RETURN(vector_int16_t);
TEST_DEINTLV_ODD_RETURN(vector_uint32_t);
TEST_DEINTLV_ODD_RETURN(vector_int32_t);
TEST_DEINTLV_ODD_RETURN(vector_half);
TEST_DEINTLV_ODD_RETURN(vector_float);
