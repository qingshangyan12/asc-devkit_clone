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
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

#define MOCK_COPY_GM2UB_ALIGN(cce_name, data_type, stub_name)                                                      \
    MOCKER_CPP(                                                                                                    \
        cce_name, void(                                                                                            \
                      __ubuf__ data_type*, __gm__ data_type*, uint8_t, uint32_t, uint32_t, uint8_t, uint8_t, bool, \
                      bool, uint8_t, uint64_t, uint32_t, bool))                                                    \
        .times(1)                                                                                                  \
        .will(invoke(stub_name))

#define TEST_ASC_COPY_GM2UB_ALIGN(test_prefix, data_type)                                                          \
    class TestAscCopyGm2ubAlign##test_prefix : public testing::Test {                                              \
    protected:                                                                                                     \
        void SetUp() override { g_coreType = C_API_AIV_TYPE; }                                                     \
    };                                                                                                             \
                                                                                                                   \
    namespace {                                                                                                    \
    void CopyGmToUbufAlignV2##test_prefix##Stub(                                                                   \
        __ubuf__ data_type* dst, __gm__ data_type* src, uint8_t sid, uint32_t burst_count, uint32_t burst_len,     \
        uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad,                             \
        bool enable_pre_read_ignore_sync, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride,         \
        bool enable_retry)                                                                                         \
    {                                                                                                              \
        EXPECT_EQ(dst, reinterpret_cast<__ubuf__ data_type*>(11));                                                 \
        EXPECT_EQ(src, reinterpret_cast<__gm__ data_type*>(22));                                                   \
        EXPECT_EQ(sid, static_cast<uint8_t>(0));                                                                   \
        EXPECT_EQ(burst_count, static_cast<uint32_t>(1));                                                          \
        EXPECT_EQ(burst_len, static_cast<uint32_t>(44));                                                           \
        EXPECT_EQ(left_padding_num, static_cast<uint8_t>(0));                                                      \
        EXPECT_EQ(right_padding_num, static_cast<uint8_t>(0));                                                     \
        EXPECT_FALSE(enable_constant_pad);                                                                         \
        EXPECT_FALSE(enable_pre_read_ignore_sync);                                                                 \
        EXPECT_EQ(l2_cache_mode, static_cast<uint8_t>(asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM));               \
        EXPECT_EQ(src_stride, static_cast<uint64_t>(0));                                                           \
        EXPECT_EQ(dst_stride, static_cast<uint32_t>(0));                                                           \
        EXPECT_FALSE(enable_retry);                                                                                \
    }                                                                                                              \
    }                                                                                                              \
                                                                                                                   \
    TEST_F(TestAscCopyGm2ubAlign##test_prefix, ExtendedFullParamsSucc)                                             \
    {                                                                                                              \
        __ubuf__ data_type* dst = reinterpret_cast<__ubuf__ data_type*>(11);                                       \
        __gm__ data_type* src = reinterpret_cast<__gm__ data_type*>(22);                                           \
        MOCK_COPY_GM2UB_ALIGN(copy_gm_to_ubuf_align_v2, data_type, CopyGmToUbufAlignV2##test_prefix##Stub);        \
        asc_copy_gm2ub_align(                                                                                      \
            dst, src, static_cast<uint32_t>(1), static_cast<uint32_t>(44), static_cast<uint8_t>(0),                \
            static_cast<uint8_t>(0), false, false, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,                    \
            static_cast<uint64_t>(0), static_cast<uint32_t>(0), false);                                            \
        GlobalMockObject::verify();                                                                                \
    }                                                                                                              \
                                                                                                                   \
    TEST_F(TestAscCopyGm2ubAlign##test_prefix, StandardFullParamsSucc)                                             \
    {                                                                                                              \
        __ubuf__ data_type* dst = reinterpret_cast<__ubuf__ data_type*>(11);                                       \
        __gm__ data_type* src = reinterpret_cast<__gm__ data_type*>(22);                                           \
        MOCK_COPY_GM2UB_ALIGN(copy_gm_to_ubuf_align_v2, data_type, CopyGmToUbufAlignV2##test_prefix##Stub);        \
        asc_copy_gm2ub_align(                                                                                      \
            dst, src, static_cast<uint32_t>(1), static_cast<uint32_t>(44), static_cast<uint8_t>(0),                \
            static_cast<uint8_t>(0), false, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, static_cast<uint64_t>(0), \
            static_cast<uint32_t>(0));                                                                             \
        GlobalMockObject::verify();                                                                                \
    }                                                                                                              \
                                                                                                                   \
    TEST_F(TestAscCopyGm2ubAlign##test_prefix, SizeSucc)                                                           \
    {                                                                                                              \
        __ubuf__ data_type* dst = reinterpret_cast<__ubuf__ data_type*>(11);                                       \
        __gm__ data_type* src = reinterpret_cast<__gm__ data_type*>(22);                                           \
        MOCK_COPY_GM2UB_ALIGN(copy_gm_to_ubuf_align_v2, data_type, CopyGmToUbufAlignV2##test_prefix##Stub);        \
        asc_copy_gm2ub_align(dst, src, static_cast<uint32_t>(44));                                                 \
        GlobalMockObject::verify();                                                                                \
    }

TEST_ASC_COPY_GM2UB_ALIGN(Uint8, uint8_t);
TEST_ASC_COPY_GM2UB_ALIGN(Int8, int8_t);
TEST_ASC_COPY_GM2UB_ALIGN(Uint16, uint16_t);
TEST_ASC_COPY_GM2UB_ALIGN(Int16, int16_t);
TEST_ASC_COPY_GM2UB_ALIGN(Uint32, uint32_t);
TEST_ASC_COPY_GM2UB_ALIGN(Int32, int32_t);
TEST_ASC_COPY_GM2UB_ALIGN(Half, half);
TEST_ASC_COPY_GM2UB_ALIGN(Float, float);
TEST_ASC_COPY_GM2UB_ALIGN(Hifloat8, hifloat8_t);
TEST_ASC_COPY_GM2UB_ALIGN(Bfloat16, bfloat16_t);
TEST_ASC_COPY_GM2UB_ALIGN(Fp8E4m3fn, fp8_e4m3fn_t);
TEST_ASC_COPY_GM2UB_ALIGN(Fp8E5m2, fp8_e5m2_t);

#undef MOCK_COPY_GM2UB_ALIGN
#undef TEST_ASC_COPY_GM2UB_ALIGN
