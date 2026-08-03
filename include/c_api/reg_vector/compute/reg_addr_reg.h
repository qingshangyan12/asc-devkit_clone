/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reg_addr_reg.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_ADDR_REG_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_ADDR_REG_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_update_addr_reg(b8/b16/b32)=========
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset);

__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset);

__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset);

__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset0, uint32_t offset1);

__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset0, uint32_t offset1);

__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset0, uint32_t offset1);

__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2);

__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2);

__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2);

__simd_callee__ inline addr_reg asc_update_addr_reg_b32(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

__simd_callee__ inline addr_reg asc_update_addr_reg_b16(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

__simd_callee__ inline addr_reg asc_update_addr_reg_b8(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

// ==========asc_create_iter_reg (deprecated, please use asc_update_addr_reg instead)=========
[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(uint32_t offset);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(uint32_t offset);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(uint32_t offset);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 instead.")]]
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

#endif
