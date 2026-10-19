/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "cache_ctrl.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CACHE_CTRL_CACHE_CTRL_H
#endif

#ifndef INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#define INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/cache_ctrl_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/cache_ctrl_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Manually preloads instructions from the current Global Memory address (PC via asc_get_program_counter masked to 48 bits) into ICache to avoid later fetch misses; prefetch_len is in 2 KB units.
 *
 * @override_id asc_icache_preload__PCv_i64
 * @archs 2201 3510
 * @param [in] addr 预取指令的Global Memory起始地址。
 * @param [in] prefetch_len 预加载数据的长度，单位为2K Byte。
 */
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len);

namespace __asc_aicore {
/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Cleans and invalidates the single Cache Line containing the given GM address dst, writing dirty data back to GM (void return, PIPE_S).
 *
 * @override_id asc_dcci_single__PGv
 * @archs 2201 3510
 * @param [in] dst 待清理与失效的Cache Line所在地址。 硬件按Cache Line边界对齐定位目标行，传入地址无需满足Cache Line对齐。
 */
__aicore__ inline void asc_dcci_single(__gm__ void* dst);
}

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Cleans and invalidates the single Cache Line containing the given UB address dst (AIV only, void return, PIPE_S).
 *
 * @override_id asc_ub_dcci_single__PUv
 * @archs 2201 3510
 * @param [in] dst 待清理与失效的Cache Line所在地址。 硬件按Cache Line边界对齐定位目标行，传入地址无需满足Cache Line对齐。
 */
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Cleans and invalidates all valid Cache Lines containing UB addresses in the entire DCache (AIV only, void return, PIPE_S).
 *
 * @override_id asc_dcci_entire_ub__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_dcci_entire_ub();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Cleans and invalidates all valid Cache Lines containing GM addresses in the entire DCache, writing dirty data back to GM (void return, PIPE_S).
 *
 * @override_id asc_dcci_entire_out__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_dcci_entire_out();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Cleans (writes dirty data back to GM) and invalidates all valid Cache Lines in the entire DCache (void return, PIPE_S).
 *
 * @override_id asc_dcci_entire_all__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_dcci_entire_all();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Cleans and invalidates all valid Cache Lines containing GM addresses related to scalar atomic operations in the entire DCache (void return, PIPE_S).
 *
 * @override_id asc_dcci_entire_atomic__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_dcci_entire_atomic();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Debug query called after asc_icache_preload that returns the ICache preload status: 0 means the previous preload is idle/completed, 1 means it is still in progress.
 *
 * @override_id asc_get_icache_preload_status__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_icache_preload_status();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Preloads one 64-byte Cache Line from the Global Memory address plus byte offset into DCache ahead of a later Scalar read, reducing first-access latency.
 *
 * @override_id asc_datacache_preload__PGu64_i64
 * @archs 2201 3510
 * @param [in] address 源操作数在Global Memory中地址，单位为字节。
 * @param [in] offset 相对`address`的有符号字节偏移，预取起始地址 = `address` + `offset`，单位为字节。
 */
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Invalidates all Cache Lines of the whole DCache, discarding dirty data without write-back so later accesses reload from Global Memory; call asc_sync_data_barrier(DSB_ALL) first.
 *
 * @override_id asc_dci__z
 * @archs 3510
 */
__aicore__ inline void asc_dci();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Sets the L2 Cache management policy for scalar load accesses to Global Memory via the asc_load_l2_cache_mode overload; later scalar GM reads follow it until changed.
 *
 * @override_id asc_set_scalar_cache_mode__loadl2
 * @archs 3510
 * @param [in] l2_cache_mode L2 Cache管理策略： &bull; 配置标量load接口的L2 Cache管理策略重载传入[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)枚举值。 &bull; 配置标量store接口的L2 Cache管理策略重载传入[asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md)枚举值。
 */
__aicore__ inline void asc_set_scalar_cache_mode(asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Sets the L2 Cache management policy for scalar store accesses to Global Memory via the asc_store_l2_cache_mode overload; later scalar GM writes follow it until changed.
 *
 * @override_id asc_set_scalar_cache_mode__storel2
 * @archs 3510
 * @param [in] l2_cache_mode L2 Cache管理策略： &bull; 配置标量load接口的L2 Cache管理策略重载传入[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)枚举值。 &bull; 配置标量store接口的L2 Cache管理策略重载传入[asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md)枚举值。
 */
__aicore__ inline void asc_set_scalar_cache_mode(asc_store_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Returns the current L2 Cache management policy of the scalar load path as an asc_load_l2_cache_mode value, e.g. to save the old setting or verify after setting it.
 *
 * @override_id asc_get_scalar_load_cache_mode__z
 * @archs 3510
 */
__aicore__ inline asc_load_l2_cache_mode asc_get_scalar_load_cache_mode();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @brief Returns the current L2 Cache management policy of the scalar store path as an asc_store_l2_cache_mode value, e.g. to save the old setting or verify after setting it.
 *
 * @override_id asc_get_scalar_store_cache_mode__z
 * @archs 3510
 */
__aicore__ inline asc_store_l2_cache_mode asc_get_scalar_store_cache_mode();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CACHE_CTRL_CACHE_CTRL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CACHE_CTRL_CACHE_CTRL_H
#endif
