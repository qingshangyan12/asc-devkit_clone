/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_SIMT_API_ASC_FP16_H
#define INCLUDE_SIMT_API_ASC_FP16_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP16_H__
#endif

#include "simt_api/asc_simt.h"

__simt_callee__ inline bool __hisnan(half x);

__simt_callee__ inline bool __hisinf(half x);

__simt_callee__ inline half __hfma(half x, half y, half z);

__simt_callee__ inline half __habs(half x);

__simt_callee__ inline half __hmax(half x, half y);

__simt_callee__ inline half __hmin(half x, half y);

__simt_callee__ inline half hcos(half x);
__simt_callee__ inline half hsin(half x);
__simt_callee__ inline half htanh(half x);

__simt_callee__ inline half hexp(half x);
__simt_callee__ inline half hexp2(half x);
__simt_callee__ inline half hexp10(half x);

__simt_callee__ inline half hlog(half x);
__simt_callee__ inline half hlog2(half x);
__simt_callee__ inline half hlog10(half x);

__simt_callee__ inline half hsqrt(half x);
__simt_callee__ inline half hrsqrt(half x);

__simt_callee__ inline half hrcp(half x);

__simt_callee__ inline half hfloor(half x);

__simt_callee__ inline half hrint(half x);

__simt_callee__ inline half hceil(half x);

__simt_callee__ inline half htrunc(half x);

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

__simt_callee__ inline half asc_atomic_add(__ubuf__ half *address, half val);

__simt_callee__ inline half2 asc_atomic_add(__ubuf__ half2 *address, half2 val);

__simt_callee__ inline half asc_atomic_add(__gm__ half *address, half val);

__simt_callee__ inline half2 asc_atomic_add(__gm__ half2 *address, half2 val);

__simt_callee__ inline half2 asc_atomic_sub(__ubuf__ half2 *address, half2 val);

__simt_callee__ inline half2 asc_atomic_sub(__gm__ half2 *address, half2 val);

__simt_callee__ inline half2 asc_atomic_exch(__ubuf__ half2 *address, half2 val);

__simt_callee__ inline half2 asc_atomic_exch(__gm__ half2 *address, half2 val);

__simt_callee__ inline half asc_atomic_max(__ubuf__ half *address, half val);

__simt_callee__ inline half2 asc_atomic_max(__ubuf__ half2 *address, half2 val);

__simt_callee__ inline half asc_atomic_max(__gm__ half *address, half val);

__simt_callee__ inline half2 asc_atomic_max(__gm__ half2 *address, half2 val);

__simt_callee__ inline half asc_atomic_min(__ubuf__ half *address, half val);

__simt_callee__ inline half2 asc_atomic_min(__ubuf__ half2 *address, half2 val);

__simt_callee__ inline half asc_atomic_min(__gm__ half *address, half val);

__simt_callee__ inline half2 asc_atomic_min(__gm__ half2 *address, half2 val);

__simt_callee__ inline half2 asc_atomic_cas(__ubuf__ half2 *address, half2 compare, half2 val);

__simt_callee__ inline half2 asc_atomic_cas(__gm__ half2 *address, half2 compare, half2 val);

__simt_callee__ inline half asc_shfl(half var, int32_t src_lane, int32_t width = warpSize);

__simt_callee__ inline half2 asc_shfl(half2 var, int32_t src_lane, int32_t width = warpSize);

__simt_callee__ inline half asc_shfl_up(half var, uint32_t delta, int32_t width = warpSize);

__simt_callee__ inline half2 asc_shfl_up(half2 var, uint32_t delta, int32_t width = warpSize);

__simt_callee__ inline half asc_shfl_down(half var, uint32_t delta, int32_t width = warpSize);

__simt_callee__ inline half2 asc_shfl_down(half2 var, uint32_t delta, int32_t width = warpSize);

__simt_callee__ inline half asc_shfl_xor(half var, int32_t lane_mask, int32_t width = warpSize);

__simt_callee__ inline half2 asc_shfl_xor(half2 var, int32_t lane_mask, int32_t width = warpSize);

__simt_callee__ inline half asc_reduce_add(half val);

__simt_callee__ inline half asc_reduce_max(half val);

__simt_callee__ inline half asc_reduce_min(half val);

#include "impl/simt_api/asc_fp16_impl.h"

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP16_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_FP16_H__
#endif
#endif  // INCLUDE_SIMT_API_ASC_FP16_H