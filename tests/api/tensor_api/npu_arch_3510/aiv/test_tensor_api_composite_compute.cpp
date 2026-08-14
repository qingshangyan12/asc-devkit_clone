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
#include "tensor_api/experimental/reg/composite_compute.h"

namespace {

constexpr uint8_t SRC0_TOKEN = 0x11;
constexpr uint8_t SRC1_TOKEN = 0x22;
constexpr uint8_t DST_TOKEN = 0x33;
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
asc::te::reg_tensor<T> MakeReg(uint8_t token)
{
    asc::te::reg_tensor<T> value {};
    FillBytes(value.reg, token);
    FillBytes(value.mask, MASK_TOKEN);
    return value;
}

struct Capture {
    vector_bool expected_mask {};
    bool masks_match = true;
    bool operands_match = true;
    size_t calls = 0;
    size_t part_calls = 0;
    std::array<int, 4> parts {};
    std::array<float, 4> scalars {};
};

Capture g_capture;

void CaptureMask(const vector_bool& mask)
{
    g_capture.masks_match = g_capture.masks_match && SameBytes(mask, g_capture.expected_mask);
}

void AxpyStub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match && HasByteValue(dst, DST_TOKEN) &&
        HasByteValue(src, SRC0_TOKEN);
    g_capture.scalars[g_capture.calls++] = scalar;
    FillBytes(dst, RESULT_TOKEN);
}

void AbsdiffStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match && HasByteValue(src0, SRC0_TOKEN) &&
        HasByteValue(src1, SRC1_TOKEN);
    ++g_capture.calls;
    FillBytes(dst, RESULT_TOKEN);
}

void ExpSubStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, int part)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match && HasByteValue(src0, SRC0_TOKEN) &&
        HasByteValue(src1, SRC1_TOKEN);
    g_capture.parts[g_capture.part_calls++] = part;
    ++g_capture.calls;
    FillBytes(dst, RESULT_TOKEN);
}

void ExpSubHalfStub(vector_float& dst, vector_half, vector_half, vector_bool mask, int part)
{
    CaptureMask(mask);
    g_capture.parts[g_capture.part_calls++] = part;
    ++g_capture.calls;
    FillBytes(dst, RESULT_TOKEN);
}

void FmaStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match && HasByteValue(dst, SRC0_TOKEN) &&
        HasByteValue(src0, SRC1_TOKEN) && HasByteValue(src1, DST_TOKEN);
    ++g_capture.calls;
    FillBytes(dst, RESULT_TOKEN);
}

void MulsCastStub(vector_half& dst, vector_float src, float scalar, vector_bool mask, int part)
{
    CaptureMask(mask);
    g_capture.operands_match = g_capture.operands_match && HasByteValue(src, SRC0_TOKEN);
    g_capture.scalars[g_capture.calls++] = scalar;
    g_capture.parts[g_capture.part_calls++] = part;
    FillBytes(dst, RESULT_TOKEN);
}

class TestTensorApiCompositeCompute : public testing::Test {
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
__aicore__ inline void CompileAxpy()
{
    asc::te::reg_tensor<T> dst {};
    asc::te::reg_tensor<T> src {};
    (void)asc::te::axpy(dst, src, T {});
}

template <typename T>
__aicore__ inline void CompileAbsdiff()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    (void)asc::te::absdiff(src0, src1);
}

template <typename T>
__aicore__ inline void CompileExpSub()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    (void)asc::te::exp_sub(src0, src1);
}

template <typename T>
__aicore__ inline void CompileFma()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    asc::te::reg_tensor<T> src2 {};
    (void)asc::te::fma(src0, src1, src2);
}

TEST_F(TestTensorApiCompositeCompute, CompileFiveApisAndSupportedTypes)
{
    AscendC::SetGCoreType(0);
    CompileAxpy<half>();
    CompileAxpy<float>();
    CompileAbsdiff<half>();
    CompileAbsdiff<float>();
    CompileExpSub<half>();
    CompileExpSub<float>();
    CompileFma<half>();
    CompileFma<float>();

    static_assert(!asc::te::detail::supports_axpy_v<int64_t>);
    static_assert(!asc::te::detail::supports_absdiff_v<int64_t>);
    static_assert(!asc::te::detail::supports_fma_v<int32_t>);

    asc::te::reg_tensor<float> src {};
    static_assert(std::is_same_v<decltype(asc::te::muls_cast(src, 2.0f)), asc::te::reg_tensor<half>>);
    (void)asc::te::muls_cast(src, 2.0f);
}

TEST_F(TestTensorApiCompositeCompute, AccumulationApisUseFirstOperandMask)
{
    auto dst = MakeReg<float>(DST_TOKEN);
    auto src0 = MakeReg<float>(SRC0_TOKEN);
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    FillBytes(src0.mask, 0x70);
    FillBytes(src1.mask, 0x71);
    g_capture = {};
    g_capture.expected_mask = dst.mask;
    MOCKER_CPP(vaxpy, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(1)
        .will(invoke(AxpyStub));
    MOCKER_CPP(vmadd, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(FmaStub));

    EXPECT_TRUE(HasByteValue(asc::te::axpy(dst, src0, 2.5f).reg, RESULT_TOKEN));
    g_capture.expected_mask = src0.mask;
    EXPECT_TRUE(HasByteValue(asc::te::fma(src0, src1, dst).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 2U);
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 2.5f);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiCompositeCompute, DifferenceApisPreserveSourceOrderAndSupportedLayout)
{
    auto src0 = MakeReg<float>(SRC0_TOKEN);
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    g_capture = {};
    g_capture.expected_mask = src0.mask;
    MOCKER_CPP(vabsdif, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(AbsdiffStub));
    MOCKER_CPP(vexpdif, void(vector_float&, vector_float, vector_float, vector_bool, int))
        .times(1)
        .will(invoke(ExpSubStub));
    MOCKER_CPP(vexpdif, void(vector_float&, vector_half, vector_half, vector_bool, int))
        .times(1)
        .will(invoke(ExpSubHalfStub));

    EXPECT_TRUE(HasByteValue(asc::te::absdiff(src0, src1).reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue(asc::te::exp_sub(src0, src1).reg, RESULT_TOKEN));
    auto half_src0 = MakeReg<half>(SRC0_TOKEN);
    auto half_src1 = MakeReg<half>(SRC1_TOKEN);
    EXPECT_TRUE(HasByteValue(asc::te::exp_sub(half_src0, half_src1).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 3U);
    EXPECT_EQ(g_capture.parts[0], PART_EVEN);
    EXPECT_EQ(g_capture.parts[1], PART_EVEN);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(TestTensorApiCompositeCompute, MulsCastMapsSupportedLayout)
{
    auto src = MakeReg<float>(SRC0_TOKEN);
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vmulscvt, void(vector_half&, vector_float, float, vector_bool, int))
        .times(1)
        .will(invoke(MulsCastStub));

    EXPECT_TRUE(HasByteValue(asc::te::muls_cast(src, 1.5f).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 1.5f);
    EXPECT_EQ(g_capture.parts[0], PART_EVEN);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

} // namespace
