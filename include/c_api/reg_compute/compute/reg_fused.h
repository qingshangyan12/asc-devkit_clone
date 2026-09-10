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

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_relu__Rvf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素执行ReLU操作，得到计算结果。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_relu(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_relu__Rvi32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素执行ReLU操作，得到计算结果。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_relu(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_relu__Rvf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素执行ReLU操作，得到计算结果。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_relu(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_relu__vf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素执行ReLU操作，得到计算结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_relu(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_relu__vi32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素执行ReLU操作，得到计算结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int32_t asc_relu(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_relu__vf32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src`按元素执行ReLU操作，得到计算结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_relu(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_abs_sub__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相减后取绝对值，得到计算结果。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_abs_sub__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相减后取绝对值，得到计算结果。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_abs_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_abs_sub__vf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相减后取绝对值，得到计算结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_abs_sub(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_abs_sub__vf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相减后取绝对值，得到计算结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_abs_sub(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_axpy__Rvf16_vf16_f16_vb
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`中的每个元素与标量`value`求积后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_axpy__Rvf32_vf32_f32_vb
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`中的每个元素与标量`value`求积后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_axpy(vector_float& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_muls__Rvf16_vf32_f32_vb
 * @archs 3510
 * @brief 对源操作数src和value进行乘法运算再按照数据类型转换的ROUND舍入模式转成half类型，根据mask将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_muls(vector_half& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_muls_v2__Rvf16_vf32_f32_vb
 * @archs 3510
 * @brief 对源操作数src和value进行乘法运算再按照数据类型转换的ROUND舍入模式转成half类型，根据mask将结果写入目的操作数dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_muls_v2(vector_half& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mul_scalar_float2half_rn__Rvf16_vf32_f32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`（float类型）按元素与标量`value`相乘后，按照RINT舍入模式转换为half类型，得到计算结果。
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
 * @override_id asc_mul_scalar_float2half_rn__Rvf16_vf32_f32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`（float类型）按元素与标量`value`相乘后，按照RINT舍入模式转换为half类型，得到计算结果。
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
 * @override_id asc_mul_scalar_float2half_rn__vf32_f32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`（float类型）按元素与标量`value`相乘后，按照RINT舍入模式转换为half类型，得到计算结果。
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
 * @override_id asc_mul_scalar_float2half_rn__vf32_f32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将源操作数`src`（float类型）按元素与标量`value`相乘后，按照RINT舍入模式转换为half类型，得到计算结果。
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
 * @override_id asc_madd__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`对目的操作数`dst`与源操作数`src0`按元素相乘后，与源操作数`src1`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_madd(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_madd__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`对目的操作数`dst`与源操作数`src0`按元素相乘后，与源操作数`src1`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_madd(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_madd__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`对目的操作数`dst`与源操作数`src0`按元素相乘后，与源操作数`src1`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_madd(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_leakyrelu__Rvf32_vf32_f32_vb
 * @archs 3510
 * @brief 根据掩码对源矢量数据寄存器中的元素执行Leaky ReLU操作，并将结果通过函数返回值返回或写入目的矢量数据寄存器。源操作数中大于0的元素直接写入输出结果，小于或等于0的元素乘以标量立即数后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_leakyrelu(vector_float& dst, vector_float src, float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_leakyrelu__Rvf16_vf16_f16_vb
 * @archs 3510
 * @brief 根据掩码对源矢量数据寄存器中的元素执行Leaky ReLU操作，并将结果通过函数返回值返回或写入目的矢量数据寄存器。源操作数中大于0的元素直接写入输出结果，小于或等于0的元素乘以标量立即数后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src`一致。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_leakyrelu(vector_half& dst, vector_half src, half alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_leakyrelu__vf32_f32_vb
 * @archs 3510
 * @brief 根据掩码对源矢量数据寄存器中的元素执行Leaky ReLU操作，并将结果通过函数返回值返回或写入目的矢量数据寄存器。源操作数中大于0的元素直接写入输出结果，小于或等于0的元素乘以标量立即数后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline vector_float asc_leakyrelu(vector_float src, float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_leakyrelu__vf16_f16_vb
 * @archs 3510
 * @brief 根据掩码对源矢量数据寄存器中的元素执行Leaky ReLU操作，并将结果通过函数返回值返回或写入目的矢量数据寄存器。源操作数中大于0的元素直接写入输出结果，小于或等于0的元素乘以标量立即数后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（标量）。数据类型须与`src`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline vector_half asc_leakyrelu(vector_half src, half alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_exp_sub__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`将`src0`与`src1`按元素相减，并计算以差值为指数的自然指数函数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与输出元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_exp_sub_half2float__Rvf32_vf16_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src0`与`src1`按元素相减，并计算以差值为指数的自然指数函数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与输出元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 指示源操作数读取位置。取值为`ASC_POSITION_EVEN`时读取偶数索引元素，取值为`ASC_POSITION_ODD`时读取奇数索引元素。
 */
__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_exp_sub_half2float__Rvf32_vf16_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src0`与`src1`按元素相减，并计算以差值为指数的自然指数函数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与输出元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 指示源操作数读取位置。取值为`ASC_POSITION_EVEN`时读取偶数索引元素，取值为`ASC_POSITION_ODD`时读取奇数索引元素。
 */
__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
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
 * @override_id asc_prelu__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @brief 根据掩码对源操作数矢量数据寄存器`src0`中的元素执行PReLU（Parametric ReLU）运算，负半轴斜率由矢量数据寄存器`alpha`中的对应元素指定，并将结果通过函数返回值返回或写入目的操作数矢量数据寄存器。`src0`大于0时直接写入输出结果，否则将`src0`与`alpha`相乘后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src0`、`alpha`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_prelu(vector_float& dst, vector_float src0, vector_float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_prelu__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @brief 根据掩码对源操作数矢量数据寄存器`src0`中的元素执行PReLU（Parametric ReLU）运算，负半轴斜率由矢量数据寄存器`alpha`中的对应元素指定，并将结果通过函数返回值返回或写入目的操作数矢量数据寄存器。`src0`大于0时直接写入输出结果，否则将`src0`与`alpha`相乘后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [out] dst 目的操作数（矢量数据寄存器）。仅无返回值类型接口包含该参数，数据类型须与`src0`、`alpha`一致。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_prelu(vector_half& dst, vector_half src0, vector_half alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_prelu__vf32_vf32_vb
 * @archs 3510
 * @brief 根据掩码对源操作数矢量数据寄存器`src0`中的元素执行PReLU（Parametric ReLU）运算，负半轴斜率由矢量数据寄存器`alpha`中的对应元素指定，并将结果通过函数返回值返回或写入目的操作数矢量数据寄存器。`src0`大于0时直接写入输出结果，否则将`src0`与`alpha`相乘后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline vector_float asc_prelu(vector_float src0, vector_float alpha, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_prelu__vf16_vf16_vb
 * @archs 3510
 * @brief 根据掩码对源操作数矢量数据寄存器`src0`中的元素执行PReLU（Parametric ReLU）运算，负半轴斜率由矢量数据寄存器`alpha`中的对应元素指定，并将结果通过函数返回值返回或写入目的操作数矢量数据寄存器。`src0`大于0时直接写入输出结果，否则将`src0`与`alpha`相乘后写入输出结果。掩码对应位置为1的元素参与计算，为0的元素在输出结果中置零。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] alpha 负半轴斜率（矢量数据寄存器）。数据类型须与`src0`一致。
 * @param [in] mask 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline vector_half asc_prelu(vector_half src0, vector_half alpha, vector_bool mask);

//=================asc_mula==================
/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvi16_vi16_vi16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvu16_vu16_vu16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvf16_vf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvbf16_vbf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvi32_vi32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvu32_vu32_vu32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_mula__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与目的操作数`dst`中的对应元素相加，将结果写入`dst`。其中，`dst`既作为输入累加器参与计算，又用于存储最终结果。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_mula(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

//=================asc_fma==================
/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_fma__vf16_vf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与源操作数`src2`中的对应元素相加，将计算结果作为返回值返回。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] src2 源操作数2（矢量数据寄存器），作为乘加运算的累加值。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_fma(vector_half src0, vector_half src1, vector_half src2, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_fma__vbf16_vbf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与源操作数`src2`中的对应元素相加，将计算结果作为返回值返回。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] src2 源操作数2（矢量数据寄存器），作为乘加运算的累加值。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_fma(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_fused
 * @override_id asc_fma__vf32_vf32_vf32_vb
 * @archs 3510
 * @brief 根据`mask`对源操作数`src0`、`src1`按元素相乘后，与源操作数`src2`中的对应元素相加，将计算结果作为返回值返回。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] src2 源操作数2（矢量数据寄存器），作为乘加运算的累加值。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_float asc_fma(vector_float src0, vector_float src1, vector_float src2, vector_bool mask);

[[deprecated("NOTICE: asc_exp_sub with half inputs and without src_pos is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_exp_sub__Rvf32_vf16_vf16_vb
 * @archs 3510
 * @brief **输入数据类型为`half`且不带`src_pos`参数的`asc_exp_sub`原型以及`asc_exp_sub_v2`原型已废弃。新接口请参见[asc_exp_sub](../reg_compute/reg_fused/asc_exp_sub.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算；未参与计算的元素在输出中置0。
 */
             "Please use asc_exp_sub_half2float with src_pos instead.")]] __simd_callee__ inline void
asc_exp_sub(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask);

[[deprecated("NOTICE: asc_exp_sub_v2 with half inputs is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_exp_sub_v2__Rvf32_vf16_vf16_vb
 * @archs 3510
 * @brief **输入数据类型为`half`且不带`src_pos`参数的`asc_exp_sub`原型以及`asc_exp_sub_v2`原型已废弃。新接口请参见[asc_exp_sub](../reg_compute/reg_fused/asc_exp_sub.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算；未参与计算的元素在输出中置0。
 */
             "Please use asc_exp_sub_half2float with src_pos instead.")]] __simd_callee__ inline void
asc_exp_sub_v2(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask);

[[deprecated("NOTICE: asc_exp_sub_v2 with float inputs is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_exp_sub_v2__Rvf32_vf32_vf32_vb
 * @archs 3510
 * @brief **输入数据类型为`half`且不带`src_pos`参数的`asc_exp_sub`原型以及`asc_exp_sub_v2`原型已废弃。新接口请参见[asc_exp_sub](../reg_compute/reg_fused/asc_exp_sub.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src0 源操作数0（矢量数据寄存器）。
 * @param [in] src1 源操作数1（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算；未参与计算的元素在输出中置0。
 */
             "Please use asc_exp_sub instead.")]] __simd_callee__ inline void
asc_exp_sub_v2(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_FUSED_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_FUSED_H
#endif
