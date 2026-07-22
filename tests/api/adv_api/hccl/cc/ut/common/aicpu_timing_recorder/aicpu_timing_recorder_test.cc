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

#include <cstdlib>
#include <cstdint>

#include "gtest/gtest.h"
#include "log.h"

namespace {
using Recorder = Hccl::AicpuTimingRecorder;

constexpr char TIMING_LEVEL_ENV[] = "HCCL_AICPU_TIMING_LOG_ENABLE";
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
        AicpuTimingRecorderTestLogCount() = 0U;
        ASSERT_EQ(setenv(TIMING_LEVEL_ENV, "4", 1), 0);
    }

    void TearDown() override
    {
        (void)unsetenv(TIMING_LEVEL_ENV);
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
    EXPECT_FALSE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 10U));
    EXPECT_FALSE(timingRecorder.IncrementCounter(Recorder::Counter::PREPARE));
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
    EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 20U));
    EXPECT_TRUE(timingRecorder.IncrementCounter(Recorder::Counter::PREPARE, 2U));
    timingRecorder.Report("complete");

    timingRecorder.Reset();
    EXPECT_FALSE(timingRecorder.IsReported());
    EXPECT_EQ(timingRecorder.Size(), 0U);
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));

    Recorder::MetricStats stats{};
    EXPECT_FALSE(timingRecorder.GetMetric(Recorder::Metric::PARAM_FORMAT, stats));
    uint64_t counter = 1U;
    EXPECT_TRUE(timingRecorder.GetCounter(Recorder::Counter::PREPARE, counter));
    EXPECT_EQ(counter, 0U);

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingRecorder.GetTimestamp(Recorder::Stage::KERNEL_ENTRY, timestamp));
    EXPECT_EQ(timestamp, 110U);
}

TEST_F(AicpuTimingRecorderTest, RecordsExplicitTimestampWithoutReadingClock)
{
    Recorder timingRecorder("test_scenario", TestClock);

    EXPECT_TRUE(timingRecorder.RecordAt(Recorder::Stage::KERNEL_ENTRY, 200U));
    EXPECT_TRUE(timingRecorder.RecordAt(Recorder::Stage::KERNEL_ENTRY, 300U));
    EXPECT_TRUE(timingRecorder.RecordAt(Recorder::Stage::KERNEL_ENTRY, 400U, Recorder::RecordPolicy::KEEP_LATEST));

    uint64_t timestamp = 0U;
    EXPECT_TRUE(timingRecorder.GetTimestamp(Recorder::Stage::KERNEL_ENTRY, timestamp));
    EXPECT_EQ(timestamp, 400U);
    EXPECT_EQ(g_clockCallCount, 0U);
}

TEST_F(AicpuTimingRecorderTest, AggregatesMetricsAndCounters)
{
    Recorder timingRecorder("test_scenario", TestClock);
    EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));

    EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 30U));
    EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 10U));
    EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 20U));
    Recorder::MetricStats stats{};
    EXPECT_TRUE(timingRecorder.GetMetric(Recorder::Metric::PARAM_FORMAT, stats));
    EXPECT_EQ(stats.count, 3U);
    EXPECT_EQ(stats.totalNs, 60U);
    EXPECT_EQ(stats.minNs, 10U);
    EXPECT_EQ(stats.maxNs, 30U);
    EXPECT_FALSE(timingRecorder.GetMetric(Recorder::Metric::WAIT_SQE_BUILD, stats));
    EXPECT_FALSE(timingRecorder.AddMetric(static_cast<Recorder::Metric>(UINT8_MAX), 1U));

    EXPECT_TRUE(timingRecorder.IncrementCounter(Recorder::Counter::EMPTY_POLL));
    EXPECT_TRUE(timingRecorder.IncrementCounter(Recorder::Counter::EMPTY_POLL, 4U));
    uint64_t counter = 0U;
    EXPECT_TRUE(timingRecorder.GetCounter(Recorder::Counter::EMPTY_POLL, counter));
    EXPECT_EQ(counter, 5U);
    EXPECT_FALSE(timingRecorder.GetCounter(static_cast<Recorder::Counter>(UINT8_MAX), counter));
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

TEST_F(AicpuTimingRecorderTest, DisablesAllTimingForMissingZeroAndInvalidLevels)
{
    const char* levels[] = {nullptr, "0", "", "5", "10", "invalid"};
    for (const char* level : levels) {
        SCOPED_TRACE(level == nullptr ? "unset" : level);
        if (level == nullptr) {
            ASSERT_EQ(unsetenv(TIMING_LEVEL_ENV), 0);
        } else {
            ASSERT_EQ(setenv(TIMING_LEVEL_ENV, level, 1), 0);
        }
        g_clockCallCount = 0U;
        AicpuTimingRecorderTestLogCount() = 0U;

        Recorder timingRecorder("disabled", TestClock);
        EXPECT_FALSE(timingRecorder.IsEnabled());
        EXPECT_FALSE(timingRecorder.IsFullEnabled());
        EXPECT_FALSE(timingRecorder.IsLogEnabled());
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
        EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::SUBMIT_TO_COMPLETION_OBSERVED, 10U));
        EXPECT_TRUE(timingRecorder.IncrementCounter(Recorder::Counter::PREPARE));
        EXPECT_EQ(timingRecorder.Size(), 0U);
        EXPECT_EQ(g_clockCallCount, 0U);

        Recorder::MetricStats stats{};
        EXPECT_FALSE(timingRecorder.GetMetric(Recorder::Metric::SUBMIT_TO_COMPLETION_OBSERVED, stats));
        uint64_t counter = 1U;
        EXPECT_TRUE(timingRecorder.GetCounter(Recorder::Counter::PREPARE, counter));
        EXPECT_EQ(counter, 0U);

        timingRecorder.Report("complete");
        EXPECT_FALSE(timingRecorder.IsReported());
        EXPECT_EQ(AicpuTimingRecorderTestLogCount(), 0U);
    }
}

