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
#error "compute/reg_reduce.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_REDUCE_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_REDUCE_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_REDUCE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
#include "impl/c_api/reg_base_impl/reg_reduce_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked reduce max of int16: writes the maximum to dst element 0 and the raw bit pattern of its index to element 1, zeroing the rest; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked reduce max of int32: writes the maximum to dst element 0 and the raw bit pattern of its index to element 1, zeroing the rest; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked reduce max of uint16: writes the maximum to dst element 0 and the raw bit pattern of its index to element 1, zeroing the rest; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked reduce max of uint32: writes the maximum to dst element 0 and the raw bit pattern of its index to element 1, zeroing the rest; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked reduce max of half: writes the maximum to dst element 0 and the raw bit pattern of its index to element 1, zeroing the rest; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked reduce max of float: writes the maximum to dst element 0 and the raw bit pattern of its index to element 1, zeroing the rest; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked reduce max of int16 as a vector data register: maximum in element 0, raw bit pattern of its index in element 1, rest zeroed; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int16_t asc_reduce_max(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked reduce max of int32 as a vector data register: maximum in element 0, raw bit pattern of its index in element 1, rest zeroed; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_max(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked reduce max of uint16 as a vector data register: maximum in element 0, raw bit pattern of its index in element 1, rest zeroed; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint16_t asc_reduce_max(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked reduce max of uint32 as a vector data register: maximum in element 0, raw bit pattern of its index in element 1, rest zeroed; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_max(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked reduce max of half as a vector data register: maximum in element 0, raw bit pattern of its index in element 1, rest zeroed; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_half asc_reduce_max(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked reduce max of float as a vector data register: maximum in element 0, raw bit pattern of its index in element 1, rest zeroed; ties keep the smallest index.
 *
 * @override_id asc_reduce_max__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_float asc_reduce_max(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected int16 elements in src and writes the value to dst element 0, the raw index bits to element 1, and zeros the rest.
 *
 * @override_id asc_reduce_min__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected int32 elements in src and writes the value to dst element 0, the raw index bits to element 1, and zeros the rest.
 *
 * @override_id asc_reduce_min__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected uint16 elements in src and writes the value to dst element 0, the raw index bits to element 1, and zeros the rest.
 *
 * @override_id asc_reduce_min__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected uint32 elements in src and writes the value to dst element 0, the raw index bits to element 1, and zeros the rest.
 *
 * @override_id asc_reduce_min__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected half elements in src and writes the value to dst element 0, the raw index bits to element 1, and zeros the rest.
 *
 * @override_id asc_reduce_min__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected float elements in src and writes the value to dst element 0, the raw index bits to element 1, and zeros the rest.
 *
 * @override_id asc_reduce_min__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the minimum of mask-selected int16 elements of src at element 0 and its raw index bits at element 1, other elements zero.
 *
 * @override_id asc_reduce_min__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int16_t asc_reduce_min(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the minimum of mask-selected int32 elements of src at element 0 and its raw index bits at element 1, other elements zero.
 *
 * @override_id asc_reduce_min__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_min(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the minimum of mask-selected uint16 elements of src at element 0 and its raw index bits at element 1, other elements zero.
 *
 * @override_id asc_reduce_min__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint16_t asc_reduce_min(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the minimum of mask-selected uint32 elements of src at element 0 and its raw index bits at element 1, other elements zero.
 *
 * @override_id asc_reduce_min__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_min(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the minimum of mask-selected half elements of src at element 0 and its raw index bits at element 1, other elements zero.
 *
 * @override_id asc_reduce_min__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_half asc_reduce_min(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the minimum of mask-selected float elements of src at element 0 and its raw index bits at element 1, other elements zero.
 *
 * @override_id asc_reduce_min__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_float asc_reduce_min(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected int16 elements per 32-byte DataBlock of src and writes per-block results to dst low elements, zeroing the rest.
 *
 * @override_id asc_reduce_min_datablock__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected int32 elements per 32-byte DataBlock of src and writes per-block results to dst low elements, zeroing the rest.
 *
 * @override_id asc_reduce_min_datablock__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected uint16 elements per 32-byte DataBlock of src and writes per-block results to dst low elements, zeroing the rest.
 *
 * @override_id asc_reduce_min_datablock__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected uint32 elements per 32-byte DataBlock of src and writes per-block results to dst low elements, zeroing the rest.
 *
 * @override_id asc_reduce_min_datablock__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected half elements per 32-byte DataBlock of src and writes per-block results to dst low elements, zeroing the rest.
 *
 * @override_id asc_reduce_min_datablock__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min_datablock(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: computes the minimum of mask-selected float elements per 32-byte DataBlock of src and writes per-block results to dst low elements, zeroing the rest.
 *
 * @override_id asc_reduce_min_datablock__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_min_datablock(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-32-byte-DataBlock minima of mask-selected int16 elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_min_datablock__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int16_t asc_reduce_min_datablock(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-32-byte-DataBlock minima of mask-selected int32 elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_min_datablock__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_min_datablock(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-32-byte-DataBlock minima of mask-selected uint16 elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_min_datablock__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint16_t asc_reduce_min_datablock(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-32-byte-DataBlock minima of mask-selected uint32 elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_min_datablock__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_min_datablock(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-32-byte-DataBlock minima of mask-selected half elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_min_datablock__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_half asc_reduce_min_datablock(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-32-byte-DataBlock minima of mask-selected float elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_min_datablock__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_float asc_reduce_min_datablock(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: adds adjacent pairs of mask-selected half elements of src (unselected treated as 0) and writes the sums to the low half of dst, zeroing the high half.
 *
 * @override_id asc_pair_reduce_sum__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: adds adjacent pairs of mask-selected float elements of src (unselected treated as 0) and writes the sums to the low half of dst, zeroing the high half.
 *
 * @override_id asc_pair_reduce_sum__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_pair_reduce_sum(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding sums of adjacent mask-selected half element pairs of src in its low half with the high half zeroed; unselected elements count as 0.
 *
 * @override_id asc_pair_reduce_sum__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_pair_reduce_sum(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding sums of adjacent mask-selected float element pairs of src in its low half with the high half zeroed; unselected elements count as 0.
 *
 * @override_id asc_pair_reduce_sum__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_pair_reduce_sum(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked per-DataBlock reduce max of half: each 32-byte DataBlock maximum is written to the first 8 elements of dst with the rest zeroed; no index is produced.
 *
 * @override_id asc_reduce_max_datablock__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max_datablock(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked per-DataBlock reduce max of float: each 32-byte DataBlock maximum is written to the first 8 elements of dst with the rest zeroed; no index is produced.
 *
 * @override_id asc_reduce_max_datablock__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max_datablock(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked per-DataBlock reduce max of uint16: each 32-byte DataBlock maximum is written to the first 8 elements of dst with the rest zeroed; no index is produced.
 *
 * @override_id asc_reduce_max_datablock__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked per-DataBlock reduce max of int16: each 32-byte DataBlock maximum is written to the first 8 elements of dst with the rest zeroed; no index is produced.
 *
 * @override_id asc_reduce_max_datablock__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked per-DataBlock reduce max of uint32: each 32-byte DataBlock maximum is written to the first 8 elements of dst with the rest zeroed; no index is produced.
 *
 * @override_id asc_reduce_max_datablock__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Void masked per-DataBlock reduce max of int32: each 32-byte DataBlock maximum is written to the first 8 elements of dst with the rest zeroed; no index is produced.
 *
 * @override_id asc_reduce_max_datablock__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_max_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked per-DataBlock reduce max of int16 as a vector data register: each DataBlock maximum goes to the first 8 elements of dst, rest zeroed; no index.
 *
 * @override_id asc_reduce_max_datablock__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int16_t asc_reduce_max_datablock(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked per-DataBlock reduce max of int32 as a vector data register: each DataBlock maximum goes to the first 8 elements of dst, rest zeroed; no index.
 *
 * @override_id asc_reduce_max_datablock__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_max_datablock(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked per-DataBlock reduce max of uint16 as a vector data register: each DataBlock maximum goes to the first 8 elements of dst, rest zeroed; no index.
 *
 * @override_id asc_reduce_max_datablock__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint16_t asc_reduce_max_datablock(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked per-DataBlock reduce max of uint32 as a vector data register: each DataBlock maximum goes to the first 8 elements of dst, rest zeroed; no index.
 *
 * @override_id asc_reduce_max_datablock__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_max_datablock(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked per-DataBlock reduce max of half as a vector data register: each DataBlock maximum goes to the first 8 elements of dst, rest zeroed; no index.
 *
 * @override_id asc_reduce_max_datablock__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_half asc_reduce_max_datablock(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Returns the masked per-DataBlock reduce max of float as a vector data register: each DataBlock maximum goes to the first 8 elements of dst, rest zeroed; no index.
 *
 * @override_id asc_reduce_max_datablock__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_float asc_reduce_max_datablock(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: sums mask-selected int16 elements of src via binary-tree reduction with int32 accumulation, writing the total to dst element 0 and zeroing the other elements.
 *
 * @override_id asc_reduce_sum__Rvi32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: sums mask-selected int32 elements of src via binary-tree reduction and writes the total to dst element 0, zeroing the other elements.
 *
 * @override_id asc_reduce_sum__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: sums mask-selected uint16 elements of src via binary-tree reduction with uint32 accumulation, writing the total to dst element 0 and zeroing the other elements.
 *
 * @override_id asc_reduce_sum__Rvu32_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: sums mask-selected uint32 elements of src via binary-tree reduction and writes the total to dst element 0, zeroing the other elements.
 *
 * @override_id asc_reduce_sum__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: sums mask-selected half elements of src via binary-tree reduction and writes the total to dst element 0, zeroing the other elements.
 *
 * @override_id asc_reduce_sum__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: sums mask-selected float elements of src via binary-tree reduction and writes the total to dst element 0, zeroing the other elements.
 *
 * @override_id asc_reduce_sum__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the binary-tree sum of mask-selected int16 elements of src in element 0, accumulated in int32 with other elements zeroed.
 *
 * @override_id asc_reduce_sum__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_sum(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the binary-tree sum of mask-selected int32 elements of src in element 0, with other elements zeroed.
 *
 * @override_id asc_reduce_sum__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_sum(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the binary-tree sum of mask-selected uint16 elements of src in element 0, accumulated in uint32 with other elements zeroed.
 *
 * @override_id asc_reduce_sum__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_sum(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the binary-tree sum of mask-selected uint32 elements of src in element 0, with other elements zeroed.
 *
 * @override_id asc_reduce_sum__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_sum(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the binary-tree sum of mask-selected half elements of src in element 0, with other elements zeroed.
 *
 * @override_id asc_reduce_sum__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_half asc_reduce_sum(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register holding the binary-tree sum of mask-selected float elements of src in element 0, with other elements zeroed.
 *
 * @override_id asc_reduce_sum__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_float asc_reduce_sum(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: reduces each 32-byte DataBlock of src by binary-tree summing its mask-selected half elements, writing per-block totals to dst low elements and zeroing the rest.
 *
 * @override_id asc_reduce_sum_datablock__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum_datablock(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: reduces each 32-byte DataBlock of src by binary-tree summing its mask-selected float elements, writing per-block totals to dst low elements and zeroing the rest.
 *
 * @override_id asc_reduce_sum_datablock__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum_datablock(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: reduces each 32-byte DataBlock of src by summing its mask-selected uint16 elements in uint32, writing per-block totals to dst low elements and zeroing the rest.
 *
 * @override_id asc_reduce_sum_datablock__Rvu32_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: reduces each 32-byte DataBlock of src by summing its mask-selected int16 elements in int32, writing per-block totals to dst low elements and zeroing the rest.
 *
 * @override_id asc_reduce_sum_datablock__Rvi32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: reduces each 32-byte DataBlock of src by binary-tree summing its mask-selected uint32 elements, writing per-block totals to dst low elements and zeroing the rest.
 *
 * @override_id asc_reduce_sum_datablock__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Output-parameter form: reduces each 32-byte DataBlock of src by binary-tree summing its mask-selected int32 elements, writing per-block totals to dst low elements and zeroing the rest.
 *
 * @override_id asc_reduce_sum_datablock__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold binary-tree per-DataBlock sums of mask-selected half elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_sum_datablock__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_half asc_reduce_sum_datablock(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold binary-tree per-DataBlock sums of mask-selected float elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_sum_datablock__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_float asc_reduce_sum_datablock(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-DataBlock binary-tree sums of mask-selected uint16 elements of src with uint32 accumulation, remaining elements zero.
 *
 * @override_id asc_reduce_sum_datablock__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_sum_datablock(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold per-DataBlock binary-tree sums of mask-selected int16 elements of src with int32 accumulation, remaining elements zero.
 *
 * @override_id asc_reduce_sum_datablock__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_sum_datablock(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold binary-tree per-DataBlock sums of mask-selected uint32 elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_sum_datablock__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_uint32_t asc_reduce_sum_datablock(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_reduce
 * @brief Return-value form: returns a vector register whose low elements hold binary-tree per-DataBlock sums of mask-selected int32 elements of src, remaining elements zero.
 *
 * @override_id asc_reduce_sum_datablock__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与归约。
 */
__simd_callee__ inline vector_int32_t asc_reduce_sum_datablock(vector_int32_t src, vector_bool mask);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
//=================asc_datablock_reduce_max_stride==================
__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_max_stride(
    vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_max_stride(
    vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline vector_half asc_datablock_reduce_max_stride(vector_half src, vector_bool mask);

__simd_callee__ inline vector_float asc_datablock_reduce_max_stride(vector_float src, vector_bool mask);

__simd_callee__ inline vector_uint16_t asc_datablock_reduce_max_stride(vector_uint16_t src, vector_bool mask);

__simd_callee__ inline vector_int16_t asc_datablock_reduce_max_stride(vector_int16_t src, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_max_stride(vector_uint32_t src, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_datablock_reduce_max_stride(vector_int32_t src, vector_bool mask);

//=================asc_datablock_reduce_min_stride==================
__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_min_stride(
    vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_min_stride(
    vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline vector_half asc_datablock_reduce_min_stride(vector_half src, vector_bool mask);

__simd_callee__ inline vector_float asc_datablock_reduce_min_stride(vector_float src, vector_bool mask);

__simd_callee__ inline vector_uint16_t asc_datablock_reduce_min_stride(vector_uint16_t src, vector_bool mask);

__simd_callee__ inline vector_int16_t asc_datablock_reduce_min_stride(vector_int16_t src, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_min_stride(vector_uint32_t src, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_datablock_reduce_min_stride(vector_int32_t src, vector_bool mask);

//=================asc_datablock_reduce_sum_stride==================
__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_sum_stride(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_sum_stride(
    vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline vector_half asc_datablock_reduce_sum_stride(vector_half src, vector_bool mask);

__simd_callee__ inline vector_float asc_datablock_reduce_sum_stride(vector_float src, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_sum_stride(vector_uint16_t src, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_datablock_reduce_sum_stride(vector_int16_t src, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_sum_stride(vector_uint32_t src, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_datablock_reduce_sum_stride(vector_int32_t src, vector_bool mask);

//=================asc_pair_reduce_sum_stride==================
__simd_callee__ inline void asc_pair_reduce_sum_stride(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_pair_reduce_sum_stride(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline vector_half asc_pair_reduce_sum_stride(vector_half src, vector_bool mask);

__simd_callee__ inline vector_float asc_pair_reduce_sum_stride(vector_float src, vector_bool mask);

//=================asc_reduce_max_last_elem==================
__simd_callee__ inline void asc_reduce_max_last_elem(vector_half& dst, vector_half src);

__simd_callee__ inline void asc_reduce_max_last_elem(vector_float& dst, vector_float src);

__simd_callee__ inline void asc_reduce_max_last_elem(vector_uint16_t& dst, vector_uint16_t src);

__simd_callee__ inline void asc_reduce_max_last_elem(vector_int16_t& dst, vector_int16_t src);

__simd_callee__ inline void asc_reduce_max_last_elem(vector_uint32_t& dst, vector_uint32_t src);

__simd_callee__ inline void asc_reduce_max_last_elem(vector_int32_t& dst, vector_int32_t src);

__simd_callee__ inline vector_half asc_reduce_max_last_elem(vector_half src);

__simd_callee__ inline vector_float asc_reduce_max_last_elem(vector_float src);

__simd_callee__ inline vector_uint16_t asc_reduce_max_last_elem(vector_uint16_t src);

__simd_callee__ inline vector_int16_t asc_reduce_max_last_elem(vector_int16_t src);

__simd_callee__ inline vector_uint32_t asc_reduce_max_last_elem(vector_uint32_t src);

__simd_callee__ inline vector_int32_t asc_reduce_max_last_elem(vector_int32_t src);

//=================asc_reduce_min_last_elem==================
__simd_callee__ inline void asc_reduce_min_last_elem(vector_half& dst, vector_half src);

__simd_callee__ inline void asc_reduce_min_last_elem(vector_float& dst, vector_float src);

__simd_callee__ inline void asc_reduce_min_last_elem(vector_uint16_t& dst, vector_uint16_t src);

__simd_callee__ inline void asc_reduce_min_last_elem(vector_int16_t& dst, vector_int16_t src);

__simd_callee__ inline void asc_reduce_min_last_elem(vector_uint32_t& dst, vector_uint32_t src);

__simd_callee__ inline void asc_reduce_min_last_elem(vector_int32_t& dst, vector_int32_t src);

__simd_callee__ inline vector_half asc_reduce_min_last_elem(vector_half src);

__simd_callee__ inline vector_float asc_reduce_min_last_elem(vector_float src);

__simd_callee__ inline vector_uint16_t asc_reduce_min_last_elem(vector_uint16_t src);

__simd_callee__ inline vector_int16_t asc_reduce_min_last_elem(vector_int16_t src);

__simd_callee__ inline vector_uint32_t asc_reduce_min_last_elem(vector_uint32_t src);

__simd_callee__ inline vector_int32_t asc_reduce_min_last_elem(vector_int32_t src);

//=================asc_reduce_sum_last_elem==================
__simd_callee__ inline void asc_reduce_sum_last_elem(vector_int32_t& dst, vector_int32_t src);

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_uint32_t& dst, vector_uint32_t src);

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_half& dst, vector_half src);

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_float& dst, vector_float src);

__simd_callee__ inline vector_int32_t asc_reduce_sum_last_elem(vector_int32_t src);

__simd_callee__ inline vector_uint32_t asc_reduce_sum_last_elem(vector_uint32_t src);

__simd_callee__ inline vector_half asc_reduce_sum_last_elem(vector_half src);

__simd_callee__ inline vector_float asc_reduce_sum_last_elem(vector_float src);
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_REDUCE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_REDUCE_H
#endif
