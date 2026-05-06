/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_SIMT_API_ASC_FP4_H
#define INCLUDE_SIMT_API_ASC_FP4_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP4_H__
#endif

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)

#include "simt_api/asc_fp8.h"
#include "simt_api/device_types.h"

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16x2_t __asc_cvt_e1m2float2_to_bfloat16x2_t(const e1m2float2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline e1m2float2 __asc_cvt_bfloat16x2_t_to_e1m2float2(const bfloat16x2_t x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16x2_t __asc_cvt_e2m1float2_to_bfloat16x2_t(const e2m1float2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline e2m1float2 __asc_cvt_bfloat16x2_t_to_e2m1float2(const bfloat16x2_t x);

#include "impl/simt_api/asc_fp4_impl.h"

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP4_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP4_H__
#endif

#endif  // INCLUDE_SIMT_API_ASC_FP4_H
