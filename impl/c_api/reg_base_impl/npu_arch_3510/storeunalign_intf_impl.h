/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREUNALIGN_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREUNALIGN_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/storeunalign_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREUNALIGN_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"
#include "impl/c_api/reg_base_impl/storeunalign_intf_impl.h"

[[deprecated("NOTICE: asc_storeunalign_postupdate with a b8 destination and vector_bool source is deprecated. "
             "Please use the b16 or b32 overload instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

// ==========asc_storeunalign_postupdate(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e8m0/e1m2/e2m1)=========
[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

//===========asc_storeunalign_post(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float/int64)===========
[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREUNALIGN_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREUNALIGN_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREUNALIGN_INTF_IMPL_H
