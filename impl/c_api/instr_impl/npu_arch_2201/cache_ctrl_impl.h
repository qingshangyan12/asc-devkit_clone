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
#warning "impl/c_api/c_api_instr_impl/cache_ctrl_c_api_impl/cache_ctrl_c_api_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif    

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_2201_CACHE_CTRL_IMPL_CACHE_CTRL_C_API_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_2201_CACHE_CTRL_IMPL_CACHE_CTRL_C_API_IMPL_H

#include "cache_ctrl_impl/asc_get_icache_preload_status_impl.h"
#include "cache_ctrl_impl/asc_data_cache_preload_impl.h"
#include "cache_ctrl_impl/asc_dcci_impl.h"
#include "cache_ctrl_impl/asc_icache_preload_impl.h"

__aicore__ inline void asc_dcci(__gm__ void* dst, uint64_t entire)
{
    asc_dcci_impl(dst, entire);
}

__aicore__ inline void asc_dcci_all(__gm__ void* dst, uint64_t entire)
{
    asc_dcci_all_impl(dst, entire);
}

__aicore__ inline void asc_dcci_ub(__gm__ void* dst, uint64_t entire)
{
    asc_dcci_ub_impl(dst, entire);
}

__aicore__ inline void asc_dcci_out(__gm__ void* dst, uint64_t entire)
{
    asc_dcci_out_impl(dst, entire);
}

__aicore__ inline void asc_dcci_atomic(__gm__ void* dst, uint64_t entire)
{
    asc_dcci_atomic_impl(dst, entire);
}

__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
{
    asc_datacache_preload_impl(address, offset);
}

__aicore__ inline int64_t asc_get_icache_preload_status()
{
    return asc_get_icache_preload_status_impl();
}

__aicore__ inline void asc_icache_preload(const void* addr)
{
    asc_icache_preload_impl(addr);
}

__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
{
    asc_icache_preload_impl(addr, prefetch_len);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif  