TEST_F(AicpuTimingRecorderTest, RecordsOnlyBasicTimingAtLevelsOneAndTwo)
{
    const char* levels[] = {"1", "2"};
    for (const char* level : levels) {
        SCOPED_TRACE(level);
        ASSERT_EQ(setenv(TIMING_LEVEL_ENV, level, 1), 0);
        g_currentTimestamp = 100U;
        g_clockCallCount = 0U;
        AicpuTimingRecorderTestLogCount() = 0U;

        Recorder timingRecorder("basic", TestClock);
        EXPECT_TRUE(timingRecorder.IsEnabled());
        EXPECT_FALSE(timingRecorder.IsFullEnabled());
        EXPECT_EQ(timingRecorder.IsLogEnabled(), level[0] == '2');

        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ARGS_PARSED));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_ACCEPTED));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_PREPARE_EXPAND_END));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::LAST_PREPARE_EXPAND_END));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::FIRST_COMM_TURN_FINISHED_OBSERVED));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::ALL_COMMUNICATION_FINISHED_OBSERVED));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::LAST_FINALIZE_RECEIVED));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::ALL_SERVERS_FINISHED));
        EXPECT_EQ(timingRecorder.Size(), 8U);
        EXPECT_EQ(g_clockCallCount, 8U);

        EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 20U));
        EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::SUBMIT_TO_COMPLETION_OBSERVED, 30U));
        Recorder::MetricStats stats{};
        EXPECT_FALSE(timingRecorder.GetMetric(Recorder::Metric::PARAM_FORMAT, stats));
        EXPECT_TRUE(timingRecorder.GetMetric(Recorder::Metric::SUBMIT_TO_COMPLETION_OBSERVED, stats));
        EXPECT_EQ(stats.totalNs, 30U);

        EXPECT_TRUE(timingRecorder.IncrementCounter(Recorder::Counter::PREPARE));
        uint64_t counter = 1U;
        EXPECT_TRUE(timingRecorder.GetCounter(Recorder::Counter::PREPARE, counter));
        EXPECT_EQ(counter, 0U);

        timingRecorder.Report("complete");
        EXPECT_TRUE(timingRecorder.IsReported());
        if (level[0] == '1') {
            EXPECT_EQ(AicpuTimingRecorderTestLogCount(), 0U);
        } else {
            EXPECT_GT(AicpuTimingRecorderTestLogCount(), 0U);
        }
    }
}

TEST_F(AicpuTimingRecorderTest, RecordsFullTimingAtLevelsThreeAndFour)
{
    const char* levels[] = {"3", "4"};
    for (const char* level : levels) {
        SCOPED_TRACE(level);
        ASSERT_EQ(setenv(TIMING_LEVEL_ENV, level, 1), 0);
        g_currentTimestamp = 100U;
        g_clockCallCount = 0U;
        AicpuTimingRecorderTestLogCount() = 0U;

        Recorder timingRecorder("full", TestClock);
        EXPECT_TRUE(timingRecorder.IsEnabled());
        EXPECT_TRUE(timingRecorder.IsFullEnabled());
        EXPECT_EQ(timingRecorder.IsLogEnabled(), level[0] == '4');

        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ENTRY));
        EXPECT_TRUE(timingRecorder.Record(Recorder::Stage::KERNEL_ARGS_PARSED));
        EXPECT_EQ(timingRecorder.Size(), 2U);
        EXPECT_EQ(g_clockCallCount, 2U);

        EXPECT_TRUE(timingRecorder.AddMetric(Recorder::Metric::PARAM_FORMAT, 20U));
        Recorder::MetricStats stats{};
        EXPECT_TRUE(timingRecorder.GetMetric(Recorder::Metric::PARAM_FORMAT, stats));
        EXPECT_EQ(stats.totalNs, 20U);

        EXPECT_TRUE(timingRecorder.IncrementCounter(Recorder::Counter::PREPARE, 2U));
        uint64_t counter = 0U;
        EXPECT_TRUE(timingRecorder.GetCounter(Recorder::Counter::PREPARE, counter));
        EXPECT_EQ(counter, 2U);

        timingRecorder.Report("complete");
        EXPECT_TRUE(timingRecorder.IsReported());
        if (level[0] == '3') {
            EXPECT_EQ(AicpuTimingRecorderTestLogCount(), 0U);
        } else {
            EXPECT_GT(AicpuTimingRecorderTestLogCount(), 0U);
        }
    }
}
} // namespace
