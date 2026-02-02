/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_bf16_impl.h
 * \brief
 */
#ifndef IMPL_SIMT_API_ASC_FP8_IMPL_H
#define IMPL_SIMT_API_ASC_FP8_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP8_IMPL__
#warning "asc_fp8_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "asc_fp8.h" and use public functions or variables defined in interface header files."
#endif

#if (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102)

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rna(const float2 x) {
    return __cvt_bfloat16_t<ROUND::A, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rna_sat(const float2 x) {
    return __cvt_bfloat16_t<ROUND::A, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rh(const float2 x) {
    return __cvt_bfloat16_t<ROUND::H, RoundingSaturation::RS_DISABLE_VALUE>(x);
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline hifloat8x2_t __float22hif82_rh_sat(const float2 x) {
    return __cvt_bfloat16_t<ROUND::H, RoundingSaturation::RS_ENABLE_VALUE>(x);
}

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP8_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP8_IMPL__
#endif

#endif  // IMPL_SIMT_API_ASC_FP8_IMPL_H