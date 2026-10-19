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
#error "reg_copy.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_COPY_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_REG_COPY_H
#define INCLUDE_C_API_REG_COMPUTE_REG_COPY_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_copy_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the int8_t elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the uint8_t elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the int16_t elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies one selected 16B data block of a uint16_t vector register into a 32B mask register, broadcasting each bit to 2 bits; void return.
 *
 * @override_id asc_copy__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the half elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the int32_t elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies one selected 8B data block of a uint32_t vector register into a 32B mask register, broadcasting each bit to 4 bits; void return.
 *
 * @override_id asc_copy__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the float elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the bfloat16_t elements of src selected by mask into the matching positions of dst, keeping the original dst values where mask bits are 0; void return.
 *
 * @override_id asc_copy__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies the mask-selected bits of the source mask register into dst and fills positions filtered out by mask with 0; void return.
 *
 * @override_id asc_copy__Rvb_vb_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。
 */
__simd_callee__ inline void asc_copy(vector_bool& dst, vector_bool src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies all bits of the source mask register to the destination mask register without applying a mask; void return.
 *
 * @override_id asc_copy__Rvb_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器，dtype须与src完全一致。mask比特位为1的位置写入src对应元素（或对应比特），mask比特位为0的位置采用Merging模式写入dst原值。dst需初始化，否则当mask为0时将出现未知值。
 * @param [in] src 源矢量数据寄存器，dtype须与dst完全一致。
 */
__simd_callee__ inline void asc_copy(vector_bool& dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies one selected 16B data block of a uint16_t vector register into a 32B mask register, broadcasting each bit to 2 bits; void return.
 *
 * @override_id asc_copy__Rvb_vu16_i16
 * @archs 3510
 * @param [out] dst 目的掩码寄存器，写入src的全部比特位。
 * @param [in] src 源矢量数据寄存器。
 * @param [in] part part决定了src中需要搬运的数据块，具体描述请参考[关键特性说明](#关键特性说明)。 &bull; 当数据类型为uint16_t，part∈[0, 15]。 &bull; 当数据类型为uint32_t，part∈[0, 31]。
 */
__simd_callee__ inline void asc_copy(vector_bool& dst, vector_uint16_t src, int16_t part);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Copies one selected 8B data block of a uint32_t vector register into a 32B mask register, broadcasting each bit to 4 bits; void return.
 *
 * @override_id asc_copy__Rvb_vu32_i16
 * @archs 3510
 * @param [out] dst 目的掩码寄存器，写入src的全部比特位。
 * @param [in] src 源矢量数据寄存器。
 * @param [in] part part决定了src中需要搬运的数据块，具体描述请参考[关键特性说明](#关键特性说明)。 &bull; 当数据类型为uint16_t，part∈[0, 15]。 &bull; 当数据类型为uint32_t，part∈[0, 31]。
 */
__simd_callee__ inline void asc_copy(vector_bool& dst, vector_uint32_t src, int16_t part);

// ==========asc_mask return-value overloads(u8/s8/half/u16/s16/float/u32/s32/bf16/bool)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_int8_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vi8_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_int8_t asc_mask(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_uint8_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vu8_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_uint8_t asc_mask(vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_int16_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vi16_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_int16_t asc_mask(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_uint16_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vu16_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_uint16_t asc_mask(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_half register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vf16_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_half asc_mask(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_int32_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vi32_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_int32_t asc_mask(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_uint32_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vu32_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_uint32_t asc_mask(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_float register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vf32_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_float asc_mask(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_bfloat16_t register with the elements of src selected by mask copied to the corresponding result positions; positions not selected by mask are undefined.
 *
 * @override_id asc_mask__vbf16_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_bfloat16_t asc_mask(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_bool mask register with the bits of src selected by mask copied over; bits not selected by mask are set to 0.
 *
 * @override_id asc_mask__vb_vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。未被mask筛选的位置值未定义。
 */
__simd_callee__ inline vector_bool asc_mask(vector_bool src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_bool mask register holding all bits of the source mask register src; this overload applies no mask filtering.
 *
 * @override_id asc_mask__vb
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline vector_bool asc_mask(vector_bool src);

// ==========asc_extract_mask return-value overloads(u16/u32)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_bool mask register extracted from the selected uint16_t data block of src, broadcasting each bit to 2 bits (32B); part must be a compile-time constant.
 *
 * @override_id asc_extract_mask__vu16_i16
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] part part决定了src中需要搬运的数据块，具体描述请参考[asc_copy关键特性说明](asc_copy.md#关键特性说明)。 &bull; 当数据类型为uint16_t，part∈[0, 15]。 &bull; 当数据类型为uint32_t，part∈[0, 31]。
 */
__simd_callee__ inline vector_bool asc_extract_mask(vector_uint16_t src, int16_t part);

/**
 * @ingroup simd_c_api.reg_compute.reg_copy
 * @brief Returns a vector_bool mask register extracted from the selected uint32_t data block of src, broadcasting each bit to 4 bits (32B); part must be a compile-time constant.
 *
 * @override_id asc_extract_mask__vu32_i16
 * @archs 3510
 * @param [in] src 源矢量数据寄存器。
 * @param [in] part part决定了src中需要搬运的数据块，具体描述请参考[asc_copy关键特性说明](asc_copy.md#关键特性说明)。 &bull; 当数据类型为uint16_t，part∈[0, 15]。 &bull; 当数据类型为uint32_t，part∈[0, 31]。
 */
__simd_callee__ inline vector_bool asc_extract_mask(vector_uint32_t src, int16_t part);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_COPY_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_COPY_H
#endif
