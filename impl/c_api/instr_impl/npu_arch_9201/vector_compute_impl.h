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
#pragma message( \
    "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please include c_api/asc_simd.h and use public functions or variables defined in interface header files.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_COMPUTE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_COMPUTE_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_9201/utils_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_float2e5m2_rn_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_e4m32float_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_int4x22int16_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_half2hif8_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_half2int4x2_impl/asc_half2int4x2_rd_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_half2int4x2_impl/asc_half2int4x2_rn_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_half2int4x2_impl/asc_half2int4x2_rna_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_half2int4x2_impl/asc_half2int4x2_ru_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_half2int4x2_impl/asc_half2int4x2_rz_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_hif82float_impl/asc_hif82float_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_hif82half_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_int4x22half_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e2m1x2_impl/asc_bfloat162e2m1x2_rd_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e2m1x2_impl/asc_bfloat162e2m1x2_rn_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e2m1x2_impl/asc_bfloat162e2m1x2_rna_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e2m1x2_impl/asc_bfloat162e2m1x2_ru_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e2m1x2_impl/asc_bfloat162e2m1x2_rz_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_float2hif8_impl/asc_float2hif8_rh_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_float2hif8_impl/asc_float2hif8_rna_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_e2m1x22bfloat16_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_e5m22float_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_float2e4m3_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e1m2x2_impl/asc_bfloat162e1m2x2_rd_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e1m2x2_impl/asc_bfloat162e1m2x2_rn_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e1m2x2_impl/asc_bfloat162e1m2x2_rna_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e1m2x2_impl/asc_bfloat162e1m2x2_ru_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_bfloat162e1m2x2_impl/asc_bfloat162e1m2x2_rz_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/vector_compute_impl/vconv/asc_e1m2x22bfloat16_impl.h"

