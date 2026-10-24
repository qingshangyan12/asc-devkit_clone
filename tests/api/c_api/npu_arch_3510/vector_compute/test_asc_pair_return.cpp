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
#include "c_api/experimental/reg_arith_exp.h"
#include "c_api/experimental/reg_permute_sel_exp.h"

namespace {
template <typename DataType>
void FillRegister(DataType& value, unsigned char pattern)
{
    std::memset(&value, pattern, sizeof(DataType));
}

template <typename ResultType, typename FirstType, typename SecondType>
void ExpectPairRegisters(const ResultType& result, const FirstType& expected_first, const SecondType& expected_second)
{
    EXPECT_EQ(0, std::memcmp(&result.first, &expected_first, sizeof(FirstType)));
    EXPECT_EQ(0, std::memcmp(&result.second, &expected_second, sizeof(SecondType)));
}

template <typename DataType>
void ExpectRegisterPattern(const DataType& value, unsigned char pattern)
{
    DataType expected{};
    FillRegister(expected, pattern);
    EXPECT_EQ(0, std::memcmp(&value, &expected, sizeof(DataType)));
}
} // namespace

class TestVectorComputeMullPairReturnCApi : public testing::Test {};

namespace {
template <typename DataType>
void MullPairStub(DataType& dst0, DataType& dst1, DataType src0, DataType src1, vector_bool mask)
{
    ExpectRegisterPattern(src0, 0x11);
    ExpectRegisterPattern(src1, 0x22);
    ExpectRegisterPattern(mask, 0x33);
    dst0 = src0;
    dst1 = src1;
}
} // namespace

TEST_F(TestVectorComputeMullPairReturnCApi, Int32PairReturn)
{
    vector_int32_t src0{};
    vector_int32_t src1{};
    vector_bool mask{};
    FillRegister(src0, 0x11);
    FillRegister(src1, 0x22);
    FillRegister(mask, 0x33);
    static_assert(std::is_same_v<decltype(asc_mull(src0, src1, mask)), asc_int32_t_int32_t_pair>);
    MOCKER_CPP(vmull, void(vector_int32_t&, vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool))
        .times(1)
        .will(invoke(MullPairStub<vector_int32_t>));
    auto result = asc_mull(src0, src1, mask);
    ExpectPairRegisters(result, src0, src1);
    GlobalMockObject::verify();
}

TEST_F(TestVectorComputeMullPairReturnCApi, Uint32PairReturn)
{
    vector_uint32_t src0{};
    vector_uint32_t src1{};
    vector_bool mask{};
    FillRegister(src0, 0x11);
    FillRegister(src1, 0x22);
    FillRegister(mask, 0x33);
    static_assert(std::is_same_v<decltype(asc_mull(src0, src1, mask)), asc_uint32_t_uint32_t_pair>);
    MOCKER_CPP(vmull, void(vector_uint32_t&, vector_uint32_t&, vector_uint32_t, vector_uint32_t, vector_bool))
        .times(1)
        .will(invoke(MullPairStub<vector_uint32_t>));
    auto result = asc_mull(src0, src1, mask);
    ExpectPairRegisters(result, src0, src1);
    GlobalMockObject::verify();
}

