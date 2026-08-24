/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_3510/utils/test_binary_instr_utils.h"

#define TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(class_name, c_api_name, cce_name, data_type)                              \
                                                                                                                       \
    class TestVectorCompute##class_name##_##data_type##_RetCApi : public testing::Test {                              \
    protected:                                                                                                        \
        void SetUp() {}                                                                                               \
        void TearDown() {}                                                                                            \
    };                                                                                                                \
                                                                                                                       \
    namespace {                                                                                                      \
    void cce_name##_##data_type##_RetStub(data_type& dst, data_type src0, data_type src1, vector_bool mask, Literal mode) \
    {}                                                                                                                \
    }                                                                                                                 \
                                                                                                                       \
    TEST_F(TestVectorCompute##class_name##_##data_type##_RetCApi, c_api_name##_##data_type##_RetSucc)                 \
    {                                                                                                                 \
        data_type src0;                                                                                               \
        data_type src1;                                                                                               \
        vector_bool mask;                                                                                             \
                                                                                                                       \
        MOCKER_CPP(cce_name, void(data_type&, data_type, data_type, vector_bool, Literal))                            \
            .times(1)                                                                                                 \
            .will(invoke(cce_name##_##data_type##_RetStub));                                                          \
                                                                                                                       \
        data_type result = c_api_name(src0, src1, mask);                                                             \
        GlobalMockObject::verify();                                                                                   \
    }

#define TEST_VECTOR_COMPUTE_PXOR_RET_INSTR(class_name, c_api_name, cce_name, data_type)                               \
                                                                                                                       \
    class TestVectorCompute##class_name##_##data_type##_RetCApi : public testing::Test {                             \
    protected:                                                                                                        \
        void SetUp() {}                                                                                               \
        void TearDown() {}                                                                                            \
    };                                                                                                                \
                                                                                                                       \
    namespace {                                                                                                      \
    void cce_name##_##data_type##_RetStub(data_type& dst, data_type src0, data_type src1, vector_bool mask) {}        \
    }                                                                                                                 \
                                                                                                                       \
    TEST_F(TestVectorCompute##class_name##_##data_type##_RetCApi, c_api_name##_##data_type##_RetSucc)                 \
    {                                                                                                                 \
        data_type src0;                                                                                               \
        data_type src1;                                                                                               \
        vector_bool mask;                                                                                             \
                                                                                                                       \
        MOCKER_CPP(cce_name, void(data_type&, data_type, data_type, vector_bool))                                     \
            .times(1)                                                                                                 \
            .will(invoke(cce_name##_##data_type##_RetStub));                                                         \
                                                                                                                       \
        data_type result = c_api_name(src0, src1, mask);                                                             \
        GlobalMockObject::verify();                                                                                   \
    }

TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(VxorRet, asc_xor, vxor, vector_int32_t);
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(VxorRet, asc_xor, vxor, vector_uint32_t);
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(VxorRet, asc_xor, vxor, vector_uint16_t);
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(VxorRet, asc_xor, vxor, vector_int16_t);
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(VxorRet, asc_xor, vxor, vector_uint8_t);
TEST_VECTOR_COMPUTE_BINARY_RET_INSTR(VxorRet, asc_xor, vxor, vector_int8_t);
TEST_VECTOR_COMPUTE_PXOR_RET_INSTR(PxorRet, asc_xor, pxor, vector_bool);
