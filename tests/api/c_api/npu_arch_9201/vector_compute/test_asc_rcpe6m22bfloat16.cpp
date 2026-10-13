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
#include "include/c_api/reg_compute/reg_convert.h"

#define TEST_RCPE6M22BFLOAT16(class_name, c_api_name, position_name, position_value)                                   \
    class TestRcpe6m22Bfloat16##class_name##_##position_name : public testing::Test {};                                \
                                                                                                                       \
    namespace {                                                                                                        \
    void vcvt_rcpe6m22bf16_##class_name##_##position_name(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal) \
    {}                                                                                                                 \
                                                                                                                       \
    TEST_F(TestRcpe6m22Bfloat16##class_name##_##position_name, InvokeVcvt)                                             \
    {                                                                                                                  \
        vector_bfloat16_t dst;                                                                                         \
        vector_fp8_e6m2_t src;                                                                                         \
        vector_bool mask;                                                                                              \
        MOCKER_CPP(vcvt_rcpe6m22bf16, void(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal))               \
            .times(1)                                                                                                  \
            .will(invoke(vcvt_rcpe6m22bf16_##class_name##_##position_name));                                           \
        c_api_name(dst, src, mask, position_value);                                                                    \
        GlobalMockObject::verify();                                                                                    \
    }                                                                                                                  \
    }

#define TEST_RCPE6M22BFLOAT16_RETURN(class_name, c_api_name, position_name, position_value)              \
    class TestRcpe6m22Bfloat16Return##class_name##_##position_name : public testing::Test {};            \
                                                                                                         \
    namespace {                                                                                          \
    void vcvt_rcpe6m22bf16_return_##class_name##_##position_name(                                        \
        vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal)                                     \
    {}                                                                                                   \
                                                                                                         \
    TEST_F(TestRcpe6m22Bfloat16Return##class_name##_##position_name, InvokeVcvt)                         \
    {                                                                                                    \
        vector_fp8_e6m2_t src;                                                                           \
        vector_bool mask;                                                                                \
        MOCKER_CPP(vcvt_rcpe6m22bf16, void(vector_bfloat16_t&, vector_fp8_e6m2_t, vector_bool, Literal)) \
            .times(1)                                                                                    \
            .will(invoke(vcvt_rcpe6m22bf16_return_##class_name##_##position_name));                      \
        c_api_name(src, mask, position_value);                                                           \
        GlobalMockObject::verify();                                                                      \
    }                                                                                                    \
    }

TEST_RCPE6M22BFLOAT16(Even, asc_rcpe6m22bfloat16, Even, ASC_POSITION_EVEN)
TEST_RCPE6M22BFLOAT16(Odd, asc_rcpe6m22bfloat16, Odd, ASC_POSITION_ODD)
TEST_RCPE6M22BFLOAT16_RETURN(Even, asc_rcpe6m22bfloat16, Even, ASC_POSITION_EVEN)
TEST_RCPE6M22BFLOAT16_RETURN(Odd, asc_rcpe6m22bfloat16, Odd, ASC_POSITION_ODD)
