/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_CACHE_CTRL_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_CACHE_CTRL_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/memory_base_impl/cache_ctrl_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CACHE_CTRL_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_intf_impl.h"

__aicore__ inline int64_t asc_get_icache_preload_status_impl() { return get_icache_prl_st(); }

__aicore__ inline void asc_datacache_preload_impl(__gm__ uint64_t* address, int64_t offset)
{
    dc_preload(address, offset);
}

namespace __asc_aicore {
__aicore__ inline void asc_dcci_single_impl(__gm__ void* dst) { dcci(dst, cache_line_t::SINGLE_CACHE_LINE); }
} // namespace __asc_aicore

// namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single_impl(__ubuf__ void* dst) { dcci(dst, cache_line_t::SINGLE_CACHE_LINE); }

__aicore__ inline void asc_dcci_entire_all_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_ALL);
}

__aicore__ inline void asc_dcci_entire_ub_impl()
{
    dcci((__ubuf__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_UB);
}

__aicore__ inline void asc_dcci_entire_out_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_OUT);
}

__aicore__ inline void asc_dcci_entire_atomic_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_ATOMIC);
}

__aicore__ inline void asc_icache_preload_impl(const void* addr, int64_t prefetch_len) { preload(addr, prefetch_len); }

namespace __asc_aicore {
__aicore__ inline void asc_dcci_single(__gm__ void* dst) { asc_dcci_single_impl(dst); }
} // namespace __asc_aicore

// namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst) { asc_ub_dcci_single_impl(dst); }

__aicore__ inline void asc_dcci_entire_all() { asc_dcci_entire_all_impl(); }

__aicore__ inline void asc_dcci_entire_ub() { asc_dcci_entire_ub_impl(); }

__aicore__ inline void asc_dcci_entire_out() { asc_dcci_entire_out_impl(); }

__aicore__ inline void asc_dcci_entire_atomic() { asc_dcci_entire_atomic_impl(); }

__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
{
    asc_datacache_preload_impl(address, offset);
}

__aicore__ inline int64_t asc_get_icache_preload_status() { return asc_get_icache_preload_status_impl(); }

__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
{
    asc_icache_preload_impl(addr, prefetch_len);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CACHE_CTRL_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CACHE_CTRL_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_CACHE_CTRL_INTF_IMPL_H
