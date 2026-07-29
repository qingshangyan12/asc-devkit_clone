/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file scalar_cache_mode_perf.h
 * \brief Kernel implementations for scalar cache mode performance verification
 *
 * 4 cases verifying the functional effect of asc_set_scalar_cache_mode on
 * scalar unit GM access, observed via msopprof L2Cache metrics:
 *   Case 1: Read channel NORMAL_PERSISTENT  — repeated read, L2 should hit on pass 2
 *   Case 2: Read channel NOTALLOC_KEEP      — repeated read, L2 should miss on pass 2
 *   Case 3: Write channel NORMAL_FIRST_VICTIM — write allocates L2 cache lines
 *   Case 4: Write channel NOTALLOC_CLEAN      — write bypasses L2
 */

#ifndef SCALAR_CACHE_MODE_PERF_H
#define SCALAR_CACHE_MODE_PERF_H

#include "c_api/asc_simd.h"

constexpr uint32_t WORK_SET_SIZE = 16384;

/* !
 * \brief Case 1: Read channel NORMAL_PERSISTENT
 *
 * Sets the read-channel scalar cache mode to NORMAL_PERSISTENT (驻留模式).
 * Performs two passes of scalar GM reads over the same 16384-element region.
 * Pass 1 fills L2; after clearing L1 Data Cache (L2 retains data due to
 * persistent mode), Pass 2 should observe high L2 read hit rate.
 */
__aicore__ inline void ProcessReadPersistent(__gm__ float* src, __gm__ float* dst)
{
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NORMAL_PERSISTENT);

    volatile float sum1 = 0.0f;
    for (uint32_t i = 0; i < WORK_SET_SIZE; i++) {
        sum1 += src[i];
    }

    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();

    volatile float sum2 = 0.0f;
    for (uint32_t i = 0; i < WORK_SET_SIZE; i++) {
        sum2 += src[i];
    }

    dst[0] = sum1 + sum2;
}

/* !
 * \brief Case 2: Read channel NOTALLOC_KEEP
 *
 * Sets the read-channel scalar cache mode to NOTALLOC_KEEP (不分配 L2 Cache Line).
 * Performs two passes of scalar GM reads over the same region.
 * Since L2 cache lines are never allocated, Pass 2 should still observe L2
 * read misses, contrasting sharply with Case 1.
 */
__aicore__ inline void ProcessReadStreamNoAlloc(__gm__ float* src, __gm__ float* dst)
{
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NOTALLOC_KEEP);

    volatile float sum1 = 0.0f;
    for (uint32_t i = 0; i < WORK_SET_SIZE; i++) {
        sum1 += src[i];
    }

    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();

    volatile float sum2 = 0.0f;
    for (uint32_t i = 0; i < WORK_SET_SIZE; i++) {
        sum2 += src[i];
    }

    dst[0] = sum1 + sum2;
}

/* !
 * \brief Case 3: Write channel NORMAL_FIRST_VICTIM
 *
 * Sets the write-channel scalar cache mode to NORMAL_FIRST_VICTIM (启用 L2 Cache).
 * Performs scalar GM writes over 16384 elements.
 * Writes should allocate L2 cache lines, observable via aiv_write_cache_miss_allocate.
 * Output data correctness is verified on host (dst[i] == float(i)).
 */
__aicore__ inline void ProcessWriteNormal(__gm__ float* dst)
{
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    asc_set_scalar_cache_mode(asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM);

    for (uint32_t i = 0; i < WORK_SET_SIZE; i++) {
        dst[i] = static_cast<float>(i);
    }

    asc_dcci_entire_out();
}

/* !
 * \brief Case 4: Write channel NOTALLOC_CLEAN
 *
 * Sets the write-channel scalar cache mode to NOTALLOC_CLEAN (不启用 L2 Cache).
 * Performs scalar GM writes over 16384 elements.
 * Writes should bypass L2, so aiv_write_cache_miss_allocate should be near zero.
 * Output data correctness is verified on host (dst[i] == float(i)).
 */
__aicore__ inline void ProcessWriteNoAlloc(__gm__ float* dst)
{
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    asc_set_scalar_cache_mode(asc_store_l2_cache_mode::NOTALLOC_CLEAN);

    for (uint32_t i = 0; i < WORK_SET_SIZE; i++) {
        dst[i] = static_cast<float>(i);
    }

    asc_dcci_entire_out();
}

#endif // SCALAR_CACHE_MODE_PERF_H
