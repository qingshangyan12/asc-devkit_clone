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
#error "spr/spr.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SPR_SPR_H
#endif

#ifndef INCLUDE_C_API_SPR_SPR_H
#define INCLUDE_C_API_SPR_SPR_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/sys_var_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl.h"
#endif

/**
 * @ingroup simd_c_api.spr
 * @override_id asc_get_ctrl__z
 * @archs 3510 2201
 * @brief 读取CTRL寄存器（控制寄存器）特定比特位上的值。
 */
__aicore__ inline int64_t asc_get_ctrl();

/**
 * @ingroup simd_c_api.spr
 * @override_id asc_set_ctrl__u64
 * @archs 3510 2201
 * @brief 对CTRL寄存器（控制寄存器）的特定比特位进行设置。
 * @param [in] config 待设置的寄存器值。常用CTRL寄存器比特位说明参考表2。
 */
__aicore__ inline void asc_set_ctrl(uint64_t config);

/**
 * @ingroup simd_c_api.spr
 * @override_id asc_get_squeeze_status__z
 * @archs 3510
 * @brief 读取squeeze操作后保存至AR特殊寄存器的有效数据长度值，用于配合[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)接口获取已连续搬出的有效数据量。
 */
__aicore__ inline int64_t asc_get_squeeze_status();

[[deprecated("NOTICE: asc_get_ar_spr is deprecated. "
             "Please use asc_get_squeeze_status instead.")]]
/**
 * @ingroup simd_c_api.spr
 * @override_id asc_get_ar_spr__z
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_get_squeeze_status](asc_get_squeeze_status.md)实现此功能。
 */
__aicore__ inline int64_t asc_get_ar_spr();

/**
 * @ingroup simd_c_api.spr
 * @override_id asc_set_saturation_flag__saturation_b
 * @archs 3510
 * @brief 按饱和模式类型设置对应饱和开关。`enable_sat`表示API语义上的“是否开启饱和”，接口内部会按模式处理不同语义。
 * @param [in] saturation_mode 饱和模式类型，取值见[asc_saturation_mode](../defs/enum/asc_saturation_mode.md)。
 * @param [in] enable_sat 是否开启饱和。`true`表示开启饱和，`false`表示关闭饱和（对INT为截断；对FLOAT/FLOAT8/CAST为非饱和或截断，见上表）。
 */
__aicore__ inline void asc_set_saturation_flag(asc_saturation_mode saturation_mode, bool enable_sat);
/**
 * @ingroup simd_c_api.spr
 * @override_id asc_get_saturation_flag__saturation
 * @archs 3510
 * @brief 按饱和模式类型读取当前是否开启饱和，并返回布尔类型的结果。
 * @param [in] saturation_mode 饱和模式类型，取值见[asc_saturation_mode](../defs/enum/asc_saturation_mode.md)。
 */
__aicore__ inline bool asc_get_saturation_flag(asc_saturation_mode saturation_mode);
/**
 * @ingroup simd_c_api.spr
 * @override_id asc_set_saturation_strategy__strategy
 * @archs 3510
 * @brief 设置饱和控制策略。
 * @param [in] strategy 饱和控制策略，取值见[asc_override_strategy](../defs/enum/asc_override_strategy.md)。
 */
__aicore__ inline void asc_set_saturation_strategy(asc_override_strategy strategy);
/**
 * @ingroup simd_c_api.spr
 * @override_id asc_get_saturation_strategy__z
 * @archs 3510
 * @brief 读取当前饱和控制策略。
 */
__aicore__ inline asc_override_strategy asc_get_saturation_strategy();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SPR_SPR_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SPR_SPR_H
#endif
