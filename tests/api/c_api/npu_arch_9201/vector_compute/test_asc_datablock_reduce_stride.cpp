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

#define TEST_DATABLOCK_REDUCE_INSTR(class_name, c_api_name, cce_name, data_type)           \
                                                                                           \
    class Test##class_name##_##data_type##_CApi : public testing::Test {                   \
    protected:                                                                             \
        void SetUp() {}                                                                    \
        void TearDown() {}                                                                 \
    };                                                                                     \
                                                                                           \
    namespace {                                                                            \
    void cce_name##_##data_type##_Stub(data_type& dst, data_type src, vector_bool mask) {} \
    }                                                                                      \
                                                                                           \
    TEST_F(Test##class_name##_##data_type##_CApi, c_api_name##_##data_type##_Succ)         \
    {                                                                                      \
        data_type dst;                                                                     \
        data_type src;                                                                     \
        vector_bool mask;                                                                  \
                                                                                           \
        MOCKER_CPP(cce_name, void(data_type&, data_type, vector_bool))                     \
            .times(1)                                                                      \
            .will(invoke(cce_name##_##data_type##_Stub));                                  \
                                                                                           \
        c_api_name(dst, src, mask);                                                        \
        GlobalMockObject::verify();                                                        \
    }

#define TEST_DATABLOCK_REDUCE_RETURN_INSTR(class_name, c_api_name, cce_name, data_type)          \
                                                                                                 \
    namespace {                                                                                  \
    void cce_name##_##data_type##_ReturnStub(data_type& dst, data_type src, vector_bool mask) {} \
    }                                                                                            \
                                                                                                 \
    TEST(Test##class_name##Return_##data_type##_CApi, c_api_name##_return_##data_type##_Succ)    \
    {                                                                                            \
        data_type src;                                                                           \
        vector_bool mask;                                                                        \
                                                                                                 \
        MOCKER_CPP(cce_name, void(data_type&, data_type, vector_bool))                           \
            .times(1)                                                                            \
            .will(invoke(cce_name##_##data_type##_ReturnStub));                                  \
                                                                                                 \
        data_type result = c_api_name(src, mask);                                                \
        (void)result;                                                                            \
        GlobalMockObject::verify();                                                              \
    }

// ==========asc_datablock_reduce_max_stride==========
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_half);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_float);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_uint16_t);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_int16_t);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_uint32_t);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_int32_t);

TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_half);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_float);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(
    DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_uint16_t);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_int16_t);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(
    DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_uint32_t);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMaxStride, asc_datablock_reduce_max_stride, vcgmaxv2, vector_int32_t);

// ==========asc_datablock_reduce_min_stride==========
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_half);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_float);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_uint16_t);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_int16_t);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_uint32_t);
TEST_DATABLOCK_REDUCE_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_int32_t);

TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_half);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_float);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(
    DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_uint16_t);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_int16_t);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(
    DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_uint32_t);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(DatablockReduceMinStride, asc_datablock_reduce_min_stride, vcgminv2, vector_int32_t);

// ==========asc_pair_reduce_sum_stride==========
TEST_DATABLOCK_REDUCE_INSTR(PairReduceSumStride, asc_pair_reduce_sum_stride, vcpaddv2, vector_half);
TEST_DATABLOCK_REDUCE_INSTR(PairReduceSumStride, asc_pair_reduce_sum_stride, vcpaddv2, vector_float);

TEST_DATABLOCK_REDUCE_RETURN_INSTR(PairReduceSumStride, asc_pair_reduce_sum_stride, vcpaddv2, vector_half);
TEST_DATABLOCK_REDUCE_RETURN_INSTR(PairReduceSumStride, asc_pair_reduce_sum_stride, vcpaddv2, vector_float);
