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
#error "sys_var.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UTILS_SYS_VAR_H
#endif

#ifndef INCLUDE_C_API_UTILS_SYS_VAR_H
#define INCLUDE_C_API_UTILS_SYS_VAR_H

#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/sys_var_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/sys_var_intf_impl.h"
#endif

#include "c_api/spr/spr.h"

/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns SMMU version information as int64_t; the low 28 bits encode year, month, day and same-day sub-version for runtime version checks.
 *
 * @override_id asc_get_smmu_tag_version__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_smmu_tag_version();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the physical stack base address (int64_t) of the current core, typically used for debugging to locate the stack in physical memory.
 *
 * @override_id asc_get_phy_stack_base__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_phy_stack_base();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Reads the full 64-bit STATUS special register value as int64_t, reporting warning events; bits stay set until reset or cleared by reading, read-only.
 *
 * @override_id asc_get_status__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_status();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Reads the read-only system virtual base address register as int64_t, used to convert relative offsets into system virtual addresses for address derivation.
 *
 * @override_id asc_get_sys_virtual_base__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_sys_virtual_base();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the number (int64_t) of AICs or AIVs on the current AI Core in split mode; a read-only value configured by the system before kernel launch.
 *
 * @override_id asc_get_sub_block_num__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_sub_block_num();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the logical index (int64_t) of the AIC or AIV on the logical AI Core in split mode; the value lies in [0, SubBlockNum).
 *
 * @override_id asc_get_sub_block_id__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_sub_block_id();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the raw 64-bit system cycle counter value as int64_t, monotonically increasing since hardware start, for performance and timing measurements.
 *
 * @override_id asc_get_system_cycle__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_system_cycle();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the current system time in microseconds as int64_t, computed internally by dividing the system cycle counter by the chip clock factor.
 *
 * @override_id asc_get_system_clock__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_system_clock();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the base address of the inter-core synchronization registers as int64_t; read-only, it must first be configured with asc_set_ffts_base_addr.
 *
 * @override_id asc_get_ffts_base_addr__z
 * @archs 2201
 */
__aicore__ inline int64_t asc_get_ffts_base_addr();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns as int64_t the number identifying the current AI Core relative to the other AI Cores, useful for splitting work across cores.
 *
 * @override_id asc_get_core_id__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_core_id();
__aicore__ inline uint64_t asc_get_overflow_status();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the on-chip physical address (uint64_t) computed from the given offset, for debugging, address derivation or tiling address calculation.
 *
 * @override_id asc_get_phy_buf_addr__u64
 * @archs 2201 3510
 * @param [in] offset 片上内存的地址偏移量，范围是0~物理内存大小。
 */
__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset);
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the program counter pointer (int64_t); only the low 48 bits are a valid GM address and the upper 16 bits must be masked off.
 *
 * @override_id asc_get_program_counter__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_program_counter();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Output-parameter interface: writes the current AI processor architecture version into core_version (uint32_t) by reference; returns nothing.
 *
 * @override_id asc_get_arch_ver__Ru32
 * @archs 2201 3510
 * @param [out] core_version AI处理器架构版本，数据类型uint32_t。接收当前核的核心版本字段值，接口返回时该参数被覆写为ARCH_VER寄存器中的核心版本字段值，调用前无需初始化。
 */
__aicore__ inline void asc_get_arch_ver(uint32_t& core_version);
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Writes the inter-core synchronization register base address obtained from aclrtGetHardwareSyncAddr before using asc_sync_block_wait/arrive; returns nothing.
 *
 * @override_id asc_set_ffts_base_addr__u64
 * @archs 2201
 * @param [in] config 核间同步的基地址。取值范围[0, 2^48-1]。
 */
__aicore__ inline void asc_set_ffts_base_addr(uint64_t config);
/**
 * @ingroup simd_c_api.utils.sys_var
 * @brief Returns the vector register length VL as a constexpr int64_t, the number of elements processed per Repeat, used to derive masks and loop counts.
 *
 * @override_id asc_get_vf_len__z
 * @archs 3510
 */
__aicore__ inline constexpr int64_t asc_get_vf_len();

ASC_DEPRECATED(9.2.0, "2027/09/07", block_num)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: returns the number of cores configured for the current task for multi-core logic control; use the built-in block_num variable, whose value matches.
 *
 * @override_id asc_get_block_num__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_block_num();

/*
Please use block_idx for pure Vector, pure Cube, and Mix(1, 1).
For Mix(1, 2), please use block_idx on the Cube core and
block_idx * asc_get_sub_block_num() + asc_get_sub_block_id()
on Vector cores instead.
*/
ASC_DEPRECATED(9.2.0, "2027/09/07", block_idx and asc_get_sub_block_num and asc_get_sub_block_id)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: returns the index of the current running logical core; use built-in block_idx combined with asc_get_sub_block_num and asc_get_sub_block_id instead.
 *
 * @override_id asc_get_block_idx__z
 * @archs 2201 3510
 */
__aicore__ inline int64_t asc_get_block_idx();
#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UTILS_SYS_VAR_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UTILS_SYS_VAR_H
#endif
