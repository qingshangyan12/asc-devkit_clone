/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reg_convert.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_CONVERT_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_REG_CONVERT_H
#define INCLUDE_C_API_REG_COMPUTE_REG_CONVERT_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_convert_intf_impl.h"
#endif

// ASC_RETURN_VALUE_VCONV_API
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint32__vi16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_uint32_t asc_int162uint32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint32__vi16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_uint32_t asc_int162uint32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int32_t asc_int642int32(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int32_t asc_int642int32(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32_sat__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int32_t asc_int642int32_sat(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32_sat__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int32_t asc_int642int32_sat(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82half__vi8_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_half asc_int82half(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82half__vi8_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_half asc_int82half(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8__vu16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_uint162uint8(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8__vu16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_uint162uint8(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8_sat__vu16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_uint162uint8_sat(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8_sat__vu16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_uint162uint8_sat(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16__vi32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline vector_uint16_t asc_int322uint16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16__vi32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline vector_uint16_t asc_int322uint16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16_sat__vi32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline vector_uint16_t asc_int322uint16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16_sat__vi32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline vector_uint16_t asc_int322uint16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162float__vi16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_int162float(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162float__vi16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_int162float(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int16__vi8_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int16_t asc_int82int16(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int16__vi8_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int16_t asc_int82int16(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int64__vi32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 源操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline vector_int64_t asc_int322int64(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int64__vi32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 源操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline vector_int64_t asc_int322int64(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162int32__vi16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_int162int32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162int32__vi16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_int162int32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8__vi16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_int162uint8(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8__vi16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_int162uint8(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8_sat__vi16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_int162uint8_sat(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8_sat__vi16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_int162uint8_sat(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint16__vu8_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_uint16_t asc_uint82uint16(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint16__vu8_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_uint16_t asc_uint82uint16(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16__vu32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint16_t asc_uint322uint16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16__vu32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint16_t asc_uint322uint16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16_sat__vu32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint16_t asc_uint322uint16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16_sat__vu32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint16_t asc_uint322uint16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82half__vhf8_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_half asc_hif82half(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82half__vhf8_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_half asc_hif82half(
    vector_hifloat8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16__vi32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_int322int16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16__vi32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_int322int16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16_sat__vi32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_int322int16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16_sat__vi32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_int322int16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ro(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ro(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ro_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ro_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_half asc_float2half_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_float2int16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna_sat__vf32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna_sat__vf32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_int64_t asc_float2int64_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16__vu32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_uint322int16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16__vu32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_uint322int16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16_sat__vu32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_uint322int16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16_sat__vu32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int16_t asc_uint322int16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82half__vu8_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_half asc_uint82half(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82half__vu8_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_half asc_uint82half(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint32__vu16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_uint32_t asc_uint162uint32(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint32__vu16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline vector_uint32_t asc_uint162uint32(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_ru__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_ru(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_ru__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_ru(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rd__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rd(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rd__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rd(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rn__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rn(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rn__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rn(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rz__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rz(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rz__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rz(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rna__vi64_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rna(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rna__vi64_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_float asc_int642float_rna(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode);

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos);

__simd_callee__ inline void asc_cast(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos);
// ==========asc_bfloat162half(rd/ru/rz/rn/rna)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rd(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd_sat__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rd_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_ru(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru_sat__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_ru_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rz(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz_sat__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rz_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rn(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn_sat__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rn_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rna(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna_sat__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rna_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rd(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rd_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_ru(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_ru_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rz(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rz_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rn(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rn_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rna(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rna_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

// ==========asc_half2bfloat16==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rd__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rd(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rn__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rn(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rna__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rna(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_ru__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_ru(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rz__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rz(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rd__Rvbf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rd(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rn__Rvbf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rn(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rna__Rvbf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rna(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_ru__Rvbf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_ru(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rz__Rvbf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rz(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

// ==========asc_int4x22int16==========
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22int16__Rvi16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22int16(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

// ==========asc_int4x22bfloat16==========
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22bfloat16__Rvbf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22bfloat16(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

// ==========asc_int322float(rd/ru/rz/rn/rna)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rd__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_int322float_rd(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_ru__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_int322float_ru(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rz__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_int322float_rz(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rn__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_int322float_rn(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rna__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_int322float_rna(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rd__Rvf32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rd(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_ru__Rvf32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_ru(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rz__Rvf32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rz(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rn__Rvf32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rn(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rna__Rvf32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rna(vector_float& dst, vector_int32_t src, vector_bool mask);

// ==========asc_half2int16(rd/rn/rna/ru/rz)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rn(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn_sat__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rn_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rna(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna_sat__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rna_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rd(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd_sat__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rd_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_ru(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru_sat__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_ru_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rz(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz_sat__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rz_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rn(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn_sat__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rn_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rna(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna_sat__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rna_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rd(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd_sat__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rd_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_ru(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru_sat__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_ru_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rz(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz_sat__Rvi16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rz_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

// ==========asc_half2int4x2==========
// rd - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// ru - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// rz - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// rn - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// rna - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

//===================asc_int162half=================
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rd__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_int162half_rd(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rn__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_int162half_rn(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rna__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_int162half_rna(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_ru__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_int162half_ru(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rz__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_int162half_rz(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rd__Rvf16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rd(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rn__Rvf16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rn(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rna__Rvf16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rna(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_ru__Rvf16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_ru(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rz__Rvf16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rz(vector_half& dst, vector_int16_t src, vector_bool mask);

//================asc_int4x22half==============
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22half__Rvf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22half(vector_half& dst, vector_int4x2_t src, vector_bool mask);

// ==========asc_float2int32_rd/ru/rz/rn/rna)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rd__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rd(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rd_sat__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rd_sat(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_ru__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_ru(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_ru_sat__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_ru_sat(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rz__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rz(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rz_sat__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rz_sat(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rn__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rn(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rn_sat__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rn_sat(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rna__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rna(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rna_sat__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_int32_t asc_float2int32_rna_sat(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rd__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rd(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rd_sat__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rd_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_ru__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_ru(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_ru_sat__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_ru_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rz__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rz(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rz_sat__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rz_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rn__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rn(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rn_sat__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rn_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rna__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rna(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rna_sat__Rvi32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rna_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

// ==========asc_ceil==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_ceil(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_ceil(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_ceil(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_ceil(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_ceil(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_ceil(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_floor==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_floor(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_floor(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_floor(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_floor(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_floor(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_floor(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_rint==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_rint(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_rint(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_rint(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_rint(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_rint(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_rint(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_round==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_round(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_round(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_round(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_round(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_round(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_round(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_trunc==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__vf16_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_trunc(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__vbf16_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_trunc(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__vf32_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_trunc(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src一致，掩码为真位置写入向零方向截断结果（仍以原浮点类型表示整数），掩码为假位置清零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_trunc(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src一致，掩码为真位置写入向零方向截断结果（仍以原浮点类型表示整数），掩码为假位置清零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_trunc(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src一致，掩码为真位置写入向零方向截断结果（仍以原浮点类型表示整数），掩码为假位置清零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_trunc(vector_float& dst, vector_float src, vector_bool mask);

// ==========Position-aware conversion overloads==========
__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint32__Rvu32_vi16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint32(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint32__Rvu32_vi16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint32(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32__Rvi32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642int32(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32__Rvi32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642int32(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32_sat__Rvi32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642int32_sat(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32_sat__Rvi32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642int32_sat(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rd(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rd(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rd_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rd_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rn(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rn(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rn_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rn_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rna(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rna(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rna_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rna_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_ru(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_ru(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_ru_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_ru_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rz(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rz(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rz_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2int8_rz_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82half__Rvf16_vi8_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82half(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82half__Rvf16_vi8_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82half(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8__Rvu8_vu16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint162uint8(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8__Rvu8_vu16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint162uint8(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8_sat__Rvu8_vu16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint162uint8_sat(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8_sat__Rvu8_vu16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint162uint8_sat(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16__Rvu16_vi32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16__Rvu16_vi32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16_sat__Rvu16_vi32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16_sat(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16_sat__Rvu16_vi32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16_sat(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162float__Rvf32_vi16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162float(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162float__Rvf32_vi16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162float(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__Rvf32_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__Rvf32_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int16__Rvi16_vi8_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82int16(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int16__Rvi16_vi8_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82int16(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int64__Rvi64_vi32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 源操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322int64(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int64__Rvi64_vi32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 源操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322int64(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rh(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rh(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rna(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rna(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162int32__Rvi32_vi16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162int32(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162int32__Rvi32_vi16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162int32(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_bfloat162float(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_bfloat162float(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__Rvf32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162float(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__Rvf32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162float(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8__Rvu8_vi16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int162uint8(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8__Rvu8_vi16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int162uint8(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8_sat__Rvu8_vi16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int162uint8_sat(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8_sat__Rvu8_vi16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int162uint8_sat(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82uint16__Rvu16_vu8_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82uint16(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82uint16__Rvu16_vu8_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82uint16(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16__Rvu16_vu32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322uint16(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16__Rvu16_vu32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322uint16(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16_sat__Rvu16_vu32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322uint16_sat(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16_sat__Rvu16_vu32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322uint16_sat(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82half__Rvf16_vhf8_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_hif82half(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82half__Rvf16_vhf8_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_hif82half(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16__Rvi16_vi32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int322int16(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16__Rvi16_vi32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int322int16(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16_sat__Rvi16_vi32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int322int16_sat(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16_sat__Rvi16_vi32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int322int16_sat(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_ru(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_ru(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rna(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rna(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rd(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rd(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rn(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rn(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rz(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rz(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rn(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rn(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_ru(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_ru(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rd(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rd(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rz(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rz(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rna(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rna(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rz(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rz(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rz_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rz_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rn(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rn(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rn_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rn_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rna(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rna(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rna_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rna_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rd(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rd(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rd_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_rd_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ro(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ro(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ro_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ro_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ru(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ru(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ru_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2half_ru_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__vbf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__vbf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rz(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rz(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rz_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rz_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rd(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rd(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rd_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rd_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rna(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rna(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rna_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rna_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_ru(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_ru(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_ru_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_ru_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rn(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rn(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rn_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_float2int16_rn_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rn(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rn(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn_sat__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rn_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rn_sat__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rn_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rz(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rz(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz_sat__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rz_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rz_sat__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rz_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_ru(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_ru(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru_sat__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_ru_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_ru_sat__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_ru_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rd(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rd(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd_sat__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rd_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rd_sat__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rd_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rna(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rna(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna_sat__Rvi64_vf32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rna_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int64_rna_sat__Rvi64_vf32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int64_rna_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16__Rvi16_vu32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322int16(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16__Rvi16_vu32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322int16(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16_sat__Rvi16_vu32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322int16_sat(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16_sat__Rvi16_vu32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_uint322int16_sat(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82half__Rvf16_vu8_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82half(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82half__Rvf16_vu8_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82half(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint32__Rvu32_vu16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint32(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint32__Rvu32_vu16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint32(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_ru__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_ru(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_ru__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_ru(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rd__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rd(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rd__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rd(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rn__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rn(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rn__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rn(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rz__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rz(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rz__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rz(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rna__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rna(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rna__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置，其他位置清零&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_int642float_rna(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__vf16_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__vf16_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rd(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rd(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rd_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rd_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_ru(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_ru(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_ru_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_ru_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rna(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rna(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rna_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rna_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rn(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rn(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rn_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rn_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rz(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rz(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rz_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译期标签分发，通过编译期重载选择对应实现），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置，其他位置清零；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置，其他位置清零。
 */
__simd_callee__ inline void asc_half2uint8_rz_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

// ==========vector_int4x2 position-aware conversion overloads==========
__simd_callee__ inline vector_int16_t asc_int4x22int16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int16_t asc_int4x22int16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int16_t asc_int4x22int16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int16_t asc_int4x22int16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_half asc_int4x22half(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_half asc_int4x22half(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_half asc_int4x22half(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_half asc_int4x22half(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_int4x22bfloat16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_int4x22bfloat16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_int4x22bfloat16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_int4x22bfloat16(
    vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rd_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rna_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_ru_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int4x2_t asc_half2int4x2_rz_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int4x22int16(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22int16(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22int16(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22int16(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22half(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22half(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22half(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22half(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22bfloat16(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22bfloat16(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22bfloat16(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int4x22bfloat16(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rd_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rn_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rna_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_ru_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_half2int4x2_rz_sat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

// ==========Deprecated conversion interfaces==========

// Legacy overloads without explicit position arguments.
ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd_sat__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru_sat__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz_sat__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn_sat__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna_sat__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro_sat__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16__Rvi16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16_sat__Rvi16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e4m32float__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162float__Rvf32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16__Rvi16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16_sat__Rvi16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_sat__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd_sat__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru_sat__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz_sat__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn_sat__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna_sat__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh_sat__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna_sat__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162float__Rvf32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint32__Rvu32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8__Rvu8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8_sat__Rvu8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rn__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rna__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rd__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_ru__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rz__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82half__Rvf16_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162int32__Rvi32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_sat__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82half__Rvf16_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82half__Rvf16_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint32__Rvu32_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82float__Rvf32_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd_sat__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn_sat__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna_sat__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru_sat__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz_sat__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int16__Rvi16_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8__Rvu8_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8_sat__Rvu8_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rd__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rn__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rna__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_ru__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rz__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e5m22float__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int64__Rvi64_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16__Rvu16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16_sat__Rvu16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32__Rvi32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32_sat__Rvi32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask, ASC_POSITION_EVEN))
__simd_callee__ inline void asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd_sat__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn_sat__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna_sat__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru_sat__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz_sat__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_sat__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd_sat__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn_sat__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna_sat__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru_sat__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz_sat__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e2m1x2_rd(
        vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rd__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e2m1x2_rn(
        vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rn__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e2m1x2_rna(
        vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rna__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e2m1x2_ru(
        vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_ru__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e2m1x2_rz(
        vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rz__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_sat__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_sat__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82uint32__Rvu32_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16_sat__Rvu16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16__Rvu16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e2m1x22bfloat16__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e1m2x2_rd(
        vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rd__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e1m2x2_rn(
        vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rn__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e1m2x2_rna(
        vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rna__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e1m2x2_ru(
        vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_ru__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_bfloat162e1m2x2_rz(
        vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rz__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e1m2x22bfloat16__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int32__Rvi32_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_sat__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(
    9.2.0, "2027/09/07", asc_half2float(vector_float& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2float__Rvf32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2float(vector_float& dst, vector_half src, vector_bool mask);
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rd_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rd_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ru_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ru_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rz_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rz_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rn_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rn_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rna_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_rna_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_ro)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ro_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_ro_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2half_ro_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322int16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16_v2__Rvi16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322int16_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322int16_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16_sat_v2__Rvi16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322int16_sat_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e4m32float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e4m32float_v2__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e4m32float_v2(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e4m32float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e4m32float_v3__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e4m32float_v3(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e4m32float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e4m32float_v4__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e4m32float_v4(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162float_v2__Rvf32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162float_v2(vector_float& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22int16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22int16_v2__Rvi16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22int16_v2(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22int16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22int16_v3__Rvi16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22int16_v3(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22int16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22int16_v4__Rvi16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22int16_v4(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322int16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16_v2__Rvi16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322int16_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322int16_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16_sat_v2__Rvi16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322int16_sat_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_v2__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_v2(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_sat_v2__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_sat_v2(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_v3__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_v3(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_sat_v3__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_sat_v3(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_v4__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_v4(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint8_sat_v4__Rvu8_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int322uint8_sat_v4(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rd_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rd_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_ru_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_ru_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rz_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rz_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rn_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rn_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rna_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int8_rna_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2hif8_rh)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rh_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2hif8_rh_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh_sat_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2hif8_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rna_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2hif8_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna_sat_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162float_v2__Rvf32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162float_v2(vector_float& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22bfloat16_v2__Rvbf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22bfloat16_v2(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22bfloat16_v3__Rvbf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22bfloat16_v3(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22bfloat16_v4__Rvbf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22bfloat16_v4(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162uint32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint32_v2__Rvu32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162uint32_v2(vector_uint32_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint162uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8_v2__Rvu8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint162uint8_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint162uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8_sat_v2__Rvu8_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint162uint8_sat_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rn_v2__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rn_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rna_v2__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rna_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rd_v2__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rd_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_ru_v2__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_ru_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rz_v2__Rvf32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642float_rz_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int82half)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82half_v2__Rvf16_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82half_v2(vector_half& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162int32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162int32_v2__Rvi32_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162int32_v2(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_v2__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_sat_v2__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_sat_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_v3__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_sat_v3__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_sat_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_v4__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint8_sat_v4__Rvu8_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint8_sat_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_hif82half)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82half_v2__Rvf16_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_hif82half_v2(vector_half& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rd_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_ru_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rz_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rn_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4x2_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int4x2_rna_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint82half)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82half_v2__Rvf16_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint82half_v2(vector_half& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint162uint32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint32_v2__Rvu32_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint162uint32_v2(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_hif82float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82float_v2__Rvf32_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_hif82float_v2(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_hif82float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82float_v3__Rvf32_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_hif82float_v3(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_hif82float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82float_v4__Rvf32_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_hif82float_v4(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22half)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22half_v2__Rvf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22half_v2(vector_half& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22half)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22half_v3__Rvf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22half_v3(vector_half& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int4x22half)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int4x22half_v4__Rvf16_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22half_v4(vector_half& dst, vector_int4x2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int82int16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int16_v2__Rvi16_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82int16_v2(vector_int16_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162uint8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8_v2__Rvu8_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162uint8_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162uint8_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8_sat_v2__Rvu8_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162uint8_sat_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rd_v2__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rd_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rn_v2__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rn_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rna_v2__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rna_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_ru_v2__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_ru_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rz_v2__Rvi32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int32_rz_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e5m22float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e5m22float_v2__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e5m22float_v2(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e5m22float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e5m22float_v3__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e5m22float_v3(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e5m22float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e5m22float_v4__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e5m22float_v4(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322int64)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int64_v2__Rvi64_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322int64_v2(vector_int64_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16_v2__Rvu16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322uint16_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322uint16_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16_sat_v2__Rvu16_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322uint16_sat_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642int32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32_v2__Rvi32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642int32_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642int32_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32_sat_v2__Rvi32_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int642int32_sat_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint82uint16)
__simd_callee__ inline void asc_uint82uint16_v2(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rd_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rd_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rn_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rn_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rna_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rna_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_ru_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_ru_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rz_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int64_rz_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e4m3_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_sat_v2__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_sat_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e4m3_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_v2__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e4m3_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_sat_v3__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_sat_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e4m3_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_v3__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e4m3_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_sat_v4__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_sat_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e4m3_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e4m3_rn_v4__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e4m3_rn_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rd_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rd_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rd_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rn_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rn_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rna_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rna_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_ru_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_ru_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_ru_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rz_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rz_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int16_rz_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rd_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v2(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rd_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v3(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rd_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v4(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rn_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v2(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rn_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v3(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rn_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v4(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rna_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v2(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rna_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v3(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rna_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v4(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_ru_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v2(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_ru_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v3(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_ru_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v4(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rz_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v2(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rz_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v3(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e2m1x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e2m1x2_rz_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v4(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rh_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_sat_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rh)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rh_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_sat_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rh)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rh_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_sat_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rh)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rh_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rh_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_sat_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_sat_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rna_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_sat_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2hif8_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2hif8_rna_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2hif8_rna_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint82uint32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82uint32_v2__Rvu32_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint82uint32_v2(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint82uint32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82uint32_v3__Rvu32_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint82uint32_v3(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint82uint32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82uint32_v4__Rvu32_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint82uint32_v4(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint16_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16_sat_v2__Rvu16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint16_sat_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint322uint16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16_v2__Rvu16_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_uint322uint16_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e2m1x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e2m1x22bfloat16_v2__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e2m1x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e2m1x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e2m1x22bfloat16_v3__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e2m1x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e2m1x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e2m1x22bfloat16_v4__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e2m1x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rd_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rd_v2(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rd_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rd_v3(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rd)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rd_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rd_v4(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rn_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rn_v2(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rn_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rn_v3(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rn_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rn_v4(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rna_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rna_v2(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rna_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rna_v3(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rna)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rna_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rna_v4(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_ru_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_ru_v2(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_ru_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_ru_v3(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_ru)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_ru_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_ru_v4(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rz_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rz_v2(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rz_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rz_v3(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162e1m2x2_rz)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162e1m2x2_rz_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_bfloat162e1m2x2_rz_v4(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e1m2x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e1m2x22bfloat16_v2__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e1m2x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e1m2x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e1m2x22bfloat16_v3__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e1m2x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_e1m2x22bfloat16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_e1m2x22bfloat16_v4__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_e1m2x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int82int32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int32_v2__Rvi32_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82int32_v2(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int82int32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int32_v3__Rvi32_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82int32_v3(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int82int32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int32_v4__Rvi32_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int82int32_v4(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e5m2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_v2__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e5m2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_v3__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e5m2_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_v4__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e5m2_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_sat_v2__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_sat_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e5m2_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_sat_v3__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_sat_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2e5m2_rn_sat)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2e5m2_rn_sat_v4__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2e5m2_rn_sat_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2float_v2__Rvf32_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2float_v2(vector_float& dst, vector_half src, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_CONVERT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_CONVERT_H

#endif
