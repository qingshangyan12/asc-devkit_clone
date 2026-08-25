/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#warning "reg_load.h is deprecated.Please use load/loadalign.h and load/loadunalign.h."

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reg_load.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_REG_LOAD_H
#define INCLUDE_C_API_REG_COMPUTE_REG_LOAD_H

#include "c_api/reg_compute/load/loadalign.h"
#include "c_api/reg_compute/load/loadunalign.h"
#include "c_api/reg_compute/gather/ub_gather.h"
#include "c_api/reg_compute/compute/reg_gather.h"
#include "c_api/reg_compute/scatter/ub_scatter.h"
#include "c_api/reg_compute/compute/reg_mask.h"
#include "c_api/composite/loadunalign_composite.h"

// ==========return-value load APIs==========
// ========== return vector data register APIs ==========
// ========== asc_load ==========
__simd_callee__ inline vector_int4x2_t asc_load(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_load(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_load(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_load(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_load(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_load(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_load(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_load(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_load(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_load(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_load(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_load(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_load(__ubuf__ bfloat16_t* src);
__simd_callee__ inline vector_int32_t asc_load(__ubuf__ int32_t* src);
__simd_callee__ inline vector_uint32_t asc_load(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_float asc_load(__ubuf__ float* src);

// ========== asc_loadalign ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_loadalign(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign(__ubuf__ bfloat16_t* src);
__simd_callee__ inline vector_int32_t asc_loadalign(__ubuf__ int32_t* src);
__simd_callee__ inline vector_uint32_t asc_loadalign(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_float asc_loadalign(__ubuf__ float* src);

// ========== asc_loadalign_brc_datablock ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_datablock(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign_brc_datablock(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign_brc_datablock(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_datablock(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_datablock(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_datablock(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_datablock(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_loadalign_brc_datablock(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_datablock(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign_brc_datablock(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_datablock(__ubuf__ bfloat16_t* src);
__simd_callee__ inline vector_int32_t asc_loadalign_brc_datablock(__ubuf__ int32_t* src);
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_datablock(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_float asc_loadalign_brc_datablock(__ubuf__ float* src);

// ========== asc_loadalign_brc_elem ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_elem(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign_brc_elem(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign_brc_elem(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_elem(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_elem(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_elem(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_elem(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign_brc_elem(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem(__ubuf__ bfloat16_t* src);
__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem(__ubuf__ int32_t* src);
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_float asc_loadalign_brc_elem(__ubuf__ float* src);

// ========== asc_loadalign_brc_elem2datablock ==========
__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem2datablock(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem2datablock(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign_brc_elem2datablock(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem2datablock(__ubuf__ bfloat16_t* src);
__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem2datablock(__ubuf__ int32_t* src);
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem2datablock(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_float asc_loadalign_brc_elem2datablock(__ubuf__ float* src);

// ========== asc_loadalign_downsample ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_downsample(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign_downsample(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign_downsample(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_downsample(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_downsample(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_downsample(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_downsample(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_downsample(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_downsample(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_loadalign_downsample(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign_downsample(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign_downsample(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_downsample(__ubuf__ bfloat16_t* src);

// ========== asc_loadalign_unpack ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign_unpack(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign_unpack(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_loadalign_unpack(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign_unpack(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign_unpack(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_unpack(__ubuf__ bfloat16_t* src);
__simd_callee__ inline vector_int32_t asc_loadalign_unpack(__ubuf__ int32_t* src);
__simd_callee__ inline vector_uint32_t asc_loadalign_unpack(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_float asc_loadalign_unpack(__ubuf__ float* src);

// ========== asc_loadalign_unpack4 ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack4(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign_unpack4(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign_unpack4(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack4(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack4(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack4(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack4(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack4(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack4(__ubuf__ fp8_e4m3fn_t* src);

// ========== asc_loadalign_upsample ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_upsample(__ubuf__ int4b_t* src);
__simd_callee__ inline vector_int8_t asc_loadalign_upsample(__ubuf__ int8_t* src);
__simd_callee__ inline vector_uint8_t asc_loadalign_upsample(__ubuf__ uint8_t* src);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_upsample(__ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_upsample(__ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_upsample(__ubuf__ hifloat8_t* src);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_upsample(__ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_upsample(__ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_upsample(__ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline vector_int16_t asc_loadalign_upsample(__ubuf__ int16_t* src);
__simd_callee__ inline vector_uint16_t asc_loadalign_upsample(__ubuf__ uint16_t* src);
__simd_callee__ inline vector_half asc_loadalign_upsample(__ubuf__ half* src);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_upsample(__ubuf__ bfloat16_t* src);

// ========== asc_loadalign_datablock_strided ==========
__simd_callee__ inline vector_int4x2_t asc_loadalign_datablock_strided(
    __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_int8_t asc_loadalign_datablock_strided(
    __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_uint8_t asc_loadalign_datablock_strided(
    __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_hifloat8_t asc_loadalign_datablock_strided(
    __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_int16_t asc_loadalign_datablock_strided(
    __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_uint16_t asc_loadalign_datablock_strided(
    __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_half asc_loadalign_datablock_strided(
    __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_bfloat16_t asc_loadalign_datablock_strided(
    __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_int32_t asc_loadalign_datablock_strided(
    __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_uint32_t asc_loadalign_datablock_strided(
    __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);
__simd_callee__ inline vector_float asc_loadalign_datablock_strided(
    __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

// ========== return mask register APIs ==========
__simd_callee__ inline vector_bool asc_loadalign_mask(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_bool asc_loadalign_mask_downsample(__ubuf__ uint32_t* src);
__simd_callee__ inline vector_bool asc_loadalign_mask_upsample(__ubuf__ uint32_t* src);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
