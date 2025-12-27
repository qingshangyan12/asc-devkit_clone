/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_SIMT_API_ASC_BF16_H
#define INCLUDE_SIMT_API_ASC_BF16_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_BF16_H__
#endif

#include "simt_api/asc_fp16.h"

__simt_callee__ inline bool __hisnan(bfloat16_t x);

__simt_callee__ inline bool __hisinf(bfloat16_t x);

__simt_callee__ inline bfloat16_t __habs(bfloat16_t x);

__simt_callee__ inline bfloat16_t __hfma(bfloat16_t x, bfloat16_t y, bfloat16_t z);

__simt_callee__ inline bfloat16_t __hmax(bfloat16_t x, bfloat16_t y);

__simt_callee__ inline bfloat16_t __hmin(bfloat16_t x, bfloat16_t y);

__simt_callee__ inline bfloat16_t hcos(bfloat16_t x);
__simt_callee__ inline bfloat16_t hsin(bfloat16_t x);
__simt_callee__ inline bfloat16_t htanh(bfloat16_t x);

__simt_callee__ inline bfloat16_t hexp(bfloat16_t x);
__simt_callee__ inline bfloat16_t hexp2(bfloat16_t x);
__simt_callee__ inline bfloat16_t hexp10(bfloat16_t x);

__simt_callee__ inline bfloat16_t hlog(bfloat16_t x);
__simt_callee__ inline bfloat16_t hlog2(bfloat16_t x);
__simt_callee__ inline bfloat16_t hlog10(bfloat16_t x);

__simt_callee__ inline bfloat16_t hsqrt(bfloat16_t x);
__simt_callee__ inline bfloat16_t hrsqrt(bfloat16_t x);

__simt_callee__ inline bfloat16_t hrcp(bfloat16_t x);

__simt_callee__ inline bfloat16_t hfloor(bfloat16_t x);

__simt_callee__ inline bfloat16_t hrint(bfloat16_t x);

__simt_callee__ inline bfloat16_t hceil(bfloat16_t x);

__simt_callee__ inline bfloat16_t htrunc(bfloat16_t x);

__simt_callee__ inline bfloat16_t __float2bfloat16(const float x);

__simt_callee__ inline bfloat16_t __float2bfloat16_rn(const float x);

__simt_callee__ inline bfloat16_t __float2bfloat16_rz(const float x);

__simt_callee__ inline bfloat16_t __float2bfloat16_rd(const float x);

__simt_callee__ inline bfloat16_t __float2bfloat16_ru(const float x);

__simt_callee__ inline bfloat16_t __half2bfloat16_rn(const half x);

__simt_callee__ inline bfloat16_t __half2bfloat16_rz(const half x);

__simt_callee__ inline bfloat16_t __half2bfloat16_rd(const half x);

__simt_callee__ inline bfloat16_t __half2bfloat16_ru(const half x);

__simt_callee__ inline half __bfloat162half_rn(const bfloat16_t x);

__simt_callee__ inline half __bfloat162half_rz(const bfloat16_t x);

__simt_callee__ inline half __bfloat162half_rd(const bfloat16_t x);

__simt_callee__ inline half __bfloat162half_ru(const bfloat16_t x);

__simt_callee__ inline float __bfloat162float(const bfloat16_t x);

__simt_callee__ inline unsigned int __bfloat162uint_rn(const bfloat16_t x);

__simt_callee__ inline unsigned int __bfloat162uint_rz(const bfloat16_t x);

__simt_callee__ inline unsigned int __bfloat162uint_rd(const bfloat16_t x);

__simt_callee__ inline unsigned int __bfloat162uint_ru(const bfloat16_t x);

__simt_callee__ inline int __bfloat162int_rn(const bfloat16_t x);

__simt_callee__ inline int __bfloat162int_rz(const bfloat16_t x);

__simt_callee__ inline int __bfloat162int_rd(const bfloat16_t x);

__simt_callee__ inline int __bfloat162int_ru(const bfloat16_t x);

__simt_callee__ inline unsigned long long int __bfloat162ull_rn(const bfloat16_t x);

__simt_callee__ inline unsigned long long int __bfloat162ull_rz(const bfloat16_t x);

__simt_callee__ inline unsigned long long int __bfloat162ull_rd(const bfloat16_t x);

__simt_callee__ inline unsigned long long int __bfloat162ull_ru(const bfloat16_t x);

__simt_callee__ inline long long int __bfloat162ll_rn(const bfloat16_t x);

__simt_callee__ inline long long int __bfloat162ll_rz(const bfloat16_t x);

__simt_callee__ inline long long int __bfloat162ll_rd(const bfloat16_t x);

__simt_callee__ inline long long int __bfloat162ll_ru(const bfloat16_t x);

__simt_callee__ inline bfloat16_t __uint2bfloat16_rn(const unsigned int x);

__simt_callee__ inline bfloat16_t __uint2bfloat16_rz(const unsigned int x);

__simt_callee__ inline bfloat16_t __uint2bfloat16_rd(const unsigned int x);

__simt_callee__ inline bfloat16_t __uint2bfloat16_ru(const unsigned int x);

__simt_callee__ inline bfloat16_t __int2bfloat16_rn(const int x);

__simt_callee__ inline bfloat16_t __int2bfloat16_rz(const int x);

__simt_callee__ inline bfloat16_t __int2bfloat16_rd(const int x);

__simt_callee__ inline bfloat16_t __int2bfloat16_ru(const int x);

__simt_callee__ inline bfloat16_t __ull2bfloat16_rn(const unsigned long long int x);

__simt_callee__ inline bfloat16_t __ull2bfloat16_rz(const unsigned long long int x);

__simt_callee__ inline bfloat16_t __ull2bfloat16_rd(const unsigned long long int x);

__simt_callee__ inline bfloat16_t __ull2bfloat16_ru(const unsigned long long int x);

__simt_callee__ inline bfloat16_t __ll2bfloat16_rn(const long long int x);

__simt_callee__ inline bfloat16_t __ll2bfloat16_rz(const long long int x);

__simt_callee__ inline bfloat16_t __ll2bfloat16_rd(const long long int x);

__simt_callee__ inline bfloat16_t __ll2bfloat16_ru(const long long int x);

__simt_callee__ inline bfloat16_t asc_atomic_add(__ubuf__ bfloat16_t *address, bfloat16_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_add(__ubuf__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16_t asc_atomic_add(__gm__ bfloat16_t *address, bfloat16_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_add(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_sub(__ubuf__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_sub(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_exch(__ubuf__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_exch(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16_t asc_atomic_max(__ubuf__ bfloat16_t *address, bfloat16_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_max(__ubuf__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16_t asc_atomic_max(__gm__ bfloat16_t *address, bfloat16_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_max(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16_t asc_atomic_min(__ubuf__ bfloat16_t *address, bfloat16_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_min(__ubuf__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16_t asc_atomic_min(__gm__ bfloat16_t *address, bfloat16_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_min(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_cas(__ubuf__ bfloat16x2_t *address, bfloat16x2_t compare, bfloat16x2_t val);

__simt_callee__ inline bfloat16x2_t asc_atomic_cas(__gm__ bfloat16x2_t *address, bfloat16x2_t compare, bfloat16x2_t val);

#include "impl/simt_api/asc_bf16_impl.h"

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_BF16_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_BF16_H__
#endif

#endif  // INCLUDE_SIMT_API_ASC_BF16_H