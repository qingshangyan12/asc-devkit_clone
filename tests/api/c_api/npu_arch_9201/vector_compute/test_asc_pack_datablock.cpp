/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

#define TEST_PACK_DATABLOCK(type_name, type)             \
    TEST(PackDatablock, type_name)                       \
    {                                                    \
        type dst;                                        \
        type src;                                        \
        MOCKER_CPP(vpackv2, void(type&, type)).times(1); \
        asc_pack_datablock(dst, src);                    \
        GlobalMockObject::verify();                      \
    }

TEST_PACK_DATABLOCK(UInt16, vector_uint16_t);
TEST_PACK_DATABLOCK(Int16, vector_int16_t);
TEST_PACK_DATABLOCK(Half, vector_half);
TEST_PACK_DATABLOCK(UInt32, vector_uint32_t);
TEST_PACK_DATABLOCK(Int32, vector_int32_t);
TEST_PACK_DATABLOCK(Float, vector_float);
TEST_PACK_DATABLOCK(UInt64, vector_uint64_t);
TEST_PACK_DATABLOCK(Int64, vector_int64_t);

#define TEST_PACK_DATABLOCK_RETURN(type_name, type)              \
    TEST(PackDatablockReturn, type_name)                         \
    {                                                            \
        type src;                                                \
        MOCKER_CPP(vpackv2, void(type&, type)).times(1);         \
        auto dst = asc_pack_datablock(src);                      \
        static_assert(std::is_same<decltype(dst), type>::value); \
        (void)dst;                                               \
        GlobalMockObject::verify();                              \
    }

TEST_PACK_DATABLOCK_RETURN(UInt16, vector_uint16_t);
TEST_PACK_DATABLOCK_RETURN(Int16, vector_int16_t);
TEST_PACK_DATABLOCK_RETURN(Half, vector_half);
TEST_PACK_DATABLOCK_RETURN(UInt32, vector_uint32_t);
TEST_PACK_DATABLOCK_RETURN(Int32, vector_int32_t);
TEST_PACK_DATABLOCK_RETURN(Float, vector_float);
TEST_PACK_DATABLOCK_RETURN(UInt64, vector_uint64_t);
TEST_PACK_DATABLOCK_RETURN(Int64, vector_int64_t);

#define TEST_PACK_TO_64(test_name, src_type, api_name, expected_part)                                        \
    namespace {                                                                                              \
    void test_name##Stub(vector_uint32_t&, src_type, Literal part) { EXPECT_EQ(part, expected_part); }       \
    }                                                                                                        \
                                                                                                             \
    TEST(PackTo64, test_name)                                                                                \
    {                                                                                                        \
        vector_uint32_t dst;                                                                                 \
        src_type src;                                                                                        \
        MOCKER_CPP(vpack, void(vector_uint32_t&, src_type, Literal)).times(2).will(invoke(test_name##Stub)); \
        api_name(dst, src);                                                                                  \
        auto result = api_name(src);                                                                         \
        static_assert(std::is_same<decltype(result), vector_uint32_t>::value);                               \
        (void)result;                                                                                        \
        GlobalMockObject::verify();                                                                          \
    }

TEST_PACK_TO_64(UInt64Low, vector_uint64_t, asc_pack_to_low, LOWER);
TEST_PACK_TO_64(UInt64High, vector_uint64_t, asc_pack_to_high, HIGHER);
TEST_PACK_TO_64(Int64Low, vector_int64_t, asc_pack_to_low, LOWER);
TEST_PACK_TO_64(Int64High, vector_int64_t, asc_pack_to_high, HIGHER);

#undef TEST_PACK_DATABLOCK_RETURN
#undef TEST_PACK_DATABLOCK
#undef TEST_PACK_TO_64
