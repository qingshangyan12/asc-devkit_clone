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

uint32_t g_aicpuTimingRunInfoLogCount = 0U;
uint32_t g_aicpuTimingWarningLogCount = 0U;
std::string g_aicpuTimingLastRunInfo;

namespace {
using Recorder = Hccl::AicpuTimingRecorder;

uint64_t g_currentTimestamp = 0U;
uint64_t g_clockStep = 1U;
uint32_t g_clockCallCount = 0U;
int32_t g_failClockAtCall = -1;

bool TestClock(uint64_t& timestampNs)
{
    const bool shouldFail = static_cast<int32_t>(g_clockCallCount) == g_failClockAtCall;
    ++g_clockCallCount;
    if (shouldFail) {
        return false;
    }
    timestampNs = g_currentTimestamp;
    g_currentTimestamp += g_clockStep;
    return true;
}

void RecordCompleteTimeline(Recorder& recorder)
{
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::KERNEL_ENTRY, 100U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::FIRST_PREPARE_ACCEPTED, 120U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_EXPAND_END, 180U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_FINALIZE_RECEIVED, 250U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::ALL_TASK_FINISHED, 270U));
}

class AicpuTimingRecorderTest : public testing::Test {
protected:
    void SetUp() override
    {
        g_currentTimestamp = 100U;
        g_clockStep = 10U;
        g_clockCallCount = 0U;
        g_failClockAtCall = -1;
        g_aicpuTimingRunInfoLogCount = 0U;
        g_aicpuTimingWarningLogCount = 0U;
        g_aicpuTimingLastRunInfo.clear();
    }
};

TEST_F(AicpuTimingRecorderTest, KeepsFirstPointUpdatesLatestPointAndAccumulatesDurations)
{
    Recorder recorder("test_scenario", TestClock);

    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::KERNEL_ENTRY, 100U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::FIRST_PREPARE_ACCEPTED, 110U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::FIRST_PREPARE_ACCEPTED, 120U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_EXPAND_END, 140U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_EXPAND_END, 190U, Recorder::RecordPolicy::KEEP_LATEST));
    EXPECT_TRUE(recorder.Accumulate(Recorder::Metric::ORCHESTRATION, 110U, 140U));
    EXPECT_TRUE(recorder.Accumulate(Recorder::Metric::ORCHESTRATION, 150U, 190U));

    uint64_t timestampNs = 0U;
    EXPECT_TRUE(recorder.GetTimestamp(Recorder::Stage::FIRST_PREPARE_ACCEPTED, timestampNs));
    EXPECT_EQ(timestampNs, 110U);
    EXPECT_TRUE(recorder.GetTimestamp(Recorder::Stage::LAST_EXPAND_END, timestampNs));
    EXPECT_EQ(timestampNs, 190U);
    EXPECT_EQ(recorder.Size(), 3U);

    Recorder::DurationStat orchestration{};
    EXPECT_TRUE(recorder.GetDuration(Recorder::Metric::ORCHESTRATION, orchestration));
    EXPECT_EQ(orchestration.count, 2U);
    EXPECT_EQ(orchestration.totalNs, 70U);
    EXPECT_EQ(orchestration.maxNs, 40U);

    recorder.Report("incomplete");
}

