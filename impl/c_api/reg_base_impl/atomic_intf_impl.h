/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_ATOMIC_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_ATOMIC_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/atomic_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_ATOMIC_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"

// asc_set_atomic_add_float16_impl.h
__aicore__ inline void asc_set_atomic_add_float16_impl()
{
    set_atomic_add();
    set_atomic_f16();
}

// asc_set_atomic_max_float_impl.h
__aicore__ inline void asc_set_atomic_max_float_impl()
{
    set_atomic_max();
    set_atomic_f32();
}

// asc_set_atomic_add_bfloat_impl.h
__aicore__ inline void asc_set_atomic_add_bfloat_impl()
{
    set_atomic_add();
    set_atomic_bf16();
}

// asc_set_atomic_min_float_impl.h
__aicore__ inline void asc_set_atomic_min_float_impl()
{
    set_atomic_min();
    set_atomic_f32();
}

// asc_disable_dma_atomic_impl.h
__aicore__ inline void asc_disable_dma_atomic_impl() { set_atomic_none(); }

// asc_set_atomic_add_float_impl.h
__aicore__ inline void asc_set_atomic_add_float_impl()
{
    set_atomic_f32();
    set_atomic_add();
}

// asc_set_atomic_max_float16_impl.h
__aicore__ inline void asc_set_atomic_max_float16_impl()
{
    set_atomic_f16();
    set_atomic_max();
}

// asc_set_atomic_min_bfloat_impl.h
__aicore__ inline void asc_set_atomic_min_bfloat_impl()
{
    set_atomic_min();
    set_atomic_bf16();
}

// asc_set_atomic_max_bfloat_impl.h
__aicore__ inline void asc_set_atomic_max_bfloat_impl()
{
    set_atomic_max();
    set_atomic_bf16();
}

// asc_set_atomic_min_float16_impl.h
__aicore__ inline void asc_set_atomic_min_float16_impl()
{
    set_atomic_min();
    set_atomic_f16();
}

// asc_set_atomic_add_int8_impl.h
__aicore__ inline void asc_set_atomic_add_int8_impl()
{
    set_atomic_add();
    set_atomic_s8();
}

// asc_set_atomic_add_int16_impl.h
__aicore__ inline void asc_set_atomic_add_int16_impl()
{
    set_atomic_add();
    set_atomic_s16();
}

// asc_set_atomic_max_int8_impl.h
__aicore__ inline void asc_set_atomic_max_int8_impl()
{
    set_atomic_max();
    set_atomic_s8();
}

// asc_set_atomic_max_int16_impl.h
__aicore__ inline void asc_set_atomic_max_int16_impl()
{
    set_atomic_max();
    set_atomic_s16();
}

// asc_set_atomic_min_int8_impl.h
__aicore__ inline void asc_set_atomic_min_int8_impl()
{
    set_atomic_min();
    set_atomic_s8();
}

// asc_set_atomic_min_int16_impl.h
__aicore__ inline void asc_set_atomic_min_int16_impl()
{
    set_atomic_min();
    set_atomic_s16();
}

// asc_set_atomic_add_int32_impl.h
__aicore__ inline void asc_set_atomic_add_int32_impl()
{
    set_atomic_add();
    set_atomic_s32();
}

// asc_set_atomic_max_int32_impl.h
__aicore__ inline void asc_set_atomic_max_int32_impl()
{
    set_atomic_max();
    set_atomic_s32();
}

// asc_set_atomic_min_int32_impl.h
__aicore__ inline void asc_set_atomic_min_int32_impl()
{
    set_atomic_min();
    set_atomic_s32();
}

// atomic_impl.h
/// Enables atomic add for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with half precision (half).
__aicore__ inline void asc_set_atomic_add_float16() { asc_set_atomic_add_float16_impl(); }

/// Enables atomic max for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with single precision (float).
__aicore__ inline void asc_set_atomic_max_float() { asc_set_atomic_max_float_impl(); }

/// Enables atomic add for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with bfloat16 precision (bfloat16_t).
__aicore__ inline void asc_set_atomic_add_bfloat() { asc_set_atomic_add_bfloat_impl(); }

/// Enables atomic min for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with single precision (float).
__aicore__ inline void asc_set_atomic_min_float() { asc_set_atomic_min_float_impl(); }

/// Enables atomic min for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with bfloat16 precision (bfloat16_t).
__aicore__ inline void asc_set_atomic_min_bfloat() { asc_set_atomic_min_bfloat_impl(); }

/// Enables atomic add for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with single precision (float).
__aicore__ inline void asc_set_atomic_add_float() { asc_set_atomic_add_float_impl(); }

/// Enables atomic max for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with half precision (half).
__aicore__ inline void asc_set_atomic_max_float16() { asc_set_atomic_max_float16_impl(); }