// ==========asc_e4m32float==========
[[deprecated("NOTICE: asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask) is deprecated. "
             "Please use asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e4m32float_v2 is deprecated. "
             "Currently asc_e4m32float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v2(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e4m32float_v3 is deprecated. "
             "Currently asc_e4m32float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v3(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e4m32float_v4 is deprecated. "
             "Currently asc_e4m32float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v4(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_v4_impl(dst, src, mask);
}

// ==========asc_int4x22int16==========
__simd_callee__ inline void asc_int4x22int16(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22int16_v2 is deprecated. "
             "Currently asc_int4x22int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v2(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22int16_v3 is deprecated. "
             "Currently asc_int4x22int16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v3(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22int16_v4 is deprecated. "
             "Currently asc_int4x22int16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v4(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_v4_impl(dst, src, mask);
}

// ==========asc_half2hif8(rh/rna)==========
[[deprecated("NOTICE: asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rh_v2 is deprecated. "
             "Currently asc_half2hif8_rh_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rh_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rh_sat_v2 is deprecated. "
             "Currently asc_half2hif8_rh_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rna_v2 is deprecated. "
             "Currently asc_half2hif8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rna_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2hif8_rna_sat_v2 is deprecated. "
    "Currently asc_half2hif8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_sat_v2_impl(dst, src, mask);
}

// ==========asc_hif82half==========
[[deprecated("NOTICE: asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask) is deprecated. "
             "Please use asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82half_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82half_v2 is deprecated. "
             "Currently asc_hif82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82half_v2(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82half_v2_impl(dst, src, mask);
}

// rd - sat:dis/en - v1 v2 v3 v4
__simd_callee__ inline void asc_half2int4x2_rd(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_half2int4x2_rd_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rd_v2 is deprecated. "
             "Currently asc_half2int4x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rd_v3 is deprecated. "
             "Currently asc_half2int4x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rd_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rd_v4 is deprecated. "
             "Currently asc_half2int4x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rd_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_v4_impl(dst, src, mask);
}

// ru - sat:dis/en - v1 v2 v3 v4
__simd_callee__ inline void asc_half2int4x2_ru(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_half2int4x2_ru_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_ru_v2 is deprecated. "
             "Currently asc_half2int4x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_ru_v3 is deprecated. "
             "Currently asc_half2int4x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_ru_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_ru_v4 is deprecated. "
             "Currently asc_half2int4x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_ru_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_v4_impl(dst, src, mask);
}

// rz - sat:dis/en - v1 v2 v3 v4
__simd_callee__ inline void asc_half2int4x2_rz(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_half2int4x2_rz_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rz_v2 is deprecated. "
             "Currently asc_half2int4x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rz_v3 is deprecated. "
             "Currently asc_half2int4x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rz_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rz_v4 is deprecated. "
             "Currently asc_half2int4x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rz_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_v4_impl(dst, src, mask);
}

// rn - sat:dis/en - v1 v2 v3 v4
__simd_callee__ inline void asc_half2int4x2_rn(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_half2int4x2_rn_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rn_v2 is deprecated. "
             "Currently asc_half2int4x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rn_v3 is deprecated. "
             "Currently asc_half2int4x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rn_v4 is deprecated. "
             "Currently asc_half2int4x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_v4_impl(dst, src, mask);
}

// rna - sat:dis/en - v1 v2 v3 v4
__simd_callee__ inline void asc_half2int4x2_rna(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_half2int4x2_rna_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rna_v2 is deprecated. "
             "Currently asc_half2int4x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rna_v3 is deprecated. "
             "Currently asc_half2int4x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rna_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rna_v4 is deprecated. "
             "Currently asc_half2int4x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rna_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_v4_impl(dst, src, mask);
}
// ==========asc_hif82float==========
[[deprecated("NOTICE: asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask) is deprecated. "
             "Please use asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82float_v2 is deprecated. "
             "Currently asc_hif82float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v2(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82float_v3 is deprecated. "
             "Currently asc_hif82float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v3(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82float_v4 is deprecated. "
             "Currently asc_hif82float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v4(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_v4_impl(dst, src, mask);
}

//================asc_int4x22half==============
__simd_callee__ inline void asc_int4x22half(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22half_v2 is deprecated. "
             "Currently asc_int4x22half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v2(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22half_v3 is deprecated. "
             "Currently asc_int4x22half_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v3(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22half_v4 is deprecated. "
             "Currently asc_int4x22half_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v4(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_v4_impl(dst, src, mask);
}

// ==========asc_bfloat162e2m1x2_rd/rn/rna/ru/rz==========
[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_v4_impl(dst, src, mask);
}

// ==========asc_float2hif8_rh/rna==========
[[deprecated("NOTICE: asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v2 is deprecated. "
    "Currently asc_float2hif8_rh_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh_v2 is deprecated. "
             "Currently asc_float2hif8_rh_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v3 is deprecated. "
    "Currently asc_float2hif8_rh_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh_v3 is deprecated. "
             "Currently asc_float2hif8_rh_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v4 is deprecated. "
    "Currently asc_float2hif8_rh_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh_v4 is deprecated. "
             "Currently asc_float2hif8_rh_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v2 is deprecated. "
    "Currently asc_float2hif8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna_v2 is deprecated. "
             "Currently asc_float2hif8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v3 is deprecated. "
    "Currently asc_float2hif8_rna_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna_v3 is deprecated. "
             "Currently asc_float2hif8_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v4 is deprecated. "
    "Currently asc_float2hif8_rna_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna_v4 is deprecated. "
             "Currently asc_float2hif8_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_v4_impl(dst, src, mask);
}

// ==========asc_e2m1x22bfloat16==========
[[deprecated(
    "NOTICE: asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask) is deprecated. "
    "Please use asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v2 is deprecated. "
             "Currently asc_e2m1x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v3 is deprecated. "
             "Currently asc_e2m1x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v4 is deprecated. "
             "Currently asc_e2m1x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_v4_impl(dst, src, mask);
}

// ==========asc_float2e5m2==========
[[deprecated("NOTICE: asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_v2 is deprecated. "
             "Currently asc_float2e5m2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_v3 is deprecated. "
             "Currently asc_float2e5m2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_v4 is deprecated. "
             "Currently asc_float2e5m2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v2 is deprecated. "
    "Currently asc_float2e5m2_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v3 is deprecated. "
    "Currently asc_float2e5m2_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v4 is deprecated. "
    "Currently asc_float2e5m2_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_v4_impl(dst, src, mask);
}

// ==========asc_e5m22float==========
[[deprecated("NOTICE: asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask) is deprecated. "
             "Please use asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e5m22float_v2 is deprecated. "
             "Currently asc_e5m22float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v2(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e5m22float_v3 is deprecated. "
             "Currently asc_e5m22float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v3(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e5m22float_v4 is deprecated. "
             "Currently asc_e5m22float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v4(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_v4_impl(dst, src, mask);
}

// ==========asc_float2e4m3_rn=========
[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v2 is deprecated. "
    "Currently asc_float2e4m3_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn_v2 is deprecated. "
             "Currently asc_float2e4m3_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v3 is deprecated. "
    "Currently asc_float2e4m3_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn_v3 is deprecated. "
             "Currently asc_float2e4m3_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v4 is deprecated. "
    "Currently asc_float2e4m3_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn_v4 is deprecated. "
             "Currently asc_float2e4m3_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_v4_impl(dst, src, mask);
}

// ==========asc_bfloat162e1m2x2_rd/rn/rna/ru/rz==========
[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_v4_impl(dst, src, mask);
}

// ==========asc_e1m2x22bfloat16==========
[[deprecated(
    "NOTICE: asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask) is deprecated. "
    "Please use asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v2 is deprecated. "
             "Currently asc_e1m2x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v3 is deprecated. "
             "Currently asc_e1m2x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v4 is deprecated. "
             "Currently asc_e1m2x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_v4_impl(dst, src, mask);
}


#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
