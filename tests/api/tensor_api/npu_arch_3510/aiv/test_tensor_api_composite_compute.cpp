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
asc::te::reg_tensor<T> make_reg(uint8_t token)
{
    asc::te::reg_tensor<T> value {};
    fill_bytes(value.reg, token);
    fill_bytes(value.mask, MASK_TOKEN);
    return value;
}

struct capture {
    vector_bool expected_mask {};
    bool masks_match = true;
    bool operands_match = true;
    size_t calls = 0;
    size_t part_calls = 0;
    std::array<int, 4> parts {};
    std::array<float, 4> scalars {};
};

capture g_capture;

void capture_mask(const vector_bool& mask)
{
    g_capture.masks_match = g_capture.masks_match && same_bytes(mask, g_capture.expected_mask);
}

void axpy_stub(vector_float& dst, vector_float src, float scalar, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match && has_byte_value(dst, DST_TOKEN) &&
        has_byte_value(src, SRC0_TOKEN);
    g_capture.scalars[g_capture.calls++] = scalar;
    fill_bytes(dst, RESULT_TOKEN);
}

void absdiff_stub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match && has_byte_value(src0, SRC0_TOKEN) &&
        has_byte_value(src1, SRC1_TOKEN);
    ++g_capture.calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void exp_sub_stub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, int part)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match && has_byte_value(src0, SRC0_TOKEN) &&
        has_byte_value(src1, SRC1_TOKEN);
    g_capture.parts[g_capture.part_calls++] = part;
    ++g_capture.calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void exp_sub_half_stub(vector_float& dst, vector_half, vector_half, vector_bool mask, int part)
{
    capture_mask(mask);
    g_capture.parts[g_capture.part_calls++] = part;
    ++g_capture.calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void fma_stub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match && has_byte_value(dst, SRC0_TOKEN) &&
        has_byte_value(src0, SRC1_TOKEN) && has_byte_value(src1, DST_TOKEN);
    ++g_capture.calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void muls_cast_stub(vector_half& dst, vector_float src, float scalar, vector_bool mask, int part)
{
    capture_mask(mask);
    g_capture.operands_match = g_capture.operands_match && has_byte_value(src, SRC0_TOKEN);
    g_capture.scalars[g_capture.calls++] = scalar;
    g_capture.parts[g_capture.part_calls++] = part;
    fill_bytes(dst, RESULT_TOKEN);
}

class test_tensor_api_composite_compute : public testing::Test {
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
__aicore__ inline void compile_axpy()
{
    asc::te::reg_tensor<T> dst {};
    asc::te::reg_tensor<T> src {};
    (void)asc::te::axpy(dst, src, T {});
}

template <typename T>
__aicore__ inline void compile_absdiff()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    (void)asc::te::absdiff(src0, src1);
}

template <typename T>
__aicore__ inline void compile_exp_sub()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    (void)asc::te::exp_sub(src0, src1);
}

template <typename T>
__aicore__ inline void compile_fma()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    asc::te::reg_tensor<T> src2 {};
    (void)asc::te::fma(src0, src1, src2);
}

TEST_F(test_tensor_api_composite_compute, compile_five_apis_and_supported_types)
{
    AscendC::SetGCoreType(0);
    compile_axpy<half>();
    compile_axpy<float>();
    compile_absdiff<half>();
    compile_absdiff<float>();
    compile_exp_sub<half>();
    compile_exp_sub<float>();
    compile_fma<half>();
    compile_fma<float>();

    static_assert(!asc::te::detail::supports_axpy_v<int64_t>);
    static_assert(!asc::te::detail::supports_absdiff_v<int64_t>);
    static_assert(!asc::te::detail::supports_fma_v<int32_t>);

    asc::te::reg_tensor<float> src {};
    static_assert(std::is_same_v<decltype(asc::te::muls_cast(src, 2.0f)), asc::te::reg_tensor<half>>);
    (void)asc::te::muls_cast(src, 2.0f);
}

TEST_F(test_tensor_api_composite_compute, accumulation_apis_use_first_operand_mask)
{
    auto dst = make_reg<float>(DST_TOKEN);
    auto src0 = make_reg<float>(SRC0_TOKEN);
    auto src1 = make_reg<float>(SRC1_TOKEN);
    fill_bytes(src0.mask, 0x70);
    fill_bytes(src1.mask, 0x71);
    g_capture = {};
    g_capture.expected_mask = dst.mask;
    MOCKER_CPP(vaxpy, void(vector_float&, vector_float, float, vector_bool, Literal))
        .times(1)
        .will(invoke(axpy_stub));
    MOCKER_CPP(vmadd, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(fma_stub));

    EXPECT_TRUE(has_byte_value(asc::te::axpy(dst, src0, 2.5f).reg, RESULT_TOKEN));
    g_capture.expected_mask = src0.mask;
    EXPECT_TRUE(has_byte_value(asc::te::fma(src0, src1, dst).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 2U);
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 2.5f);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_composite_compute, difference_apis_preserve_source_order_and_supported_layout)
{
    auto src0 = make_reg<float>(SRC0_TOKEN);
    auto src1 = make_reg<float>(SRC1_TOKEN);
    g_capture = {};
    g_capture.expected_mask = src0.mask;
    MOCKER_CPP(vabsdif, void(vector_float&, vector_float, vector_float, vector_bool, Literal))
        .times(1)
        .will(invoke(absdiff_stub));
    MOCKER_CPP(vexpdif, void(vector_float&, vector_float, vector_float, vector_bool, int))
        .times(1)
        .will(invoke(exp_sub_stub));
    MOCKER_CPP(vexpdif, void(vector_float&, vector_half, vector_half, vector_bool, int))
        .times(1)
        .will(invoke(exp_sub_half_stub));

    EXPECT_TRUE(has_byte_value(asc::te::absdiff(src0, src1).reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value(asc::te::exp_sub(src0, src1).reg, RESULT_TOKEN));
    auto half_src0 = make_reg<half>(SRC0_TOKEN);
    auto half_src1 = make_reg<half>(SRC1_TOKEN);
    EXPECT_TRUE(has_byte_value(asc::te::exp_sub(half_src0, half_src1).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.calls, 3U);
    EXPECT_EQ(g_capture.parts[0], PART_EVEN);
    EXPECT_EQ(g_capture.parts[1], PART_EVEN);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_composite_compute, muls_cast_maps_supported_layout)
{
    auto src = make_reg<float>(SRC0_TOKEN);
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vmulscvt, void(vector_half&, vector_float, float, vector_bool, int))
        .times(1)
        .will(invoke(muls_cast_stub));

    EXPECT_TRUE(has_byte_value(asc::te::muls_cast(src, 1.5f).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_FLOAT_EQ(g_capture.scalars[0], 1.5f);
    EXPECT_EQ(g_capture.parts[0], PART_EVEN);
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
}

} // namespace
