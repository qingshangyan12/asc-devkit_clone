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
#include "include/c_api/vector_datamove/vector_datamove.h"

#define TEST_VECTOR_DATAMOVE_GATHER_GM2UB_INSTR(class_name, data_type)                                             \
                                                                                                                   \
    class TestVectorDataMove##class_name##_##data_type##_CApi : public testing::Test {                             \
    protected:                                                                                                     \
        void SetUp() {}                                                                                            \
        void TearDown() {}                                                                                         \
    };                                                                                                             \
                                                                                                                   \
    namespace {                                                                                                    \
    void gather_gm_to_ubuf_##data_type##_Stub(                                                                     \
        __ubuf__ data_type* dst, __gm__ data_type* base_src, uint8_t sid, uint8_t offset_type,                     \
        bool constant_padding_ctl, __ubuf__ void* offset_src, uint8_t l2_cache_ctl, uint32_t burst_len,            \
        uint32_t dst_stride, uint32_t offset_num)                                                                  \
    {                                                                                                              \
        EXPECT_EQ(dst, reinterpret_cast<__ubuf__ data_type*>(11));                                                 \
        EXPECT_EQ(base_src, reinterpret_cast<__gm__ data_type*>(22));                                              \
        EXPECT_EQ(sid, static_cast<uint8_t>(0));                                                                   \
        EXPECT_EQ(offset_type, static_cast<uint8_t>(1));                                                           \
        EXPECT_EQ(constant_padding_ctl, true);                                                                     \
        EXPECT_EQ(offset_src, reinterpret_cast<__ubuf__ void*>(33));                                               \
        EXPECT_EQ(l2_cache_ctl, static_cast<uint8_t>(asc_load_l2_cache_mode::NORMAL_PERSISTENT));                  \
        EXPECT_EQ(burst_len, static_cast<uint32_t>(2));                                                            \
        EXPECT_EQ(dst_stride, static_cast<uint32_t>(3));                                                           \
        EXPECT_EQ(offset_num, static_cast<uint32_t>(4));                                                           \
    }                                                                                                              \
    }                                                                                                              \
                                                                                                                   \
    TEST_F(TestVectorDataMove##class_name##_##data_type##_CApi, asc_gather_gm2ub_##data_type##_Succ)               \
    {                                                                                                              \
        __ubuf__ data_type* dst = reinterpret_cast<__ubuf__ data_type*>(11);                                       \
        __gm__ data_type* src = reinterpret_cast<__gm__ data_type*>(22);                                           \
        __ubuf__ void* offset_src = reinterpret_cast<__ubuf__ void*>(33);                                          \
                                                                                                                   \
        MOCKER_CPP(                                                                                                \
            gather_gm_to_ubuf, void(                                                                               \
                                   __ubuf__ data_type*, __gm__ data_type*, uint8_t, uint8_t, bool, __ubuf__ void*, \
                                   uint8_t, uint32_t, uint32_t, uint32_t))                                         \
            .times(1)                                                                                              \
            .will(invoke(gather_gm_to_ubuf_##data_type##_Stub));                                                   \
                                                                                                                   \
        asc_gather_gm2ub(                                                                                          \
            dst, src, static_cast<uint8_t>(1), true, offset_src, asc_load_l2_cache_mode::NORMAL_PERSISTENT,        \
            static_cast<uint32_t>(2), static_cast<uint32_t>(3), static_cast<uint32_t>(4));                         \
        GlobalMockObject::verify();                                                                                \
    }

TEST_VECTOR_DATAMOVE_GATHER_GM2UB_INSTR(VGatherGm2Ub, uint8_t);
TEST_VECTOR_DATAMOVE_GATHER_GM2UB_INSTR(VGatherGm2Ub, uint16_t);
TEST_VECTOR_DATAMOVE_GATHER_GM2UB_INSTR(VGatherGm2Ub, uint32_t);
TEST_VECTOR_DATAMOVE_GATHER_GM2UB_INSTR(VGatherGm2Ub, uint64_t);

#undef TEST_VECTOR_DATAMOVE_GATHER_GM2UB_INSTR
