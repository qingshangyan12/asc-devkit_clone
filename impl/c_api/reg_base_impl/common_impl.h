/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_COMMON_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_COMMON_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/common_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_COMMON_IMPL_H
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

constexpr int64_t SYSTEM_CLOCK_FREQ_MHZ = 1000;

union ctrl_scalar_cache_bits {
    uint64_t value;
    struct {
        uint64_t reserved0 : 16;
        uint64_t scalar_load_cache_mode : 4;
        uint64_t scalar_store_cache_mode : 4;
        uint64_t reserved1 : 40;
    };
};

union asc_store_align_config {
    int32_t config;
    struct {
        uint32_t repeat_stride : 16;
        uint32_t block_stride : 16;
    };
};

union asc_store_align_config_post {
    int32_t config;
    struct {
        uint32_t repeat_stride : 16;
        uint32_t block_stride : 16;
    };
};

constexpr uint8_t ASC_L13D_RPT_REPEAT_TIMES_SHIFT = 16;

constexpr uint8_t ASC_L13D_RPT_DIRECTION_SHIFT = 24;

constexpr uint8_t ASC_L13D_RPT_DST_STRIDE_K_SHIFT = 32;

constexpr uint8_t ASC_L13D_RPT_DST_START_POS_M_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_H_SHIFT = 16;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_LEFT_SHIFT = 32;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_RIGHT_SHIFT = 40;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_TOP_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_BOTTOM_SHIFT = 56;

constexpr uint8_t ASC_L13D_FMATRIX_B_H_SHIFT = 16;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_LEFT_SHIFT = 32;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_RIGHT_SHIFT = 40;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_TOP_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_BOTTOM_SHIFT = 56;

constexpr uint8_t ASC_GM2L1_SID_DEFAULT = 0;

constexpr uint8_t ASC_GM2L1_DECOMP_MODE_DEFAULT = 0;

constexpr uint8_t ASC_3D_PADDING_SHIFT_BIT = 8;

constexpr uint8_t ASC_L0C_COPY_CHANNEL_PARA_SHIFT = 48;

constexpr uint8_t ASC_L0C_COPY_NZ_PARA_SRC_SHIFT = 16;

constexpr uint8_t ASC_L0C_COPY_NZ_PARA_DST_SHIFT = 32;

constexpr uint8_t ASC_3D_PADDING_B_SHIFT_BIT = 8;

constexpr uint8_t ASC_L3D_RPT_B_REPEAT_TIMES_SHIFT = 16;

constexpr uint8_t ASC_L3D_RPT_B_DIRECTION_SHIFT = 24;

constexpr uint8_t ASC_L3D_RPT_B_DST_STRIDE_K_SHIFT = 32;

constexpr uint8_t ASC_L3D_RPT_B_DST_START_POS_M_SHIFT = 48;

constexpr uint8_t RELU_POST_DEFAULT = 0;

constexpr bool CLIP_RELU_POST_DEFAULT = false;

constexpr uint8_t ELTWISE_OP_DEFAULT = 0;

constexpr uint64_t QUANT_POST_DEFAULT = 0;

constexpr int16_t ASC_C_API_DEFAULT_OFFSET = 0;

constexpr int16_t ASC_C_API_LOAD_DEV_DEFAULT_OFFSET = 0;

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_COMMON_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_COMMON_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_COMMON_IMPL_H
