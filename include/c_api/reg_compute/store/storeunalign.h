/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "store/storeunalign.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREUNALIGN_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_STORE_STOREUNALIGN_H
#define INCLUDE_C_API_REG_COMPUTE_STORE_STOREUNALIGN_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/storeunalign_intf_impl.h"
#endif

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t*& dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t*& dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t*& dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t*& dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half*& dst, vector_store_unalign& src0, vector_half src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float*& dst, vector_store_unalign& src0, vector_float src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t*& dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_bool src1);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_bool src1);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, addr_reg& count);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ hifloat8_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ hifloat8_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src, addr_reg offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int8_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int4b_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int16_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ half*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int32_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ float*& dst, vector_store_unalign src, int32_t offset);

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int64_t*& dst, vector_store_unalign src, int32_t offset);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_storeunalign_postupdate(__ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_bool src1) or
        asc_storeunalign_postupdate(__ubuf__ uint32_t * &dst, vector_store_unalign& src0, vector_bool src1))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated b8 mask-register variant: stores mask register data to consecutive non-32B-aligned Unified Buffer addresses and auto-updates the dst pointer after each call.
 *
 * @override_id asc_storeunalign_postupdate__RPUu8_Rvstoreunalign_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_bool src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores int8_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUi8_Rvstoreunalign_vi8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores uint8_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUu8_Rvstoreunalign_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores int16_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUi16_Rvstoreunalign_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores uint16_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUu16_Rvstoreunalign_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores int32_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUi32_Rvstoreunalign_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores uint32_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUu32_Rvstoreunalign_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores int64_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUi64_Rvstoreunalign_vi64
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores half data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf16_Rvstoreunalign_vf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores float data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf32_Rvstoreunalign_vf32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores bfloat16_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUbf16_Rvstoreunalign_vbf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores fp8_e4m3fn_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf8e4m3_Rvstoreunalign_vf8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores fp8_e5m2_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf8e5m2_Rvstoreunalign_vf8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores fp8_e8m0_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf8e8m0_Rvstoreunalign_vf8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores fp4x2_e2m1_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf4e2m1x2_Rvstoreunalign_vf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores fp4x2_e1m2_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUf4e1m2x2_Rvstoreunalign_vf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores int4b_t data from a vector register to consecutive non-32B-aligned Unified Buffer addresses and updates the destination address after each call.
 *
 * @override_id asc_storeunalign_postupdate__PUi4x2_Rvstoreunalign_vi4x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in,out] src0 非对齐寄存器，用于保存非对齐数据，长度为32B。
 * @param [in] src1 源操作数（矢量数据寄存器或掩码寄存器）。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the int8_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUi8_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the uint8_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUu8_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the fp4x2_e2m1_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf4e2m1x2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the fp4x2_e1m2_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf4e1m2x2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the int4b_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUi4x2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the fp8_e8m0_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf8e8m0_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the fp8_e5m2_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf8e5m2_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the fp8_e4m3fn_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf8e4m3_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the int16_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUi16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the uint16_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUu16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the half tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the bfloat16_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUbf16_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the int32_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUi32_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the uint32_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUu32_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the float tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUf32_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_squeeze_and_storeunalign_finalize)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: stores the int64_t tail block buffered in the unaligned register to Unified Buffer, completing the unaligned postupdate store sequence.
 *
 * @override_id asc_storeunalign_post__PUi64_vstoreunalign
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREUNALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREUNALIGN_H
#endif
