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
void FillBytes(T& value, uint8_t byte)
{
    std::array<uint8_t, sizeof(T)> bytes {};
    bytes.fill(byte);
    std::memcpy(&value, bytes.data(), bytes.size());
}

template <typename T>
bool HasByteValue(const T& value, uint8_t byte)
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
bool SameBytes(const T& lhs, const T& rhs)
{
    return std::memcmp(&lhs, &rhs, sizeof(T)) == 0;
}

template <typename T>
asc::te::reg_tensor<T> MakeReg(uint8_t reg_token = SRC0_TOKEN)
{
    asc::te::reg_tensor<T> value {};
    FillBytes(value.reg, reg_token);
    FillBytes(value.mask, MASK_TOKEN);
    return value;
}

template <typename T>
void ExpectResult(const asc::te::reg_tensor<T>& result, const vector_bool& mask,
    uint8_t result_token = RESULT_TOKEN)
{
    EXPECT_TRUE(HasByteValue(result.reg, result_token));
    EXPECT_TRUE(SameBytes(result.mask, mask));
}

struct Capture {
    vector_bool expected_mask {};
    bool masks_match = true;
    bool operands_match = true;
    size_t calls = 0;
    size_t scalar_calls = 0;
    size_t duplicate_calls = 0;
    std::array<float, 8> scalars {};
};

Capture g_capture;

void ResetCapture(const vector_bool& mask)
{
    g_capture = {};
    g_capture.expected_mask = mask;
}

void CaptureMask(const vector_bool& mask)
{
    g_capture.masks_match = g_capture.masks_match && SameBytes(mask, g_capture.expected_mask);
}

void UnaryFloatStub(vector_float& dst, vector_float, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++g_capture.calls;
    FillBytes(dst, RESULT_TOKEN);
}

void LnStub(vector_float& dst, vector_float, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++g_capture.calls;
    FillBytes(dst, SRC1_TOKEN);
}

void ScalarFloatStub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match &&
        (HasByteValue(src, SRC0_TOKEN) || HasByteValue(src, SRC1_TOKEN));
    g_capture.scalars[g_capture.scalar_calls++] = scalar;
    FillBytes(dst, RESULT_TOKEN);
}

void BinaryFloatStub(vector_float& dst, vector_float, vector_float, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++g_capture.calls;
    FillBytes(dst, RESULT_TOKEN);
}

void DuplicateFloatStub(vector_float& dst, float scalar, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.scalars[g_capture.scalar_calls++] = scalar;
    ++g_capture.duplicate_calls;
    FillBytes(dst, BROADCAST_TOKEN);
}

void OrderedFloatStub(vector_float& dst, vector_float left, vector_float right, vector_bool mask, Literal)
{
    CaptureMask(mask);
    constexpr std::array<uint8_t, 3> left_tokens {SRC0_TOKEN, SRC0_TOKEN, BROADCAST_TOKEN};
    constexpr std::array<uint8_t, 3> right_tokens {SRC1_TOKEN, BROADCAST_TOKEN, SRC0_TOKEN};
    size_t call = g_capture.calls++;
    g_capture.operands_match = g_capture.operands_match && call < left_tokens.size() &&
        HasByteValue(left, left_tokens[call]) && HasByteValue(right, right_tokens[call]);
    FillBytes(dst, RESULT_TOKEN);
}

void PreluStub(vector_float& dst, vector_float src, vector_float slope, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.operands_match = HasByteValue(src, SRC0_TOKEN) && HasByteValue(slope, SRC1_TOKEN);
    FillBytes(dst, RESULT_TOKEN);
}

void LeakyReluStub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match && HasByteValue(src, SRC0_TOKEN);
    g_capture.scalars[g_capture.scalar_calls++] = scalar;
    FillBytes(dst, RESULT_TOKEN);
}

class TestTensorApiBasicArithmetic : public testing::Test {
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
__aicore__ inline void CompileUnaryArithmetic()
{
    asc::te::reg_tensor<T> src {};
    (void)asc::te::abs(src);
    (void)-src;
}

template <typename T>
__aicore__ inline void CompileFloatMath()
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
__aicore__ inline void CompileRelu()
{
    asc::te::reg_tensor<T> src {};
    (void)asc::te::relu(src);
}

template <typename T>
__aicore__ inline void CompileAddSub()
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
__aicore__ inline void CompileMul()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    (void)(src0 * src1);
}

template <typename T>
__aicore__ inline void CompileMulDivScalar()
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
__aicore__ inline void CompileMinMax()
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
__aicore__ inline void CompileCarryAndMull()
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
__aicore__ inline void CompileMull()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    asc::te::reg_tensor<T> high {};
    (void)asc::te::mull(high, src0, src1);
}

