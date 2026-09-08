/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_UTILS_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_UTILS_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/memory_base_impl/utils_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_UTILS_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include <cstdint>
#include <type_traits>
#include "impl/utils/sys_macros_impl.h"
#include "utils/base/sys_constants.h"

constexpr uint32_t C_API_AIC_TYPE = AscendC::AIC;

constexpr uint32_t C_API_AIV_TYPE = AscendC::AIV;

constexpr uint32_t C_API_MIX_TYPE = AscendC::MIX;

#define ASC_IS_AIV ASCEND_IS_AIV

#define ASC_IS_AIC ASCEND_IS_AIC

#define ASC_IS_NOT_AIV ASCEND_IS_NOT_AIV

#define ASC_IS_NOT_AIC ASCEND_IS_NOT_AIC

struct asc_c_api_default_repeat {
    uint8_t U8 = 1;
};

struct asc_c_api_default_block_stride {
    uint8_t U8 = 1;
    uint16_t U16 = 1;
};

struct asc_c_api_default_repeat_stride {
    uint8_t U8 = 8;
    uint16_t U16 = 8;
};

struct asc_c_api_half_default_repeat_stride {
    uint8_t U8 = 4;
    uint16_t U16 = 4;
};

struct asc_c_api_one_fourth_default_repeat_stride {
    uint8_t U8 = 2;
    uint16_t U16 = 2;
};

constexpr asc_c_api_default_repeat ASC_C_API_DEFAULT_REPEAT;

constexpr asc_c_api_default_block_stride ASC_C_API_DEFAULT_BLOCK_STRIDE;

constexpr asc_c_api_default_repeat_stride ASC_C_API_DEFAULT_REPEAT_STRIDE;

constexpr asc_c_api_half_default_repeat_stride ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE;

constexpr asc_c_api_one_fourth_default_repeat_stride ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE;

constexpr uint16_t ASC_C_API_DATABLOCK_NUM = 8;

constexpr uint16_t ASC_C_API_ONE_DATABLOCK_SIZE = 32;

constexpr uint16_t ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE = 1;

using order_t = Order_t;

union asc_sparse_addr_config {
    uint64_t config;
    struct {
        uint64_t src : 32;
        uint64_t index : 32;
    };
};

__aicore__ inline void asc_set_mask_count_begin(uint32_t count)
{
    if ASC_IS_AIV {
        set_mask_count();
        set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    }
}

__aicore__ inline void asc_set_mask_count_end()
{
    if ASC_IS_AIV {
        set_mask_norm();
    }
}

__aicore__ inline void asc_sync_post_process() { pipe_barrier(pipe_t::PIPE_ALL); }

union asc_capi_fpc_reg_config {
    uint64_t config;
    struct {
        uint64_t relu_units : 8;
        uint64_t quant_units : 8;
        uint64_t reserved : 47;
        uint64_t unit_flag : 1;
    };
};

union asc_scalar_bitcode {
    __aicore__ asc_scalar_bitcode() {}
    half input_half;
    int16_t input_int16_t;
    uint16_t input_uint16_t;
    uint64_t output;
};

__aicore__ inline int64_t asc_get_core_id_impl() { return get_coreid(); }

__aicore__ inline uint64_t asc_get_phy_buf_addr_impl(uint64_t offset) { return get_imm(offset); }

__aicore__ inline uint64_t asc_get_overflow_status_impl() { return get_overflow_status(); }

__aicore__ inline int64_t asc_get_sub_block_id_impl() { return get_subblockid(); }

__aicore__ inline int64_t asc_get_sub_block_num_impl() { return get_subblockdim(); }

constexpr int64_t SYSTEM_CLOCK_FREQ_MHZ = 50;

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

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_UTILS_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_UTILS_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_UTILS_INTF_IMPL_H
