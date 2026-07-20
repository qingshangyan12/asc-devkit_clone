/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "aicpu_timing_recorder.h"

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

class AicpuTimingRecorderTest : public testing::Test {
protected:
    void SetUp() override
    {
        g_currentTimestamp = 100U;
        g_clockStep = 10U;
        g_clockCallCount = 0U;
    }
};

TEST_F(AicpuTimingRecorderTest, KeepsFirstRecordAndUpdatesLatestRecord)
{
    Hccl::AicpuTimingRecorder timingRecorder("test_scenario", TestClock);

    EXPECT_TRUE(timingRecorder.Record("kernel_entry"));
    EXPECT_TRUE(timingRecorder.Record("first_prepare"));
    EXPECT_TRUE(timingRecorder.Record("first_prepare"));
    EXPECT_TRUE(timingRecorder.Record("last_expand_end"));
    EXPECT_TRUE(timingRecorder.Record("last_expand_end", Hccl::AicpuTimingRecorder::RecordPolicy::KEEP_LATEST));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingRecorder.GetTimestamp("kernel_entry", timestamp));
    EXPECT_EQ(timestamp, 100U);
    EXPECT_TRUE(timingRecorder.GetTimestamp("first_prepare", timestamp));
    EXPECT_EQ(timestamp, 110U);
    EXPECT_TRUE(timingRecorder.GetTimestamp("last_expand_end", timestamp));
    EXPECT_EQ(timestamp, 130U);
    EXPECT_EQ(timingRecorder.Size(), 3U);
    EXPECT_EQ(g_clockCallCount, 4U);

    uint64_t elapsed = 0U;
    EXPECT_TRUE(timingRecorder.GetElapsed("kernel_entry", "last_expand_end", elapsed));
    EXPECT_EQ(elapsed, 30U);
}

TEST_F(AicpuTimingRecorderTest, RejectsInvalidAndLateRecords)
{
    Hccl::AicpuTimingRecorder timingRecorder("test_scenario", TestClock);

    EXPECT_FALSE(timingRecorder.Record(nullptr));
    EXPECT_FALSE(timingRecorder.Record(""));
    EXPECT_TRUE(timingRecorder.Record("kernel_entry"));
    timingRecorder.Report("complete");

    EXPECT_TRUE(timingRecorder.IsReported());
    EXPECT_FALSE(timingRecorder.Record("late_stage"));
    EXPECT_EQ(timingRecorder.Size(), 1U);

    uint64_t value = 0U;
    EXPECT_FALSE(timingRecorder.GetTimestamp("missing", value));
    EXPECT_FALSE(timingRecorder.GetElapsed("kernel_entry", "missing", value));
}

TEST_F(AicpuTimingRecorderTest, ResetMakesSessionReusable)
{
    Hccl::AicpuTimingRecorder timingRecorder("test_scenario", TestClock);
    EXPECT_TRUE(timingRecorder.Record("kernel_entry"));
    timingRecorder.Report("complete");

    timingRecorder.Reset();
    EXPECT_FALSE(timingRecorder.IsReported());
    EXPECT_EQ(timingRecorder.Size(), 0U);
    EXPECT_TRUE(timingRecorder.Record("new_entry"));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingRecorder.GetTimestamp("new_entry", timestamp));
    EXPECT_EQ(timestamp, 110U);
}

TEST_F(AicpuTimingRecorderTest, RejectsRecordsBeyondCapacity)
{
    Hccl::AicpuTimingRecorder timingRecorder("test_scenario", TestClock);
    for (std::size_t i = 0U; i < Hccl::AicpuTimingRecorder::CAPACITY; ++i) {
        const std::string stage = "stage_" + std::to_string(i);
        EXPECT_TRUE(timingRecorder.Record(stage.c_str()));
    }

    EXPECT_FALSE(timingRecorder.Record("overflow"));
    EXPECT_EQ(timingRecorder.Size(), Hccl::AicpuTimingRecorder::CAPACITY);
}
} // namespace