/// Clears the atomic operation state, disabling any active atomic add/max/min.
__aicore__ inline void asc_disable_dma_atomic() { asc_disable_dma_atomic_impl(); }

/// Enables atomic max for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with bfloat16 precision (bfloat16_t).
__aicore__ inline void asc_set_atomic_max_bfloat() { asc_set_atomic_max_bfloat_impl(); }

/// Enables atomic min for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with half precision (half).
__aicore__ inline void asc_set_atomic_min_float16() { asc_set_atomic_min_float16_impl(); }

/// Enables atomic add for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 8-bit signed integer (int8_t).
__aicore__ inline void asc_set_atomic_add_int8() { asc_set_atomic_add_int8_impl(); }

/// Enables atomic add for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 16-bit signed integer (int16_t).
__aicore__ inline void asc_set_atomic_add_int16() { asc_set_atomic_add_int16_impl(); }

/// Enables atomic add for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 32-bit signed integer (int32_t).
__aicore__ inline void asc_set_atomic_add_int32() { asc_set_atomic_add_int32_impl(); }

/// Enables atomic max for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 8-bit signed integer (int8_t).
__aicore__ inline void asc_set_atomic_max_int8() { asc_set_atomic_max_int8_impl(); }

/// Enables atomic max for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 16-bit signed integer (int16_t).
__aicore__ inline void asc_set_atomic_max_int16() { asc_set_atomic_max_int16_impl(); }

/// Enables atomic max for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 32-bit signed integer (int32_t).
__aicore__ inline void asc_set_atomic_max_int32() { asc_set_atomic_max_int32_impl(); }

/// Enables atomic min for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 8-bit signed integer (int8_t).
__aicore__ inline void asc_set_atomic_min_int8() { asc_set_atomic_min_int8_impl(); }

/// Enables atomic min for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 16-bit signed integer (int16_t).
__aicore__ inline void asc_set_atomic_min_int16() { asc_set_atomic_min_int16_impl(); }

/// Enables atomic min for data transfer from Unified Buffer/L0C Buffer/L1 Buffer to
/// Global Memory with 32-bit signed integer (int32_t).
__aicore__ inline void asc_set_atomic_min_int32() { asc_set_atomic_min_int32_impl(); }

// asc_atomic_add_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_add_impl(__gm__ int32_t* address, int32_t val) { return atomicAdd(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_add_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicAdd(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_add_impl(__gm__ float* address, float val) { return atomicAdd(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_add_impl(__gm__ int64_t* address, int64_t val) { return atomicAdd(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_add_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicAdd(address, val);
}
} // namespace __asc_aicore

// asc_atomic_sub_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_sub_impl(__gm__ int32_t* address, int32_t val) { return atomicSub(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_sub_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicSub(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_sub_impl(__gm__ float* address, float val) { return atomicSub(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_sub_impl(__gm__ int64_t* address, int64_t val) { return atomicSub(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_sub_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicSub(address, val);
}
} // namespace __asc_aicore

