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
#error "reg_vector.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_REG_VECTOR_H
#define INCLUDE_C_API_REG_VECTOR_REG_VECTOR_H

#include "c_api/reg_vector/compute/reg_addr_reg.h"
#include "c_api/reg_vector/compute/reg_arith.h"
#include "c_api/reg_vector/compute/reg_broadcast.h"
#include "c_api/reg_vector/compute/reg_compare.h"
#include "c_api/reg_vector/compute/reg_fused.h"
#include "c_api/reg_vector/compute/reg_gather.h"
#include "c_api/reg_vector/compute/reg_histogram.h"
#include "c_api/reg_vector/compute/reg_index.h"
#include "c_api/reg_vector/compute/reg_logic.h"
#include "c_api/reg_vector/compute/reg_mask.h"
#include "c_api/reg_vector/compute/reg_permute_sel.h"
#include "c_api/reg_vector/compute/reg_reduce.h"
#include "c_api/reg_vector/compute/reg_scatter.h"

//===================asc_set_va_reg=====================
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
