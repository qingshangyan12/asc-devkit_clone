/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/experimental/reg/reg_compute.h"

namespace {

static_assert(AscendC::Te::CURRENT_ARCH_VERSION == AscendC::Te::ArchVersion::V3510);

class TestTensorApiRegCompute : public testing::Test {
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

constexpr uint8_t SRC0_TOKEN = 0x11;
constexpr uint8_t SRC1_TOKEN = 0x22;
constexpr uint8_t BROADCAST_TOKEN = 0x33;
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
asc::te::reg_tensor<T> MakeReg(uint8_t regToken = SRC0_TOKEN)
{
    asc::te::reg_tensor<T> value {};
    FillBytes(value.reg, regToken);
    FillBytes(value.mask, MASK_TOKEN);
    return value;
}

template <typename T>
void ExpectResult(const asc::te::reg_tensor<T>& result, const vector_bool& expectedMask,
    uint8_t resultToken = RESULT_TOKEN)
{
    EXPECT_TRUE(HasByteValue(result.reg, resultToken));
    EXPECT_TRUE(SameBytes(result.mask, expectedMask));
}

struct IntrinsicCapture {
    vector_bool expectedMask {};
    bool masksMatch = true;
    bool operandsMatch = true;
    bool predicateMaskMatch = true;
    size_t unaryCalls = 0;
    size_t binaryCalls = 0;
    size_t scalarCalls = 0;
    size_t subCalls = 0;
    size_t divCalls = 0;
    size_t duplicateCalls = 0;
    size_t intScalarCalls = 0;
    size_t axpyCalls = 0;
    size_t absSubCalls = 0;
    size_t expSubCalls = 0;
    size_t mulDstAddCalls = 0;
    size_t mulAddDstCalls = 0;
    size_t mulsCastCalls = 0;
    size_t partCalls = 0;
    std::array<float, 16> floatScalars {};
    std::array<int32_t, 16> intScalars {};
    std::array<int16_t, 8> shiftScalars {};
    std::array<int, 4> parts {};
};

IntrinsicCapture gCapture;

void ResetCapture(const vector_bool& expectedMask)
{
    gCapture = {};
    gCapture.expectedMask = expectedMask;
}

void CaptureMask(const vector_bool& mask)
{
    gCapture.masksMatch = gCapture.masksMatch && SameBytes(mask, gCapture.expectedMask);
}

template <typename T, size_t N>
void CaptureScalar(std::array<T, N>& values, size_t& count, T value)
{
    if (count >= values.size()) {
        ADD_FAILURE() << "scalar capture capacity exceeded";
        return;
    }
    values[count++] = value;
}

void UnaryFloatStub(vector_float& dst, vector_float, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++gCapture.unaryCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void BinaryFloatStub(vector_float& dst, vector_float, vector_float, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++gCapture.binaryCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void ScalarFloatStub(vector_float& dst, vector_float, float value, vector_bool mask, Literal)
{
    CaptureMask(mask);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, value);
    FillBytes(dst, RESULT_TOKEN);
}

void LnStub(vector_float& dst, vector_float, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++gCapture.unaryCalls;
    FillBytes(dst, SRC1_TOKEN);
}

void LogScaleStub(vector_float& dst, vector_float src, float value, vector_bool mask, Literal)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(src, SRC1_TOKEN);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, value);
    FillBytes(dst, RESULT_TOKEN);
}

void DuplicateFloatStub(vector_float& dst, float value, vector_bool mask, Literal)
{
    CaptureMask(mask);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, value);
    ++gCapture.duplicateCalls;
    FillBytes(dst, BROADCAST_TOKEN);
}

void PreluStub(vector_float& dst, vector_float src, vector_float slope, vector_bool mask, Literal)
{
    CaptureMask(mask);
    const bool expectedSlope = gCapture.binaryCalls == 0 ? HasByteValue(slope, SRC1_TOKEN) :
                                                          HasByteValue(slope, BROADCAST_TOKEN);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(src, SRC0_TOKEN) && expectedSlope;
    ++gCapture.binaryCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void LeakyReluStub(vector_float& dst, vector_float src, float value, vector_bool mask, Literal)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(src, SRC0_TOKEN);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, value);
    FillBytes(dst, RESULT_TOKEN);
}

void SubStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    constexpr std::array<uint8_t, 3> expectedSrc0 {SRC0_TOKEN, SRC0_TOKEN, BROADCAST_TOKEN};
    constexpr std::array<uint8_t, 3> expectedSrc1 {SRC1_TOKEN, BROADCAST_TOKEN, SRC0_TOKEN};
    const size_t call = gCapture.subCalls++;
    gCapture.operandsMatch = gCapture.operandsMatch && call < expectedSrc0.size() &&
        HasByteValue(src0, expectedSrc0[call]) && HasByteValue(src1, expectedSrc1[call]);
    FillBytes(dst, static_cast<uint8_t>(RESULT_TOKEN + call));
}

void DivStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    constexpr std::array<uint8_t, 3> expectedSrc0 {SRC0_TOKEN, SRC0_TOKEN, BROADCAST_TOKEN};
    constexpr std::array<uint8_t, 3> expectedSrc1 {SRC1_TOKEN, BROADCAST_TOKEN, SRC0_TOKEN};
    const size_t call = gCapture.divCalls++;
    gCapture.operandsMatch = gCapture.operandsMatch && call < expectedSrc0.size() &&
        HasByteValue(src0, expectedSrc0[call]) && HasByteValue(src1, expectedSrc1[call]);
    FillBytes(dst, static_cast<uint8_t>(RESULT_TOKEN + call));
}

void BinaryIntStub(vector_int32_t& dst, vector_int32_t, vector_int32_t, vector_bool mask, Literal)
{
    CaptureMask(mask);
    ++gCapture.binaryCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void DuplicateIntStub(vector_int32_t& dst, int32_t value, vector_bool mask, Literal)
{
    const size_t call = gCapture.duplicateCalls++;
    CaptureScalar(gCapture.intScalars, gCapture.intScalarCalls, value);
    if (call == 0) {
        CaptureMask(mask);
        gCapture.operandsMatch = gCapture.operandsMatch && value == 0;
    } else if (call == 1) {
        gCapture.predicateMaskMatch = HasByteValue(mask, PREDICATE_TOKEN);
        gCapture.operandsMatch = gCapture.operandsMatch && value == 1;
    } else {
        CaptureMask(mask);
        gCapture.operandsMatch = gCapture.operandsMatch && value == 3;
    }
    FillBytes(dst, call == 0 ? 0 : BROADCAST_TOKEN);
}

void EqZeroStub(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
{
    CaptureMask(mask);
    gCapture.operandsMatch = HasByteValue(src, SRC0_TOKEN) && value == 0;
    FillBytes(dst, PREDICATE_TOKEN);
}

void ShiftScalarIntStub(vector_int32_t& dst, vector_int32_t, int16_t shift, vector_bool mask, Literal)
{
    CaptureMask(mask);
    CaptureScalar(gCapture.shiftScalars, gCapture.scalarCalls, shift);
    FillBytes(dst, RESULT_TOKEN);
}

void CompareRegFloatStub(vector_bool& dst, vector_float, vector_float, vector_bool mask)
{
    CaptureMask(mask);
    ++gCapture.binaryCalls;
    FillBytes(dst, PREDICATE_TOKEN);
}

void CompareScalarFloatStub(vector_bool& dst, vector_float, float value, vector_bool mask)
{
    CaptureMask(mask);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, value);
    FillBytes(dst, PREDICATE_TOKEN);
}

void AxpyFloatStub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(dst, BROADCAST_TOKEN) &&
        HasByteValue(src, SRC0_TOKEN);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, scalar);
    ++gCapture.axpyCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void AbsSubFloatStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(src0, SRC0_TOKEN) &&
        HasByteValue(src1, SRC1_TOKEN);
    ++gCapture.absSubCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void ExpSubFloatStub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, int part)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(src0, SRC0_TOKEN) &&
        HasByteValue(src1, SRC1_TOKEN);
    CaptureScalar(gCapture.parts, gCapture.partCalls, part);
    ++gCapture.expSubCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void MulDstAddFloatStub(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(dst, BROADCAST_TOKEN) &&
        HasByteValue(src0, SRC0_TOKEN) && HasByteValue(src1, SRC1_TOKEN);
    ++gCapture.mulDstAddCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void MulAddDstFloatStub(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(dst, BROADCAST_TOKEN) &&
        HasByteValue(src0, SRC0_TOKEN) && HasByteValue(src1, SRC1_TOKEN);
    ++gCapture.mulAddDstCalls;
    FillBytes(dst, RESULT_TOKEN);
}

void MulsCastFloatStub(vector_half& dst, vector_float src, float scalar, vector_bool mask, int part)
{
    CaptureMask(mask);
    gCapture.operandsMatch = gCapture.operandsMatch && HasByteValue(src, SRC0_TOKEN);
    CaptureScalar(gCapture.floatScalars, gCapture.scalarCalls, scalar);
    CaptureScalar(gCapture.parts, gCapture.partCalls, part);
    ++gCapture.mulsCastCalls;
    FillBytes(dst, RESULT_TOKEN);
}

static_assert(asc::te::detail::supports_abs_v<int8_t>);
static_assert(asc::te::detail::supports_abs_v<int16_t>);
static_assert(asc::te::detail::supports_abs_v<half>);
static_assert(asc::te::detail::supports_abs_v<int32_t>);
static_assert(asc::te::detail::supports_abs_v<float>);
static_assert(!asc::te::detail::supports_abs_v<uint8_t>);
static_assert(!asc::te::detail::supports_abs_v<int64_t>);

static_assert(asc::te::detail::supports_add_sub_v<bfloat16_t>);
static_assert(!asc::te::detail::supports_add_sub_v<int64_t>);
static_assert(asc::te::detail::supports_mul_v<bfloat16_t>);
static_assert(!asc::te::detail::supports_mul_scalar_v<bfloat16_t>);
static_assert(asc::te::detail::supports_min_max_v<bfloat16_t>);
static_assert(!asc::te::detail::supports_min_max_v<uint64_t>);
static_assert(asc::te::detail::supports_not_v<int32_t>);
static_assert(!asc::te::detail::supports_not_v<float>);
static_assert(asc::te::detail::supports_bitwise_v<uint32_t>);
static_assert(!asc::te::detail::supports_bitwise_v<half>);
static_assert(asc::te::detail::supports_shift_pair_v<uint8_t, int8_t>);
static_assert(asc::te::detail::supports_shift_pair_v<uint16_t, int16_t>);
static_assert(asc::te::detail::supports_shift_pair_v<uint32_t, int32_t>);
static_assert(!asc::te::detail::supports_shift_pair_v<uint32_t, uint32_t>);
static_assert(asc::te::detail::supports_compare_v<bfloat16_t>);
static_assert(asc::te::detail::supports_axpy_v<half>);
static_assert(asc::te::detail::supports_axpy_v<float>);
static_assert(asc::te::detail::supports_axpy_v<int64_t>);
static_assert(asc::te::detail::supports_axpy_v<uint64_t>);
static_assert(asc::te::detail::supports_abs_sub_v<half>);
static_assert(asc::te::detail::supports_abs_sub_v<float>);
static_assert(asc::te::detail::supports_abs_sub_v<int64_t>);
static_assert(asc::te::detail::supports_exp_sub_v<half>);
static_assert(asc::te::detail::supports_exp_sub_v<float>);
static_assert(asc::te::detail::supports_mul_dst_add_v<bfloat16_t>);
static_assert(!asc::te::detail::supports_mul_dst_add_v<int32_t>);
static_assert(asc::te::detail::supports_mul_add_dst_v<int16_t>);
static_assert(asc::te::detail::supports_mul_add_dst_v<uint32_t>);
static_assert(asc::te::detail::supports_mul_add_dst_v<int64_t>);
static_assert(asc::te::detail::supports_mul_add_dst_v<uint64_t>);

template <typename T>
__aicore__ inline void CompileAbs()
{
    asc::te::reg_tensor<T> src {};
    static_assert(std::is_same_v<decltype(asc::te::abs(src)), asc::te::reg_tensor<T>>);
    (void)asc::te::abs(src);
}

template <typename T>
__aicore__ inline void CompileNeg()
{
    asc::te::reg_tensor<T> src {};
    (void)-src;
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
__aicore__ inline void CompileFloatingPointRegCompute()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};

    static_assert(std::is_same_v<decltype(asc::te::abs(src0)), asc::te::reg_tensor<T>>);
    static_assert(std::is_same_v<decltype(src0 == src1), asc::te::reg_tensor<bool>>);

    (void)asc::te::abs(src0);
    (void)asc::te::exp(src0);
    (void)asc::te::sqrt(src0);
    (void)asc::te::log(src0);
    (void)asc::te::log2(src0);
    (void)asc::te::log10(src0);
    (void)-src0;
    (void)asc::te::relu(src0);
    static_assert(std::is_same_v<decltype(asc::te::prelu(src0)), asc::te::reg_tensor<T>>);
    (void)asc::te::prelu(src0);
    (void)asc::te::prelu(src0, src1);
    (void)asc::te::prelu(src0, 0.25f);
    (void)asc::te::leaky_relu(src0);
    (void)asc::te::leaky_relu(src0, 0.01f);

    (void)(src0 + src1);
    (void)(src0 + 1.0f);
    (void)(1.0f + src0);
    (void)(src0 - src1);
    (void)(src0 - 1.0f);
    (void)(1.0f - src0);
    (void)(src0 * src1);
    (void)(src0 * 2.0f);
    (void)(2.0f * src0);
    (void)(src0 / src1);
    (void)(src0 / 2.0f);
    (void)(2.0f / src0);
    (void)asc::te::max(src0, src1);
    (void)asc::te::max(src0, 1.0f);

    (void)asc::te::max(1.0f, src0);
    (void)asc::te::min(src0, src1);
    (void)asc::te::min(src0, 1.0f);
    (void)asc::te::min(1.0f, src0);

    (void)(src0 == src1);
    (void)(src0 != 1.0f);
    (void)(1.0f < src0);
    (void)(src0 <= 1.0f);
    (void)(1.0f > src0);
    (void)(src0 >= src1);
}