// asc_atomic_exch_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_exch_impl(__gm__ int32_t* address, int32_t val)
{
    return atomicExch(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_exch_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicExch(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_exch_impl(__gm__ float* address, float val) { return atomicExch(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_exch_impl(__gm__ int64_t* address, int64_t val)
{
    return atomicExch(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_exch_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicExch(address, val);
}
} // namespace __asc_aicore

// asc_atomic_max_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_max_impl(__gm__ int32_t* address, int32_t val) { return atomicMax(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_max_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicMax(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_max_impl(__gm__ float* address, float val) { return atomicMax(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_max_impl(__gm__ int64_t* address, int64_t val) { return atomicMax(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_max_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicMax(address, val);
}
} // namespace __asc_aicore

// asc_atomic_min_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_min_impl(__gm__ int32_t* address, int32_t val) { return atomicMin(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_min_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicMin(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_min_impl(__gm__ float* address, float val) { return atomicMin(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_min_impl(__gm__ int64_t* address, int64_t val) { return atomicMin(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_min_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicMin(address, val);
}
} // namespace __asc_aicore

// asc_atomic_cas_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_cas_impl(__gm__ int32_t* address, int32_t compare, int32_t val)
{
    return atomicCAS(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_cas_impl(__gm__ uint32_t* address, uint32_t compare, uint32_t val)
{
    return atomicCAS(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_cas_impl(__gm__ float* address, float compare, float val)
{
    return atomicCAS(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_cas_impl(__gm__ int64_t* address, int64_t compare, int64_t val)
{
    return atomicCAS(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_cas_impl(__gm__ uint64_t* address, uint64_t compare, uint64_t val)
{
    return atomicCAS(address, compare, val);
}
} // namespace __asc_aicore

// asc_atomic_and_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_and_impl(__gm__ int32_t* address, int32_t val) { return atomicAnd(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_and_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicAnd(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_and_impl(__gm__ int64_t* address, int64_t val) { return atomicAnd(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_and_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicAnd(address, val);
}
} // namespace __asc_aicore

// asc_atomic_or_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_or_impl(__gm__ int32_t* address, int32_t val) { return atomicOr(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_or_impl(__gm__ uint32_t* address, uint32_t val) { return atomicOr(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_or_impl(__gm__ int64_t* address, int64_t val) { return atomicOr(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_or_impl(__gm__ uint64_t* address, uint64_t val) { return atomicOr(address, val); }
} // namespace __asc_aicore

// asc_atomic_xor_impl.h
namespace __asc_aicore {
__aicore__ inline int32_t asc_atomic_xor_impl(__gm__ int32_t* address, int32_t val) { return atomicXOr(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_xor_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicXOr(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_xor_impl(__gm__ int64_t* address, int64_t val) { return atomicXOr(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_xor_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicXOr(address, val);
}
} // namespace __asc_aicore

// asc_atomic_inc_impl.h
namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_inc_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicInc(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_inc_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicInc(address, val);
}
} // namespace __asc_aicore

// asc_atomic_dec_impl.h
namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_dec_impl(__gm__ uint32_t* address, uint32_t val)
{
    return atomicDec(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_dec_impl(__gm__ uint64_t* address, uint64_t val)
{
    return atomicDec(address, val);
}
} // namespace __asc_aicore

// scalar_compute_impl.h
namespace __asc_aicore {
// ==========asc_atomic_add==========
__aicore__ inline int32_t asc_atomic_add(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_add_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_add(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_add_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_add(__gm__ float* address, float val) { return asc_atomic_add_impl(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_add(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_add_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_add(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_add_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_sub==========
__aicore__ inline int32_t asc_atomic_sub(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_sub_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_sub(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_sub_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_sub(__gm__ float* address, float val) { return asc_atomic_sub_impl(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_sub(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_sub_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_sub(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_sub_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_exch==========
__aicore__ inline int32_t asc_atomic_exch(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_exch_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_exch(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_exch_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_exch(__gm__ float* address, float val) { return asc_atomic_exch_impl(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_exch(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_exch_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_exch(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_exch_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_max==========
__aicore__ inline int32_t asc_atomic_max(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_max_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_max(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_max_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_max(__gm__ float* address, float val) { return asc_atomic_max_impl(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_max(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_max_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_max(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_max_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_min==========
__aicore__ inline int32_t asc_atomic_min(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_min_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_min(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_min_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_min(__gm__ float* address, float val) { return asc_atomic_min_impl(address, val); }
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_min(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_min_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_min(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_min_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_cas==========
__aicore__ inline int32_t asc_atomic_cas(__gm__ int32_t* address, int32_t compare, int32_t val)
{
    return asc_atomic_cas_impl(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_cas(__gm__ uint32_t* address, uint32_t compare, uint32_t val)
{
    return asc_atomic_cas_impl(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline float asc_atomic_cas(__gm__ float* address, float compare, float val)
{
    return asc_atomic_cas_impl(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_cas(__gm__ int64_t* address, int64_t compare, int64_t val)
{
    return asc_atomic_cas_impl(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_cas(__gm__ uint64_t* address, uint64_t compare, uint64_t val)
{
    return asc_atomic_cas_impl(address, compare, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_and==========
__aicore__ inline int32_t asc_atomic_and(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_and_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_and(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_and_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_and(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_and_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_and(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_and_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_or==========
__aicore__ inline int32_t asc_atomic_or(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_or_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_or(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_or_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_or(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_or_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_or(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_or_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_xor==========
__aicore__ inline int32_t asc_atomic_xor(__gm__ int32_t* address, int32_t val)
{
    return asc_atomic_xor_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint32_t asc_atomic_xor(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_xor_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline int64_t asc_atomic_xor(__gm__ int64_t* address, int64_t val)
{
    return asc_atomic_xor_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_xor(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_xor_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_inc==========
__aicore__ inline uint32_t asc_atomic_inc(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_inc_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_inc(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_inc_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
// ==========asc_atomic_dec==========
__aicore__ inline uint32_t asc_atomic_dec(__gm__ uint32_t* address, uint32_t val)
{
    return asc_atomic_dec_impl(address, val);
}
} // namespace __asc_aicore

namespace __asc_aicore {
__aicore__ inline uint64_t asc_atomic_dec(__gm__ uint64_t* address, uint64_t val)
{
    return asc_atomic_dec_impl(address, val);
}
} // namespace __asc_aicore

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_ATOMIC_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_ATOMIC_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_ATOMIC_INTF_IMPL_H
