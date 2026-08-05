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

#include "impl/utils/debug/asc_aicore_dump_common.h"
#include "impl/utils/debug/asc_utils_types.h"

namespace {
using namespace __asc_aicore;

TEST(TestAicoreDumpUtils, L0cCopyUsesByteBasedBlockCount)
{
    struct TestCase {
        uint32_t dumpBytes;
        uint16_t blockCount;
        uint16_t nSize;
        uint16_t mSize;
        uint32_t physicalBytes;
    };
    constexpr TestCase testCases[] = {{0U, 0U, 0U, 16U, 0U},        {32U, 1U, 16U, 16U, 1024U},
                                      {128U, 1U, 16U, 16U, 1024U},  {1024U, 1U, 16U, 16U, 1024U},
                                      {1056U, 2U, 32U, 16U, 2048U}, {2048U, 2U, 32U, 16U, 2048U}};

    for (const auto& testCase : testCases) {
        const L0cDumpCopyParams params = get_l0c_dump_copy_params(testCase.dumpBytes);
        EXPECT_EQ(params.blockCount, testCase.blockCount);
        EXPECT_EQ(params.nSize, testCase.nSize);
        EXPECT_EQ(params.mSize, testCase.mSize);
        EXPECT_EQ(params.physicalBytes, testCase.physicalBytes);
    }

    EXPECT_EQ(get_l0c_dump_max_copy_bytes(32U), 1055U);
    EXPECT_EQ(get_l0c_dump_max_copy_bytes(1024U), 1055U);
    EXPECT_EQ(get_l0c_dump_max_copy_bytes(1056U), 2079U);
    EXPECT_EQ(get_l0c_dump_reservation_bytes(32U), 1055U);
    EXPECT_EQ(get_l0c_dump_reservation_bytes(1024U), 1055U);
    EXPECT_EQ(get_l0c_dump_reservation_bytes((static_cast<uint32_t>(UINT16_MAX) + 1U) * 1024U), 64U * 1024U * 1024U);
}

TEST(TestAicoreDumpUtils, L0cCopyHandlesFloatElementBoundaries)
{
    constexpr uint32_t dataBlockBytes = 32U;
    constexpr uint32_t float255Bytes = 255U * sizeof(float);
    constexpr uint32_t float256Bytes = 256U * sizeof(float);
    constexpr uint32_t float257Bytes = 257U * sizeof(float);
    constexpr uint32_t float255AlignedBytes = (float255Bytes + dataBlockBytes - 1U) / dataBlockBytes * dataBlockBytes;
    constexpr uint32_t float256AlignedBytes = (float256Bytes + dataBlockBytes - 1U) / dataBlockBytes * dataBlockBytes;
    constexpr uint32_t float257AlignedBytes = (float257Bytes + dataBlockBytes - 1U) / dataBlockBytes * dataBlockBytes;

    EXPECT_EQ(get_l0c_dump_copy_params(float255AlignedBytes).blockCount, 1U);
    EXPECT_EQ(get_l0c_dump_copy_params(float256AlignedBytes).blockCount, 1U);
    EXPECT_EQ(get_l0c_dump_copy_params(float257AlignedBytes).blockCount, 2U);
}

TEST(TestAicoreDumpUtils, L0cCopyProtectsTheTwentyNinthOneKilobyteDump)
{
    constexpr uint32_t ringBufLen = 32U * 1024U;
    constexpr uint32_t dumpTlvHeadLen = 72U;
    constexpr uint32_t dumpLen = 1024U;
    constexpr uint32_t tlvLen = dumpTlvHeadLen + dumpLen;
    constexpr uint32_t maxResvTlvLen = dumpTlvHeadLen + get_l0c_dump_reservation_bytes(dumpLen);

    EXPECT_LE(28U * tlvLen + maxResvTlvLen, ringBufLen);
    EXPECT_GT(29U * tlvLen + maxResvTlvLen, ringBufLen);
    EXPECT_LE(dumpTlvHeadLen + get_l0c_dump_reservation_bytes(30U * 1024U), ringBufLen);
}

TEST(TestAicoreDumpUtils, L0cDestinationAlignmentOffsetCoversAllResidues)
{
    EXPECT_EQ(get_l0c_dump_gm_align_offset(0U), 0U);
    EXPECT_EQ(get_l0c_dump_gm_align_offset(1U), 31U);
    EXPECT_EQ(get_l0c_dump_gm_align_offset(8U), 24U);
    EXPECT_EQ(get_l0c_dump_gm_align_offset(31U), 1U);
}

TEST(TestAicoreDumpUtils, RingBufferWritesSkipOnlyWhenTheTailCanHoldIt)
{
    constexpr uint32_t ringBufLen = 1024U;
    constexpr uint32_t skipTlvLen = 8U;

    EXPECT_FALSE(should_ringbuf_skip_directly(ringBufLen - skipTlvLen, ringBufLen, skipTlvLen));
    EXPECT_TRUE(should_ringbuf_skip_directly(ringBufLen - skipTlvLen + 1U, ringBufLen, skipTlvLen));
    EXPECT_TRUE(should_ringbuf_skip_directly(ringBufLen + 1U, ringBufLen, skipTlvLen));
}
} // namespace
