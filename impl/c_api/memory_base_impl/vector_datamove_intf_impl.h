/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/memory_base_impl/vector_datamove_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_intf_impl.h"

// asc_copy_ub2ub_impl.h
__aicore__ inline void asc_copy_ub2ub_impl(
    __ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_ubuf(dst, src, 0, burst_count, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2ub_impl(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_copy_gm2ub_impl.h
__aicore__ inline void asc_copy_gm2ub_impl(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf(dst, src, 0, burst_count, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_impl(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_copy_ub2gm_impl.h
__aicore__ inline void asc_copy_ub2gm_impl(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm(dst, src, 0, burst_count, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_copy_gm2ub_align_impl.h
#ifndef ASC_C_API_DEFAULT_ZERO_VALUE
#define ASC_C_API_DEFAULT_ZERO_VALUE 0
#endif

#ifndef ASC_C_API_DEFAULT_NBURST
#define ASC_C_API_DEFAULT_NBURST 1
#endif

// uint8_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// int8_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// half
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ half* dst, __gm__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// bfloat16_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// int16_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// uint16_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// uint32_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// int32_t
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

// float
__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ float* dst, __gm__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

#undef ASC_C_API_DEFAULT_ZERO_VALUE

#undef ASC_C_API_DEFAULT_NBURST

// asc_copy_ub2gm_align_impl.h
#ifndef ASC_C_API_DEFAULT_ZERO_VALUE
#define ASC_C_API_DEFAULT_ZERO_VALUE 0
#endif

#ifndef ASC_C_API_DEFAULT_NBURST
#define ASC_C_API_DEFAULT_NBURST 1
#endif

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b8(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b8(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ half* dst, __ubuf__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b8(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b16(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b16(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b16(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b32(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ float* dst, __ubuf__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b32(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b32(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ double* dst, __ubuf__ double* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b32(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ double* dst, __ubuf__ double* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int64_t* dst, __ubuf__ int64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b32(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_b32(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, left_padding_num, right_padding_num,
            src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

#undef ASC_C_API_DEFAULT_ZERO_VALUE

#undef ASC_C_API_DEFAULT_NBURST

// asc_set_copy_pad_val_impl.h
//===========asc_set_copy_pad_val(int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_set_copy_pad_val_impl(int8_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint8_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(uint8_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint8_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(int16_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(uint16_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(half pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(bfloat16_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(int32_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(uint32_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(float pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

// vector_datamove_impl.h
__aicore__ inline void asc_copy_ub2ub(
    __ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_ub2ub_impl(dst, src, burst_count, burst_len, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2ub_impl(dst, src, burst_count, burst_len, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_ub2gm_impl(dst, src, burst_count, burst_len, src_gap, dst_gap);
}

// asc_copy_gm2ub_align  int8_t / uint8_t / half / bfloat16_t / int16_t / uint16_t / float / int32_t / uint32_t
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_gm2ub_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

// asc_copy_ub2gm_align
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ double* dst, __ubuf__ double* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ double* dst, __ubuf__ double* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int64_t* dst, __ubuf__ int64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

//===========asc_set_copy_pad_val(int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_set_copy_pad_val(int8_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(uint8_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(int16_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(uint16_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(half pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(bfloat16_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(int32_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(uint32_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(float pad_value) { asc_set_copy_pad_val_impl(pad_value); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
