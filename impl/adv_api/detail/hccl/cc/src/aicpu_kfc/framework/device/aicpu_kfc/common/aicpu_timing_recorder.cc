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

#include <cstring>
#include <ctime>
#include <limits>

#include "log.h"

namespace Hccl {
namespace {
constexpr uint64_t NANOSECONDS_PER_SECOND = 1000000000ULL;
constexpr char UNKNOWN_NAME[] = "unknown";
constexpr char COMPLETE_STATUS[] = "complete";
constexpr char INCOMPLETE_STATUS[] = "incomplete";
constexpr char INVALID_STATUS[] = "invalid";
} // namespace

constexpr std::size_t AicpuTimingRecorder::STAGE_COUNT;
constexpr std::size_t AicpuTimingRecorder::METRIC_COUNT;

bool AicpuTimingRecorder::MonotonicNow(uint64_t& timestampNs)
{
    struct timespec timestamp{};
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &timestamp) != 0) {
        return false;
    }
    timestampNs =
        static_cast<uint64_t>(timestamp.tv_sec) * NANOSECONDS_PER_SECOND + static_cast<uint64_t>(timestamp.tv_nsec);
    return true;
}

AicpuTimingRecorder::AicpuTimingRecorder(const char* scenario, Clock clock)
    : scenario_((scenario == nullptr || scenario[0] == '\0') ? UNKNOWN_NAME : scenario),
      clock_(clock == nullptr ? MonotonicNow : clock)
{}

AicpuTimingRecorder::~AicpuTimingRecorder()
{
    if (!reported_ && pointCount_ != 0U) {
        Report(INCOMPLETE_STATUS);
    }
}

bool AicpuTimingRecorder::CaptureTimestamp(uint64_t& timestampNs)
{
    if (reported_) {
        HCCL_WARNING("[TIMING_STAT] Reject timestamp capture after scenario[%s] was reported.", scenario_);
        return false;
    }
    if (!clock_(timestampNs)) {
        valid_ = false;
        HCCL_WARNING("[TIMING_STAT] Failed to read monotonic clock in scenario[%s].", scenario_);
        return false;
    }
    return true;
}

bool AicpuTimingRecorder::Record(Stage stage, RecordPolicy policy)
{
    uint64_t timestampNs = 0U;
    if (!CaptureTimestamp(timestampNs)) {
        return false;
    }
    return RecordAt(stage, timestampNs, policy);
}

bool AicpuTimingRecorder::RecordAt(Stage stage, uint64_t timestampNs, RecordPolicy policy)
{
    const std::size_t index = StageIndex(stage);
    if (index == STAGE_COUNT) {
        valid_ = false;
        HCCL_WARNING("[TIMING_STAT] Reject invalid stage in scenario[%s].", scenario_);
        return false;
    }
    if (reported_) {
        HCCL_WARNING("[TIMING_STAT] Reject stage[%zu] after scenario[%s] was reported.", index, scenario_);
        return false;
    }

    TimePoint& point = points_[index];
    if (point.recorded) {
        if (policy == RecordPolicy::KEEP_LATEST) {
            point.timestampNs = timestampNs;
        }
        return true;
    }

    point.timestampNs = timestampNs;
    point.recorded = true;
    ++pointCount_;
    return true;
}

bool AicpuTimingRecorder::Accumulate(Metric metric, uint64_t beginTimestampNs, uint64_t endTimestampNs)
{
    const std::size_t index = MetricIndex(metric);
    if (index == METRIC_COUNT || endTimestampNs < beginTimestampNs || reported_) {
        valid_ = false;
        HCCL_WARNING("[TIMING_STAT] Reject invalid duration in scenario[%s].", scenario_);
        return false;
    }

    const uint64_t elapsedNs = endTimestampNs - beginTimestampNs;
    DurationStat& stat = durations_[index];
    if (std::numeric_limits<uint64_t>::max() - stat.totalNs < elapsedNs ||
        stat.count == std::numeric_limits<uint32_t>::max()) {
        valid_ = false;
        HCCL_WARNING("[TIMING_STAT] Duration overflow in scenario[%s].", scenario_);
        return false;
    }

    stat.totalNs += elapsedNs;
    stat.maxNs = (elapsedNs > stat.maxNs) ? elapsedNs : stat.maxNs;
    ++stat.count;
    return true;
}

