/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 */

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

TEST(PackTo64, SupportsSignedAndUnsignedSources)
{
    vector_uint32_t dst;
    vector_uint64_t u_src;
    vector_int64_t s_src;
    MOCKER_CPP(vpack, void(vector_uint32_t&, vector_uint64_t, Literal)).times(2);
    MOCKER_CPP(vpack, void(vector_uint32_t&, vector_int64_t, Literal)).times(2);

    asc_pack_to_low(dst, u_src);
    asc_pack_to_high(dst, u_src);
    asc_pack_to_low(dst, s_src);
    asc_pack_to_high(dst, s_src);
    GlobalMockObject::verify();
}

#undef TEST_PACK_DATABLOCK
