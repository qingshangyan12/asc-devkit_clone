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
#include "tensor_api/experimental/reg/logical_compute.h"

namespace {

constexpr uint8_t SRC_TOKEN = 0x11;
constexpr uint8_t RESULT_TOKEN = 0x44;
constexpr uint8_t MASK_TOKEN = 0x5A;
constexpr uint8_t PREDICATE_TOKEN = 0x6B;

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
asc::te::reg_tensor<T> MakeReg(uint8_t token = SRC_TOKEN)
{
    asc::te::reg_tensor<T> value {};
    FillBytes(value.reg, token);
    FillBytes(value.mask, MASK_TOKEN);
    return value;
}

struct Capture {
    vector_bool expected_mask {};
    bool masks_match = true;
    size_t binary_calls = 0;
    size_t scalar_calls = 0;
    std::array<int16_t, 2> shifts {};
};

Capture g_capture;

void CaptureMask(const vector_bool& mask)
{
    g_capture.masks_match = g_capture.masks_match && SameBytes(mask, g_capture.expected_mask);
}

void BinaryIntStub(vector_int32_t& dst, vector_int32_t, vector_int32_t, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++g_capture.binary_calls;
    FillBytes(dst, RESULT_TOKEN);
}

void ShiftScalarStub(vector_int32_t& dst, vector_int32_t, int16_t shift, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.shifts[g_capture.scalar_calls++] = shift;
    FillBytes(dst, RESULT_TOKEN);
}

void EqZeroStub(vector_bool& dst, vector_int32_t, int32_t value, vector_bool mask)
{
    CaptureMask(mask);
    EXPECT_EQ(value, 0);
    FillBytes(dst, PREDICATE_TOKEN);
}

void DuplicateIntStub(vector_int32_t& dst, int32_t value, vector_bool mask, Literal)
{
    if (value == 0) {
        CaptureMask(mask);
        FillBytes(dst, 0);
    } else {
        EXPECT_EQ(value, 1);
        EXPECT_TRUE(HasByteValue(mask, PREDICATE_TOKEN));
        FillBytes(dst, RESULT_TOKEN);
    }
}

class TestTensorApiLogicalCompute : public testing::Test {
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
__aicore__ inline void CompileLogicalNot()
{
    asc::te::reg_tensor<T> src {};
    (void)!src;
}

template <typename T>
__aicore__ inline void CompileBitwiseAnd()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 & src1);
    (void)(src0 & scalar);
    (void)(scalar & src0);
}

template <typename T>
__aicore__ inline void CompileBitwiseOr()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 | src1);
    (void)(src0 | scalar);
    (void)(scalar | src0);
}

template <typename T>
__aicore__ inline void CompileBitwiseXor()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 ^ src1);
    (void)(src0 ^ scalar);
    (void)(scalar ^ src0);
}

template <typename T, typename ShiftT>
__aicore__ inline void CompileShift()
{
    asc::te::reg_tensor<T> src {};
    asc::te::reg_tensor<ShiftT> shift {};
    (void)(src << shift);
    (void)(src << static_cast<int16_t>(2));
    (void)(src >> shift);
    (void)(src >> static_cast<int16_t>(2));
}

