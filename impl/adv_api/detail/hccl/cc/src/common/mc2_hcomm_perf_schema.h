/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCOMM_PERF_SCHEMA_V1_H
#define HCOMM_PERF_SCHEMA_V1_H
#include <cstdint>
namespace HcommPerf {
constexpr uint32_t ABI_VERSION = 1;
enum Metric : uint32_t {
    SQE_COUNT,
    SQE_BATCHES,
    RTSQ_COPY_CALLS,
    RTSQ_COPY_BYTES,
    RTSQ_WRAPS,
    RTSQ_SPACE_CHECKS,
    RTSQ_WAIT_CALLS,
    RTSQ_TAIL_UPDATES,
    WQE_COUNT,
    WQE_WAIT_CALLS,
    CAPTURE_ARRAYS,
    CAPTURE_BYTES,
    CLOCK_ERRORS,
    RTSQ_COPY_NS,
    RTSQ_SPACE_NS,
    RTSQ_WAIT_NS,
    RTSQ_TAIL_NS,
    SQE_REFRESH_NS,
    WQE_REFRESH_NS,
    CAPTURE_NS,
    WQE_SUBMIT_NS,
    WQE_SPACE_NS,
    WQE_WAIT_NS,
    METRIC_COUNT
};
constexpr uint32_t COUNT_METRICS = 13;
constexpr const char* NAMES[] = {
    "hcomm_sqe_count",     "hcomm_sqe_batches",       "hcomm_rtsq_copy_calls", "hcomm_rtsq_copy_bytes",
    "hcomm_rtsq_wraps",    "hcomm_rtsq_space_checks", "hcomm_rtsq_wait_calls", "hcomm_rtsq_tail_updates",
    "hcomm_wqe_count",     "hcomm_wqe_wait_calls",    "hcomm_capture_arrays",  "hcomm_capture_bytes",
    "hcomm_clock_errors",  "hcomm_rtsq_copy_ns",      "hcomm_rtsq_space_ns",   "hcomm_rtsq_wait_ns",
    "hcomm_rtsq_tail_ns",  "hcomm_sqe_refresh_ns",    "hcomm_wqe_refresh_ns",  "hcomm_capture_ns",
    "hcomm_wqe_submit_ns", "hcomm_wqe_space_ns",      "hcomm_wqe_wait_ns"};
} // namespace HcommPerf
#endif
