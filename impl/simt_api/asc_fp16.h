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
 * \file asc_fp16.h
 * \brief
 */
#ifndef ASCENDC_MODULE_SIMT_C_ASC_FP16_H
#define ASCENDC_MODULE_SIMT_C_ASC_FP16_H

__simt_callee__ inline half __float2half(const float x);

__simt_callee__ inline half __float2half_rn(const float x);

__simt_callee__ inline half __float2half_rz(const float x);

__simt_callee__ inline half __float2half_rd(const float x);

__simt_callee__ inline half __float2half_ru(const float x);

__simt_callee__ inline float __half2float(const half x);

__simt_callee__ inline unsigned int __half2uint_rn(const half x);

__simt_callee__ inline unsigned int __half2uint_rz(const half x);

__simt_callee__ inline unsigned int __half2uint_rd(const half x);

__simt_callee__ inline unsigned int __half2uint_ru(const half x);

__simt_callee__ inline int __half2int_rn(const half x);

__simt_callee__ inline int __half2int_rz(const half x);

__simt_callee__ inline int __half2int_rd(const half x);

__simt_callee__ inline int __half2int_ru(const half x);

__simt_callee__ inline unsigned long long int __half2ull_rn(const half x);

__simt_callee__ inline unsigned long long int __half2ull_rz(const half x);

__simt_callee__ inline unsigned long long int __half2ull_rd(const half x);

__simt_callee__ inline unsigned long long int __half2ull_ru(const half x);

__simt_callee__ inline long long int __half2ll_rn(const half x);

__simt_callee__ inline long long int __half2ll_rz(const half x);

__simt_callee__ inline long long int __half2ll_rd(const half x);

__simt_callee__ inline long long int __half2ll_ru(const half x);

__simt_callee__ inline half __uint2half_rn(const unsigned int x);

__simt_callee__ inline half __uint2half_rz(const unsigned int x);

__simt_callee__ inline half __uint2half_rd(const unsigned int x);

__simt_callee__ inline half __uint2half_ru(const unsigned int x);

__simt_callee__ inline half __int2half_rn(const int x);

__simt_callee__ inline half __int2half_rz(const int x);

__simt_callee__ inline half __int2half_rd(const int x);

__simt_callee__ inline half __int2half_ru(const int x);

__simt_callee__ inline half __ll2half_rn(const long long int x);

__simt_callee__ inline half __ll2half_rz(const long long int x);

__simt_callee__ inline half __ll2half_rd(const long long int x);

__simt_callee__ inline half __ll2half_ru(const long long int x);

__simt_callee__ inline half __ull2half_rn(const unsigned long long int x);

__simt_callee__ inline half __ull2half_rz(const unsigned long long int x);

__simt_callee__ inline half __ull2half_rd(const unsigned long long int x);

__simt_callee__ inline half __ull2half_ru(const unsigned long long int x);

#include "impl/simt_api/asc_fp16_impl.h"
#endif  //  ASCENDC_MODULE_SIMT_C_ASC_FP16_H
