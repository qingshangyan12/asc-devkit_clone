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

#define MOCK_COPY_UB2GM_ALIGN(cce_name, stub_name)                                                             \
    MOCKER_CPP(                                                                                                \
        cce_name, void(                                                                                        \
                      __gm__ void*, __ubuf__ void*, uint8_t, uint32_t, uint32_t, bool, bool, uint8_t, uint8_t, \
                      uint64_t, uint32_t, bool))                                                               \
        .times(1)                                                                                              \
        .will(invoke(stub_name))

#define TEST_ASC_COPY_UB2GM_ALIGN(test_prefix, data_type)                                                      \
    class TestAscCopyUb2gmAlign##test_prefix : public testing::Test {                                          \
    protected:                                                                                                 \
        void SetUp() override { g_coreType = C_API_AIV_TYPE; }                                                 \
    };                                                                                                         \
                                                                                                               \
    namespace {                                                                                                \
    void CopyUbufToGmAlignV2##test_prefix##Stub(                                                               \
        __gm__ void* dst, __ubuf__ void* src, uint8_t sid, uint32_t burst_count, uint32_t burst_len,           \
        bool hardcoded_first, bool hardcoded_second, uint8_t hardcoded_zero, uint8_t l2_cache_mode,            \
        uint64_t dst_stride, uint32_t src_stride, bool hardcoded_last)                                         \
    {                                                                                                          \
        EXPECT_EQ(dst, reinterpret_cast<__gm__ void*>(11));                                                    \
        EXPECT_EQ(src, reinterpret_cast<__ubuf__ void*>(22));                                                  \
        EXPECT_EQ(sid, static_cast<uint8_t>(0));                                                               \
        EXPECT_EQ(burst_count, static_cast<uint32_t>(1));                                                      \
        EXPECT_EQ(burst_len, static_cast<uint32_t>(44));                                                       \
        EXPECT_FALSE(hardcoded_first);                                                                         \
        EXPECT_FALSE(hardcoded_second);                                                                        \
        EXPECT_EQ(hardcoded_zero, static_cast<uint8_t>(0));                                                    \
        EXPECT_EQ(l2_cache_mode, static_cast<uint8_t>(asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM));          \
        EXPECT_EQ(dst_stride, static_cast<uint64_t>(0));                                                       \
        EXPECT_EQ(src_stride, static_cast<uint32_t>(0));                                                       \
        EXPECT_FALSE(hardcoded_last);                                                                          \
    }                                                                                                          \
    }                                                                                                          \
                                                                                                               \
    TEST_F(TestAscCopyUb2gmAlign##test_prefix, FullParamsSucc)                                                 \
    {                                                                                                          \
        __gm__ data_type* dst = reinterpret_cast<__gm__ data_type*>(11);                                       \
        __ubuf__ data_type* src = reinterpret_cast<__ubuf__ data_type*>(22);                                   \
        MOCK_COPY_UB2GM_ALIGN(copy_ubuf_to_gm_align_v2, CopyUbufToGmAlignV2##test_prefix##Stub);               \
        asc_copy_ub2gm_align(                                                                                  \
            dst, src, static_cast<uint32_t>(1), static_cast<uint32_t>(44),                                     \
            asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, static_cast<uint64_t>(0), static_cast<uint32_t>(0)); \
        GlobalMockObject::verify();                                                                            \
    }                                                                                                          \
                                                                                                               \
    TEST_F(TestAscCopyUb2gmAlign##test_prefix, SizeSucc)                                                       \
    {                                                                                                          \
        __gm__ data_type* dst = reinterpret_cast<__gm__ data_type*>(11);                                       \
        __ubuf__ data_type* src = reinterpret_cast<__ubuf__ data_type*>(22);                                   \
        MOCK_COPY_UB2GM_ALIGN(copy_ubuf_to_gm_align_v2, CopyUbufToGmAlignV2##test_prefix##Stub);               \
        asc_copy_ub2gm_align(dst, src, static_cast<uint32_t>(44));                                             \
        GlobalMockObject::verify();                                                                            \
    }

TEST_ASC_COPY_UB2GM_ALIGN(Uint8, uint8_t);
TEST_ASC_COPY_UB2GM_ALIGN(Int8, int8_t);
TEST_ASC_COPY_UB2GM_ALIGN(Uint16, uint16_t);
TEST_ASC_COPY_UB2GM_ALIGN(Int16, int16_t);
TEST_ASC_COPY_UB2GM_ALIGN(Uint32, uint32_t);
TEST_ASC_COPY_UB2GM_ALIGN(Int32, int32_t);
TEST_ASC_COPY_UB2GM_ALIGN(Half, half);
TEST_ASC_COPY_UB2GM_ALIGN(Float, float);
TEST_ASC_COPY_UB2GM_ALIGN(Hifloat8, hifloat8_t);
TEST_ASC_COPY_UB2GM_ALIGN(Bfloat16, bfloat16_t);
TEST_ASC_COPY_UB2GM_ALIGN(Fp8E4m3fn, fp8_e4m3fn_t);
TEST_ASC_COPY_UB2GM_ALIGN(Fp8E5m2, fp8_e5m2_t);

#undef MOCK_COPY_UB2GM_ALIGN
#undef TEST_ASC_COPY_UB2GM_ALIGN
