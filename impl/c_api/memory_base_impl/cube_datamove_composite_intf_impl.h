/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/cube_datamove_composite_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_impl.h"
#include "impl/c_api/memory_base_impl/cube_datamove_intf_impl.h"

__aicore__ inline void asc_copy_l12l0b_sparse_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
{
    asc_copy_l12l0b_sparse_impl(dst, src, index, start_index, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12bt_sync_impl(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_trans_sync_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_load_image_to_cbuf_sync_impl(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    asc_load_image_to_cbuf_impl(
        dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size, left_pad_size,
        right_pad_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_load_image_to_cbuf_sync_impl(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    asc_load_image_to_cbuf_impl(
        dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size, left_pad_size,
        right_pad_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(
    __ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(__ca__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_sync_impl(
    __ca__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(
    __cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0b_sync_impl(
    __cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12fb_sync_impl(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad1_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad1_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad2_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad2_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad3_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad3_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad4_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad4_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad5_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad5_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad6_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad6_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad7_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad7_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_pad8_sync_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad8_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0a_sync_impl(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12gm_sync_impl(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12gm_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0c_sync_impl(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l0b_sync_impl(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
    asc_sync_post_process();
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_sparse and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sparse_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
{
    asc_copy_l12l0b_sparse_sync_impl(dst, src, index, start_index, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0b_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0a_sync_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_load_image_to_cbuf and asc_sync)
__aicore__ inline void asc_load_image_to_cbuf_sync(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    asc_load_image_to_cbuf_sync_impl(
        dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size, left_pad_size,
        right_pad_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_load_image_to_cbuf and asc_sync)
__aicore__ inline void asc_load_image_to_cbuf_sync(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    asc_load_image_to_cbuf_sync_impl(
        dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size, left_pad_size,
        right_pad_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0a and asc_sync)
__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l0b and asc_sync)
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(
        dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad1_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad2 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad2_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad2_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad3 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad3_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad3_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad4 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad4_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad4_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad5 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad5_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad5_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad6 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad6_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad6_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad7 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad7_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad7_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_pad8 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_pad8_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad8_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12fb and asc_sync)
__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0a and asc_sync)
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12gm and asc_sync)
__aicore__ inline void asc_copy_l12gm_sync(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12gm_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0c and asc_sync)
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_sync_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l0b and asc_sync)
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_sync_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H
