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
#warning "impl/c_api/c_api_instr_impl/vector_compute_c_api_impl/vector_compute_c_api_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif    

#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_H

#include "get_rsvd_cnt_impl/npu_arch_2201/get_rsvd_cnt_impl.h"
#include "vbrcb_impl/npu_arch_2201/asc_brcb_impl.h"
#include "vconv_c_api_impl/vconv_bfloat162float_impl/npu_arch_2201/asc_bfloat162float_impl.h"
#include "vconv_c_api_impl/vconv_bfloat162int32_impl/npu_arch_2201/asc_bfloat162int32_impl.h"
#include "vconv_c_api_impl/vconv_deqint322half_impl/npu_arch_2201/asc_deqint322half_impl.h"
#include "vconv_c_api_impl/vconv_float2float_impl/npu_arch_2201/asc_float2float_impl.h"
#include "vconv_c_api_impl/vconv_float2bfloat16_impl/npu_arch_2201/asc_float2bfloat16_impl.h"
#include "vconv_c_api_impl/vconv_float2half_impl/npu_arch_2201/asc_float2half_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_rna_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_ru_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_rd_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_rn_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_rz_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_rna_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_ru_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_rd_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_rn_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_rz_impl.h"
#include "vconv_c_api_impl/vconv_half2float_impl/npu_arch_2201/asc_half2float_impl.h"
#include "vconv_c_api_impl/vconv_half2int4_impl/npu_arch_2201/asc_half2int4_impl.h"
#include "vconv_c_api_impl/vconv_half2int16_impl/npu_arch_2201/asc_half2int16_impl.h"
#include "vconv_c_api_impl/vconv_half2int32_impl/npu_arch_2201/asc_half2int32_impl.h"
#include "vconv_c_api_impl/vconv_half2int8_impl/npu_arch_2201/asc_half2int8_impl.h"
#include "vconv_c_api_impl/vconv_vdeqs162b8_impl/npu_arch_2201/asc_vdeqs162b8h_impl.h"
#include "vconv_c_api_impl/vconv_vdeqs162b8_impl/npu_arch_2201/asc_vdeqs162b8l_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_rna_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_ru_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_rd_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_rn_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_rz_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_a_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_c_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_f_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_r_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_z_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_rna_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_ru_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_rd_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_rn_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_rz_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/npu_arch_2201/asc_int642float_rna_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/npu_arch_2201/asc_int642float_ru_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/npu_arch_2201/asc_int642float_rd_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/npu_arch_2201/asc_int642float_rn_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/npu_arch_2201/asc_int642float_rz_impl.h"
#include "vconv_c_api_impl/vconv_int42half_impl/npu_arch_2201/asc_int42half_impl.h"
#include "vconv_c_api_impl/vconv_int82half_impl/npu_arch_2201/asc_int82half_impl.h"
#include "vconv_c_api_impl/vconv_uint82half_impl/npu_arch_2201/asc_uint82half_impl.h"
#include "vconv_c_api_impl/vconv_int162float_impl/npu_arch_2201/asc_int162float_impl.h"
#include "vconv_c_api_impl/vconv_int322int16_impl/npu_arch_2201/asc_int322int16_impl.h"
#include "vconv_c_api_impl/vconv_int322int64_impl/npu_arch_2201/asc_int322int64_impl.h"
#include "vconv_c_api_impl/vconv_int642int32_impl/npu_arch_2201/asc_int642int32_impl.h"
#include "vconv_c_api_impl/vconv_deqint162b8_impl/npu_arch_2201/asc_deq_int162b8_h_impl.h"
#include "vconv_c_api_impl/vconv_deqint162b8_impl/npu_arch_2201/asc_deq_int162b8_l_impl.h"
#include "vector_dup_impl/npu_arch_2201/asc_duplicate_impl.h"
#include "vtranspose_impl/npu_arch_2201/asc_transpose_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_rna_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_ru_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_rd_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_rn_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_rz_impl.h"
#include "set_va_reg_impl/npu_arch_2201/asc_set_va_reg_impl.h"
#include "scatter_vnchwconv_impl/npu_arch_2201/asc_transto5hd_impl.h"

__aicore__ inline int64_t asc_get_rsvd_count()
{
    return asc_get_rsvd_count_impl();
}

// ==========asc_brcb(uint16_t/uint32_t)==========
__aicore__ inline void asc_brcb(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_sync_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_sync_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

// ==========asc_bfloat162float==========
__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162float_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162float_sync_impl(dst, src, count);
}

// ==========asc_bfloat162int32==========
__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rna_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_ru_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rd_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rn_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rz_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rz_sync_impl(dst, src, count);
}

// ==========asc_float2bfloat16==========
__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rn_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rna_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rd_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_ru_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rz_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rz_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rz_sync_impl(dst, src, count);
}

// ==========asc_float2float==========
__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rn_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rd_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_ru_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rna_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rz_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rz_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rz_sync_impl(dst, src, count);
}

// ==========asc_float2half==========
__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rn_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rna_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rd_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_ru_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rz_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rz_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rz_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_ro_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ro_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_ro_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ro_sync_impl(dst, src, count);
}

