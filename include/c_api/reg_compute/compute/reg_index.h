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
#error "compute/reg_index.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_INDEX_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_INDEX_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_INDEX_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_index_intf_impl.h"
#endif
/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_int8_t register holding an ascending index sequence starting at value (return-value form).
 *
 * @override_id asc_arange__i8
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_int8_t asc_arange(int8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_int16_t register holding an ascending index sequence starting at value (return-value form).
 *
 * @override_id asc_arange__i16
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_int16_t asc_arange(int16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_int32_t register holding an ascending index sequence starting at value (return-value form).
 *
 * @override_id asc_arange__i32
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_int32_t asc_arange(int32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_half register holding an ascending index sequence starting at value (return-value form).
 *
 * @override_id asc_arange__f16
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_half asc_arange(half value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_float register holding an ascending index sequence starting at value (return-value form).
 *
 * @override_id asc_arange__f32
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_float asc_arange(float value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_int8_t register containing a descending index sequence whose first element is value + VL_T - 1 (return-value form).
 *
 * @override_id asc_arange_descend__i8
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_int8_t asc_arange_descend(int8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_int16_t register containing a descending index sequence whose first element is value + VL_T - 1 (return-value form).
 *
 * @override_id asc_arange_descend__i16
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_int16_t asc_arange_descend(int16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_int32_t register containing a descending index sequence whose first element is value + VL_T - 1 (return-value form).
 *
 * @override_id asc_arange_descend__i32
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_int32_t asc_arange_descend(int32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_half register containing a descending index sequence whose first element is value + VL_T - 1 (return-value form).
 *
 * @override_id asc_arange_descend__f16
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_half asc_arange_descend(half value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Returns a vector_float register containing a descending index sequence whose first element is value + VL_T - 1 (return-value form).
 *
 * @override_id asc_arange_descend__f32
 * @archs 3510
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline vector_float asc_arange_descend(float value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates an ascending index sequence starting at value into the vector_int8_t register dst (output-parameter form, void return).
 *
 * @override_id asc_arange__Rvi8_i8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange(vector_int8_t& dst, int8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates an ascending index sequence starting at value into the vector_int16_t register dst (output-parameter form, void return).
 *
 * @override_id asc_arange__Rvi16_i16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange(vector_int16_t& dst, int16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates an ascending index sequence starting at value into the vector_int32_t register dst (output-parameter form, void return).
 *
 * @override_id asc_arange__Rvi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange(vector_int32_t& dst, int32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates an ascending index sequence starting at value into the vector_half register dst (output-parameter form, void return).
 *
 * @override_id asc_arange__Rvf16_f16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange(vector_half& dst, half value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates an ascending index sequence starting at value into the vector_float register dst (output-parameter form, void return).
 *
 * @override_id asc_arange__Rvf32_f32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange(vector_float& dst, float value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates a descending index sequence whose first element is value + VL_T - 1 into the vector_int8_t register dst (output-parameter form, void return).
 *
 * @override_id asc_arange_descend__Rvi8_i8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange_descend(vector_int8_t& dst, int8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates a descending index sequence whose first element is value + VL_T - 1 into the vector_int16_t register dst (output-parameter form, void return).
 *
 * @override_id asc_arange_descend__Rvi16_i16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange_descend(vector_int16_t& dst, int16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates a descending index sequence whose first element is value + VL_T - 1 into the vector_int32_t register dst (output-parameter form, void return).
 *
 * @override_id asc_arange_descend__Rvi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange_descend(vector_int32_t& dst, int32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates a descending index sequence whose first element is value + VL_T - 1 into the vector_half register dst (output-parameter form, void return).
 *
 * @override_id asc_arange_descend__Rvf16_f16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange_descend(vector_half& dst, half value);

/**
 * @ingroup simd_c_api.reg_compute.reg_index
 * @brief Generates a descending index sequence whose first element is value + VL_T - 1 into the vector_float register dst (output-parameter form, void return).
 *
 * @override_id asc_arange_descend__Rvf32_f32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] value 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增。取值范围为该dtype的可表示范围。
 */
__simd_callee__ inline void asc_arange_descend(vector_float& dst, float value);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_INDEX_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_INDEX_H
#endif
