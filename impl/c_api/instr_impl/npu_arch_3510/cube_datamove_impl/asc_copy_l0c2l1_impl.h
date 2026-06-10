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
#warning "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2l1_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_IMPL_ASC_COPY_L0C2L1_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_IMPL_ASC_COPY_L0C2L1_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl/utils_impl.h"

// half  float
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}


// int8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}


// uint8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}

// half int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}

// int8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}

// uint8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}

// int32_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size,
                                            uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
                                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
                                            bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
                                            bool enable_clip_relu_post_mode, uint8_t eltwise_op, uint8_t eltwise_antq_cfg,
                                            bool enable_c0_pad)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(dst, src, 0, n_size, m_size, dst_stride, src_stride, clip_relu_pre_mode, unit_flag,
        static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
        relu_post_mode, enable_clip_relu_post_mode, eltwise_op, eltwise_antq_cfg, enable_c0_pad);
    }
}

// void float
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ void *dst, __cc__ float *src, uint16_t n_size,
            uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode,
            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode,
            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
            bool enable_c0_pad, bool enable_broadcast, bool NZ2DN_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_ctrl, clip_relu_pre_mode,
            unit_flag, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
            relu_post_mode, enable_clip_relu_post_mode, false, eltwise_op, enable_antiquant, false, enable_c0_pad, false,
            enable_broadcast, NZ2DN_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(__cbuf__ void *dst, __cc__ float *src, uint16_t n_size,
            uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode,
            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode,
            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
            bool enable_c0_pad, bool enable_broadcast, bool NZ2DN_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_ctrl, clip_relu_pre_mode,
        unit_flag, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post_mode, relu_post_mode, enable_clip_relu_post_mode,
        eltwise_op, enable_antiquant, enable_c0_pad, enable_broadcast, NZ2DN_en);
    asc_sync_post_process();
}

// void int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(__cbuf__ void *dst, __cc__ int32_t *src, uint16_t n_size,
            uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode,
            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode,
            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
            bool enable_c0_pad, bool enable_broadcast, bool NZ2DN_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_ctrl, clip_relu_pre_mode,
            unit_flag, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post_mode),
            relu_post_mode, enable_clip_relu_post_mode, false, eltwise_op, enable_antiquant, false, enable_c0_pad, false,
            enable_broadcast, NZ2DN_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(__cbuf__ void *dst, __cc__ int32_t *src, uint16_t n_size,
            uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode,
            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode,
            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
            bool enable_c0_pad, bool enable_broadcast, bool NZ2DN_en)
{
    asc_copy_l0c2l1_impl(dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_ctrl, clip_relu_pre_mode,
        unit_flag, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post_mode, relu_post_mode, enable_clip_relu_post_mode,
        eltwise_op, enable_antiquant, enable_c0_pad, enable_broadcast, NZ2DN_en);
    asc_sync_post_process();
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
