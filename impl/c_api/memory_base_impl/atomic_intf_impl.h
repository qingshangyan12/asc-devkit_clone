/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_ATOMIC_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_ATOMIC_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/memory_base_impl/atomic_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_ATOMIC_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_intf_impl.h"

// asc_disable_dma_atomic_impl.h
__aicore__ inline void asc_disable_dma_atomic_impl() { set_atomic_none(); }

// asc_set_atomic_none_impl.h
__aicore__ inline void asc_set_atomic_none_impl() { set_atomic_none(); }

// asc_get_store_atomic_config_impl.h
__aicore__ inline void asc_get_store_atomic_config_impl(asc_store_atomic_config& config)
{
    config.config = get_st_atomic_cfg();
}

// asc_set_store_atomic_config_impl.h
__aicore__ inline void asc_set_store_atomic_config_v1_impl(uint16_t type, uint16_t op)
{
    constexpr uint64_t type_mask = 0x7;
    constexpr uint64_t op_mask = 0x3;
    constexpr uint64_t op_bit = 3;
    uint64_t config = (type & type_mask) | ((op & op_mask) << op_bit);
    set_st_atomic_cfg(config);
}

// asc_set_atomic_add_bfloat_impl.h
__aicore__ inline void asc_set_atomic_add_bfloat_impl()
{
    set_atomic_add();
    set_atomic_bf16();
}

// asc_set_atomic_add_float_impl.h
__aicore__ inline void asc_set_atomic_add_float_impl()
{
    set_atomic_add();
    set_atomic_f32();
}

// asc_set_atomic_add_float16_impl.h
__aicore__ inline void asc_set_atomic_add_float16_impl()
{
    set_atomic_add();
    set_atomic_f16();
}

// asc_set_atomic_add_int_impl.h
__aicore__ inline void asc_set_atomic_add_int_impl()
{
    set_atomic_add();
    set_atomic_s32();
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

// asc_set_atomic_max_bfloat_impl.h
__aicore__ inline void asc_set_atomic_max_bfloat_impl()
{
    set_atomic_max();
    set_atomic_bf16();
}

// asc_set_atomic_max_float_impl.h
__aicore__ inline void asc_set_atomic_max_float_impl()
{
    set_atomic_max();
    set_atomic_f32();
}

// asc_set_atomic_max_float16_impl.h
__aicore__ inline void asc_set_atomic_max_float16_impl()
{
    set_atomic_max();
    set_atomic_f16();
}

// asc_set_atomic_max_int_impl.h
__aicore__ inline void asc_set_atomic_max_int_impl()
{
    set_atomic_max();
    set_atomic_s32();
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

// asc_set_atomic_min_bfloat_impl.h
__aicore__ inline void asc_set_atomic_min_bfloat_impl()
{
    set_atomic_min();
    set_atomic_bf16();
}

// asc_set_atomic_min_float_impl.h
__aicore__ inline void asc_set_atomic_min_float_impl()
{
    set_atomic_min();
    set_atomic_f32();
}

// asc_set_atomic_min_float16_impl.h
__aicore__ inline void asc_set_atomic_min_float16_impl()
{
    set_atomic_min();
    set_atomic_f16();
}

// asc_set_atomic_min_int_impl.h
__aicore__ inline void asc_set_atomic_min_int_impl()
{
    set_atomic_min();
    set_atomic_s32();
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
__aicore__ inline void asc_disable_dma_atomic() { asc_disable_dma_atomic_impl(); }

[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
__aicore__ inline void asc_set_atomic_none()
{
    asc_disable_dma_atomic();
}

__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config)
{
    asc_get_store_atomic_config_impl(config);
}

__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op)
{
    asc_set_store_atomic_config_v1_impl(type, op);
}

__aicore__ inline void asc_set_atomic_add_bfloat() { asc_set_atomic_add_bfloat_impl(); }

__aicore__ inline void asc_set_atomic_add_float() { asc_set_atomic_add_float_impl(); }

__aicore__ inline void asc_set_atomic_add_float16() { asc_set_atomic_add_float16_impl(); }

[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
__aicore__ inline void asc_set_atomic_add_int()
{
    asc_set_atomic_add_int_impl();
}

__aicore__ inline void asc_set_atomic_add_int8() { asc_set_atomic_add_int8_impl(); }

__aicore__ inline void asc_set_atomic_add_int16() { asc_set_atomic_add_int16_impl(); }

__aicore__ inline void asc_set_atomic_add_int32() { asc_set_atomic_add_int32_impl(); }

__aicore__ inline void asc_set_atomic_max_bfloat() { asc_set_atomic_max_bfloat_impl(); }

__aicore__ inline void asc_set_atomic_max_float() { asc_set_atomic_max_float_impl(); }

__aicore__ inline void asc_set_atomic_max_float16() { asc_set_atomic_max_float16_impl(); }

[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
__aicore__ inline void asc_set_atomic_max_int()
{
    asc_set_atomic_max_int_impl();
}

__aicore__ inline void asc_set_atomic_max_int8() { asc_set_atomic_max_int8_impl(); }

__aicore__ inline void asc_set_atomic_max_int16() { asc_set_atomic_max_int16_impl(); }

__aicore__ inline void asc_set_atomic_max_int32() { asc_set_atomic_max_int32_impl(); }

__aicore__ inline void asc_set_atomic_min_bfloat() { asc_set_atomic_min_bfloat_impl(); }

__aicore__ inline void asc_set_atomic_min_float() { asc_set_atomic_min_float_impl(); }

__aicore__ inline void asc_set_atomic_min_float16() { asc_set_atomic_min_float16_impl(); }

[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
__aicore__ inline void asc_set_atomic_min_int()
{
    asc_set_atomic_min_int_impl();
}

__aicore__ inline void asc_set_atomic_min_int8() { asc_set_atomic_min_int8_impl(); }

__aicore__ inline void asc_set_atomic_min_int16() { asc_set_atomic_min_int16_impl(); }

__aicore__ inline void asc_set_atomic_min_int32() { asc_set_atomic_min_int32_impl(); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_ATOMIC_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_ATOMIC_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_ATOMIC_INTF_IMPL_H
