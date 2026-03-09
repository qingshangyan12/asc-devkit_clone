/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_IMPL_H

#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2l1_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2gm_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0a_mx_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_rpt_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_fmatrix_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l0c_copy_prequant_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop_size_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop1_stride_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop2_stride_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_pad_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12fb_impl.h"
#include "instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12bt_impl.h"

// ==========asc_copy_l0c2l1===========
// half  float
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// float  float
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// half int32_t
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// int8_t int32_t
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// uint8_t int32_t
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

// int32_t int32_t
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre,
                                            uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre,
                                            bool channel_split, bool nz2nd_en, uint64_t quant_post, uint8_t relu_post,
                                            bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, clip_relu_pre, unit_flag_mode,
        quant_pre, relu_pre, channel_split, nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_cfg, c0_pad_en);
}

__aicore__ inline void asc_copy_l12l0a_mx(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos,
    uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config)
{
    asc_set_l0c_copy_prequant_impl(config);
}

__aicore__ inline void asc_set_gm2l1_loop_size(uint64_t loop1_size, uint64_t loop2_size)
{
    asc_set_gm2l1_loop_size_impl(loop1_size, loop2_size);
}

__aicore__ inline void asc_set_gm2l1_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    asc_set_gm2l1_loop1_stride_impl(loop1_src_stride, loop1_dst_stride);
}

__aicore__ inline void asc_set_gm2l1_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    asc_set_gm2l1_loop2_stride_impl(loop2_src_stride, loop2_dst_stride);
}

__aicore__ inline void asc_set_gm2l1_pad(uint32_t pad_val)
{
    asc_set_gm2l1_pad_impl(pad_val);
}

// ==========asc_set_l13d_rpt==========
__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config)
{
    asc_set_l13d_rpt_impl(config);
}

// ==========asc_set_l13d_fmatrix==========
__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config)
{
    asc_set_l13d_fmatrix_impl(config);
}

// =============asc_copy_l12fb===============
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t burst_num, uint16_t burst_len,
                                      uint16_t src_gap_size, uint16_t dst_gap_size)
{
    asc_copy_l12fb_impl(dst, src, burst_num, burst_len, src_gap_size, dst_gap_size);
}

__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, size);
}

__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_sync_impl(dst, src, size);
}

//=============asc_copy_l12bt===============
// void -> uint64_t
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst,
                                      uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

// bfloat16_t -> uint64_t
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t n_burst,
                                      uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

// half -> uint64_t
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t n_burst,
                                      uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

// float -> uint64_t
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t n_burst,
                                      uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

// int32_t -> uint64_t
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t n_burst,
                                      uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

//=============asc_copy_l0c2l1===============
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void *dst_addr, __cc__ float *src_addr, uint16_t n_size, 
            uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre, 
            uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post, 
            uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en, 
            bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en, bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2l1_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl, 
        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post, clip_relu_post, 
        loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en, wino_post_en, broadcast_en, NZ2DN_en);
}

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void *dst_addr, __cc__ int32_t *src_addr, uint16_t n_size,
            uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl, uint8_t clip_relu_pre,
            uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en, bool NZ2ND_en, uint64_t quant_post,
            uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en, uint8_t eltwise_op, bool eltwise_antq_en,
            bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en, bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2l1_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post, clip_relu_post,
        loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en, wino_post_en, broadcast_en, NZ2DN_en);
}

// ==========asc_copy_l0c2gm===========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// half  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ half *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// float8_e4m3_t  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ float8_e4m3_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// float8_e5m2_t  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ float8_e5m2_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ hifloat8_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ int8_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ uint8_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// float  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ float *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ half *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// float8_e4m3_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ float8_e4m3_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// float8_e5m2_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ float8_e5m2_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// hifloat8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ hifloat8_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ int8_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ uint8_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ int32_t *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// void  float
__aicore__ inline void asc_copy_l0c2gm(__gm__ void *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

// void  int32_t
__aicore__ inline void asc_copy_l0c2gm(__gm__ void *dst_addr, __cc__ int32_t *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctl, uint64_t quant_pre, uint8_t relu_pre, bool split_en,
                        bool NZ2ND_en, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, bool loop_enhance_en,
                        uint8_t eltwise_op, bool eltwise_antq_en, bool loop_enhance_merge_en, bool C0_pad_en, bool wino_post_en,
                        bool broadcast_en, bool NZ2DN_en)
{
    asc_copy_l0c2gm_impl(dst_addr, src_addr, n_size, m_size, loop_dst_stride, loop_src_stride, l2_cache_ctl,
                        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, quant_post, relu_post,
                        clip_relu_post, loop_enhance_en, eltwise_op, eltwise_antq_en, loop_enhance_merge_en, C0_pad_en,
                        wino_post_en, broadcast_en, NZ2DN_en);
}

#endif