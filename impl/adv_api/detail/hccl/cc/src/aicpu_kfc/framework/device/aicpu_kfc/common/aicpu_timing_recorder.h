/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_AICPU_TIMING_RECORDER_H
#define HCCL_AICPU_TIMING_RECORDER_H

#include <array>
#include <cstddef>
#include <cstdint>

namespace Hccl {
class AicpuTimingRecorder {
public:
    enum class Stage : uint8_t {
        KERNEL_ENTRY = 0,
        KERNEL_ARGS_PARSED,
        DISPATCHER_ENTER,
        SERVER_CONTEXT_READY,
        FIRST_PREPARE_RECEIVED,
        FIRST_PREPARE_ACCEPTED,
        FIRST_PREPARE_EXPAND_END,
        LAST_PREPARE_EXPAND_END,
        FIRST_COMM_TURN_FINISHED_OBSERVED,
        ALL_COMMUNICATION_FINISHED_OBSERVED,
        FIRST_FINALIZE_RECEIVED,
        LAST_FINALIZE_RECEIVED,
        LAST_FINALIZE_PROCESSED,
        ALL_SERVERS_FINISHED,
        REPORT_BEGIN,
        COUNT,
    };

    enum class Metric : uint8_t {
        ORCHESTRATE_PRECHECK = 0,
        PARAM_FORMAT,
        WAIT_SQE_BUILD,
        WAIT_SQE_SUBMIT,
        ALGORITHM_EXPAND,
        RESOURCE_DESERIALIZE,
        VARIABLE_DATA_RESTORE,
        EXECUTOR_RESOLVE,
        EXECUTOR_ORCHESTRATE,
        POST_SQE_BUILD,
        POST_SQE_SUBMIT,
        PREPARE_BOOKKEEPING,
        SUBMIT_TO_COMPLETION_OBSERVED,
        COUNT,
    };

    enum class Counter : uint8_t {
        EMPTY_POLL = 0,
        SEQ_RETRY,
        INTER_GROUP_SYNC_RETRY,
        PREPARE,
        FINALIZE,
        COMM_OVERLAP_EXPAND,
        FINALIZE_BEFORE_COMM_FINISH,
        COUNT,
    };

    enum class RecordPolicy : uint8_t {
        KEEP_FIRST = 0,
        KEEP_LATEST,
    };

    enum class Level : uint8_t {
        DISABLED = 0,
        BASIC_TIMING,
        BASIC_TIMING_WITH_LOG,
        FULL_TIMING,
        FULL_TIMING_WITH_LOG,
    };

    using Clock = uint64_t (*)();

    struct MetricStats {
        uint64_t count;
        uint64_t totalNs;
        uint64_t minNs;
        uint64_t maxNs;
    };

    static uint64_t MonotonicNow();

    explicit AicpuTimingRecorder(const char* scenario, Clock clock = MonotonicNow);
    ~AicpuTimingRecorder();

    AicpuTimingRecorder(const AicpuTimingRecorder&) = delete;
    AicpuTimingRecorder& operator=(const AicpuTimingRecorder&) = delete;
    AicpuTimingRecorder(AicpuTimingRecorder&&) = delete;
    AicpuTimingRecorder& operator=(AicpuTimingRecorder&&) = delete;

    uint64_t Now() const;
    bool IsEnabled() const;
    bool IsFullEnabled() const;
    bool IsLogEnabled() const;
    bool Record(Stage stage, RecordPolicy policy = RecordPolicy::KEEP_FIRST);
    bool RecordAt(Stage stage, uint64_t timestampNs, RecordPolicy policy = RecordPolicy::KEEP_FIRST);
    bool AddMetric(Metric metric, uint64_t elapsedNs);
    bool IncrementCounter(Counter counter, uint64_t amount = 1U);
    void Report(const char* status);
    void Reset();

    std::size_t Size() const;
    bool IsReported() const;
    bool GetTimestamp(Stage stage, uint64_t& timestampNs) const;
    bool GetElapsed(Stage beginStage, Stage endStage, uint64_t& elapsedNs) const;
    bool GetMetric(Metric metric, MetricStats& stats) const;
    bool GetCounter(Counter counter, uint64_t& value) const;

private:
    static constexpr std::size_t STAGE_COUNT = static_cast<std::size_t>(Stage::COUNT);
    static constexpr std::size_t METRIC_COUNT = static_cast<std::size_t>(Metric::COUNT);
    static constexpr std::size_t COUNTER_COUNT = static_cast<std::size_t>(Counter::COUNT);

    struct TimePoint {
        uint64_t timestampNs;
        bool recorded;
    };

    static std::size_t StageIndex(Stage stage);
    static std::size_t MetricIndex(Metric metric);
    static std::size_t CounterIndex(Counter counter);
    bool IsStageEnabled(Stage stage) const;
    bool IsMetricEnabled(Metric metric) const;
    static const char* StageName(Stage stage);
    static const char* MetricName(Metric metric);
    static const char* CounterName(Counter counter);

    const char* scenario_;
    Clock clock_;
    Level level_{Level::DISABLED};
    std::array<TimePoint, STAGE_COUNT> points_{};
    std::array<MetricStats, METRIC_COUNT> metrics_{};
    std::array<uint64_t, COUNTER_COUNT> counters_{};
    std::size_t pointCount_{0U};
    bool reported_{false};
};
} // namespace Hccl

#endif // HCCL_AICPU_TIMING_RECORDER_H
