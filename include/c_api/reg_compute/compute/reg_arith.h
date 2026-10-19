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
#error "compute/reg_arith.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ARITH_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_ARITH_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_ARITH_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/reg_arith_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint8 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_sub(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int8 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_sub(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint16 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_sub(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int16 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_sub(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise half difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_sub(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise bfloat16 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_sub(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint32 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_sub(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int32 difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_sub(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise float difference src0 - src1 under mask, type matching the source operands.
 *
 * @override_id asc_sub__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_sub(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint8 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int8 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint16 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int16 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise half subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise bfloat16 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint32 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int32 subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise float subtraction of src1 from src0 under mask writes the difference to dst; returns void.
 *
 * @override_id asc_sub__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint32 subtraction under mask writes the low 32 bits of the two's-complement difference to dst and the carry output to carry; returns void.
 *
 * @override_id asc_sub__Rvb_Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器），用于存储减法借位输出。减法不借位等价于补码加法进位。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储计算结果的低32位。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int32 subtraction under mask writes the low 32 bits of the two's-complement difference to dst and the carry output to carry; returns void.
 *
 * @override_id asc_sub__Rvb_Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器），用于存储减法借位输出。减法不借位等价于补码加法进位。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储计算结果的低32位。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): subtracts src1 and input borrow carry_src from src0 elementwise under mask, writing the low 32-bit difference to dst and borrow output to carry; no return value.
 *
 * @override_id asc_subc__Rvb_Rvu32_vu32_vu32_vb_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器）。用于存储减法借位输出。减法不借位等价于补码加法进位。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储计算结果的低32位。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] carry_src 源操作数（掩码寄存器），作为输入的借位标志。
 * @param [in] mask 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_subc(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): subtracts src1 and input borrow carry_src from src0 elementwise under mask, writing the low 32-bit difference to dst and borrow output to carry; no return value.
 *
 * @override_id asc_subc__Rvb_Rvi32_vi32_vi32_vb_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器）。用于存储减法借位输出。减法不借位等价于补码加法进位。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储计算结果的低32位。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] carry_src 源操作数（掩码寄存器），作为输入的借位标志。
 * @param [in] mask 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_subc(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of int8 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_min(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of int16 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_min(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of int32 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_min(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of uint8 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_min(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of uint16 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_min(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of uint32 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_min(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of half src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_min(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of bfloat16 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_min(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of float src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_min__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_min(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of int8 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of int16 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of int32 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of uint8 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of uint16 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of uint32 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of half src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of bfloat16 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of float src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_min__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise negation of the 8-bit signed integer vector src and writes the result to dst; no return value.
 *
 * @override_id asc_neg__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise negation of the 8-bit signed integer vector src; non-void, result type matches src.
 *
 * @override_id asc_neg__vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_neg(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise negation of the 16-bit signed integer vector src and writes the result to dst; no return value.
 *
 * @override_id asc_neg__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise negation of the 16-bit signed integer vector src; non-void, result type matches src.
 *
 * @override_id asc_neg__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_neg(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise negation of the 32-bit signed integer vector src and writes the result to dst; no return value.
 *
 * @override_id asc_neg__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise negation of the 32-bit signed integer vector src; non-void, result type matches src.
 *
 * @override_id asc_neg__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_neg(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise negation of the half vector src and writes the result to dst; no return value.
 *
 * @override_id asc_neg__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise negation of the half vector src; non-void, result type matches src.
 *
 * @override_id asc_neg__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_neg(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise negation of the float vector src and writes the result to dst; no return value.
 *
 * @override_id asc_neg__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise negation of the float vector src; non-void, result type matches src.
 *
 * @override_id asc_neg__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_neg(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Computes element-wise absolute values of src under mask and stores the results in dst.
 *
 * @override_id asc_abs__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Computes element-wise absolute values of src under mask and stores the results in dst.
 *
 * @override_id asc_abs__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Computes element-wise absolute values of src under mask and stores the results in dst.
 *
 * @override_id asc_abs__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Computes element-wise absolute values of src under mask and stores the results in dst.
 *
 * @override_id asc_abs__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Computes element-wise absolute values of src under mask and stores the results in dst.
 *
 * @override_id asc_abs__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise absolute values of src under mask.
 *
 * @override_id asc_abs__vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_abs(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise absolute values of src under mask.
 *
 * @override_id asc_abs__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_abs(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise absolute values of src under mask.
 *
 * @override_id asc_abs__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_abs(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise absolute values of src under mask.
 *
 * @override_id asc_abs__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_abs(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise absolute values of src under mask.
 *
 * @override_id asc_abs__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_abs(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies src0 and src1 elementwise under mask into 64-bit products, writing the low 32 bits to dst0 and the high 32 bits to dst1; no return value.
 *
 * @override_id asc_mull__Rvu32_Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且`dst0`、`dst1`对应元素置零。
 */
__simd_callee__ inline void asc_mull(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies src0 and src1 elementwise under mask into 64-bit products, writing the low 32 bits to dst0 and the high 32 bits to dst1; no return value.
 *
 * @override_id asc_mull__Rvi32_Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst0 目的操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且`dst0`、`dst1`对应元素置零。
 */
__simd_callee__ inline void asc_mull(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise square root of the half vector src; non-void, return type matches src.
 *
 * @override_id asc_sqrt__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_sqrt(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise square root of the float vector src; non-void, return type matches src.
 *
 * @override_id asc_sqrt__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_sqrt(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise square root of the half vector src and writes it to dst; no return value.
 *
 * @override_id asc_sqrt__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sqrt(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise square root of the float vector src and writes it to dst; no return value.
 *
 * @override_id asc_sqrt__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sqrt(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int16 product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_mul(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint16 product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_mul(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise half product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_mul(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise bfloat16 product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_mul(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int32 product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_mul(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint32 product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_mul(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise float product of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_mul__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_mul(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int16 multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint16 multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise half multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise bfloat16 multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int32 multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint32 multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise float multiplication of src0 and src1 under mask writes the product to dst; returns void.
 *
 * @override_id asc_mul__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies each 16-bit signed integer vector element by the 16-bit signed integer scalar value under mask and writes the product to dst; no return value.
 *
 * @override_id asc_mul_scalar__Rvi16_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies each 16-bit unsigned integer vector element by the 16-bit unsigned integer scalar value under mask and writes the product to dst; no return value.
 *
 * @override_id asc_mul_scalar__Rvu16_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies each half vector element by the half scalar value under mask and writes the product to dst; no return value.
 *
 * @override_id asc_mul_scalar__Rvf16_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies each 32-bit signed integer vector element by the 32-bit signed integer scalar value under mask and writes the product to dst; no return value.
 *
 * @override_id asc_mul_scalar__Rvi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies each 32-bit unsigned integer vector element by the 32-bit unsigned integer scalar value under mask and writes the product to dst; no return value.
 *
 * @override_id asc_mul_scalar__Rvu32_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): multiplies each float vector element by the float scalar value under mask and writes the product to dst; no return value.
 *
 * @override_id asc_mul_scalar__Rvf32_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint8 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_add(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int8 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_add(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint16 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_add(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int16 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_add(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise half sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_add(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise float sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_add(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int32 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_add(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint32 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_add(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise bfloat16 sum of src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_add__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_add(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise product of the 16-bit signed integer vector src and the 16-bit signed integer scalar value under mask; non-void, result type matches src.
 *
 * @override_id asc_mul_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_mul_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise product of the 16-bit unsigned integer vector src and the 16-bit unsigned integer scalar value under mask; non-void, result type matches src.
 *
 * @override_id asc_mul_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_mul_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise product of the half vector src and the half scalar value under mask; non-void, result type matches src.
 *
 * @override_id asc_mul_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_mul_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise product of the 32-bit signed integer vector src and the 32-bit signed integer scalar value under mask; non-void, result type matches src.
 *
 * @override_id asc_mul_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_mul_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise product of the 32-bit unsigned integer vector src and the 32-bit unsigned integer scalar value under mask; non-void, result type matches src.
 *
 * @override_id asc_mul_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_mul_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise product of the float vector src and the float scalar value under mask; non-void, result type matches src.
 *
 * @override_id asc_mul_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_mul_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint8 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int8 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint16 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int16 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise half addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise float addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int32 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint32 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise bfloat16 addition of src0 and src1 under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint32 addition under mask writes the low 32 bits of each sum to dst and the per-element add carry to carry; returns void.
 *
 * @override_id asc_add__Rvb_Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器），用于存储加法计算后的进位数据。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int32 addition under mask writes the low 32 bits of each sum to dst and the per-element add carry to carry; returns void.
 *
 * @override_id asc_add__Rvb_Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器），用于存储加法计算后的进位数据。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): adds src0, src1 and input carry carry_src elementwise under mask, writing the low 32 bits of the sum to dst and per-element carry output to carry; no return value.
 *
 * @override_id asc_addc__Rvb_Rvu32_vu32_vu32_vb_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器）。存储加法计算后的进位数据。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] carry_src 源操作数（掩码寄存器），输入的进位标志。
 * @param [in] mask 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_addc(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): adds src0, src1 and input carry carry_src elementwise under mask, writing the low 32 bits of the sum to dst and per-element carry output to carry; no return value.
 *
 * @override_id asc_addc__Rvb_Rvi32_vi32_vi32_vb_vb
 * @archs 3510
 * @param [out] carry 目的操作数（掩码寄存器）。存储加法计算后的进位数据。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] carry_src 源操作数（掩码寄存器），输入的进位标志。
 * @param [in] mask 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_addc(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of uint8 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_max(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of int8 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_max(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of uint16 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_max(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of int16 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_max(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of int32 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_max(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of uint32 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_max(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of float src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_max(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of half src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_max(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of bfloat16 src0 and src1 under mask, type matching the source operands.
 *
 * @override_id asc_max__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_max(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of uint8 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvu8_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of int8 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvi8_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of uint16 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of int16 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of int32 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of uint32 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of float src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of half src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of bfloat16 src0 and src1 under mask writes the result to dst; returns void.
 *
 * @override_id asc_max__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of int32 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of uint32 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvu32_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of int16 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvi16_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of uint16 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvu16_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of int8 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvi8_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of uint8 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvu8_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of float vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvf32_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of half vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvf16_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise minimum of bfloat16 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_min_scalar__Rvbf16_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise natural exponential of the half vector src; non-void, return type matches src.
 *
 * @override_id asc_exp__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_exp(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the mask-controlled elementwise natural exponential of the float vector src; non-void, return type matches src.
 *
 * @override_id asc_exp__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_exp(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of int32 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_min_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of uint32 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_min_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of int16 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_min_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of uint16 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_min_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of int8 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_min_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of uint8 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_min_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of float vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_min_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of half vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_min_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise minimum of bfloat16 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_min_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_min_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise natural exponential of the half vector src and writes it to dst; no return value.
 *
 * @override_id asc_exp__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_exp(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise natural exponential of the float vector src and writes it to dst; no return value.
 *
 * @override_id asc_exp__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_exp(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int8 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvi8_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint8 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvu8_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int16 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvi16_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint16 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvu16_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise half addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvf16_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise bfloat16 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvbf16_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise int32 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise uint32 addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvu32_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise float addition of vector src and scalar value under mask writes the sum to dst; returns void.
 *
 * @override_id asc_add_scalar__Rvf32_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int8 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_add_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint8 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_add_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int16 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_add_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint16 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_add_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise half sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_add_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise bfloat16 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_add_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise int32 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_add_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise uint32 sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_add_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise float sum of vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_add_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_add_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise natural logarithm of the half vector src and writes the result to dst; no return value.
 *
 * @override_id asc_ln__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ln(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise natural logarithm of the float vector src and writes the result to dst; no return value.
 *
 * @override_id asc_ln__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ln(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise quotients of src0 divided by src1 under mask.
 *
 * @override_id asc_div__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_div(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise quotients of src0 divided by src1 under mask.
 *
 * @override_id asc_div__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_div(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise quotients of src0 divided by src1 under mask.
 *
 * @override_id asc_div__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_div(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise quotients of src0 divided by src1 under mask.
 *
 * @override_id asc_div__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_div(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise quotients of src0 divided by src1 under mask.
 *
 * @override_id asc_div__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_div(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Returns element-wise quotients of src0 divided by src1 under mask.
 *
 * @override_id asc_div__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_div(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise natural logarithm of the half vector src and writes the result to dst; no return value.
 *
 * @override_id asc_log__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_log(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form (void): computes the mask-controlled elementwise natural logarithm of the float vector src and writes the result to dst; no return value.
 *
 * @override_id asc_log__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_log(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Divides src0 by src1 element-wise under mask and stores the results in dst.
 *
 * @override_id asc_div__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Divides src0 by src1 element-wise under mask and stores the results in dst.
 *
 * @override_id asc_div__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Divides src0 by src1 element-wise under mask and stores the results in dst.
 *
 * @override_id asc_div__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Divides src0 by src1 element-wise under mask and stores the results in dst.
 *
 * @override_id asc_div__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Divides src0 by src1 element-wise under mask and stores the results in dst.
 *
 * @override_id asc_div__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Divides src0 by src1 element-wise under mask and stores the results in dst.
 *
 * @override_id asc_div__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of int8 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvi8_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of int16 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvi16_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of int32 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of uint8 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvu8_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of uint16 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvu16_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of uint32 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvu32_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of half vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvf16_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of float vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvf32_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Output-parameter form: elementwise maximum of bfloat16 vector src and scalar value under mask writes the result to dst; returns void.
 *
 * @override_id asc_max_scalar__Rvbf16_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of int8 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_max_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of int16 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_max_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of int32 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_max_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of uint8 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_max_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of uint16 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_max_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of uint32 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_max_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of half vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_max_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of float vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_max_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @brief Return-value form: returns the elementwise maximum of bfloat16 vector src and scalar value under mask, a vector register of the src type.
 *
 * @override_id asc_max_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_max_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
__simd_callee__ inline void asc_max_merge(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_max_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_add_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min_merge(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ARITH_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ARITH_H
#endif
