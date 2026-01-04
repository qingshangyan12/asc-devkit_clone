/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef SIMT_COMPILER_STUB_H
#define SIMT_COMPILER_STUB_H

#include "simt_api/dav_c310/kernel_simt_cpu.h"
#include "kernel_vectorized.h"
#include "stub_def.h"
#include "stub_fun.h"

#define R CAST_RINT
#define A CAST_ROUND
#define F CAST_FLOOR
#define C CAST_CEIL
#define Z CAST_TRUNC
#define O CAST_ODD
#define H CAST_HYBRID

using bfloat16x2_t = bhalf2;

#ifndef SIMT_WARP_SIZE
#define SIMT_WARP_SIZE
constexpr int32_t warpSize = 32;
#endif
constexpr int32_t MAX_SHLF_OFFSET = 31;
constexpr int32_t WARP_SIZE = 32;
constexpr uint32_t half_inf = 0x7C00;
constexpr uint32_t half_max_nan = 0x7FFF;
constexpr uint32_t half_neg_inf = 0xFC00;
constexpr uint32_t b_half_inf = 0x7F80;
constexpr uint32_t b_half_neg_inf = 0xFF80;
constexpr uint32_t b_float_inf = 0x7F800000;
constexpr uint32_t max_nan = 0x7FFFFFFF;
constexpr uint32_t b_float_neg_inf = 0xFF800000;

template <ROUND rnd = ROUND::R, RoundingSaturation sat = RoundingSaturation::RS_DISABLE_VALUE, typename SRC_TYPE>
float __cvt_float(SRC_TYPE src) {
    if constexpr (std::is_same<SRC_TYPE, half>::value) {
        return static_cast<float>(src);
    }
    if constexpr (std::is_same<SRC_TYPE, bfloat16_t>::value) {
        return static_cast<float>(src);
    }
}

template <ROUND rnd = ROUND::R, RoundingSaturation sat = RoundingSaturation::RS_DISABLE_VALUE, typename SRC_TYPE>
half __cvt_half(SRC_TYPE src) {
    if constexpr (std::is_same<SRC_TYPE, float>::value) {
        return half(src);
    }
}

template <ROUND rnd = ROUND::R, RoundingSaturation sat = RoundingSaturation::RS_DISABLE_VALUE, typename SRC_TYPE>
bfloat16_t __cvt_bfloat16_t(SRC_TYPE src) {
    if constexpr (std::is_same<SRC_TYPE, float>::value) {
        return bfloat16_t(src);
    }
}

float __expf(float x);
half __expf(half x);
half2 __expf(half2 x);

float __powf(float x);
half __powf(half x);

float __logf(float x);
half __logf(half x);

float __sqrtf(float x);
half __sqrtf(half x);

