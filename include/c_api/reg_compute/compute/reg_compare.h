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
#error "compute/reg_compare.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_COMPARE_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_COMPARE_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_COMPARE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_compare_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two int8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two int8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two int16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two int16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two int32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two int32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two uint8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two uint8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two uint16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two uint16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two uint32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two uint32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two half vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two half vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two bfloat16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two bfloat16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two float vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge__Rvb_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than-or-equal comparison (src0 >= src1) of two float vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ge(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src >= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_ge_scalar__Rvb_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src >= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_ge_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ge_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two uint8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two uint8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two int8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two int8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two uint16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two uint16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two int16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two int16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two uint32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two uint32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two int32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two int32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two half vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two half vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two float vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two float vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two bfloat16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le__Rvb_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise less-than-or-equal comparison (src0 <= src1) of two bfloat16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src <= value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_le_scalar__Rvb_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src <= value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_le_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_le_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two int8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two int8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two uint8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two uint8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two int16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two int16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two uint16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two uint16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two half vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two half vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two bfloat16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two bfloat16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two int32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two int32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two uint32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two uint32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two float vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq__Rvb_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise equality comparison (src0 == src1) of two float vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src == value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_eq_scalar__Rvb_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src == value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_eq_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_eq_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_uint8_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_uint8_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_int8_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_int8_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_half vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_half vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_uint16_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_uint16_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_int16_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_int16_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_float vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_float vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_uint32_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_uint32_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_int32_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_int32_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise less-than comparison of two vector_bfloat16_t vectors, writing the mask result bits into dst (1 when src0 is less than src1); the void prototype returns nothing.
 *
 * @override_id asc_lt__Rvb_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise less-than comparison of two vector_bfloat16_t vectors; a bit is 1 when src0 is less than src1, else 0.
 *
 * @override_id asc_lt__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_uint8_t elements with a scalar uint8_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_uint8_t elements with a scalar uint8_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_int8_t elements with a scalar int8_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_int8_t elements with a scalar int8_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_bfloat16_t elements with a scalar bfloat16_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_bfloat16_t elements with a scalar bfloat16_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_uint16_t elements with a scalar uint16_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_uint16_t elements with a scalar uint16_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_int16_t elements with a scalar int16_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_int16_t elements with a scalar int16_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_half elements with a scalar half value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_half elements with a scalar half value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_uint32_t elements with a scalar uint32_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_uint32_t elements with a scalar uint32_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_int32_t elements with a scalar int32_t value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_int32_t elements with a scalar int32_t value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_float elements with a scalar float value elementwise and writes the mask result bits into dst (1 when the element is less than the scalar); void return.
 *
 * @override_id asc_lt_scalar__Rvb_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise comparison of vector_float elements with a scalar float value; a bit is 1 when the element is less than the scalar, else 0.
 *
 * @override_id asc_lt_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_lt_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two uint8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two uint8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two int8_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two int8_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two half vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two half vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two float vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two float vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two uint16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two uint16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two int16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two int16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two uint32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two uint32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two int32_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two int32_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two bfloat16_t vectors under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt__Rvb_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Element-wise greater-than comparison (src0 > src1) of two bfloat16_t vectors under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_gt(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src > value) under mask; writes the 1-bit-per-element result to vector_bool dst; returns void.
 *
 * @override_id asc_gt_scalar__Rvb_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint8_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int8_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each bfloat16_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint16_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int16_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each uint32_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each int32_t vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each half vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares each float vector element with a scalar (src > value) under mask; returns the 1-bit-per-element result as a vector_bool mask.
 *
 * @override_id asc_gt_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_gt_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_uint8_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_uint8_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vu8_vu8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_int8_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_int8_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vi8_vi8_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_int8_t src0, vector_int8_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_uint16_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_uint16_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vu16_vu16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_int16_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_int16_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vi16_vi16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_int16_t src0, vector_int16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_uint32_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_uint32_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vu32_vu32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_int32_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_int32_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vi32_vi32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_half vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_half vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vf16_vf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_half src0, vector_half src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_bfloat16_t vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_bfloat16_t vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vbf16_vbf16_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Elementwise not-equal comparison of two vector_float vectors, writing the mask result bits into dst (1 when src0 differs from src1); the void prototype returns nothing.
 *
 * @override_id asc_ne__Rvb_vf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器），仅用于无返回值原型。
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of two vector_float vectors; a bit is 1 when src0 differs from src1, else 0.
 *
 * @override_id asc_ne__vf32_vf32_vb
 * @archs 3510
 * @param [in] src0 源操作数（矢量数据寄存器）。
 * @param [in] src1 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bool asc_ne(vector_float src0, vector_float src1, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_uint8_t elements with a scalar uint8_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_int8_t elements with a scalar int8_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_uint16_t elements with a scalar uint16_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_int16_t elements with a scalar int16_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_uint32_t elements with a scalar uint32_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_int32_t elements with a scalar int32_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_half elements with a scalar half value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_float elements with a scalar float value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Compares vector_bfloat16_t elements with a scalar bfloat16_t value for inequality and writes the mask result bits into dst (1 when the element differs); void return.
 *
 * @override_id asc_ne_scalar__Rvb_vbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（掩码寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_uint8_t elements with a scalar uint8_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vu8_u8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_uint8_t src, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_int8_t elements with a scalar int8_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vi8_i8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_int8_t src, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_uint16_t elements with a scalar uint16_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vu16_u16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_uint16_t src, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_int16_t elements with a scalar int16_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vi16_i16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_int16_t src, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_uint32_t elements with a scalar uint32_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vu32_u32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_uint32_t src, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_int32_t elements with a scalar int32_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vi32_i32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_int32_t src, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_half elements with a scalar half value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vf16_f16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_half src, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_float elements with a scalar float value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vf32_f32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_float src, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_compare
 * @brief Returns a vector_bool mask from the elementwise not-equal comparison of vector_bfloat16_t elements with a scalar bfloat16_t value; a bit is 1 when the element differs from the scalar.
 *
 * @override_id asc_ne_scalar__vbf16_bf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与比较。`mask`中与元素对应的比特位为1时，该元素参与比较；为0时，该元素不参与比较。
 */
__simd_callee__ inline vector_bool asc_ne_scalar(vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_COMPARE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_COMPARE_H
#endif
