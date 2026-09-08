/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_COMPOSITE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/composite_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_COMPOSITE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/cube_compute_intf_impl.h"
#include "impl/c_api/memory_base_impl/cube_datamove_intf_impl.h"
#include "impl/c_api/memory_base_impl/utils_intf_impl.h"
#include "impl/c_api/memory_base_impl/vector_compute_intf_impl.h"
#include "impl/c_api/memory_base_impl/vector_datamove_intf_impl.h"

__aicore__ inline void asc_mmad_sparse_sync_impl(
    __cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sparse_impl(c, a, b, m, k, n, unit_flag, c_matrix_source, c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_s4_sync_impl(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_s4_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_s4_sync_impl(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_s4_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
    asc_sync_post_process();
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_sparse and asc_sync)
__aicore__ inline void asc_mmad_sparse_sync(
    __cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sparse_sync_impl(c, a, b, m, k, n, unit_flag, c_matrix_source, c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_s4 and asc_sync)
__aicore__ inline void asc_mmad_s4_sync(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_s4_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, k_direction_align, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_s4 and asc_sync)
__aicore__ inline void asc_mmad_s4_sync(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_s4_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, feat_offset, smask_offset, unit_flag,
        k_direction_align, is_weight_offset, c_matrix_source, c_matrix_init_val);
}

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

__aicore__ inline void asc_max_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_div_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_div_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_div_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_div_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_datablock_reduce_sum_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_datablock_reduce_sum_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_datablock_reduce_max_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_datablock_reduce_max_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_datablock_reduce_min_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_datablock_reduce_min_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_sum_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_sum_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_index_value_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_value_index_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_only_value_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_only_index_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_index_value_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_value_index_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_only_value_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_max_only_index_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_index_value_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_value_index_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_only_value_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_only_index_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_index_value_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_value_index_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_only_value_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_repeat_reduce_min_only_index_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_scalar_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_scalar_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_scalar_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_max_scalar_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_scalar_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_scalar_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_scalar_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_scalar_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_scalar_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_scalar_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_scalar_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_scalar_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_scalar_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_scalar_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_scalar_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_scalar_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_scalar_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_scalar_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_scalar_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_min_scalar_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_leakyrelu_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_leakyrelu_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_leakyrelu_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_leakyrelu_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_exp_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_exp_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_abs_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_abs_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_rcp_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_rcp_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sqrt_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sqrt_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_rsqrt_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_rsqrt_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_relu_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_relu_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_relu_sync_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_not_sync_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_not_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_and_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_and_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_or_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_or_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_or_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_or_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftleft_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftleft_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftleft_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftleft_sync_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftright_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftright_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftright_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftright_sync_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftright_round_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_shiftright_round_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_lt_sync_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_lt_sync_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_lt_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_lt_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_gt_sync_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_gt_sync_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_gt_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_gt_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_sync_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_sync_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_sync_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_sync_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ne_sync_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ne_sync_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ne_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ne_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_le_sync_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_le_sync_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_le_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_le_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_le_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_le_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_lt_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_lt_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_lt_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_lt_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_eq_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ne_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ne_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_le_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_le_scalar_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_select_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_select_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_reduce_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_reduce_sync_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_relu_sync_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_relu_sync_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_relu_sync_impl(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_relu_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_relu_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_add_relu_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_axpy_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_axpy_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_axpy_sync_impl(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_fma_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_fma_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_fma_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_fma_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_fma_sync_impl(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_fma_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_datablock_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_impl(dst, src, src_offset, dst_repeat_stride, dst_block_stride, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_datablock_sync_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_impl(dst, src, src_offset, dst_repeat_stride, dst_block_stride, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_gather_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mrgsort4_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
    asc_sync_post_process();
}

__aicore__ inline void asc_mrgsort4_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_add_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_add_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_add_relu_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_add_relu_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_cast_half2int8_sync_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2int8_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_mul_cast_half2uint8_sync_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2uint8_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_relu_sync_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_relu_sync_impl(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_relu_sync_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_relu_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_relu_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_sub_relu_sync_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int8_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int8_rna_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int8_ru_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int8_rd_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int8_rn_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int8_rz_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int642float_rd_sync_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int642float_rn_sync_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int642float_rna_sync_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int642float_ru_sync_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int642float_rz_sync_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_bfloat162float_sync_impl(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162float_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_bfloat162int32_rna_sync_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_bfloat162int32_ru_sync_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_bfloat162int32_rd_sync_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_bfloat162int32_rn_sync_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_bfloat162int32_rz_sync_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2float_sync_impl(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int16_rna_sync_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int16_ru_sync_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int16_rd_sync_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int16_rn_sync_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int16_rz_sync_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int32_rna_sync_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int32_ru_sync_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int32_rd_sync_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int32_rn_sync_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int32_rz_sync_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int4_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int4_rna_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int4_ru_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int4_rd_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int4_rn_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2int4_rz_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2uint8_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2uint8_rd_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2uint8_rn_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2uint8_rna_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2uint8_ru_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_half2uint8_rz_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_rn_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_rna_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_rd_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_ru_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_rz_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2half_ro_sync_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ro_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2float_rn_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2float_rd_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2float_ru_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2float_rna_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2float_rz_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int16_rd_sync_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int16_rn_sync_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int16_rna_sync_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int16_ru_sync_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int16_rz_sync_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int32_rd_sync_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rd_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int32_rn_sync_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rn_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int32_rna_sync_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rna_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int32_ru_sync_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_ru_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int32_rz_sync_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rz_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int64_rd_sync_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rd_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int64_rn_sync_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rn_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int64_rna_sync_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rna_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int64_ru_sync_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_ru_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2int64_rz_sync_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rz_count_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2bfloat16_rn_sync_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2bfloat16_rna_sync_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2bfloat16_rd_sync_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2bfloat16_ru_sync_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_float2bfloat16_rz_sync_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_deq_int322half_sync_impl(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_deq_int162b8_h_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_deq_int162b8_h_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_deq_int162b8_l_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_deq_int162b8_l_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int42half_sync_impl(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count)
{
    asc_int42half_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int82half_sync_impl(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_uint82half_sync_impl(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162half_sync_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162half_rd_sync_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162half_rn_sync_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162half_rna_sync_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162half_ru_sync_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162half_rz_sync_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int162float_sync_impl(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322float_sync_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322float_rd_sync_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rd_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322float_rn_sync_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rn_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322float_rna_sync_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rna_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322float_ru_sync_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_ru_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322float_rz_sync_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rz_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322int16_sync_impl(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int322int64_sync_impl(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_int642int32_sync_impl(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8h_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8h_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8l_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8l_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_transpose_sync_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_impl(dst, src);
    asc_sync_post_process();
}

__aicore__ inline void asc_transpose_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_impl(dst, src);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_brcb_sync_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_brcb_sync_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_transto5hd_b8_sync_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    asc_transto5hd_b8_impl(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
    asc_sync_post_process();
}

__aicore__ inline void asc_transto5hd_b16_sync_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b16_impl(dst, src, repeat, dst_stride, src_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_transto5hd_b32_sync_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b32_impl(dst, src, repeat, dst_stride, src_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_pair_reduce_sum_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_pair_reduce_sum_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_pair_reduce_sum_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_pair_reduce_sum_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_sync_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_sync_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_sync_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
    asc_sync_post_process();
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add and asc_sync)
__aicore__ inline void asc_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add and asc_sync)
__aicore__ inline void asc_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add and asc_sync)
__aicore__ inline void asc_add_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add and asc_sync)
__aicore__ inline void asc_add_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub and asc_sync)
__aicore__ inline void asc_sub_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub and asc_sync)
__aicore__ inline void asc_sub_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub and asc_sync)
__aicore__ inline void asc_sub_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub and asc_sync)
__aicore__ inline void asc_sub_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul and asc_sync)
__aicore__ inline void asc_mul_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul and asc_sync)
__aicore__ inline void asc_mul_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul and asc_sync)
__aicore__ inline void asc_mul_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul and asc_sync)
__aicore__ inline void asc_mul_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_div and asc_sync)
__aicore__ inline void asc_div_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_div_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_div and asc_sync)
__aicore__ inline void asc_div_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_div_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max and asc_sync)
__aicore__ inline void asc_max_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max and asc_sync)
__aicore__ inline void asc_max_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max and asc_sync)
__aicore__ inline void asc_max_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max and asc_sync)
__aicore__ inline void asc_max_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min and asc_sync)
__aicore__ inline void asc_min_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min and asc_sync)
__aicore__ inline void asc_min_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min and asc_sync)
__aicore__ inline void asc_min_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min and asc_sync)
__aicore__ inline void asc_min_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_datablock_reduce_sum and asc_sync)
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_sum_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_datablock_reduce_sum and asc_sync)
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_sum_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_datablock_reduce_max and asc_sync)
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_datablock_reduce_max and asc_sync)
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_datablock_reduce_min and asc_sync)
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_min_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_datablock_reduce_min and asc_sync)
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_min_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_sum and asc_sync)
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_sum_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_sum and asc_sync)
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_sum_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_index_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_index_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_value_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_value_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_only_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_only_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_only_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_max_only_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_index_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_index_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_value_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_value_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_only_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_only_value and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_only_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_repeat_reduce_min_only_index and asc_sync)
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max_scalar and asc_sync)
__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max_scalar and asc_sync)
__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max_scalar and asc_sync)
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_max_scalar and asc_sync)
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_scalar and asc_sync)
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_scalar and asc_sync)
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_scalar and asc_sync)
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_scalar and asc_sync)
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_scalar and asc_sync)
__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_scalar and asc_sync)
__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_scalar and asc_sync)
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_scalar and asc_sync)
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_scalar and asc_sync)
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_scalar and asc_sync)
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_scalar and asc_sync)
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_scalar and asc_sync)
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_leakyrelu and asc_sync)
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_leakyrelu_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_leakyrelu and asc_sync)
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_leakyrelu_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_exp and asc_sync)
__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_exp and asc_sync)
__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_abs and asc_sync)
__aicore__ inline void asc_abs_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_abs and asc_sync)
__aicore__ inline void asc_abs_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_rcp and asc_sync)
__aicore__ inline void asc_rcp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_rcp and asc_sync)
__aicore__ inline void asc_rcp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_rsqrt and asc_sync)
__aicore__ inline void asc_rsqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rsqrt_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_rsqrt and asc_sync)
__aicore__ inline void asc_rsqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rsqrt_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sqrt and asc_sync)
__aicore__ inline void asc_sqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sqrt and asc_sync)
__aicore__ inline void asc_sqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_relu and asc_sync)
__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_relu and asc_sync)
__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_relu and asc_sync)
__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_log and asc_sync)
__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_sync_impl_half(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_log and asc_sync)
__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_sync_impl_float(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_not and asc_sync)
__aicore__ inline void asc_not_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_not and asc_sync)
__aicore__ inline void asc_not_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_and and asc_sync)
__aicore__ inline void asc_and_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_and_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_and and asc_sync)
__aicore__ inline void asc_and_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_and_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_or and asc_sync)
__aicore__ inline void asc_or_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_or_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_or and asc_sync)
__aicore__ inline void asc_or_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_or_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftleft and asc_sync)
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftleft and asc_sync)
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftleft and asc_sync)
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftleft and asc_sync)
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_sync_impl(dst, src, distance, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftright and asc_sync)
__aicore__ inline void asc_shiftright_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftright and asc_sync)
__aicore__ inline void asc_shiftright_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftright and asc_sync)
__aicore__ inline void asc_shiftright_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftright and asc_sync)
__aicore__ inline void asc_shiftright_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftright_round and asc_sync)
__aicore__ inline void asc_shiftright_round_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_shiftright_round and asc_sync)
__aicore__ inline void asc_shiftright_round_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min_scalar and asc_sync)
__aicore__ inline void asc_min_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min_scalar and asc_sync)
__aicore__ inline void asc_min_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min_scalar and asc_sync)
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_min_scalar and asc_sync)
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_lt and asc_sync)
__aicore__ inline void asc_lt_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_lt and asc_sync)
__aicore__ inline void asc_lt_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_lt and asc_sync)
__aicore__ inline void asc_lt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_lt and asc_sync)
__aicore__ inline void asc_lt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gt and asc_sync)
__aicore__ inline void asc_gt_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gt and asc_sync)
__aicore__ inline void asc_gt_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gt and asc_sync)
__aicore__ inline void asc_gt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gt and asc_sync)
__aicore__ inline void asc_gt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ge and asc_sync)
__aicore__ inline void asc_ge_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ge_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ge and asc_sync)
__aicore__ inline void asc_ge_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ge_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ge and asc_sync)
__aicore__ inline void asc_ge_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ge_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ge and asc_sync)
__aicore__ inline void asc_ge_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ge_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq and asc_sync)
__aicore__ inline void asc_eq_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq and asc_sync)
__aicore__ inline void asc_eq_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq and asc_sync)
__aicore__ inline void asc_eq_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq and asc_sync)
__aicore__ inline void asc_eq_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq and asc_sync)
__aicore__ inline void asc_eq_sync(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ne and asc_sync)
__aicore__ inline void asc_ne_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ne and asc_sync)
__aicore__ inline void asc_ne_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ne and asc_sync)
__aicore__ inline void asc_ne_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ne and asc_sync)
__aicore__ inline void asc_ne_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_le and asc_sync)
__aicore__ inline void asc_le_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_le and asc_sync)
__aicore__ inline void asc_le_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_le and asc_sync)
__aicore__ inline void asc_le_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_le and asc_sync)
__aicore__ inline void asc_le_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_lt_scalar and asc_sync)
__aicore__ inline void asc_lt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_lt_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_lt_scalar and asc_sync)
__aicore__ inline void asc_lt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_lt_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gt_scalar and asc_sync)
__aicore__ inline void asc_gt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_gt_scalar_sync_impl_half(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gt_scalar and asc_sync)
__aicore__ inline void asc_gt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_gt_scalar_sync_impl_float(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ge_scalar and asc_sync)
__aicore__ inline void asc_ge_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ge_scalar and asc_sync)
__aicore__ inline void asc_ge_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq_scalar and asc_sync)
__aicore__ inline void asc_eq_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq_scalar and asc_sync)
__aicore__ inline void asc_eq_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_eq_scalar and asc_sync)
__aicore__ inline void asc_eq_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ne_scalar and asc_sync)
__aicore__ inline void asc_ne_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_ne_scalar and asc_sync)
__aicore__ inline void asc_ne_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_le_scalar and asc_sync)
__aicore__ inline void asc_le_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_le_scalar and asc_sync)
__aicore__ inline void asc_le_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_sync_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_select and asc_sync)
__aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_select and asc_sync)
__aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_reduce and asc_sync)
__aicore__ inline void asc_reduce_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_reduce_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_reduce and asc_sync)
__aicore__ inline void asc_reduce_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    asc_reduce_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_relu and asc_sync)
__aicore__ inline void asc_add_relu_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_relu and asc_sync)
__aicore__ inline void asc_add_relu_sync(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_relu and asc_sync)
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_relu and asc_sync)
__aicore__ inline void asc_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_relu and asc_sync)
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_add_relu and asc_sync)
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_axpy and asc_sync)
__aicore__ inline void asc_axpy_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_axpy and asc_sync)
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_axpy_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_axpy and asc_sync)
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_sync_impl(dst, src, value, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bitsort and asc_sync)
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_sync_impl_half(dst, src0, src1, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bitsort and asc_sync)
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_sync_impl_float(dst, src0, src1, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fma and asc_sync)
__aicore__ inline void asc_fma_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_fma_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fma and asc_sync)
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_fma_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fma and asc_sync)
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_fma_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather_datablock and asc_sync)
__aicore__ inline void asc_gather_datablock_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_sync_impl(dst, src, src_offset, dst_repeat_stride, dst_block_stride, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather_datablock and asc_sync)
__aicore__ inline void asc_gather_datablock_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_sync_impl(dst, src, src_offset, dst_repeat_stride, dst_block_stride, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_gather and asc_sync)
__aicore__ inline void asc_gather_sync(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_sync_impl(dst, src, src_offset, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mrgsort4 and asc_sync)
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_sync_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mrgsort4 and asc_sync)
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_sync_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_add and asc_sync)
__aicore__ inline void asc_mul_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_add and asc_sync)
__aicore__ inline void asc_mul_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_add_relu and asc_sync)
__aicore__ inline void asc_mul_add_relu_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_add_relu and asc_sync)
__aicore__ inline void asc_mul_add_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_cast_half2int8 and asc_sync)
__aicore__ inline void asc_mul_cast_half2int8_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2int8_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mul_cast_half2uint8 and asc_sync)
__aicore__ inline void asc_mul_cast_half2uint8_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2uint8_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_relu and asc_sync)
__aicore__ inline void asc_sub_relu_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_relu and asc_sync)
__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_relu and asc_sync)
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_relu and asc_sync)
__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_relu and asc_sync)
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_sub_relu and asc_sync)
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_sync_impl(dst, src0, src1, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bfloat162float and asc_sync)
__aicore__ inline void asc_bfloat162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162float_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bfloat162int32_rna and asc_sync)
__aicore__ inline void asc_bfloat162int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bfloat162int32_ru and asc_sync)
__aicore__ inline void asc_bfloat162int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bfloat162int32_rd and asc_sync)
__aicore__ inline void asc_bfloat162int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bfloat162int32_rn and asc_sync)
__aicore__ inline void asc_bfloat162int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bfloat162int32_rz and asc_sync)
__aicore__ inline void asc_bfloat162int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2float and asc_sync)
__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int16_rna and asc_sync)
__aicore__ inline void asc_half2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int16_ru and asc_sync)
__aicore__ inline void asc_half2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int16_rd and asc_sync)
__aicore__ inline void asc_half2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int16_rn and asc_sync)
__aicore__ inline void asc_half2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int16_rz and asc_sync)
__aicore__ inline void asc_half2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int32_rna and asc_sync)
__aicore__ inline void asc_half2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int32_ru and asc_sync)
__aicore__ inline void asc_half2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int32_rd and asc_sync)
__aicore__ inline void asc_half2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int32_rn and asc_sync)
__aicore__ inline void asc_half2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int32_rz and asc_sync)
__aicore__ inline void asc_half2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int4 and asc_sync)
__aicore__ inline void asc_half2int4_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int4_rna and asc_sync)
__aicore__ inline void asc_half2int4_rna_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int4_ru and asc_sync)
__aicore__ inline void asc_half2int4_ru_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int4_rd and asc_sync)
__aicore__ inline void asc_half2int4_rd_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int4_rn and asc_sync)
__aicore__ inline void asc_half2int4_rn_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int4_rz and asc_sync)
__aicore__ inline void asc_half2int4_rz_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int8 and asc_sync)
__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int8_rna and asc_sync)
__aicore__ inline void asc_half2int8_rna_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int8_ru and asc_sync)
__aicore__ inline void asc_half2int8_ru_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int8_rd and asc_sync)
__aicore__ inline void asc_half2int8_rd_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int8_rn and asc_sync)
__aicore__ inline void asc_half2int8_rn_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2int8_rz and asc_sync)
__aicore__ inline void asc_half2int8_rz_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2uint8 and asc_sync)
__aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2uint8_rna and asc_sync)
__aicore__ inline void asc_half2uint8_rna_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2uint8_ru and asc_sync)
__aicore__ inline void asc_half2uint8_ru_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2uint8_rd and asc_sync)
__aicore__ inline void asc_half2uint8_rd_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2uint8_rn and asc_sync)
__aicore__ inline void asc_half2uint8_rn_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_half2uint8_rz and asc_sync)
__aicore__ inline void asc_half2uint8_rz_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half and asc_sync)
__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half_rn and asc_sync)
__aicore__ inline void asc_float2half_rn_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half_rna and asc_sync)
__aicore__ inline void asc_float2half_rna_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half_rd and asc_sync)
__aicore__ inline void asc_float2half_rd_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half_ru and asc_sync)
__aicore__ inline void asc_float2half_ru_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half_rz and asc_sync)
__aicore__ inline void asc_float2half_rz_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2half_ro and asc_sync)
__aicore__ inline void asc_float2half_ro_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ro_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2float_rn and asc_sync)
__aicore__ inline void asc_float2float_rn_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2float_rd and asc_sync)
__aicore__ inline void asc_float2float_rd_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2float_ru and asc_sync)
__aicore__ inline void asc_float2float_ru_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2float_rna and asc_sync)
__aicore__ inline void asc_float2float_rna_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2float_rz and asc_sync)
__aicore__ inline void asc_float2float_rz_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int16_rna and asc_sync)
__aicore__ inline void asc_float2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int16_ru and asc_sync)
__aicore__ inline void asc_float2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int16_rd and asc_sync)
__aicore__ inline void asc_float2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int16_rn and asc_sync)
__aicore__ inline void asc_float2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int16_rz and asc_sync)
__aicore__ inline void asc_float2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int32_rna and asc_sync)
__aicore__ inline void asc_float2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int32_ru and asc_sync)
__aicore__ inline void asc_float2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int32_rd and asc_sync)
__aicore__ inline void asc_float2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int32_rn and asc_sync)
__aicore__ inline void asc_float2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int32_rz and asc_sync)
__aicore__ inline void asc_float2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int64_rna and asc_sync)
__aicore__ inline void asc_float2int64_rna_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int64_ru and asc_sync)
__aicore__ inline void asc_float2int64_ru_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int64_rd and asc_sync)
__aicore__ inline void asc_float2int64_rd_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int64_rn and asc_sync)
__aicore__ inline void asc_float2int64_rn_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2int64_rz and asc_sync)
__aicore__ inline void asc_float2int64_rz_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2bfloat16_rn and asc_sync)
__aicore__ inline void asc_float2bfloat16_rn_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2bfloat16_rna and asc_sync)
__aicore__ inline void asc_float2bfloat16_rna_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2bfloat16_rd and asc_sync)
__aicore__ inline void asc_float2bfloat16_rd_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2bfloat16_ru and asc_sync)
__aicore__ inline void asc_float2bfloat16_ru_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_float2bfloat16_rz and asc_sync)
__aicore__ inline void asc_float2bfloat16_rz_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_deq_int322half and asc_sync)
__aicore__ inline void asc_deq_int322half_sync(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_deq_int162b8_h and asc_sync)
__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_deq_int162b8_h and asc_sync)
__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_deq_int162b8_l and asc_sync)
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_deq_int162b8_l and asc_sync)
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int42half and asc_sync)
__aicore__ inline void asc_int42half_sync(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count)
{
    asc_int42half_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int82half and asc_sync)
