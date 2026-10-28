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

#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/experimental/vector_ops.h"
#include "tensor_api/tensor.h"

class tensor_api_vector_compare_and_select_3510 : public testing::Test {
protected:
    void SetUp() override { AscendC::SetGCoreType(2); }
    void TearDown() override
    {
        AscendC::SetGCoreType(0);
        GlobalMockObject::verify();
    }
};

namespace {

vector_bool expected_input_mask = 0;

template <typename RegT, typename OtherT, vector_bool Result>
void compare_result_stub(vector_bool& dst, RegT, OtherT, vector_bool mask)
{
    EXPECT_EQ(mask, expected_input_mask);
    dst = Result;
}

#define MOCK_COMPARE_INSTR(Instr, RegT, OtherT, Result)              \
    MOCKER_CPP(Instr, void(vector_bool&, RegT, OtherT, vector_bool)) \
        .times(1)                                                    \
        .will(invoke(compare_result_stub<RegT, OtherT, Result>))

template <typename RegT>
void mock_vector_compare(vector_bool maskValue)
{
    expected_input_mask = maskValue;
    MOCK_COMPARE_INSTR(vcmp_eq, RegT, RegT, 2);
    MOCK_COMPARE_INSTR(vcmp_ne, RegT, RegT, 3);
    MOCK_COMPARE_INSTR(vcmp_lt, RegT, RegT, 4);
    MOCK_COMPARE_INSTR(vcmp_le, RegT, RegT, 5);
    MOCK_COMPARE_INSTR(vcmp_gt, RegT, RegT, 6);
    MOCK_COMPARE_INSTR(vcmp_ge, RegT, RegT, 7);
}

template <typename RegT, typename T>
void mock_scalar_compare(vector_bool maskValue)
{
    expected_input_mask = maskValue;
    MOCK_COMPARE_INSTR(vcmps_eq, RegT, T, 2);
    MOCK_COMPARE_INSTR(vcmps_ne, RegT, T, 3);
    MOCK_COMPARE_INSTR(vcmps_lt, RegT, T, 4);
    MOCK_COMPARE_INSTR(vcmps_le, RegT, T, 5);
    MOCK_COMPARE_INSTR(vcmps_gt, RegT, T, 6);
    MOCK_COMPARE_INSTR(vcmps_ge, RegT, T, 7);
}

#undef MOCK_COMPARE_INSTR

// Comparison output is stored in .reg; the current implementation leaves .mask uninitialized.
template <typename ResultT>
void expect_compare_result(const ResultT& result, vector_bool expected)
{
    static_assert(AscendC::Std::is_same_v<ResultT, asc::te::experimental::reg_tensor<bool>>);
    EXPECT_EQ(result.reg, expected);
}

} // namespace

template <typename T>
__aicore__ inline void test_compare_reg_tensor(vector_bool maskValue)
{
    asc::te::experimental::reg_tensor<T> src0{};
    asc::te::experimental::reg_tensor<T> src1{};
    src0.mask = maskValue;
    src1.mask = maskValue ^ 1;
    mock_vector_compare<typename decltype(src0)::reg_type>(maskValue);

    auto eq = src0 == src1;
    auto ne = src0 != src1;
    auto lt = src0 < src1;
    auto le = src0 <= src1;
    auto gt = src0 > src1;
    auto ge = src0 >= src1;
    expect_compare_result(eq, 2);
    expect_compare_result(ne, 3);
    expect_compare_result(lt, 4);
    expect_compare_result(le, 5);
    expect_compare_result(gt, 6);
    expect_compare_result(ge, 7);
    GlobalMockObject::verify();
}

