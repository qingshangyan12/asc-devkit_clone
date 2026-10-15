/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reg_compute/load/loadalign.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_LOAD_LOADALIGN_H
#define INCLUDE_C_API_REG_COMPUTE_LOAD_LOADALIGN_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/loadalign_intf_impl.h"
#endif

// ========== return-value load APIs ==========
// ========== return vector data register APIs ==========
// ========== asc_loadalign ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu64_PUu64
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu64_PUu64_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_int64_t& dst, __ubuf__ int64_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint64_t& dst, __ubuf__ uint64_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_half& dst0, vector_half& dst1, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_float& dst0, vector_float& dst1, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int64_t& dst, __ubuf__ int64_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_half& dst, __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_float& dst, __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int64_t& dst, __ubuf__ int64_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_half& dst, __ubuf__ half*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_float& dst, __ubuf__ float*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu64_PUu64_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvb_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvb_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvb_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_postupdate(vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int32_t& dst, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(vector_half& dst, __ubuf__ half*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(vector_float& dst, __ubuf__ float*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(vector_half& dst, __ubuf__ half*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(vector_float& dst, __ubuf__ float*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, addr_reg offset);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H
#endif