__aicore__ inline void asc_int82half_sync(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_uint82half and asc_sync)
__aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162half and asc_sync)
__aicore__ inline void asc_int162half_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162half_rna and asc_sync)
__aicore__ inline void asc_int162half_rna_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162half_ru and asc_sync)
__aicore__ inline void asc_int162half_ru_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162half_rd and asc_sync)
__aicore__ inline void asc_int162half_rd_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162half_rn and asc_sync)
__aicore__ inline void asc_int162half_rn_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162half_rz and asc_sync)
__aicore__ inline void asc_int162half_rz_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int162float and asc_sync)
__aicore__ inline void asc_int162float_sync(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322float and asc_sync)
__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322float_rna and asc_sync)
__aicore__ inline void asc_int322float_rna_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322float_ru and asc_sync)
__aicore__ inline void asc_int322float_ru_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322float_rd and asc_sync)
__aicore__ inline void asc_int322float_rd_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322float_rn and asc_sync)
__aicore__ inline void asc_int322float_rn_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322float_rz and asc_sync)
__aicore__ inline void asc_int322float_rz_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322int16 and asc_sync)
__aicore__ inline void asc_int322int16_sync(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int322int64 and asc_sync)
__aicore__ inline void asc_int322int64_sync(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int642int32 and asc_sync)
__aicore__ inline void asc_int642int32_sync(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int642float_rna and asc_sync)
__aicore__ inline void asc_int642float_rna_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rna_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int642float_ru and asc_sync)
__aicore__ inline void asc_int642float_ru_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_ru_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int642float_rd and asc_sync)
__aicore__ inline void asc_int642float_rd_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rd_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int642float_rn and asc_sync)
__aicore__ inline void asc_int642float_rn_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rn_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_int642float_rz and asc_sync)
__aicore__ inline void asc_int642float_rz_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rz_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_vdeq_int162b8_h and asc_sync)
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_vdeq_int162b8_h and asc_sync)
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_vdeq_int162b8_l and asc_sync)
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_vdeq_int162b8_l and asc_sync)
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_transpose and asc_sync)
__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_transpose and asc_sync)
__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

