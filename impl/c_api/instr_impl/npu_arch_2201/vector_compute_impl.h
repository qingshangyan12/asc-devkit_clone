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
#warning                                                                                                               \
    "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_2201_VECTOR_COMPUTE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_2201_VECTOR_COMPUTE_IMPL_H

#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_max_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_add_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_sub_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_min_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_leakyrelu_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_exp_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_log_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_abs_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_rcp_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_sqrt_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_rsqrt_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_relu_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_not_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_and_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_or_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_shiftleft_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_shiftright_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_shiftright_round_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_set_deq_scale_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_set_mask_count_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_set_mask_norm_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_set_vector_mask_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_lt_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_gt_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_ge_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_eq_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_ne_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_le_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_lt_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_gt_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_ge_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_eq_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_ne_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_le_scalar_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_get_cmp_mask_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_set_cmp_mask_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_select_impl.h"
#include "instr_impl/npu_arch_2201/vector_compute_impl/asc_reduce_impl.h"

// ==========asc_max_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_mul_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_add_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_sub_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_leakyrelu(half/float)==========
__aicore__ inline void asc_leakyrelu(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_leakyrelu_impl(dst, src, value, count);
}
__aicore__ inline void asc_leakyrelu(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_leakyrelu_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_leakyrelu_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_leakyrelu_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_leakyrelu(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_leakyrelu_impl(dst, src, value, count);
}
__aicore__ inline void asc_leakyrelu(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_leakyrelu_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_leakyrelu_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_leakyrelu_sync_impl(dst, src, value, count);
}

// ==========asc_exp(half/float)==========
__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_exp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
}

__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_sync_impl(dst, src, count);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_exp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
}

__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_sync_impl(dst, src, count);
}

// ==========asc_abs(half/float)==========
__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_abs_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
}

__aicore__ inline void asc_abs_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_sync_impl(dst, src, count);
}

__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_abs_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
}

__aicore__ inline void asc_abs_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_sync_impl(dst, src, count);
}

// ==========asc_rcp(half/float)==========
__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rcp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rcp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_sync_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rcp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rcp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_sync_impl(dst, src, count);
}

// ==========asc_rsqrt(half/float)==========
__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
}

__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                                 uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rsqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rsqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
}

__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
}

__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                                 uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rsqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rsqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
}

// ==========asc_sqrt(half/float)==========
__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                                uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_sync_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                                uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_sync_impl(dst, src, count);
}

// ==========asc_relu(half/float/int32_t)==========
__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                                uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                                uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
                                uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

// ==========asc_log(half/float)==========
__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_log_impl_half(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_count_impl_half(dst, src, count);
}

__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_sync_impl_half(dst, src, count);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_log_impl_float(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_count_impl_float(dst, src, count);
}

__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_sync_impl_float(dst, src, count);
}

// ==========asc_not(int16_t/uint16_t)==========
__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
}

__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_not_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_not_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_sync_impl(dst, src, count);
}

__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
}

__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat,
                               uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                               uint16_t src_repeat_stride)
{
    asc_not_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_not_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_sync_impl(dst, src, count);
}

// ==========asc_and(int16_t/uint16_t)==========
__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat,
                               uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                               uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_and_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                 src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_and_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1,
                                    uint32_t count)
{
    asc_and_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat,
                               uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                               uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_and_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                 src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_and_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1,
                                    uint32_t count)
{
    asc_and_sync_impl(dst, src0, src1, count);
}

// ==========asc_or(int16_t/uint16_t)==========
__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_or_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_or_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
                        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_or_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1,
                                   uint32_t count)
{
    asc_or_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_or_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_or_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
                         dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_or_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1,
                                   uint32_t count)
{
    asc_or_sync_impl(dst, src0, src1, count);
}

// ==========asc_shiftleft(int16_t/uint16_t/int32_t/uint32_t)==========
__aicore__ inline void asc_shiftleft(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance,
                                          uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance,
                                          uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}
__aicore__ inline void asc_shiftleft(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance,
                                          uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance,
                                          uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

// ==========asc_shiftright(int16_t/int32_t/uint16_t/uint32_t)==========
__aicore__ inline void asc_shiftright(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_shiftright_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_shiftright_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_shiftright_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_shiftright_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_shiftright_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_shiftright_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value,
                                           uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_shiftright_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_shiftright_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value,
                                           uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

// ==========asc_shiftright_round(int16_t/int32_t)==========
__aicore__ inline void asc_shiftright_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat,
                                            uint16_t dst_block_stride, uint16_t src_block_stride,
                                            uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_round_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                              src_repeat_stride);
}

__aicore__ inline void asc_shiftright_round_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value,
                                                 uint32_t count)
{
    asc_shiftright_round_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                            uint16_t dst_block_stride, uint16_t src_block_stride,
                                            uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_round_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                              src_repeat_stride);
}

