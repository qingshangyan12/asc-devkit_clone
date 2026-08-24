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

#define TEST_VECTOR_COMPUTE_UNPACK_RET(class_name, c_api_name, cce_name, dst_type, src_type)                  \
                                                                                                             \
    class TestVectorCompute##class_name##dst_type##src_type##RetCApi : public testing::Test {                \
    protected:                                                                                               \
        void SetUp() {}                                                                                      \
        void TearDown() {}                                                                                   \
    };                                                                                                       \
                                                                                                             \
    namespace {                                                                                              \
    void cce_name##_##dst_type##_##src_type##_RetStub(dst_type& dst, src_type& src, Literal part) {}         \
    }                                                                                                        \
                                                                                                             \
    TEST_F(TestVectorCompute##class_name##dst_type##src_type##RetCApi, c_api_name##_RetSucc)                 \
    {                                                                                                        \
        src_type src;                                                                                        \
                                                                                                             \
        MOCKER_CPP(cce_name, void(dst_type&, src_type&, Literal))                                            \
            .times(1)                                                                                        \
            .will(invoke(cce_name##_##dst_type##_##src_type##_RetStub));                                     \
                                                                                                             \
        dst_type result = c_api_name(src);                                                                  \
        GlobalMockObject::verify();                                                                         \
    }

#define TEST_VECTOR_COMPUTE_PUNPACK_RET(class_name, c_api_name, cce_name)                                     \
                                                                                                             \
    class TestVectorCompute##class_name##vector_bool##RetCApi : public testing::Test {                       \
    protected:                                                                                               \
        void SetUp() {}                                                                                      \
        void TearDown() {}                                                                                   \
    };                                                                                                       \
                                                                                                             \
    namespace {                                                                                              \
    void cce_name##_##vector_bool##_RetStub(vector_bool& dst, vector_bool src, Literal part) {}             \
    }                                                                                                        \
                                                                                                             \
    TEST_F(TestVectorCompute##class_name##vector_bool##RetCApi, c_api_name##_RetSucc)                        \
    {                                                                                                        \
        vector_bool src;                                                                                     \
                                                                                                             \
        MOCKER_CPP(cce_name, void(vector_bool&, vector_bool, Literal))                                       \
            .times(1)                                                                                        \
            .will(invoke(cce_name##_##vector_bool##_RetStub));                                               \
                                                                                                             \
        vector_bool result = c_api_name(src);                                                               \
        GlobalMockObject::verify();                                                                         \
    }

TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackURet, asc_unpack_upper, vunpack, vector_uint16_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackURet, asc_unpack_upper, vunpack, vector_int16_t, vector_int8_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackURet, asc_unpack_upper, vunpack, vector_uint32_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackURet, asc_unpack_upper, vunpack, vector_int32_t, vector_int16_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackLRet, asc_unpack_lower, vunpack, vector_uint16_t, vector_uint8_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackLRet, asc_unpack_lower, vunpack, vector_int16_t, vector_int8_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackLRet, asc_unpack_lower, vunpack, vector_uint32_t, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNPACK_RET(UnPackLRet, asc_unpack_lower, vunpack, vector_int32_t, vector_int16_t);
TEST_VECTOR_COMPUTE_PUNPACK_RET(PunpackURet, asc_unpack_upper, punpack);
TEST_VECTOR_COMPUTE_PUNPACK_RET(PunpackLRet, asc_unpack_lower, punpack);
