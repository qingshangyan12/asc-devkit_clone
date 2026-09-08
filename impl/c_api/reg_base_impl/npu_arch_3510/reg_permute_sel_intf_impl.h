/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_PERMUTE_SEL_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_PERMUTE_SEL_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/reg_permute_sel_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_PERMUTE_SEL_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"
#include "impl/c_api/reg_base_impl/reg_permute_sel_intf_impl.h"

__simd_callee__ inline void asc_squeeze_v2_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

// ==========asc_squeeze_with_status(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/bfloat16_t/hifloat8_t/fp8_e4m3fn_t/fp8_e5m2_t/fp8_e8m0_t)==========
[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

// ==========asc_pack(bool/uint16_t/int16_t/uint32_t/int32_t)==========
[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint8_t& dst, vector_uint16_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint8_t& dst, vector_int16_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint16_t& dst, vector_uint32_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint16_t& dst, vector_int32_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_bool& dst, vector_bool src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint8_t& dst, vector_uint16_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint8_t& dst, vector_int16_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint16_t& dst, vector_uint32_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint16_t& dst, vector_int32_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_bool& dst, vector_bool src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_clear_ar_spr is deprecated. Please use asc_squeeze_and_storeunalign_init instead.")]]
__simd_callee__ inline void asc_clear_ar_spr()
{
    asc_clear_ar_spr_impl();
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_PERMUTE_SEL_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_PERMUTE_SEL_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_PERMUTE_SEL_INTF_IMPL_H
