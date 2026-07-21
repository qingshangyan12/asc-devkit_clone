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

struct PhaseRange {
    const char* name;
    AicpuTimingRecorder::Stage begin;
    AicpuTimingRecorder::Stage end;
};

const PhaseRange PHASE_RANGES[] = {
    {"head_overhead", AicpuTimingRecorder::Stage::KERNEL_ENTRY, AicpuTimingRecorder::Stage::FIRST_PREPARE_ACCEPTED},
    {"first_prepare_expand", AicpuTimingRecorder::Stage::FIRST_PREPARE_ACCEPTED,
     AicpuTimingRecorder::Stage::FIRST_PREPARE_EXPAND_END},
    {"communication_and_tail", AicpuTimingRecorder::Stage::FIRST_PREPARE_EXPAND_END,
     AicpuTimingRecorder::Stage::LAST_FINALIZE_RECEIVED},
    {"first_expand_end_to_all_communication_finished_observed", AicpuTimingRecorder::Stage::FIRST_PREPARE_EXPAND_END,
     AicpuTimingRecorder::Stage::ALL_COMMUNICATION_FINISHED_OBSERVED},
    {"post_communication_tail", AicpuTimingRecorder::Stage::ALL_COMMUNICATION_FINISHED_OBSERVED,
     AicpuTimingRecorder::Stage::LAST_FINALIZE_RECEIVED},
    {"kernel_total", AicpuTimingRecorder::Stage::KERNEL_ENTRY, AicpuTimingRecorder::Stage::ALL_SERVERS_FINISHED},
};
} // namespace

constexpr std::size_t AicpuTimingRecorder::STAGE_COUNT;
constexpr std::size_t AicpuTimingRecorder::METRIC_COUNT;
constexpr std::size_t AicpuTimingRecorder::COUNTER_COUNT;

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

uint64_t AicpuTimingRecorder::Now() const { return clock_(); }

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

    if (points_[index].recorded && policy == RecordPolicy::KEEP_FIRST) {
        return true;
    }
    return RecordAt(stage, Now(), policy);
}

bool AicpuTimingRecorder::RecordAt(Stage stage, uint64_t timestampNs, RecordPolicy policy)
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
    if (point.recorded && policy == RecordPolicy::KEEP_FIRST) {
        return true;
    }
    if (!point.recorded) {
        point.recorded = true;
        ++pointCount_;
    }
    point.timestampNs = timestampNs;
    return true;
}

bool AicpuTimingRecorder::AddMetric(Metric metric, uint64_t elapsedNs)
{
    const std::size_t index = MetricIndex(metric);
    if (index == METRIC_COUNT || reported_) {
        return false;
    }

    MetricStats& stats = metrics_[index];
    if (stats.count == 0U) {
        stats.minNs = elapsedNs;
        stats.maxNs = elapsedNs;
    } else {
        stats.minNs = elapsedNs < stats.minNs ? elapsedNs : stats.minNs;
        stats.maxNs = elapsedNs > stats.maxNs ? elapsedNs : stats.maxNs;
    }
    ++stats.count;
    stats.totalNs += elapsedNs;
    return true;
}

bool AicpuTimingRecorder::IncrementCounter(Counter counter, uint64_t amount)
{
    const std::size_t index = CounterIndex(counter);
    if (index == COUNTER_COUNT || reported_) {
        return false;
    }
    counters_[index] += amount;
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
    Stage previousStage = Stage::COUNT;
    std::size_t outputIndex = 0U;
    for (std::size_t i = 0U; i < STAGE_COUNT; ++i) {
        if (!points_[i].recorded) {
            continue;
        }
        const Stage stage = static_cast<Stage>(i);
        uint64_t elapsedNs = 0U;
        const bool hasPrevious = previousStage != Stage::COUNT;
        const bool elapsedValid = !hasPrevious || GetElapsed(previousStage, stage, elapsedNs);
        uint64_t fromKernelEntryNs = 0U;
        const bool totalValid = GetElapsed(Stage::KERNEL_ENTRY, stage, fromKernelEntryNs);
        HCCL_RUN_INFO(
            "[TIMING_STAT][STAGE][%zu] scenario[%s], previousRecordedStage[%s], stage[%s], "
            "monotonicTimestampNs[%llu], elapsedFromPreviousNs[%llu], elapsedFromPreviousValid[%u], "
            "elapsedFromKernelEntryNs[%llu], elapsedFromKernelEntryValid[%u].",
            outputIndex, scenario_, hasPrevious ? StageName(previousStage) : NO_PREVIOUS_STAGE, StageName(stage),
            static_cast<unsigned long long>(points_[i].timestampNs), static_cast<unsigned long long>(elapsedNs),
            static_cast<unsigned int>(elapsedValid), static_cast<unsigned long long>(fromKernelEntryNs),
            static_cast<unsigned int>(totalValid));
        previousStage = stage;
        ++outputIndex;
    }

    for (const auto& phase : PHASE_RANGES) {
        uint64_t elapsedNs = 0U;
        if (!GetElapsed(phase.begin, phase.end, elapsedNs)) {
            continue;
        }
        HCCL_RUN_INFO(
            "[TIMING_STAT][PHASE] scenario[%s], phase[%s], beginStage[%s], endStage[%s], elapsedNs[%llu].", scenario_,
            phase.name, StageName(phase.begin), StageName(phase.end), static_cast<unsigned long long>(elapsedNs));
    }

    for (std::size_t i = 0U; i < METRIC_COUNT; ++i) {
        const MetricStats& stats = metrics_[i];
        if (stats.count == 0U) {
            continue;
        }
        HCCL_RUN_INFO(
            "[TIMING_STAT][METRIC][%zu] scenario[%s], metric[%s], count[%llu], totalNs[%llu], "
            "averageNs[%llu], minNs[%llu], maxNs[%llu].",
            i, scenario_, MetricName(static_cast<Metric>(i)), static_cast<unsigned long long>(stats.count),
            static_cast<unsigned long long>(stats.totalNs),
            static_cast<unsigned long long>(stats.totalNs / stats.count), static_cast<unsigned long long>(stats.minNs),
            static_cast<unsigned long long>(stats.maxNs));
    }

    for (std::size_t i = 0U; i < COUNTER_COUNT; ++i) {
        HCCL_RUN_INFO(
            "[TIMING_STAT][COUNTER][%zu] scenario[%s], counter[%s], value[%llu].", i, scenario_,
            CounterName(static_cast<Counter>(i)), static_cast<unsigned long long>(counters_[i]));
    }
}

