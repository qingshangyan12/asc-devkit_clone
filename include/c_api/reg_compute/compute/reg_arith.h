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

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_sub(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_sub(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_sub(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_sub(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_sub(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_sub(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_sub(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_sub(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_sub(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_uint8_t&,vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_int8_t&,vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_uint16_t&,vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_int16_t&,vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_half&,vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_bfloat16_t&,vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_float&,vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_bool&,vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] carry 目的操作数（掩码寄存器），用于存储减法借位输出。减法不借位等价于补码加法进位。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sub(vector_bool&,vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求差操作，将结果写入目的操作数`dst`。
 * @param [out] carry 目的操作数（掩码寄存器），用于存储减法借位输出。减法不借位等价于补码加法进位。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储减法结果，仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sub(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_subc(vector_bool&,vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1及输入进位carry_src进行按元素求差操作，减法运算在硬件底层通过补码加法实现，将结果写入目的操作数dst，同时将每个元素的进位结果写入carry（存放进位标志的掩码寄存器）。
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
 * @function asc_subc(vector_bool&,vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1及输入进位carry_src进行按元素求差操作，减法运算在硬件底层通过补码加法实现，将结果写入目的操作数dst，同时将每个元素的进位结果写入carry（存放进位标志的掩码寄存器）。
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
 * @function asc_min(vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_min(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_min(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_min(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_min(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_min(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_min(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_min(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_min(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_min(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_int8_t&,vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_int16_t&,vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_uint8_t&,vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_uint16_t&,vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_half&,vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_bfloat16_t&,vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min(vector_float&,vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。 计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_min(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_int8_t&,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_neg(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_int16_t&,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_neg(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_int32_t&,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_neg(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_half&,vector_half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_neg(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_float&,vector_float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储取反结果，仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_neg(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_neg(vector_float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取相反数，并将结果作为返回值返回或写入目的操作数`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_neg(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_int8_t&,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_int16_t&,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_int32_t&,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_half&,vector_half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_float&,vector_float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_abs(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_abs(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_abs(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_abs(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_abs(vector_float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_abs(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mull(vector_uint32_t&,vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据掩码将两个源矢量数据寄存器中的有效元素相乘，数据类型位宽为32位的元素相乘得到64位乘积，拆分为低32位与高32位分别写入两个目的矢量数据寄存器。掩码对应位置为1的元素参与计算，为0的元素在目的矢量数据寄存器中置零。计算公式如下：
 * @param [out] dst0 目的操作数（矢量数据寄存器）。数据类型须与src0一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器）。数据类型须与src0一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与src0一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且dst0、dst1对应元素置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_mull(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mull(vector_int32_t&,vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 根据掩码将两个源矢量数据寄存器中的有效元素相乘，数据类型位宽为32位的元素相乘得到64位乘积，拆分为低32位与高32位分别写入两个目的矢量数据寄存器。掩码对应位置为1的元素参与计算，为0的元素在目的矢量数据寄存器中置零。计算公式如下：
 * @param [out] dst0 目的操作数（矢量数据寄存器）。数据类型须与src0一致。
 * @param [out] dst1 目的操作数（矢量数据寄存器）。数据类型须与src0一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。数据类型须与src0一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且dst0、dst1对应元素置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_mull(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sqrt(vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求开方操作，将结果写入目的操作数dst。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_sqrt(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sqrt(vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求开方操作，将结果写入目的操作数dst。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_sqrt(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sqrt(vector_half&,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求开方操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sqrt(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_sqrt(vector_float&,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求开方操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_sqrt(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_mul(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_mul(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_mul(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_mul(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_mul(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_mul(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_mul(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_int16_t&,vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_uint16_t&,vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_half&,vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_bfloat16_t&,vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul(vector_float&,vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`按元素进行乘法运算，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_mul(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_int16_t&,vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_uint16_t&,vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_half&,vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_int32_t&,vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_uint32_t&,vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_float&,vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mul_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_add(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_add(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_add(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_add(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_add(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_add(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_add(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_add(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_add(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_mul_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_mul_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_mul_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_mul_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_mul_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_mul_scalar(vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素与标量`value`相乘，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_mul_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_uint8_t&,vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_int8_t&,vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_uint16_t&,vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_int16_t&,vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_half&,vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_float&,vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_bfloat16_t&,vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_bool&,vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] carry 目的操作数（掩码寄存器），用于存储加法计算后的进位数据。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add(vector_bool&,vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。
 * @param [out] carry 目的操作数（掩码寄存器），用于存储加法计算后的进位数据。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于存储加法和。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_add(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_addc(vector_bool&,vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1及输入进位carry_src进行按元素求和操作，将结果写入目的操作数dst，同时将每个元素的进位结果写入carry（存放进位标志的掩码寄存器）。
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
 * @function asc_addc(vector_bool&,vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1及输入进位carry_src进行按元素求和操作，将结果写入目的操作数dst，同时将每个元素的进位结果写入carry（存放进位标志的掩码寄存器）。
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
 * @function asc_max(vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint8_t asc_max(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int8_t asc_max(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_max(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_max(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_max(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_max(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_max(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_max(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_max(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_uint8_t&,vector_uint8_t,vector_uint8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_int8_t&,vector_int8_t,vector_int8_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_uint16_t&,vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_int16_t&,vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_float&,vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_half&,vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max(vector_bfloat16_t&,vector_bfloat16_t,vector_bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求最大值操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_max(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_int32_t&,vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_uint32_t&,vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_int16_t&,vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_uint16_t&,vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_int8_t&,vector_int8_t,int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_uint8_t&,vector_uint8_t,uint8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_float&,vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_half&,vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_bfloat16_t&,vector_bfloat16_t,bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_min_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_exp(vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求自然指数操作，将结果写入目的操作数dst。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_exp(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_exp(vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求自然指数操作，将结果写入目的操作数dst。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_exp(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_min_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_min_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_min_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_min_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_int8_t,int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_min_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_uint8_t,uint8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_min_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_min_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_min_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_min_scalar(vector_bfloat16_t,bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最小值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_min_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_exp(vector_half&,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求自然指数操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_exp(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_exp(vector_float&,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src进行按元素求自然指数操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_exp(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_int8_t&,vector_int8_t,int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_uint8_t&,vector_uint8_t,uint8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_int16_t&,vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_uint16_t&,vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_half&,vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_bfloat16_t&,vector_bfloat16_t,bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_int32_t&,vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_uint32_t&,vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_float&,vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_add_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_int8_t,int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_add_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_uint8_t,uint8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_add_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_add_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_add_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_add_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_bfloat16_t,bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_add_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_add_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_add_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_add_scalar(vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`相加，得到计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_add_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_ln(vector_half&,vector_half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素取自然对数，将结果写入目的操作数`dst`。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ln(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_ln(vector_float&,vector_float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素取自然对数，将结果写入目的操作数`dst`。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ln(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_div(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_div(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int16_t asc_div(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint16_t asc_div(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_div(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_uint32_t asc_div(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_log(vector_half&,vector_half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素取自然对数，将结果写入目的操作数`dst`。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_log(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_log(vector_float&,vector_float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素取自然对数，将结果写入目的操作数`dst`。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_log(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_half&,vector_half,vector_half,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_float&,vector_float,vector_float,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_int16_t&,vector_int16_t,vector_int16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_uint16_t&,vector_uint16_t,vector_uint16_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_int32_t&,vector_int32_t,vector_int32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_div(vector_uint32_t&,vector_uint32_t,vector_uint32_t,vector_bool)
 * @archs 3510
 * @brief 该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_div(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_int8_t&,vector_int8_t,int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_int16_t&,vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_int32_t&,vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_uint8_t&,vector_uint8_t,uint8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_uint16_t&,vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_uint32_t&,vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_half&,vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_float&,vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_bfloat16_t&,vector_bfloat16_t,bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_max_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_int8_t,int8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int8_t asc_max_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_int16_t,int16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_max_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_int32_t,int32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_max_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_uint8_t,uint8_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_max_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_uint16_t,uint16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint16_t asc_max_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_uint32_t,uint32_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint32_t asc_max_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_half,half,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_max_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_float,float,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_max_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_arith
 * @function asc_max_scalar(vector_bfloat16_t,bfloat16_t,vector_bool)
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`按元素与标量`value`进行比较，得到最大值作为计算结果。计算公式如下：
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_max_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ARITH_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ARITH_H
#endif
