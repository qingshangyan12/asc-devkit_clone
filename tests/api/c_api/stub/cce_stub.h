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
#include "c_api/defs/enum.h"

inline void vcvt(
    vector_f8e4m3& dst, vector_bf16 src, vector_bool preg, Literal tag0, Literal tag1, Literal tag2, Literal tag3)
{}
inline void vcvt(
    vector_f8e5m2& dst, vector_bf16 src, vector_bool preg, Literal tag0, Literal tag1, Literal tag2, Literal tag3)
{}
inline void vcvt_bf162e6m2(
    vector_f8e6m2& dst, vector_bf16 src, vector_bool preg, Literal tag0, Literal tag1, Literal tag2)
{}
inline void vcvt_bf162e8m0(
    vector_f8e8m0& dst, vector_bf16 src, vector_bool preg, Literal tag0, Literal tag1, Literal tag2, Literal tag3)
{}

using hif4x2_t = hifloat4x2_t;

using vector_hif4x2 = std::array<hifloat4x2_t, ConstantsInternal::b8EleSize>;

template <typename C, typename A, typename B>
inline void mad(
    __cc__ C* c_matrix, __ca__ A* a_matrix, __cb__ B* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, asc_unit_flag_mode unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    mad(c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, static_cast<uint8_t>(unit_flag), disable_gemv,
        c_matrix_source, c_matrix_init_val);
}

template <typename C, typename A, typename B>
inline void mad_mx(
    __cc__ C* c_matrix, __ca__ A* a_matrix, __cb__ B* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, asc_unit_flag_mode unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    mad_mx(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, static_cast<uint8_t>(unit_flag), disable_gemv,
        c_matrix_source, c_matrix_init_val);
}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp4x2_e1m2_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp4x2_e1m2_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp4x2_e1m2_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp4x2_e2m1_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp4x2_e2m1_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp4x2_e1m2_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp4x2_e2m1_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp4x2_e2m1_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp8_e4m3fn_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp8_e5m2_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp8_e5m2_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp8_e4m3fn_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

