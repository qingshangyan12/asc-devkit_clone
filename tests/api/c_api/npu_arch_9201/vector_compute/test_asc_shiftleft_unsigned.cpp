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

#define TEST_SHIFTLEFT_UNSIGNED_INSTR(data_type, shift_type)                                                    \
                                                                                                                \
    class TestShiftLeftUnsigned##data_type##shift_type##CApi : public testing::Test {                           \
    protected:                                                                                                  \
        void SetUp() {}                                                                                         \
        void TearDown() {}                                                                                      \
    };                                                                                                          \
                                                                                                                \
    namespace {                                                                                                 \
    void vshl##_##data_type##_##shift_type##_UnsignedStub(                                                      \
        data_type& dst, data_type src0, shift_type src1, vector_bool mask, Literal mode)                        \
    {}                                                                                                          \
    }                                                                                                           \
                                                                                                                \
    TEST_F(TestShiftLeftUnsigned##data_type##shift_type##CApi, asc_shiftleft_##data_type##_##shift_type##_Succ) \
    {                                                                                                           \
        data_type dst;                                                                                          \
        data_type src0;                                                                                         \
        shift_type src1;                                                                                        \
        vector_bool mask;                                                                                       \
                                                                                                                \
        MOCKER_CPP(vshl, void(data_type&, data_type, shift_type, vector_bool, Literal))                         \
            .times(1)                                                                                           \
            .will(invoke(vshl##_##data_type##_##shift_type##_UnsignedStub));                                    \
                                                                                                                \
        asc_shiftleft(dst, src0, src1, mask);                                                                   \
        GlobalMockObject::verify();                                                                             \
    }

#define TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(data_type, shift_type)                                                    \
                                                                                                                       \
    namespace {                                                                                                        \
    void vshl##_##data_type##_##shift_type##_UnsignedRetStub(                                                          \
        data_type& dst, data_type src0, shift_type src1, vector_bool mask, Literal mode)                               \
    {}                                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    TEST(TestShiftLeftUnsignedReturn##data_type##shift_type##CApi, asc_shiftleft_##data_type##_##shift_type##_RetSucc) \
    {                                                                                                                  \
        data_type src0;                                                                                                \
        shift_type src1;                                                                                               \
        vector_bool mask;                                                                                              \
                                                                                                                       \
        MOCKER_CPP(vshl, void(data_type&, data_type, shift_type, vector_bool, Literal))                                \
            .times(1)                                                                                                  \
            .will(invoke(vshl##_##data_type##_##shift_type##_UnsignedRetStub));                                        \
                                                                                                                       \
        data_type result = asc_shiftleft(src0, src1, mask);                                                            \
        (void)result;                                                                                                  \
        GlobalMockObject::verify();                                                                                    \
    }

TEST_SHIFTLEFT_UNSIGNED_INSTR(vector_uint8_t, vector_uint8_t);
TEST_SHIFTLEFT_UNSIGNED_INSTR(vector_int8_t, vector_uint8_t);
TEST_SHIFTLEFT_UNSIGNED_INSTR(vector_uint16_t, vector_uint16_t);
TEST_SHIFTLEFT_UNSIGNED_INSTR(vector_int16_t, vector_uint16_t);
TEST_SHIFTLEFT_UNSIGNED_INSTR(vector_uint32_t, vector_uint32_t);
TEST_SHIFTLEFT_UNSIGNED_INSTR(vector_int32_t, vector_uint32_t);

TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(vector_uint8_t, vector_uint8_t);
TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(vector_int8_t, vector_uint8_t);
TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(vector_uint16_t, vector_uint16_t);
TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(vector_int16_t, vector_uint16_t);
TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(vector_uint32_t, vector_uint32_t);
TEST_SHIFTLEFT_UNSIGNED_RETURN_INSTR(vector_int32_t, vector_uint32_t);
