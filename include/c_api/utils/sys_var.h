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

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/sys_var_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl.h"
#endif

#include "c_api/defs/defs.h"
#include "c_api/spr/spr.h"

/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_smmu_tag_version__z
 * @archs 3510 2201
 * @brief 获取SMMU（System Memory Management Unit）版本信息。
 */
__aicore__ inline int64_t asc_get_smmu_tag_version();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_phy_stack_base__z
 * @archs 3510 2201
 * @brief 获取物理堆栈基地址。
 */
__aicore__ inline int64_t asc_get_phy_stack_base();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_status__z
 * @archs 3510 2201
 * @brief 获取状态信息。
 */
__aicore__ inline int64_t asc_get_status();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_sys_virtual_base__z
 * @archs 3510 2201
 * @brief 获取系统虚拟基地址。
 */
__aicore__ inline int64_t asc_get_sys_virtual_base();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_sub_block_num__z
 * @archs 3510 2201
 * @brief 分离模式下，获取一个AI Core上Cube Core（AIC）或者Vector Core（AIV）的数量。
 */
__aicore__ inline int64_t asc_get_sub_block_num();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_sub_block_id__z
 * @archs 3510 2201
 * @brief 获取AI Core上Vector核的ID。
 */
__aicore__ inline int64_t asc_get_sub_block_id();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_system_cycle__z
 * @archs 3510 2201
 * @brief 读取当前系统cycle计数器，返回int64_t类型的当前计数值。该计数器为只读系统计数器，宽度64bit，复位值为0，随系统时钟持续递增，反映硬件运行以来的累计cycle数，常用于性能统计、耗时测量与执行序的时序判断。本接口在AIC与AIV上均可调用，返回值含义一致。
 */
__aicore__ inline int64_t asc_get_system_cycle();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_ffts_base_addr__z
 * @archs 2201
 * @brief 获取核间同步寄存器的基地址。需要在Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步寄存器的基地址，并作为参数传入[asc_set_ffts_base_addr](asc_set_ffts_base_addr.md)后使用。
 */
__aicore__ inline int64_t asc_get_ffts_base_addr();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_core_id__z
 * @archs 3510 2201
 * @brief 获取当前核的编号。
 */
__aicore__ inline int64_t asc_get_core_id();
__aicore__ inline uint64_t asc_get_overflow_status();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_phy_buf_addr__u64
 * @archs 3510 2201
 * @brief 基于偏移量offset获取片上实际物理地址。
 * @param [in] offset 片上内存的地址偏移量，范围是0~物理内存大小。
 */
__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset);
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_program_counter__z
 * @archs 3510 2201
 * @brief 获取程序计数器的指针，程序计数器用于记录当前程序执行的位置。
 */
__aicore__ inline int64_t asc_get_program_counter();
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_arch_ver__Ru32
 * @archs 3510 2201
 * @brief 获取当前AI处理器架构版本号。
 * @param [out] core_version AI处理器架构版本，数据类型uint32_t。
 */
__aicore__ inline void asc_get_arch_ver(uint32_t& core_version);
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_set_ffts_base_addr__u64
 * @archs 2201
 * @brief 在[asc_sync_block_wait](../../sync/asc_sync_block_wait.md)和[asc_sync_block_arrive](../../sync/asc_sync_block_arrive.md)之前使用，设置核间同步寄存器的基地址。需要在Host侧调用接口aclrtGetHardwareSyncAddr获取，并作为参数传入。
 * @param [in] config 核间同步的基地址。取值范围[0, 2^48-1]。
 */
__aicore__ inline void asc_set_ffts_base_addr(uint64_t config);
/**
 * @ingroup simd_c_api.utils.sys_var
 * @override_id asc_get_vf_len__z
 * @archs 3510
 * @brief 获取Tensor位宽VL（Vector Length）的大小。
 */
__aicore__ inline constexpr int64_t asc_get_vf_len();

[[deprecated("NOTICE: asc_get_block_num is deprecated. "
             "Please use block_num instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_get_block_num__z
 * @archs 3510 2201
 * @brief **该接口已废弃，请使用[内置变量block_num](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#内置变量)替代。
 */
__aicore__ inline int64_t asc_get_block_num();

[[deprecated("NOTICE: asc_get_block_idx is deprecated. "
             "Please use block_idx for pure Vector, pure Cube, and Mix(1, 1). "
             "For Mix(1, 2), please use block_idx on the Cube core and "
             "block_idx * asc_get_sub_block_num() + asc_get_sub_block_id() "
             "on Vector cores instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_get_block_idx__z
 * @archs 3510 2201
 * @brief **该接口已废弃，请使用[内置变量block_idx](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#内置变量)结合[asc_get_sub_block_num](../utils/sys_var/asc_get_sub_block_num.md)和[asc_get_sub_block_id](../utils/sys_var/asc_get_sub_block_id.md)接口替代。
 */
__aicore__ inline int64_t asc_get_block_idx();
#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UTILS_SYS_VAR_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UTILS_SYS_VAR_H
#endif
