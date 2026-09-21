/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_ENUM_H
#endif

/*!
 * \file reg_enum.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_UTILS_REG_ENUM_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_UTILS_REG_ENUM_H

#include <cstdint>

namespace asc {
namespace te {
namespace experimental {

/** @brief Data layout selected by a width-changing register cast. */
enum class cast_layout {
    unknown = -1,
    zero,
    one,
    two,
    three,
};

/** @brief Rounding behavior for register cast. */
enum class cast_round_mode {
    unknown = -1,
    rint,
    round,
    floor,
    ceil,
    trunc,
};

/** @brief Saturation behavior for register cast. */
enum class cast_sat_mode {
    unknown = -1,
    not_sat,
    sat,
};

/**
 * @brief Mask write-back behavior for inactive lanes.
 * @details
 * - zeroing: inactive lanes in the result are set to zero.
 * - merging: inactive lanes keep the destination register's original values.
 */
enum class merge_mode {
    zeroing,
    merging,
};

struct cast_options {
    cast_layout layout = cast_layout::zero;
    cast_round_mode round = cast_round_mode::rint;
    cast_sat_mode sat = cast_sat_mode::not_sat;
};

/**
 * @brief Algorithm / Subnormal policy for register div (aligned with basic_api DivAlgo).
 */
enum class div_algo {
    intrinsic = 0,
    diff_compensation,
    precision_1ulp_ftz_true,
    precision_0ulp_ftz_true,
    precision_0ulp_ftz_false,
    precision_1ulp_ftz_false,
};

/**
 * @brief Div precision options (aligned with basic_api DivSpecificMode / DivConfig).
 * @details When @c precision_mode is true, high-precision paths are preferred
 *          (diff-compensation / 0ulp), subject to @c algo and element type.
 */
struct div_options {
    bool precision_mode = false;
    div_algo algo = div_algo::intrinsic;
};

/**
 * @brief Algorithm / Subnormal policy for register sqrt (aligned with basic_api SqrtAlgo).
 */
enum class sqrt_algo {
    intrinsic = 0,
    fast_inverse,
    precision_1ulp_ftz_true,
    precision_0ulp_ftz_false,
    precision_1ulp_ftz_false,
};

/**
 * @brief Sqrt precision options (aligned with basic_api SqrtSpecificMode / SqrtConfig).
 * @details When @c precision_mode is true, fast-inverse (0ulp, float only) is used.
 */
struct sqrt_options {
    bool precision_mode = false;
    sqrt_algo algo = sqrt_algo::intrinsic;
};

/**
 * @brief Algorithm / Subnormal policy for register rsqrt (aligned with basic_api RsqrtAlgo).
 */
enum class rsqrt_algo {
    intrinsic = 0,
    fast_inverse,
    precision_1ulp_ftz_true,
    precision_0ulp_ftz_false,
    precision_1ulp_ftz_false,
};

/**
 * @brief Rsqrt precision options (aligned with basic_api RsqrtConfig).
 */
struct rsqrt_options {
    rsqrt_algo algo = rsqrt_algo::intrinsic;
};

constexpr div_options default_div_ops = {};
constexpr sqrt_options default_sqrt_ops = {};
constexpr rsqrt_options default_rsqrt_ops = {};

enum class load_sideband_mode : uint8_t {
    direct,
    deintlv,
    downsample,
    unpack,
    unpack4,
    upsample,
};

enum class store_sideband_mode : uint8_t {
    direct,
    store_1st,
    intlv,
    pack,
    pack_quarter,
};

enum class broadcast_mode : uint8_t {
    elem,
    datablock,
    elem2datablock,
};

enum class mask_pattern : uint8_t {
    all,
    vl1,
    vl2,
    vl3,
    vl4,
    vl8,
    vl16,
    vl32,
    vl64,
    vl128,
    every3,
    every4,
    half,
    quarter,
    none,
};

enum class reduce_scope : uint8_t { element, datablock, pair };

enum class histogram_half {
    low,
    high,
};
 
enum class histogram_mode {
    frequency,
    cumulative,
};
} // namespace experimental
} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_UTILS_REG_ENUM_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_ENUM_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_ENUM_H
#endif
