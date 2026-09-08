/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_SCALAR_COMPUTE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_SCALAR_COMPUTE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/scalar_compute_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_SCALAR_COMPUTE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"

__aicore__ inline int64_t asc_popc_impl(uint64_t value) { return bcnt1(value); }

__aicore__ inline int64_t asc_clz_impl(uint64_t value_in) { return clz(value_in); }

__aicore__ inline int64_t asc_zero_bits_cnt_impl(uint64_t value) { return bcnt0(value); }

__aicore__ inline int64_t asc_ffs_impl(uint64_t value) { return sff1(value); }

constexpr int16_t ASC_C_API_DEFAULT_OFFSET = 0;

__aicore__ inline void asc_store_dev_impl(__gm__ int8_t* addr, int8_t value)
{
    st_dev(*(reinterpret_cast<uint8_t*>(&value)), reinterpret_cast<__gm__ uint8_t*>(addr), ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ uint8_t* addr, uint8_t value)
{
    st_dev(value, addr, ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ int16_t* addr, int16_t value)
{
    st_dev(*(reinterpret_cast<uint16_t*>(&value)), reinterpret_cast<__gm__ uint16_t*>(addr), ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ uint16_t* addr, uint16_t value)
{
    st_dev(value, addr, ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ int32_t* addr, int32_t value)
{
    st_dev(*(reinterpret_cast<uint32_t*>(&value)), reinterpret_cast<__gm__ uint32_t*>(addr), ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ uint32_t* addr, uint32_t value)
{
    st_dev(value, addr, ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ int64_t* addr, int64_t value)
{
    st_dev(*(reinterpret_cast<uint64_t*>(&value)), reinterpret_cast<__gm__ uint64_t*>(addr), ASC_C_API_DEFAULT_OFFSET);
}

__aicore__ inline void asc_store_dev_impl(__gm__ uint64_t* addr, uint64_t value)
{
    st_dev(value, addr, ASC_C_API_DEFAULT_OFFSET);
}

constexpr int16_t ASC_C_API_LOAD_DEV_DEFAULT_OFFSET = 0;

__aicore__ inline int8_t asc_load_dev_impl(__gm__ int8_t* addr)
{
    return ld_dev(reinterpret_cast<__gm__ uint8_t*>(addr), ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline uint8_t asc_load_dev_impl(__gm__ uint8_t* addr)
{
    return ld_dev(addr, ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline int16_t asc_load_dev_impl(__gm__ int16_t* addr)
{
    return ld_dev(reinterpret_cast<__gm__ uint16_t*>(addr), ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline uint16_t asc_load_dev_impl(__gm__ uint16_t* addr)
{
    return ld_dev(addr, ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline int32_t asc_load_dev_impl(__gm__ int32_t* addr)
{
    return ld_dev(reinterpret_cast<__gm__ uint32_t*>(addr), ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline uint32_t asc_load_dev_impl(__gm__ uint32_t* addr)
{
    return ld_dev(addr, ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline int64_t asc_load_dev_impl(__gm__ int64_t* addr)
{
    return ld_dev(addr, ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline uint64_t asc_load_dev_impl(__gm__ uint64_t* addr)
{
    return ld_dev(addr, ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
}

__aicore__ inline half asc_float2half_ro_impl(float value) { return conv_f322f16o(value); }

__aicore__ inline int32_t asc_float2int32_rd_impl(float value) { return conv_f322s32f(value); }

__aicore__ inline int32_t asc_float2int32_ru_impl(float value) { return conv_f322s32c(value); }

__aicore__ inline int32_t asc_float2int32_rn_impl(float value) { return conv_f322s32r(value); }

__aicore__ inline int32_t asc_float2int32_rna_impl(float value) { return conv_f322s32a(value); }

__aicore__ inline int64_t asc_sflbits_impl(int64_t value) { return sflbits(value); }

__aicore__ inline int64_t asc_ffz_impl(uint64_t value) { return sff0(value); }

__aicore__ inline uint64_t asc_clear_nthbit_impl(uint64_t bits, int64_t idx) { return sbitset0(bits, idx); }

__aicore__ inline uint64_t asc_set_nthbit_impl(uint64_t bits, int64_t idx) { return sbitset1(bits, idx); }

// ==========asc_clz==========
__aicore__ inline int64_t asc_clz(uint64_t value_in) { return asc_clz_impl(value_in); }

__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value) { return asc_zero_bits_cnt_impl(value); }

__aicore__ inline int64_t asc_ffs(uint64_t value) { return asc_ffs_impl(value); }

__aicore__ inline int64_t asc_popc(uint64_t value) { return asc_popc_impl(value); }

__aicore__ inline void asc_store_dev(__gm__ int8_t* addr, int8_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ uint8_t* addr, uint8_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ int16_t* addr, int16_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ uint16_t* addr, uint16_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ int32_t* addr, int32_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ uint32_t* addr, uint32_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ int64_t* addr, int64_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline void asc_store_dev(__gm__ uint64_t* addr, uint64_t value) { asc_store_dev_impl(addr, value); }

__aicore__ inline int8_t asc_load_dev(__gm__ int8_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline uint8_t asc_load_dev(__gm__ uint8_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline int16_t asc_load_dev(__gm__ int16_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline uint16_t asc_load_dev(__gm__ uint16_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline int32_t asc_load_dev(__gm__ int32_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline uint32_t asc_load_dev(__gm__ uint32_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline int64_t asc_load_dev(__gm__ int64_t* addr) { return asc_load_dev_impl(addr); }

__aicore__ inline uint64_t asc_load_dev(__gm__ uint64_t* addr) { return asc_load_dev_impl(addr); }

// ==========asc_float2half (ro)==========
__aicore__ inline half asc_float2half_ro(float value) { return asc_float2half_ro_impl(value); }

// ==========asc_float2int32 (rd/ru/rn/rna)==========
__aicore__ inline int32_t asc_float2int32_rd(float value) { return asc_float2int32_rd_impl(value); }

__aicore__ inline int32_t asc_float2int32_ru(float value) { return asc_float2int32_ru_impl(value); }

__aicore__ inline int32_t asc_float2int32_rn(float value) { return asc_float2int32_rn_impl(value); }

__aicore__ inline int32_t asc_float2int32_rna(float value) { return asc_float2int32_rna_impl(value); }

//=============asc_sflbits============
__aicore__ inline int64_t asc_sflbits(int64_t value) { return asc_sflbits_impl(value); }

//=============asc_ffz================
__aicore__ inline int64_t asc_ffz(uint64_t value) { return asc_ffz_impl(value); }

//=============asc_clear_nthbit================
__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx) { return asc_clear_nthbit_impl(bits, idx); }

//=============asc_set_nthbit================
__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx) { return asc_set_nthbit_impl(bits, idx); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_SCALAR_COMPUTE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_SCALAR_COMPUTE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_SCALAR_COMPUTE_INTF_IMPL_H
