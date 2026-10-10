/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/reg_base_impl/npu_arch_9201/reg_permute_sel_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"c_api/asc_simd.h\"\" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_PERMUTE_SEL_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_PERMUTE_SEL_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_uint16_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, LOWER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_int16_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, LOWER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, LOWER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, LOWER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_low)
__simd_callee__ inline void asc_pack(vector_bool& dst, vector_bool src)
{
    if ASC_IS_AIV {
        ppack(dst, src, LOWER);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_uint16_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, HIGHER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_int16_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, HIGHER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, HIGHER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, HIGHER, MODE_UNKNOWN);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pack_to_high)
__simd_callee__ inline void asc_pack_v2(vector_bool& dst, vector_bool src)
{
    if ASC_IS_AIV {
        ppack(dst, src, HIGHER);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_init)
__simd_callee__ inline void asc_clear_ar_spr()
{
    if ASC_IS_AIV {
        sprclr(SPR_AR);
    }
}

__simd_callee__ inline void asc_pack_to_low(vector_uint32_t& dst, vector_uint64_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, LOWER);
    }
}

__simd_callee__ inline void asc_pack_to_low(vector_uint32_t& dst, vector_int64_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, LOWER);
    }
}

__simd_callee__ inline void asc_pack_to_high(vector_uint32_t& dst, vector_uint64_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, HIGHER);
    }
}

__simd_callee__ inline void asc_pack_to_high(vector_uint32_t& dst, vector_int64_t src)
{
    if ASC_IS_AIV {
        vpack(dst, src, HIGHER);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_uint16_t& dst, vector_uint16_t src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_int16_t& dst, vector_int16_t src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_half& dst, vector_half src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_uint32_t& dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_int32_t& dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_float& dst, vector_float src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_uint64_t& dst, vector_uint64_t src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline void asc_pack_datablock(vector_int64_t& dst, vector_int64_t src)
{
    if ASC_IS_AIV {
        vpackv2(dst, src);
    }
}

__simd_callee__ inline vector_uint32_t asc_pack_to_low(vector_uint64_t src)
{
    vector_uint32_t dst;
    asc_pack_to_low(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_pack_to_low(vector_int64_t src)
{
    vector_uint32_t dst;
    asc_pack_to_low(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_pack_to_high(vector_uint64_t src)
{
    vector_uint32_t dst;
    asc_pack_to_high(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_pack_to_high(vector_int64_t src)
{
    vector_uint32_t dst;
    asc_pack_to_high(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_pack_datablock(vector_uint16_t src)
{
    vector_uint16_t dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_pack_datablock(vector_int16_t src)
{
    vector_int16_t dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_pack_datablock(vector_half src)
{
    vector_half dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_pack_datablock(vector_uint32_t src)
{
    vector_uint32_t dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_pack_datablock(vector_int32_t src)
{
    vector_int32_t dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_pack_datablock(vector_float src)
{
    vector_float dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint64_t asc_pack_datablock(vector_uint64_t src)
{
    vector_uint64_t dst;
    asc_pack_datablock(dst, src);
    return dst;
}

__simd_callee__ inline vector_int64_t asc_pack_datablock(vector_int64_t src)
{
    vector_int64_t dst;
    asc_pack_datablock(dst, src);
    return dst;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
