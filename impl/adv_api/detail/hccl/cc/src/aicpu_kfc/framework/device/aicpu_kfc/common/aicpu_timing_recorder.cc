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

#include <algorithm>
#include <cstring>

#include "log.h"

namespace mc2_aicpu_timing {
namespace {
constexpr uint64_t NANOSECONDS_PER_SECOND = 1000000000ULL;
constexpr uint32_t CLOCK_CALIBRATION_ROUNDS = 32U;
constexpr std::size_t REPORT_BUFFER_SIZE = 4096U;
constexpr char UNKNOWN_NAME[] = "unknown";
constexpr char COMPLETE_STATUS[] = "complete";
constexpr char INCOMPLETE_STATUS[] = "incomplete";
constexpr char ALL_GATHER_MESH_1D[] = "InsAllGatherMesh1D";

constexpr uint64_t SegmentBit(Segment segment)
{
    return 1ULL << static_cast<uint8_t>(segment);
}

constexpr uint64_t OUTER_EXPECTED_MASK =
    SegmentBit(Segment::PREPARE_DISPATCH) | SegmentBit(Segment::SERVER_SETUP) |
    SegmentBit(Segment::FORMAT_PARAM) | SegmentBit(Segment::CCORE_WAIT_SUBMIT) |
    SegmentBit(Segment::OPEN_LAUNCH_SETUP) | SegmentBit(Segment::BATCH_START) |
    SegmentBit(Segment::EXECUTOR_ORCHESTRATE) | SegmentBit(Segment::BATCH_END_FLUSH) |
    SegmentBit(Segment::CCORE_POST_SUBMIT) | SegmentBit(Segment::PREPARE_TOTAL);

constexpr uint64_t ALL_GATHER_EXPECTED_MASK =
    SegmentBit(Segment::EXECUTOR_SETUP) | SegmentBit(Segment::TEMPLATE_SETUP) |
    SegmentBit(Segment::LOCAL_COPY_EXPAND) | SegmentBit(Segment::PRE_SYNC_EXPAND) |
    SegmentBit(Segment::PEER_SLICE_PREPARE) | SegmentBit(Segment::SEND_RECV_READ_EXPAND) |
    SegmentBit(Segment::POST_SYNC_EXPAND) | SegmentBit(Segment::EXECUTOR_TAIL);
} // namespace

AicpuTimingRecorder::AicpuTimingRecorder(uint32_t groupIdx)
    : groupIdx_(groupIdx), counterFrequencyHz_(ReadCounterFrequency()), clockOverheadCycles_(CalibrateClockOverhead())
{
    if (counterFrequencyHz_ == 0U || clockOverheadCycles_ == std::numeric_limits<uint64_t>::max()) {
        invalid_ = true;
        clockOverheadCycles_ = 0U;
    }
}

uint64_t AicpuTimingRecorder::ReadCounter()
{
#if defined(__aarch64__)
    uint64_t value = 0U;
    asm volatile("isb\nmrs %0, cntvct_el0" : "=r"(value) : : "memory");
    return value;
#else
    return 0U;
#endif
}

uint64_t AicpuTimingRecorder::ReadCounterFrequency()
{
#if defined(__aarch64__)
    uint64_t value = 0U;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(value) : : "memory");
    return value;
#else
    return 0U;
#endif
}

uint64_t AicpuTimingRecorder::CalibrateClockOverhead()
{
    uint64_t overhead = std::numeric_limits<uint64_t>::max();
    uint64_t previous = ReadCounter();
    if (previous == 0U) {
        return std::numeric_limits<uint64_t>::max();
    }
    for (uint32_t i = 0U; i < CLOCK_CALIBRATION_ROUNDS; ++i) {
        const uint64_t current = ReadCounter();
        if (current < previous) {
            return std::numeric_limits<uint64_t>::max();
        }
        overhead = std::min(overhead, current - previous);
        previous = current;
    }
    return overhead;
}

void AicpuTimingRecorder::BeginTurn()
{
    if (sampleCount_ == std::numeric_limits<uint64_t>::max()) {
        invalid_ = true;
        return;
    }
    ++sampleCount_;
}

void AicpuTimingRecorder::AddPeer()
{
    if (peerCount_ == std::numeric_limits<uint64_t>::max()) {
        invalid_ = true;
        return;
    }
    ++peerCount_;
}

