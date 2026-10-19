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
#error "compute/reg_fused.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_FUSED_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_FUSED_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_FUSED_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
#include "impl/c_api/reg_base_impl/reg_fused_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies elementwise ReLU to the half src under mask and writes the result to dst; elements not selected by mask are set to 0.
 *
 * @override_id asc_relu__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_relu(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies elementwise ReLU to the int32_t src under mask and writes the result to dst; elements not selected by mask are set to 0.
 *
 * @override_id asc_relu__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_relu(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies elementwise ReLU to the float src under mask and writes the result to dst; elements not selected by mask are set to 0.
 *
 * @override_id asc_relu__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_relu(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector half register with elementwise ReLU applied to src under mask; elements not selected by mask are set to 0 in the result.
 *
 * @override_id asc_relu__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_relu(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector int32_t register with elementwise ReLU applied to src under mask; elements not selected by mask are set to 0 in the result.
 *
 * @override_id asc_relu__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_relu(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector float register with elementwise ReLU applied to src under mask; elements not selected by mask are set to 0 in the result.
 *
 * @override_id asc_relu__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_relu(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that computes the absolute difference of half src0 and src1 elementwise under mask and writes the result to dst; masked-off elements are set to 0.
 *
 * @override_id asc_abs_sub__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that computes the absolute difference of float src0 and src1 elementwise under mask and writes the result to dst; masked-off elements are set to 0.
 *
 * @override_id asc_abs_sub__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector half register holding the absolute difference of src0 and src1 computed elementwise under mask; elements not selected by mask are set to 0.
 *
 * @override_id asc_abs_sub__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_abs_sub(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector float register holding the absolute difference of src0 and src1 computed elementwise under mask; elements not selected by mask are set to 0.
 *
 * @override_id asc_abs_sub__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_abs_sub(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that multiplies each half element of src by scalar value under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0.
 *
 * @override_id asc_axpy__Rvf16_vf16_f16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that multiplies each float element of src by scalar value under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0.
 *
 * @override_id asc_axpy__Rvf32_vf32_f32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_axpy(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that multiplies float src by scalar value under mask and writes the round-to-half results to even indexed positions of dst, zeroing the other positions.
 *
 * @override_id asc_mul_scalar_float2half_rn__Rvf16_vf32_f32_vb_icposeven
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器，half类型）。
 * @param [in] src 源操作数（矢量数据寄存器，float类型）。
 * @param [in] value 源操作数（标量，float类型）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 数据写入位置模式，取值为`ASC_POSITION_EVEN`（偶数位置）或`ASC_POSITION_ODD`（奇数位置）。选择偶数位置时，奇数位置置0；选择奇数位置时，偶数位置置0。
 */
__simd_callee__ inline void asc_mul_scalar_float2half_rn(
    vector_half& dst, vector_float src, float value, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that multiplies float src by scalar value under mask and writes the round-to-half results to odd indexed positions of dst, zeroing the other positions.
 *
 * @override_id asc_mul_scalar_float2half_rn__Rvf16_vf32_f32_vb_icposodd
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器，half类型）。
 * @param [in] src 源操作数（矢量数据寄存器，float类型）。
 * @param [in] value 源操作数（标量，float类型）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 数据写入位置模式，取值为`ASC_POSITION_EVEN`（偶数位置）或`ASC_POSITION_ODD`（奇数位置）。选择偶数位置时，奇数位置置0；选择奇数位置时，偶数位置置0。
 */
__simd_callee__ inline void asc_mul_scalar_float2half_rn(
    vector_half& dst, vector_float src, float value, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector_half register with src multiplied by scalar value and rounded to half under mask, written to even indexed positions; other positions are set to 0.
 *
 * @override_id asc_mul_scalar_float2half_rn__vf32_f32_vb_icposeven
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器，float类型）。
 * @param [in] value 源操作数（标量，float类型）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 数据写入位置模式，取值为`ASC_POSITION_EVEN`（偶数位置）或`ASC_POSITION_ODD`（奇数位置）。选择偶数位置时，奇数位置置0；选择奇数位置时，偶数位置置0。
 */
__simd_callee__ inline vector_half asc_mul_scalar_float2half_rn(
    vector_float src, float value, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector_half register with src multiplied by scalar value and rounded to half under mask, written to odd indexed positions; other positions are set to 0.
 *
 * @override_id asc_mul_scalar_float2half_rn__vf32_f32_vb_icposodd
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器，float类型）。
 * @param [in] value 源操作数（标量，float类型）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 数据写入位置模式，取值为`ASC_POSITION_EVEN`（偶数位置）或`ASC_POSITION_ODD`（奇数位置）。选择偶数位置时，奇数位置置0；选择奇数位置时，偶数位置置0。
 */
__simd_callee__ inline vector_half asc_mul_scalar_float2half_rn(
    vector_float src, float value, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the half dst register by src0 elementwise under mask, adds src1, and writes the accumulated result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_madd__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_madd(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the float dst register by src0 elementwise under mask, adds src1, and writes the accumulated result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_madd__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_madd(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the bfloat16_t dst register by src0 elementwise under mask, adds src1, and writes the accumulated result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_madd__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_madd(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies Leaky ReLU to float src under mask (src>0 keeps src, otherwise src*alpha) and writes the result to dst; masked-off elements are set to 0.
 *
 * @override_id asc_leakyrelu__Rvf32_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_leakyrelu(vector_float& dst, vector_float src, float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies Leaky ReLU to half src under mask (src>0 keeps src, otherwise src*alpha) and writes the result to dst; masked-off elements are set to 0.
 *
 * @override_id asc_leakyrelu__Rvf16_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_leakyrelu(vector_half& dst, vector_half src, half alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector float register holding Leaky ReLU of src under mask (src>0 keeps src, otherwise src*alpha); elements not selected by mask are set to 0.
 *
 * @override_id asc_leakyrelu__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_leakyrelu(vector_float src, float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector half register holding Leaky ReLU of src under mask (src>0 keeps src, otherwise src*alpha); elements not selected by mask are set to 0.
 *
 * @override_id asc_leakyrelu__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_leakyrelu(vector_half src, half alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Reg fused API: computes e^(src0 - src1) elementwise under mask and writes the result into dst via the output-parameter form.
 *
 * @override_id asc_exp_sub__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与输出元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Reg fused API: computes e^(src0 - src1) elementwise under mask and returns the vector_float result (return-value form).
 *
 * @override_id asc_exp_sub__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与输出元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_exp_sub(vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline vector_float asc_exp_sub_half2float(
    vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_float asc_exp_sub_half2float(
    vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies PReLU to float src0 under mask using elementwise alpha slope and writes the result to dst; masked-off elements are set to 0.
 *
 * @override_id asc_prelu__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src0`、`alpha`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_prelu(vector_float& dst, vector_float src0, vector_float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Void-return interface that applies PReLU to half src0 under mask using elementwise alpha slope and writes the result to dst; masked-off elements are set to 0.
 *
 * @override_id asc_prelu__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src0`、`alpha`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_prelu(vector_half& dst, vector_half src0, vector_half alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector float register holding PReLU of src0 under mask with the vector alpha slope; elements not selected by mask are set to 0.
 *
 * @override_id asc_prelu__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_prelu(vector_float src0, vector_float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns a vector half register holding PReLU of src0 under mask with the vector alpha slope; elements not selected by mask are set to 0.
 *
 * @override_id asc_prelu__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_prelu(vector_half src0, vector_half alpha, vector_bool mask);

//=================asc_mula==================
/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the int16_t registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the uint16_t registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the half registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the bfloat16_t registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the int32_t registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the uint32_t registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Multiplies the float registers src0 and src1 elementwise under mask, accumulates onto dst, and writes the result back to dst; masked-off elements are set to 0 (void return).
 *
 * @override_id asc_mula__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

//=================asc_fma==================
/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns the vector half register holding src0 * src1 + src2 computed elementwise under mask; elements not selected by mask are set to 0 in the result.
 *
 * @override_id asc_fma__vf16_vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] src2 源操作数2（矢量数据寄存器），作为乘加运算的累加值。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_fma(vector_half src0, vector_half src1, vector_half src2, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns the vector bfloat16_t register holding src0 * src1 + src2 computed elementwise under mask; elements not selected by mask are set to 0 in the result.
 *
 * @override_id asc_fma__vbf16_vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] src2 源操作数2（矢量数据寄存器），作为乘加运算的累加值。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_fma(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @brief Returns the vector float register holding src0 * src1 + src2 computed elementwise under mask; elements not selected by mask are set to 0 in the result.
 *
 * @override_id asc_fma__vf32_vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] src2 源操作数2（矢量数据寄存器），作为乘加运算的累加值。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_fma(vector_float src0, vector_float src1, vector_float src2, vector_bool mask);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
//=================asc_fms==================
__simd_callee__ inline vector_half asc_fms(vector_half src0, vector_half src1, vector_half src2, vector_bool mask);

__simd_callee__ inline vector_bfloat16_t asc_fms(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask);

__simd_callee__ inline vector_float asc_fms(vector_float src0, vector_float src1, vector_float src2, vector_bool mask);

//=================asc_fnma==================
__simd_callee__ inline vector_half asc_fnma(vector_half src0, vector_half src1, vector_half src2, vector_bool mask);

__simd_callee__ inline vector_bfloat16_t asc_fnma(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask);

__simd_callee__ inline vector_float asc_fnma(vector_float src0, vector_float src1, vector_float src2, vector_bool mask);

//=================asc_fnms==================
__simd_callee__ inline vector_half asc_fnms(vector_half src0, vector_half src1, vector_half src2, vector_bool mask);

__simd_callee__ inline vector_bfloat16_t asc_fnms(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask);

__simd_callee__ inline vector_float asc_fnms(vector_float src0, vector_float src1, vector_float src2, vector_bool mask);
#endif

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar_float2half_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-return interface that multiplies float src by scalar value, rounds to half under mask, and writes the result to even indexed positions of dst; replaced by asc_mul_scalar_float2half_rn.
 *
 * @override_id asc_muls__Rvf16_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_muls(vector_half& dst, vector_float src, float value, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar_float2half_rn)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-return interface that multiplies float src by scalar value, rounds to half under mask, and writes the result to odd indexed positions of dst; replaced by asc_mul_scalar_float2half_rn.
 *
 * @override_id asc_muls_v2__Rvf16_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_muls_v2(vector_half& dst, vector_float src, float value, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_exp_sub_half2float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_exp_sub overload that subtracts half src1 from src0 elementwise and writes exp of the difference into the float dst; migrate to asc_exp_sub_half2float with ASC_POSITION_EVEN.
 *
 * @override_id asc_exp_sub__Rvf32_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算；未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_exp_sub_half2float)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_exp_sub_v2 half-source overload that writes exp(src0 - src1) into the float dst; migrate to asc_exp_sub_half2float with ASC_POSITION_ODD.
 *
 * @override_id asc_exp_sub_v2__Rvf32_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算；未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_exp_sub_v2(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask);

ASC_DEPRECATED(9.1.0, "2027/09/07", asc_exp_sub)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_exp_sub_v2 float-source overload that writes exp(src0 - src1) into the float dst; migrate to asc_exp_sub(dst, src0, src1, mask).
 *
 * @override_id asc_exp_sub_v2__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算；未参与计算的元素在输出中置0。
 */
__simd_callee__ inline void asc_exp_sub_v2(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_FUSED_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_FUSED_H
#endif
