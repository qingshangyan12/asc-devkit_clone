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
#include <cstdint>
#include <cstring>
#include <type_traits>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/experimental/reg/histogram_compute.h"

namespace {

constexpr uint8_t DST_TOKEN = 0x2A;
constexpr uint8_t SRC_TOKEN = 0x3B;
constexpr uint8_t MASK_TOKEN = 0x4C;
constexpr uint8_t RESULT_TOKEN = 0x5D;
constexpr asc::te::histogram_options frequency_high_options {
    asc::te::histogram_half::high, asc::te::histogram_mode::frequency};
constexpr asc::te::histogram_options cumulative_low_options {
    asc::te::histogram_half::low, asc::te::histogram_mode::cumulative};
constexpr asc::te::histogram_options cumulative_high_options {
    asc::te::histogram_half::high, asc::te::histogram_mode::cumulative};

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

struct histogram_capture {
    vector_bool expected_mask {};
    bool operands_match = true;
    bool masks_match = true;
    size_t frequency_calls = 0;
    size_t cumulative_calls = 0;
    std::array<uint32_t, 2> frequency_bins {};
    std::array<uint32_t, 2> cumulative_bins {};
};

histogram_capture g_capture;

void capture_call(vector_u16& dst, vector_u8 src, vector_bool mask, uint32_t bin,
    std::array<uint32_t, 2>& bins, size_t& calls)
{
    g_capture.operands_match =
        g_capture.operands_match && has_byte_value(dst, DST_TOKEN) && has_byte_value(src, SRC_TOKEN);
    g_capture.masks_match = g_capture.masks_match && same_bytes(mask, g_capture.expected_mask);
    if (calls < bins.size()) {
        bins[calls] = bin;
    }
    ++calls;
    fill_bytes(dst, RESULT_TOKEN);
}

void frequency_histogram_stub(vector_u16& dst, vector_u8 src, vector_bool mask, uint32_t bin)
{
    capture_call(dst, src, mask, bin, g_capture.frequency_bins, g_capture.frequency_calls);
}

void cumulative_histogram_stub(vector_u16& dst, vector_u8 src, vector_bool mask, uint32_t bin)
{
    capture_call(dst, src, mask, bin, g_capture.cumulative_bins, g_capture.cumulative_calls);
}

class test_tensor_api_histogram_compute : public testing::Test {
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

TEST_F(test_tensor_api_histogram_compute, compile_return_value_api_and_type_matrix)
{
    AscendC::SetGCoreType(0);
    asc::te::reg_tensor<uint16_t> dst {};
    asc::te::reg_tensor<uint8_t> src {};

    static_assert(std::is_same_v<decltype(asc::te::histograms(dst, src)),
        asc::te::reg_tensor<uint16_t>>);
    static_assert(std::is_same_v<decltype(asc::te::histograms<cumulative_high_options>(dst, src)),
        asc::te::reg_tensor<uint16_t>>);
}

TEST_F(test_tensor_api_histogram_compute, maps_four_modes_to_existing_cann_histogram_apis)
{
    asc::te::reg_tensor<uint16_t> dst {};
    asc::te::reg_tensor<uint8_t> src {};
    fill_bytes(dst.reg, DST_TOKEN);
    fill_bytes(dst.mask, 0x6E);
    fill_bytes(src.reg, SRC_TOKEN);
    fill_bytes(src.mask, MASK_TOKEN);
    g_capture = {};
    g_capture.expected_mask = src.mask;

    MOCKER_CPP(dhistv2, void(vector_u16&, vector_u8, vector_bool, uint32_t))
        .times(2)
        .will(invoke(frequency_histogram_stub));
    MOCKER_CPP(chistv2, void(vector_u16&, vector_u8, vector_bool, uint32_t))
        .times(2)
        .will(invoke(cumulative_histogram_stub));

    auto frequency_low = asc::te::histograms(dst, src);
    auto frequency_high = asc::te::histograms<frequency_high_options>(dst, src);
    auto cumulative_low = asc::te::histograms<cumulative_low_options>(dst, src);
    auto cumulative_high = asc::te::histograms<cumulative_high_options>(dst, src);

    GlobalMockObject::verify();
    EXPECT_EQ(g_capture.frequency_calls, 2U);
    EXPECT_EQ(g_capture.cumulative_calls, 2U);
    EXPECT_EQ(g_capture.frequency_bins[0], static_cast<uint32_t>(Bin_N0));
    EXPECT_EQ(g_capture.frequency_bins[1], static_cast<uint32_t>(Bin_N1));
    EXPECT_EQ(g_capture.cumulative_bins[0], static_cast<uint32_t>(Bin_N0));
    EXPECT_EQ(g_capture.cumulative_bins[1], static_cast<uint32_t>(Bin_N1));
    EXPECT_TRUE(g_capture.operands_match);
    EXPECT_TRUE(g_capture.masks_match);
    EXPECT_TRUE(has_byte_value(frequency_low.reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value(frequency_high.reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value(cumulative_low.reg, RESULT_TOKEN));
    EXPECT_TRUE(has_byte_value(cumulative_high.reg, RESULT_TOKEN));
    EXPECT_TRUE(same_bytes(frequency_low.mask, dst.mask));
    EXPECT_TRUE(same_bytes(frequency_high.mask, dst.mask));
    EXPECT_TRUE(same_bytes(cumulative_low.mask, dst.mask));
    EXPECT_TRUE(same_bytes(cumulative_high.mask, dst.mask));
}

} // namespace
