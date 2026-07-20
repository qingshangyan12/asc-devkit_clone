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
        FIRST_PREPARE_ACCEPTED,
        LAST_EXPAND_END,
        LAST_FINALIZE_RECEIVED,
        ALL_TASK_FINISHED,
        COUNT,
    };

    enum class Metric : uint8_t {
        ORCHESTRATION = 0,
        COUNT,
    };

    enum class RecordPolicy : uint8_t {
        KEEP_FIRST = 0,
        KEEP_LATEST,
    };

    struct DurationStat {
        uint64_t totalNs{0U};
        uint64_t maxNs{0U};
        uint32_t count{0U};
    };

    using Clock = bool (*)(uint64_t& timestampNs);

    static bool MonotonicNow(uint64_t& timestampNs);

    explicit AicpuTimingRecorder(const char* scenario, Clock clock = MonotonicNow);
    ~AicpuTimingRecorder();

    AicpuTimingRecorder(const AicpuTimingRecorder&) = delete;
    AicpuTimingRecorder& operator=(const AicpuTimingRecorder&) = delete;
    AicpuTimingRecorder(AicpuTimingRecorder&&) = delete;
    AicpuTimingRecorder& operator=(AicpuTimingRecorder&&) = delete;

    bool CaptureTimestamp(uint64_t& timestampNs);
    bool Record(Stage stage, RecordPolicy policy = RecordPolicy::KEEP_FIRST);
    bool RecordAt(Stage stage, uint64_t timestampNs, RecordPolicy policy = RecordPolicy::KEEP_FIRST);
    bool Accumulate(Metric metric, uint64_t beginTimestampNs, uint64_t endTimestampNs);
    void Report(const char* status, uint32_t serverNum = 0U);
    void Reset();

    std::size_t Size() const;
    bool IsReported() const;
    bool IsValid() const;
    bool GetTimestamp(Stage stage, uint64_t& timestampNs) const;
    bool GetElapsed(Stage beginStage, Stage endStage, uint64_t& elapsedNs) const;
    bool GetDuration(Metric metric, DurationStat& stat) const;

private:
    static constexpr std::size_t STAGE_COUNT = static_cast<std::size_t>(Stage::COUNT);
    static constexpr std::size_t METRIC_COUNT = static_cast<std::size_t>(Metric::COUNT);

    struct TimePoint {
        uint64_t timestampNs{0U};
        bool recorded{false};
    };

    static std::size_t StageIndex(Stage stage);
    static std::size_t MetricIndex(Metric metric);
    bool HasCompleteTimeline() const;
    uint64_t GetTimestampOrZero(Stage stage) const;
    uint64_t GetElapsedOrZero(Stage beginStage, Stage endStage) const;

    const char* scenario_;
    Clock clock_;
    std::array<TimePoint, STAGE_COUNT> points_{};
    std::array<DurationStat, METRIC_COUNT> durations_{};
    std::size_t pointCount_{0U};
    bool valid_{true};
    bool reported_{false};
};
} // namespace Hccl

#endif // HCCL_AICPU_TIMING_RECORDER_H