template <typename T>
__aicore__ inline void test_compare_reg_tensor_scalar(vector_bool maskValue)
{
    asc::te::experimental::reg_tensor<T> src0{};
    T src1{};
    src0.mask = maskValue;
    mock_scalar_compare<typename decltype(src0)::reg_type, T>(maskValue);

    auto eq = src0 == src1;
    auto ne = src0 != src1;
    auto lt = src0 < src1;
    auto le = src0 <= src1;
    auto gt = src0 > src1;
    auto ge = src0 >= src1;

    expect_compare_result(eq, 2);
    expect_compare_result(ne, 3);
    expect_compare_result(lt, 4);
    expect_compare_result(le, 5);
    expect_compare_result(gt, 6);
    expect_compare_result(ge, 7);
    GlobalMockObject::verify();
}

template <typename T>
__aicore__ inline void test_compare_scalar_reg_tensor(vector_bool maskValue)
{
    T src0{};
    asc::te::experimental::reg_tensor<T> src1{};
    src1.mask = maskValue;
    mock_scalar_compare<typename decltype(src1)::reg_type, T>(maskValue);

    auto eq = src0 == src1;
    auto ne = src0 != src1;
    auto lt = src0 < src1;
    auto le = src0 <= src1;
    auto gt = src0 > src1;
    auto ge = src0 >= src1;
    expect_compare_result(eq, 2);
    expect_compare_result(ne, 3);
    expect_compare_result(lt, 6);
    expect_compare_result(le, 7);
    expect_compare_result(gt, 4);
    expect_compare_result(ge, 5);
    GlobalMockObject::verify();
}

template <typename T>
__aicore__ inline void test_select(vector_bool maskValue)
{
    asc::te::experimental::reg_tensor<bool> condition{};
    asc::te::experimental::reg_tensor<T> src0{};
    asc::te::experimental::reg_tensor<T> src1{};
    src0.mask = maskValue;
    src1.mask = maskValue ^ 1;

    auto dst = asc::te::experimental::select(condition, src0, src1);
    static_assert(AscendC::Std::is_same_v<decltype(dst), asc::te::experimental::reg_tensor<T>>);
    EXPECT_EQ(dst.mask, src0.mask);
}

#define COMPARE_AND_SELECT_TEST(Function, DataType)                          \
    TEST_F(tensor_api_vector_compare_and_select_3510, Function##_##DataType) \
    {                                                                        \
        for (vector_bool maskValue = 0; maskValue < 2; ++maskValue) {        \
            SCOPED_TRACE(maskValue);                                         \
            test_##Function<DataType>(maskValue);                            \
        }                                                                    \
    }

#define SELECT_TYPE_TEST(DataType) COMPARE_AND_SELECT_TEST(select, DataType)

#define COMPARE_TYPE_TESTS(DataType)                             \
    COMPARE_AND_SELECT_TEST(compare_reg_tensor, DataType)        \
    COMPARE_AND_SELECT_TEST(compare_reg_tensor_scalar, DataType) \
    COMPARE_AND_SELECT_TEST(compare_scalar_reg_tensor, DataType)

COMPARE_TYPE_TESTS(uint8_t)
COMPARE_TYPE_TESTS(int8_t)
COMPARE_TYPE_TESTS(uint16_t)
COMPARE_TYPE_TESTS(int16_t)
COMPARE_TYPE_TESTS(half)
COMPARE_TYPE_TESTS(bfloat16_t)
COMPARE_TYPE_TESTS(uint32_t)
COMPARE_TYPE_TESTS(int32_t)
COMPARE_TYPE_TESTS(float)

SELECT_TYPE_TEST(bool)
SELECT_TYPE_TEST(uint8_t)
SELECT_TYPE_TEST(int8_t)
SELECT_TYPE_TEST(fp8_e4m3fn_t)
SELECT_TYPE_TEST(hifloat8_t)
SELECT_TYPE_TEST(fp8_e5m2_t)
SELECT_TYPE_TEST(uint16_t)
SELECT_TYPE_TEST(int16_t)
SELECT_TYPE_TEST(half)
SELECT_TYPE_TEST(bfloat16_t)
SELECT_TYPE_TEST(uint32_t)
SELECT_TYPE_TEST(int32_t)
SELECT_TYPE_TEST(float)
