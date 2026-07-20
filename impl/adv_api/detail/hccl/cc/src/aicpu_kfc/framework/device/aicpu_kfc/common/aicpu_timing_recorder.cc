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

#include <ctime>

#include "log.h"

namespace Hccl {
namespace {
constexpr uint64_t NANOSECONDS_PER_SECOND = 1000000000ULL;
constexpr char UNKNOWN_NAME[] = "unknown";
constexpr char INCOMPLETE_STATUS[] = "incomplete";
} // namespace

constexpr std::size_t AicpuTimingRecorder::CAPACITY;

uint64_t AicpuTimingRecorder::MonotonicNow()
{
    struct timespec timestamp{};
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &timestamp) != 0) {
        HCCL_WARNING("[TIMING_STAT] Failed to read CLOCK_MONOTONIC_RAW.");
        return 0U;
    }
    return static_cast<uint64_t>(timestamp.tv_sec) * NANOSECONDS_PER_SECOND + static_cast<uint64_t>(timestamp.tv_nsec);
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

bool AicpuTimingRecorder::Record(const char* stage, RecordPolicy policy)
{
    if (stage == nullptr || stage[0] == '\0') {
        HCCL_WARNING("[TIMING_STAT] Reject an empty stage in scenario[%s].", scenario_.c_str());
        return false;
    }
    if (reported_) {
        HCCL_WARNING("[TIMING_STAT] Reject stage[%s] after scenario[%s] was reported.", stage, scenario_.c_str());
        return false;
    }

    const std::size_t index = FindPoint(stage);
    if (index != CAPACITY) {
        if (policy == RecordPolicy::KEEP_LATEST) {
            points_[index].timestampNs = clock_();
        }
        return true;
    }
    if (pointCount_ == CAPACITY) {
        HCCL_WARNING("[TIMING_STAT] Scenario[%s] exceeds point capacity[%zu].", scenario_.c_str(), CAPACITY);
        return false;
    }

    points_[pointCount_].stage = stage;
    points_[pointCount_].timestampNs = clock_();
    ++pointCount_;
    return true;
}

void AicpuTimingRecorder::Report(const char* status)
{
    if (reported_ || pointCount_ == 0U) {
        return;
    }
    reported_ = true;

    const char* reportStatus = (status == nullptr || status[0] == '\0') ? UNKNOWN_NAME : status;
    const uint64_t firstTimestamp = points_[0U].timestampNs;
    HCCL_RUN_INFO(
        "[TIMING_STAT] scenario[%s], status[%s], pointCount[%zu].", scenario_.c_str(), reportStatus, pointCount_);
    for (std::size_t i = 0U; i < pointCount_; ++i) {
        const uint64_t timestamp = points_[i].timestampNs;
        const uint64_t previousTimestamp = (i == 0U) ? timestamp : points_[i - 1U].timestampNs;
        const uint64_t delta = (timestamp >= previousTimestamp) ? timestamp - previousTimestamp : 0U;
        const uint64_t total = (timestamp >= firstTimestamp) ? timestamp - firstTimestamp : 0U;
        HCCL_RUN_INFO(
            "[TIMING_STAT][%zu] scenario[%s], stage[%s], timestampNs[%llu], deltaNs[%llu], totalNs[%llu].", i,
            scenario_.c_str(), points_[i].stage.c_str(), static_cast<unsigned long long>(timestamp),
            static_cast<unsigned long long>(delta), static_cast<unsigned long long>(total));
    }
}

void AicpuTimingRecorder::Reset()
{
    for (std::size_t i = 0U; i < pointCount_; ++i) {
        points_[i].stage.clear();
        points_[i].timestampNs = 0U;
    }
    pointCount_ = 0U;
    reported_ = false;
}

std::size_t AicpuTimingRecorder::Size() const { return pointCount_; }

bool AicpuTimingRecorder::IsReported() const { return reported_; }

bool AicpuTimingRecorder::GetTimestamp(const char* stage, uint64_t& timestampNs) const
{
    if (stage == nullptr || stage[0] == '\0') {
        return false;
    }
    const std::size_t index = FindPoint(stage);
    if (index == CAPACITY) {
        return false;
    }
    timestampNs = points_[index].timestampNs;
    return true;
}

bool AicpuTimingRecorder::GetElapsed(const char* beginStage, const char* endStage, uint64_t& elapsedNs) const
{
    uint64_t beginTimestamp = 0U;
    uint64_t endTimestamp = 0U;
    if (!GetTimestamp(beginStage, beginTimestamp) || !GetTimestamp(endStage, endTimestamp) ||
        endTimestamp < beginTimestamp) {
        return false;
    }
    elapsedNs = endTimestamp - beginTimestamp;
    return true;
}

std::size_t AicpuTimingRecorder::FindPoint(const char* stage) const
{
    for (std::size_t i = 0U; i < pointCount_; ++i) {
        if (points_[i].stage == stage) {
            return i;
        }
    }
    return CAPACITY;
}
} // namespace Hccl