template <typename T, typename ShiftT>
__aicore__ inline void CompileIntegerRegCompute()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    asc::te::reg_tensor<ShiftT> shift {};

    static_assert(std::is_same_v<decltype(!src0), asc::te::reg_tensor<T>>);
    (void)!src0;
    (void)(src0 & src1);
    (void)(src0 & 3);
    (void)(3 & src0);
    (void)(src0 | src1);
    (void)(src0 | 3);
    (void)(3 | src0);
    (void)(src0 ^ src1);
    (void)(src0 ^ 3);
    (void)(3 ^ src0);
    (void)(src0 << shift);
    (void)(src0 << 2);
    (void)(src0 >> shift);
    (void)(src0 >> 2);
}

template <typename T>
__aicore__ inline void CompileCompare()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};

    (void)(src0 == src1);
    (void)(src0 != scalar);
    (void)(scalar < src0);
    (void)(src0 <= scalar);
    (void)(scalar > src0);
    (void)(src0 >= src1);
}

template <typename T>
__aicore__ inline void CompileAxpyAndAbsSub()
{
    asc::te::reg_tensor<T> dst {};
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};

    static_assert(std::is_same_v<decltype(asc::te::axpy(dst, src0, scalar)), asc::te::reg_tensor<T>>);
    static_assert(std::is_same_v<decltype(asc::te::abs_sub(src0, src1)), asc::te::reg_tensor<T>>);
    (void)asc::te::axpy(dst, src0, scalar);
    (void)asc::te::abs_sub(src0, src1);
}

