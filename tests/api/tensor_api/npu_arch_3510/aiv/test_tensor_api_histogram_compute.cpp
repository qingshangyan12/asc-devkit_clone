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

struct HistogramCapture {
    vector_bool expectedMask {};
    bool operandsMatch = true;
    bool masksMatch = true;
    size_t frequencyCalls = 0;
    size_t cumulativeCalls = 0;
    std::array<uint32_t, 2> frequencyBins {};
    std::array<uint32_t, 2> cumulativeBins {};
};

HistogramCapture gCapture;

void CaptureCall(vector_u16& dst, vector_u8 src, vector_bool mask, uint32_t bin,
    std::array<uint32_t, 2>& bins, size_t& calls)
{
    gCapture.operandsMatch =
        gCapture.operandsMatch && HasByteValue(dst, DST_TOKEN) && HasByteValue(src, SRC_TOKEN);
    gCapture.masksMatch = gCapture.masksMatch && SameBytes(mask, gCapture.expectedMask);
    if (calls < bins.size()) {
        bins[calls] = bin;
    }
    ++calls;
    FillBytes(dst, RESULT_TOKEN);
}

void FrequencyHistogramStub(vector_u16& dst, vector_u8 src, vector_bool mask, uint32_t bin)
{
    CaptureCall(dst, src, mask, bin, gCapture.frequencyBins, gCapture.frequencyCalls);
}

void CumulativeHistogramStub(vector_u16& dst, vector_u8 src, vector_bool mask, uint32_t bin)
{
    CaptureCall(dst, src, mask, bin, gCapture.cumulativeBins, gCapture.cumulativeCalls);
}

class TestTensorApiHistogramCompute : public testing::Test {
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

TEST_F(TestTensorApiHistogramCompute, CompileReturnValueApiAndTypeMatrix)
{
    AscendC::SetGCoreType(0);
    asc::te::reg_tensor<uint16_t> dst {};
    asc::te::reg_tensor<uint8_t> src {};

    static_assert(std::is_same_v<decltype(asc::te::histograms<
                                     asc::te::histogram_bin::bin0, asc::te::histogram_type::frequency>(dst, src)),
        asc::te::reg_tensor<uint16_t>>);
    static_assert(std::is_same_v<decltype(asc::te::histograms<
                                     asc::te::histogram_bin::bin1, asc::te::histogram_type::accumulate>(dst, src)),
        asc::te::reg_tensor<uint16_t>>);
}

TEST_F(TestTensorApiHistogramCompute, MapsFourModesToExistingCannHistogramApis)
{
    asc::te::reg_tensor<uint16_t> dst {};
    asc::te::reg_tensor<uint8_t> src {};
    FillBytes(dst.reg, DST_TOKEN);
    FillBytes(dst.mask, 0x6E);
    FillBytes(src.reg, SRC_TOKEN);
    FillBytes(src.mask, MASK_TOKEN);
    gCapture = {};
    gCapture.expectedMask = src.mask;

    MOCKER_CPP(dhistv2, void(vector_u16&, vector_u8, vector_bool, uint32_t))
        .times(2)
        .will(invoke(FrequencyHistogramStub));
    MOCKER_CPP(chistv2, void(vector_u16&, vector_u8, vector_bool, uint32_t))
        .times(2)
        .will(invoke(CumulativeHistogramStub));

    auto frequencyBin0 = asc::te::histograms<
        asc::te::histogram_bin::bin0, asc::te::histogram_type::frequency>(dst, src);
    auto frequencyBin1 = asc::te::histograms<
        asc::te::histogram_bin::bin1, asc::te::histogram_type::frequency>(dst, src);
    auto cumulativeBin0 = asc::te::histograms<
        asc::te::histogram_bin::bin0, asc::te::histogram_type::accumulate>(dst, src);
    auto cumulativeBin1 = asc::te::histograms<
        asc::te::histogram_bin::bin1, asc::te::histogram_type::accumulate>(dst, src);

    GlobalMockObject::verify();
    EXPECT_EQ(gCapture.frequencyCalls, 2U);
    EXPECT_EQ(gCapture.cumulativeCalls, 2U);
    EXPECT_EQ(gCapture.frequencyBins[0], static_cast<uint32_t>(Bin_N0));
    EXPECT_EQ(gCapture.frequencyBins[1], static_cast<uint32_t>(Bin_N1));
    EXPECT_EQ(gCapture.cumulativeBins[0], static_cast<uint32_t>(Bin_N0));
    EXPECT_EQ(gCapture.cumulativeBins[1], static_cast<uint32_t>(Bin_N1));
    EXPECT_TRUE(gCapture.operandsMatch);
    EXPECT_TRUE(gCapture.masksMatch);
    EXPECT_TRUE(HasByteValue(frequencyBin0.reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue(frequencyBin1.reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue(cumulativeBin0.reg, RESULT_TOKEN));
    EXPECT_TRUE(HasByteValue(cumulativeBin1.reg, RESULT_TOKEN));
    EXPECT_TRUE(SameBytes(frequencyBin0.mask, dst.mask));
    EXPECT_TRUE(SameBytes(frequencyBin1.mask, dst.mask));
    EXPECT_TRUE(SameBytes(cumulativeBin0.mask, dst.mask));
    EXPECT_TRUE(SameBytes(cumulativeBin1.mask, dst.mask));
}

} // namespace
