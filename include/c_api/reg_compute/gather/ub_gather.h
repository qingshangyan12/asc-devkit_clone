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
#error "gather/ub_gather.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_GATHER_UB_GATHER_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_GATHER_UB_GATHER_H
#define INCLUDE_C_API_REG_COMPUTE_GATHER_UB_GATHER_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/ub_gather_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int8_t value by gathering elements from a Unified Buffer int8 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUi8_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_gather(__ubuf__ int8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint8_t value by gathering elements from a Unified Buffer uint8 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUu8_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_gather(__ubuf__ uint8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int16_t value by gathering elements from a Unified Buffer int16 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUi16_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_gather(__ubuf__ int16_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint16_t value by gathering elements from a Unified Buffer uint16 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUu16_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_gather(__ubuf__ uint16_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int32_t value by gathering elements from a Unified Buffer int32 source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUi32_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_gather(__ubuf__ int32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint32_t value by gathering elements from a Unified Buffer uint32 source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUu32_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_gather(__ubuf__ uint32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_half value by gathering elements from a Unified Buffer half source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUf16_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_gather(__ubuf__ half* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_float value by gathering elements from a Unified Buffer float source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUf32_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_gather(__ubuf__ float* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_bfloat16_t value by gathering elements from a Unified Buffer bfloat16 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUbf16_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_gather(__ubuf__ bfloat16_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e4m3fn_t value by gathering elements from a Unified Buffer fp8_e4m3fn source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUf8e4m3_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_gather(
    __ubuf__ fp8_e4m3fn_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_hifloat8_t value by gathering elements from a Unified Buffer hifloat8 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUhf8_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_hifloat8_t asc_gather(__ubuf__ hifloat8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e5m2_t value by gathering elements from a Unified Buffer fp8_e5m2 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUf8e5m2_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_gather(__ubuf__ fp8_e5m2_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e8m0_t value by gathering elements from a Unified Buffer fp8_e8m0 source using uint16_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUf8e8m0_vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_gather(__ubuf__ fp8_e8m0_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int16_t value by gathering elements from a Unified Buffer int16 source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUi16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_gather(__ubuf__ int16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint16_t value by gathering elements from a Unified Buffer uint16 source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUu16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_gather(__ubuf__ uint16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_half value by gathering elements from a Unified Buffer half source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUf16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_gather(__ubuf__ half* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_bfloat16_t value by gathering elements from a Unified Buffer bfloat16 source using uint32_t index vector and mask; masked-off elements are zeroed.
 *
 * @override_id asc_gather__PUbf16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_gather(__ubuf__ bfloat16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer int8 source into referenced vector_int16_t dst using uint16_t index vector and mask, zero-extending 8-bit values; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvi16_PUi8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_int16_t& dst, __ubuf__ int8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer uint8 source into referenced vector_uint16_t dst using uint16_t index vector and mask, zero-extending 8-bit values; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvu16_PUu8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_uint16_t& dst, __ubuf__ uint8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer int8 source into referenced vector_int8_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvi8_PUi8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_int8_t& dst, __ubuf__ int8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer uint8 source into referenced vector_uint8_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvu8_PUu8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer int16 source into referenced vector_int16_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvi16_PUi16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer uint16 source into referenced vector_uint16_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvu16_PUu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer int32 source into referenced vector_int32_t dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvi32_PUi32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_int32_t& dst, __ubuf__ int32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer uint32 source into referenced vector_uint32_t dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvu32_PUu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer half source into referenced vector_half dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvf16_PUf16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer float source into referenced vector_float dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvf32_PUf32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(vector_float& dst, __ubuf__ float* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer bfloat16 source into referenced vector_bfloat16_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvbf16_PUbf16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer fp8_e4m3fn source into referenced vector_fp8_e4m3fn_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvf8e4m3_PUf8e4m3_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer hifloat8 source into referenced vector_hifloat8_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvhf8_PUhf8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer fp8_e5m2 source into referenced vector_fp8_e5m2_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvf8e5m2_PUf8e5m2_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer fp8_e8m0 source into referenced vector_fp8_e8m0_t dst using uint16_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvf8e8m0_PUf8e8m0_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer int16 source into referenced vector_int16_t dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvi16_PUi16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer uint16 source into referenced vector_uint16_t dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvu16_PUu16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer half source into referenced vector_half dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvf16_PUf16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers elements from a Unified Buffer bfloat16 source into referenced vector_bfloat16_t dst using uint32_t index vector and mask; masked-off elements are zeroed; void return.
 *
 * @override_id asc_gather__Rvbf16_PUbf16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。在UB中的起始地址需要32B对齐，须落在UB地址空间内。
 * @param [in] index 数据索引（矢量数据寄存器）。dst中每个元素在UB中相对于src的索引位置，单位是元素个数。
 * @param [in] mask 源操作数掩码（掩码寄存器）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_gather(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int8_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer int8 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi8_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_int8_t asc_gather_datablock(
    __ubuf__ int8_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint8_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer uint8 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUu8_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_uint8_t asc_gather_datablock(
    __ubuf__ uint8_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int16_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer int16 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_int16_t asc_gather_datablock(
    __ubuf__ int16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint16_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer uint16 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUu16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_uint16_t asc_gather_datablock(
    __ubuf__ uint16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int32_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer int32 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi32_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_int32_t asc_gather_datablock(
    __ubuf__ int32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint32_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer uint32 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUu32_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_uint32_t asc_gather_datablock(
    __ubuf__ uint32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_half value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer half source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_half asc_gather_datablock(__ubuf__ half* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_float value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer float source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf32_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_float asc_gather_datablock(__ubuf__ float* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_bfloat16_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer bfloat16 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUbf16_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_bfloat16_t asc_gather_datablock(
    __ubuf__ bfloat16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e4m3fn_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer fp8_e4m3fn source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf8e4m3_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_gather_datablock(
    __ubuf__ fp8_e4m3fn_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_hifloat8_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer hifloat8 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUhf8_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_hifloat8_t asc_gather_datablock(
    __ubuf__ hifloat8_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e5m2_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer fp8_e5m2 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf8e5m2_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_gather_datablock(
    __ubuf__ fp8_e5m2_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp4x2_e2m1_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer fp4x2_e2m1 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf4e2m1x2_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_gather_datablock(
    __ubuf__ fp4x2_e2m1_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp4x2_e1m2_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer fp4x2_e1m2 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf4e1m2x2_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_gather_datablock(
    __ubuf__ fp4x2_e1m2_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int4x2_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer int4b source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi4x2_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_int4x2_t asc_gather_datablock(
    __ubuf__ int4b_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e8m0_t value gathered from mask-selected 32-byte DataBlocks of a Unified Buffer fp8_e8m0 source at uint32 indices (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf8e8m0_vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_gather_datablock(
    __ubuf__ fp8_e8m0_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int8_t value gathered from a Unified Buffer int8 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi8_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_int8_t asc_gather_datablock(__ubuf__ int8_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint8_t value gathered from a Unified Buffer uint8 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUu8_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_uint8_t asc_gather_datablock(__ubuf__ uint8_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int16_t value gathered from a Unified Buffer int16 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi16_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_int16_t asc_gather_datablock(__ubuf__ int16_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint16_t value gathered from a Unified Buffer uint16 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUu16_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_uint16_t asc_gather_datablock(__ubuf__ uint16_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int32_t value gathered from a Unified Buffer int32 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi32_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_int32_t asc_gather_datablock(__ubuf__ int32_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_uint32_t value gathered from a Unified Buffer uint32 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUu32_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_uint32_t asc_gather_datablock(__ubuf__ uint32_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_half value gathered from a Unified Buffer half source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf16_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_half asc_gather_datablock(__ubuf__ half* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_float value gathered from a Unified Buffer float source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf32_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_float asc_gather_datablock(__ubuf__ float* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_bfloat16_t value gathered from a Unified Buffer bfloat16 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUbf16_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_bfloat16_t asc_gather_datablock(__ubuf__ bfloat16_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e4m3fn_t value gathered from a Unified Buffer fp8_e4m3fn source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf8e4m3_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_gather_datablock(__ubuf__ fp8_e4m3fn_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_hifloat8_t value gathered from a Unified Buffer hifloat8 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUhf8_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_hifloat8_t asc_gather_datablock(__ubuf__ hifloat8_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e5m2_t value gathered from a Unified Buffer fp8_e5m2 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf8e5m2_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_gather_datablock(__ubuf__ fp8_e5m2_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp4x2_e2m1_t value gathered from a Unified Buffer fp4x2_e2m1 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf4e2m1x2_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_gather_datablock(__ubuf__ fp4x2_e2m1_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp4x2_e1m2_t value gathered from a Unified Buffer fp4x2_e1m2 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf4e1m2x2_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_gather_datablock(__ubuf__ fp4x2_e1m2_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_int4x2_t value gathered from a Unified Buffer int4b source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUi4x2_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_int4x2_t asc_gather_datablock(__ubuf__ int4b_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Returns a vector_fp8_e8m0_t value gathered from a Unified Buffer fp8_e8m0 source, taking one 32-byte DataBlock per uint32 index (first 8 valid).
 *
 * @override_id asc_gather_datablock__PUf8e8m0_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_gather_datablock(__ubuf__ fp8_e8m0_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer int8 source at uint32 indices (first 8 valid) into referenced vector_int8_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi8_PUi8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_int8_t& dst, __ubuf__ int8_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer uint8 source at uint32 indices (first 8 valid) into referenced vector_uint8_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu8_PUu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer int16 source at uint32 indices (first 8 valid) into referenced vector_int16_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi16_PUi16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer uint16 source at uint32 indices (first 8 valid) into referenced vector_uint16_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu16_PUu16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer int32 source at uint32 indices (first 8 valid) into referenced vector_int32_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi32_PUi32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer uint32 source at uint32 indices (first 8 valid) into referenced vector_uint32_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu32_PUu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer int64 source at uint32 indices (first 8 valid) into referenced vector_int64_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi64_PUi64_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_int64_t& dst, __ubuf__ int64_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer uint64 source at uint32 indices (first 8 valid) into referenced vector_uint64_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu64_PUu64_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_uint64_t& dst, __ubuf__ uint64_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer half source at uint32 indices (first 8 valid) into referenced vector_half dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf16_PUf16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_half& dst, __ubuf__ half* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer float source at uint32 indices (first 8 valid) into referenced vector_float dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf32_PUf32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_float& dst, __ubuf__ float* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer bfloat16 source at uint32 indices (first 8 valid) into referenced vector_bfloat16_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvbf16_PUbf16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer fp8_e4m3fn source at uint32 indices (first 8 valid) into referenced vector_fp8_e4m3fn_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf8e4m3_PUf8e4m3_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer hifloat8 source at uint32 indices (first 8 valid) into referenced vector_hifloat8_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvhf8_PUhf8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer fp8_e5m2 source at uint32 indices (first 8 valid) into referenced vector_fp8_e5m2_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf8e5m2_PUf8e5m2_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer fp4x2_e2m1 source at uint32 indices (first 8 valid) into referenced vector_fp4x2_e2m1_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf4e2m1x2_PUf4e2m1x2_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer fp4x2_e1m2 source at uint32 indices (first 8 valid) into referenced vector_fp4x2_e1m2_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf4e1m2x2_PUf4e1m2x2_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer int4b source at uint32 indices (first 8 valid) into referenced vector_int4x2_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi4x2_PUi4x2_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers mask-selected 32-byte DataBlocks from a Unified Buffer fp8_e8m0 source at uint32 indices (first 8 valid) into referenced vector_fp8_e8m0_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf8e8m0_PUf8e8m0_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 * @param [in] mask 源操作数掩码（掩码寄存器）。**DataBlock搬运的有效指示，按b32格式解释。一个DataBlock对应4bit，仅每4bit中的最低位有效。由于index仅前8个元素有效，因此mask仅使用前8个b32元素对应的bit 0、4、8、12、16、20、24、28，分别控制dst中DataBlock0至DataBlock7是否更新，其余bit无效。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer int8 source at uint32 indices (first 8 valid) into referenced vector_int8_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi8_PUi8_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer uint8 source at uint32 indices (first 8 valid) into referenced vector_uint8_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu8_PUu8_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer int16 source at uint32 indices (first 8 valid) into referenced vector_int16_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi16_PUi16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer uint16 source at uint32 indices (first 8 valid) into referenced vector_uint16_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu16_PUu16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer int32 source at uint32 indices (first 8 valid) into referenced vector_int32_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi32_PUi32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer uint32 source at uint32 indices (first 8 valid) into referenced vector_uint32_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu32_PUu32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer int64 source at uint32 indices (first 8 valid) into referenced vector_int64_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi64_PUi64_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_int64_t& dst, __ubuf__ int64_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer uint64 source at uint32 indices (first 8 valid) into referenced vector_uint64_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvu64_PUu64_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_uint64_t& dst, __ubuf__ uint64_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer half source at uint32 indices (first 8 valid) into referenced vector_half dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf16_PUf16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_half& dst, __ubuf__ half* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer float source at uint32 indices (first 8 valid) into referenced vector_float dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf32_PUf32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_float& dst, __ubuf__ float* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer bfloat16 source at uint32 indices (first 8 valid) into referenced vector_bfloat16_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvbf16_PUbf16_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer fp8_e4m3fn source at uint32 indices (first 8 valid) into referenced vector_fp8_e4m3fn_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf8e4m3_PUf8e4m3_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer hifloat8 source at uint32 indices (first 8 valid) into referenced vector_hifloat8_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvhf8_PUhf8_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer fp8_e5m2 source at uint32 indices (first 8 valid) into referenced vector_fp8_e5m2_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf8e5m2_PUf8e5m2_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer fp4x2_e2m1 source at uint32 indices (first 8 valid) into referenced vector_fp4x2_e2m1_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf4e2m1x2_PUf4e2m1x2_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer fp4x2_e1m2 source at uint32 indices (first 8 valid) into referenced vector_fp4x2_e1m2_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf4e1m2x2_PUf4e1m2x2_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer int4b source at uint32 indices (first 8 valid) into referenced vector_int4x2_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvi4x2_PUi4x2_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.ub_gather
 * @brief Gathers 32-byte DataBlocks from a Unified Buffer fp8_e8m0 source at uint32 indices (first 8 valid) into referenced vector_fp8_e8m0_t dst; void return.
 *
 * @override_id asc_gather_datablock__Rvf8e8m0_PUf8e8m0_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] index 源操作数（矢量数据寄存器），表示dst中每个DataBlock在UB中相对于src的索引位置。索引位置要大于等于0且32B对齐，索引可以存在相同的值。**index仅前8个数有效，单位是字节。**
 */
__simd_callee__ inline void asc_gather_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, vector_uint32_t index);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_GATHER_UB_GATHER_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_GATHER_UB_GATHER_H
#endif
