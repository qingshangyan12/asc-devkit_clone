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
 * \file device_functions.h
 * \brief
 */
#ifndef ASCENDC_MODULE_SIMT_C_DEVICE_FUNCTIONS_H
#define ASCENDC_MODULE_SIMT_C_DEVICE_FUNCTIONS_H

__simt_callee__ inline unsigned int __float2uint_rn(const float x);

__simt_callee__ inline unsigned int __float2uint_rz(const float x);

__simt_callee__ inline unsigned int __float2uint_rd(const float x);

__simt_callee__ inline unsigned int __float2uint_ru(const float x);

__simt_callee__ inline int __float2int_rn(const float x);

__simt_callee__ inline int __float2int_rz(const float x);

__simt_callee__ inline int __float2int_rd(const float x);

__simt_callee__ inline int __float2int_ru(const float x);

__simt_callee__ inline unsigned long long int __float2ull_rn(const float x);

__simt_callee__ inline unsigned long long int __float2ull_rz(const float x);

__simt_callee__ inline unsigned long long int __float2ull_rd(const float x);

__simt_callee__ inline unsigned long long int __float2ull_ru(const float x);

__simt_callee__ inline long long int __float2ll_rn(const float x);

__simt_callee__ inline long long int __float2ll_rz(const float x);

__simt_callee__ inline long long int __float2ll_rd(const float x);

__simt_callee__ inline long long int __float2ll_ru(const float x);

__simt_callee__ inline float __uint2float_rn(const unsigned int x);

__simt_callee__ inline float __uint2float_rz(const unsigned int x);

__simt_callee__ inline float __uint2float_rd(const unsigned int x);

__simt_callee__ inline float __uint2float_ru(const unsigned int x);

__simt_callee__ inline float __int2float_rn(const int x);

__simt_callee__ inline float __int2float_rz(const int x);

__simt_callee__ inline float __int2float_rd(const int x);

__simt_callee__ inline float __int2float_ru(const int x);

__simt_callee__ inline float __ull2float_rn(const unsigned long long int x);

__simt_callee__ inline float __ull2float_rz(const unsigned long long int x);

__simt_callee__ inline float __ull2float_rd(const unsigned long long int x);

__simt_callee__ inline float __ull2float_ru(const unsigned long long int x);

__simt_callee__ inline float __ll2float_rn(const long long int x);

__simt_callee__ inline float __ll2float_rz(const long long int x);

__simt_callee__ inline float __ll2float_rd(const long long int x);

__simt_callee__ inline float __ll2float_ru(const long long int x);

__simt_callee__ inline float __int_as_float(const int x);

__simt_callee__ inline float __uint_as_float(const unsigned int x);

__simt_callee__ inline int __float_as_int(const float x);

__simt_callee__ inline unsigned int __float_as_uint(const float x);

#include "impl/simt_api/device_functions_impl.h"
#endif //ASCENDC_MODULE_SIMT_C_DEVICE_FUNCTIONS_H
