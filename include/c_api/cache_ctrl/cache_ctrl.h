/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#define INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "../../../impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "../../../impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h"

#endif

/*
 * @ingroup CacheControl
 * @brief Preload instruction cache
 * @param [in] addr address to preload
 * @param [in] prefetch_len prefetch length in bytes
 * @details Prefetch instructions from specified address into instruction cache to improve execution performance
 */
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len);

namespace __asc_aicore {
/*
 * @ingroup CacheControl
 * @brief Data cache clean and invalidate single entry (Global Memory) 
 * @param [in] dst address in Global Memory to clean and invalidate
 * @details These functions are deprecated and will be moved to lower-level implementation files.
 *          Clean and invalidate a single data cache entry for the specified Global Memory address
 */
__aicore__ inline void asc_dcci_single(__gm__ void* dst);
}

/*
 * @ingroup CacheControl
 * @brief Data cache clean and invalidate single entry (Global Memory)
 * @param [in] dst address in Global Memory to clean and invalidate
 * @details Clean and invalidate a single data cache entry for the specified Global Memory address
 */
__aicore__ inline void asc_simd_dcci_single(__gm__ void* dst);

/*
 * @ingroup CacheControl
 * @brief Unified Buffer data cache clean and invalidate single entry
 * @param [in] dst address in Unified Buffer to clean and invalidate
 * @details Clean and invalidate a single data cache entry for the specified Unified Buffer address
 */
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst);


/*
 * @ingroup CacheControl
 * @brief Clean and invalidate entire output data cache
 * @details Clean and invalidate all data cache entries for output buffer
 */
__aicore__ inline void asc_dcci_entire_out();

/*
 * @ingroup CacheControl
 * @brief Clean and invalidate entire data cache
 * @details Clean and invalidate all data cache entries across the entire system
 */
__aicore__ inline void asc_dcci_entire_all();

/*
 * @ingroup CacheControl
 * @brief Clean and invalidate entire atomic data cache
 * @details Clean and invalidate all data cache entries used for atomic operations
 */
__aicore__ inline void asc_dcci_entire_atomic();

/*
 * @ingroup CacheControl
 * @brief Get instruction cache preload status
 * @return preload status value
 * @details Retrieve the current status of instruction cache preload operations
 */
__aicore__ inline int64_t asc_get_icache_preload_status();

/*
 * @ingroup CacheControl
 * @brief Preload data cache
 * @param [in] address base address in Global Memory
 * @param [in] offset offset from base address
 * @details Prefetch data from specified Global Memory address into data cache to improve access performance
 */
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
/*
 * @ingroup CacheControl
 * @brief Clean and invalidate entire Unified Buffer cache
 * @details Clean and invalidate all data cache entries for Unified Buffer (NPU architecture 2201 only)
 */
__aicore__ inline void asc_dcci_entire_ub();
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/*
 * @ingroup CacheControl
 * @brief Data cache invalidate
 * @details Invalidate data cache entries for NPU architecture 3510
 */
__aicore__ inline void asc_dci();
#endif

#endif // INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif    
