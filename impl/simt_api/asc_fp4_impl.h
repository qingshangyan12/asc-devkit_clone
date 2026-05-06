/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file asc_fp4_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP4_IMPL__
#warning "impl/simt_api/asc_fp4_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "simt_api/asc_fp4.h" and use public functions or variables defined in interface header files."
#endif

#ifndef IMPL_SIMT_API_ASC_FP4_IMPL_H
#define IMPL_SIMT_API_ASC_FP4_IMPL_H

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#include "__clang_cce_simt_fp4.h"
#endif

#include "simt_api/device_types.h"
#include "impl/simt_api/internal_functions_impl.h"

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16x2_t __asc_cvt_e1m2float2_to_bfloat16x2_t(const e1m2float2 x) {
    return _cvt_e1m2float2_to_bfloat16x2_t<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline e1m2float2 __asc_cvt_bfloat16x2_t_to_e1m2float2(const bfloat16x2_t x) {
    return _cvt_bfloat16x2_t_to_e1m2float2<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16x2_t __asc_cvt_e2m1float2_to_bfloat16x2_t(const e2m1float2 x) {
    return _cvt_e2m1float2_to_bfloat16x2_t<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline e2m1float2 __asc_cvt_bfloat16x2_t_to_e2m1float2(const bfloat16x2_t x) {
    return _cvt_bfloat16x2_t_to_e2m1float2<__internal_get_round<__RoundMode::CAST_RINT>(), RoundingSaturation::RS_DISABLE_VALUE>(x);
}

#endif
#endif  // IMPL_SIMT_API_ASC_FP4_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP4_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP4_IMPL__
#endif
