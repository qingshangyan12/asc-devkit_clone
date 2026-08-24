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

#define TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(c_api_name, data_type)                                       \
                                                                                                       \
    class TestVectorComputeUnsqueezeRet##data_type##CApi : public testing::Test {                      \
    protected:                                                                                         \
        void SetUp() {}                                                                               \
        void TearDown() {}                                                                            \
    };                                                                                                \
                                                                                                       \
    namespace {                                                                                       \
    void vusqz##_##data_type##_RetStub(data_type& dst, vector_bool mask) {}                           \
    }                                                                                                 \
                                                                                                       \
    TEST_F(TestVectorComputeUnsqueezeRet##data_type##CApi, c_api_name##_RetSucc)                       \
    {                                                                                                 \
        vector_bool mask;                                                                             \
                                                                                                       \
        MOCKER_CPP(vusqz, void(data_type&, vector_bool)).times(1).will(invoke(vusqz##_##data_type##_RetStub)); \
                                                                                                       \
        data_type result = c_api_name(mask);                                                          \
        GlobalMockObject::verify();                                                                  \
    }

TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(asc_unsqueeze_u8, vector_uint8_t);
TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(asc_unsqueeze_s8, vector_int8_t);
TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(asc_unsqueeze_u16, vector_uint16_t);
TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(asc_unsqueeze_s16, vector_int16_t);
TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(asc_unsqueeze_u32, vector_uint32_t);
TEST_VECTOR_COMPUTE_UNSQUEEZE_RET(asc_unsqueeze_s32, vector_int32_t);
