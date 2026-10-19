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
#error "compute/reg_histogram.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_HISTOGRAM_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_HISTOGRAM_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_HISTOGRAM_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_histogram_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_histogram
 * @brief Accumulates a cumulative histogram into dst: dst[n] adds the count of uint8 src values not greater than n (low-range mode, thresholds 0-127), with mask selecting counted elements.
 *
 * @override_id asc_cumulative_histogram_bin0__Rvu16_vu8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_cumulative_histogram_bin0(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_histogram
 * @brief Accumulates a cumulative histogram into dst: dst[n] adds the count of uint8 src values not greater than 128+n (high-range mode, thresholds 128-255), with mask selecting counted elements.
 *
 * @override_id asc_cumulative_histogram_bin1__Rvu16_vu8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_cumulative_histogram_bin1(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_histogram
 * @brief Counts occurrences of uint8 src values in [0, 127] and accumulates those frequencies into dst (low-range mode), with mask selecting which src elements are counted.
 *
 * @override_id asc_frequency_histogram_bin0__Rvu16_vu8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
__simd_callee__ inline void asc_frequency_histogram_bin0(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_histogram
 * @brief Counts occurrences of uint8 src values in [128, 255] and accumulates those frequencies into dst (high-range mode), with mask selecting which src elements are counted.
 *
 * @override_id asc_frequency_histogram_bin1__Rvu16_vu8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
__simd_callee__ inline void asc_frequency_histogram_bin1(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_HISTOGRAM_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_HISTOGRAM_H
#endif
