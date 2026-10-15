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
#error "compute/reg_mask.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_MASK_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_MASK_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_MASK_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_mask_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_mask
 * @brief Returns a mask for 8-bit elements and reduces value by the processed element count.
 *
 * @override_id asc_update_mask_b8__Ru32
 * @archs 3510
 * @param [in,out] value 元素个数。调用后自动减去当前向量处理单元的元素个数（b8模式、b16模式、b32模式分别为256、128、64）。 执行完一次该接口后，value = (value < VL_T) ? 0 : (value - VL_T)，VL_T表示位宽为VL的矢量数据寄存器中，可存放数据类型T的元素个数。 例如，有320个b16数据类型的元素，每次处理128个元素： 第一次调用接口，生成的mask对应的元素全为有效数据，value = 320-128 = 192。 第二次调用接口，生成的mask对应的元素全为有效数据，value = 192-128 = 64。 第三次调用接口，生成的mask对应的元素只有低半部分为有效数据，value = 0。
 */
__simd_callee__ inline vector_bool asc_update_mask_b8(uint32_t& value);

/**
 * @ingroup simd_c_api.reg_compute.reg_mask
 * @brief Returns a mask for 16-bit elements and reduces value by the processed element count.
 *
 * @override_id asc_update_mask_b16__Ru32
 * @archs 3510
 * @param [in,out] value 元素个数。调用后自动减去当前向量处理单元的元素个数（b8模式、b16模式、b32模式分别为256、128、64）。 执行完一次该接口后，value = (value < VL_T) ? 0 : (value - VL_T)，VL_T表示位宽为VL的矢量数据寄存器中，可存放数据类型T的元素个数。 例如，有320个b16数据类型的元素，每次处理128个元素： 第一次调用接口，生成的mask对应的元素全为有效数据，value = 320-128 = 192。 第二次调用接口，生成的mask对应的元素全为有效数据，value = 192-128 = 64。 第三次调用接口，生成的mask对应的元素只有低半部分为有效数据，value = 0。
 */
__simd_callee__ inline vector_bool asc_update_mask_b16(uint32_t& value);

/**
 * @ingroup simd_c_api.reg_compute.reg_mask
 * @brief Returns a mask for 32-bit elements and reduces value by the processed element count.
 *
 * @override_id asc_update_mask_b32__Ru32
 * @archs 3510
 * @param [in,out] value 元素个数。调用后自动减去当前向量处理单元的元素个数（b8模式、b16模式、b32模式分别为256、128、64）。 执行完一次该接口后，value = (value < VL_T) ? 0 : (value - VL_T)，VL_T表示位宽为VL的矢量数据寄存器中，可存放数据类型T的元素个数。 例如，有320个b16数据类型的元素，每次处理128个元素： 第一次调用接口，生成的mask对应的元素全为有效数据，value = 320-128 = 192。 第二次调用接口，生成的mask对应的元素全为有效数据，value = 192-128 = 64。 第三次调用接口，生成的mask对应的元素只有低半部分为有效数据，value = 0。
 */
__simd_callee__ inline vector_bool asc_update_mask_b32(uint32_t& value);

__simd_callee__ inline vector_bool asc_get_mask_spr_b16();

__simd_callee__ inline vector_bool asc_get_mask_spr_b32();

#define asc_create_mask_b8 pset_b8

#define asc_create_mask_b16 pset_b16

#define asc_create_mask_b32 pset_b32

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_MASK_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_MASK_H
#endif