void AicpuTimingRecorder::Reset()
{
    points_ = {};
    metrics_ = {};
    counters_ = {};
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

bool AicpuTimingRecorder::GetMetric(Metric metric, MetricStats& stats) const
{
    const std::size_t index = MetricIndex(metric);
    if (index == METRIC_COUNT || metrics_[index].count == 0U) {
        return false;
    }
    stats = metrics_[index];
    return true;
}

bool AicpuTimingRecorder::GetCounter(Counter counter, uint64_t& value) const
{
    const std::size_t index = CounterIndex(counter);
    if (index == COUNTER_COUNT) {
        return false;
    }
    value = counters_[index];
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

std::size_t AicpuTimingRecorder::CounterIndex(Counter counter)
{
    const std::size_t index = static_cast<std::size_t>(counter);
    return index < COUNTER_COUNT ? index : COUNTER_COUNT;
}

const char* AicpuTimingRecorder::StageName(Stage stage)
{
    switch (stage) {
        case Stage::KERNEL_ENTRY:
            return "kernel_entry";
        case Stage::KERNEL_ARGS_PARSED:
            return "kernel_args_parsed";
        case Stage::DISPATCHER_ENTER:
            return "dispatcher_enter";
        case Stage::SERVER_CONTEXT_READY:
            return "server_context_ready";
        case Stage::FIRST_PREPARE_RECEIVED:
            return "first_prepare_received";
        case Stage::FIRST_PREPARE_ACCEPTED:
            return "first_prepare_accepted";
        case Stage::FIRST_PREPARE_EXPAND_END:
            return "first_prepare_expand_end";
        case Stage::LAST_PREPARE_EXPAND_END:
            return "last_prepare_expand_end";
        case Stage::FIRST_COMM_TURN_FINISHED_OBSERVED:
            return "first_comm_turn_finished_observed";
        case Stage::ALL_COMMUNICATION_FINISHED_OBSERVED:
            return "all_communication_finished_observed";
        case Stage::FIRST_FINALIZE_RECEIVED:
            return "first_finalize_received";
        case Stage::LAST_FINALIZE_RECEIVED:
            return "last_finalize_received";
        case Stage::LAST_FINALIZE_PROCESSED:
            return "last_finalize_processed";
        case Stage::ALL_SERVERS_FINISHED:
            return "all_servers_finished";
        case Stage::REPORT_BEGIN:
            return "report_begin";
        default:
            return UNKNOWN_NAME;
    }
}

const char* AicpuTimingRecorder::MetricName(Metric metric)
{
    switch (metric) {
        case Metric::ORCHESTRATE_PRECHECK:
            return "orchestrate_precheck";
        case Metric::PARAM_FORMAT:
            return "param_format";
        case Metric::WAIT_SQE_BUILD:
            return "wait_sqe_build";
        case Metric::WAIT_SQE_SUBMIT:
            return "wait_sqe_submit";
        case Metric::ALGORITHM_EXPAND:
            return "algorithm_expand";
        case Metric::RESOURCE_DESERIALIZE:
            return "resource_deserialize";
        case Metric::VARIABLE_DATA_RESTORE:
            return "variable_data_restore";
        case Metric::EXECUTOR_RESOLVE:
            return "executor_resolve";
        case Metric::EXECUTOR_ORCHESTRATE:
            return "executor_orchestrate";
        case Metric::POST_SQE_BUILD:
            return "post_sqe_build";
        case Metric::POST_SQE_SUBMIT:
            return "post_sqe_submit";
        case Metric::PREPARE_BOOKKEEPING:
            return "prepare_bookkeeping";
        case Metric::SUBMIT_TO_COMPLETION_OBSERVED:
            return "submit_to_completion_observed";
        default:
            return UNKNOWN_NAME;
    }
}

const char* AicpuTimingRecorder::CounterName(Counter counter)
{
    switch (counter) {
        case Counter::EMPTY_POLL:
            return "empty_poll";
        case Counter::SEQ_RETRY:
            return "seq_retry";
        case Counter::INTER_GROUP_SYNC_RETRY:
            return "inter_group_sync_retry";
        case Counter::PREPARE:
            return "prepare";
        case Counter::FINALIZE:
            return "finalize";
        case Counter::COMM_OVERLAP_EXPAND:
            return "communication_overlap_expand";
        case Counter::FINALIZE_BEFORE_COMM_FINISH:
            return "finalize_before_communication_finish";
        default:
            return UNKNOWN_NAME;
    }
}
} // namespace Hccl
