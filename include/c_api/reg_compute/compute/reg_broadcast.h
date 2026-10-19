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
#error "compute/reg_broadcast.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_BROADCAST_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_BROADCAST_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_BROADCAST_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint8_t value into dst at positions selected by mask and zeroes unselected positions of the vector_uint8_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvu8_u8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_uint8_t& dst, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int8_t value into dst at positions selected by mask and zeroes unselected positions of the vector_int8_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvi8_i8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_int8_t& dst, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e4m3fn_t value into dst at positions selected by mask and zeroes unselected positions of the vector_fp8_e4m3fn_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvf8e4m3_f8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_fp8_e4m3fn_t& dst, fp8_e4m3fn_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e5m2_t value into dst at positions selected by mask and zeroes unselected positions of the vector_fp8_e5m2_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvf8e5m2_f8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_fp8_e5m2_t& dst, fp8_e5m2_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e8m0_t value into dst at positions selected by mask and zeroes unselected positions of the vector_fp8_e8m0_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvf8e8m0_f8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_fp8_e8m0_t& dst, fp8_e8m0_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint16_t value into dst at positions selected by mask and zeroes unselected positions of the vector_uint16_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvu16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_uint16_t& dst, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int16_t value into dst at positions selected by mask and zeroes unselected positions of the vector_int16_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvi16_i16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_int16_t& dst, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar half value into dst at positions selected by mask and zeroes unselected positions of the vector_half register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvf16_f16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_half& dst, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar bfloat16_t value into dst at positions selected by mask and zeroes unselected positions of the vector_bfloat16_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvbf16_bf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_bfloat16_t& dst, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint32_t value into dst at positions selected by mask and zeroes unselected positions of the vector_uint32_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvu32_u32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_uint32_t& dst, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int32_t value into dst at positions selected by mask and zeroes unselected positions of the vector_int32_t register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_int32_t& dst, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar float value into dst at positions selected by mask and zeroes unselected positions of the vector_float register (void return).
 *
 * @override_id asc_duplicate_scalar__Rvf32_f32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_float& dst, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint8_t register with the scalar uint8_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__u8_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_uint8_t asc_duplicate_scalar(uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int8_t register with the scalar int8_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__i8_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_int8_t asc_duplicate_scalar(int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e4m3fn_t register with the scalar fp8_e4m3fn_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__f8e4m3_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_duplicate_scalar(fp8_e4m3fn_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e5m2_t register with the scalar fp8_e5m2_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__f8e5m2_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_duplicate_scalar(fp8_e5m2_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e8m0_t register with the scalar fp8_e8m0_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__f8e8m0_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_duplicate_scalar(fp8_e8m0_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint16_t register with the scalar uint16_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__u16_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_uint16_t asc_duplicate_scalar(uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int16_t register with the scalar int16_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__i16_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_int16_t asc_duplicate_scalar(int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_half register with the scalar half value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__f16_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_half asc_duplicate_scalar(half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_bfloat16_t register with the scalar bfloat16_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__bf16_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_bfloat16_t asc_duplicate_scalar(bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint32_t register with the scalar uint32_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__u32_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_uint32_t asc_duplicate_scalar(uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int32_t register with the scalar int32_t value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__i32_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_int32_t asc_duplicate_scalar(int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_float register with the scalar float value broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate_scalar__f32_vb
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。
 */
__simd_callee__ inline vector_float asc_duplicate_scalar(float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint8_t value into the mask-selected positions of vector_uint8_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvu8_u8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_uint8_t& dst, uint8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int8_t value into the mask-selected positions of vector_int8_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvi8_i8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_int8_t& dst, int8_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e4m3fn_t value into the mask-selected positions of vector_fp8_e4m3fn_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvf8e4m3_f8e4m3_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_fp8_e4m3fn_t& dst, fp8_e4m3fn_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e5m2_t value into the mask-selected positions of vector_fp8_e5m2_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvf8e5m2_f8e5m2_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_fp8_e5m2_t& dst, fp8_e5m2_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e8m0_t value into the mask-selected positions of vector_fp8_e8m0_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvf8e8m0_f8e8m0_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_fp8_e8m0_t& dst, fp8_e8m0_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint16_t value into the mask-selected positions of vector_uint16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvu16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_uint16_t& dst, uint16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int16_t value into the mask-selected positions of vector_int16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvi16_i16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_int16_t& dst, int16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar half value into the mask-selected positions of vector_half dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvf16_f16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_half& dst, half value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar bfloat16_t value into the mask-selected positions of vector_bfloat16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvbf16_bf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_bfloat16_t& dst, bfloat16_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint32_t value into the mask-selected positions of vector_uint32_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvu32_u32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_uint32_t& dst, uint32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int32_t value into the mask-selected positions of vector_int32_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvi32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_int32_t& dst, int32_t value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar float value into the mask-selected positions of vector_float dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_scalar_merge__Rvf32_f32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入`value`，未被筛选的元素保留调用接口前的值。元素数据类型需要与`value`保持一致。
 * @param [in] value 源操作数（标量），作为待广播的数据。数据类型需要与`dst`的元素数据类型保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`value`；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_float& dst, float value, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint8_t value to every element of the vector_uint8_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvu8_u8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_uint8_t& dst, uint8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int8_t value to every element of the vector_int8_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvi8_i8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_int8_t& dst, int8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint16_t value to every element of the vector_uint16_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvu16_u16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_uint16_t& dst, uint16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int16_t value to every element of the vector_int16_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvi16_i16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_int16_t& dst, int16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar uint32_t value to every element of the vector_uint32_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvu32_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_uint32_t& dst, uint32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar int32_t value to every element of the vector_int32_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_int32_t& dst, int32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar half value to every element of the vector_half register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvf16_f16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_half& dst, half value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar float value to every element of the vector_float register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvf32_f32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_float& dst, float value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar bfloat16_t value to every element of the vector_bfloat16_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvbf16_bf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_bfloat16_t& dst, bfloat16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e4m3fn_t value to every element of the vector_fp8_e4m3fn_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvf8e4m3_f8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_fp8_e4m3fn_t& dst, fp8_e4m3fn_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e5m2_t value to every element of the vector_fp8_e5m2_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvf8e5m2_f8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_fp8_e5m2_t& dst, fp8_e5m2_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts scalar fp8_e8m0_t value to every element of the vector_fp8_e8m0_t register dst; the result is written into dst.
 *
 * @override_id asc_duplicate_scalar__Rvf8e8m0_f8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline void asc_duplicate_scalar(vector_fp8_e8m0_t& dst, fp8_e8m0_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint8_t register with the scalar uint8_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__u8
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_uint8_t asc_duplicate_scalar(uint8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int8_t register with the scalar int8_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__i8
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_int8_t asc_duplicate_scalar(int8_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint16_t register with the scalar uint16_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__u16
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_uint16_t asc_duplicate_scalar(uint16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int16_t register with the scalar int16_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__i16
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_int16_t asc_duplicate_scalar(int16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint32_t register with the scalar uint32_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__u32
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_uint32_t asc_duplicate_scalar(uint32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int32_t register with the scalar int32_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__i32
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_int32_t asc_duplicate_scalar(int32_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_half register with the scalar half value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__f16
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_half asc_duplicate_scalar(half value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_float register with the scalar float value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__f32
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_float asc_duplicate_scalar(float value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_bfloat16_t register with the scalar bfloat16_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__bf16
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_bfloat16_t asc_duplicate_scalar(bfloat16_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e4m3fn_t register with the scalar fp8_e4m3fn_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__f8e4m3
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_duplicate_scalar(fp8_e4m3fn_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e5m2_t register with the scalar fp8_e5m2_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__f8e5m2
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_duplicate_scalar(fp8_e5m2_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e8m0_t register with the scalar fp8_e8m0_t value broadcast to every element.
 *
 * @override_id asc_duplicate_scalar__f8e8m0
 * @archs 3510
 * @param [in] value 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_duplicate_scalar(fp8_e8m0_t value);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_uint8_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_int8_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_fp8_e4m3fn_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_fp8_e5m2_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_fp8_e8m0_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_uint16_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_int16_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_half register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_bfloat16_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_uint32_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_int32_t register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest element (index 0) of the vector_float register src into dst at mask-selected positions; unselected dst positions are zero (void return).
 *
 * @override_id asc_duplicate__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。仅无返回值类型接口包含该参数。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline void asc_duplicate(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint8_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vu8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_uint8_t asc_duplicate(vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int8_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vi8_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_int8_t asc_duplicate(vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e4m3fn_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vf8e4m3_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_duplicate(vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e5m2_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vf8e5m2_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_duplicate(vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_fp8_e8m0_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vf8e8m0_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_duplicate(vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint16_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vu16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_uint16_t asc_duplicate(vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int16_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vi16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_int16_t asc_duplicate(vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_half register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_half asc_duplicate(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_bfloat16_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vbf16_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_duplicate(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_uint32_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vu32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_uint32_t asc_duplicate(vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_int32_t register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vi32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_int32_t asc_duplicate(vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Returns a vector_float register with the lowest element (index 0) of src broadcast to mask-selected positions; unselected positions are zero.
 *
 * @override_id asc_duplicate__vf32_vb
 * @archs 3510
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。
 * @param [in] mask 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。
 */
__simd_callee__ inline vector_float asc_duplicate(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_uint8_t src into the mask-selected positions of vector_uint8_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_int8_t src into the mask-selected positions of vector_int8_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_fp8_e4m3fn_t src into the mask-selected positions of vector_fp8_e4m3fn_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_fp8_e5m2_t src into the mask-selected positions of vector_fp8_e5m2_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_fp8_e8m0_t src into the mask-selected positions of vector_fp8_e8m0_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_uint16_t src into the mask-selected positions of vector_uint16_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_int16_t src into the mask-selected positions of vector_int16_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_half src into the mask-selected positions of vector_half dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_bfloat16_t src into the mask-selected positions of vector_bfloat16_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_uint32_t src into the mask-selected positions of vector_uint32_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_int32_t src into the mask-selected positions of vector_int32_t dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_float src into the mask-selected positions of vector_float dst and zeroes the unmasked elements.
 *
 * @override_id asc_duplicate_highest__Rvf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器），保存广播结果。被`mask`筛选的元素写入广播值，未被筛选的元素置0。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素置0。
 */
__simd_callee__ inline void asc_duplicate_highest(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_uint8_t src into the mask-selected positions of vector_uint8_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvu8_vu8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_int8_t src into the mask-selected positions of vector_int8_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvi8_vi8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_fp8_e4m3fn_t src into the mask-selected positions of vector_fp8_e4m3fn_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_fp8_e5m2_t src into the mask-selected positions of vector_fp8_e5m2_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_fp8_e8m0_t src into the mask-selected positions of vector_fp8_e8m0_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_uint16_t src into the mask-selected positions of vector_uint16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvu16_vu16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_int16_t src into the mask-selected positions of vector_int16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvi16_vi16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_half src into the mask-selected positions of vector_half dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvf16_vf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_bfloat16_t src into the mask-selected positions of vector_bfloat16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_uint32_t src into the mask-selected positions of vector_uint32_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvu32_vu32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_int32_t src into the mask-selected positions of vector_int32_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvi32_vi32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the lowest-index element of vector_float src into the mask-selected positions of vector_float dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_merge__Rvf32_vf32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最低位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_merge(vector_float& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_uint8_t src into the mask-selected positions of vector_uint8_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvu8_vu8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_int8_t src into the mask-selected positions of vector_int8_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvi8_vi8_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_fp8_e4m3fn_t src into the mask-selected positions of vector_fp8_e4m3fn_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_fp8_e5m2_t src into the mask-selected positions of vector_fp8_e5m2_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_fp8_e8m0_t src into the mask-selected positions of vector_fp8_e8m0_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(
    vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_uint16_t src into the mask-selected positions of vector_uint16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvu16_vu16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_int16_t src into the mask-selected positions of vector_int16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvi16_vi16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_half src into the mask-selected positions of vector_half dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvf16_vf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_bfloat16_t src into the mask-selected positions of vector_bfloat16_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvbf16_vbf16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_uint32_t src into the mask-selected positions of vector_uint32_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvu32_vu32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_int32_t src into the mask-selected positions of vector_int32_t dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvi32_vi32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_broadcast
 * @brief Broadcasts the highest-index element of vector_float src into the mask-selected positions of vector_float dst, preserving unmasked dst elements (merge mode).
 *
 * @override_id asc_duplicate_highest_merge__Rvf32_vf32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量数据寄存器）。被`mask`筛选的元素写入广播值，未被筛选的元素保留调用接口前的值。数据类型需要与`src`保持一致。
 * @param [in] src 源操作数（矢量数据寄存器），其最高位元素作为待广播的数据。数据类型需要与`dst`保持一致。
 * @param [in] mask 目的操作数元素操作的有效指示（掩码寄存器）。对应比特位为1时，`dst`中的元素写入`src`的最高位元素；为0时，`dst`中的元素保留原值。
 */
__simd_callee__ inline void asc_duplicate_highest_merge(vector_float& dst, vector_float src, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_BROADCAST_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_BROADCAST_H
#endif