TEST_F(TestTensorApiBasicArithmetic, CompilePublicApisAndSupportedTypes)
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

    CompileUnaryArithmetic<int8_t>();
    CompileUnaryArithmetic<int16_t>();
    CompileUnaryArithmetic<half>();
    CompileUnaryArithmetic<int32_t>();
    CompileUnaryArithmetic<float>();
    CompileFloatMath<half>();
    CompileFloatMath<float>();
    CompileRelu<half>();
    CompileRelu<int32_t>();
    CompileRelu<float>();

    CompileAddSub<int8_t>();
    CompileAddSub<uint8_t>();
    CompileAddSub<int16_t>();
    CompileAddSub<uint16_t>();
    CompileAddSub<half>();
    CompileAddSub<bfloat16_t>();
    CompileAddSub<int32_t>();
    CompileAddSub<uint32_t>();
    CompileAddSub<float>();

    CompileMul<int16_t>();
    CompileMul<uint16_t>();
    CompileMul<half>();
    CompileMul<bfloat16_t>();
    CompileMul<int32_t>();
    CompileMul<uint32_t>();
    CompileMul<float>();

    CompileMulDivScalar<int16_t>();
    CompileMulDivScalar<uint16_t>();
    CompileMulDivScalar<half>();
    CompileMulDivScalar<int32_t>();
    CompileMulDivScalar<uint32_t>();
    CompileMulDivScalar<float>();

    CompileMinMax<int8_t>();
    CompileMinMax<uint8_t>();
    CompileMinMax<int16_t>();
    CompileMinMax<uint16_t>();
    CompileMinMax<half>();
    CompileMinMax<bfloat16_t>();
    CompileMinMax<int32_t>();
    CompileMinMax<uint32_t>();
    CompileMinMax<float>();
    CompileCarryAndMull<uint32_t>();
    CompileMull<int32_t>();
    static_assert(!asc::te::detail::supports_carry_v<int32_t>);
}

TEST_F(TestTensorApiBasicArithmetic, UnaryAndLogApisForwardMask)
{
    auto src = MakeReg<float>();
    ResetCapture(src.mask);
    MOCKER_CPP(vabs, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(UnaryFloatStub));
    MOCKER_CPP(vexp, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(UnaryFloatStub));
    MOCKER_CPP(vsqrt, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(UnaryFloatStub));
    MOCKER_CPP(vln, void(vector_float&, vector_float, vector_bool, Literal)).times(3).will(invoke(LnStub));
    MOCKER_CPP(vneg, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(UnaryFloatStub));
    MOCKER_CPP(vrelu, void(vector_float&, vector_float, vector_bool, Literal)).times(1).will(invoke(UnaryFloatStub));
    MOCKER_CPP(vmuls, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(ScalarFloatStub));

    ExpectResult(asc::te::abs(src), src.mask);
    ExpectResult(asc::te::exp(src), src.mask);
    ExpectResult(asc::te::sqrt(src), src.mask);
    ExpectResult(asc::te::log(src), src.mask, SRC1_TOKEN);
    ExpectResult(asc::te::log2(src), src.mask);
    ExpectResult(asc::te::log10(src), src.mask);
    ExpectResult(-src, src.mask);
    ExpectResult(asc::te::relu(src), src.mask);

    GlobalMockObject::verify();
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 1.4426950408889634f);
    EXPECT_FLOAT_EQ(g_capture.scalars[1], 0.4342944819032518f);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiBasicArithmetic, ActivationApisPreserveOperandsAndMask)
{
    auto src = MakeReg<float>();
    auto slope = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src.mask);
    MOCKER_CPP(vprelu, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(PreluStub));
    MOCKER_CPP(vlrelu, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(1)
        .will(invoke(LeakyReluStub));

    ExpectResult(asc::te::prelu(src, slope), src.mask);
    ExpectResult(asc::te::leaky_relu(src, 0.2f), src.mask);

    GlobalMockObject::verify();
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 0.2f);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiBasicArithmetic, AddAndMultiplyChooseRegisterOrScalarIntrinsic)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src0.mask);
    MOCKER_CPP(vadd, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryFloatStub));
    MOCKER_CPP(vadds, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(ScalarFloatStub));
    MOCKER_CPP(vmul, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryFloatStub));
    MOCKER_CPP(vmuls, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(ScalarFloatStub));

    ExpectResult(src0 + src1, src0.mask);
    ExpectResult(src0 + 2.0f, src0.mask);
    ExpectResult(2.0f + src0, src0.mask);
    ExpectResult(src0 * src1, src0.mask);
    ExpectResult(src0 * 3.0f, src0.mask);
    ExpectResult(3.0f * src0, src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 2U);
    EXPECT_EQ(g_capture.scalar_calls, 4U);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiBasicArithmetic, SubtractAndDividePreserveOperandOrder)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src0.mask);
    MOCKER_CPP(vdup, void(vector_float&, float, vector_bool, Literal))
        .times(4)
        .will(invoke(DuplicateFloatStub));
    MOCKER_CPP(vsub, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(OrderedFloatStub));

    ExpectResult(src0 - src1, src0.mask);
    ExpectResult(src0 - 4.0f, src0.mask);
    ExpectResult(4.0f - src0, src0.mask);
    EXPECT_EQ(g_capture.calls, 3U);
    g_capture.calls = 0;

    MOCKER_CPP(vdiv, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(OrderedFloatStub));
    ExpectResult(src0 / src1, src0.mask);
    ExpectResult(src0 / 8.0f, src0.mask);
    ExpectResult(8.0f / src0, src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 3U);
    EXPECT_EQ(g_capture.duplicate_calls, 4U);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

} // namespace