[[deprecated("NOTICE: asc_transto5hd_b8_sync is deprecated. "
             "Please use asc_transto5hd_b8 and asc_sync instead.")]]
__aicore__ inline void asc_transto5hd_b8_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    asc_transto5hd_b8_sync_impl(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
}

[[deprecated("NOTICE: asc_transto5hd_b16_sync is deprecated. "
             "Please use asc_transto5hd_b16 and asc_sync instead.")]]
__aicore__ inline void asc_transto5hd_b16_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b16_sync_impl(dst, src, repeat, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_transto5hd_b32_sync is deprecated. "
             "Please use asc_transto5hd_b32 and asc_sync instead.")]]
__aicore__ inline void asc_transto5hd_b32_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b32_sync_impl(dst, src, repeat, dst_stride, src_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_duplicate and asc_sync)
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_brcb and asc_sync)
__aicore__ inline void asc_brcb_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_brcb_sync_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_brcb and asc_sync)
__aicore__ inline void asc_brcb_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_brcb_sync_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_pair_reduce_sum and asc_sync)
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_pair_reduce_sum_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_pair_reduce_sum and asc_sync)
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_pair_reduce_sum_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy and asc_sync)
__aicore__ inline void asc_copy_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_copy_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy and asc_sync)
__aicore__ inline void asc_copy_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_copy_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy and asc_sync)
__aicore__ inline void asc_copy_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_copy_sync_impl(dst, src, count);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy and asc_sync)
__aicore__ inline void asc_copy_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count)
{
    asc_copy_sync_impl(dst, src, count);
}

__aicore__ inline void asc_copy_ub2ub_sync_impl(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_sync_impl(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_sync_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ double* dst, __ubuf__ double* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2ub and asc_sync)
__aicore__ inline void asc_copy_ub2ub_sync(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub and asc_sync)
__aicore__ inline void asc_copy_gm2ub_sync(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm and asc_sync)
__aicore__ inline void asc_copy_ub2gm_sync(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ double* dst, __ubuf__ double* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_COMPOSITE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_COMPOSITE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_COMPOSITE_INTF_IMPL_H
