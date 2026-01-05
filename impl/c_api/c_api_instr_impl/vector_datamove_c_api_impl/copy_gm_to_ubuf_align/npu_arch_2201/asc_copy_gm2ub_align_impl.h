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
 * \file asc_copy_gm_to_ubuf_align_impl.h
 * \brief
 */


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#warning "c_api_instr_impl/vector_datamove_c_api_impl/copy_gm_to_ubuf_align/npu_arch_2201/asc_copy_gm2ub_align_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif    

#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_DATAMOVE_C_API_IMPL_COPY_GM_TO_UBUF_ALIGN_NPU_ARCH_2201_ASC_COPY_GM_TO_UBUF_ALIGN_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_DATAMOVE_C_API_IMPL_COPY_GM_TO_UBUF_ALIGN_NPU_ARCH_2201_ASC_COPY_GM_TO_UBUF_ALIGN_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

constexpr uint8_t ASC_DEFAULT_SID_PADDING_GAP = 0;
constexpr uint16_t ASC_DEFAULT_NBURST = 1;

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint8_t sid, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b8((__ubuf__ void*)dst, (__gm__ void*)src, sid, n_burst, len_burst, left_padding_num, right_padding_num, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_NBURST, size / ASC_ONE_DATABLOCK_SIZE, ASC_DEFAULT_SID_PADDING_GAP,
                ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_SID_PADDING_GAP);
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint8_t sid, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b16((__ubuf__ void*)dst, (__gm__ void*)src, sid, n_burst, len_burst, left_padding_num, right_padding_num, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_NBURST, size / ASC_ONE_DATABLOCK_SIZE, ASC_DEFAULT_SID_PADDING_GAP,
                ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_SID_PADDING_GAP);
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint8_t sid, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b32((__ubuf__ void*)dst, (__gm__ void*)src, sid, n_burst, len_burst, left_padding_num, right_padding_num, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_NBURST, size / ASC_ONE_DATABLOCK_SIZE, ASC_DEFAULT_SID_PADDING_GAP,
                ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_SID_PADDING_GAP, ASC_DEFAULT_SID_PADDING_GAP);
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif  

