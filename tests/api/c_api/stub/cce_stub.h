/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef __TESTS_C_API_STUB__
#define __TESTS_C_API_STUB__
#include <cstdint>
#include "stub_fun.h"

void vsts(vector_f8e4m3 data, __ubuf__ fp8_e4m3fn_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e5m2 data, __ubuf__ fp8_e5m2_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e8m0 data, __ubuf__ fp8_e8m0_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f4e2m1x2 data, __ubuf__ fp4x2_e2m1_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f4e1m2x2 data, __ubuf__ fp4x2_e1m2_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);

void vsts(vector_s8 src0, vector_s8 src1, __ubuf__ int8_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_u8 src0, vector_u8 src1, __ubuf__ uint8_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_s16 src0, vector_s16 src1, __ubuf__ int16_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_u16 src0, vector_u16 src1, __ubuf__ uint16_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_s32 src0, vector_s32 src1, __ubuf__ int32_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_u32 src0, vector_u32 src1, __ubuf__ uint32_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f16 src0, vector_f16 src1, __ubuf__ half* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_bf16 src0, vector_bf16 src1, __ubuf__ bfloat16_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e4m3 src0, vector_f8e4m3 src1, __ubuf__ fp8_e4m3fn_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e5m2 src0, vector_f8e5m2 src1, __ubuf__ fp8_e5m2_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e8m0 src0, vector_f8e8m0 src1, __ubuf__ fp8_e8m0_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f4e2m1x2 src0, vector_f4e2m1x2 src1, __ubuf__ fp4x2_e2m1_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f4e1m2x2 src0, vector_f4e1m2x2 src1, __ubuf__ fp4x2_e1m2_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);

using float8_e4m3_t = fp8_e4m3fn_t;
using float8_e5m2_t = fp8_e5m2_t;
using float4_e1m2x2_t = fp4x2_e1m2_t;
using float4_e2m1x2_t = fp4x2_e2m1_t;

// ==========copy_matrix_cc_to_cbuf_s4===========
inline void copy_matrix_cc_to_cbuf_s4(__cbuf__ void *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_cbuf_s4(__cbuf__ void *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

// ==========copy_matrix_cc_to_gm===========
inline void copy_matrix_cc_to_gm(__gm__ bfloat16_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ half *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ float8_e4m3_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ float8_e5m2_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ hifloat8_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ int8_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ uint8_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ float *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ bfloat16_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ half *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ float8_e4m3_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ float8_e5m2_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ hifloat8_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ int8_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ uint8_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm(__gm__ int32_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm_s4(__gm__ void *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_gm_s4(__gm__ void *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

// ==========copy_matrix_cc_to_ub===========
inline void copy_matrix_cc_to_ub(__ubuf__ bfloat16_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ half *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ float8_e4m3_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ float8_e5m2_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ hifloat8_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ int8_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ uint8_t *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ float *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ bfloat16_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ half *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ float8_e4m3_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ float8_e5m2_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ hifloat8_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ int8_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ uint8_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub(__ubuf__ int32_t *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub_s4(__ubuf__ void *dst_addr, __cc__ float *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

inline void copy_matrix_cc_to_ub_s4(__ubuf__ void *dst_addr, __cc__ int32_t *src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag_ctl,
    uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
    bool broadcast_en, bool NZ2DN_en) {}

// ==========load_gm_to_cbuf_2dv2===========
inline void load_gm_to_cbuf_2dv2(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ float *dst, __gm__ float *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ float8_e4m3_t *dst, __gm__ float8_e4m3_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ float8_e5m2_t *dst, __gm__ float8_e5m2_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ half *dst, __gm__ half *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ hifloat8_t *dst, __gm__ hifloat8_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

// ==========load_gm_to_cbuf_2dv2_s4===========
inline void load_gm_to_cbuf_2dv2_s4(__cbuf__ float4_e1m2x2_t *dst, __gm__ float4_e1m2x2_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2_s4(__cbuf__ float4_e2m1x2_t *dst, __gm__ float4_e2m1x2_t *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

inline void load_gm_to_cbuf_2dv2_s4(__cbuf__ void *dst, __gm__ void *src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    uint8_t l2_cache_ctl) {}

// ==========copy_cbuf_to_ubuf===========
inline void copy_cbuf_to_ubuf(__ubuf__ void *dst_addr, __cbuf__ void *src_addr, bool sub_blockid, 
    uint16_t burst_num, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap) {}


inline void create_cbuf_matrix_h(__cbuf__ bfloat16_t *dst, int64_t repeat, half value) {}

inline void create_cbuf_matrix_ui(__cbuf__ bfloat16_t *dst, int64_t repeat, uint32_t value) {}



#if defined(__DAV_CUBE__)
    inline int32_t g_coreType = 1;
#else
    inline int32_t g_coreType = 2;
#endif

typedef std::integral_constant<Pos, Pos::LOWEST> Lowest_Type;
typedef std::integral_constant<Pos, Pos::HIGHEST> Highest_Type;
constexpr Lowest_Type POS_LOWEST = Lowest_Type();
constexpr Highest_Type POS_HIGHEST = Highest_Type();

#endif
