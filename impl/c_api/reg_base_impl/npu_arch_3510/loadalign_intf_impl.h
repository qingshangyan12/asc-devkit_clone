/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_LOADALIGN_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_LOADALIGN_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/loadalign_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_LOADALIGN_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/loadalign_intf_impl.h"

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

// unpack v2
[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

// brc_v2
[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

/// brc_v3(B16/B32)
[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_LOADALIGN_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_LOADALIGN_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_LOADALIGN_INTF_IMPL_H
