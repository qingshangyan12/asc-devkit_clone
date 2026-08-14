/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the license.
 */

#include <array>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"

namespace {

constexpr uint8_t SRC0_TOKEN = 0x11;
constexpr uint8_t SRC1_TOKEN = 0x22;
constexpr uint8_t BROADCAST_TOKEN = 0x33;
constexpr uint8_t RESULT_TOKEN = 0x44;
constexpr uint8_t MASK_TOKEN = 0x5A;

template <typename T>
void fill_bytes(T& value, uint8_t byte)
{
    std::array<uint8_t, sizeof(T)> bytes {};
    bytes.fill(byte);
    std::memcpy(&value, bytes.data(), bytes.size());
}

template <typename T>
bool has_byte_value(const T& value, uint8_t byte)
{
    std::array<uint8_t, sizeof(T)> bytes {};
    std::memcpy(bytes.data(), &value, bytes.size());
    for (uint8_t current : bytes) {
        if (current != byte) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool same_bytes(const T& lhs, const T& rhs)
{
    return std::memcmp(&lhs, &rhs, sizeof(T)) == 0;
}

template <typename T>
asc::te::reg_tensor<T> make_reg(uint8_t reg_token = SRC0_TOKEN)
{
    asc::te::reg_tensor<T> value {};
    fill_bytes(value.reg, reg_token);
    fill_bytes(value.mask, MASK_TOKEN);
    return value;
}

template <typename T>
void expect_result(const asc::te::reg_tensor<T>& result, const vector_bool& mask,
    uint8_t result_token = RESULT_TOKEN)
{
    EXPECT_TRUE(has_byte_value(result.reg, result_token));
    EXPECT_TRUE(same_bytes(result.mask, mask));
}

struct capture {
    vector_bool expected_mask {};
    bool masks_match = true;
    bool operands_match = true;
    size_t calls = 0;
    size_t scalar_calls = 0;
    size_t duplicate_calls = 0;
    std::array<float, 8> scalars {};
};

capture g_capture;

void reset_capture(const vector_bool& mask)
{
    g_capture = {};
    g_capture.expected_mask = mask;
}

void capture_mask(const vector_bool& mask)
{
    g_capture.masks_match = g_capture.masks_match && same_bytes(mask, g_capture.expected_mask);
}

void unary_float_stub(vector_float& dst, vector_float, vector_bool mask, Literal)
{
    capture_mask(mask);
    ++g_capture.calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void ln_stub(vector_float& dst, vector_float, vector_bool mask, Literal)
{
    capture_mask(mask);
    ++g_capture.calls;
    fill_bytes(dst, SRC1_TOKEN);
}

void scalar_float_stub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match &&
        (has_byte_value(src, SRC0_TOKEN) || has_byte_value(src, SRC1_TOKEN));
    g_capture.scalars[g_capture.scalar_calls++] = scalar;
    fill_bytes(dst, RESULT_TOKEN);
}

void binary_float_stub(vector_float& dst, vector_float, vector_float, vector_bool mask, Literal)
{
    capture_mask(mask);
    ++g_capture.calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void duplicate_float_stub(vector_float& dst, float scalar, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.scalars[g_capture.scalar_calls++] = scalar;
    ++g_capture.duplicate_calls;
    fill_bytes(dst, BROADCAST_TOKEN);
}

void ordered_float_stub(vector_float& dst, vector_float left, vector_float right, vector_bool mask, Literal)
{
    capture_mask(mask);
    constexpr std::array<uint8_t, 3> left_tokens {SRC0_TOKEN, SRC0_TOKEN, BROADCAST_TOKEN};
    constexpr std::array<uint8_t, 3> right_tokens {SRC1_TOKEN, BROADCAST_TOKEN, SRC0_TOKEN};
    size_t call = g_capture.calls++;
    g_capture.operands_match = g_capture.operands_match && call < left_tokens.size() &&
        has_byte_value(left, left_tokens[call]) && has_byte_value(right, right_tokens[call]);
    fill_bytes(dst, RESULT_TOKEN);
}

void prelu_stub(vector_float& dst, vector_float src, vector_float slope, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.operands_match = has_byte_value(src, SRC0_TOKEN) && has_byte_value(slope, SRC1_TOKEN);
    fill_bytes(dst, RESULT_TOKEN);
}

void leaky_relu_stub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match && has_byte_value(src, SRC0_TOKEN);
    g_capture.scalars[g_capture.scalar_calls++] = scalar;
    fill_bytes(dst, RESULT_TOKEN);
}

class test_tensor_api_basic_arithmetic : public testing::Test {
protected:
    void SetUp() override
    {
        AscendC::SetGCoreType(2);
    }

    void TearDown() override
    {
        AscendC::SetGCoreType(0);
    }
};

template <typename T>
__aicore__ inline void compile_unary_arithmetic()
{
    asc::te::reg_tensor<T> src {};
    (void)asc::te::abs(src);
    (void)-src;
}

template <typename T>
__aicore__ inline void compile_float_math()
{
    asc::te::reg_tensor<T> src {};
    asc::te::reg_tensor<T> slope {};
    (void)asc::te::exp(src);
    (void)asc::te::sqrt(src);
    (void)asc::te::log(src);
    (void)asc::te::log2(src);
    (void)asc::te::log10(src);
    (void)asc::te::prelu(src, slope);
    (void)asc::te::leaky_relu(src, T {});
}

template <typename T>
__aicore__ inline void compile_relu()
{
    asc::te::reg_tensor<T> src {};
    (void)asc::te::relu(src);
}

template <typename T>
__aicore__ inline void compile_add_sub()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 + src1);
    (void)(src0 + scalar);
    (void)(scalar + src0);
    (void)(src0 - src1);
    (void)(src0 - scalar);
    (void)(scalar - src0);
}

template <typename T>
__aicore__ inline void compile_mul()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    (void)(src0 * src1);
}

