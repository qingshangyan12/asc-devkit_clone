/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_REDUCE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_REDUCE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/vector_reduce_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_REDUCE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/common_impl.h"

__aicore__ inline void asc_get_reduce_max_cnt_impl(half& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            half h;
            uint16_t u;
        } u162half = {.u = static_cast<uint16_t>(0xffff & max_min_cnt)};
        val = u162half.h;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_get_reduce_max_cnt_impl(float& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            float f;
            uint32_t u;
        } u322float = {.u = static_cast<uint32_t>(0xffffffff & max_min_cnt)};
        val = u322float.f;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_get_reduce_min_cnt_impl(half& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            half h;
            uint16_t u;
        } u162half = {.u = static_cast<uint16_t>(0xffff & max_min_cnt)};
        val = u162half.h;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_get_reduce_min_cnt_impl(float& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            float f;
            uint32_t u;
        } u322float = {.u = static_cast<uint32_t>(0xffffffff & max_min_cnt)};
        val = u322float.f;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_datablock_reduce_sum_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_sum_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_sum_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_sum_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_max_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_max_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_max_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_max_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_min_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_min_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_min_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_min_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_min_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_min_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_sum_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, 0);
    }
}

__aicore__ inline void asc_repeat_reduce_sum_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_sum_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, 0);
    }
}

__aicore__ inline void asc_repeat_reduce_sum_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline int64_t asc_get_acc_val_impl()
{
    if ASC_IS_AIV {
        return get_acc_val();
    }
    return 0;
}

// asc_reduce uint16_t
__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vreducev2(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_reduce_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, 0,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_reduce uint32_t
__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vreducev2(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_reduce_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, 0,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline int64_t asc_get_rsvd_count_impl()
{
    if ASC_IS_AIV {
        return get_rsvd_cnt();
    }
    return 0;
}

__aicore__ inline void asc_pair_reduce_sum_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcpadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_pair_reduce_sum_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_pair_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_pair_reduce_sum_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcpadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_pair_reduce_sum_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_pair_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// ==========asc_datablock_reduce(half/float)==========
__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_max_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_max_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_min_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_min_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
}

// ==========asc_repeat_reduce(half/float)==========
__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
}

// ===========asc_get_acc_val=================
__aicore__ inline int64_t asc_get_acc_val() { return asc_get_acc_val_impl(); }

// ============asc_get_reduce_max_cnt==========
__aicore__ inline void asc_get_reduce_max_cnt(half& val, uint32_t& index) { asc_get_reduce_max_cnt_impl(val, index); }

__aicore__ inline void asc_get_reduce_max_cnt(float& val, uint32_t& index) { asc_get_reduce_max_cnt_impl(val, index); }

__aicore__ inline void asc_get_reduce_min_cnt(half& val, uint32_t& index) { asc_get_reduce_min_cnt_impl(val, index); }

__aicore__ inline void asc_get_reduce_min_cnt(float& val, uint32_t& index) { asc_get_reduce_min_cnt_impl(val, index); }

// ==========asc_reduce(uint16_t/uint32_t)==========
__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_reduce_impl(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_reduce_impl(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
}

// ==========asc_get_rsvd_count==========
__aicore__ inline int64_t asc_get_rsvd_count() { return asc_get_rsvd_count_impl(); }

//==========asc_pair_reduce_sum(half/float)============
__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_pair_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_pair_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_pair_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_pair_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_pair_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_pair_reduce_sum_impl(dst, src, count);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_REDUCE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_REDUCE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_REDUCE_INTF_IMPL_H
