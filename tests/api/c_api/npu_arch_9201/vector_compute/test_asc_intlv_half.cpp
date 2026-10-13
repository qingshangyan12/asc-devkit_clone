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

#define TEST_INTLV_HALF_LOW(data_type)                                                                    \
                                                                                                          \
    class TestIntlvHalfLow##data_type##CApi : public testing::Test {                                      \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void vintlvv2##_##data_type##_LowStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestIntlvHalfLow##data_type##CApi, asc_intlv_half_low_##data_type##_Succ)                      \
    {                                                                                                     \
        data_type dst;                                                                                    \
        data_type src0;                                                                                   \
        data_type src1;                                                                                   \
                                                                                                          \
        MOCKER_CPP(vintlvv2, void(data_type&, data_type, data_type, Literal))                             \
            .times(1)                                                                                     \
            .will(invoke(vintlvv2##_##data_type##_LowStub));                                              \
                                                                                                          \
        asc_intlv_half_low(dst, src0, src1);                                                              \
        GlobalMockObject::verify();                                                                       \
    }

#define TEST_INTLV_HALF_HIGH(data_type)                                                                    \
                                                                                                           \
    class TestIntlvHalfHigh##data_type##CApi : public testing::Test {                                      \
    protected:                                                                                             \
        void SetUp() {}                                                                                    \
        void TearDown() {}                                                                                 \
    };                                                                                                     \
                                                                                                           \
    namespace {                                                                                            \
    void vintlvv2##_##data_type##_HighStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                      \
                                                                                                           \
    TEST_F(TestIntlvHalfHigh##data_type##CApi, asc_intlv_half_high_##data_type##_Succ)                     \
    {                                                                                                      \
        data_type dst;                                                                                     \
        data_type src0;                                                                                    \
        data_type src1;                                                                                    \
                                                                                                           \
        MOCKER_CPP(vintlvv2, void(data_type&, data_type, data_type, Literal))                              \
            .times(1)                                                                                      \
            .will(invoke(vintlvv2##_##data_type##_HighStub));                                              \
                                                                                                           \
        asc_intlv_half_high(dst, src0, src1);                                                              \
        GlobalMockObject::verify();                                                                        \
    }

#define TEST_INTLV_HALF_LOW_RETURN(data_type)                                                                \
                                                                                                             \
    namespace {                                                                                              \
    void vintlvv2##_##data_type##_LowRetStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                        \
                                                                                                             \
    TEST(TestIntlvHalfLowReturn##data_type##CApi, asc_intlv_half_low_##data_type##_RetSucc)                  \
    {                                                                                                        \
        data_type src0;                                                                                      \
        data_type src1;                                                                                      \
                                                                                                             \
        MOCKER_CPP(vintlvv2, void(data_type&, data_type, data_type, Literal))                                \
            .times(1)                                                                                        \
            .will(invoke(vintlvv2##_##data_type##_LowRetStub));                                              \
                                                                                                             \
        data_type result = asc_intlv_half_low(src0, src1);                                                   \
        (void)result;                                                                                        \
        GlobalMockObject::verify();                                                                          \
    }

#define TEST_INTLV_HALF_HIGH_RETURN(data_type)                                                                \
                                                                                                              \
    namespace {                                                                                               \
    void vintlvv2##_##data_type##_HighRetStub(data_type& dst, data_type src0, data_type src1, Literal pos) {} \
    }                                                                                                         \
                                                                                                              \
    TEST(TestIntlvHalfHighReturn##data_type##CApi, asc_intlv_half_high_##data_type##_RetSucc)                 \
    {                                                                                                         \
        data_type src0;                                                                                       \
        data_type src1;                                                                                       \
                                                                                                              \
        MOCKER_CPP(vintlvv2, void(data_type&, data_type, data_type, Literal))                                 \
            .times(1)                                                                                         \
            .will(invoke(vintlvv2##_##data_type##_HighRetStub));                                              \
                                                                                                              \
        data_type result = asc_intlv_half_high(src0, src1);                                                   \
        (void)result;                                                                                         \
        GlobalMockObject::verify();                                                                           \
    }

TEST_INTLV_HALF_LOW(vector_uint8_t);
TEST_INTLV_HALF_LOW(vector_int8_t);
TEST_INTLV_HALF_LOW(vector_uint16_t);
TEST_INTLV_HALF_LOW(vector_int16_t);
TEST_INTLV_HALF_LOW(vector_uint32_t);
TEST_INTLV_HALF_LOW(vector_int32_t);
TEST_INTLV_HALF_LOW(vector_half);
TEST_INTLV_HALF_LOW(vector_float);

TEST_INTLV_HALF_HIGH(vector_uint8_t);
TEST_INTLV_HALF_HIGH(vector_int8_t);
TEST_INTLV_HALF_HIGH(vector_uint16_t);
TEST_INTLV_HALF_HIGH(vector_int16_t);
TEST_INTLV_HALF_HIGH(vector_uint32_t);
TEST_INTLV_HALF_HIGH(vector_int32_t);
TEST_INTLV_HALF_HIGH(vector_half);
TEST_INTLV_HALF_HIGH(vector_float);

TEST_INTLV_HALF_LOW_RETURN(vector_uint8_t);
TEST_INTLV_HALF_LOW_RETURN(vector_int8_t);
TEST_INTLV_HALF_LOW_RETURN(vector_uint16_t);
TEST_INTLV_HALF_LOW_RETURN(vector_int16_t);
TEST_INTLV_HALF_LOW_RETURN(vector_uint32_t);
TEST_INTLV_HALF_LOW_RETURN(vector_int32_t);
TEST_INTLV_HALF_LOW_RETURN(vector_half);
TEST_INTLV_HALF_LOW_RETURN(vector_float);

TEST_INTLV_HALF_HIGH_RETURN(vector_uint8_t);
TEST_INTLV_HALF_HIGH_RETURN(vector_int8_t);
TEST_INTLV_HALF_HIGH_RETURN(vector_uint16_t);
TEST_INTLV_HALF_HIGH_RETURN(vector_int16_t);
TEST_INTLV_HALF_HIGH_RETURN(vector_uint32_t);
TEST_INTLV_HALF_HIGH_RETURN(vector_int32_t);
TEST_INTLV_HALF_HIGH_RETURN(vector_half);
TEST_INTLV_HALF_HIGH_RETURN(vector_float);
