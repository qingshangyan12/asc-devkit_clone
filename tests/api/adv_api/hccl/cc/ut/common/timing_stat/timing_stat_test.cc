/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "timing_stat.h"

#include <cstdint>
#include <string>

#include "gtest/gtest.h"

namespace {
uint64_t g_currentTimestamp = 0U;
uint64_t g_clockStep = 1U;
uint32_t g_clockCallCount = 0U;

uint64_t TestClock()
{
    const uint64_t timestamp = g_currentTimestamp;
    g_currentTimestamp += g_clockStep;
    ++g_clockCallCount;
    return timestamp;
}

class TimingStatTest : public testing::Test {
protected:
    void SetUp() override
    {
        g_currentTimestamp = 100U;
        g_clockStep = 10U;
        g_clockCallCount = 0U;
    }
};

TEST_F(TimingStatTest, KeepsFirstMarkAndUpdatesLatestMark)
{
    Hccl::TimingStat timingStat("test_scenario", TestClock);

    EXPECT_TRUE(timingStat.Mark("kernel_entry"));
    EXPECT_TRUE(timingStat.Mark("first_prepare"));
    EXPECT_TRUE(timingStat.Mark("first_prepare"));
    EXPECT_TRUE(timingStat.Mark("last_expand_end"));
    EXPECT_TRUE(timingStat.Mark("last_expand_end", Hccl::TimingStat::MarkPolicy::KEEP_LATEST));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingStat.GetTimestamp("kernel_entry", timestamp));
    EXPECT_EQ(timestamp, 100U);
    EXPECT_TRUE(timingStat.GetTimestamp("first_prepare", timestamp));
    EXPECT_EQ(timestamp, 110U);
    EXPECT_TRUE(timingStat.GetTimestamp("last_expand_end", timestamp));
    EXPECT_EQ(timestamp, 130U);
    EXPECT_EQ(timingStat.Size(), 3U);
    EXPECT_EQ(g_clockCallCount, 4U);

    uint64_t elapsed = 0U;
    EXPECT_TRUE(timingStat.GetElapsed("kernel_entry", "last_expand_end", elapsed));
    EXPECT_EQ(elapsed, 30U);
}

TEST_F(TimingStatTest, RejectsInvalidAndLateMarks)
{
    Hccl::TimingStat timingStat("test_scenario", TestClock);

    EXPECT_FALSE(timingStat.Mark(nullptr));
    EXPECT_FALSE(timingStat.Mark(""));
    EXPECT_TRUE(timingStat.Mark("kernel_entry"));
    timingStat.Report("complete");

    EXPECT_TRUE(timingStat.IsReported());
    EXPECT_FALSE(timingStat.Mark("late_stage"));
    EXPECT_EQ(timingStat.Size(), 1U);

    uint64_t value = 0U;
    EXPECT_FALSE(timingStat.GetTimestamp("missing", value));
    EXPECT_FALSE(timingStat.GetElapsed("kernel_entry", "missing", value));
}

TEST_F(TimingStatTest, ResetMakesSessionReusable)
{
    Hccl::TimingStat timingStat("test_scenario", TestClock);
    EXPECT_TRUE(timingStat.Mark("kernel_entry"));
    timingStat.Report("complete");

    timingStat.Reset();
    EXPECT_FALSE(timingStat.IsReported());
    EXPECT_EQ(timingStat.Size(), 0U);
    EXPECT_TRUE(timingStat.Mark("new_entry"));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingStat.GetTimestamp("new_entry", timestamp));
    EXPECT_EQ(timestamp, 110U);
}

TEST_F(TimingStatTest, RejectsMarksBeyondCapacity)
{
    Hccl::TimingStat timingStat("test_scenario", TestClock);
    for (std::size_t i = 0U; i < Hccl::TimingStat::CAPACITY; ++i) {
        const std::string stage = "stage_" + std::to_string(i);
        EXPECT_TRUE(timingStat.Mark(stage.c_str()));
    }

    EXPECT_FALSE(timingStat.Mark("overflow"));
    EXPECT_EQ(timingStat.Size(), Hccl::TimingStat::CAPACITY);
}
} // namespace
