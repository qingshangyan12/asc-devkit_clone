/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_UB_SCATTER_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_UB_SCATTER_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/ub_scatter_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_UB_SCATTER_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/ub_scatter_intf_impl.h"

[[deprecated("NOTICE: asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int8_t* dst, vector_int8_t src, vector_uint16_t index, vector_bool mask) "
             "instead.")]] __simd_callee__ inline void
asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ uint8_t* dst, vector_uint8_t src, vector_uint16_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int16_t* dst, vector_int16_t src, vector_uint16_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_uint16_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int32_t* dst, vector_int32_t src, vector_uint32_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_uint32_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_uint16_t index, "
             "vector_bool mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated(
    "NOTICE: asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask) is deprecated. "
    "Please use asc_scatter(__ubuf__ half* dst, vector_half src, vector_uint16_t index, vector_bool mask) "
    "instead.")]] __simd_callee__ inline void
asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask) is "
             "deprecated. "
             "Please use asc_scatter(__ubuf__ float* dst, vector_float src, vector_uint32_t index, vector_bool mask) "
             "instead.")]] __simd_callee__ inline void
asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_UB_SCATTER_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_UB_SCATTER_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_UB_SCATTER_INTF_IMPL_H
