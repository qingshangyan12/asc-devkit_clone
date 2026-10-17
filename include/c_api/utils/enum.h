/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef INCLUDE_C_API_UTILS_C_API_ENUM_H
#define INCLUDE_C_API_UTILS_C_API_ENUM_H

#include <cstdint>
#include <type_traits>

enum class asc_load_l2_cache_mode : uint8_t {
    NORMAL_FIRST_VICTIM = 0,
    NORMAL_LAST_VICTIM = 1,
    NORMAL_PERSISTENT = 2,
    NOTALLOC_KEEP = 4,
    NOTALLOC_CLEAN = 5,
    NOTALLOC_DROP = 6
};

enum class asc_store_l2_cache_mode : uint8_t {
    NORMAL_FIRST_VICTIM = 0,
    NORMAL_LAST_VICTIM = 1,
    NORMAL_PERSISTENT = 2,
    NOTALLOC_CLEAN = 4
};

enum class asc_hf32_round_mode : uint8_t { NEAREST_ZERO, NEAREST_EVEN };

enum class asc_unit_flag_mode : uint8_t { DISABLE = 0, ENABLE_KEEP = 2, ENABLE_UPDATE = 3 };

enum class asc_relu_pre_mode : uint8_t {
    NONE = 0,
    NORMAL = 1,
    SCALAR = 2,
    VECTOR = 3,
};

enum class asc_channel_pad_mode : uint8_t {
    // No padding; each 32-byte channel block passes through unchanged.
    NONE = 0,
    // Write path (expand): insert padding values within each 32-byte channel block.
    // Expand: for every 1 source byte, insert 31 padding values.
    EXPAND_1B = 1,
    // Expand: for every 2 source bytes, insert 15 padding values.
    EXPAND_2B = 2,
    // Expand: for every 4 source bytes, insert 14 padding values.
    EXPAND_4B = 3,
    // Expand: for every 8 source bytes, insert 12 padding values.
    EXPAND_8B = 4,
    // Expand: for every 16 source bytes, insert 8 padding values.
    EXPAND_16B = 5,
    // Read path (compact): strip MSB bytes, keep the lowest valid region.
    // Compact: per 32-byte source block, discard 28 MSB bytes, keep the lowest 4 bytes.
    COMPACT_4B = 6,
    // Compact: per 32-byte source block, discard 24 MSB bytes, keep the lowest 8 bytes.
    COMPACT_8B = 7,
    // Compact: per 32-byte source block, discard 16 MSB bytes, keep the lowest 16 bytes.
    COMPACT_16B = 8,
};

enum class asc_position_mode { EVEN = 0, ODD };

constexpr std::integral_constant<asc_position_mode, asc_position_mode::EVEN> ASC_POSITION_EVEN;
constexpr std::integral_constant<asc_position_mode, asc_position_mode::ODD> ASC_POSITION_ODD;

enum class asc_position_quarter_mode {
    DISPERSE_FIRST_QUARTER = 0,
    DISPERSE_SECOND_QUARTER,
    DISPERSE_THIRD_QUARTER,
    DISPERSE_FOURTH_QUARTER
};

constexpr std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
    ASC_DISPERSE_FIRST_QUARTER;
constexpr std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
    ASC_DISPERSE_SECOND_QUARTER;
constexpr std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
    ASC_DISPERSE_THIRD_QUARTER;
constexpr std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
    ASC_DISPERSE_FOURTH_QUARTER;

#endif