void AicpuTimingRecorder::Report(const char* status, uint32_t serverNum)
{
    if (reported_ || pointCount_ == 0U) {
        return;
    }
    reported_ = true;

    const bool requestedComplete = status != nullptr && std::strcmp(status, COMPLETE_STATUS) == 0;
    const char* reportStatus = (status == nullptr || status[0] == '\0') ? UNKNOWN_NAME : status;
    if (!valid_) {
        reportStatus = INVALID_STATUS;
    } else if (requestedComplete && !HasCompleteTimeline()) {
        reportStatus = INCOMPLETE_STATUS;
    }

    const DurationStat& orchestration = durations_[MetricIndex(Metric::ORCHESTRATION)];
    HCCL_RUN_INFO(
        "[TIMING_STAT] scenario[%s], status[%s], serverNum[%u], pointCount[%zu], "
        "kernelEntryTimestampNs[%llu], firstPrepareAcceptedTimestampNs[%llu], lastExpandEndTimestampNs[%llu], "
        "lastFinalizeReceivedTimestampNs[%llu], allTaskFinishedTimestampNs[%llu], startupNs[%llu], "
        "orchestrationWindowNs[%llu], deviceEnvelopeNs[%llu], postExpandTailNs[%llu], finalizeDrainNs[%llu], "
        "serverWindowNs[%llu], orchestrationCount[%u], orchestrationTotalNs[%llu], orchestrationMaxNs[%llu].",
        scenario_, reportStatus, serverNum, pointCount_,
        static_cast<unsigned long long>(GetTimestampOrZero(Stage::KERNEL_ENTRY)),
        static_cast<unsigned long long>(GetTimestampOrZero(Stage::FIRST_PREPARE_ACCEPTED)),
        static_cast<unsigned long long>(GetTimestampOrZero(Stage::LAST_EXPAND_END)),
        static_cast<unsigned long long>(GetTimestampOrZero(Stage::LAST_FINALIZE_RECEIVED)),
        static_cast<unsigned long long>(GetTimestampOrZero(Stage::ALL_TASK_FINISHED)),
        static_cast<unsigned long long>(GetElapsedOrZero(Stage::KERNEL_ENTRY, Stage::FIRST_PREPARE_ACCEPTED)),
        static_cast<unsigned long long>(GetElapsedOrZero(Stage::FIRST_PREPARE_ACCEPTED, Stage::LAST_EXPAND_END)),
        static_cast<unsigned long long>(GetElapsedOrZero(Stage::FIRST_PREPARE_ACCEPTED, Stage::LAST_FINALIZE_RECEIVED)),
        static_cast<unsigned long long>(GetElapsedOrZero(Stage::LAST_EXPAND_END, Stage::LAST_FINALIZE_RECEIVED)),
        static_cast<unsigned long long>(GetElapsedOrZero(Stage::LAST_FINALIZE_RECEIVED, Stage::ALL_TASK_FINISHED)),
        static_cast<unsigned long long>(GetElapsedOrZero(Stage::KERNEL_ENTRY, Stage::ALL_TASK_FINISHED)),
        orchestration.count, static_cast<unsigned long long>(orchestration.totalNs),
        static_cast<unsigned long long>(orchestration.maxNs));
}

void AicpuTimingRecorder::Reset()
{
    points_ = {};
    durations_ = {};
    pointCount_ = 0U;
    valid_ = true;
    reported_ = false;
}

std::size_t AicpuTimingRecorder::Size() const { return pointCount_; }

bool AicpuTimingRecorder::IsReported() const { return reported_; }

bool AicpuTimingRecorder::IsValid() const { return valid_; }

bool AicpuTimingRecorder::GetTimestamp(Stage stage, uint64_t& timestampNs) const
{
    const std::size_t index = StageIndex(stage);
    if (index == STAGE_COUNT || !points_[index].recorded) {
        return false;
    }
    timestampNs = points_[index].timestampNs;
    return true;
}

bool AicpuTimingRecorder::GetElapsed(Stage beginStage, Stage endStage, uint64_t& elapsedNs) const
{
    uint64_t beginTimestampNs = 0U;
    uint64_t endTimestampNs = 0U;
    if (!GetTimestamp(beginStage, beginTimestampNs) || !GetTimestamp(endStage, endTimestampNs) ||
        endTimestampNs < beginTimestampNs) {
        return false;
    }
    elapsedNs = endTimestampNs - beginTimestampNs;
    return true;
}

bool AicpuTimingRecorder::GetDuration(Metric metric, DurationStat& stat) const
{
    const std::size_t index = MetricIndex(metric);
    if (index == METRIC_COUNT) {
        return false;
    }
    stat = durations_[index];
    return true;
}

std::size_t AicpuTimingRecorder::StageIndex(Stage stage)
{
    const std::size_t index = static_cast<std::size_t>(stage);
    return index < STAGE_COUNT ? index : STAGE_COUNT;
}

std::size_t AicpuTimingRecorder::MetricIndex(Metric metric)
{
    const std::size_t index = static_cast<std::size_t>(metric);
    return index < METRIC_COUNT ? index : METRIC_COUNT;
}

bool AicpuTimingRecorder::HasCompleteTimeline() const
{
    if (pointCount_ != STAGE_COUNT) {
        return false;
    }

    uint64_t previousTimestampNs = points_[0U].timestampNs;
    for (std::size_t i = 1U; i < STAGE_COUNT; ++i) {
        if (points_[i].timestampNs < previousTimestampNs) {
            return false;
        }
        previousTimestampNs = points_[i].timestampNs;
    }
    return true;
}

uint64_t AicpuTimingRecorder::GetTimestampOrZero(Stage stage) const
{
    uint64_t timestampNs = 0U;
    (void)GetTimestamp(stage, timestampNs);
    return timestampNs;
}

uint64_t AicpuTimingRecorder::GetElapsedOrZero(Stage beginStage, Stage endStage) const
{
    uint64_t elapsedNs = 0U;
    (void)GetElapsed(beginStage, endStage, elapsedNs);
    return elapsedNs;
}
} // namespace Hccl