template <typename T>
__aicore__ inline void compile_mul_div_scalar()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 * scalar);
    (void)(scalar * src0);
    (void)(src0 / src1);
    (void)(src0 / scalar);
    (void)(scalar / src0);
}

template <typename T>
__aicore__ inline void compile_min_max()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)asc::te::max(src0, src1);
    (void)asc::te::max(src0, scalar);
    (void)asc::te::max(scalar, src0);
    (void)asc::te::min(src0, src1);
    (void)asc::te::min(src0, scalar);
    (void)asc::te::min(scalar, src0);
}

template <typename T>
__aicore__ inline void compile_carry_and_mull()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    asc::te::reg_tensor<T> high {};
    asc::te::reg_tensor<bool> carry {};
    (void)asc::te::add(carry, src0, src1);
    (void)asc::te::add_c(carry, src0, src1, carry);
    (void)asc::te::sub(carry, src0, src1);
    (void)asc::te::sub_c(carry, src0, src1, carry);
    (void)asc::te::mull(high, src0, src1);
}

template <typename T>
__aicore__ inline void compile_mull()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    asc::te::reg_tensor<T> high {};
    (void)asc::te::mull(high, src0, src1);
}

TEST_F(test_tensor_api_basic_arithmetic, compile_public_apis_and_supported_types)
{
    AscendC::SetGCoreType(0);
    asc::te::reg_tensor<float> float_src {};
    asc::te::reg_tensor<float> float_slope {};
    asc::te::reg_tensor<int32_t> int_src0 {};
    asc::te::reg_tensor<int32_t> int_src1 {};
    asc::te::reg_tensor<uint32_t> uint_src0 {};
    asc::te::reg_tensor<uint32_t> uint_src1 {};
    asc::te::reg_tensor<bool> carry {};

    static_assert(std::is_same_v<decltype(asc::te::log(float_src)), asc::te::reg_tensor<float>>);
    static_assert(std::is_same_v<decltype(asc::te::prelu(float_src, float_slope)), asc::te::reg_tensor<float>>);
    static_assert(std::is_same_v<decltype(asc::te::mull(int_src1, int_src0, int_src1)),
        asc::te::reg_tensor<int32_t>>);

    (void)asc::te::exp(float_src);
    (void)asc::te::sqrt(float_src);
    (void)asc::te::log(float_src);
    (void)asc::te::log2(float_src);
    (void)asc::te::log10(float_src);
    (void)asc::te::relu(float_src);
    (void)asc::te::prelu(float_src, float_slope);
    (void)asc::te::leaky_relu(float_src, 0.1f);
    (void)asc::te::add(carry, uint_src0, uint_src1);
    (void)asc::te::add_c(carry, uint_src0, uint_src1, carry);
    (void)asc::te::sub(carry, uint_src0, uint_src1);
    (void)asc::te::sub_c(carry, uint_src0, uint_src1, carry);
    (void)asc::te::mull(int_src1, int_src0, int_src1);

    compile_unary_arithmetic<int8_t>();
    compile_unary_arithmetic<int16_t>();
    compile_unary_arithmetic<half>();
    compile_unary_arithmetic<int32_t>();
    compile_unary_arithmetic<float>();
    compile_float_math<half>();
    compile_float_math<float>();
    compile_relu<half>();
    compile_relu<int32_t>();
    compile_relu<float>();

    compile_add_sub<int8_t>();
    compile_add_sub<uint8_t>();
    compile_add_sub<int16_t>();
    compile_add_sub<uint16_t>();
    compile_add_sub<half>();
    compile_add_sub<bfloat16_t>();
    compile_add_sub<int32_t>();
    compile_add_sub<uint32_t>();
    compile_add_sub<float>();

    compile_mul<int16_t>();
    compile_mul<uint16_t>();
    compile_mul<half>();
    compile_mul<bfloat16_t>();
    compile_mul<int32_t>();
    compile_mul<uint32_t>();
    compile_mul<float>();

    compile_mul_div_scalar<int16_t>();
    compile_mul_div_scalar<uint16_t>();
    compile_mul_div_scalar<half>();
    compile_mul_div_scalar<int32_t>();
    compile_mul_div_scalar<uint32_t>();
    compile_mul_div_scalar<float>();

    compile_min_max<int8_t>();
    compile_min_max<uint8_t>();
    compile_min_max<int16_t>();
    compile_min_max<uint16_t>();
    compile_min_max<half>();
    compile_min_max<bfloat16_t>();
    compile_min_max<int32_t>();
    compile_min_max<uint32_t>();
    compile_min_max<float>();
    compile_carry_and_mull<uint32_t>();
    compile_mull<int32_t>();
    static_assert(!asc::te::detail::supports_carry_v<int32_t>);
}

