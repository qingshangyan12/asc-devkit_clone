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
        FIRST_PREPARE_EXPAND_END,
        LAST_FINALIZE_RECEIVED,
        COUNT,
    };

    enum class RecordPolicy : uint8_t {
        KEEP_FIRST = 0,
        KEEP_LATEST,
    };

    using Clock = uint64_t (*)();

    static uint64_t MonotonicNow();

    explicit AicpuTimingRecorder(const char* scenario, Clock clock = MonotonicNow);
    ~AicpuTimingRecorder();

    AicpuTimingRecorder(const AicpuTimingRecorder&) = delete;
    AicpuTimingRecorder& operator=(const AicpuTimingRecorder&) = delete;
    AicpuTimingRecorder(AicpuTimingRecorder&&) = delete;
    AicpuTimingRecorder& operator=(AicpuTimingRecorder&&) = delete;

    bool Record(Stage stage, RecordPolicy policy = RecordPolicy::KEEP_FIRST);
    void Report(const char* status);
    void Reset();

    std::size_t Size() const;
    bool IsReported() const;
    bool GetTimestamp(Stage stage, uint64_t& timestampNs) const;
    bool GetElapsed(Stage beginStage, Stage endStage, uint64_t& elapsedNs) const;

private:
    static constexpr std::size_t STAGE_COUNT = static_cast<std::size_t>(Stage::COUNT);

    struct TimePoint {
        uint64_t timestampNs{0U};
        bool recorded{false};
    };

    static std::size_t StageIndex(Stage stage);
    static const char* StageName(Stage stage);
    static const char* PhaseName(Stage stage);
    uint64_t GetElapsedOrZero(Stage beginStage, Stage endStage) const;

    const char* scenario_;
    Clock clock_;
    std::array<TimePoint, STAGE_COUNT> points_{};
    std::size_t pointCount_{0U};
    bool reported_{false};
};
} // namespace Hccl

#endif // HCCL_AICPU_TIMING_RECORDER_H