// ==========asc_float2int32==========
__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rna_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_ru_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rd_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rn_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rz_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rz_sync_impl(dst, src, count);
}

// ==========asc_float2int64==========
__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rna_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rna_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_ru_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_ru_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rd_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rd_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rn_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rn_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, const uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rz_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rz_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rz_sync_impl(dst, src, count);
}

// ==========asc_half2float==========
__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_impl(dst, src, count);
}

__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_sync_impl(dst, src, count);
}

// ==========asc_half2int4==========
__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rna_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_ru_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rd_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rn_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rz_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rz_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rz_sync_impl(dst, src, count);
}

// ==========asc_half2int8==========
__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rna_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_ru_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rd_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rn_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rz_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rz_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rz_sync_impl(dst, src, count);
}

// ==========asc_half2int16==========
__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rz_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rz_sync_impl(dst, src, count);
}

// ==========asc_half2int32==========
__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rz_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rz_sync_impl(dst, src, count);
}

// ==========asc_half2uint8==========
__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rna_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_ru_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rd_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rn_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rz_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rz_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rz_sync_impl(dst, src, count);
}

// ==========asc_duplicate(half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t)==========
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

// ==========asc_deq_int322half==========
__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int322half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int322half_sync(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_sync_impl(dst, src, count);
}

// ==========asc_int322float==========
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_impl(dst, src, count);
}

__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_sync_impl(dst, src, count);
}

// ==========asc_int322float_rna==========
__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rna_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_rna_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rna_sync_impl(dst, src, count);
}

// ==========asc_int322float_ru==========
__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_ru_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_ru_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_ru_sync_impl(dst, src, count);
}

// ==========asc_int322float_rd==========
__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rd_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_rd_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rd_sync_impl(dst, src, count);
}

// ==========asc_int322float_rn==========
__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rn_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_rn_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rn_sync_impl(dst, src, count);
}

// ==========asc_int322float_rz==========
__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rz_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_rz_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rz_sync_impl(dst, src, count);
}

// ==========asc_transpose(int16_t/uint16_t)==========
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_impl(dst, src);
}

__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_impl(dst, src);
}

__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

// ==========asc_int642float_rna==========
__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rna_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_rna_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rna_sync_impl(dst, src, count);
}

// ==========asc_int642float_ru==========
__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_ru_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_ru_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_ru_sync_impl(dst, src, count);
}

// ==========asc_int642float_rd==========
__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rd_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_rd_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rd_sync_impl(dst, src, count);
}

// ==========asc_int642float_rn==========
__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rn_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_rn_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rn_sync_impl(dst, src, count);
}

// ==========asc_int642float_rz==========
__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rz_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_rz_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rz_sync_impl(dst, src, count);
}

// ==========asc_vdeq_int162b8_h==========
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_sync_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_sync_impl(dst, src, count);
}

// ==========asc_vdeq_int162b8_l(int8_t/uint8_t)==========
__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_sync_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_sync_impl(dst, src, count);
}

// ==========asc_deq_int162b8==========
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_sync_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_sync_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_sync_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_sync_impl(dst, src, count);
}

// ==========asc_int42half==========
__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_int42half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count)
{
    asc_int42half_impl(dst, src, count);
}

__aicore__ inline void asc_int42half_sync(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count)
{
    asc_int42half_sync_impl(dst, src, count);
}

// ==========asc_int82half==========
__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int82half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_impl(dst, src, count);
}

__aicore__ inline void asc_int82half_sync(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_sync_impl(dst, src, count);
}

// ==========asc_uint82half==========
__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_uint82half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_impl(dst, src, count);
}

__aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_sync_impl(dst, src, count);
}

// ==========asc_int162float==========
__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_impl(dst, src, count);
}

__aicore__ inline void asc_int162float_sync(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_sync_impl(dst, src, count);
}

// ==========asc_int322int16==========
__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322int16_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_impl(dst, src, count);
}

__aicore__ inline void asc_int322int16_sync(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_sync_impl(dst, src, count);
}

// ==========asc_int322int64==========
__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322int64_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_impl(dst, src, count);
}

__aicore__ inline void asc_int322int64_sync(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_sync_impl(dst, src, count);
}

// ==========asc_int642int32==========
__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642int32_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_impl(dst, src, count);
}

__aicore__ inline void asc_int642int32_sync(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_sync_impl(dst, src, count);
}

// ==========asc_int162half==========
__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_sync_impl(dst, src, count);
}

// ==========asc_int162half_rna==========
__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rna_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_rna_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rna_sync_impl(dst, src, count);
}

// ==========asc_int162half_ru==========
__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_ru_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_ru_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_ru_sync_impl(dst, src, count);
}

// ==========asc_int162half_rd==========
__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rd_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_rd_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rd_sync_impl(dst, src, count);
}

// ==========asc_int162half_rn==========
__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rn_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_rn_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rn_sync_impl(dst, src, count);
}

// ==========asc_int162half_rz==========
__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rz_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_rz_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
   asc_int162half_rz_sync_impl(dst, src, count);
}

// ==========asc_float2int16==========
__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rna_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_ru_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rd_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rn_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rz_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rz_sync_impl(dst, src, count);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif  

