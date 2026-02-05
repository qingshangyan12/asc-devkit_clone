/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_debug_utils_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_NPU_ARCH_2002_ASC_DEBUG_UTILS_H
#define IMPL_UTILS_DEBUG_NPU_ARCH_2002_ASC_DEBUG_UTILS_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS_IMPL__
#warning "asc_debug_utils_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#include "impl/utils/debug/asc_utils_macros.h"
namespace __asc_aicore {
__aicore__ inline void asc_entire_dcci_impl(__gm__ uint64_t* ptr)
{
    dcci(ptr, cache_line_t::ENTIRE_DATA_CACHE);
}

__aicore__ inline uint64_t asc_debug_get_system_cycle_impl()
{
    return 0;
}

template <uint64_t timeoutCycle>
__aicore__ inline void ringbuf_wait_rts_sync_impl()
{
    uint64_t counter = 0;
    while (counter < timeoutCycle) {
        counter++;
        __asm__ __volatile__("");
    }
}

__aicore__ inline uint32_t asc_debug_get_core_idx_impl()
{
#ifdef __ENABLE_VECTOR_CORE__
    return block_idx + get_data_main_base();
#else
    return block_idx;
#endif
}

__aicore__ inline uint64_t asc_debug_get_block_idx_impl()
{
    return static_cast<uint64_t>(asc_debug_get_core_idx_impl());
}

template<typename T>
__aicore__ inline void mem_copy_cbuf_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
    
}

template<typename T>
__aicore__ inline void mem_copy_l1buf_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
    
}

template<typename T>
__aicore__ inline void mem_copy_ub_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
    static_assert(reinterpret_cast<uint64_t>(src) % ASC_ONE_DATABLOCK_SIZE == 0, "src address shouled be 32B aligned!\n");
    // 校验len的范围
    // 溢出校验
    copy_ubuf_to_gm(reinterpret_cast<__gm__ void*>(dst), reinterpret_cast<__ubuf__ void*>(src), 0, 1, len, 0, 0);
}

} // namespace __asc_aicore
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS_IMPL__
#endif

#endif // IMPL_UTILS_DEBUG_NPU_ARCH_2002_ASC_DEBUG_UTILS_H