__aicore__ inline void asc_shiftright_round_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value,
                                                 uint32_t count)
{
    asc_shiftright_round_sync_impl(dst, src, value, count);
}

// ==========asc_min_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                      uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                      uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                        src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_set_deq_scale(float scale, int16_t offset, bool sign_mode)
{
    asc_set_deq_scale_impl(scale, offset, sign_mode);
}

__aicore__ inline void asc_set_deq_scale(half scale)
{
    asc_set_deq_scale_impl(scale);
}

__aicore__ inline void asc_set_mask_count()
{
    asc_set_mask_count_impl();
}

__aicore__ inline void asc_set_mask_norm()
{
    asc_set_mask_norm_impl();
}

__aicore__ inline void asc_set_vector_mask(uint64_t mask1, uint64_t mask0)
{
    asc_set_vector_mask_impl(mask1, mask0);
}

// ==========asc_lt(half/float)==========
__aicore__ inline void asc_lt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_gt(half/float)==========
__aicore__ inline void asc_gt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_ge(half/float/uint16_t)==========
__aicore__ inline void asc_ge(__ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_impl(scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge_sync(__ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_sync_impl(scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge(__ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_impl(scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge_sync(__ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_sync_impl(scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge(__ubuf__ uint8_t* dst, __ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_impl(dst, scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge_sync(__ubuf__ uint8_t* dst, __ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_sync_impl(dst, scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge(__ubuf__ uint8_t* dst, __ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_impl(dst, scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge_sync(__ubuf__ uint8_t* dst, __ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, int8_t src0_repeat_stride, int8_t src1_repeat_stride)
{
    asc_ge_sync_impl(dst, scr0, scr1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_eq(half/float/uint8_t-half/uint8_t-float/uint8_t-int32_t)==========
__aicore__ inline void asc_eq(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1,
                                   uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
                                   uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
                                   uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_ne(half/float)==========
__aicore__ inline void asc_ne(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_le(half/float)==========
__aicore__ inline void asc_le(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                     src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_lt_scalar(half/float)==========
__aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src0, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src0_repeat_stride)
{
    asc_lt_scalar_impl(dst, src0, value, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride,
                       src0_repeat_stride);
}

__aicore__ inline void asc_lt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src0_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src0_repeat_stride)
{
    asc_lt_scalar_sync_impl(dst, src0, value, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride,
                            src0_repeat_stride);
}

__aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src0, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src0_repeat_stride)
{
    asc_lt_scalar_impl(dst, src0, value, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride,
                       src0_repeat_stride);
}

__aicore__ inline void asc_lt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src0_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src0_repeat_stride)
{
    asc_lt_scalar_sync_impl(dst, src0, value, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride,
                            src0_repeat_stride);
}

// ==========asc_gt_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride,
                                     uint8_t src_repeat_stride)
{
    asc_gt_scalar_impl_half(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_gt_scalar_sync_impl_half(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                                 src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride,
                                     uint8_t src_repeat_stride)
{
    asc_gt_scalar_impl_float(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                             src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_gt_scalar_sync_impl_float(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                                  src_repeat_stride);
}

// ==========asc_ge_scalar(half/float)==========
__aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

// ==========asc_eq_scalar(half/float/int32_t)==========
__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

// ==========asc_ne_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

// ==========asc_le_scalar(half/float)==========
__aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                       src_repeat_stride);
}

__aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
                            src_repeat_stride);
}

__aicore__ inline void asc_get_cmp_mask(__ubuf__ void* dst)
{
    asc_get_cmp_mask_impl(dst);
}

__aicore__ inline void asc_set_cmp_mask(__ubuf__ void* sel_mask)
{
    asc_set_cmp_mask_impl(sel_mask);
}

// ==========asc_select(half/float)==========
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                  uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                  uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_select_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                    src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                  uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                  uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_select_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
                    src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_sync_impl(dst, src0, src1, count);
}

// ==========asc_reduce(uint16_t/uint32_t)==========
__aicore__ inline void asc_reduce(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1,
                                  uint16_t dst_repeat_stride, uint8_t src_block_stride, uint8_t src_repeat_stride,
                                  uint16_t reserved, uint8_t repeat)
{
    asc_reduce_impl(dst, src0, src1, dst_repeat_stride, src_block_stride, src_repeat_stride, reserved, repeat);
}

__aicore__ inline void asc_reduce(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1,
                                  uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_reduce_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1,
                                       uint32_t count)
{
    asc_reduce_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1,
                                  uint16_t dst_repeat_stride, uint8_t src_block_stride, uint8_t src_repeat_stride,
                                  uint16_t reserved, uint8_t repeat)
{
    asc_reduce_impl(dst, src0, src1, dst_repeat_stride, src_block_stride, src_repeat_stride, reserved, repeat);
}

__aicore__ inline void asc_reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1,
                                  uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_reduce_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1,
                                       uint32_t count)
{
    asc_reduce_sync_impl(dst, src0, src1, count);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
