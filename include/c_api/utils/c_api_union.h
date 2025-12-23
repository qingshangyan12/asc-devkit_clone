/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_C_API_UTILS_C_API_UNION_H
#define INCLUDE_C_API_UTILS_C_API_UNION_H

#include <cstdint>

constexpr uint64_t CAPI_DEFAULT_BINARY_CONFIG_VALUE = 0x0100080808010101;
constexpr uint64_t CAPI_DEFAULT_UNARY_CONFIG_VALUE = 0x0100800800010001;
constexpr uint64_t CAPI_DEFAULT_FILL_VALUE_CONFIG_VALUE = 0x0000000000000000;

union asc_binary_config {
    uint64_t config = CAPI_DEFAULT_BINARY_CONFIG_VALUE;
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
    uint64_t config = CAPI_DEFAULT_UNARY_CONFIG_VALUE;
    struct {
        uint64_t dst_block_stride : 16;
        uint64_t src_block_stride : 16;
        uint64_t dst_repeat_stride : 12;
        uint64_t src_repeat_stride : 12;
        uint64_t repeat : 8;
    };
};

union asc_fill_value_config {
    uint64_t config = CAPI_DEFAULT_FILL_VALUE_CONFIG_VALUE;
    struct {
        uint64_t repeat : 15;
        uint64_t reserved1 : 1;
        uint64_t block_num : 15;
        uint64_t reserved2 : 1;
        uint64_t dst_gap : 15;
        uint64_t reserved3 : 17;
    };
};

constexpr asc_binary_config CAPI_DEFAULT_BINARY_CFG{};
constexpr asc_unary_config CAPI_DEFAULT_UNARY_CFG{};
constexpr asc_fill_value_config CAPI_DEFAULT_FILL_VALUE_CFG{};
#endif