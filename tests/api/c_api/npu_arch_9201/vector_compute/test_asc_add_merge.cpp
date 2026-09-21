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

#define TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(class_name, c_api_name, cce_name, data_type)               \
                                                                                                          \
    class TestVectorCompute##class_name##_##data_type##_MergeCApi : public testing::Test {                \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void cce_name##_merging_##data_type##_Stub(                                                           \
        data_type& dst, data_type src0, data_type src1, vector_bool mask, Literal mode)                   \
    {                                                                                                     \
        EXPECT_EQ(mode, MODE_MERGING);                                                                    \
    }                                                                                                     \
    }                                                                                                     \
                                                                                                          \
    TEST_F(TestVectorCompute##class_name##_##data_type##_MergeCApi, c_api_name##_##data_type##_MergeSucc) \
    {                                                                                                     \
        data_type dst;                                                                                    \
        data_type src0;                                                                                   \
        data_type src1;                                                                                   \
        vector_bool mask;                                                                                 \
                                                                                                          \
        MOCKER_CPP(cce_name, void(data_type&, data_type, data_type, vector_bool, Literal))                \
            .times(1)                                                                                     \
            .will(invoke(cce_name##_merging_##data_type##_Stub));                                         \
                                                                                                          \
        c_api_name(dst, src0, src1, mask);                                                                \
        GlobalMockObject::verify();                                                                       \
    }

TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_uint8_t);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_int8_t);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_uint16_t);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_int16_t);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_half);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_float);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_int32_t);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_uint32_t);
TEST_VECTOR_COMPUTE_BINARY_MERGE_INSTR(VAddMerge, asc_add_merge, vadd, vector_bfloat16_t);