void AicpuTimingRecorder::SetMetadata(uint32_t rank, uint32_t opType, const char* algName)
{
    if (algName == nullptr) {
        invalid_ = true;
        return;
    }
    if (metadataSet_) {
        if (rank_ != rank || opType_ != opType || std::strncmp(algName_.data(), algName, algName_.size()) != 0) {
            invalid_ = true;
        }
        return;
    }

    const int copyRet = strncpy_s(algName_.data(), algName_.size(), algName, algName_.size() - 1U);
    if (copyRet != EOK) {
        invalid_ = true;
        algName_[0] = '\0';
        return;
    }
    rank_ = rank;
    opType_ = opType;
    metadataSet_ = true;
}

void AicpuTimingRecorder::AddDuration(Segment segment, uint64_t begin, uint64_t end)
{
    const std::size_t index = static_cast<std::size_t>(segment);
    if (index >= SEGMENT_COUNT || begin == 0U || end < begin) {
        invalid_ = true;
        return;
    }

    const uint64_t rawCycles = end - begin;
    const uint64_t cycles = rawCycles > clockOverheadCycles_ ? rawCycles - clockOverheadCycles_ : 0U;
    SegmentStats& stats = stats_[index];
    if (stats.sumCycles > std::numeric_limits<uint64_t>::max() - cycles ||
        stats.count == std::numeric_limits<uint64_t>::max()) {
        invalid_ = true;
        return;
    }
    ++stats.count;
    stats.sumCycles += cycles;
    stats.minCycles = std::min(stats.minCycles, cycles);
    stats.maxCycles = std::max(stats.maxCycles, cycles);
    recordedMask_ |= SegmentBit(segment);
}

uint64_t AicpuTimingRecorder::ToNanoseconds(uint64_t cycles)
{
    if (counterFrequencyHz_ == 0U) {
        invalid_ = true;
        return 0U;
    }
    const uint64_t seconds = cycles / counterFrequencyHz_;
    const uint64_t remainder = cycles % counterFrequencyHz_;
    if (seconds > std::numeric_limits<uint64_t>::max() / NANOSECONDS_PER_SECOND ||
        remainder > std::numeric_limits<uint64_t>::max() / NANOSECONDS_PER_SECOND) {
        invalid_ = true;
        return 0U;
    }
    return seconds * NANOSECONDS_PER_SECOND + remainder * NANOSECONDS_PER_SECOND / counterFrequencyHz_;
}

uint64_t AicpuTimingRecorder::ExpectedMask() const
{
    if (metadataSet_ && std::strcmp(algName_.data(), ALL_GATHER_MESH_1D) == 0) {
        return OUTER_EXPECTED_MASK | ALL_GATHER_EXPECTED_MASK;
    }
    return OUTER_EXPECTED_MASK;
}

const char* AicpuTimingRecorder::SegmentName(Segment segment)
{
    switch (segment) {
        case Segment::PREPARE_DISPATCH:
            return "prepare_dispatch";
        case Segment::SERVER_SETUP:
            return "server_setup";
        case Segment::FORMAT_PARAM:
            return "format_param";
        case Segment::CCORE_WAIT_SUBMIT:
            return "ccore_wait_submit";
        case Segment::OPEN_LAUNCH_SETUP:
            return "open_launch_setup";
        case Segment::BATCH_START:
            return "batch_start";
        case Segment::EXECUTOR_ORCHESTRATE:
            return "executor_orchestrate";
        case Segment::BATCH_END_FLUSH:
            return "batch_end_flush";
        case Segment::CCORE_POST_SUBMIT:
            return "ccore_post_submit";
        case Segment::PREPARE_TOTAL:
            return "prepare_total";
        case Segment::EXECUTOR_SETUP:
            return "executor_setup";
        case Segment::TEMPLATE_SETUP:
            return "template_setup";
        case Segment::LOCAL_COPY_EXPAND:
            return "local_copy_expand";
        case Segment::PRE_SYNC_EXPAND:
            return "pre_sync_expand";
        case Segment::PEER_SLICE_PREPARE:
            return "peer_slice_prepare";
        case Segment::SEND_RECV_READ_EXPAND:
            return "send_recv_read_expand";
        case Segment::POST_SYNC_EXPAND:
            return "post_sync_expand";
        case Segment::EXECUTOR_TAIL:
            return "executor_tail";
        default:
            return UNKNOWN_NAME;
    }
}

