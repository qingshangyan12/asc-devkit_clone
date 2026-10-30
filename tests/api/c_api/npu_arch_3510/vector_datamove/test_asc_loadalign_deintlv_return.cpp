/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstring>
#include <type_traits>
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"
#define CANN_ASC_USE_EXPERIMENTAL
#include "c_api/experimental/loadalign_exp.h"

namespace {
template <typename DataType>
void FillRegister(DataType& value, unsigned char pattern)
{
    std::memset(&value, pattern, sizeof(DataType));
}

template <typename DataType>
void ExpectRegisterPattern(const DataType& value, unsigned char pattern)
{
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    for (size_t index = 0; index < sizeof(DataType); ++index) {
        EXPECT_EQ(bytes[index], pattern);
    }
}
} // namespace

#define TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(                                           \
    class_name, c_api_name, cce_name, dst_type, src_type, expected_dist, result_type)                 \
    class TestVectorDatamove##class_name##dst_type##src_type##PairReturnCApi : public testing::Test { \
    protected:                                                                                        \
        void SetUp() {}                                                                               \
        void TearDown() {}                                                                            \
    };                                                                                                \
                                                                                                      \
    namespace {                                                                                       \
    void c_api_name##_##dst_type##_##src_type##_PairStub(                                             \
        dst_type& dst0, dst_type& dst1, __ubuf__ src_type* src, int32_t offset, int load_dist)        \
    {                                                                                                 \
        EXPECT_EQ(src, reinterpret_cast<__ubuf__ src_type*>(11));                                     \
        EXPECT_EQ(offset, static_cast<int32_t>(0));                                                   \
        EXPECT_EQ(load_dist, static_cast<int>(expected_dist));                                        \
        FillRegister(dst0, 0x11);                                                                     \
        FillRegister(dst1, 0x22);                                                                     \
    }                                                                                                 \
    }                                                                                                 \
                                                                                                      \
    TEST_F(                                                                                           \
        TestVectorDatamove##class_name##dst_type##src_type##PairReturnCApi,                           \
        c_api_name##_##dst_type##_##src_type##_PairReturnSucc)                                        \
    {                                                                                                 \
        __ubuf__ src_type* src = reinterpret_cast<__ubuf__ src_type*>(11);                            \
        static_assert(std::is_same_v<decltype(c_api_name(src)), result_type>);                        \
                                                                                                      \
        MOCKER_CPP(cce_name, void(dst_type&, dst_type&, __ubuf__ src_type*, int32_t, int))            \
            .times(1)                                                                                 \
            .will(invoke(c_api_name##_##dst_type##_##src_type##_PairStub));                           \
                                                                                                      \
        auto result = c_api_name(src);                                                                \
        ExpectRegisterPattern(result.first, 0x11);                                                    \
        ExpectRegisterPattern(result.second, 0x22);                                                   \
        GlobalMockObject::verify();                                                                   \
    }

#define TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN_HIF8(                                          \
    class_name, c_api_name, cce_name, dst_type, src_type, expected_dist, result_type)                     \
    class TestVectorDatamove##class_name##dst_type##src_type##PairReturnCApi : public testing::Test {     \
    protected:                                                                                            \
        void SetUp() {}                                                                                   \
        void TearDown() {}                                                                                \
    };                                                                                                    \
                                                                                                          \
    namespace {                                                                                           \
    void c_api_name##_##dst_type##_##src_type##_PairStub(                                                 \
        vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, int32_t offset, int load_dist) \
    {                                                                                                     \
        EXPECT_EQ(src, reinterpret_cast<__ubuf__ uint8_t*>(11));                                          \
        EXPECT_EQ(offset, static_cast<int32_t>(0));                                                       \
        EXPECT_EQ(load_dist, static_cast<int>(expected_dist));                                            \
        FillRegister(dst0, 0x11);                                                                         \
        FillRegister(dst1, 0x22);                                                                         \
    }                                                                                                     \
    }                                                                                                     \
                                                                                                          \
    TEST_F(                                                                                               \
        TestVectorDatamove##class_name##dst_type##src_type##PairReturnCApi,                               \
        c_api_name##_##dst_type##_##src_type##_PairReturnSucc)                                            \
    {                                                                                                     \
        __ubuf__ src_type* src = reinterpret_cast<__ubuf__ src_type*>(11);                                \
        static_assert(std::is_same_v<decltype(c_api_name(src)), result_type>);                            \
                                                                                                          \
        MOCKER_CPP(cce_name, void(vector_uint8_t&, vector_uint8_t&, __ubuf__ uint8_t*, int32_t, int))     \
            .times(1)                                                                                     \
            .will(invoke(c_api_name##_##dst_type##_##src_type##_PairStub));                               \
                                                                                                          \
        auto result = c_api_name(src);                                                                    \
        ExpectRegisterPattern(result.first, 0x11);                                                        \
        ExpectRegisterPattern(result.second, 0x22);                                                       \
        GlobalMockObject::verify();                                                                       \
    }

#define TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN_INT4(                                                 \
    class_name, c_api_name, cce_name, dst_type, src_type, expected_dist, result_type)                            \
    class TestVectorDatamove##class_name##dst_type##src_type##PairReturnCApi : public testing::Test {            \
    protected:                                                                                                   \
        void SetUp() {}                                                                                          \
        void TearDown() {}                                                                                       \
    };                                                                                                           \
                                                                                                                 \
    namespace {                                                                                                  \
    void c_api_name##_##dst_type##_##src_type##_PairStub(                                                        \
        vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ float4_e1m2x2_t* src, int32_t offset,     \
        int load_dist)                                                                                           \
    {                                                                                                            \
        EXPECT_EQ(src, reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(11));                                         \
        EXPECT_EQ(offset, static_cast<int32_t>(0));                                                              \
        EXPECT_EQ(load_dist, static_cast<int>(expected_dist));                                                   \
        FillRegister(dst0, 0x11);                                                                                \
        FillRegister(dst1, 0x22);                                                                                \
    }                                                                                                            \
    }                                                                                                            \
                                                                                                                 \
    TEST_F(                                                                                                      \
        TestVectorDatamove##class_name##dst_type##src_type##PairReturnCApi,                                      \
        c_api_name##_##dst_type##_##src_type##_PairReturnSucc)                                                   \
    {                                                                                                            \
        __ubuf__ src_type* src = reinterpret_cast<__ubuf__ src_type*>(11);                                       \
        static_assert(std::is_same_v<decltype(c_api_name(src)), result_type>);                                   \
                                                                                                                 \
        MOCKER_CPP(                                                                                              \
            cce_name, void(vector_fp4x2_e1m2_t&, vector_fp4x2_e1m2_t&, __ubuf__ float4_e1m2x2_t*, int32_t, int)) \
            .times(1)                                                                                            \
            .will(invoke(c_api_name##_##dst_type##_##src_type##_PairStub));                                      \
                                                                                                                 \
        auto result = c_api_name(src);                                                                           \
        ExpectRegisterPattern(result.first, 0x11);                                                               \
        ExpectRegisterPattern(result.second, 0x22);                                                              \
        GlobalMockObject::verify();                                                                              \
    }

TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_int8_t, int8_t, DINTLV_B8, asc_int8_t_int8_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_uint8_t, uint8_t, DINTLV_B8, asc_uint8_t_uint8_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_fp4x2_e2m1_t, fp4x2_e2m1_t, DINTLV_B8,
    asc_fp4x2_e2m1_t_fp4x2_e2m1_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_fp4x2_e1m2_t, fp4x2_e1m2_t, DINTLV_B8,
    asc_fp4x2_e1m2_t_fp4x2_e1m2_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_fp8_e8m0_t, fp8_e8m0_t, DINTLV_B8,
    asc_fp8_e8m0_t_fp8_e8m0_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_fp8_e5m2_t, fp8_e5m2_t, DINTLV_B8,
    asc_fp8_e5m2_t_fp8_e5m2_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_fp8_e4m3fn_t, fp8_e4m3fn_t, DINTLV_B8,
    asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN_HIF8(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_hifloat8_t, hifloat8_t, DINTLV_B8,
    asc_hifloat8_t_hifloat8_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_int16_t, int16_t, DINTLV_B16, asc_int16_t_int16_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_uint16_t, uint16_t, DINTLV_B16,
    asc_uint16_t_uint16_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_half, half, DINTLV_B16, asc_half_half_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_bfloat16_t, bfloat16_t, DINTLV_B16,
    asc_bfloat16_t_bfloat16_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_int32_t, int32_t, DINTLV_B32, asc_int32_t_int32_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_uint32_t, uint32_t, DINTLV_B32,
    asc_uint32_t_uint32_t_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_float, float, DINTLV_B32, asc_float_float_pair);
TEST_VECTOR_DATAMOVE_LOADALIGN_DEINTLV_PAIR_RETURN_INT4(
    LoadAlignDeintlvPair, asc_loadalign_deintlv, vlds, vector_int4x2_t, int4b_t, DINTLV_B8, asc_int4x2_t_int4x2_t_pair);
