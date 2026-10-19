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
#error "compute/reg_gather.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_GATHER_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_GATHER_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_GATHER_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_gather_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers int8_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vi8_vu8
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_int8_t asc_gather(vector_int8_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers uint8_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vu8_vu8
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_uint8_t asc_gather(vector_uint8_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers int16_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vi16_vu16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_int16_t asc_gather(vector_int16_t src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers uint16_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vu16_vu16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_uint16_t asc_gather(vector_uint16_t src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers int32_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vi32_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_int32_t asc_gather(vector_int32_t src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers uint32_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vu32_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_uint32_t asc_gather(vector_uint32_t src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers float elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vf32_vu32
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_float asc_gather(vector_float src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers half elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vf16_vu16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_half asc_gather(vector_half src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers bfloat16_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vbf16_vu16
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_bfloat16_t asc_gather(vector_bfloat16_t src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers fp8_e4m3fn_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vf8e4m3_vu8
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_gather(vector_fp8_e4m3fn_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers hifloat8_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vhf8_vu8
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_hifloat8_t asc_gather(vector_hifloat8_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers fp8_e5m2_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vf8e5m2_vu8
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_gather(vector_fp8_e5m2_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers fp8_e8m0_t elements from the src vector register using per-element indices held in index and returns the collected vector register (non-void return).
 *
 * @override_id asc_gather__vf8e8m0_vu8
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_gather(vector_fp8_e8m0_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers int8_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvi8_vi8_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_int8_t& dst, vector_int8_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers uint8_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvu8_vu8_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_uint8_t& dst, vector_uint8_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers int16_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvi16_vi16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_int16_t& dst, vector_int16_t src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers uint16_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvu16_vu16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_uint16_t& dst, vector_uint16_t src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers int32_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvi32_vi32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_int32_t& dst, vector_int32_t src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers uint32_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvu32_vu32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_uint32_t& dst, vector_uint32_t src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers float elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvf32_vf32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_float& dst, vector_float src, vector_uint32_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers half elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvf16_vf16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_half& dst, vector_half src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers bfloat16_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvbf16_vbf16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_uint16_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers fp8_e4m3fn_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvf8e4m3_vf8e4m3_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers hifloat8_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvhf8_vhf8_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers fp8_e5m2_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvf8e5m2_vf8e5m2_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_uint8_t index);

/**
 * @ingroup simd_c_api.reg_compute.reg_gather
 * @brief Gathers fp8_e8m0_t elements from the src vector register using per-element indices held in index and writes the collected elements into dst; void return.
 *
 * @override_id asc_gather__Rvf8e8m0_vf8e8m0_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器），dtype须与dst一致。
 * @param [in] index 数据索引（矢量数据寄存器）。单位是元素个数，dst[i] = src[index[i]]。
 */
__simd_callee__ inline void asc_gather(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_uint8_t index);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_GATHER_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_GATHER_H
#endif