template <typename T>
__aicore__ inline void CompileExpSub()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};

    static_assert(std::is_same_v<decltype(asc::te::exp_sub(src0, src1)), asc::te::reg_tensor<float>>);
    static_assert(std::is_same_v<decltype(asc::te::exp_sub<asc::te::reg_layout::one>(src0, src1)),
        asc::te::reg_tensor<float>>);
    (void)asc::te::exp_sub(src0, src1);
    (void)asc::te::exp_sub<asc::te::reg_layout::one>(src0, src1);
}

template <typename T>
__aicore__ inline void CompileMulDstAdd()
{
    asc::te::reg_tensor<T> dst {};
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};

    static_assert(std::is_same_v<decltype(asc::te::mul_dst_add(dst, src0, src1)), asc::te::reg_tensor<T>>);
    (void)asc::te::mul_dst_add(dst, src0, src1);
}

template <typename T>
__aicore__ inline void CompileMulAddDst()
{
    asc::te::reg_tensor<T> dst {};
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};

    static_assert(std::is_same_v<decltype(asc::te::mul_add_dst(dst, src0, src1)), asc::te::reg_tensor<T>>);
    (void)asc::te::mul_add_dst(dst, src0, src1);
}

__aicore__ inline void CompileMulsCast()
{
    asc::te::reg_tensor<float> src {};
    static_assert(std::is_same_v<decltype(asc::te::muls_cast(src, 2.0f)), asc::te::reg_tensor<half>>);
    static_assert(std::is_same_v<decltype(asc::te::muls_cast<asc::te::reg_layout::one>(src, 2.0f)),
        asc::te::reg_tensor<half>>);
    (void)asc::te::muls_cast(src, 2.0f);
    (void)asc::te::muls_cast<asc::te::reg_layout::one>(src, 2.0f);
}

TEST_F(TestTensorApiRegCompute, CompileReturnValueStyleApis)
{
    AscendC::SetGCoreType(0);
    CompileFloatingPointRegCompute<half>();
    CompileFloatingPointRegCompute<float>();
    CompileIntegerRegCompute<int32_t, int32_t>();
}

TEST_F(TestTensorApiRegCompute, CompileDocumentedLogicAndComparisonTypeMatrix)
{
    AscendC::SetGCoreType(0);

#define CHECK_LOGIC_COMPILE(TYPE, SHIFT_TYPE)                                                                        \
    do {                                                                                                              \
        CompileIntegerRegCompute<TYPE, SHIFT_TYPE>();                                                                 \
    } while (false)

    CHECK_LOGIC_COMPILE(int8_t, int8_t);
    CHECK_LOGIC_COMPILE(uint8_t, int8_t);
    CHECK_LOGIC_COMPILE(int16_t, int16_t);
    CHECK_LOGIC_COMPILE(uint16_t, int16_t);
    CHECK_LOGIC_COMPILE(int32_t, int32_t);
    CHECK_LOGIC_COMPILE(uint32_t, int32_t);

#undef CHECK_LOGIC_COMPILE

#define CHECK_COMPARE_COMPILE(TYPE)                                                                                   \
    do {                                                                                                              \
        CompileCompare<TYPE>();                                                                                       \
    } while (false)

    CHECK_COMPARE_COMPILE(int8_t);
    CHECK_COMPARE_COMPILE(uint8_t);
    CHECK_COMPARE_COMPILE(int16_t);
    CHECK_COMPARE_COMPILE(uint16_t);
    CHECK_COMPARE_COMPILE(half);
    CHECK_COMPARE_COMPILE(bfloat16_t);
    CHECK_COMPARE_COMPILE(int32_t);
    CHECK_COMPARE_COMPILE(uint32_t);
    CHECK_COMPARE_COMPILE(float);

#undef CHECK_COMPARE_COMPILE
}

