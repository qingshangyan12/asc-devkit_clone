/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_LOGIC_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_LOGIC_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/reg_logic_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_LOGIC_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"
#include "impl/c_api/reg_base_impl/reg_logic_intf_impl.h"

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_or floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_or(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_or floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_or(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_LOGIC_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_LOGIC_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_LOGIC_INTF_IMPL_H
