#ifndef __CCE_AICORE_INTRINSICS_STUBS__
#define __CCE_AICORE_INTRINSICS_STUBS__
#include <cstdint>
#include "kernel_bf16.h"
#include "kernel_fp16.h"
#include "kernel_vectorized.h"
#include "kernel_fp8_e5m2.h"
#include "kernel_fp8_e4m3.h"
#include "kernel_fp8_e8m0.h"
#include "kernel_fp4_e2m1.h"
#include "kernel_fp4_e1m2.h"
#include "kernel_hif8.h"
#include "stub_def.h"
typedef enum {
    CAST_RINT = 0,
    CAST_ROUND = 1,
    CAST_FLOOR = 2,
    CAST_CEIL = 3,
    CAST_TRUNC = 4,
    CAST_ODD = 5,
    CAST_HYBRID = 6,
} ROUND;

typedef enum {
    RS_DISABLE_VALUE = 0,
    RS_ENABLE_VALUE = 1,
} RoundingSaturation;

int64_t get_arch_ver();
int64_t get_block_idx();
int64_t get_block_num();
int __cce_simt_get_BLOCK_DIM_X();
int __cce_simt_get_BLOCK_DIM_Y();
int __cce_simt_get_BLOCK_DIM_Z();
int __cce_simt_get_TID_X();
int __cce_simt_get_TID_Y();
int __cce_simt_get_TID_Z();
void set_block_dim(int idx);
uint32_t atomicSub(__gm__ uint32_t* address, uint32_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t atomicSub(__gm__ uint32_t* address, uint32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t atomicSub(__gm__ int32_t* address, int32_t value);
namespace bisheng {
namespace cce {
namespace simt {
int32_t atomicSub(__gm__ int32_t* address, int32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t atomicSub(__gm__ uint64_t* address, uint64_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t atomicSub(__gm__ uint64_t* address, uint64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t atomicSub(__gm__ int64_t* address, int64_t value);
namespace bisheng {
namespace cce {
namespace simt {
int64_t atomicSub(__gm__ int64_t* address, int64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float atomicSub(__gm__ float* address, float value);
namespace bisheng {
namespace cce {
namespace simt {
float atomicSub(__gm__ float* address, float value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t atomicExch(__gm__ int32_t* address, int32_t value);
namespace bisheng {
namespace cce {
namespace simt {
int32_t atomicExch(__gm__ int32_t* address, int32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t atomicExch(__gm__ int64_t* address, int64_t value);
namespace bisheng {
namespace cce {
namespace simt {
int64_t atomicExch(__gm__ int64_t* address, int64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float atomicExch(__gm__ float* address, float value);
namespace bisheng {
namespace cce {
namespace simt {
float atomicExch(__gm__ float* address, float value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t atomicInc(__gm__ uint32_t* address, uint32_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t atomicInc(__gm__ uint32_t* address, uint32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t atomicInc(__gm__ uint64_t* address, uint64_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t atomicInc(__gm__ uint64_t* address, uint64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t atomicDec(__gm__ uint32_t* address, uint32_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t atomicDec(__gm__ uint32_t* address, uint32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t atomicDec(__gm__ uint64_t* address, uint64_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t atomicDec(__gm__ uint64_t* address, uint64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t atomicCAS(__gm__ int32_t* address, int32_t value1, int32_t value2);
namespace bisheng {
namespace cce {
namespace simt {
int32_t atomicCAS(__gm__ int32_t* address, int32_t value1, int32_t value2); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t atomicCAS(__gm__ int64_t* address, int64_t value1, int64_t value2);
namespace bisheng {
namespace cce {
namespace simt {
int64_t atomicCAS(__gm__ int64_t* address, int64_t value1, int64_t value2); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float atomicCAS(__gm__ float* address, float value1, float value2);
namespace bisheng {
namespace cce {
namespace simt {
float atomicCAS(__gm__ float* address, float value1, float value2); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t atomicAnd(__gm__ uint32_t* address, uint32_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t atomicAnd(__gm__ uint32_t* address, uint32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t atomicAnd(__gm__ int32_t* address, int32_t value);
namespace bisheng {
namespace cce {
namespace simt {
int32_t atomicAnd(__gm__ int32_t* address, int32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t atomicAnd(__gm__ uint64_t* address, uint64_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t atomicAnd(__gm__ uint64_t* address, uint64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t atomicAnd(__gm__ int64_t* address, int64_t value);
namespace bisheng {
namespace cce {
namespace simt {
int64_t atomicAnd(__gm__ int64_t* address, int64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t atomicOr(__gm__ uint32_t* address, uint32_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t atomicOr(__gm__ uint32_t* address, uint32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t atomicOr(__gm__ int32_t* address, int32_t value);
namespace bisheng {
namespace cce {
namespace simt {
int32_t atomicOr(__gm__ int32_t* address, int32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t atomicOr(__gm__ uint64_t* address, uint64_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t atomicOr(__gm__ uint64_t* address, uint64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t atomicOr(__gm__ int64_t* address, int64_t value);
namespace bisheng {
namespace cce {
namespace simt {
int64_t atomicOr(__gm__ int64_t* address, int64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t atomicXOr(__gm__ uint32_t* address, uint32_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t atomicXOr(__gm__ uint32_t* address, uint32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t atomicXOr(__gm__ int32_t* address, int32_t value);
namespace bisheng {
namespace cce {
namespace simt {
int32_t atomicXOr(__gm__ int32_t* address, int32_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t atomicXOr(__gm__ uint64_t* address, uint64_t value);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t atomicXOr(__gm__ uint64_t* address, uint64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t atomicXOr(__gm__ int64_t* address, int64_t value);
namespace bisheng {
namespace cce {
namespace simt {
int64_t atomicXOr(__gm__ int64_t* address, int64_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
void __sync_workitems();
void __threadfence();
namespace bisheng {
namespace cce {
namespace simt {
void __threadfence(); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
void __threadfence_block();
int32_t __all(int32_t predicate);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __all(int32_t predicate); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __any(int32_t predicate);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __any(int32_t predicate); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __ballot(int32_t predicate);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __ballot(int32_t predicate); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __activemask();
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __activemask(); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __shfl(int32_t var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __shfl(int32_t var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __shfl(uint32_t var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __shfl(uint32_t var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __shfl(float var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
float __shfl(float var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t __shfl(int64_t var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int64_t __shfl(int64_t var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t __shfl(uint64_t var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t __shfl(uint64_t var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __shfl_up(int32_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __shfl_up(int32_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __shfl_up(uint32_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __shfl_up(uint32_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __shfl_up(float var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
float __shfl_up(float var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t __shfl_up(int64_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int64_t __shfl_up(int64_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t __shfl_up(uint64_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t __shfl_up(uint64_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __shfl_down(int32_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __shfl_down(int32_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __shfl_down(uint32_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __shfl_down(uint32_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __shfl_down(float var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
float __shfl_down(float var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t __shfl_down(int64_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int64_t __shfl_down(int64_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t __shfl_down(uint64_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t __shfl_down(uint64_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __shfl_xor(int32_t var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __shfl_xor(int32_t var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __shfl_xor(uint32_t var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __shfl_xor(uint32_t var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __shfl_xor(float var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
float __shfl_xor(float var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int64_t __shfl_xor(int64_t var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
int64_t __shfl_xor(int64_t var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint64_t __shfl_xor(uint64_t var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
uint64_t __shfl_xor(uint64_t var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __reduce_add(int32_t val);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __reduce_add(int32_t val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __reduce_add(uint32_t val);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __reduce_add(uint32_t val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __reduce_add(float val);
namespace bisheng {
namespace cce {
namespace simt {
float __reduce_add(float val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __reduce_max(int32_t val);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __reduce_max(int32_t val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __reduce_max(uint32_t val);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __reduce_max(uint32_t val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __reduce_max(float val);
namespace bisheng {
namespace cce {
namespace simt {
float __reduce_max(float val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
int32_t __reduce_min(int32_t val);
namespace bisheng {
namespace cce {
namespace simt {
int32_t __reduce_min(int32_t val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
uint32_t __reduce_min(uint32_t val);
namespace bisheng {
namespace cce {
namespace simt {
uint32_t __reduce_min(uint32_t val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __reduce_min(float val);
namespace bisheng {
namespace cce {
namespace simt {
float __reduce_min(float val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __expf(float x);
float __logf(float x);
float __sqrtf(float x);
bool __isfinite(float x);
bool __isnan(float x);
bool __isinf(float x);
float __powf(float x, float y);
int64_t max(const long long int x, const long long int y);
uint64_t max(const unsigned long long int x, const unsigned long long int y);
uint32_t max(const unsigned int x, const unsigned int y);
int64_t min(const long long int x, const long long int y);
uint64_t min(const unsigned long long int x, const unsigned long long int y);
uint32_t min(const unsigned int x, const unsigned int y);
int signbitf(float x);
float __fmaxf(float x, float y);
float __fminf(float x, float y);
float __roundf(float x);
namespace bisheng {
namespace cce {
namespace simt {
float __roundf(float x); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __rintf(float x);
namespace bisheng {
namespace cce {
namespace simt {
float __rintf(float x); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __floorf(float x);
namespace bisheng {
namespace cce {
namespace simt {
float __floorf(float x); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __ceilf(float x);
namespace bisheng {
namespace cce {
namespace simt {
float __ceilf(float x); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __fma(float x, float y, float z);
namespace bisheng {
namespace cce {
namespace simt {
float __fma(float x, float y, float z); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bool __isnan(half x);
bool __isinf(half x);
half __fma(half x, half y, half z);
half __expf(half x);
half __logf(half x);
half __sqrtf(half x);
half __rintf(half x);
half __floorf(half x);
half __ceilf(half x);
half2 __expf(half2 x);
half2 __logf(half2 x);
half2 __sqrtf(half2 x);
half atomicAdd(__gm__ half* address, half value);
namespace bisheng {
namespace cce {
namespace simt {
half atomicAdd(__gm__ half* address, half value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half atomicMax(__gm__ half* address, half value);
namespace bisheng {
namespace cce {
namespace simt {
half atomicMax(__gm__ half* address, half value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half atomicMin(__gm__ half* address, half value);
namespace bisheng {
namespace cce {
namespace simt {
half atomicMin(__gm__ half* address, half value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 atomicAdd(__gm__ half2* address, half2 value);
namespace bisheng {
namespace cce {
namespace simt {
half2 atomicAdd(__gm__ half2* address, half2 value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 atomicSub(__gm__ half2* address, half2 value);
namespace bisheng {
namespace cce {
namespace simt {
half2 atomicSub(__gm__ half2* address, half2 value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 atomicExch(__gm__ half2* address, half2 value);
namespace bisheng {
namespace cce {
namespace simt {
half2 atomicExch(__gm__ half2* address, half2 value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 atomicMax(__gm__ half2* address, half2 value);
namespace bisheng {
namespace cce {
namespace simt {
half2 atomicMax(__gm__ half2* address, half2 value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 atomicMin(__gm__ half2* address, half2 value);
namespace bisheng {
namespace cce {
namespace simt {
half2 atomicMin(__gm__ half2* address, half2 value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 atomicCAS(__gm__ half2* address, half2 value1, half2 value2);
namespace bisheng {
namespace cce {
namespace simt {
half2 atomicCAS(__gm__ half2* address, half2 value1, half2 value2); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __shfl(half var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half __shfl(half var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 __shfl(half2 var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half2 __shfl(half2 var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __shfl_up(half var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half __shfl_up(half var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 __shfl_up(half2 var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half2 __shfl_up(half2 var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __shfl_down(half var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half __shfl_down(half var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 __shfl_down(half2 var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half2 __shfl_down(half2 var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __shfl_xor(half var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half __shfl_xor(half var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half2 __shfl_xor(half2 var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
half2 __shfl_xor(half2 var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16_t __shfl(bfloat16_t var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t __shfl(bfloat16_t var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t __shfl(bfloat16x2_t var, int32_t src_lane, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t __shfl(bfloat16x2_t var, int32_t src_lane, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16_t __shfl_up(bfloat16_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t __shfl_up(bfloat16_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t __shfl_up(bfloat16x2_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t __shfl_up(bfloat16x2_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16_t __shfl_down(bfloat16_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t __shfl_down(bfloat16_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t __shfl_down(bfloat16x2_t var, int32_t delta, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t __shfl_down(bfloat16x2_t var, int32_t delta, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16_t __shfl_xor(bfloat16_t var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t __shfl_xor(bfloat16_t var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t __shfl_xor(bfloat16x2_t var, int32_t lane_mask, int32_t width);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t __shfl_xor(bfloat16x2_t var, int32_t lane_mask, int32_t width); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __reduce_add(half val);
namespace bisheng {
namespace cce {
namespace simt {
half __reduce_add(half val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __reduce_max(half val);
namespace bisheng {
namespace cce {
namespace simt {
half __reduce_max(half val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
half __reduce_min(half val);
namespace bisheng {
namespace cce {
namespace simt {
half __reduce_min(half val); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bool __isnan(bfloat16_t x);
bool __isinf(bfloat16_t x);
bfloat16_t __fma(bfloat16_t x, bfloat16_t y, bfloat16_t z);
bfloat16_t __max(bfloat16_t x, bfloat16_t y);
bfloat16_t __min(bfloat16_t x, bfloat16_t y);
bfloat16_t __floorf(bfloat16_t x);
bfloat16_t __rintf(bfloat16_t x);
bfloat16_t __ceilf(bfloat16_t x);
bfloat16_t atomicAdd(__gm__ bfloat16_t* address, bfloat16_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t atomicAdd(__gm__ bfloat16_t* address, bfloat16_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16_t atomicMax(__gm__ bfloat16_t* address, bfloat16_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t atomicMax(__gm__ bfloat16_t* address, bfloat16_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16_t atomicMin(__gm__ bfloat16_t* address, bfloat16_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16_t atomicMin(__gm__ bfloat16_t* address, bfloat16_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t atomicAdd(__gm__ bfloat16x2_t* address, bfloat16x2_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t atomicAdd(__gm__ bfloat16x2_t* address, bfloat16x2_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t atomicSub(__gm__ bfloat16x2_t* address, bfloat16x2_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t atomicSub(__gm__ bfloat16x2_t* address, bfloat16x2_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t atomicExch(__gm__ bfloat16x2_t* address, bfloat16x2_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t atomicExch(__gm__ bfloat16x2_t* address, bfloat16x2_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t atomicMax(__gm__ bfloat16x2_t* address, bfloat16x2_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t atomicMax(__gm__ bfloat16x2_t* address, bfloat16x2_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t atomicMin(__gm__ bfloat16x2_t* address, bfloat16x2_t value);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t atomicMin(__gm__ bfloat16x2_t* address, bfloat16x2_t value); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
bfloat16x2_t atomicCAS(__gm__ bfloat16x2_t* address, bfloat16x2_t value1, bfloat16x2_t value2);
namespace bisheng {
namespace cce {
namespace simt {
bfloat16x2_t atomicCAS(__gm__ bfloat16x2_t* address, bfloat16x2_t value1, bfloat16x2_t value2); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
float __fabsf(float x);
namespace bisheng {
namespace cce {
namespace simt {
float __fabsf(float x); //namespace bisheng  //namespace cce  //namespace simt
} // namespace bisheng
} // namespace cce
} // namespace simt
#endif
