/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_C_API_CUBE_COMPUTE_H
#define INCLUDE_C_API_CUBE_COMPUTE_H

#include "c_api_instr_impl/cube_compute_c_api_impl/cube_compute_c_api_impl.h"

__aicore__ inline int64_t asc_clz(uint64_t value_in);

// asc_mmad(float-bfloat16_t)
__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix,
    uint16_t left_height, uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix,
    uint16_t left_height, uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source,
    bool c_matrix_init_val);

__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix,
    uint16_t left_height, uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix,
    uint16_t left_height, uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

// asc_mmad(float-half)
__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

// asc_mmad(float-float)
__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset,
    bool c_matrix_source, bool c_matrix_init_val);

// asc_mmad(int32_t-int8_t)
__aicore__ inline void asc_mmad(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_sync(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, 
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

// asc_mmad_s4
__aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height,
    uint16_t right_width, uint16_t n_dim, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_set_mm_transform_m();

__aicore__ inline void asc_set_mm_transform_n();
#endif