TEST_F(TestTensorApiRegCompute, CompileDocumentedArithmeticTypeMatrix)
{
    AscendC::SetGCoreType(0);

#define CHECK_COMPILE(API, TYPE)                                                                                      \
    do {                                                                                                              \
        API<TYPE>();                                                                                                  \
    } while (false)

    CHECK_COMPILE(CompileAbs, int8_t);
    CHECK_COMPILE(CompileAbs, int16_t);
    CHECK_COMPILE(CompileAbs, half);
    CHECK_COMPILE(CompileAbs, int32_t);
    CHECK_COMPILE(CompileAbs, float);

    CHECK_COMPILE(CompileNeg, int8_t);
    CHECK_COMPILE(CompileNeg, int16_t);
    CHECK_COMPILE(CompileNeg, half);
    CHECK_COMPILE(CompileNeg, int32_t);
    CHECK_COMPILE(CompileNeg, float);

    CHECK_COMPILE(CompileRelu, half);
    CHECK_COMPILE(CompileRelu, int32_t);
    CHECK_COMPILE(CompileRelu, float);

    CHECK_COMPILE(CompileAddSub, int8_t);
    CHECK_COMPILE(CompileAddSub, uint8_t);
    CHECK_COMPILE(CompileAddSub, int16_t);
    CHECK_COMPILE(CompileAddSub, uint16_t);
    CHECK_COMPILE(CompileAddSub, half);
    CHECK_COMPILE(CompileAddSub, bfloat16_t);
    CHECK_COMPILE(CompileAddSub, int32_t);
    CHECK_COMPILE(CompileAddSub, uint32_t);
    CHECK_COMPILE(CompileAddSub, float);

    CHECK_COMPILE(CompileMul, int16_t);
    CHECK_COMPILE(CompileMul, uint16_t);
    CHECK_COMPILE(CompileMul, half);
    CHECK_COMPILE(CompileMul, bfloat16_t);
    CHECK_COMPILE(CompileMul, int32_t);
    CHECK_COMPILE(CompileMul, uint32_t);
    CHECK_COMPILE(CompileMul, float);

    CHECK_COMPILE(CompileMulDivScalar, int16_t);
    CHECK_COMPILE(CompileMulDivScalar, uint16_t);
    CHECK_COMPILE(CompileMulDivScalar, half);
    CHECK_COMPILE(CompileMulDivScalar, int32_t);
    CHECK_COMPILE(CompileMulDivScalar, uint32_t);
    CHECK_COMPILE(CompileMulDivScalar, float);

    CHECK_COMPILE(CompileMinMax, int8_t);
    CHECK_COMPILE(CompileMinMax, uint8_t);
    CHECK_COMPILE(CompileMinMax, int16_t);
    CHECK_COMPILE(CompileMinMax, uint16_t);
    CHECK_COMPILE(CompileMinMax, half);
    CHECK_COMPILE(CompileMinMax, bfloat16_t);
    CHECK_COMPILE(CompileMinMax, int32_t);
    CHECK_COMPILE(CompileMinMax, uint32_t);
    CHECK_COMPILE(CompileMinMax, float);

#undef CHECK_COMPILE
}

TEST_F(TestTensorApiRegCompute, CompileCompositeTypeMatrix)
{
    AscendC::SetGCoreType(0);

    CompileAxpyAndAbsSub<half>();
    CompileAxpyAndAbsSub<float>();
    CompileAxpyAndAbsSub<int64_t>();
    {
        asc::te::reg_tensor<uint64_t> dst {};
        asc::te::reg_tensor<uint64_t> src {};
        (void)asc::te::axpy(dst, src, static_cast<uint64_t>(2));
    }
    CompileExpSub<half>();
    CompileExpSub<float>();
    CompileMulDstAdd<half>();
    CompileMulDstAdd<bfloat16_t>();
    CompileMulDstAdd<float>();
    CompileMulAddDst<int16_t>();
    CompileMulAddDst<uint16_t>();
    CompileMulAddDst<half>();
    CompileMulAddDst<bfloat16_t>();
    CompileMulAddDst<int32_t>();
    CompileMulAddDst<uint32_t>();
    CompileMulAddDst<float>();
    CompileMulAddDst<int64_t>();
    CompileMulAddDst<uint64_t>();
    CompileMulsCast();
}

