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
#error "compute/reg_logic.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_LOGIC_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_LOGIC_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_LOGIC_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
#include "impl/c_api/reg_base_impl/reg_logic_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of mask registers src0 and src1 under mask and stores the result in vector_bool dst; unmasked bits are zeroed.
 *
 * @override_id asc_and__Rvb_vb_vb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of mask registers src0 and src1 under mask as a vector_bool; unmasked bits are zeroed.
 *
 * @override_id asc_and__vb_vb_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_bool asc_and(vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of vector_uint8_t src0 and src1 under mask and stores the result in vector_uint8_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_and__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of vector_uint8_t src0 and src1 under mask as a vector_uint8_t; unmasked elements are zeroed.
 *
 * @override_id asc_and__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint8_t asc_and(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of vector_int8_t src0 and src1 under mask and stores the result in vector_int8_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_and__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of vector_int8_t src0 and src1 under mask as a vector_int8_t; unmasked elements are zeroed.
 *
 * @override_id asc_and__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int8_t asc_and(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of vector_uint16_t src0 and src1 under mask and stores the result in vector_uint16_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_and__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of vector_uint16_t src0 and src1 under mask as a vector_uint16_t; unmasked elements are zeroed.
 *
 * @override_id asc_and__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint16_t asc_and(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of vector_int16_t src0 and src1 under mask and stores the result in vector_int16_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_and__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of vector_int16_t src0 and src1 under mask as a vector_int16_t; unmasked elements are zeroed.
 *
 * @override_id asc_and__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int16_t asc_and(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of vector_uint32_t src0 and src1 under mask and stores the result in vector_uint32_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_and__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of vector_uint32_t src0 and src1 under mask as a vector_uint32_t; unmasked elements are zeroed.
 *
 * @override_id asc_and__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint32_t asc_and(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise AND of vector_int32_t src0 and src1 under mask and stores the result in vector_int32_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_and__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_and(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise AND of vector_int32_t src0 and src1 under mask as a vector_int32_t; unmasked elements are zeroed.
 *
 * @override_id asc_and__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int32_t asc_and(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of vector_int8_t src0 and src1 under mask and stores the result in vector_int8_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_or__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of vector_int8_t src0 and src1 under mask as a vector_int8_t; unmasked elements are zeroed.
 *
 * @override_id asc_or__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int8_t asc_or(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of vector_uint8_t src0 and src1 under mask and stores the result in vector_uint8_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_or__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of vector_uint8_t src0 and src1 under mask as a vector_uint8_t; unmasked elements are zeroed.
 *
 * @override_id asc_or__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint8_t asc_or(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of vector_int16_t src0 and src1 under mask and stores the result in vector_int16_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_or__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of vector_int16_t src0 and src1 under mask as a vector_int16_t; unmasked elements are zeroed.
 *
 * @override_id asc_or__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int16_t asc_or(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of vector_uint16_t src0 and src1 under mask and stores the result in vector_uint16_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_or__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of vector_uint16_t src0 and src1 under mask as a vector_uint16_t; unmasked elements are zeroed.
 *
 * @override_id asc_or__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint16_t asc_or(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of vector_int32_t src0 and src1 under mask and stores the result in vector_int32_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_or__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of vector_int32_t src0 and src1 under mask as a vector_int32_t; unmasked elements are zeroed.
 *
 * @override_id asc_or__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int32_t asc_or(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of vector_uint32_t src0 and src1 under mask and stores the result in vector_uint32_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_or__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of vector_uint32_t src0 and src1 under mask as a vector_uint32_t; unmasked elements are zeroed.
 *
 * @override_id asc_or__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint32_t asc_or(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise OR of mask registers src0 and src1 under mask and stores the result in vector_bool dst; unmasked bits are zeroed.
 *
 * @override_id asc_or__Rvb_vb_vb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_or(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise OR of mask registers src0 and src1 under mask as a vector_bool; unmasked bits are zeroed.
 *
 * @override_id asc_or__vb_vb_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_bool asc_or(vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_uint8_t register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_uint8_t register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vu8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint8_t asc_not(vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_int8_t register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_int8_t register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int8_t asc_not(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_uint16_t register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_uint16_t register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint16_t asc_not(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_int16_t register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_int16_t register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int16_t asc_not(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_half register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_half register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_half asc_not(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_uint32_t register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_uint32_t register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_uint32_t asc_not(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_int32_t register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_int32_t register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_int32_t asc_not(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs element-wise bitwise NOT of the vector_float register src under mask and writes the result into dst (void return); mask-excluded elements are zero.
 *
 * @override_id asc_not__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the element-wise bitwise NOT of the vector_float register src under mask; mask-excluded elements are zero.
 *
 * @override_id asc_not__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_float asc_not(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Performs bitwise NOT of mask register src under mask and writes the result into vector_bool dst (void return); bits with mask 0 are set to 0.
 *
 * @override_id asc_not__Rvb_vb_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器或掩码寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline void asc_not(vector_bool& dst, vector_bool src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise NOT of mask register src computed under mask; bits not selected by mask are 0.
 *
 * @override_id asc_not__vb_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器或掩码寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。 &bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 &bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。
 */
__simd_callee__ inline vector_bool asc_not(vector_bool src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of vector_int32_t src0 and src1 under mask and stores the result in vector_int32_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_xor__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of vector_uint32_t src0 and src1 under mask and stores the result in vector_uint32_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_xor__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of vector_uint16_t src0 and src1 under mask and stores the result in vector_uint16_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_xor__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of vector_int16_t src0 and src1 under mask and stores the result in vector_int16_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_xor__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of vector_uint8_t src0 and src1 under mask and stores the result in vector_uint8_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_xor__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of vector_int8_t src0 and src1 under mask and stores the result in vector_int8_t dst; unmasked elements are zeroed.
 *
 * @override_id asc_xor__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Computes the bitwise XOR of mask registers src0 and src1 under mask and stores the result in vector_bool dst; unmasked bits are zeroed.
 *
 * @override_id asc_xor__Rvb_vb_vb_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，仅无返回值类型接口包含该参数，写入按位异或结果。`dtype`须与`src0`、`src1`一致。
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_xor(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of vector_int32_t src0 and src1 under mask as a vector_int32_t; unmasked elements are zeroed.
 *
 * @override_id asc_xor__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_xor(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of vector_uint32_t src0 and src1 under mask as a vector_uint32_t; unmasked elements are zeroed.
 *
 * @override_id asc_xor__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_xor(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of vector_uint16_t src0 and src1 under mask as a vector_uint16_t; unmasked elements are zeroed.
 *
 * @override_id asc_xor__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_xor(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of vector_int16_t src0 and src1 under mask as a vector_int16_t; unmasked elements are zeroed.
 *
 * @override_id asc_xor__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_xor(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of vector_uint8_t src0 and src1 under mask as a vector_uint8_t; unmasked elements are zeroed.
 *
 * @override_id asc_xor__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_xor(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of vector_int8_t src0 and src1 under mask as a vector_int8_t; unmasked elements are zeroed.
 *
 * @override_id asc_xor__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_xor(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the bitwise XOR of mask registers src0 and src1 under mask as a vector_bool; unmasked bits are zeroed.
 *
 * @override_id asc_xor__vb_vb_vb
 * @archs 3510
 * @param [in] src0 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] src1 源矢量数据寄存器，参与按位异或的操作数。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bool asc_xor(vector_bool src0, vector_bool src1, vector_bool mask);

#if !defined(__NPU_ARCH__) || (__NPU_ARCH__ != 9201)
//=================asc_shiftleft (signed shift overloads, 3510)==================
/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__Rvu8_vu8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft(
    vector_uint8_t& dst, vector_uint8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft(vector_int8_t& dst, vector_int8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__Rvu16_vu16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft(
    vector_uint16_t& dst, vector_uint16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft(
    vector_int16_t& dst, vector_int16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__Rvu32_vu32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft(
    vector_uint32_t& dst, vector_uint32_t src, vector_int32_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft(
    vector_int32_t& dst, vector_int32_t src, vector_int32_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__vu8_vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint8_t asc_shiftleft(vector_uint8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__vi8_vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int8_t asc_shiftleft(vector_int8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__vu16_vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint16_t asc_shiftleft(vector_uint16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__vi16_vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int16_t asc_shiftleft(vector_int16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__vu32_vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint32_t asc_shiftleft(vector_uint32_t src, vector_int32_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft__vi32_vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int32_t asc_shiftleft(vector_int32_t src, vector_int32_t shift, vector_bool mask);

//=================asc_shiftright (signed shift overloads, 3510)==================
/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__Rvu8_vu8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright(
    vector_uint8_t& dst, vector_uint8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright(
    vector_int8_t& dst, vector_int8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__Rvu16_vu16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright(
    vector_uint16_t& dst, vector_uint16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright(
    vector_int16_t& dst, vector_int16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__Rvu32_vu32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright(
    vector_uint32_t& dst, vector_uint32_t src, vector_int32_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the corresponding element of the shift vector and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright(
    vector_int32_t& dst, vector_int32_t src, vector_int32_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__vu8_vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint8_t asc_shiftright(vector_uint8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__vi8_vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int8_t asc_shiftright(vector_int8_t src, vector_int8_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__vu16_vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint16_t asc_shiftright(vector_uint16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__vi16_vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int16_t asc_shiftright(vector_int16_t src, vector_int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__vu32_vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint32_t asc_shiftright(vector_uint32_t src, vector_int32_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the corresponding element of the shift vector; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright__vi32_vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（矢量数据寄存器）。数据类型须与`src`的位宽一致，且为有符号整数类型。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int32_t asc_shiftright(vector_int32_t src, vector_int32_t shift, vector_bool mask);

//=================asc_shiftleft_scalar (signed shift overloads, 3510)==================
/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__Rvu8_vu8_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint8_t& dst, vector_uint8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__Rvi8_vi8_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int8_t& dst, vector_int8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__Rvu16_vu16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint16_t& dst, vector_uint16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__Rvi16_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int16_t& dst, vector_int16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__Rvu32_vu32_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint32_t& dst, vector_uint32_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Left-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__Rvi32_vi32_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int32_t& dst, vector_int32_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__vu8_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint8_t asc_shiftleft_scalar(vector_uint8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__vi8_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int8_t asc_shiftleft_scalar(vector_int8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__vu16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint16_t asc_shiftleft_scalar(vector_uint16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int16_t asc_shiftleft_scalar(vector_int16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__vu32_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint32_t asc_shiftleft_scalar(vector_uint32_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of left-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftleft_scalar__vi32_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int32_t asc_shiftleft_scalar(vector_int32_t src, int16_t shift, vector_bool mask);

//=================asc_shiftright_scalar (signed shift overloads, 3510)==================
/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__Rvu8_vu8_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint8_t& dst, vector_uint8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__Rvi8_vi8_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright_scalar(
    vector_int8_t& dst, vector_int8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__Rvu16_vu16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint16_t& dst, vector_uint16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__Rvi16_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright_scalar(
    vector_int16_t& dst, vector_int16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__Rvu32_vu32_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint32_t& dst, vector_uint32_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Right-shifts each masked element of src by the scalar immediate shift and stores the result in dst; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__Rvi32_vi32_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline void asc_shiftright_scalar(
    vector_int32_t& dst, vector_int32_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__vu8_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint8_t asc_shiftright_scalar(vector_uint8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__vi8_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int8_t asc_shiftright_scalar(vector_int8_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__vu16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint16_t asc_shiftright_scalar(vector_uint16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int16_t asc_shiftright_scalar(vector_int16_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__vu32_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_uint32_t asc_shiftright_scalar(vector_uint32_t src, int16_t shift, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_logic
 * @brief Returns the result of right-shifting each masked element of src by the scalar immediate; masked-off elements are set to zero.
 *
 * @override_id asc_shiftright_scalar__vi32_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] shift 位移量（标量），类型为`int16_t`，不支持设置为负数。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且输出结果对应元素置零。
 */
__simd_callee__ inline vector_int32_t asc_shiftright_scalar(vector_int32_t src, int16_t shift, vector_bool mask);

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9201)
#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
//=================asc_shiftleft (unsigned shift overloads, 9201)==================
__simd_callee__ inline void asc_shiftleft(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftleft(
    vector_int8_t& dst, vector_int8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftleft(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftleft(
    vector_int16_t& dst, vector_int16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftleft(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftleft(
    vector_int32_t& dst, vector_int32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline vector_uint8_t asc_shiftleft(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline vector_int8_t asc_shiftleft(vector_int8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline vector_uint16_t asc_shiftleft(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline vector_int16_t asc_shiftleft(vector_int16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_shiftleft(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_shiftleft(vector_int32_t src0, vector_uint32_t src1, vector_bool mask);

//=================asc_shiftleft_scalar (uint value overloads, 9201)==================
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint8_t& dst, vector_uint8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int8_t& dst, vector_int8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int16_t& dst, vector_int16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int32_t& dst, vector_int32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline vector_uint8_t asc_shiftleft_scalar(vector_uint8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_int8_t asc_shiftleft_scalar(vector_int8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_uint16_t asc_shiftleft_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_int16_t asc_shiftleft_scalar(vector_int16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_shiftleft_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_shiftleft_scalar(vector_int32_t src, uint32_t value, vector_bool mask);

//=================asc_shiftright (unsigned shift overloads, 9201)==================
__simd_callee__ inline void asc_shiftright(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftright(
    vector_int8_t& dst, vector_int8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftright(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftright(
    vector_int16_t& dst, vector_int16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftright(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_shiftright(
    vector_int32_t& dst, vector_int32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline vector_uint8_t asc_shiftright(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline vector_int8_t asc_shiftright(vector_int8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline vector_uint16_t asc_shiftright(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline vector_int16_t asc_shiftright(vector_int16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_shiftright(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_shiftright(vector_int32_t src0, vector_uint32_t src1, vector_bool mask);

//=================asc_shiftright_scalar (uint value overloads, 9201)==================
__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint8_t& dst, vector_uint8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int8_t& dst, vector_int8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int16_t& dst, vector_int16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int32_t& dst, vector_int32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline vector_uint8_t asc_shiftright_scalar(vector_uint8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_int8_t asc_shiftright_scalar(vector_int8_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_uint16_t asc_shiftright_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_int16_t asc_shiftright_scalar(vector_int16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline vector_uint32_t asc_shiftright_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline vector_int32_t asc_shiftright_scalar(vector_int32_t src, uint32_t value, vector_bool mask);
#endif
#endif

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise AND of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_and__Rvf8e4m3_vf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_and(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise AND of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_and__Rvf8e5m2_vf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_and(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise AND of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_and__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_and(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise AND of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_and__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_and(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise AND of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_and__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_and(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_or integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise OR of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_or__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_or(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_or integer overload)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Computes the bitwise OR of the mask-selected elements of floating-point src0 and src1 and stores the result in dst; unmasked elements are set to zero.
 *
 * @override_id asc_or__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_or(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_LOGIC_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_LOGIC_H
#endif
