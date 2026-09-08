/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_UTILS_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_UTILS_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/utils_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_UTILS_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include <cstdint>
#include <type_traits>
#include "impl/utils/sys_macros_impl.h"
#include "utils/base/sys_constants.h"

// utils_impl.h
constexpr uint32_t C_API_AIC_TYPE = AscendC::AIC;

constexpr uint32_t C_API_AIV_TYPE = AscendC::AIV;

constexpr uint32_t C_API_MIX_TYPE = AscendC::MIX;

#define ASC_IS_AIV ASCEND_IS_AIV

#define ASC_IS_AIC ASCEND_IS_AIC

constexpr uint16_t ASC_C_API_ONE_DATABLOCK_SIZE = 32;

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

union asc_3d_padding_bitcode {
    __aicore__ asc_3d_padding_bitcode() {}
    half input_half;
    bfloat16_t input_bfloat16;
    int16_t input_int16;
    uint16_t input_uint16;
    int32_t input_int32;
    uint32_t input_uint32;
    float input_float;
    uint64_t output;
};

union asc_gm2l1_loop_size_config {
    uint64_t config;
    struct {
        uint64_t loop1_size : 21;
        uint64_t loop2_size : 42;
    };
};

union asc_gm2l1_loop_stride_config {
    uint64_t config;
    struct {
        uint64_t src_stride : 40;
        uint64_t dst_stride : 24;
    };
};

union asc_gm2ub_loop_size_config {
    uint64_t config;
    struct {
        uint64_t loop1_size : 21;
        uint64_t loop2_size : 42;
    };
};

union asc_gm2ub_loop_stride_config {
    uint64_t config;
    struct {
        uint64_t src_stride : 40;
        uint64_t dst_stride : 24;
    };
};

// asc_get_program_counter_impl.h
__aicore__ inline int64_t asc_get_program_counter_impl() { return get_pc(); }

// asc_get_system_cycle_impl.h
constexpr int64_t SYSTEM_CLOCK_FREQ_MHZ = 1000;

__aicore__ inline int64_t asc_get_system_cycle_impl() { return get_sys_cnt(); }

__aicore__ inline int64_t asc_get_system_clock_impl() { return get_sys_cnt() / SYSTEM_CLOCK_FREQ_MHZ; }

// asc_get_phy_buf_addr_impl.h
__aicore__ inline uint64_t asc_get_phy_buf_addr_impl(uint64_t offset) { return get_imm(offset); }

// asc_get_vf_len_impl.h
__aicore__ inline constexpr int64_t asc_get_vf_len_impl()
{
    constexpr int64_t VECTOR_REG_WIDTH_3510 = 256;
    return VECTOR_REG_WIDTH_3510;
}

// asc_get_arch_ver_impl.h
__aicore__ inline void asc_get_arch_ver_impl(uint32_t& core_version) { core_version = 3510U; }

// asc_get_core_id_impl.h
__aicore__ inline int64_t asc_get_core_id_impl() { return get_coreid(); }

// asc_get_smmu_tag_version_impl.h
__aicore__ inline int64_t asc_get_smmu_tag_version_impl() { return get_smmu_tag_ver(); }

// asc_get_phy_stack_base_impl.h
__aicore__ inline int64_t asc_get_phy_stack_base_impl() { return get_stack_phy_base(); }

// asc_get_status_impl.h
__aicore__ inline int64_t asc_get_status_impl() { return get_status(); }

// asc_get_sub_block_id_impl.h
__aicore__ inline int64_t asc_get_sub_block_id_impl() { return get_subblockid(); }

// asc_get_sub_block_num_impl.h
__aicore__ inline int64_t asc_get_sub_block_num_impl() { return get_subblockdim(); }

// asc_get_sys_virtual_base_impl.h
__aicore__ inline int64_t asc_get_sys_virtual_base_impl() { return get_sys_va_base(); }

// sys_var_impl.h
__aicore__ inline int64_t asc_get_program_counter() { return asc_get_program_counter_impl(); }

__aicore__ inline int64_t asc_get_sub_block_num() { return asc_get_sub_block_num_impl(); }

__aicore__ inline int64_t asc_get_sub_block_id() { return asc_get_sub_block_id_impl(); }

__aicore__ inline int64_t asc_get_system_cycle() { return asc_get_system_cycle_impl(); }

__aicore__ inline int64_t asc_get_system_clock() { return asc_get_system_clock_impl(); }

__aicore__ inline constexpr int64_t asc_get_vf_len() { return asc_get_vf_len_impl(); }

__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset) { return asc_get_phy_buf_addr_impl(offset); }

__aicore__ inline void asc_get_arch_ver(uint32_t& core_version) { asc_get_arch_ver_impl(core_version); }

__aicore__ inline int64_t asc_get_core_id() { return asc_get_core_id_impl(); }

__aicore__ inline int64_t asc_get_smmu_tag_version() { return asc_get_smmu_tag_version_impl(); }

__aicore__ inline int64_t asc_get_phy_stack_base() { return asc_get_phy_stack_base_impl(); }

__aicore__ inline int64_t asc_get_status() { return asc_get_status_impl(); }

__aicore__ inline int64_t asc_get_sys_virtual_base() { return asc_get_sys_virtual_base_impl(); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_UTILS_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_UTILS_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_UTILS_INTF_IMPL_H
