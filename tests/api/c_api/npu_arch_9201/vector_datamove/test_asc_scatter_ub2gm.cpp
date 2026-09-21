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

#define TEST_VECTOR_DATAMOVE_SCATTER_UB2GM_INSTR(class_name, data_type)                                                \
                                                                                                                       \
    class TestVectorDataMove##class_name##_##data_type##_CApi : public testing::Test {                                 \
    protected:                                                                                                         \
        void SetUp() {}                                                                                                \
        void TearDown() {}                                                                                             \
    };                                                                                                                 \
                                                                                                                       \
    namespace {                                                                                                        \
    void scatter_ubuf_to_gm_##data_type##_Stub(                                                                        \
        __gm__ data_type* dst, __ubuf__ data_type* data_src, uint8_t sid, uint8_t offset_type,                         \
        __ubuf__ void* offset_src, uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num) \
    {                                                                                                                  \
        EXPECT_EQ(dst, reinterpret_cast<__gm__ data_type*>(11));                                                       \
        EXPECT_EQ(data_src, reinterpret_cast<__ubuf__ data_type*>(22));                                                \
        EXPECT_EQ(sid, static_cast<uint8_t>(0));                                                                       \
        EXPECT_EQ(offset_type, static_cast<uint8_t>(1));                                                               \
        EXPECT_EQ(offset_src, reinterpret_cast<__ubuf__ void*>(33));                                                   \
        EXPECT_EQ(l2_cache_ctl, static_cast<uint8_t>(asc_store_l2_cache_mode::NORMAL_PERSISTENT));                     \
        EXPECT_EQ(burst_len, static_cast<uint32_t>(2));                                                                \
        EXPECT_EQ(src_stride, static_cast<uint32_t>(3));                                                               \
        EXPECT_EQ(offset_num, static_cast<uint32_t>(4));                                                               \
    }                                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    TEST_F(TestVectorDataMove##class_name##_##data_type##_CApi, asc_scatter_ub2gm_##data_type##_Succ)                  \
    {                                                                                                                  \
        __gm__ data_type* dst = reinterpret_cast<__gm__ data_type*>(11);                                               \
        __ubuf__ data_type* src = reinterpret_cast<__ubuf__ data_type*>(22);                                           \
        __ubuf__ void* offset_src = reinterpret_cast<__ubuf__ void*>(33);                                              \
                                                                                                                       \
        MOCKER_CPP(                                                                                                    \
            scatter_ubuf_to_gm, void(                                                                                  \
                                    __gm__ data_type*, __ubuf__ data_type*, uint8_t, uint8_t, __ubuf__ void*, uint8_t, \
                                    uint32_t, uint32_t, uint32_t))                                                     \
            .times(1)                                                                                                  \
            .will(invoke(scatter_ubuf_to_gm_##data_type##_Stub));                                                      \
                                                                                                                       \
        asc_scatter_ub2gm(                                                                                             \
            dst, src, static_cast<uint8_t>(1), offset_src, asc_store_l2_cache_mode::NORMAL_PERSISTENT,                 \
            static_cast<uint32_t>(2), static_cast<uint32_t>(3), static_cast<uint32_t>(4));                             \
        GlobalMockObject::verify();                                                                                    \
    }

TEST_VECTOR_DATAMOVE_SCATTER_UB2GM_INSTR(VScatterUb2Gm, uint8_t);
TEST_VECTOR_DATAMOVE_SCATTER_UB2GM_INSTR(VScatterUb2Gm, uint16_t);
TEST_VECTOR_DATAMOVE_SCATTER_UB2GM_INSTR(VScatterUb2Gm, uint32_t);
TEST_VECTOR_DATAMOVE_SCATTER_UB2GM_INSTR(VScatterUb2Gm, uint64_t);

#undef TEST_VECTOR_DATAMOVE_SCATTER_UB2GM_INSTR
