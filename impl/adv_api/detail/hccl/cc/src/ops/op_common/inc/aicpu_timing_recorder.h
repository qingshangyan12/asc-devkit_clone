/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_AICPU_TIMING_RECORDER_H
#define MC2_AICPU_TIMING_RECORDER_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

#if defined(__GNUC__)
#define MC2_AICPU_TIMING_LOCAL __attribute__((visibility("hidden")))
#else
#define MC2_AICPU_TIMING_LOCAL
#endif

namespace mc2_aicpu_timing {
enum class Segment : uint8_t {
    PREPARE_DISPATCH = 0,
    SERVER_SETUP,
    FORMAT_PARAM,
    CCORE_WAIT_SUBMIT,
    OPEN_LAUNCH_SETUP,
    BATCH_START,
    EXECUTOR_ORCHESTRATE,
    BATCH_END_FLUSH,
    CCORE_POST_SUBMIT,
    PREPARE_TOTAL,
    EXECUTOR_SETUP,
    TEMPLATE_SETUP,
    LOCAL_COPY_EXPAND,
    PRE_SYNC_EXPAND,
    PEER_SLICE_PREPARE,
    SEND_RECV_READ_EXPAND,
    POST_SYNC_EXPAND,
    EXECUTOR_TAIL,
    COUNT,
};

class MC2_AICPU_TIMING_LOCAL AicpuTimingRecorder;

class MC2_AICPU_TIMING_LOCAL AicpuTimingSample {
public:
    AicpuTimingSample() = default;
    explicit AicpuTimingSample(AicpuTimingRecorder* recorder) : recorder_(recorder) {}

    void BeginTurn();
    void AddPeer();
    void SetMetadata(uint32_t rank, uint32_t opType, const char* algName);
    void AddDuration(Segment segment, uint64_t begin, uint64_t end);
    void MarkIncomplete();
    uint64_t Now() const;
    bool IsEnabled() const { return recorder_ != nullptr; }

private:
    AicpuTimingRecorder* recorder_{nullptr};
};

class MC2_AICPU_TIMING_LOCAL AicpuTimingScope {
public:
    AicpuTimingScope(AicpuTimingSample* sample, Segment segment);
    ~AicpuTimingScope();

    AicpuTimingScope(const AicpuTimingScope&) = delete;
    AicpuTimingScope& operator=(const AicpuTimingScope&) = delete;

private:
    AicpuTimingSample* sample_{nullptr};
    Segment segment_{Segment::COUNT};
    uint64_t begin_{0U};
};

class MC2_AICPU_TIMING_LOCAL AicpuTimingAwareExecutor {
public:
    virtual ~AicpuTimingAwareExecutor() = default;
    virtual void SetAicpuTimingSample(AicpuTimingSample* sample) = 0;
};

class MC2_AICPU_TIMING_LOCAL AicpuTimingRecorder {
public:
    explicit AicpuTimingRecorder(uint32_t groupIdx);

    AicpuTimingSample CreateSample() { return AicpuTimingSample(this); }
    void Report(const char* requestedStatus);
    bool HasSamples() const { return sampleCount_ != 0U; }
    bool IsReported() const { return reported_; }

    static uint64_t ReadCounter();

private:
    friend class AicpuTimingSample;

    struct SegmentStats {
        uint64_t count{0U};
        uint64_t sumCycles{0U};
        uint64_t minCycles{std::numeric_limits<uint64_t>::max()};
        uint64_t maxCycles{0U};
    };

    static constexpr std::size_t SEGMENT_COUNT = static_cast<std::size_t>(Segment::COUNT);
    static constexpr std::size_t ALG_NAME_CAPACITY = 128U;

    void BeginTurn();
    void AddPeer();
    void SetMetadata(uint32_t rank, uint32_t opType, const char* algName);
    void AddDuration(Segment segment, uint64_t begin, uint64_t end);
    void MarkIncomplete() { invalid_ = true; }
    uint64_t ToNanoseconds(uint64_t cycles);
    uint64_t ExpectedMask() const;
    static const char* SegmentName(Segment segment);
    static uint64_t ReadCounterFrequency();
    static uint64_t CalibrateClockOverhead();

    uint32_t groupIdx_{0U};
    uint32_t rank_{std::numeric_limits<uint32_t>::max()};
    uint32_t opType_{std::numeric_limits<uint32_t>::max()};
    uint64_t counterFrequencyHz_{0U};
    uint64_t clockOverheadCycles_{0U};
    uint64_t sampleCount_{0U};
    uint64_t peerCount_{0U};
    uint64_t recordedMask_{0U};
    std::array<SegmentStats, SEGMENT_COUNT> stats_{};
    std::array<char, ALG_NAME_CAPACITY> algName_{};
    bool metadataSet_{false};
    bool invalid_{false};
    bool reported_{false};
};
} // namespace mc2_aicpu_timing

#undef MC2_AICPU_TIMING_LOCAL

#endif // MC2_AICPU_TIMING_RECORDER_H
