/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_SYS_VAR_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_SYS_VAR_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/sys_var_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SYS_VAR_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/common_impl.h"

__aicore__ inline int64_t asc_get_core_id_impl() { return get_coreid(); }

__aicore__ inline uint64_t asc_get_phy_buf_addr_impl(uint64_t offset) { return get_imm(offset); }

__aicore__ inline uint64_t asc_get_overflow_status_impl() { return get_overflow_status(); }

__aicore__ inline int64_t asc_get_sub_block_id_impl() { return get_subblockid(); }

__aicore__ inline int64_t asc_get_sub_block_num_impl() { return get_subblockdim(); }

__aicore__ inline int64_t asc_get_system_cycle_impl() { return get_sys_cnt(); }

__aicore__ inline int64_t asc_get_system_clock_impl() { return get_sys_cnt() / SYSTEM_CLOCK_FREQ_MHZ; }

__aicore__ inline int64_t asc_get_block_idx_impl() { return get_block_idx(); }

__aicore__ inline int64_t asc_get_block_num_impl() { return get_block_num(); }

__aicore__ inline void asc_get_arch_ver_impl(uint32_t& core_version) { core_version = 2201U; }

__aicore__ inline int64_t asc_get_program_counter_impl() { return get_pc(); }

__aicore__ inline int64_t asc_get_ffts_base_addr_impl() { return get_ffts_base_addr(); }

__aicore__ inline void asc_set_ffts_base_addr_impl(uint64_t config) { set_ffts_base_addr(config); }

__aicore__ inline int64_t asc_get_smmu_tag_version_impl() { return get_smmu_tag_ver(); }

__aicore__ inline int64_t asc_get_phy_stack_base_impl() { return get_stack_phy_base(); }

__aicore__ inline int64_t asc_get_status_impl() { return get_status(); }

__aicore__ inline int64_t asc_get_sys_virtual_base_impl() { return get_sys_va_base(); }

__aicore__ inline int64_t asc_get_core_id() { return asc_get_core_id_impl(); }

[[deprecated("NOTICE: asc_get_block_idx is deprecated. "
             "Please use block_idx for pure Vector, pure Cube, and Mix(1, 1). "
             "For Mix(1, 2), please use block_idx on the Cube core and "
             "block_idx * asc_get_sub_block_num() + asc_get_sub_block_id() "
             "on Vector cores instead.")]]
__aicore__ inline int64_t asc_get_block_idx()
{
    return asc_get_block_idx_impl();
}

[[deprecated("NOTICE: asc_get_block_num is deprecated. "
             "Please use block_num instead.")]]
__aicore__ inline int64_t asc_get_block_num()
{
    return asc_get_block_num_impl();
}

__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset) { return asc_get_phy_buf_addr_impl(offset); }

__aicore__ inline uint64_t asc_get_overflow_status() { return asc_get_overflow_status_impl(); }

__aicore__ inline int64_t asc_get_sub_block_id() { return asc_get_sub_block_id_impl(); }

__aicore__ inline int64_t asc_get_sub_block_num() { return asc_get_sub_block_num_impl(); }

__aicore__ inline int64_t asc_get_system_cycle() { return asc_get_system_cycle_impl(); }

__aicore__ inline int64_t asc_get_system_clock() { return asc_get_system_clock_impl(); }

__aicore__ inline void asc_get_arch_ver(uint32_t& core_version) { asc_get_arch_ver_impl(core_version); }

__aicore__ inline int64_t asc_get_program_counter() { return asc_get_program_counter_impl(); }

__aicore__ inline int64_t asc_get_ffts_base_addr() { return asc_get_ffts_base_addr_impl(); }

__aicore__ inline void asc_set_ffts_base_addr(uint64_t config) { asc_set_ffts_base_addr_impl(config); }

__aicore__ inline int64_t asc_get_smmu_tag_version() { return asc_get_smmu_tag_version_impl(); }

__aicore__ inline int64_t asc_get_phy_stack_base() { return asc_get_phy_stack_base_impl(); }

__aicore__ inline int64_t asc_get_status() { return asc_get_status_impl(); }

__aicore__ inline int64_t asc_get_sys_virtual_base() { return asc_get_sys_virtual_base_impl(); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SYS_VAR_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SYS_VAR_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_SYS_VAR_INTF_IMPL_H