namespace bisheng {
namespace cce {
namespace simt {

int32_t atomicAdd(__gm__ int32_t *address, int32_t val);

uint32_t atomicAdd(__gm__ uint32_t *address, uint32_t val);

float atomicAdd(__gm__ float *address, float val);

int64_t atomicAdd(__gm__ int64_t *address, int64_t val);

uint64_t atomicAdd(__gm__ uint64_t *address, uint64_t val);

int32_t atomicSub(__gm__ int32_t *address, int32_t val);

uint32_t atomicSub(__gm__ uint32_t *address, uint32_t val);

float atomicSub(__gm__ float *address, float val);

int64_t atomicSub(__gm__ int64_t *address, int64_t val);

uint64_t atomicSub(__gm__ uint64_t *address, uint64_t val);

int32_t atomicExch(__gm__ int32_t *address, int32_t val);

uint32_t atomicExch(__gm__ uint32_t *address, uint32_t val);

int64_t atomicExch(__gm__ int64_t *address, int64_t val);

uint64_t atomicExch(__gm__ uint64_t *address, uint64_t val);

float atomicExch(__gm__ float *address, float val);

int32_t atomicMax(__gm__ int32_t *address, int32_t val);

uint32_t atomicMax(__gm__ uint32_t *address, uint32_t val);

float atomicMax(__gm__ float *address, float val);

int64_t atomicMax(__gm__ int64_t *address, int64_t val);

uint64_t atomicMax(__gm__ uint64_t *address, uint64_t val);

int32_t atomicMin(__gm__ int32_t *address, int32_t val);

uint32_t atomicMin(__gm__ uint32_t *address, uint32_t val);

float atomicMin(__gm__ float *address, float val);

int64_t atomicMin(__gm__ int64_t *address, int64_t val);

uint64_t atomicMin(__gm__ uint64_t *address, uint64_t val);

uint32_t atomicInc(__gm__ uint32_t *address, uint32_t val);

uint64_t atomicInc(__gm__ uint64_t *address, uint64_t val);

uint32_t atomicDec(__gm__ uint32_t *address, uint32_t val);

uint64_t atomicDec(__gm__ uint64_t *address, uint64_t val);

int32_t atomicCAS(__gm__ int32_t *address, int32_t compare, int32_t val);

uint32_t atomicCAS(__gm__ uint32_t *address, uint32_t compare, uint32_t val);

int64_t atomicCAS(__gm__ int64_t *address, int64_t compare, int64_t val);

uint64_t atomicCAS(__gm__ uint64_t *address, uint64_t compare, uint64_t val);

float atomicCAS(__gm__ float *address, float compare, float val);

int32_t atomicAnd(__gm__ int32_t *address, int32_t val);

uint32_t atomicAnd(__gm__ uint32_t *address, uint32_t val);

int64_t atomicAnd(__gm__ int64_t *address, int64_t val);

uint64_t atomicAnd(__gm__ uint64_t *address, uint64_t val);

int32_t atomicOr(__gm__ int32_t *address, int32_t val);

uint32_t atomicOr(__gm__ uint32_t *address, uint32_t val);

int64_t atomicOr(__gm__ int64_t *address, int64_t val);

uint64_t atomicOr(__gm__ uint64_t *address, uint64_t val);

int32_t atomicXOr(__gm__ int32_t *address, int32_t val);

uint32_t atomicXOr(__gm__ uint32_t *address, uint32_t val);

int64_t atomicXOr(__gm__ int64_t *address, int64_t val);

uint64_t atomicXOr(__gm__ uint64_t *address, uint64_t val);

int32_t __all(int32_t predicate);

int32_t __any(int32_t predicate);

uint32_t __ballot(int32_t predicate);

uint32_t __activemask();

int32_t __shfl(int32_t var, int32_t num);

uint32_t __shfl(uint32_t var, int32_t num);

float __shfl(float var, int32_t num);

int64_t __shfl(int64_t var, int32_t num);

uint64_t __shfl(uint64_t var, int32_t num);

half __shfl(half var, int32_t num);

half2 __shfl(half2 var, int32_t num);

int32_t __shfl_up(int32_t var, int32_t num);

uint32_t __shfl_up(uint32_t var, int32_t num);

float __shfl_up(float var, int32_t num);

int64_t __shfl_up(int64_t var, int32_t num);

uint64_t __shfl_up(uint64_t var, int32_t num);

half __shfl_up(half var, int32_t num);

half2 __shfl_up(half2 var, int32_t num);

int32_t __shfl_down(int32_t var, int32_t num);

uint32_t __shfl_down(uint32_t var, int32_t num);

float __shfl_down(float var, int32_t num);

int64_t __shfl_down(int64_t var, int32_t num);

uint64_t __shfl_down(uint64_t var, int32_t num);

half __shfl_down(half var, int32_t num);

half2 __shfl_down(half2 var, int32_t num);

int32_t __shfl_xor(int32_t var, int32_t num);

uint32_t __shfl_xor(uint32_t var, int32_t num);

float __shfl_xor(float var, int32_t num);

int64_t __shfl_xor(int64_t var, int32_t num);

uint64_t __shfl_xor(uint64_t var, int32_t num);

half __shfl_xor(half var, int32_t num);

half2 __shfl_xor(half2 var, int32_t num);

int32_t __reduce_add(int32_t val);

uint32_t __reduce_add(uint32_t val);

float __reduce_add(float val);

half __reduce_add(half val);

int32_t __reduce_max(int32_t val);

uint32_t __reduce_max(uint32_t val);

float __reduce_max(float val);

half __reduce_max(half val);

int32_t __reduce_min(int32_t val);

uint32_t __reduce_min(uint32_t val);

float __reduce_min(float val);

half __reduce_min(half val);

void __threadfence();

float roundf(float x);

float rintf(float x);

float floorf(float x);

float ceilf(float x);

float __fma(float x, float y, float z);

template<ROUND rnd, RoundingSaturation rst>
int32_t __cvt_int32_t(float x);

template<ROUND rnd, RoundingSaturation rst>
int32_t __cvt_int32_t(half x);

template<ROUND rnd, RoundingSaturation rst>
int32_t __cvt_int32_t(bfloat16_t x);

template<ROUND rnd, RoundingSaturation rst>
uint32_t __cvt_uint32_t(float x);

template<ROUND rnd, RoundingSaturation rst>
uint32_t __cvt_uint32_t(half x);

template<ROUND rnd, RoundingSaturation rst>
uint32_t __cvt_uint32_t(bfloat16_t x);

template<ROUND rnd, RoundingSaturation rst>
int64_t __cvt_int64_t(float x);

template<ROUND rnd, RoundingSaturation rst>
uint64_t __cvt_uint64_t(float x);

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(int32_t x);

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(uint32_t x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(int32_t x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(uint32_t x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(int64_t x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(uint64_t x);

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(int32_t x);

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(uint32_t x);

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(half x);

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(float x);

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(bfloat16_t x);

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(bfloat16_t x);

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(half x);

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(float x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(bfloat16_t x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(half x);

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(float x);

int32_t get_block_idx();

int32_t get_block_num();

half atomicAdd(__gm__ half *address, half val);

bfloat16_t atomicAdd(__gm__ bfloat16_t *address, bfloat16_t val);

half2 atomicAdd(__gm__ half2 *address, half2 val);

bfloat16x2_t atomicAdd(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

half2 atomicSub(__gm__ half2 *address, half2 val);

bfloat16x2_t atomicSub(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

half atomicMax(__gm__ half *address, half val);

bfloat16_t atomicMax(__gm__ bfloat16_t *address, bfloat16_t val);

half2 atomicMax(__gm__ half2 *address, half2 val);

bfloat16x2_t atomicMax(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

half atomicMin(__gm__ half *address, half val);

bfloat16_t atomicMin(__gm__ bfloat16_t *address, bfloat16_t val);

half2 atomicMin(__gm__ half2 *address, half2 val);

bfloat16x2_t atomicMin(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

half2 atomicExch(__gm__ half2 *address, half2 val);

bfloat16x2_t atomicExch(__gm__ bfloat16x2_t *address, bfloat16x2_t val);

half2 atomicCAS(__gm__ half2 *address, half2 compare, half2 val);

bfloat16x2_t atomicCAS(__gm__ bfloat16x2_t *address, bfloat16x2_t compare, bfloat16x2_t val);

}
}
}

void __sync_workitems();

bool __isnan(half x);

bool __isnan(bfloat16_t x);

bool __isinf(half x);

bool __isinf(bfloat16_t x);

bool __isfinite(float x);

float __floorf(float x);

half __floorf(half x);

bfloat16_t __floorf(bfloat16_t x);

float __roundf(float x);

float __rintf(float x);

half __rintf(half x);

bfloat16_t __rintf(bfloat16_t x);

float __ceilf(float x);

half __ceilf(half x);

bfloat16_t __ceilf(bfloat16_t x);

float __fma(float x, float y, float z);

half __fma(half x, half y, half z);

bfloat16_t __fma(bfloat16_t x, bfloat16_t y, bfloat16_t z);

float __fabsf(float x);

float __fmaxf(float x, float y);

float __fminf(float x, float y);

bfloat16_t max(bfloat16_t x, bfloat16_t y);

bfloat16_t min(bfloat16_t x, bfloat16_t y);

#ifndef SIMT_CCE
#define SIMT_CCE
namespace cce {
struct dim3 {
    uint32_t x = 1u, y = 1u, z = 1u;
    dim3(uint32_t x_) { x = x_; }
    dim3(uint32_t x_, uint32_t y_)
    {
        x = x_;
        y = y_;
    }
    dim3(uint32_t x_, uint32_t y_, uint32_t z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
};

template <auto funcPtr, typename... Args>
void async_invoke(const dim3 &dim, Args &&...args)
{
    g_threadDimX = dim.x;
    g_threadDimY = dim.y;
    g_threadDimZ = dim.z;
    AscendC::Simt::ThreadBlock &threadBlock = AscendC::Simt::ThreadBlock::GetBlockInstance();
    const uint32_t threadNum = g_threadDimX * g_threadDimY * g_threadDimZ;
    threadBlock.Init(threadNum);
    auto func = [&args...]() { funcPtr(args...); };
    for (uint32_t i = 0; i < threadNum; i++) {
        threadBlock.Schedule(func, i);
    }
    threadBlock.FinishJobs();
}
}  // namespace cce
#endif

#endif