#define TEST_VECTOR_COMPUTE_CARRY_PAIR_RETURN(class_name, c_api_name, cce_name, data_type, result_type)         \
    class TestVectorCompute##class_name##data_type##PairReturnCApi : public testing::Test {                     \
    protected:                                                                                                  \
        void SetUp() {}                                                                                         \
        void TearDown() {}                                                                                      \
    };                                                                                                          \
                                                                                                                \
    namespace {                                                                                                 \
    void cce_name##_##class_name##_##data_type##_PairStub(                                                      \
        vector_bool& carry, data_type& dst, data_type src0, data_type src1, vector_bool mask)                   \
    {                                                                                                           \
        ExpectRegisterPattern(src0, 0x11);                                                                      \
        ExpectRegisterPattern(src1, 0x22);                                                                      \
        ExpectRegisterPattern(mask, 0x33);                                                                      \
        dst = src0;                                                                                             \
        carry = mask;                                                                                           \
    }                                                                                                           \
    }                                                                                                           \
                                                                                                                \
    TEST_F(TestVectorCompute##class_name##data_type##PairReturnCApi, c_api_name##_##data_type##_PairReturnSucc) \
    {                                                                                                           \
        data_type src0{};                                                                                       \
        data_type src1{};                                                                                       \
        vector_bool mask{};                                                                                     \
        FillRegister(src0, 0x11);                                                                               \
        FillRegister(src1, 0x22);                                                                               \
        FillRegister(mask, 0x33);                                                                               \
        static_assert(std::is_same_v<decltype(c_api_name(src0, src1, mask)), result_type>);                     \
                                                                                                                \
        MOCKER_CPP(cce_name, void(vector_bool&, data_type&, data_type, data_type, vector_bool))                 \
            .times(1)                                                                                           \
            .will(invoke(cce_name##_##class_name##_##data_type##_PairStub));                                    \
                                                                                                                \
        auto result = c_api_name(src0, src1, mask);                                                             \
        ExpectPairRegisters(result, src0, mask);                                                                \
        GlobalMockObject::verify();                                                                             \
    }

#define TEST_VECTOR_COMPUTE_CARRY_SRC_PAIR_RETURN(class_name, c_api_name, cce_name, data_type, result_type)          \
    class TestVectorCompute##class_name##data_type##PairReturnCApi : public testing::Test {                          \
    protected:                                                                                                       \
        void SetUp() {}                                                                                              \
        void TearDown() {}                                                                                           \
    };                                                                                                               \
                                                                                                                     \
    namespace {                                                                                                      \
    void cce_name##_##class_name##_##data_type##_PairStub(                                                           \
        vector_bool& carry, data_type& dst, data_type src0, data_type src1, vector_bool carry_src, vector_bool mask) \
    {                                                                                                                \
        ExpectRegisterPattern(src0, 0x11);                                                                           \
        ExpectRegisterPattern(src1, 0x22);                                                                           \
        ExpectRegisterPattern(carry_src, 0x33);                                                                      \
        ExpectRegisterPattern(mask, 0x44);                                                                           \
        dst = src0;                                                                                                  \
        carry = carry_src;                                                                                           \
    }                                                                                                                \
    }                                                                                                                \
                                                                                                                     \
    TEST_F(TestVectorCompute##class_name##data_type##PairReturnCApi, c_api_name##_##data_type##_PairReturnSucc)      \
    {                                                                                                                \
        data_type src0{};                                                                                            \
        data_type src1{};                                                                                            \
        vector_bool carry_src{};                                                                                     \
        vector_bool mask{};                                                                                          \
        FillRegister(src0, 0x11);                                                                                    \
        FillRegister(src1, 0x22);                                                                                    \
        FillRegister(carry_src, 0x33);                                                                               \
        FillRegister(mask, 0x44);                                                                                    \
        static_assert(std::is_same_v<decltype(c_api_name(src0, src1, carry_src, mask)), result_type>);               \
                                                                                                                     \
        MOCKER_CPP(cce_name, void(vector_bool&, data_type&, data_type, data_type, vector_bool, vector_bool))         \
            .times(1)                                                                                                \
            .will(invoke(cce_name##_##class_name##_##data_type##_PairStub));                                         \
                                                                                                                     \
        auto result = c_api_name(src0, src1, carry_src, mask);                                                       \
        ExpectPairRegisters(result, src0, carry_src);                                                                \
        GlobalMockObject::verify();                                                                                  \
    }

TEST_VECTOR_COMPUTE_CARRY_PAIR_RETURN(AddCarry, asc_add_carry, vaddc, vector_uint32_t, asc_uint32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_PAIR_RETURN(AddCarry, asc_add_carry, vaddc, vector_int32_t, asc_int32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_SRC_PAIR_RETURN(AddCarrySrc, asc_add_carry, vaddcs, vector_uint32_t, asc_uint32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_SRC_PAIR_RETURN(AddCarrySrc, asc_add_carry, vaddcs, vector_int32_t, asc_int32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_PAIR_RETURN(SubCarry, asc_sub_carry, vsubc, vector_uint32_t, asc_uint32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_PAIR_RETURN(SubCarry, asc_sub_carry, vsubc, vector_int32_t, asc_int32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_SRC_PAIR_RETURN(SubCarrySrc, asc_sub_carry, vsubcs, vector_uint32_t, asc_uint32_t_bool_pair);
TEST_VECTOR_COMPUTE_CARRY_SRC_PAIR_RETURN(SubCarrySrc, asc_sub_carry, vsubcs, vector_int32_t, asc_int32_t_bool_pair);

#define TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(class_name, c_api_name, cce_name, data_type, result_type)       \
    class TestVectorCompute##class_name##data_type##PairReturnCApi : public testing::Test {                     \
    protected:                                                                                                  \
        void SetUp() {}                                                                                         \
        void TearDown() {}                                                                                      \
    };                                                                                                          \
                                                                                                                \
    namespace {                                                                                                 \
    void cce_name##_##class_name##_##data_type##_PairStub(                                                      \
        data_type& dst0, data_type& dst1, data_type src0, data_type src1)                                       \
    {                                                                                                           \
        dst0 = src0;                                                                                            \
        dst1 = src1;                                                                                            \
    }                                                                                                           \
    }                                                                                                           \
                                                                                                                \
    TEST_F(TestVectorCompute##class_name##data_type##PairReturnCApi, c_api_name##_##data_type##_PairReturnSucc) \
    {                                                                                                           \
        data_type src0{};                                                                                       \
        data_type src1{};                                                                                       \
        FillRegister(src0, 0x11);                                                                               \
        FillRegister(src1, 0x22);                                                                               \
        static_assert(std::is_same_v<decltype(c_api_name(src0, src1)), result_type>);                           \
                                                                                                                \
        MOCKER_CPP(cce_name, void(data_type&, data_type&, data_type, data_type))                                \
            .times(1)                                                                                           \
            .will(invoke(cce_name##_##class_name##_##data_type##_PairStub));                                    \
                                                                                                                \
        auto result = c_api_name(src0, src1);                                                                   \
        ExpectPairRegisters(result, src0, src1);                                                                \
        GlobalMockObject::verify();                                                                             \
    }

#define TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN_HIF8(class_name, c_api_name, cce_name, result_type)  \
    class TestVectorCompute##class_name##vector_hifloat8_t##PairReturnCApi : public testing::Test {  \
    protected:                                                                                       \
        void SetUp() {}                                                                              \
        void TearDown() {}                                                                           \
    };                                                                                               \
                                                                                                     \
    namespace {                                                                                      \
    void cce_name##_##class_name##_vector_hifloat8_t_PairStub(                                       \
        vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1)        \
    {                                                                                                \
        dst0 = src0;                                                                                 \
        dst1 = src1;                                                                                 \
    }                                                                                                \
    }                                                                                                \
                                                                                                     \
    TEST_F(                                                                                          \
        TestVectorCompute##class_name##vector_hifloat8_t##PairReturnCApi,                            \
        c_api_name##_vector_hifloat8_t_PairReturnSucc)                                               \
    {                                                                                                \
        vector_hifloat8_t src0{};                                                                    \
        vector_hifloat8_t src1{};                                                                    \
        FillRegister(src0, 0x11);                                                                    \
        FillRegister(src1, 0x22);                                                                    \
        static_assert(std::is_same_v<decltype(c_api_name(src0, src1)), result_type>);                \
                                                                                                     \
        MOCKER_CPP(cce_name, void(vector_uint8_t&, vector_uint8_t&, vector_uint8_t, vector_uint8_t)) \
            .times(1)                                                                                \
            .will(invoke(cce_name##_##class_name##_vector_hifloat8_t_PairStub));                     \
                                                                                                     \
        auto result = c_api_name(src0, src1);                                                        \
        ExpectPairRegisters(result, src0, src1);                                                     \
        GlobalMockObject::verify();                                                                  \
    }

TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_uint8_t, asc_uint8_t_uint8_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_int8_t, asc_int8_t_int8_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_uint16_t, asc_uint16_t_uint16_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_int16_t, asc_int16_t_int16_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_uint32_t, asc_uint32_t_uint32_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_int32_t, asc_int32_t_int32_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_half, asc_half_half_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_float, asc_float_float_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_fp8_e8m0_t, asc_fp8_e8m0_t_fp8_e8m0_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_fp8_e5m2_t, asc_fp8_e5m2_t_fp8_e5m2_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(
    Intlv, asc_intlv, vintlv, vector_fp8_e4m3fn_t, asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Intlv, asc_intlv, vintlv, vector_bfloat16_t, asc_bfloat16_t_bfloat16_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN_HIF8(Intlv, asc_intlv, vintlv, asc_hifloat8_t_hifloat8_t_pair);

TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_int32_t, asc_int32_t_int32_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_uint32_t, asc_uint32_t_uint32_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_int16_t, asc_int16_t_int16_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_uint16_t, asc_uint16_t_uint16_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_int8_t, asc_int8_t_int8_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_uint8_t, asc_uint8_t_uint8_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(
    Deintlv, asc_deintlv, vdintlv, vector_fp8_e4m3fn_t, asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN_HIF8(Deintlv, asc_deintlv, vdintlv, asc_hifloat8_t_hifloat8_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(
    Deintlv, asc_deintlv, vdintlv, vector_fp8_e5m2_t, asc_fp8_e5m2_t_fp8_e5m2_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(
    Deintlv, asc_deintlv, vdintlv, vector_fp8_e8m0_t, asc_fp8_e8m0_t_fp8_e8m0_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(
    Deintlv, asc_deintlv, vdintlv, vector_bfloat16_t, asc_bfloat16_t_bfloat16_t_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_float, asc_float_float_pair);
TEST_VECTOR_COMPUTE_REORDER_PAIR_RETURN(Deintlv, asc_deintlv, vdintlv, vector_half, asc_half_half_pair);

#define TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(class_name, c_api_name, cce_name, result_type)                   \
    class TestVectorCompute##class_name##c_api_name##MaskPairReturnCApi : public testing::Test {                      \
    protected:                                                                                                        \
        void SetUp() {}                                                                                               \
        void TearDown() {}                                                                                            \
    };                                                                                                                \
                                                                                                                      \
    namespace {                                                                                                       \
    void cce_name##_##class_name##_PairStub(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1) \
    {                                                                                                                 \
        dst0 = src0;                                                                                                  \
        dst1 = src1;                                                                                                  \
    }                                                                                                                 \
    }                                                                                                                 \
                                                                                                                      \
    TEST_F(TestVectorCompute##class_name##c_api_name##MaskPairReturnCApi, c_api_name##_PairReturnSucc)                \
    {                                                                                                                 \
        vector_bool src0{};                                                                                           \
        vector_bool src1{};                                                                                           \
        FillRegister(src0, 0x11);                                                                                     \
        FillRegister(src1, 0x22);                                                                                     \
        static_assert(std::is_same_v<decltype(c_api_name(src0, src1)), result_type>);                                 \
                                                                                                                      \
        MOCKER_CPP(cce_name, void(vector_bool&, vector_bool&, vector_bool, vector_bool))                              \
            .times(1)                                                                                                 \
            .will(invoke(cce_name##_##class_name##_PairStub));                                                        \
                                                                                                                      \
        auto result = c_api_name(src0, src1);                                                                         \
        ExpectPairRegisters(result, src0, src1);                                                                      \
        GlobalMockObject::verify();                                                                                   \
    }

TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(Intlv, asc_intlv_b8, pintlv_b8, asc_bool_bool_pair);
TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(Intlv, asc_intlv_b16, pintlv_b16, asc_bool_bool_pair);
TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(Intlv, asc_intlv_b32, pintlv_b32, asc_bool_bool_pair);
TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(Deintlv, asc_deintlv_b8, pdintlv_b8, asc_bool_bool_pair);
TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(Deintlv, asc_deintlv_b16, pdintlv_b16, asc_bool_bool_pair);
TEST_VECTOR_COMPUTE_MASK_REORDER_PAIR_RETURN(Deintlv, asc_deintlv_b32, pdintlv_b32, asc_bool_bool_pair);
