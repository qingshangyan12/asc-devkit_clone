/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_SIMT_API_ASC_FP8_H
#define INCLUDE_SIMT_API_ASC_FP8_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP8_H__
#endif

#if (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102)

#include "simt_api/asc_bf16.h"
#include "simt_api/device_types.h"

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rna(const float2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rna_sat(const float2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rh(const float2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rh_sat(const float2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __half22hif82_rna(const half2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __half22hif82_rna_sat(const half2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __half22hif82_rh(const half2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __half22hif82_rh_sat(const half2 x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float2 __hif822float2_rna(const hifloat8x2_t x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float2 __hif822float2_rna_sat(const hifloat8x2_t x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float2 __hif822float2_rh(const hifloat8x2_t x);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline float2 __hif822float2_rh_sat(const hifloat8x2_t x);

#include "impl/simt_api/asc_fp8_impl.h"

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP8_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP8_H__
#endif

#endif  // INCLUDE_SIMT_API_ASC_FP8_H