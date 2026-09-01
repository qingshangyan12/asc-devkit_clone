/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)

#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2l1_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_l12l0a_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_l0c2gm_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_l0c2ub_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_l12l0a_mx_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_l12l0b_impl.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_l12l0b_mx_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_rpt_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_fmatrix_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_fmatrix_b_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l0c2gm_lrelu_alpha_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l0c_copy_lrelu_alpha_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l0c_copy_prequant_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop_size_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop1_stride_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop2_stride_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_pad_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_trans_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0a_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_align_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_dn2nz_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_nd2nz_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12fb_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12bt_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12ub_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_fill_l1_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/asc_set_l0c2gm_config_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/asc_set_l0c2gm_nz2nd_impl.h"

constexpr uint8_t RELU_POST_DEFAULT = 0;
constexpr bool CLIP_RELU_POST_DEFAULT = false;
constexpr uint8_t ELTWISE_OP_DEFAULT = 0;
constexpr uint64_t QUANT_POST_DEFAULT = 0;

// ==========asc_copy_l0c2gm===========
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

// ==========asc_copy_l0c2ub===========
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false, enable_nz2dn);
}

// ==========asc_copy_l12l0a==========
__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

// ==========asc_copy_l12l0a,
// int4b_t/uint8_t/int8_t/fp4x2_e2m1_t/fp4x2_e1m2_t/hif4x2_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/half/bfloat16_t/int16_t/uint16_t/uint32_t/int32_t/float==========
__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// transpose

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// ==========asc_copy_l12l0b==========
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

// ==========asc_copy_l12l0b,
// int4b_t/uint8_t/int8_t/fp4x2_e2m1_t/fp4x2_e1m2_t/hif4x2_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/half/bfloat16_t/int16_t/uint16_t/uint32_t/int32_t/float==========
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// transpose

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_mx(
    uint64_t dst, __cbuf__ hif4_scale* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_mx(
    uint64_t dst, __cbuf__ hif4_scale* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
