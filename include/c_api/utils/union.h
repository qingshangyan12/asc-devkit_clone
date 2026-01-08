/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_UTILS_C_API_UNION_H
#define INCLUDE_C_API_UTILS_C_API_UNION_H

#include <cstdint>

constexpr uint64_t ASC_DEFAULT_BINARY_CONFIG_VALUE = 0x0100080808010101;
constexpr uint64_t ASC_DEFAULT_UNARY_CONFIG_VALUE = 0x0100800800010001;
constexpr uint64_t ASC_DEFAULT_REDUCE_CONFIG_VALUE = 0x0100000800010001;
constexpr uint64_t ASC_FILL_VALUE_DEFAULT_VALUE = 0x0000000000000000;
constexpr uint64_t ASC_STORE_ATOMIC_DEFAULT_VALUE = 0x0000000000000000;
constexpr uint64_t ASC_DEFAULT_COPY_L12L0B_TRANS_CONFIG_VALUE = 0x0000000000000000;
constexpr uint64_t ASC_DEFAULT_SET_L0C_COPY_PARAMS_CONFIG_VALUE = 0x0000000000000000;
constexpr uint64_t ASC_DEFAULT_LOAD3D_V2_CONFIG_VALUE = 0x0000000000010000;

union asc_binary_config {
    uint64_t config = ASC_DEFAULT_BINARY_CONFIG_VALUE;
    struct {
        uint64_t dst_block_stride : 8;
        uint64_t src0_block_stride : 8;
        uint64_t src1_block_stride : 8;
        uint64_t dst_repeat_stride : 8;
        uint64_t src0_repeat_stride : 8;
        uint64_t src1_repeat_stride : 8;
        uint64_t reserved : 8;
        uint64_t repeat : 8;
    };
};

union asc_unary_config {
    uint64_t config = ASC_DEFAULT_UNARY_CONFIG_VALUE;
    struct {
        uint64_t dst_block_stride : 16;
        uint64_t src_block_stride : 16;
        uint64_t dst_repeat_stride : 12;
        uint64_t src_repeat_stride : 12;
        uint64_t repeat : 8;
    };
};

union asc_fill_value_config {
    uint64_t config = ASC_FILL_VALUE_DEFAULT_VALUE;
    struct {
        uint64_t repeat : 15;
        uint64_t reserved1 : 1;
        uint64_t block_num : 15;
        uint64_t reserved2 : 1;
        uint64_t dst_gap : 15;
        uint64_t reserved3 : 17;
    };
};

union asc_store_atomic_config {
    uint64_t config = ASC_STORE_ATOMIC_DEFAULT_VALUE;
    struct {
        uint64_t atomic_type : 3;
        uint64_t atomic_op : 2;
        uint64_t reserved1 : 59;
    };
};

union asc_deq_scale_config {
    uint64_t val = 0;
    struct {
        uint64_t reserved1 : 5;
        uint64_t offset : 9;
        uint64_t sign_mode : 1;
        uint64_t reserved2 : 17;
        uint64_t scale;
    };
};

union asc_copy_l12l0b_trans_config {
    uint64_t config = ASC_DEFAULT_COPY_L12L0B_TRANS_CONFIG_VALUE;
    struct {
        uint64_t index_id : 16;
        uint64_t repeat : 8;
        uint64_t src_stride : 16;
        uint64_t dst_stride : 16;
        uint64_t addrmode : 1;
    };
};

union asc_set_l0c_copy_params_config {
    uint64_t config = ASC_DEFAULT_SET_L0C_COPY_PARAMS_CONFIG_VALUE;
    struct {
        uint64_t nd_num : 16;
        uint64_t src_nd_stride : 16;
        uint64_t dst_nd_stride : 16;
    };
};

union asc_load3d_v2_config {
    uint64_t config = ASC_DEFAULT_LOAD3D_V2_CONFIG_VALUE;
    struct {
        uint64_t rpt_stride : 16;
        uint64_t rpt_time : 8;
        uint64_t rpt_mode : 1;
        uint64_t reserved2 : 39;
    };
};

constexpr asc_binary_config CAPI_DEFAULT_BINARY_CFG{};
constexpr asc_unary_config CAPI_DEFAULT_UNARY_CFG{};
constexpr asc_fill_value_config ASC_FILL_VALUE_DEFAULT_CFG{};
constexpr asc_store_atomic_config ASC_STORE_ATOMIC_DEFAULT_CFG{};
constexpr asc_copy_l12l0b_trans_config ASC_DEFAULT_COPY_L12L0B_TRANS_CFG{};
constexpr asc_set_l0c_copy_params_config ASC_DEFAULT_SET_L0C_COPY_PARAMS_CFG{};
constexpr asc_load3d_v2_config ASC_DEFAULT_LOAD3D_V2_CONFIG{};
#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif    

