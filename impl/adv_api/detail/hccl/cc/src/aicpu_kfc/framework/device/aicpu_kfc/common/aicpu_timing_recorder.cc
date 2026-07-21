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
constexpr char NO_PREVIOUS_STAGE[] = "none";
constexpr char NO_PHASE[] = "none";
} // namespace

constexpr std::size_t AicpuTimingRecorder::STAGE_COUNT;

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

bool AicpuTimingRecorder::Record(Stage stage, RecordPolicy policy)
{
    const std::size_t index = StageIndex(stage);
    if (index == STAGE_COUNT) {
        HCCL_WARNING("[TIMING_STAT] Reject an invalid stage in scenario[%s].", scenario_);
        return false;
    }
    if (reported_) {
        HCCL_WARNING("[TIMING_STAT] Reject stage[%s] after scenario[%s] was reported.", StageName(stage), scenario_);
        return false;
    }

    TimePoint& point = points_[index];
    if (point.recorded) {
        if (policy == RecordPolicy::KEEP_LATEST) {
            point.timestampNs = clock_();
        }
        return true;
    }

    point.timestampNs = clock_();
    point.recorded = true;
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
    HCCL_RUN_INFO("[TIMING_STAT] scenario[%s], status[%s], pointCount[%zu].", scenario_, reportStatus, pointCount_);
    for (std::size_t i = 0U; i < STAGE_COUNT; ++i) {
        if (!points_[i].recorded) {
            continue;
        }
        const Stage stage = static_cast<Stage>(i);
        const Stage previousStage = (i == 0U) ? stage : static_cast<Stage>(i - 1U);
        const char* previousStageName = (i == 0U) ? NO_PREVIOUS_STAGE : StageName(previousStage);
        const uint64_t elapsed = (i == 0U) ? 0U : GetElapsedOrZero(previousStage, stage);
        const uint64_t total = GetElapsedOrZero(Stage::KERNEL_ENTRY, stage);
        HCCL_RUN_INFO(
            "[TIMING_STAT][%zu] scenario[%s], previousStage[%s], stage[%s], phase[%s], timestampNs[%llu], "
            "elapsedNs[%llu], totalNs[%llu].",
            i, scenario_, previousStageName, StageName(stage), PhaseName(stage),
            static_cast<unsigned long long>(points_[i].timestampNs), static_cast<unsigned long long>(elapsed),
            static_cast<unsigned long long>(total));
    }
}

void AicpuTimingRecorder::Reset()
{
    points_ = {};
    pointCount_ = 0U;
    reported_ = false;
}

std::size_t AicpuTimingRecorder::Size() const { return pointCount_; }

bool AicpuTimingRecorder::IsReported() const { return reported_; }

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
    uint64_t beginTimestamp = 0U;
    uint64_t endTimestamp = 0U;
    if (!GetTimestamp(beginStage, beginTimestamp) || !GetTimestamp(endStage, endTimestamp) ||
        endTimestamp < beginTimestamp) {
        return false;
    }
    elapsedNs = endTimestamp - beginTimestamp;
    return true;
}

std::size_t AicpuTimingRecorder::StageIndex(Stage stage)
{
    const std::size_t index = static_cast<std::size_t>(stage);
    return index < STAGE_COUNT ? index : STAGE_COUNT;
}

const char* AicpuTimingRecorder::StageName(Stage stage)
{
    switch (stage) {
        case Stage::KERNEL_ENTRY:
            return "kernel_entry";
        case Stage::FIRST_PREPARE_ACCEPTED:
            return "first_prepare_accepted";
        case Stage::FIRST_PREPARE_EXPAND_END:
            return "first_prepare_expand_end";
        case Stage::LAST_FINALIZE_RECEIVED:
            return "last_finalize_received";
        default:
            return UNKNOWN_NAME;
    }
}

const char* AicpuTimingRecorder::PhaseName(Stage stage)
{
    switch (stage) {
        case Stage::KERNEL_ENTRY:
            return NO_PHASE;
        case Stage::FIRST_PREPARE_ACCEPTED:
            return "head_overhead";
        case Stage::FIRST_PREPARE_EXPAND_END:
            return "single_prepare_expand";
        case Stage::LAST_FINALIZE_RECEIVED:
            return "communication_and_tail";
        default:
            return UNKNOWN_NAME;
    }
}

uint64_t AicpuTimingRecorder::GetElapsedOrZero(Stage beginStage, Stage endStage) const
{
    uint64_t elapsedNs = 0U;
    (void)GetElapsed(beginStage, endStage, elapsedNs);
    return elapsedNs;
}
} // namespace Hccl
