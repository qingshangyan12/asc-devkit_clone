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
asc::te::reg_tensor<T> make_reg(uint8_t token = SRC_TOKEN)
{
    asc::te::reg_tensor<T> value {};
    fill_bytes(value.reg, token);
    fill_bytes(value.mask, MASK_TOKEN);
    return value;
}

struct capture {
    vector_bool expected_mask {};
    bool masks_match = true;
    size_t binary_calls = 0;
    size_t scalar_calls = 0;
    std::array<int16_t, 2> shifts {};
};

capture g_capture;

void capture_mask(const vector_bool& mask)
{
    g_capture.masks_match = g_capture.masks_match && same_bytes(mask, g_capture.expected_mask);
}

void binary_int_stub(vector_int32_t& dst, vector_int32_t, vector_int32_t, vector_bool mask, Literal)
{
    capture_mask(mask);
    ++g_capture.binary_calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void shift_scalar_stub(vector_int32_t& dst, vector_int32_t, int16_t shift, vector_bool mask, Literal)
{
    capture_mask(mask);
    g_capture.shifts[g_capture.scalar_calls++] = shift;
    fill_bytes(dst, RESULT_TOKEN);
}

void eq_zero_stub(vector_bool& dst, vector_int32_t, int32_t value, vector_bool mask)
{
    capture_mask(mask);
    EXPECT_EQ(value, 0);
    fill_bytes(dst, PREDICATE_TOKEN);
}

void duplicate_int_stub(vector_int32_t& dst, int32_t value, vector_bool mask, Literal)
{
    if (value == 0) {
        capture_mask(mask);
        fill_bytes(dst, 0);
    } else {
        EXPECT_EQ(value, 1);
        EXPECT_TRUE(has_byte_value(mask, PREDICATE_TOKEN));
        fill_bytes(dst, RESULT_TOKEN);
    }
}

class test_tensor_api_logical_compute : public testing::Test {
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
__aicore__ inline void compile_logical_not()
{
    asc::te::reg_tensor<T> src {};
    (void)!src;
}

template <typename T>
__aicore__ inline void compile_bitwise_and()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 & src1);
    (void)(src0 & scalar);
    (void)(scalar & src0);
}

template <typename T>
__aicore__ inline void compile_bitwise_or()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 | src1);
    (void)(src0 | scalar);
    (void)(scalar | src0);
}

template <typename T>
__aicore__ inline void compile_bitwise_xor()
{
    asc::te::reg_tensor<T> src0 {};
    asc::te::reg_tensor<T> src1 {};
    T scalar {};
    (void)(src0 ^ src1);
    (void)(src0 ^ scalar);
    (void)(scalar ^ src0);
}

template <typename T, typename shift_type>
__aicore__ inline void compile_shift()
{
    asc::te::reg_tensor<T> src {};
    asc::te::reg_tensor<shift_type> shift {};
    (void)(src << shift);
    (void)(src << static_cast<int16_t>(2));
    (void)(src >> shift);
    (void)(src >> static_cast<int16_t>(2));
}

TEST_F(test_tensor_api_logical_compute, compile_public_apis_and_supported_types)
{
    AscendC::SetGCoreType(0);
    compile_logical_not<int8_t>();
    compile_logical_not<uint8_t>();
    compile_logical_not<int16_t>();
    compile_logical_not<uint16_t>();
    compile_logical_not<half>();
    compile_logical_not<bfloat16_t>();
    compile_logical_not<int32_t>();
    compile_logical_not<uint32_t>();
    compile_logical_not<float>();

    compile_bitwise_and<int8_t>();
    compile_bitwise_and<uint8_t>();
    compile_bitwise_and<fp8_e5m2_t>();
    compile_bitwise_and<fp8_e4m3fn_t>();
    compile_bitwise_and<int16_t>();
    compile_bitwise_and<uint16_t>();
    compile_bitwise_and<half>();
    compile_bitwise_and<bfloat16_t>();
    compile_bitwise_and<int32_t>();
    compile_bitwise_and<uint32_t>();
    compile_bitwise_and<float>();

    compile_bitwise_or<int8_t>();
    compile_bitwise_or<uint8_t>();
    compile_bitwise_or<int16_t>();
    compile_bitwise_or<uint16_t>();
    compile_bitwise_or<half>();
    compile_bitwise_or<int32_t>();
    compile_bitwise_or<uint32_t>();
    compile_bitwise_or<float>();

    compile_bitwise_xor<int8_t>();
    compile_bitwise_xor<uint8_t>();
    compile_bitwise_xor<int16_t>();
    compile_bitwise_xor<uint16_t>();
    compile_bitwise_xor<int32_t>();
    compile_bitwise_xor<uint32_t>();

    compile_shift<int8_t, int8_t>();
    compile_shift<uint8_t, int8_t>();
    compile_shift<int16_t, int16_t>();
    compile_shift<uint16_t, int16_t>();
    compile_shift<int32_t, int32_t>();
    compile_shift<uint32_t, int32_t>();

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

TEST_F(test_tensor_api_logical_compute, logical_not_returns_one_for_zero_lanes)
{
    auto src = make_reg<int32_t>();
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vcmps_eq, void(vector_bool&, vector_int32_t, int32_t, vector_bool))
        .times(1)
        .will(invoke(eq_zero_stub));
    MOCKER_CPP(vdup, void(vector_int32_t&, int32_t, vector_bool, Literal))
        .times(2)
        .will(invoke(duplicate_int_stub));

    auto result = !src;

    GlobalMockObject::verify();
    EXPECT_TRUE(has_byte_value(result.reg, RESULT_TOKEN));
    EXPECT_TRUE(same_bytes(result.mask, src.mask));
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_logical_compute, register_bitwise_and_shift_apis_forward_mask)
{
    auto src = make_reg<int32_t>();
    auto other = make_reg<int32_t>(0x22);
    auto shift = make_reg<int32_t>(0x33);
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vand, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_int_stub));
    MOCKER_CPP(vor, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_int_stub));
    MOCKER_CPP(vxor, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_int_stub));
    MOCKER_CPP(vshl, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_int_stub));
    MOCKER_CPP(vshr, void(vector_int32_t&, vector_int32_t, vector_int32_t, vector_bool, Literal))
        .times(1)
        .will(invoke(binary_int_stub));

    EXPECT_TRUE(has_byte_value((src & other).reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value((src | other).reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value((src ^ other).reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value((src << shift).reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value((src >> shift).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.binary_calls, 5U);
    EXPECT_TRUE(g_capture.masks_match);
}

TEST_F(test_tensor_api_logical_compute, scalar_shift_uses_int16_immediate)
{
    auto src = make_reg<int32_t>();
    g_capture = {};
    g_capture.expected_mask = src.mask;
    MOCKER_CPP(vshls, void(vector_int32_t&, vector_int32_t, int16_t, vector_bool, Literal))
        .times(1)
        .will(invoke(shift_scalar_stub));
    MOCKER_CPP(vshrs, void(vector_int32_t&, vector_int32_t, int16_t, vector_bool, Literal))
        .times(1)
        .will(invoke(shift_scalar_stub));

    EXPECT_TRUE(has_byte_value((src << static_cast<int16_t>(2)).reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value((src >> static_cast<int16_t>(3)).reg, RESULT_TOKEN));

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.shifts[0], 2);
    EXPECT_EQ(g_capture.shifts[1], 3);
    EXPECT_TRUE(g_capture.masks_match);
}

} // namespace