TEST_F(TestTensorApiRegCompute, UnaryIntrinsicsForwardMaskAndResult)
{
    auto src = MakeReg<float>();
    ResetCapture(src.mask);

    MOCKER_CPP(vabs, void(vector_float&, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(UnaryFloatStub));
    MOCKER_CPP(vexp, void(vector_float&, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(UnaryFloatStub));
    MOCKER_CPP(vsqrt, void(vector_float&, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(UnaryFloatStub));
    MOCKER_CPP(vln, void(vector_float&, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(UnaryFloatStub));
    MOCKER_CPP(vneg, void(vector_float&, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(UnaryFloatStub));
    MOCKER_CPP(vrelu, void(vector_float&, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(UnaryFloatStub));

    ExpectResult(asc::te::abs(src), src.mask);
    ExpectResult(asc::te::exp(src), src.mask);
    ExpectResult(asc::te::sqrt(src), src.mask);
    ExpectResult(asc::te::log(src), src.mask);
    ExpectResult(-src, src.mask);
    ExpectResult(asc::te::relu(src), src.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.unaryCalls, 6U);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, LogBaseConversionsUseDocumentedCoefficients)
{
    auto src = MakeReg<float>();
    ResetCapture(src.mask);

    MOCKER_CPP(vln, void(vector_float&, vector_float, vector_bool, Literal))
        .times(2)
        .will(invoke(LnStub));
    MOCKER_CPP(vmuls, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(LogScaleStub));

    ExpectResult(asc::te::log2(src), src.mask);
    ExpectResult(asc::te::log10(src), src.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.unaryCalls, 2U);
    EXPECT_EQ(gCapture.scalarCalls, 2U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 1.4426950408889634f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[1], 0.4342944819032518f);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, ActivationOverloadsUseBroadcastAndDocumentedDefaults)
{
    auto src = MakeReg<float>();
    auto slope = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src.mask);

    MOCKER_CPP(vdup, void(vector_float&, float, vector_bool, Literal))
        .times(2)
        .will(invoke(DuplicateFloatStub));
    MOCKER_CPP(vprelu, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(PreluStub));
    MOCKER_CPP(vlrelu, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(LeakyReluStub));

    ExpectResult(asc::te::prelu(src, slope), src.mask);
    ExpectResult(asc::te::prelu(src, 0.5f), src.mask);
    ExpectResult(asc::te::prelu(src), src.mask);
    ExpectResult(asc::te::leaky_relu(src, 0.2f), src.mask);
    ExpectResult(asc::te::leaky_relu(src), src.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.binaryCalls, 3U);
    EXPECT_EQ(gCapture.duplicateCalls, 2U);
    EXPECT_EQ(gCapture.scalarCalls, 4U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 0.5f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[1], 0.25f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[2], 0.2f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[3], 0.01f);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, AddAndMultiplyOverloadsMapToDirectScalarIntrinsics)
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
    EXPECT_EQ(gCapture.binaryCalls, 2U);
    EXPECT_EQ(gCapture.scalarCalls, 4U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 2.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[1], 2.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[2], 3.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[3], 3.0f);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, SubtractAndDivideOverloadsPreserveOperandOrder)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src0.mask);

    MOCKER_CPP(vdup, void(vector_float&, float, vector_bool, Literal))
        .times(4)
        .will(invoke(DuplicateFloatStub));
    MOCKER_CPP(vsub, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(SubStub));
    MOCKER_CPP(vdiv, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(DivStub));

    ExpectResult(src0 - src1, src0.mask, RESULT_TOKEN);
    ExpectResult(src0 - 4.0f, src0.mask, RESULT_TOKEN + 1);
    ExpectResult(4.0f - src0, src0.mask, RESULT_TOKEN + 2);
    ExpectResult(src0 / src1, src0.mask, RESULT_TOKEN);
    ExpectResult(src0 / 8.0f, src0.mask, RESULT_TOKEN + 1);
    ExpectResult(8.0f / src0, src0.mask, RESULT_TOKEN + 2);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.subCalls, 3U);
    EXPECT_EQ(gCapture.divCalls, 3U);
    EXPECT_EQ(gCapture.duplicateCalls, 4U);
    EXPECT_EQ(gCapture.scalarCalls, 4U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 4.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[1], 4.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[2], 8.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[3], 8.0f);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, MinMaxOverloadsUseDocumentedIntrinsicPaths)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src0.mask);

    MOCKER_CPP(vmax, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryFloatStub));
    MOCKER_CPP(vmaxs, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(2)
        .will(invoke(ScalarFloatStub));
    MOCKER_CPP(vdup, void(vector_float&, float, vector_bool, Literal))
        .times(2)
        .will(invoke(DuplicateFloatStub));
    MOCKER_CPP(vmin, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(3)
        .will(invoke(BinaryFloatStub));

    ExpectResult(asc::te::max(src0, src1), src0.mask);
    ExpectResult(asc::te::max(src0, 2.0f), src0.mask);
    ExpectResult(asc::te::max(2.0f, src0), src0.mask);
    ExpectResult(asc::te::min(src0, src1), src0.mask);
    ExpectResult(asc::te::min(src0, 3.0f), src0.mask);
    ExpectResult(asc::te::min(3.0f, src0), src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.binaryCalls, 4U);
    EXPECT_EQ(gCapture.duplicateCalls, 2U);
    EXPECT_EQ(gCapture.scalarCalls, 4U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 2.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[1], 2.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[2], 3.0f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[3], 3.0f);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, LogicalAndBitwiseOverloadsUsePredicateAndBroadcastPaths)
{
    auto src0 = MakeReg<int32_t>();
    auto src1 = MakeReg<int32_t>(SRC1_TOKEN);
    ResetCapture(src0.mask);

    MOCKER_CPP(vcmps_eq, void(vector_bool&, vector_int32_t, int32_t, vector_bool))
        .times(1)
        .will(invoke(EqZeroStub));
    MOCKER_CPP(vdup, void(vector_int32_t&, int32_t, vector_bool, Literal))
        .times(8)
        .will(invoke(DuplicateIntStub));
    MOCKER_CPP(vand, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(3)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vor, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(3)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vxor, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(3)
        .will(invoke(BinaryIntStub));

    ExpectResult(!src0, src0.mask, BROADCAST_TOKEN);
    ExpectResult(src0 & src1, src0.mask);
    ExpectResult(src0 & 3, src0.mask);
    ExpectResult(3 & src0, src0.mask);
    ExpectResult(src0 | src1, src0.mask);
    ExpectResult(src0 | 3, src0.mask);
    ExpectResult(3 | src0, src0.mask);
    ExpectResult(src0 ^ src1, src0.mask);
    ExpectResult(src0 ^ 3, src0.mask);
    ExpectResult(3 ^ src0, src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.binaryCalls, 9U);
    EXPECT_EQ(gCapture.duplicateCalls, 8U);
    EXPECT_EQ(gCapture.intScalarCalls, 8U);
    EXPECT_EQ(gCapture.intScalars[0], 0);
    EXPECT_EQ(gCapture.intScalars[1], 1);
    for (size_t i = 2; i < gCapture.intScalarCalls; ++i) {
        EXPECT_EQ(gCapture.intScalars[i], 3);
    }
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.predicateMaskMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, ShiftOverloadsUseRegisterAndInt16ScalarIntrinsics)
{
    auto src = MakeReg<int32_t>();
    auto shift = MakeReg<int32_t>(SRC1_TOKEN);
    ResetCapture(src.mask);

    MOCKER_CPP(vshl, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vshls, void(vector_int32_t&, vector_int32_t, int16_t, vector_bool, Literal))
        .times(1)
        .will(invoke(ShiftScalarIntStub));
    MOCKER_CPP(vshr, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(BinaryIntStub));
    MOCKER_CPP(vshrs, void(vector_int32_t&, vector_int32_t, int16_t, vector_bool, Literal))
        .times(1)
        .will(invoke(ShiftScalarIntStub));

    ExpectResult(src << shift, src.mask);
    ExpectResult(src << 2, src.mask);
    ExpectResult(src >> shift, src.mask);
    ExpectResult(src >> 3, src.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.binaryCalls, 2U);
    EXPECT_EQ(gCapture.scalarCalls, 2U);
    EXPECT_EQ(gCapture.shiftScalars[0], 2);
    EXPECT_EQ(gCapture.shiftScalars[1], 3);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, RegisterComparisonsMapAllSixPredicates)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    ResetCapture(src0.mask);

#define MOCK_COMPARE_REG(INSTR)                                                                                       \
    MOCKER_CPP(INSTR, void(vector_bool&, vector_float, vector_float, vector_bool))                                    \
        .times(1)                                                                                                     \
        .will(invoke(CompareRegFloatStub))

    MOCK_COMPARE_REG(vcmp_eq);
    MOCK_COMPARE_REG(vcmp_ne);
    MOCK_COMPARE_REG(vcmp_lt);
    MOCK_COMPARE_REG(vcmp_le);
    MOCK_COMPARE_REG(vcmp_gt);
    MOCK_COMPARE_REG(vcmp_ge);

#undef MOCK_COMPARE_REG

    EXPECT_TRUE(HasByteValue((src0 == src1).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src0 != src1).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src0 < src1).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src0 <= src1).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src0 > src1).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src0 >= src1).reg, PREDICATE_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.binaryCalls, 6U);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, ScalarComparisonsMapDirectAndReversedPredicates)
{
    auto src = MakeReg<float>();
    ResetCapture(src.mask);

#define MOCK_COMPARE_SCALAR(INSTR)                                                                                    \
    MOCKER_CPP(INSTR, void(vector_bool&, vector_float, float, vector_bool))                                           \
        .times(2)                                                                                                     \
        .will(invoke(CompareScalarFloatStub))

    MOCK_COMPARE_SCALAR(vcmps_eq);
    MOCK_COMPARE_SCALAR(vcmps_ne);
    MOCK_COMPARE_SCALAR(vcmps_lt);
    MOCK_COMPARE_SCALAR(vcmps_le);
    MOCK_COMPARE_SCALAR(vcmps_gt);
    MOCK_COMPARE_SCALAR(vcmps_ge);

#undef MOCK_COMPARE_SCALAR

    EXPECT_TRUE(HasByteValue((src == 1.0f).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((1.0f == src).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src != 1.0f).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((1.0f != src).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src < 1.0f).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((1.0f < src).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src <= 1.0f).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((1.0f <= src).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src > 1.0f).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((1.0f > src).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((src >= 1.0f).reg, PREDICATE_TOKEN));
    EXPECT_TRUE(HasByteValue((1.0f >= src).reg, PREDICATE_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.scalarCalls, 12U);
    for (size_t i = 0; i < gCapture.scalarCalls; ++i) {
        EXPECT_FLOAT_EQ(gCapture.floatScalars[i], 1.0f);
    }
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, CompositeAccumulationApisPreserveFormulaAndDestinationMask)
{
    auto dst = MakeReg<float>(BROADCAST_TOKEN);
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    FillBytes(src0.mask, PREDICATE_TOKEN);
    FillBytes(src1.mask, PREDICATE_TOKEN);
    ResetCapture(dst.mask);

    MOCKER_CPP(vaxpy, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(1)
        .will(invoke(AxpyFloatStub));
    MOCKER_CPP(vmadd, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(MulDstAddFloatStub));
    MOCKER_CPP(vmula, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(MulAddDstFloatStub));

    ExpectResult(asc::te::axpy(dst, src0, 2.5f), dst.mask);
    ExpectResult(asc::te::mul_dst_add(dst, src0, src1), dst.mask);
    ExpectResult(asc::te::mul_add_dst(dst, src0, src1), dst.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.axpyCalls, 1U);
    EXPECT_EQ(gCapture.mulDstAddCalls, 1U);
    EXPECT_EQ(gCapture.mulAddDstCalls, 1U);
    EXPECT_EQ(gCapture.scalarCalls, 1U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 2.5f);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, AbsSubUsesFirstSourceMaskAndPreservesOperandOrder)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    FillBytes(src0.mask, PREDICATE_TOKEN);
    ResetCapture(src0.mask);

    MOCKER_CPP(vabsdif, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(AbsSubFloatStub));

    ExpectResult(asc::te::abs_sub(src0, src1), src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.absSubCalls, 1U);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, ExpSubMapsBothRegisterLayouts)
{
    auto src0 = MakeReg<float>();
    auto src1 = MakeReg<float>(SRC1_TOKEN);
    FillBytes(src0.mask, PREDICATE_TOKEN);
    ResetCapture(src0.mask);

    MOCKER_CPP(vexpdif, void(vector_float&, vector_float, vector_float, vector_bool, int))
        .times(2)
        .will(invoke(ExpSubFloatStub));

    ExpectResult(asc::te::exp_sub(src0, src1), src0.mask);
    ExpectResult(asc::te::exp_sub<asc::te::reg_layout::one>(src0, src1), src0.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.expSubCalls, 2U);
    EXPECT_EQ(gCapture.partCalls, 2U);
    EXPECT_EQ(gCapture.parts[0], PART_EVEN);
    EXPECT_EQ(gCapture.parts[1], PART_ODD);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

TEST_F(TestTensorApiRegCompute, MulsCastMapsBothLayoutsAndReturnsHalf)
{
    auto src = MakeReg<float>();
    FillBytes(src.mask, PREDICATE_TOKEN);
    ResetCapture(src.mask);

    MOCKER_CPP(vmulscvt, void(vector_half&, vector_float, float, vector_bool, int))
        .times(2)
        .will(invoke(MulsCastFloatStub));

    ExpectResult(asc::te::muls_cast(src, 1.5f), src.mask);
    ExpectResult(asc::te::muls_cast<asc::te::reg_layout::one>(src, 3.0f), src.mask);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.mulsCastCalls, 2U);
    EXPECT_EQ(gCapture.scalarCalls, 2U);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[0], 1.5f);
    EXPECT_FLOAT_EQ(gCapture.floatScalars[1], 3.0f);
    EXPECT_EQ(gCapture.partCalls, 2U);
    EXPECT_EQ(gCapture.parts[0], PART_EVEN);
    EXPECT_EQ(gCapture.parts[1], PART_ODD);
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
}

} // namespace
