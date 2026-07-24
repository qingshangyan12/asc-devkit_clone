/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CACHE_CTRL_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CACHE_CTRL_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_icache_preload_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_data_cache_preload_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dcci_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_get_icache_preload_status_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dci_impl.h"

// ==========asc_icache_preload==========
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
{
    asc_icache_preload_impl(addr, prefetch_len);
}

__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
{
    asc_datacache_preload_impl(address, offset);
}

namespace __asc_aicore {
__aicore__ inline void asc_dcci_single(__gm__ void* dst) { asc_dcci_single_impl(dst); }
} // namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst) { asc_ub_dcci_single_impl(dst); }

__aicore__ inline void asc_dcci_entire_all() { asc_dcci_entire_all_impl(); }

__aicore__ inline void asc_dcci_entire_out() { asc_dcci_entire_out_impl(); }

__aicore__ inline void asc_dcci_entire_atomic() { asc_dcci_entire_atomic_impl(); }

//=========asc_get_icache_preload_status========
__aicore__ inline int64_t asc_get_icache_preload_status() { return asc_get_icache_preload_status_impl(); }

//==============asc_dci===============
__aicore__ inline void asc_dci() { asc_dci_impl(); }

//=========asc_set_scalar_cache_mode (READ)=========
__aicore__ inline void asc_set_scalar_cache_mode(asc_load_l2_cache_mode cache_mode)
{
    constexpr uint8_t startBit = 16; // READ mode: CTRL[19:16]
    uint64_t cacheBits = 0;
    switch (cache_mode) {
        case asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM:
            cacheBits = 0;
            break;
        case asc_load_l2_cache_mode::NORMAL_LAST_VICTIM:
            cacheBits = 1;
            break;
        case asc_load_l2_cache_mode::NORMAL_PERSISTENT:
            cacheBits = 2;
            break;
        case asc_load_l2_cache_mode::NOTALLOC_KEEP:
            cacheBits = 4;
            break;
        default: // NOTALLOC_CLEAN(5), NOTALLOC_DROP(6) not supported
            return;
    }
    uint64_t mask = ~(uint64_t(0xF) << startBit);
    int64_t ctrlValue = get_ctrl() & mask;
    ctrlValue |= (cacheBits << startBit);
    set_ctrl(ctrlValue);
}

//=========asc_set_scalar_cache_mode (WRITE)=========
__aicore__ inline void asc_set_scalar_cache_mode(asc_store_l2_cache_mode cache_mode)
{
    constexpr uint8_t startBit = 20; // WRITE mode: CTRL[23:20]
    uint64_t mask = ~(uint64_t(0xF) << startBit);
    int64_t ctrlValue = get_ctrl() & mask;
    ctrlValue |= (static_cast<uint64_t>(cache_mode) << startBit);
    set_ctrl(ctrlValue);
}

//=========asc_get_scalar_cache_mode (READ)=========
__aicore__ inline void asc_get_scalar_cache_mode(asc_load_l2_cache_mode& cache_mode)
{
    constexpr uint8_t startBit = 16; // READ mode: CTRL[19:16]
    uint64_t ctrlValue = static_cast<uint64_t>(get_ctrl());
    uint8_t cacheBits = static_cast<uint8_t>((ctrlValue >> startBit) & 0xF);

    switch (cacheBits) {
        case 0:
            cache_mode = asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM;
            break;
        case 1:
            cache_mode = asc_load_l2_cache_mode::NORMAL_LAST_VICTIM;
            break;
        case 2:
            cache_mode = asc_load_l2_cache_mode::NORMAL_PERSISTENT;
            break;
        case 4:
            cache_mode = asc_load_l2_cache_mode::NOTALLOC_KEEP;
            break;
        default:
            cache_mode = asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM;
            break;
    }
}

//=========asc_get_scalar_cache_mode (WRITE)=========
__aicore__ inline void asc_get_scalar_cache_mode(asc_store_l2_cache_mode& cache_mode)
{
    constexpr uint8_t startBit = 20; // WRITE mode: CTRL[23:20]
    uint64_t ctrlValue = static_cast<uint64_t>(get_ctrl());
    uint8_t cacheBits = static_cast<uint8_t>((ctrlValue >> startBit) & 0xF);

    switch (cacheBits) {
        case 0:
            cache_mode = asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM;
            break;
        case 1:
            cache_mode = asc_store_l2_cache_mode::NORMAL_LAST_VICTIM;
            break;
        case 2:
            cache_mode = asc_store_l2_cache_mode::NORMAL_PERSISTENT;
            break;
        case 4:
            cache_mode = asc_store_l2_cache_mode::NOTALLOC_CLEAN;
            break;
        default:
            cache_mode = asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM;
            break;
    }
}

#endif
