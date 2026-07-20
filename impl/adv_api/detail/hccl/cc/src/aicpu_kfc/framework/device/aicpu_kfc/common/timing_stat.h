/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_AICPU_TIMING_STAT_H
#define HCCL_AICPU_TIMING_STAT_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace Hccl {
class TimingStat {
public:
    static constexpr std::size_t CAPACITY = 32U;

    enum class MarkPolicy : uint8_t {
        KEEP_FIRST = 0,
        KEEP_LATEST,
    };

    using Clock = uint64_t (*)();

    static uint64_t MonotonicNow();

    explicit TimingStat(const char* scenario, Clock clock = MonotonicNow);
    ~TimingStat();

    TimingStat(const TimingStat&) = delete;
    TimingStat& operator=(const TimingStat&) = delete;
    TimingStat(TimingStat&&) = delete;
    TimingStat& operator=(TimingStat&&) = delete;

    bool Mark(const char* stage, MarkPolicy policy = MarkPolicy::KEEP_FIRST);
    void Report(const char* status);
    void Reset();

    std::size_t Size() const;
    bool IsReported() const;
    bool GetTimestamp(const char* stage, uint64_t& timestampNs) const;
    bool GetElapsed(const char* beginStage, const char* endStage, uint64_t& elapsedNs) const;

private:
    struct TimePoint {
        std::string stage;
        uint64_t timestampNs{0U};
    };

    std::size_t FindPoint(const char* stage) const;

    std::string scenario_;
    Clock clock_;
    std::array<TimePoint, CAPACITY> points_{};
    std::size_t pointCount_{0U};
    bool reported_{false};
};
} // namespace Hccl

#endif // HCCL_AICPU_TIMING_STAT_H
