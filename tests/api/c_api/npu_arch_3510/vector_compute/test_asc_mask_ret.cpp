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
#include "c_api/reg_compute/reg_copy.h"

// ==========asc_mask return-value overloads(u8/s8/half/u16/s16/float/u32/s32/bf16)==========
#define TEST_VECTOR_COMPUTE_MASK_RET_VMOV(class_name, c_api_name, cce_name, data_type)                       \
                                                                                                             \
    class TestVectorCompute##class_name##_##data_type##_RetCApi : public testing::Test {                     \
    protected:                                                                                               \
        void SetUp() {}                                                                                      \
        void TearDown() {}                                                                                   \
    };                                                                                                       \
                                                                                                             \
    namespace {                                                                                              \
    void cce_name##_##data_type##_RetStub(data_type& dst, data_type src, vector_bool mask, int32_t mode) {}  \
    }                                                                                                        \
                                                                                                             \
    TEST_F(TestVectorCompute##class_name##_##data_type##_RetCApi, c_api_name##_##data_type##_RetSucc)        \
    {                                                                                                        \
        data_type src;                                                                                       \
        vector_bool mask;                                                                                    \
                                                                                                             \
        MOCKER_CPP(cce_name, void(data_type&, data_type, vector_bool, int32_t))                              \
            .times(1)                                                                                        \
            .will(invoke(cce_name##_##data_type##_RetStub));                                                 \
                                                                                                             \
        data_type result = c_api_name(src, mask);                                                           \
        GlobalMockObject::verify();                                                                         \
    }

// ==========asc_mask return-value overloads(bool with mask / bool without mask)==========
#define TEST_VECTOR_COMPUTE_MASK_RET_PMOV_MASK(class_name, c_api_name, cce_name, data_type)                 \
                                                                                                             \
    class TestVectorCompute##class_name##_##data_type##_RetCApi : public testing::Test {                    \
    protected:                                                                                               \
        void SetUp() {}                                                                                      \
        void TearDown() {}                                                                                   \
    };                                                                                                       \
                                                                                                             \
    namespace {                                                                                              \
    void cce_name##_##data_type##_RetStubMask(data_type& dst, data_type src, vector_bool mask) {}            \
    }                                                                                                        \
                                                                                                             \
    TEST_F(TestVectorCompute##class_name##_##data_type##_RetCApi, c_api_name##_##data_type##_MaskRetSucc)    \
    {                                                                                                        \
        data_type src;                                                                                       \
        vector_bool mask;                                                                                    \
                                                                                                             \
        MOCKER_CPP(cce_name, void(data_type&, data_type, vector_bool))                                       \
            .times(1)                                                                                        \
            .will(invoke(cce_name##_##data_type##_RetStubMask));                                             \
                                                                                                             \
        data_type result = c_api_name(src, mask);                                                           \
        GlobalMockObject::verify();                                                                         \
    }

#define TEST_VECTOR_COMPUTE_MASK_RET_PMOV(class_name, c_api_name, cce_name, data_type)                       \
                                                                                                             \
    class TestVectorCompute##class_name##_##data_type##_RetCApi : public testing::Test {                    \
    protected:                                                                                               \
        void SetUp() {}                                                                                      \
        void TearDown() {}                                                                                   \
    };                                                                                                       \
                                                                                                             \
    namespace {                                                                                              \
    void cce_name##_##data_type##_RetStubNoMask(data_type& dst, data_type src) {}                            \
    }                                                                                                        \
                                                                                                             \
    TEST_F(TestVectorCompute##class_name##_##data_type##_RetCApi, c_api_name##_##data_type##_NoMaskRetSucc)  \
    {                                                                                                        \
        data_type src;                                                                                       \
                                                                                                             \
        MOCKER_CPP(cce_name, void(data_type&, data_type))                                                   \
            .times(1)                                                                                        \
            .will(invoke(cce_name##_##data_type##_RetStubNoMask));                                          \
                                                                                                             \
        data_type result = c_api_name(src);                                                                \
        GlobalMockObject::verify();                                                                         \
    }

TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_int8_t);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_uint8_t);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_int16_t);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_uint16_t);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_half);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_int32_t);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_uint32_t);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_float);
TEST_VECTOR_COMPUTE_MASK_RET_VMOV(MASK, asc_mask, vmov, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_MASK_RET_PMOV_MASK(MASKB, asc_mask, pmov, vector_bool);
TEST_VECTOR_COMPUTE_MASK_RET_PMOV(MASKB2, asc_mask, pmov, vector_bool);
