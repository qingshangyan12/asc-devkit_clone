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

#include "gtest/gtest.h"

namespace {
using Recorder = Hccl::AicpuTimingRecorder;

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
    Recorder timingRecorder("test_scenario", TestClock);

    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_ACCEPTED));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_ACCEPTED));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_EXPAND_END));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_EXPAND_END, Recorder::RecordPolicy::KEEP_LATEST));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingRecorder.GetTimestamp(Recorder::Stage::KERNEL_ENTRY, timestamp));
    EXPECT_EQ(timestamp, 100U);
    EXPECT_TRUE(timingRecorder.GetTimestamp(Recorder::Stage::FIRST_PREPARE_ACCEPTED, timestamp));
    EXPECT_EQ(timestamp, 110U);
    EXPECT_TRUE(timingRecorder.GetTimestamp(Recorder::Stage::FIRST_PREPARE_EXPAND_END, timestamp));
    EXPECT_EQ(timestamp, 130U);
    EXPECT_EQ(timingRecorder.Size(), 3U);
    EXPECT_EQ(g_clockCallCount, 4U);

    uint64_t elapsed = 0U;
    EXPECT_TRUE(
        timingRecorder.GetElapsed(Recorder::Stage::KERNEL_ENTRY, Recorder::Stage::FIRST_PREPARE_EXPAND_END, elapsed));
    EXPECT_EQ(elapsed, 30U);
}

TEST_F(AicpuTimingRecorderTest, RejectsInvalidAndLateRecords)
{
    Recorder timingRecorder("test_scenario", TestClock);

    EXPECT_FALSE(timingRecorder.Record(static_cast<Recorder::Stage>(UINT8_MAX)));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
    timingRecorder.Report("complete");

    EXPECT_TRUE(timingRecorder.IsReported());
    EXPECT_FALSE(timingRecorder.Record(Recorder::Stage::LAST_FINALIZE_RECEIVED));
    EXPECT_EQ(timingRecorder.Size(), 1U);

    uint64_t value = 0U;
    EXPECT_FALSE(timingRecorder.GetTimestamp(Recorder::Stage::FIRST_PREPARE_EXPAND_END, value));
    EXPECT_FALSE(
        timingRecorder.GetElapsed(Recorder::Stage::KERNEL_ENTRY, Recorder::Stage::FIRST_PREPARE_EXPAND_END, value));
}

TEST_F(AicpuTimingRecorderTest, ResetMakesSessionReusable)
{
    Recorder timingRecorder("test_scenario", TestClock);
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
    timingRecorder.Report("complete");

    timingRecorder.Reset();
    EXPECT_FALSE(timingRecorder.IsReported());
    EXPECT_EQ(timingRecorder.Size(), 0U);
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingRecorder.GetTimestamp(Recorder::Stage::KERNEL_ENTRY, timestamp));
    EXPECT_EQ(timestamp, 110U);
}

TEST_F(AicpuTimingRecorderTest, RecordsCompleteFourPointTimeline)
{
    Recorder timingRecorder("test_scenario", TestClock);
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_ACCEPTED));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_EXPAND_END));
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::LAST_FINALIZE_RECEIVED));
    EXPECT_EQ(timingRecorder.Size(), 4U);

    uint64_t elapsedNs = 0U;
    EXPECT_TRUE(
        timingRecorder.GetElapsed(Recorder::Stage::KERNEL_ENTRY, Recorder::Stage::FIRST_PREPARE_ACCEPTED, elapsedNs));
    EXPECT_EQ(elapsedNs, 10U);
    EXPECT_TRUE(timingRecorder.GetElapsed(
        Recorder::Stage::FIRST_PREPARE_ACCEPTED, Recorder::Stage::FIRST_PREPARE_EXPAND_END, elapsedNs));
    EXPECT_EQ(elapsedNs, 10U);
    EXPECT_TRUE(timingRecorder.GetElapsed(
        Recorder::Stage::FIRST_PREPARE_EXPAND_END, Recorder::Stage::LAST_FINALIZE_RECEIVED, elapsedNs));
    EXPECT_EQ(elapsedNs, 10U);
}
} // namespace
