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
#include "impl/c_api/memory_base_impl/spr_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/spr_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.spr
 * @brief Returns the value of specified bits of the CTRL control register as int64_t; see asc_set_ctrl for the bit field meanings.
 *
 * @override_id asc_get_ctrl__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_ctrl();

/**
 * @ingroup simd_c_api.spr
 * @brief Writes the given 64-bit config to specific bits of the CTRL control register (float saturation, mask mode, scalar UB DCache access, etc.); returns nothing.
 *
 * @override_id asc_set_ctrl__u64
 * @archs 2201 3510
 * @param [in] config 待设置的寄存器值。常用CTRL寄存器比特位说明参考表2。
 */
__aicore__ inline void asc_set_ctrl(uint64_t config);

/**
 * @ingroup simd_c_api.spr
 * @brief Returns from the AR special register the valid data length (int64_t) packed by squeeze, used with asc_squeeze_and_storeunalign to learn how much data was moved out.
 *
 * @override_id asc_get_squeeze_status__z
 * @archs 3510
 */
__aicore__ inline int64_t asc_get_squeeze_status();

/**
 * @ingroup simd_c_api.spr
 * @brief Enables or disables saturation for the selected saturation mode without changing other modes.
 *
 * @override_id asc_set_saturation_flag__saturation_b
 * @archs 3510
 * @param [in] saturation_mode 饱和模式类型，取值见[asc_saturation_mode](../defs/enum/asc_saturation_mode.md)。
 * @param [in] enable_sat 是否开启饱和。`true`表示开启饱和，`false`表示关闭饱和（对INT为截断；对FLOAT/FLOAT8/CAST为非饱和或截断，见上表）。
 */
__aicore__ inline void asc_set_saturation_flag(asc_saturation_mode saturation_mode, bool enable_sat);
/**
 * @ingroup simd_c_api.spr
 * @brief Returns a bool indicating whether saturation is enabled for the given saturation mode; CAST also requires the USE_GLOBAL saturation strategy.
 *
 * @override_id asc_get_saturation_flag__saturation
 * @archs 3510
 * @param [in] saturation_mode 饱和模式类型，取值见[asc_saturation_mode](../defs/enum/asc_saturation_mode.md)。
 */
__aicore__ inline bool asc_get_saturation_flag(asc_saturation_mode saturation_mode);
/**
 * @ingroup simd_c_api.spr
 * @brief Selects per-API or global saturation control without changing the saturation flags.
 *
 * @override_id asc_set_saturation_strategy__strategy
 * @archs 3510
 * @param [in] strategy 饱和控制策略，取值见[asc_override_strategy](../defs/enum/asc_override_strategy.md)。
 */
__aicore__ inline void asc_set_saturation_strategy(asc_override_strategy strategy);
/**
 * @ingroup simd_c_api.spr
 * @brief Returns the current saturation control strategy as asc_override_strategy, either USE_API or USE_GLOBAL; commonly used to save or verify the configuration.
 *
 * @override_id asc_get_saturation_strategy__z
 * @archs 3510
 */
__aicore__ inline asc_override_strategy asc_get_saturation_strategy();

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_get_squeeze_status)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads the value of the specified special register and returns it as an int64_t; use asc_get_squeeze_status to achieve this functionality instead.
 *
 * @override_id asc_get_ar_spr__z
 * @archs 3510
 */
__aicore__ inline int64_t asc_get_ar_spr();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SPR_SPR_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SPR_SPR_H
#endif