TEST_F(AicpuTimingRecorderTest, ReportsCompleteTimelineInOneStructuredLog)
{
    Recorder recorder("test_scenario", TestClock);
    RecordCompleteTimeline(recorder);
    EXPECT_TRUE(recorder.Accumulate(Recorder::Metric::ORCHESTRATION, 120U, 180U));

    recorder.Report("complete", 2U);

    EXPECT_TRUE(recorder.IsReported());
    EXPECT_TRUE(recorder.IsValid());
    EXPECT_EQ(g_aicpuTimingRunInfoLogCount, 1U);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("status[complete]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("serverNum[2]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("firstPrepareAcceptedTimestampNs[120]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("lastExpandEndTimestampNs[180]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("startupNs[20]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("orchestrationWindowNs[60]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("deviceEnvelopeNs[130]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("postExpandTailNs[70]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("finalizeDrainNs[20]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("serverWindowNs[170]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("orchestrationCount[1]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("orchestrationTotalNs[60]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("orchestrationMaxNs[60]"), std::string::npos);

    recorder.Report("complete", 2U);
    EXPECT_EQ(g_aicpuTimingRunInfoLogCount, 1U);
    EXPECT_FALSE(recorder.Record(Recorder::Stage::ALL_TASK_FINISHED));
}

TEST_F(AicpuTimingRecorderTest, MarksCompleteRequestIncompleteWhenStagesAreMissing)
{
    Recorder recorder("test_scenario", TestClock);
    EXPECT_TRUE(recorder.Record(Recorder::Stage::KERNEL_ENTRY));

    recorder.Report("complete", 1U);

    EXPECT_NE(g_aicpuTimingLastRunInfo.find("status[incomplete]"), std::string::npos);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("pointCount[1]"), std::string::npos);
}

TEST_F(AicpuTimingRecorderTest, MarksCompleteRequestIncompleteWhenTimelineIsOutOfOrder)
{
    Recorder recorder("test_scenario", TestClock);
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::KERNEL_ENTRY, 100U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::FIRST_PREPARE_ACCEPTED, 120U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_EXPAND_END, 260U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_FINALIZE_RECEIVED, 250U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::ALL_TASK_FINISHED, 270U));

    recorder.Report("complete", 1U);

    EXPECT_NE(g_aicpuTimingLastRunInfo.find("status[incomplete]"), std::string::npos);
}

TEST_F(AicpuTimingRecorderTest, MarksReportInvalidAfterClockFailure)
{
    Recorder recorder("test_scenario", TestClock);
    EXPECT_TRUE(recorder.Record(Recorder::Stage::KERNEL_ENTRY));
    g_failClockAtCall = static_cast<int32_t>(g_clockCallCount);
    EXPECT_FALSE(recorder.Record(Recorder::Stage::FIRST_PREPARE_ACCEPTED));
    EXPECT_FALSE(recorder.IsValid());

    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::FIRST_PREPARE_ACCEPTED, 120U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_EXPAND_END, 180U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::LAST_FINALIZE_RECEIVED, 250U));
    EXPECT_TRUE(recorder.RecordAt(Recorder::Stage::ALL_TASK_FINISHED, 270U));
    recorder.Report("complete", 1U);

    EXPECT_NE(g_aicpuTimingLastRunInfo.find("status[invalid]"), std::string::npos);
    EXPECT_EQ(g_aicpuTimingWarningLogCount, 1U);
}

TEST_F(AicpuTimingRecorderTest, RejectsBackwardDurationAndInvalidIndexes)
{
    Recorder recorder("test_scenario", TestClock);
    EXPECT_TRUE(recorder.Record(Recorder::Stage::KERNEL_ENTRY));

    EXPECT_FALSE(recorder.Accumulate(Recorder::Metric::ORCHESTRATION, 200U, 100U));
    EXPECT_FALSE(recorder.RecordAt(static_cast<Recorder::Stage>(UINT8_MAX), 300U));
    Recorder::DurationStat stat{};
    EXPECT_FALSE(recorder.GetDuration(static_cast<Recorder::Metric>(UINT8_MAX), stat));
    EXPECT_FALSE(recorder.IsValid());

    recorder.Report("complete", 1U);
    EXPECT_NE(g_aicpuTimingLastRunInfo.find("status[invalid]"), std::string::npos);
}

TEST_F(AicpuTimingRecorderTest, ResetMakesRecorderReusable)
{
    Recorder recorder("test_scenario", TestClock);
    EXPECT_TRUE(recorder.Record(Recorder::Stage::KERNEL_ENTRY));
    recorder.Report("incomplete");

    recorder.Reset();
    EXPECT_FALSE(recorder.IsReported());
    EXPECT_TRUE(recorder.IsValid());
    EXPECT_EQ(recorder.Size(), 0U);
    EXPECT_TRUE(recorder.Record(Recorder::Stage::KERNEL_ENTRY));

    uint64_t timestampNs = 0U;
    EXPECT_TRUE(recorder.GetTimestamp(Recorder::Stage::KERNEL_ENTRY, timestampNs));
    EXPECT_EQ(timestampNs, 110U);
    recorder.Report("incomplete");
}
} // namespace
