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

#define TEST_ASC_FMA_INSTR(data_type)                                                                                 \
    namespace {                                                                                                      \
    void vmula_##data_type##_Stub(data_type& dst, data_type src0, data_type src1, vector_bool mask, Literal mode)   \
    {}                                                                                                               \
    }                                                                                                                \
                                                                                                                     \
    TEST(TestVectorComputeAscFma, asc_fma_##data_type##_Succ)                                                       \
    {                                                                                                                \
        data_type src0;                                                                                              \
        data_type src1;                                                                                              \
        data_type src2;                                                                                              \
        vector_bool mask;                                                                                            \
                                                                                                                     \
        MOCKER_CPP(vmula, void(data_type&, data_type, data_type, vector_bool, Literal))                             \
            .times(1)                                                                                                \
            .will(invoke(vmula_##data_type##_Stub));                                                                 \
                                                                                                                     \
        data_type dst = asc_fma(src0, src1, src2, mask);                                                            \
        (void)dst;                                                                                                   \
        GlobalMockObject::verify();                                                                                  \
    }

TEST_ASC_FMA_INSTR(vector_int8_t);
TEST_ASC_FMA_INSTR(vector_uint8_t);
TEST_ASC_FMA_INSTR(vector_int16_t);
TEST_ASC_FMA_INSTR(vector_uint16_t);
TEST_ASC_FMA_INSTR(vector_half);
TEST_ASC_FMA_INSTR(vector_bfloat16_t);
TEST_ASC_FMA_INSTR(vector_int32_t);
TEST_ASC_FMA_INSTR(vector_uint32_t);
TEST_ASC_FMA_INSTR(vector_float);

#undef TEST_ASC_FMA_INSTR
