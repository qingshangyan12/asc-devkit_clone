/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_SCALAR_CONVERT_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_SCALAR_CONVERT_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/scalar_convert_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_SCALAR_CONVERT_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"

__aicore__ inline half asc_float2half_ro_impl(float value) { return conv_f322f16o(value); }

__aicore__ inline int32_t asc_float2int32_rd_impl(float value) { return conv_f322s32f(value); }

__aicore__ inline int32_t asc_float2int32_ru_impl(float value) { return conv_f322s32c(value); }

__aicore__ inline int32_t asc_float2int32_rn_impl(float value) { return conv_f322s32r(value); }

__aicore__ inline int32_t asc_float2int32_rna_impl(float value) { return conv_f322s32a(value); }

// ==========asc_float2half (ro)==========
__aicore__ inline half asc_float2half_ro(float value) { return asc_float2half_ro_impl(value); }

// ==========asc_float2int32 (rd/ru/rn/rna)==========
__aicore__ inline int32_t asc_float2int32_rd(float value) { return asc_float2int32_rd_impl(value); }

__aicore__ inline int32_t asc_float2int32_ru(float value) { return asc_float2int32_ru_impl(value); }

__aicore__ inline int32_t asc_float2int32_rn(float value) { return asc_float2int32_rn_impl(value); }

__aicore__ inline int32_t asc_float2int32_rna(float value) { return asc_float2int32_rna_impl(value); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_SCALAR_CONVERT_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_SCALAR_CONVERT_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_SCALAR_CONVERT_INTF_IMPL_H