TEST_F(TestTensorApiLogicalCompute, CompilePublicApisAndSupportedTypes)
{
    AscendC::SetGCoreType(0);
    CompileLogicalNot<int8_t>();
    CompileLogicalNot<uint8_t>();
    CompileLogicalNot<int16_t>();
    CompileLogicalNot<uint16_t>();
    CompileLogicalNot<half>();
    CompileLogicalNot<bfloat16_t>();
    CompileLogicalNot<int32_t>();
    CompileLogicalNot<uint32_t>();
    CompileLogicalNot<float>();

    CompileBitwiseAnd<int8_t>();
    CompileBitwiseAnd<uint8_t>();
    CompileBitwiseAnd<fp8_e5m2_t>();
    CompileBitwiseAnd<fp8_e4m3fn_t>();
    CompileBitwiseAnd<int16_t>();
    CompileBitwiseAnd<uint16_t>();
    CompileBitwiseAnd<half>();
    CompileBitwiseAnd<bfloat16_t>();
    CompileBitwiseAnd<int32_t>();
    CompileBitwiseAnd<uint32_t>();
    CompileBitwiseAnd<float>();

    CompileBitwiseOr<int8_t>();
    CompileBitwiseOr<uint8_t>();
    CompileBitwiseOr<int16_t>();
    CompileBitwiseOr<uint16_t>();
    CompileBitwiseOr<half>();
    CompileBitwiseOr<int32_t>();
    CompileBitwiseOr<uint32_t>();
    CompileBitwiseOr<float>();

    CompileBitwiseXor<int8_t>();
    CompileBitwiseXor<uint8_t>();
    CompileBitwiseXor<int16_t>();
    CompileBitwiseXor<uint16_t>();
    CompileBitwiseXor<int32_t>();
    CompileBitwiseXor<uint32_t>();

    CompileShift<int8_t, int8_t>();
    CompileShift<uint8_t, int8_t>();
    CompileShift<int16_t, int16_t>();
    CompileShift<uint16_t, int16_t>();
    CompileShift<int32_t, int32_t>();
    CompileShift<uint32_t, int32_t>();

    asc::te::reg_tensor<bool> predicate0 {};
    asc::te::reg_tensor<bool> predicate1 {};
    static_assert(std::is_same_v<decltype(!predicate0), asc::te::reg_tensor<bool>>);
    static_assert(std::is_same_v<decltype(predicate0 & predicate1), asc::te::reg_tensor<bool>>);
    (void)!predicate0;
    (void)(predicate0 & predicate1);
    (void)(predicate0 | predicate1);
    (void)(predicate0 ^ predicate1);

    static_assert(!asc::te::detail::supports_not_v<int64_t>);
    static_assert(!asc::te::detail::supports_and_v<uint64_t>);
    static_assert(!asc::te::detail::supports_or_v<bfloat16_t>);
    static_assert(!asc::te::detail::supports_xor_v<float>);
    static_assert(!asc::te::detail::supports_shift_v<int64_t>);
}

TEST_F(TestTensorApiLogicalCompute, LogicalNotReturnsOneForZeroLanes)
{
    auto src = MakeReg<int32_t>();
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vcmps_eq, void(vector_bool&, vector_int32_t, int32_t, vector_bool))
        .times(1)
        .will(invoke(EqZeroStub));
    MOCKER_CPP(vdup, void(vector_int32_t&, int32_t, vector_bool, Literal))
        .times(2)
        .will(invoke(DuplicateIntStub));

    auto result = !src;

    GlobalMockObject::verify();
    EXPECT_TRUE(HasByteValue(result.reg, RESULT_TOKEN));
    EXPECT_TRUE(SameBytes(result.mask, src.mask));
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiLogicalCompute, RegisterBitwiseAndShiftApisForwardMask)
{
    auto src = MakeReg<int32_t>();
    auto other = MakeReg<int32_t>(0x22);
    auto shift = MakeReg<int32_t>(0x33);
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vand, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vor, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vxor, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vshl, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vshr, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));

    EXPECT_TRUE(HasByteValue((src & other).reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue((src | other).reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue((src ^ other).reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue((src << shift).reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue((src >> shift).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.binary_calls, 5U);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiLogicalCompute, ScalarShiftUsesInt16Immediate)
{
    auto src = MakeReg<int32_t>();
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vshls, void(vector_int32_t&, vector_int32_t, int16_t, vector_bool, Literal))
        .times(1)
        .will(invoke(ShiftScalarStub));
    MOCKER_CPP(vshrs, void(vector_int32_t&, vector_int32_t, int16_t, vector_bool, Literal))
        .times(1)
        .will(invoke(ShiftScalarStub));

    EXPECT_TRUE(HasByteValue((src << static_cast<int16_t>(2)).reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue((src >> static_cast<int16_t>(3)).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.shifts[0], 2);
    EXPECT_EQ(g_capture.shifts[1], 3);
    EXPECT_TRUE(g_capture.masks_match);
}

} // namespace