TEST_F(test_tensor_api_basic_arithmetic, unary_and_log_apis_forward_mask)
{
    auto src = make_reg<float>();
    reset_capture(src.mask);
    MOCKER_CPP(vabs, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(unary_float_stub));
    MOCKER_CPP(vexp, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(unary_float_stub));
    MOCKER_CPP(vsqrt, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(unary_float_stub));
    MOCKER_CPP(vln, void(vector_float&, vector_float, vector_bool, Literal)).times(3).will(invoke(ln_stub));
    MOCKER_CPP(vneg, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(unary_float_stub));
    MOCKER_CPP(vrelu, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(unary_float_stub));
    MOCKER_CPP(vmuls, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(scalar_float_stub));

    expect_result(asc::te::abs(src), src.mask);
    expect_result(asc::te::exp(src), src.mask);
    expect_result(asc::te::sqrt(src), src.mask);
    expect_result(asc::te::log(src), src.mask, SRC1_TOKEN);
    expect_result(asc::te::log2(src), src.mask);
    expect_result(asc::te::log10(src), src.mask);
    expect_result(-src, src.mask);
    expect_result(asc::te::relu(src), src.mask);

    GlobalMockObject::verify();
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 1.4426950408889634f);
    EXPECT_FLOAT_EQ(g_capture.scalars[1], 0.4342944819032518f);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_basic_arithmetic, activation_apis_preserve_operands_and_mask)
{
    auto src = make_reg<float>();
    auto slope = make_reg<float>(SRC1_TOKEN);
    reset_capture(src.mask);
    MOCKER_CPP(vprelu, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(prelu_stub));
    MOCKER_CPP(vlrelu, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(1)
        .will(invoke(leaky_relu_stub));

    expect_result(asc::te::prelu(src, slope), src.mask);
    expect_result(asc::te::leaky_relu(src, 0.2f), src.mask);

    GlobalMockObject::verify();
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 0.2f);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_basic_arithmetic, add_and_multiply_choose_register_or_scalar_intrinsic)
{
    auto src0 = make_reg<float>();
    auto src1 = make_reg<float>(SRC1_TOKEN);
    reset_capture(src0.mask);
    MOCKER_CPP(vadd, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_float_stub));
    MOCKER_CPP(vadds, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(scalar_float_stub));
    MOCKER_CPP(vmul, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_float_stub));
    MOCKER_CPP(vmuls, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(scalar_float_stub));

    expect_result(src0 + src1, src0.mask);
    expect_result(src0 + 2.0f, src0.mask);
    expect_result(2.0f + src0, src0.mask);
    expect_result(src0 * src1, src0.mask);
    expect_result(src0 * 3.0f, src0.mask);
    expect_result(3.0f * src0, src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 2U);
    EXPECT_EQ(g_capture.scalar_calls, 4U);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_basic_arithmetic, subtract_and_divide_preserve_operand_order)
{
    auto src0 = make_reg<float>();
    auto src1 = make_reg<float>(SRC1_TOKEN);
    reset_capture(src0.mask);
    MOCKER_CPP(vdup, void(vector_float&, float, vector_bool, Literal))
        .times(4)
        .will(invoke(duplicate_float_stub));
    MOCKER_CPP(vsub, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(ordered_float_stub));

    expect_result(src0 - src1, src0.mask);
    expect_result(src0 - 4.0f, src0.mask);
    expect_result(4.0f - src0, src0.mask);
    EXPECT_EQ(g_capture.calls, 3U);
    g_capture.calls = 0;

    MOCKER_CPP(vdiv, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(ordered_float_stub));
    expect_result(src0 / src1, src0.mask);
    expect_result(src0 / 8.0f, src0.mask);
    expect_result(8.0f / src0, src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 3U);
    EXPECT_EQ(g_capture.duplicate_calls, 4U);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

} // namespace
