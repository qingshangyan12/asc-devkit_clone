/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_REG_HISTOGRAM_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_REG_HISTOGRAM_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/reg_histogram_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_HISTOGRAM_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"

__simd_callee__ inline void asc_cumulative_histogram_bin0_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        chistv2(dst, src, mask, Bin_N0);
    }
}

__simd_callee__ inline void asc_cumulative_histogram_bin1_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        chistv2(dst, src, mask, Bin_N1);
    }
}

__simd_callee__ inline void asc_frequency_histogram_bin0_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        dhistv2(dst, src, mask, Bin_N0);
    }
}

__simd_callee__ inline void asc_frequency_histogram_bin1_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        dhistv2(dst, src, mask, Bin_N1);
    }
}

// ==========asc_cumulative_histogram/asc_frequency_histogram==========
__simd_callee__ inline void asc_cumulative_histogram_bin0(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_cumulative_histogram_bin0_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cumulative_histogram_bin1(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_cumulative_histogram_bin1_impl(dst, src, mask);
}

__simd_callee__ inline void asc_frequency_histogram_bin0(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_frequency_histogram_bin0_impl(dst, src, mask);
}

__simd_callee__ inline void asc_frequency_histogram_bin1(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_frequency_histogram_bin1_impl(dst, src, mask);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_HISTOGRAM_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_HISTOGRAM_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_REG_HISTOGRAM_INTF_IMPL_H