inline void mad_mx(
    __cc__ float* c_matrix, uint64_t bias, __ca__ fp8_e5m2_t* a_matrix, uint64_t scale_a_addr,
    __cb__ fp8_e5m2_t* b_matrix, uint64_t scale_b_addr, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{}

// The compiler supports this overload, but it is missing from the CPU debug stub header.
inline void vmadd(vector_bf16& dst, vector_bf16 src0, vector_bf16 src1, vector_bool mask, Literal mode) {}

#ifndef ULL
#define ULL unsigned long long
#endif

static bool is_mock_copy_matrix_cc_to_gm = false;
static uint16_t n_size_global = 0;
static uint16_t m_size_global = 0;
static uint32_t dst_stride_global = 0;
static uint16_t src_stride_global = 0;
static bool NZ2ND_en_global = false;
static bool NZ2DN_en_global = false;
static void* gm_addr_global = nullptr;
static void* l0c_addr_global = nullptr;
static bool is_mock_copy_matrix_cc_to_ub = false;
static void* ub_addr_global = nullptr;
static uint64_t quant_pre_global = 0;

#ifndef __biasbuf__
#define __biasbuf__
#endif

inline void sprsts(Literal, __ubuf__ uint32_t*, int32_t) {}

inline void sprsts(Literal, __ubuf__ uint32_t*&, int32_t, Literal) {}

void vsts(
    vector_f8e4m3 data, __ubuf__ fp8_e4m3fn_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e5m2 data, __ubuf__ fp8_e5m2_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(vector_f8e8m0 data, __ubuf__ fp8_e8m0_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(
    vector_f4e2m1x2 data, __ubuf__ fp4x2_e2m1_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);
void vsts(
    vector_f4e1m2x2 data, __ubuf__ fp4x2_e1m2_t* base, int32_t offset, Literal dist, vector_bool mask, Literal mode);

void vsts(
    vector_s8 src0, vector_s8 src1, __ubuf__ int8_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_u8 src0, vector_u8 src1, __ubuf__ uint8_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_s16 src0, vector_s16 src1, __ubuf__ int16_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_u16 src0, vector_u16 src1, __ubuf__ uint16_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_s32 src0, vector_s32 src1, __ubuf__ int32_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_u32 src0, vector_u32 src1, __ubuf__ uint32_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_f16 src0, vector_f16 src1, __ubuf__ half* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_bf16 src0, vector_bf16 src1, __ubuf__ bfloat16_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_f8e4m3 src0, vector_f8e4m3 src1, __ubuf__ fp8_e4m3fn_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_f8e5m2 src0, vector_f8e5m2 src1, __ubuf__ fp8_e5m2_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_f8e8m0 src0, vector_f8e8m0 src1, __ubuf__ fp8_e8m0_t* base, int32_t offset, Literal dist, vector_bool mask,
    Literal mode);
void vsts(
    vector_f4e2m1x2 src0, vector_f4e2m1x2 src1, __ubuf__ fp4x2_e2m1_t* base, int32_t offset, Literal dist,
    vector_bool mask, Literal mode);
void vsts(
    vector_f4e1m2x2 src0, vector_f4e1m2x2 src1, __ubuf__ fp4x2_e1m2_t* base, int32_t offset, Literal dist,
    vector_bool mask, Literal mode);

using float4_e2m1x2_t = fp4x2_e2m1_t;

// ==========copy_matrix_cc_to_cbuf_s4===========

// npu_arch_9201: no sid, bool quant_pre_rnd
inline void copy_matrix_cc_to_cbuf_s4(
    __cbuf__ void* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, bool quant_pre_rnd, uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre,
    uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool c0_pad_en,
    bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_cbuf_s4(
    __cbuf__ void* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, bool quant_pre_rnd, uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre,
    uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool c0_pad_en,
    bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

// ==========copy_matrix_cc_to_gm===========

inline void copy_matrix_cc_to_gm_s4(
    __gm__ void* dst_addr, __cc__ float* src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_gm_s4(
    __gm__ void* dst_addr, __cc__ int32_t* src_addr, uint8_t sid, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

// ==========copy_matrix_cc_to_ub===========

inline void copy_matrix_cc_to_ub(
    __ubuf__ bfloat16_t* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ half* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ float8_e4m3_t* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd,
    uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op,
    bool eltwise_antq_en, bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en,
    bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ float8_e5m2_t* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd,
    uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op,
    bool eltwise_antq_en, bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en,
    bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ hifloat8_t* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ int8_t* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ uint8_t* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ float* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ bfloat16_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ half* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ float8_e4m3_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd,
    uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op,
    bool eltwise_antq_en, bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en,
    bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ float8_e5m2_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size,
    uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd,
    uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op,
    bool eltwise_antq_en, bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en,
    bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ hifloat8_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ int8_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ uint8_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub(
    __ubuf__ int32_t* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub_s4(
    __ubuf__ void* dst_addr, __cc__ float* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

inline void copy_matrix_cc_to_ub_s4(
    __ubuf__ void* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride,
    uint16_t loop_src_stride, uint8_t dual_dst_ctl, bool sub_blockid, bool quant_pre_rnd, uint8_t clip_relu_pre,
    uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool nz2nd_en, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
    bool loop_enhance_merge_en, bool c0_pad_en, bool wino_post_en, bool broadcast_en, bool nz2dn_en)
{}

// ==========load_gm_to_cbuf_2dv2===========

// ==========load_gm_to_cbuf_2dv2_s4===========

inline void load_gm_to_cbuf_2dv2_s4(
    __cbuf__ float4_e2m1x2_t* dst, __gm__ float4_e2m1x2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{}
inline void load_gm_to_cbuf_2dv2_s4(
    __cbuf__ float4_e2m1x2_t* dst, __gm__ float4_e2m1x2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t sid, uint8_t decomp_mode,
    bool enable_pre_load_ignore_sync, uint8_t l2_cache_ctl)
{}

// ==========copy_cbuf_to_ubuf===========

inline void wait_intra_block(pipe_t pipe, uint8_t flag_id) {}
inline void set_intra_block(pipe_t pipe, uint8_t sync_id) {}
inline void wait_intra_block(pipe_t pipe, uint16_t flag_id) {}
inline void set_intra_block(pipe_t pipe, uint16_t sync_id) {}
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
inline void wait_intra_block(pipe_t pipe, int flag_id) {}
inline void set_intra_block(pipe_t pipe, int sync_id) {}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))

#endif

#if defined(__DAV_VEC__) && defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
#include "c_api/defs/type.h"
inline void pstu(vector_store_unalign& ureg, vector_bool mask, __ubuf__ uint16_t*& dst) {}
inline void pstu(vector_store_unalign& ureg, vector_bool mask, __ubuf__ uint32_t*& dst) {}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201))
// asc_copy_gm2ub_impl uses void*; stub_fun.h void* overload lacks l2_cache_ctl param.
inline void copy_gm_to_ubuf_align_v2(
    __ubuf__ void* dst_addr, __gm__ void* src_addr, uint8_t sid, uint32_t burst_num, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool constant_padding_ctl, uint8_t l2_cache_ctl,
    uint64_t burst_src_stride, uint32_t burst_dst_stride)
{}
// asc_copy_ub2gm_impl uses void*; stub_fun.h void* overload lacks l2_cache_mode param.

#endif

// ==========asc_prefetch_l2cache stubs (match npu_arch_9201 impl arity)==========

// ==========vstar===========

// ==========vstur===========

inline void load_cbuf_to_cb_transpose_s4(
    float4_e2m1x2_t* dst, __cbuf__ float4_e2m1x2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride, uint16_t src_frac_stride)
{}

// ==========scatter_ubuf_to_gm===========
inline void scatter_ubuf_to_gm(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* dataSrc, uint8_t sid, uint8_t offset_type, __ubuf__ void* offsetSrc,
    uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{}

inline void scatter_ubuf_to_gm(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* dataSrc, uint8_t sid, uint8_t offset_type, __ubuf__ void* offsetSrc,
    uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{}

inline void scatter_ubuf_to_gm(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* dataSrc, uint8_t sid, uint8_t offset_type, __ubuf__ void* offsetSrc,
    uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{}

inline void scatter_ubuf_to_gm(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* dataSrc, uint8_t sid, uint8_t offset_type, __ubuf__ void* offsetSrc,
    uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{}

// ==========gather_gm_to_ubuf===========
inline void gather_gm_to_ubuf(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* baseSrc, uint8_t sid, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offsetSrc, uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t dst_stride, uint32_t offset_num)
{}

inline void gather_gm_to_ubuf(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* baseSrc, uint8_t sid, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offsetSrc, uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t dst_stride, uint32_t offset_num)
{}

inline void gather_gm_to_ubuf(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* baseSrc, uint8_t sid, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offsetSrc, uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t dst_stride, uint32_t offset_num)
{}

inline void gather_gm_to_ubuf(
    __ubuf__ uint64_t* dst, __gm__ uint64_t* baseSrc, uint8_t sid, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offsetSrc, uint8_t l2_cache_ctl, uint32_t burst_len, uint32_t dst_stride, uint32_t offset_num)
{}

#if defined(__DAV_CUBE__)
inline int32_t g_coreType = 1;
#else
inline int32_t g_coreType = 2;
#endif

// ==========9201 v2 instruction stubs==========
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9201)

inline void vshls(vector_u8& dst, vector_u8 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshls(vector_s8& dst, vector_s8 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshls(vector_u16& dst, vector_u16 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshls(vector_s16& dst, vector_s16 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshls(vector_u32& dst, vector_u32 src, uint32_t shift, vector_bool mask, Literal mode) {}
inline void vshls(vector_s32& dst, vector_s32 src, uint32_t shift, vector_bool mask, Literal mode) {}
inline void vshrs(vector_u8& dst, vector_u8 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshrs(vector_s8& dst, vector_s8 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshrs(vector_u16& dst, vector_u16 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshrs(vector_s16& dst, vector_s16 src, uint16_t shift, vector_bool mask, Literal mode) {}
inline void vshrs(vector_u32& dst, vector_u32 src, uint32_t shift, vector_bool mask, Literal mode) {}
inline void vshrs(vector_s32& dst, vector_s32 src, uint32_t shift, vector_bool mask, Literal mode) {}

inline void vcpaddv2(vector_f16& dst, vector_f16 src, vector_bool mask) {}
inline void vcpaddv2(vector_f32& dst, vector_f32 src, vector_bool mask) {}

inline void vintlvv2(vector_f8e6m2& dst, vector_f8e6m2 src0, vector_f8e6m2 src1, Literal pos) {}

inline void vdintlvv2(vector_f8e6m2& dst, vector_f8e6m2 src0, vector_f8e6m2 src1, Literal pos) {}

#endif

#endif
