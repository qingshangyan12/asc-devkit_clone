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
#include "include/c_api/reg_compute/reg_convert.h"

#define TEST_FLOAT2HALF_M(class_name, c_api_name, position_name, position_value, round_mode, sat_mode, part_mode) \
    class TestFloat2HalfM##class_name##_##position_name : public testing::Test {};                                \
                                                                                                                  \
    namespace {                                                                                                   \
    void vcvt_float2half_m_##class_name##_##position_name(                                                        \
        vector_half&, vector_float, vector_bool, Literal actual_round, Literal actual_sat, Literal actual_part,   \
        Literal actual_mode)                                                                                      \
    {}                                                                                                            \
                                                                                                                  \
    TEST_F(TestFloat2HalfM##class_name##_##position_name, InvokeVcvt)                                             \
    {                                                                                                             \
        vector_half dst;                                                                                          \
        vector_float src;                                                                                         \
        vector_bool mask;                                                                                         \
        MOCKER_CPP(vcvt, void(vector_half&, vector_float, vector_bool, Literal, Literal, Literal, Literal))       \
            .times(1)                                                                                             \
            .will(invoke(vcvt_float2half_m_##class_name##_##position_name));                                      \
        c_api_name(dst, src, mask, position_value);                                                               \
        GlobalMockObject::verify();                                                                               \
    }                                                                                                             \
    }

TEST_FLOAT2HALF_M(Rd, asc_float2half_rd_merge, Even, ASC_POSITION_EVEN, ROUND_F, RS_DISABLE, PART_EVEN)
TEST_FLOAT2HALF_M(Rd, asc_float2half_rd_merge, Odd, ASC_POSITION_ODD, ROUND_F, RS_DISABLE, PART_ODD)
TEST_FLOAT2HALF_M(RdSat, asc_float2half_rd_sat_merge, Even, ASC_POSITION_EVEN, ROUND_F, RS_ENABLE, PART_EVEN)
TEST_FLOAT2HALF_M(RdSat, asc_float2half_rd_sat_merge, Odd, ASC_POSITION_ODD, ROUND_F, RS_ENABLE, PART_ODD)

TEST_FLOAT2HALF_M(Rn, asc_float2half_rn_merge, Even, ASC_POSITION_EVEN, ROUND_R, RS_DISABLE, PART_EVEN)
TEST_FLOAT2HALF_M(Rn, asc_float2half_rn_merge, Odd, ASC_POSITION_ODD, ROUND_R, RS_DISABLE, PART_ODD)
TEST_FLOAT2HALF_M(RnSat, asc_float2half_rn_sat_merge, Even, ASC_POSITION_EVEN, ROUND_R, RS_ENABLE, PART_EVEN)
TEST_FLOAT2HALF_M(RnSat, asc_float2half_rn_sat_merge, Odd, ASC_POSITION_ODD, ROUND_R, RS_ENABLE, PART_ODD)

TEST_FLOAT2HALF_M(Rna, asc_float2half_rna_merge, Even, ASC_POSITION_EVEN, ROUND_A, RS_DISABLE, PART_EVEN)
TEST_FLOAT2HALF_M(Rna, asc_float2half_rna_merge, Odd, ASC_POSITION_ODD, ROUND_A, RS_DISABLE, PART_ODD)
TEST_FLOAT2HALF_M(RnaSat, asc_float2half_rna_sat_merge, Even, ASC_POSITION_EVEN, ROUND_A, RS_ENABLE, PART_EVEN)
TEST_FLOAT2HALF_M(RnaSat, asc_float2half_rna_sat_merge, Odd, ASC_POSITION_ODD, ROUND_A, RS_ENABLE, PART_ODD)

TEST_FLOAT2HALF_M(Ro, asc_float2half_ro_merge, Even, ASC_POSITION_EVEN, ROUND_O, RS_DISABLE, PART_EVEN)
TEST_FLOAT2HALF_M(Ro, asc_float2half_ro_merge, Odd, ASC_POSITION_ODD, ROUND_O, RS_DISABLE, PART_ODD)
TEST_FLOAT2HALF_M(RoSat, asc_float2half_ro_sat_merge, Even, ASC_POSITION_EVEN, ROUND_O, RS_ENABLE, PART_EVEN)
TEST_FLOAT2HALF_M(RoSat, asc_float2half_ro_sat_merge, Odd, ASC_POSITION_ODD, ROUND_O, RS_ENABLE, PART_ODD)

TEST_FLOAT2HALF_M(Ru, asc_float2half_ru_merge, Even, ASC_POSITION_EVEN, ROUND_C, RS_DISABLE, PART_EVEN)
TEST_FLOAT2HALF_M(Ru, asc_float2half_ru_merge, Odd, ASC_POSITION_ODD, ROUND_C, RS_DISABLE, PART_ODD)
TEST_FLOAT2HALF_M(RuSat, asc_float2half_ru_sat_merge, Even, ASC_POSITION_EVEN, ROUND_C, RS_ENABLE, PART_EVEN)
TEST_FLOAT2HALF_M(RuSat, asc_float2half_ru_sat_merge, Odd, ASC_POSITION_ODD, ROUND_C, RS_ENABLE, PART_ODD)

TEST_FLOAT2HALF_M(Rz, asc_float2half_rz_merge, Even, ASC_POSITION_EVEN, ROUND_Z, RS_DISABLE, PART_EVEN)
TEST_FLOAT2HALF_M(Rz, asc_float2half_rz_merge, Odd, ASC_POSITION_ODD, ROUND_Z, RS_DISABLE, PART_ODD)
TEST_FLOAT2HALF_M(RzSat, asc_float2half_rz_sat_merge, Even, ASC_POSITION_EVEN, ROUND_Z, RS_ENABLE, PART_EVEN)
TEST_FLOAT2HALF_M(RzSat, asc_float2half_rz_sat_merge, Odd, ASC_POSITION_ODD, ROUND_Z, RS_ENABLE, PART_ODD)

#undef TEST_FLOAT2HALF_M

// ================asc_cast_merge================
#define TEST_CAST_MERGE_M(                                                                                          \
    class_name, round_const, sat_const, expected_round, expected_sat, position_name, position_value, expected_part) \
    class TestCastMerge##class_name##_##position_name : public testing::Test {};                                    \
                                                                                                                    \
    namespace {                                                                                                     \
    void vcvt_cast_merge_##class_name##_##position_name(                                                            \
        vector_half&, vector_float, vector_bool, Literal actual_round, Literal actual_sat, Literal actual_part,     \
        Literal actual_mode)                                                                                        \
    {                                                                                                               \
        EXPECT_EQ(actual_round, expected_round);                                                                    \
        EXPECT_EQ(actual_sat, expected_sat);                                                                        \
        EXPECT_EQ(actual_part, expected_part);                                                                      \
        EXPECT_EQ(actual_mode, MODE_MERGING);                                                                       \
    }                                                                                                               \
                                                                                                                    \
    TEST_F(TestCastMerge##class_name##_##position_name, InvokeVcvt)                                                 \
    {                                                                                                               \
        vector_half dst;                                                                                            \
        vector_float src;                                                                                           \
        vector_bool mask;                                                                                           \
        MOCKER_CPP(vcvt, void(vector_half&, vector_float, vector_bool, Literal, Literal, Literal, Literal))         \
            .times(1)                                                                                               \
            .will(invoke(vcvt_cast_merge_##class_name##_##position_name));                                          \
        asc_cast_merge(dst, src, mask, round_const, sat_const, position_value);                                     \
        GlobalMockObject::verify();                                                                                 \
    }                                                                                                               \
    }

TEST_CAST_MERGE_M(Rd, ASC_CAST_RD, ASC_CAST_NO_SAT, ROUND_F, RS_DISABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(Rd, ASC_CAST_RD, ASC_CAST_NO_SAT, ROUND_F, RS_DISABLE, Odd, ASC_POSITION_ODD, PART_ODD)
TEST_CAST_MERGE_M(RdSat, ASC_CAST_RD, ASC_CAST_SAT, ROUND_F, RS_ENABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(RdSat, ASC_CAST_RD, ASC_CAST_SAT, ROUND_F, RS_ENABLE, Odd, ASC_POSITION_ODD, PART_ODD)

TEST_CAST_MERGE_M(Rn, ASC_CAST_RN, ASC_CAST_NO_SAT, ROUND_R, RS_DISABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(Rn, ASC_CAST_RN, ASC_CAST_NO_SAT, ROUND_R, RS_DISABLE, Odd, ASC_POSITION_ODD, PART_ODD)
TEST_CAST_MERGE_M(RnSat, ASC_CAST_RN, ASC_CAST_SAT, ROUND_R, RS_ENABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(RnSat, ASC_CAST_RN, ASC_CAST_SAT, ROUND_R, RS_ENABLE, Odd, ASC_POSITION_ODD, PART_ODD)

TEST_CAST_MERGE_M(Rna, ASC_CAST_RNA, ASC_CAST_NO_SAT, ROUND_A, RS_DISABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(Rna, ASC_CAST_RNA, ASC_CAST_NO_SAT, ROUND_A, RS_DISABLE, Odd, ASC_POSITION_ODD, PART_ODD)
TEST_CAST_MERGE_M(RnaSat, ASC_CAST_RNA, ASC_CAST_SAT, ROUND_A, RS_ENABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(RnaSat, ASC_CAST_RNA, ASC_CAST_SAT, ROUND_A, RS_ENABLE, Odd, ASC_POSITION_ODD, PART_ODD)

TEST_CAST_MERGE_M(Ro, ASC_CAST_RO, ASC_CAST_NO_SAT, ROUND_O, RS_DISABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(Ro, ASC_CAST_RO, ASC_CAST_NO_SAT, ROUND_O, RS_DISABLE, Odd, ASC_POSITION_ODD, PART_ODD)
TEST_CAST_MERGE_M(RoSat, ASC_CAST_RO, ASC_CAST_SAT, ROUND_O, RS_ENABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(RoSat, ASC_CAST_RO, ASC_CAST_SAT, ROUND_O, RS_ENABLE, Odd, ASC_POSITION_ODD, PART_ODD)

TEST_CAST_MERGE_M(Ru, ASC_CAST_RU, ASC_CAST_NO_SAT, ROUND_C, RS_DISABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(Ru, ASC_CAST_RU, ASC_CAST_NO_SAT, ROUND_C, RS_DISABLE, Odd, ASC_POSITION_ODD, PART_ODD)
TEST_CAST_MERGE_M(RuSat, ASC_CAST_RU, ASC_CAST_SAT, ROUND_C, RS_ENABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(RuSat, ASC_CAST_RU, ASC_CAST_SAT, ROUND_C, RS_ENABLE, Odd, ASC_POSITION_ODD, PART_ODD)

TEST_CAST_MERGE_M(Rz, ASC_CAST_RZ, ASC_CAST_NO_SAT, ROUND_Z, RS_DISABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(Rz, ASC_CAST_RZ, ASC_CAST_NO_SAT, ROUND_Z, RS_DISABLE, Odd, ASC_POSITION_ODD, PART_ODD)
TEST_CAST_MERGE_M(RzSat, ASC_CAST_RZ, ASC_CAST_SAT, ROUND_Z, RS_ENABLE, Even, ASC_POSITION_EVEN, PART_EVEN)
TEST_CAST_MERGE_M(RzSat, ASC_CAST_RZ, ASC_CAST_SAT, ROUND_Z, RS_ENABLE, Odd, ASC_POSITION_ODD, PART_ODD)

#undef TEST_CAST_MERGE_M
