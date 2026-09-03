/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/instr_impl/npu_arch_9201/vector_datamove_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please include c_api/asc_simd.h and use public functions or variables defined in interface header files.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_DATAMOVE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_DATAMOVE_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_9201/vector_datamove_impl/asc_copy_gm2ub_align_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_datamove_impl/asc_copy_ub2gm_align_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_datamove_impl/asc_copy_ub2l1_impl.h"

// ==========asc_copy_gm2ub_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

[[deprecated("NOTICE: asc_copy_gm2ub_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_load_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

// ==========asc_copy_ub2gm_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_copy_ub2gm_align the uint8_t l2_cache_mode parameter is deprecated. "
             "Please use enum class asc_store_l2_cache_mode instead")]]
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}
//=============asc_copy_ub2l1===============
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2l1_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2l1(
    __cbuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_ub2l1_impl(dst, src, burst_count, burst_len, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2l1_sync(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2l1_sync_impl(dst, src, size);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