void AicpuTimingRecorder::Report(const char* requestedStatus)
{
    if (reported_) {
        return;
    }
    const bool requestedComplete = requestedStatus != nullptr && std::strcmp(requestedStatus, COMPLETE_STATUS) == 0;
    if (!HasSamples() && requestedComplete) {
        return;
    }
    reported_ = true;

    struct SegmentNsStats {
        uint64_t mean{0U};
        uint64_t min{0U};
        uint64_t max{0U};
    };
    std::array<SegmentNsStats, SEGMENT_COUNT> nsStats{};
    for (std::size_t i = 0U; i < SEGMENT_COUNT; ++i) {
        const SegmentStats& stats = stats_[i];
        if (stats.count == 0U) {
            continue;
        }
        nsStats[i].mean = ToNanoseconds(stats.sumCycles / stats.count);
        nsStats[i].min = ToNanoseconds(stats.minCycles);
        nsStats[i].max = ToNanoseconds(stats.maxCycles);
    }

    const uint64_t missingMask = ExpectedMask() & ~recordedMask_;
    const bool complete = requestedComplete && metadataSet_ && !invalid_ && missingMask == 0U;
    const char* status = complete ? COMPLETE_STATUS : INCOMPLETE_STATUS;
    const char* algName = metadataSet_ && algName_[0] != '\0' ? algName_.data() : UNKNOWN_NAME;
    std::array<char, REPORT_BUFFER_SIZE> report{};
    int written = snprintf_s(
        report.data(), report.size(), report.size() - 1U,
        "[MC2_AICPU_TIMING] status[%s], group[%u], rank[%u], opType[%u], algName[%s], sampleCount[%llu], "
        "peerCount[%llu], counterFreqHz[%llu], clockOverheadCycles[%llu], missingMask[0x%llx], "
        "statOrder[mean/min/max/count]",
        status, groupIdx_, rank_, opType_, algName, static_cast<unsigned long long>(sampleCount_),
        static_cast<unsigned long long>(peerCount_),
        static_cast<unsigned long long>(counterFrequencyHz_), static_cast<unsigned long long>(clockOverheadCycles_),
        static_cast<unsigned long long>(missingMask));
    if (written < 0) {
        HCCL_RUN_INFO(
            "[MC2_AICPU_TIMING] status[incomplete], group[%u], report formatting failed.", groupIdx_);
        return;
    }

    std::size_t offset = static_cast<std::size_t>(written);
    bool formatFailed = false;
    for (std::size_t i = 0U; i < SEGMENT_COUNT; ++i) {
        const SegmentStats& stats = stats_[i];
        if (stats.count == 0U) {
            continue;
        }
        if (offset >= report.size()) {
            formatFailed = true;
            break;
        }
        const std::size_t remaining = report.size() - offset;
        written = snprintf_s(
            report.data() + offset, remaining, remaining - 1U, ", %s_ns[%llu/%llu/%llu/%llu]",
            SegmentName(static_cast<Segment>(i)), static_cast<unsigned long long>(nsStats[i].mean),
            static_cast<unsigned long long>(nsStats[i].min), static_cast<unsigned long long>(nsStats[i].max),
            static_cast<unsigned long long>(stats.count));
        if (written < 0) {
            formatFailed = true;
            break;
        }
        offset += static_cast<std::size_t>(written);
    }
    if (formatFailed) {
        HCCL_RUN_INFO(
            "[MC2_AICPU_TIMING] status[incomplete], group[%u], report formatting failed.", groupIdx_);
        return;
    }
    HCCL_RUN_INFO("%s", report.data());
}

void AicpuTimingSample::BeginTurn()
{
    if (recorder_ != nullptr) {
        recorder_->BeginTurn();
    }
}

void AicpuTimingSample::AddPeer()
{
    if (recorder_ != nullptr) {
        recorder_->AddPeer();
    }
}

void AicpuTimingSample::SetMetadata(uint32_t rank, uint32_t opType, const char* algName)
{
    if (recorder_ != nullptr) {
        recorder_->SetMetadata(rank, opType, algName);
    }
}

void AicpuTimingSample::AddDuration(Segment segment, uint64_t begin, uint64_t end)
{
    if (recorder_ != nullptr) {
        recorder_->AddDuration(segment, begin, end);
    }
}

void AicpuTimingSample::MarkIncomplete()
{
    if (recorder_ != nullptr) {
        recorder_->MarkIncomplete();
    }
}

uint64_t AicpuTimingSample::Now() const
{
    return recorder_ == nullptr ? 0U : AicpuTimingRecorder::ReadCounter();
}

AicpuTimingScope::AicpuTimingScope(AicpuTimingSample* sample, Segment segment)
    : sample_(sample), segment_(segment), begin_(sample == nullptr ? 0U : sample->Now())
{}

AicpuTimingScope::~AicpuTimingScope()
{
    if (sample_ != nullptr) {
        sample_->AddDuration(segment_, begin_, sample_->Now());
    }
}
} // namespace mc2_aicpu_timing
