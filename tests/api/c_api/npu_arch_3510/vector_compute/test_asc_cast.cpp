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

#define TEST_ASC_CAST_ROUND(type, tag, expected_round)                                                        \
    namespace {                                                                                              \
    void AscCast##type##tag##Stub(type& dst, type src, int32_t round, vector_bool mask, int32_t mode)        \
    {                                                                                                        \
        EXPECT_EQ(round, expected_round);                                                                    \
        EXPECT_EQ(mode, MODE_ZEROING);                                                                       \
    }                                                                                                        \
    }                                                                                                        \
                                                                                                             \
    TEST(AscCastRound, type##tag)                                                                             \
    {                                                                                                        \
        type dst;                                                                                            \
        type src;                                                                                            \
        vector_bool mask;                                                                                    \
                                                                                                             \
        MOCKER_CPP(vtrc, void(type&, type, int32_t, vector_bool, int32_t))                                   \
            .times(1)                                                                                        \
            .will(invoke(AscCast##type##tag##Stub));                                                         \
                                                                                                             \
        asc_cast(dst, src, mask, ASC_CAST_##tag);                                                            \
        GlobalMockObject::verify();                                                                          \
    }

#define TEST_ASC_CAST_ROUND_TYPES(tag, expected_round)                  \
    TEST_ASC_CAST_ROUND(vector_half, tag, expected_round);              \
    TEST_ASC_CAST_ROUND(vector_bfloat16_t, tag, expected_round);        \
    TEST_ASC_CAST_ROUND(vector_float, tag, expected_round)

TEST_ASC_CAST_ROUND_TYPES(RN, ROUND_R);
TEST_ASC_CAST_ROUND_TYPES(RNA, ROUND_A);
TEST_ASC_CAST_ROUND_TYPES(RD, ROUND_F);
TEST_ASC_CAST_ROUND_TYPES(RU, ROUND_C);
TEST_ASC_CAST_ROUND_TYPES(RZ, ROUND_Z);

#undef TEST_ASC_CAST_ROUND_TYPES
#undef TEST_ASC_CAST_ROUND
