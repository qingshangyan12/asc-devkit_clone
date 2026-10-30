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

#define TEST_LAST_ELEM_REDUCE_INSTR(class_name, c_api_name, cce_name, data_type)                                \
                                                                                                                \
    class Test##class_name##_##data_type##_CApi : public testing::Test {                                        \
    protected:                                                                                                  \
        void SetUp() {}                                                                                         \
        void TearDown() {}                                                                                      \
    };                                                                                                          \
                                                                                                                \
    namespace {                                                                                                 \
    void cce_name##_##data_type##_Stub(data_type& dst, data_type src) {}                                        \
    }                                                                                                           \
                                                                                                                \
    TEST_F(Test##class_name##_##data_type##_CApi, c_api_name##_##data_type##_Succ)                              \
    {                                                                                                           \
        data_type dst;                                                                                          \
        data_type src;                                                                                          \
                                                                                                                \
        MOCKER_CPP(cce_name, void(data_type&, data_type)).times(1).will(invoke(cce_name##_##data_type##_Stub)); \
                                                                                                                \
        c_api_name(dst, src);                                                                                   \
        GlobalMockObject::verify();                                                                             \
    }

#define TEST_LAST_ELEM_REDUCE_RETURN_INSTR(class_name, c_api_name, cce_name, data_type)                               \
                                                                                                                      \
    namespace {                                                                                                       \
    void cce_name##_##data_type##_ReturnStub(data_type& dst, data_type src) {}                                        \
    }                                                                                                                 \
                                                                                                                      \
    TEST(Test##class_name##Return_##data_type##_CApi, c_api_name##_return_##data_type##_Succ)                         \
    {                                                                                                                 \
        data_type src;                                                                                                \
                                                                                                                      \
        MOCKER_CPP(cce_name, void(data_type&, data_type)).times(1).will(invoke(cce_name##_##data_type##_ReturnStub)); \
                                                                                                                      \
        data_type result = c_api_name(src);                                                                           \
        (void)result;                                                                                                 \
        GlobalMockObject::verify();                                                                                   \
    }

// ==========asc_reduce_max_last_elem==========
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_half);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_float);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_uint16_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_int16_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_uint32_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_int32_t);

TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_half);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_float);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_uint16_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_int16_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_uint32_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMaxLastElem, asc_reduce_max_last_elem, vcmaxv2, vector_int32_t);

// ==========asc_reduce_min_last_elem==========
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_half);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_float);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_uint16_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_int16_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_uint32_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_int32_t);

TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_half);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_float);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_uint16_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_int16_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_uint32_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceMinLastElem, asc_reduce_min_last_elem, vcminv2, vector_int32_t);

// ==========asc_reduce_sum_last_elem==========
TEST_LAST_ELEM_REDUCE_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_int32_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_uint32_t);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_half);
TEST_LAST_ELEM_REDUCE_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_float);

TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_int32_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_uint32_t);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_half);
TEST_LAST_ELEM_REDUCE_RETURN_INSTR(ReduceSumLastElem, asc_reduce_sum_last_elem, vcaddv2, vector_float);
