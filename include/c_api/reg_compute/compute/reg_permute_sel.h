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
#error "compute/reg_permute_sel.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_PERMUTE_SEL_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_PERMUTE_SEL_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_PERMUTE_SEL_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
#include "impl/c_api/reg_base_impl/reg_permute_sel_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_bool: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvb_vb_vb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_uint8_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_int8_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_fp8_e4m3fn_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvf8e4m3_vf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_hifloat8_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvhf8_vhf8_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(
    vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_fp8_e5m2_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvf8e5m2_vf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_uint16_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_int16_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_half: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_bfloat16_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_uint32_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_int32_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_float: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, writing the result to dst.
 *
 * @override_id asc_select__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器或矢量数据寄存器）。
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline void asc_select(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_bool: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vb_vb_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_bool asc_select(vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_uint8_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_uint8_t asc_select(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_int8_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_int8_t asc_select(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_fp8_e4m3fn_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_select(
    vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_hifloat8_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vhf8_vhf8_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_hifloat8_t asc_select(vector_hifloat8_t src0, vector_hifloat8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_fp8_e5m2_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_select(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_uint16_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_uint16_t asc_select(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_int16_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_int16_t asc_select(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_half: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_half asc_select(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_bfloat16_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_bfloat16_t asc_select(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_uint32_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_uint32_t asc_select(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_int32_t: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_int32_t asc_select(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_float: selects each element from src0 where the mask bit is 1 and from src1 where it is 0, returning the result vector.
 *
 * @override_id asc_select__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数0（掩码寄存器或矢量数据寄存器）。
 * @param [in] src1 源操作数1（掩码寄存器或矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。指定选择`src0`或`src1`为有效数据。`mask`的比特位为1时，选取`src0`；`mask`的比特位为0时，选取`src1`。
 */
__simd_callee__ inline vector_float asc_select(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_uint8_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_uint8_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vu8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_squeeze(vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_int8_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_int8_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_squeeze(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_uint16_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_uint16_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_squeeze(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_int16_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_int16_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_squeeze(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_uint32_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_uint32_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_squeeze(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_int32_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_int32_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_squeeze(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_half: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_half: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_squeeze(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_float: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_float: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_squeeze(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_bfloat16_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_bfloat16_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_squeeze(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_hifloat8_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvhf8_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_hifloat8_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vhf8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_hifloat8_t asc_squeeze(vector_hifloat8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_fp8_e4m3fn_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_fp8_e4m3fn_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vf8e4m3_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_squeeze(vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_fp8_e5m2_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_fp8_e5m2_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vf8e5m2_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_squeeze(vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_fp8_e8m0_t: compacts the src elements selected by mask into dst, laid out contiguously from the low end with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_squeeze(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_fp8_e8m0_t: returns the src elements selected by mask compacted contiguously from the low end, with the remaining positions zeroed.
 *
 * @override_id asc_squeeze__vf8e8m0_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_squeeze(vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Clears the AR special register; call once before each group of asc_squeeze_and_storeunalign contiguous store operations to reset the byte offset.
 *
 * @override_id asc_squeeze_and_storeunalign_init__z
 * @archs 3510
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_init();

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUi8_Rvi8_Rvstoreunalign_vi8_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ int8_t* dst_addr, vector_int8_t& dst, vector_store_unalign& src0, vector_int8_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUu8_Rvu8_Rvstoreunalign_vu8_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ uint8_t* dst_addr, vector_uint8_t& dst, vector_store_unalign& src0, vector_uint8_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUi16_Rvi16_Rvstoreunalign_vi16_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ int16_t* dst_addr, vector_int16_t& dst, vector_store_unalign& src0, vector_int16_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUu16_Rvu16_Rvstoreunalign_vu16_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ uint16_t* dst_addr, vector_uint16_t& dst, vector_store_unalign& src0, vector_uint16_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUi32_Rvi32_Rvstoreunalign_vi32_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ int32_t* dst_addr, vector_int32_t& dst, vector_store_unalign& src0, vector_int32_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUu32_Rvu32_Rvstoreunalign_vu32_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ uint32_t* dst_addr, vector_uint32_t& dst, vector_store_unalign& src0, vector_uint32_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf16_Rvf16_Rvstoreunalign_vf16_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ half* dst_addr, vector_half& dst, vector_store_unalign& src0, vector_half src1, vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf32_Rvf32_Rvstoreunalign_vf32_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ float* dst_addr, vector_float& dst, vector_store_unalign& src0, vector_float src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUbf16_Rvbf16_Rvstoreunalign_vbf16_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ bfloat16_t* dst_addr, vector_bfloat16_t& dst, vector_store_unalign& src0, vector_bfloat16_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf8e4m3_Rvf8e4m3_Rvstoreunalign_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ fp8_e4m3fn_t* dst_addr, vector_fp8_e4m3fn_t& dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf8e5m2_Rvf8e5m2_Rvstoreunalign_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ fp8_e5m2_t* dst_addr, vector_fp8_e5m2_t& dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf8e8m0_Rvf8e8m0_Rvstoreunalign_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ fp8_e8m0_t* dst_addr, vector_fp8_e8m0_t& dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf4e2m1x2_Rvf4e2m1x2_Rvstoreunalign_vf4e2m1x2_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ fp4x2_e2m1_t* dst_addr, vector_fp4x2_e2m1_t& dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void: compresses the src1 elements selected by squeeze_mask into dst and stores them contiguously to UB at dst_addr, buffering the sub-32B tail in src0.
 *
 * @override_id asc_squeeze_and_storeunalign__PUf4e1m2x2_Rvf4e1m2x2_Rvstoreunalign_vf4e1m2x2_vb
 * @archs 3510
 * @param [out] dst_addr 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐。连续搬出时，实际写入位置为`dst_addr`加AR寄存器记录的字节偏移。接口不会修改`dst_addr`的值。
 * @param [out] dst 目的操作数（矢量数据寄存器），通过引用返回压缩结果。被选中的元素连续排列在低位，剩余元素置0。
 * @param [in,out] src0 非对齐寄存器，长度为32B，用于暂存未满32B的尾块。连续搬出及收尾时需使用同一个寄存器；首次调用无需预先初始化该寄存器。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] squeeze_mask 源操作数掩码（掩码寄存器）。对应位置为1时选择该元素，为0时不选择该元素。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ fp4x2_e1m2_t* dst_addr, vector_fp4x2_e1m2_t& dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1,
    vector_bool squeeze_mask);

__simd_callee__ inline void asc_squeeze_and_storeunalign(
    __ubuf__ int4b_t* dst_addr, vector_int4x2_t& dst, vector_store_unalign& src0, vector_int4x2_t src1,
    vector_bool squeeze_mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUi8_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ int8_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUu8_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ uint8_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf4e2m1x2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf4e1m2x2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUi4x2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ int4b_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf8e8m0_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf8e5m2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf8e4m3_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUi16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ int16_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUu16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ uint16_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ half* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUbf16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ bfloat16_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUi32_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ int32_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUu32_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ uint32_t* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void finalizer for a group of asc_squeeze_and_storeunalign calls: writes the 32B tail block held in the unaligned register src to UB at dst; dst/src must match the main calls.
 *
 * @override_id asc_squeeze_and_storeunalign_finalize__PUf32_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的起始地址。地址不要求32B对齐，但需按`sizeof(dtype)`字节对齐；需与同组`asc_squeeze_and_storeunalign`调用中的`dst_addr`保持一致。
 * @param [in] src 非对齐寄存器，长度为32B，保存待写入UB的尾块；需与同组`asc_squeeze_and_storeunalign`调用中的`src0`使用同一个寄存器。
 */
__simd_callee__ inline void asc_squeeze_and_storeunalign_finalize(__ubuf__ float* dst, vector_store_unalign src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form: interleaves src0/src1 mask bits in 1-bit groups, writing the interleaved results to dst0 and dst1.
 *
 * @override_id asc_intlv_b8__Rvb_Rvb_vb_vb
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv_b8(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form: interleaves src0/src1 mask bits in 2-bit groups, writing the interleaved results to dst0 and dst1.
 *
 * @override_id asc_intlv_b16__Rvb_Rvb_vb_vb
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv_b16(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form: interleaves src0/src1 mask bits in 4-bit groups, writing the interleaved results to dst0 and dst1.
 *
 * @override_id asc_intlv_b32__Rvb_Rvb_vb_vb
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv_b32(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_uint8_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvu8_Rvu8_vu8_vu8
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_int8_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvi8_Rvi8_vi8_vi8
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(vector_int8_t& dst0, vector_int8_t& dst1, vector_int8_t src0, vector_int8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_uint16_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvu16_Rvu16_vu16_vu16
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, vector_uint16_t src0, vector_uint16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_int16_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvi16_Rvi16_vi16_vi16
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_int16_t& dst0, vector_int16_t& dst1, vector_int16_t src0, vector_int16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_uint32_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvu32_Rvu32_vu32_vu32
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_int32_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvi32_Rvi32_vi32_vi32
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_half: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvf16_Rvf16_vf16_vf16
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(vector_half& dst0, vector_half& dst1, vector_half src0, vector_half src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_float: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvf32_Rvf32_vf32_vf32
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(vector_float& dst0, vector_float& dst1, vector_float src0, vector_float src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_fp8_e8m0_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvf8e8m0_Rvf8e8m0_vf8e8m0_vf8e8m0
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_fp8_e5m2_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvf8e5m2_Rvf8e5m2_vf8e5m2_vf8e5m2
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_fp8_e4m3fn_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvf8e4m3_Rvf8e4m3_vf8e4m3_vf8e4m3
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_hifloat8_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvhf8_Rvhf8_vhf8_vhf8
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, vector_hifloat8_t src0, vector_hifloat8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_bfloat16_t: interleaves src0/src1 alternately element by element, front halves into dst0 and back halves into dst1.
 *
 * @override_id asc_intlv__Rvbf16_Rvbf16_vbf16_vbf16
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器），用于保存交织后的低半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器），用于保存交织后的高半部分数据。数据类型须与`src0`、`src1`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 */
__simd_callee__ inline void asc_intlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, vector_bfloat16_t src0, vector_bfloat16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the mask prefix popcount into vector_uint8_t dst in place: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze__Rvu8_vb
 * @archs 3510
 * @param [in,out] dst 源和目的矢量数据寄存器，仅无返回值类型接口包含该参数，接口在掩码前缀计数过程中写入计数结果。
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline void asc_unsqueeze(vector_uint8_t& dst, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the mask prefix popcount into vector_int8_t dst in place: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze__Rvi8_vb
 * @archs 3510
 * @param [in,out] dst 源和目的矢量数据寄存器，仅无返回值类型接口包含该参数，接口在掩码前缀计数过程中写入计数结果。
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline void asc_unsqueeze(vector_int8_t& dst, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the mask prefix popcount into vector_uint16_t dst in place: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze__Rvu16_vb
 * @archs 3510
 * @param [in,out] dst 源和目的矢量数据寄存器，仅无返回值类型接口包含该参数，接口在掩码前缀计数过程中写入计数结果。
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline void asc_unsqueeze(vector_uint16_t& dst, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the mask prefix popcount into vector_int16_t dst in place: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze__Rvi16_vb
 * @archs 3510
 * @param [in,out] dst 源和目的矢量数据寄存器，仅无返回值类型接口包含该参数，接口在掩码前缀计数过程中写入计数结果。
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline void asc_unsqueeze(vector_int16_t& dst, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the mask prefix popcount into vector_uint32_t dst in place: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze__Rvu32_vb
 * @archs 3510
 * @param [in,out] dst 源和目的矢量数据寄存器，仅无返回值类型接口包含该参数，接口在掩码前缀计数过程中写入计数结果。
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline void asc_unsqueeze(vector_uint32_t& dst, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the mask prefix popcount into vector_int32_t dst in place: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze__Rvi32_vb
 * @archs 3510
 * @param [in,out] dst 源和目的矢量数据寄存器，仅无返回值类型接口包含该参数，接口在掩码前缀计数过程中写入计数结果。
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline void asc_unsqueeze(vector_int32_t& dst, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns the mask prefix popcount as vector_uint8_t: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze_u8__vb
 * @archs 3510
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline vector_uint8_t asc_unsqueeze_u8(vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns the mask prefix popcount as vector_int8_t: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze_s8__vb
 * @archs 3510
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline vector_int8_t asc_unsqueeze_s8(vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns the mask prefix popcount as vector_uint16_t: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze_u16__vb
 * @archs 3510
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline vector_uint16_t asc_unsqueeze_u16(vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns the mask prefix popcount as vector_int16_t: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze_s16__vb
 * @archs 3510
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline vector_int16_t asc_unsqueeze_s16(vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns the mask prefix popcount as vector_uint32_t: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze_u32__vb
 * @archs 3510
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline vector_uint32_t asc_unsqueeze_u32(vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns the mask prefix popcount as vector_int32_t: element 0 is 0 and element i counts 1s in mask bits 0..i-1, ignoring the highest mask bit.
 *
 * @override_id asc_unsqueeze_s32__vb
 * @archs 3510
 * @param [in] mask 源掩码寄存器，用于提供dst解压缩信息。
 */
__simd_callee__ inline vector_int32_t asc_unsqueeze_s32(vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 8 bits (b16) of vector_uint16_t src into the low half of vector_uint8_t dst.
 *
 * @override_id asc_pack_to_low__Rvu8_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_low(vector_uint8_t& dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 8 bits (b16) of vector_int16_t src into the low half of vector_uint8_t dst.
 *
 * @override_id asc_pack_to_low__Rvu8_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_low(vector_uint8_t& dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 16 bits (b32) of vector_uint32_t src into the low half of vector_uint16_t dst.
 *
 * @override_id asc_pack_to_low__Rvu16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_low(vector_uint16_t& dst, vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 16 bits (b32) of vector_int32_t src into the low half of vector_uint16_t dst.
 *
 * @override_id asc_pack_to_low__Rvu16_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_low(vector_uint16_t& dst, vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Packs vector_bool src bitwise into the low half of dst, storing the lower bit of each 2-bit group.
 *
 * @override_id asc_pack_to_low__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_low(vector_bool& dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 8 bits (b16) of vector_uint16_t src into the high half of vector_uint8_t dst.
 *
 * @override_id asc_pack_to_high__Rvu8_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_high(vector_uint8_t& dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 8 bits (b16) of vector_int16_t src into the high half of vector_uint8_t dst.
 *
 * @override_id asc_pack_to_high__Rvu8_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_high(vector_uint8_t& dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 16 bits (b32) of vector_uint32_t src into the high half of vector_uint16_t dst.
 *
 * @override_id asc_pack_to_high__Rvu16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_high(vector_uint16_t& dst, vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Writes the low 16 bits (b32) of vector_int32_t src into the high half of vector_uint16_t dst.
 *
 * @override_id asc_pack_to_high__Rvu16_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_high(vector_uint16_t& dst, vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Packs vector_bool src bitwise into the high half of dst, storing the lower bit of each 2-bit group.
 *
 * @override_id asc_pack_to_high__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_to_high(vector_bool& dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint8_t whose low half holds the packed low 8 bits (b16) of each vector_uint16_t src element.
 *
 * @override_id asc_pack_to_low__vu16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint8_t asc_pack_to_low(vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint8_t whose low half holds the packed low 8 bits (b16) of each vector_int16_t src element.
 *
 * @override_id asc_pack_to_low__vi16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint8_t asc_pack_to_low(vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint16_t whose low half holds the packed low 16 bits (b32) of each vector_uint32_t src element.
 *
 * @override_id asc_pack_to_low__vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint16_t asc_pack_to_low(vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint16_t whose low half holds the packed low 16 bits (b32) of each vector_int32_t src element.
 *
 * @override_id asc_pack_to_low__vi32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint16_t asc_pack_to_low(vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_bool whose low half holds the bitwise-packed result of src.
 *
 * @override_id asc_pack_to_low__vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_bool asc_pack_to_low(vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint8_t whose high half holds the packed low 8 bits (b16) of each vector_uint16_t src element.
 *
 * @override_id asc_pack_to_high__vu16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint8_t asc_pack_to_high(vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint8_t whose high half holds the packed low 8 bits (b16) of each vector_int16_t src element.
 *
 * @override_id asc_pack_to_high__vi16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint8_t asc_pack_to_high(vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint16_t whose high half holds the packed low 16 bits (b32) of each vector_uint32_t src element.
 *
 * @override_id asc_pack_to_high__vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint16_t asc_pack_to_high(vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_uint16_t whose high half holds the packed low 16 bits (b32) of each vector_int32_t src element.
 *
 * @override_id asc_pack_to_high__vi32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_uint16_t asc_pack_to_high(vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Returns a vector_bool whose high half holds the bitwise-packed result of src.
 *
 * @override_id asc_pack_to_high__vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline vector_bool asc_pack_to_high(vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_int32_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvi32_Rvi32_vi32_vi32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_uint32_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvu32_Rvu32_vu32_vu32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_int16_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvi16_Rvi16_vi16_vi16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, vector_int16_t src0, vector_int16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_uint16_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvu16_Rvu16_vu16_vu16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, vector_uint16_t src0, vector_uint16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_int8_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvi8_Rvi8_vi8_vi8
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, vector_int8_t src0, vector_int8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_uint8_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvu8_Rvu8_vu8_vu8
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_fp8_e4m3fn_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvf8e4m3_Rvf8e4m3_vf8e4m3_vf8e4m3
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_hifloat8_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvhf8_Rvhf8_vhf8_vhf8
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, vector_hifloat8_t src0, vector_hifloat8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_fp8_e5m2_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvf8e5m2_Rvf8e5m2_vf8e5m2_vf8e5m2
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_fp8_e8m0_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvf8e8m0_Rvf8e8m0_vf8e8m0_vf8e8m0
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_bfloat16_t: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvbf16_Rvbf16_vbf16_vbf16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, vector_bfloat16_t src0, vector_bfloat16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_float: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvf32_Rvf32_vf32_vf32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(vector_float& dst0, vector_float& dst1, vector_float src0, vector_float src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form for vector_half: deinterleaves src0/src1 elements, even indices into dst0 and odd indices into dst1, each dst combining front and back halves.
 *
 * @override_id asc_deintlv__Rvf16_Rvf16_vf16_vf16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv(vector_half& dst0, vector_half& dst1, vector_half src0, vector_half src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form: deinterleaves src0/src1 mask bits at the bit width given by the b8 suffix, even-index bits into dst0 and odd-index bits into dst1.
 *
 * @override_id asc_deintlv_b8__Rvb_Rvb_vb_vb
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv_b8(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form: deinterleaves src0/src1 mask bits at the bit width given by the b16 suffix, even-index bits into dst0 and odd-index bits into dst1.
 *
 * @override_id asc_deintlv_b16__Rvb_Rvb_vb_vb
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv_b16(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Void output-parameter form: deinterleaves src0/src1 mask bits at the bit width given by the b32 suffix, even-index bits into dst0 and odd-index bits into dst1.
 *
 * @override_id asc_deintlv_b32__Rvb_Rvb_vb_vb
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器，写入解交织后的偶数索引元素集合。dtype须与dst1、src0、src1一致。
 * @param [out] dst1 目的矢量数据寄存器，写入解交织后的奇数索引元素集合。dtype须与dst0、src0、src1一致。
 * @param [in] src0 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0前半部分，奇数索引元素写入dst1前半部分。
 * @param [in] src1 源矢量数据寄存器，参与解交织的操作数。dtype须与dst0一致。其偶数索引元素写入dst0后半部分，奇数索引元素写入dst1后半部分。
 */
__simd_callee__ inline void asc_deintlv_b32(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_uint8_t to vector_uint16_t: takes the upper half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_upper__Rvu16_vu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_upper(vector_uint16_t& dst, vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_uint8_t to vector_uint16_t: takes the lower half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_lower__Rvu16_vu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_lower(vector_uint16_t& dst, vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_int8_t to vector_int16_t: takes the upper half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_upper__Rvi16_vi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_upper(vector_int16_t& dst, vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_int8_t to vector_int16_t: takes the lower half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_lower__Rvi16_vi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_lower(vector_int16_t& dst, vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_uint16_t to vector_uint32_t: takes the upper half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_upper__Rvu32_vu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_upper(vector_uint32_t& dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_uint16_t to vector_uint32_t: takes the lower half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_lower__Rvu32_vu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_lower(vector_uint32_t& dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_int16_t to vector_int32_t: takes the upper half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_upper__Rvi32_vi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_upper(vector_int32_t& dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form widening vector_int16_t to vector_int32_t: takes the lower half of src, extends each element (unsigned zero-fill, signed sign-preserve) and writes it to dst.
 *
 * @override_id asc_unpack_lower__Rvi32_vi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_lower(vector_int32_t& dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_bool masks: takes the upper half of src and writes each bit expanded to 2 bits with the high bit zero-filled into dst.
 *
 * @override_id asc_unpack_upper__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_upper(vector_bool& dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Output-parameter form for vector_bool masks: takes the lower half of src and writes each bit expanded to 2 bits with the high bit zero-filled into dst.
 *
 * @override_id asc_unpack_lower__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，读取src低半段或高半段元素，扩展位宽后写入dst全部有效位置，dst全部有效位置均被改写。
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline void asc_unpack_lower(vector_bool& dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the upper half of a vector_uint8_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_upper__vu8
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_uint16_t asc_unpack_upper(vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the lower half of a vector_uint8_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_lower__vu8
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_uint16_t asc_unpack_lower(vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the upper half of a vector_int8_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_upper__vi8
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_int16_t asc_unpack_upper(vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the lower half of a vector_int8_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_lower__vi8
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_int16_t asc_unpack_lower(vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the upper half of a vector_uint16_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_upper__vu16
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_uint32_t asc_unpack_upper(vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the lower half of a vector_uint16_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_lower__vu16
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_uint32_t asc_unpack_lower(vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the upper half of a vector_int16_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_upper__vi16
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_int32_t asc_unpack_upper(vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form: takes the lower half of a vector_int16_t source and returns each element widened (unsigned zero-fill, signed sign-preserve).
 *
 * @override_id asc_unpack_lower__vi16
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_int32_t asc_unpack_lower(vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_bool masks: takes the upper half of src and returns each bit expanded to 2 bits with the high bit zero-filled.
 *
 * @override_id asc_unpack_upper__vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_bool asc_unpack_upper(vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_permute_sel
 * @brief Return-value form for vector_bool masks: takes the lower half of src and returns each bit expanded to 2 bits with the high bit zero-filled.
 *
 * @override_id asc_unpack_lower__vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，取src低半段或高半段元素扩展位宽写入dst。
 */
__simd_callee__ inline vector_bool asc_unpack_lower(vector_bool src);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
//=================asc_intlv_half (for 9201)==================
__simd_callee__ inline void asc_intlv_half_low_b8(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_half_low_b16(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_half_low_b32(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_half_high_b8(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_half_high_b16(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_half_high_b32(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_intlv_half_low_b8(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_intlv_half_low_b16(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_intlv_half_low_b32(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_intlv_half_high_b8(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_intlv_half_high_b16(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_intlv_half_high_b32(vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_half_low(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_half& dst, vector_half src0, vector_half src1);

__simd_callee__ inline void asc_intlv_half_low(vector_float& dst, vector_float src0, vector_float src1);

__simd_callee__ inline void asc_intlv_half_low(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_fp8_e6m2_t& dst, vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_intlv_half_low(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline vector_uint8_t asc_intlv_half_low(vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline vector_int8_t asc_intlv_half_low(vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline vector_uint16_t asc_intlv_half_low(vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline vector_int16_t asc_intlv_half_low(vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline vector_uint32_t asc_intlv_half_low(vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline vector_int32_t asc_intlv_half_low(vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline vector_half asc_intlv_half_low(vector_half src0, vector_half src1);

__simd_callee__ inline vector_float asc_intlv_half_low(vector_float src0, vector_float src1);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_intlv_half_low(vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline vector_fp8_e6m2_t asc_intlv_half_low(vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline vector_hifloat8_t asc_intlv_half_low(vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline vector_fp8_e5m2_t asc_intlv_half_low(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline vector_fp8_e8m0_t asc_intlv_half_low(vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline vector_bfloat16_t asc_intlv_half_low(vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_half& dst, vector_half src0, vector_half src1);

__simd_callee__ inline void asc_intlv_half_high(vector_float& dst, vector_float src0, vector_float src1);

__simd_callee__ inline void asc_intlv_half_high(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_fp8_e6m2_t& dst, vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_intlv_half_high(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline vector_uint8_t asc_intlv_half_high(vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline vector_int8_t asc_intlv_half_high(vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline vector_uint16_t asc_intlv_half_high(vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline vector_int16_t asc_intlv_half_high(vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline vector_uint32_t asc_intlv_half_high(vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline vector_int32_t asc_intlv_half_high(vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline vector_half asc_intlv_half_high(vector_half src0, vector_half src1);

__simd_callee__ inline vector_float asc_intlv_half_high(vector_float src0, vector_float src1);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_intlv_half_high(vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline vector_fp8_e6m2_t asc_intlv_half_high(vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline vector_hifloat8_t asc_intlv_half_high(vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline vector_fp8_e5m2_t asc_intlv_half_high(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline vector_fp8_e8m0_t asc_intlv_half_high(vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline vector_bfloat16_t asc_intlv_half_high(vector_bfloat16_t src0, vector_bfloat16_t src1);

//=================asc_deintlv_half (for 9201)==================
__simd_callee__ inline void asc_deintlv_even_b8(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_even_b16(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_even_b32(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_odd_b8(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_odd_b16(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_odd_b32(vector_bool& dst, vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_deintlv_even_b8(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_deintlv_even_b16(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_deintlv_even_b32(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_deintlv_odd_b8(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_deintlv_odd_b16(vector_bool src0, vector_bool src1);

__simd_callee__ inline vector_bool asc_deintlv_odd_b32(vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_even(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_half& dst, vector_half src0, vector_half src1);

__simd_callee__ inline void asc_deintlv_even(vector_float& dst, vector_float src0, vector_float src1);

__simd_callee__ inline void asc_deintlv_even(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_fp8_e6m2_t& dst, vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_deintlv_even(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline vector_uint8_t asc_deintlv_even(vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline vector_int8_t asc_deintlv_even(vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline vector_uint16_t asc_deintlv_even(vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline vector_int16_t asc_deintlv_even(vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline vector_uint32_t asc_deintlv_even(vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline vector_int32_t asc_deintlv_even(vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline vector_half asc_deintlv_even(vector_half src0, vector_half src1);

__simd_callee__ inline vector_float asc_deintlv_even(vector_float src0, vector_float src1);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_deintlv_even(vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline vector_fp8_e6m2_t asc_deintlv_even(vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline vector_hifloat8_t asc_deintlv_even(vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline vector_fp8_e5m2_t asc_deintlv_even(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline vector_fp8_e8m0_t asc_deintlv_even(vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline vector_bfloat16_t asc_deintlv_even(vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_half& dst, vector_half src0, vector_half src1);

__simd_callee__ inline void asc_deintlv_odd(vector_float& dst, vector_float src0, vector_float src1);

__simd_callee__ inline void asc_deintlv_odd(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_fp8_e6m2_t& dst, vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_deintlv_odd(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline vector_uint8_t asc_deintlv_odd(vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline vector_int8_t asc_deintlv_odd(vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline vector_uint16_t asc_deintlv_odd(vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline vector_int16_t asc_deintlv_odd(vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline vector_uint32_t asc_deintlv_odd(vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline vector_int32_t asc_deintlv_odd(vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline vector_half asc_deintlv_odd(vector_half src0, vector_half src1);

__simd_callee__ inline vector_float asc_deintlv_odd(vector_float src0, vector_float src1);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_deintlv_odd(vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline vector_fp8_e6m2_t asc_deintlv_odd(vector_fp8_e6m2_t src0, vector_fp8_e6m2_t src1);

__simd_callee__ inline vector_hifloat8_t asc_deintlv_odd(vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline vector_fp8_e5m2_t asc_deintlv_odd(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline vector_fp8_e8m0_t asc_deintlv_odd(vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline vector_bfloat16_t asc_deintlv_odd(vector_bfloat16_t src0, vector_bfloat16_t src1);
#endif

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_uint8_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_int8_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_uint16_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_int16_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_uint32_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_int32_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_half elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_half& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_float elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_float& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_bfloat16_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_hifloat8_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvhf8_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_fp8_e4m3fn_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_fp8_e5m2_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Copies mask-selected vector_fp8_e8m0_t elements from src into dst contiguously, zero-fills the remaining positions, and saves the valid byte count in the AR register (deprecated).
 *
 * @override_id asc_squeeze_with_status__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_uint8_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_int8_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_uint16_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_int16_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_uint32_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_int32_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_half: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_half& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_float: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_float& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_bfloat16_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_hifloat8_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvhf8_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_fp8_e4m3fn_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_fp8_e5m2_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void form for vector_fp8_e8m0_t: copies the elements of src selected by mask contiguously into dst with the remaining positions zeroed; use asc_squeeze_and_storeunalign instead.
 *
 * @override_id asc_squeeze_v2__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 8 bits (b16) of vector_uint16_t src and writes them into the low half of vector_uint8_t dst (deprecated; use asc_pack_to_low).
 *
 * @override_id asc_pack__Rvu8_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_uint16_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 8 bits (b16) of vector_int16_t src and writes them into the low half of vector_uint8_t dst (deprecated; use asc_pack_to_low).
 *
 * @override_id asc_pack__Rvu8_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_int16_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 16 bits (b32) of vector_uint32_t src and writes them into the low half of vector_uint16_t dst (deprecated; use asc_pack_to_low).
 *
 * @override_id asc_pack__Rvu16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_uint32_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 16 bits (b32) of vector_int32_t src and writes them into the low half of vector_uint16_t dst (deprecated; use asc_pack_to_low).
 *
 * @override_id asc_pack__Rvu16_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_int32_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Packs vector_bool src into the low half of vector_bool dst via reference output (deprecated; use asc_pack_to_low).
 *
 * @override_id asc_pack__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack(vector_bool& dst, vector_bool src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 8 bits (b16) of vector_uint16_t src and writes them into the high half of vector_uint8_t dst (deprecated; use asc_pack_to_high).
 *
 * @override_id asc_pack_v2__Rvu8_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_uint16_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 8 bits (b16) of vector_int16_t src and writes them into the high half of vector_uint8_t dst (deprecated; use asc_pack_to_high).
 *
 * @override_id asc_pack_v2__Rvu8_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_int16_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 16 bits (b32) of vector_uint32_t src and writes them into the high half of vector_uint16_t dst (deprecated; use asc_pack_to_high).
 *
 * @override_id asc_pack_v2__Rvu16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_uint32_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Selects the low 16 bits (b32) of vector_int32_t src and writes them into the high half of vector_uint16_t dst (deprecated; use asc_pack_to_high).
 *
 * @override_id asc_pack_v2__Rvu16_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_int32_t src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Packs vector_bool src into the high half of vector_bool dst via reference output (deprecated; use asc_pack_to_high).
 *
 * @override_id asc_pack_v2__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器/掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器/掩码寄存器）。
 */
__simd_callee__ inline void asc_pack_v2(vector_bool& dst, vector_bool src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_init)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Clears the AR special register to reset the accumulated valid-element byte count before calling asc_squeeze_with_status (deprecated; use asc_squeeze_and_storeunalign_init).
 *
 * @override_id asc_clear_ar_spr__z
 * @archs 3510
 */
__simd_callee__ inline void asc_clear_ar_spr();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_PERMUTE_SEL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_PERMUTE_SEL_H
#endif
