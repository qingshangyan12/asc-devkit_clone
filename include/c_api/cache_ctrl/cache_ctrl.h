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
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#define INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h"

#endif

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_icache_preload(const void*,int64_t)
 * @archs 3510 2201
 */
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len);

namespace __asc_aicore {
/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_dcci_single(__gm__ void*)
 * @archs 3510 2201
 */
__aicore__ inline void asc_dcci_single(__gm__ void* dst);
}

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_ub_dcci_single(__ubuf__ void*)
 * @archs 3510 2201
 */
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_dcci_entire_ub()
 * @archs 3510 2201
 */
__aicore__ inline void asc_dcci_entire_ub();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_dcci_entire_out()
 * @archs 3510 2201
 */
__aicore__ inline void asc_dcci_entire_out();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_dcci_entire_all()
 * @archs 3510 2201
 */
__aicore__ inline void asc_dcci_entire_all();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_dcci_entire_atomic()
 * @archs 3510 2201
 */
__aicore__ inline void asc_dcci_entire_atomic();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_get_icache_preload_status()
 * @archs 3510 2201
 */
__aicore__ inline int64_t asc_get_icache_preload_status();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_datacache_preload(__gm__ uint64_t*,int64_t)
 * @archs 3510 2201
 */
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_dci()
 * @archs 3510
 */
__aicore__ inline void asc_dci();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_set_scalar_cache_mode(asc_load_l2_cache_mode)
 * @archs 3510
 */
__aicore__ inline void asc_set_scalar_cache_mode(asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_set_scalar_cache_mode(asc_store_l2_cache_mode)
 * @archs 3510
 */
__aicore__ inline void asc_set_scalar_cache_mode(asc_store_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_get_scalar_load_cache_mode()
 * @archs 3510
 */
__aicore__ inline asc_load_l2_cache_mode asc_get_scalar_load_cache_mode();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @function asc_get_scalar_store_cache_mode()
 * @archs 3510
 */
__aicore__ inline asc_store_l2_cache_mode asc_get_scalar_store_cache_mode();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
