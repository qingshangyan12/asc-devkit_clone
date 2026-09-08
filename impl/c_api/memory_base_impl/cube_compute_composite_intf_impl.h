/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_CUBE_COMPUTE_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_CUBE_COMPUTE_COMPOSITE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/cube_compute_composite_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_COMPUTE_COMPOSITE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_impl.h"
#include "impl/c_api/memory_base_impl/cube_compute_intf_impl.h"

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

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_COMPUTE_COMPOSITE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_COMPUTE_COMPOSITE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_CUBE_COMPUTE_COMPOSITE_INTF_IMPL_H
