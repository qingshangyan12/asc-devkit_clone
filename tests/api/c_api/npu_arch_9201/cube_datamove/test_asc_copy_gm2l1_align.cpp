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

template <typename DTYPE>
__aicore__ inline void copy_gm_to_cbuf_align_v2_stub(
    __cbuf__ DTYPE* dst, __gm__ DTYPE* src, uint8_t sid, uint32_t n_burst, uint32_t len_burst,
    uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, bool enable_pre_load_ignore_sync,
    uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
{
    EXPECT_EQ(dst, reinterpret_cast<__cbuf__ DTYPE*>(11));
    EXPECT_EQ(src, reinterpret_cast<__gm__ DTYPE*>(22));
    EXPECT_EQ(sid, static_cast<uint8_t>(0));
    EXPECT_EQ(n_burst, static_cast<uint32_t>(33));
    EXPECT_EQ(len_burst, static_cast<uint32_t>(44));
    EXPECT_EQ(left_padding_count, static_cast<uint8_t>(55));
    EXPECT_EQ(right_padding_count, static_cast<uint8_t>(66));
    EXPECT_EQ(data_select_bit, static_cast<bool>(false));
    EXPECT_TRUE(enable_pre_load_ignore_sync);
    EXPECT_EQ(l2_cache_ctl, static_cast<uint8_t>(4));
    EXPECT_EQ(burst_src_stride, static_cast<uint64_t>(88));
    EXPECT_EQ(burst_dst_stride, static_cast<uint32_t>(99));
}

class TEST_COPY_GM_TO_L1_ALIGN : public testing::Test {
protected:
    void SetUp() { g_coreType = C_API_AIC_TYPE; }
    void TearDown() { g_coreType = C_API_AIV_TYPE; }
};

namespace {
struct Gm2L1AlignBoundaryParams {
    uint32_t burstCount;
    uint32_t burstLen;
    uint8_t leftPaddingCount;
    uint8_t rightPaddingCount;
    uint64_t srcStride;
    uint32_t dstStride;
};

Gm2L1AlignBoundaryParams g_gm2l1AlignBoundaryParams{};

__aicore__ inline void copy_gm_to_cbuf_align_v2_boundary_stub(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint8_t sid, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, bool enable_pre_load_ignore_sync,
    uint8_t l2_cache_ctl, uint64_t src_stride, uint32_t dst_stride)
{
    EXPECT_EQ(dst, reinterpret_cast<__cbuf__ uint8_t*>(11));
    EXPECT_EQ(src, reinterpret_cast<__gm__ uint8_t*>(22));
    EXPECT_EQ(sid, static_cast<uint8_t>(0));
    EXPECT_EQ(burst_count, g_gm2l1AlignBoundaryParams.burstCount);
    EXPECT_EQ(burst_len, g_gm2l1AlignBoundaryParams.burstLen);
    EXPECT_EQ(left_padding_count, g_gm2l1AlignBoundaryParams.leftPaddingCount);
    EXPECT_EQ(right_padding_count, g_gm2l1AlignBoundaryParams.rightPaddingCount);
    EXPECT_FALSE(data_select_bit);
    EXPECT_FALSE(enable_pre_load_ignore_sync);
    EXPECT_EQ(l2_cache_ctl, static_cast<uint8_t>(0));
    EXPECT_EQ(src_stride, g_gm2l1AlignBoundaryParams.srcStride);
    EXPECT_EQ(dst_stride, g_gm2l1AlignBoundaryParams.dstStride);
}

void TestGm2L1AlignBoundary(const Gm2L1AlignBoundaryParams& params)
{
    g_gm2l1AlignBoundaryParams = params;
    MOCKER_CPP(
        copy_gm_to_cbuf_align_v2, void(
                                      __cbuf__ uint8_t*, __gm__ uint8_t*, uint8_t, uint32_t, uint32_t, uint8_t, uint8_t,
                                      bool, bool, uint8_t, uint64_t, uint32_t))
        .times(1)
        .will(invoke(copy_gm_to_cbuf_align_v2_boundary_stub));

    asc_copy_gm2l1_align(
        reinterpret_cast<__cbuf__ uint8_t*>(11), reinterpret_cast<__gm__ uint8_t*>(22), params.burstCount,
        params.burstLen, params.leftPaddingCount, params.rightPaddingCount, false, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, params.srcStride, params.dstStride);
    GlobalMockObject::verify();
}
} // namespace

#define TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(dtype)                                                              \
                                                                                                                \
    TEST_F(TEST_COPY_GM_TO_L1_ALIGN, TEST_COPY_GM_TO_L1_ALIGN_##dtype)                                          \
    {                                                                                                           \
        __cbuf__ dtype* dst = reinterpret_cast<__cbuf__ dtype*>(11);                                            \
        __gm__ dtype* src = reinterpret_cast<__gm__ dtype*>(22);                                                \
                                                                                                                \
        uint32_t n_burst = 33;                                                                                  \
        uint32_t len_burst = 44;                                                                                \
        uint8_t left_padding_count = 55;                                                                        \
        uint8_t right_padding_count = 66;                                                                       \
        bool data_select_bit = false;                                                                           \
        asc_load_l2_cache_mode l2_cache_mode = asc_load_l2_cache_mode::NOTALLOC_KEEP;                           \
        uint64_t burst_src_stride = 88;                                                                         \
        uint32_t burst_dst_stride = 99;                                                                         \
                                                                                                                \
        MOCKER_CPP(                                                                                             \
            copy_gm_to_cbuf_align_v2, void(                                                                     \
                                          __cbuf__ dtype*, __gm__ dtype*, uint8_t, uint32_t, uint32_t, uint8_t, \
                                          uint8_t, bool, bool, uint8_t, uint64_t, uint32_t))                    \
            .times(1)                                                                                           \
            .will(invoke(copy_gm_to_cbuf_align_v2_stub<dtype>));                                                \
                                                                                                                \
        asc_copy_gm2l1_align(                                                                                   \
            dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, true,       \
            l2_cache_mode, burst_src_stride, burst_dst_stride);                                                 \
        GlobalMockObject::verify();                                                                             \
    }

TEST_F(TEST_COPY_GM_TO_L1_ALIGN, TEST_COPY_GM_TO_L1_ALIGN_AIV_NOOP)
{
    g_coreType = C_API_AIV_TYPE;
    MOCKER_CPP(
        copy_gm_to_cbuf_align_v2, void(
                                      __cbuf__ half*, __gm__ half*, uint8_t, uint32_t, uint32_t, uint8_t, uint8_t, bool,
                                      bool, uint8_t, uint64_t, uint32_t))
        .times(0);

    asc_copy_gm2l1_align(
        reinterpret_cast<__cbuf__ half*>(11), reinterpret_cast<__gm__ half*>(22), 33, 44, 0, 0, true, false,
        asc_load_l2_cache_mode::NORMAL_LAST_VICTIM, 88, 99);
    GlobalMockObject::verify();
}

TEST_F(TEST_COPY_GM_TO_L1_ALIGN, TEST_COPY_GM_TO_L1_ALIGN_ParameterLowerBounds)
{
    TestGm2L1AlignBoundary({1, 1, 0, 0, 0, 0});
}

TEST_F(TEST_COPY_GM_TO_L1_ALIGN, TEST_COPY_GM_TO_L1_ALIGN_ParameterUpperBounds)
{
    constexpr uint32_t max_21_bit_value = (1U << 21) - 1;
    constexpr uint64_t max_40_bit_value = (1ULL << 40) - 1;
    TestGm2L1AlignBoundary({max_21_bit_value, max_21_bit_value, 32, 32, max_40_bit_value, max_21_bit_value});
}

TEST_F(TEST_COPY_GM_TO_L1_ALIGN, TEST_COPY_GM_TO_L1_ALIGN_21BitOverflowIsForwarded)
{
    constexpr uint32_t overflow_21_bit_value = 1U << 21;
    TestGm2L1AlignBoundary({overflow_21_bit_value, overflow_21_bit_value, 0, 0, 0, overflow_21_bit_value});
}

// ==========asc_copy_gm2l1_align==========
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(bfloat16_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(half);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(float);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(hifloat8_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(int16_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(int32_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(int8_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(uint16_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(uint32_t);
TEST_CUBE_DATAMOVE_COPY_GM2L1_ALIGN(uint8_t);
