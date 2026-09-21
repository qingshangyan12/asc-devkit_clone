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
#warning \
    "impl/c_api/reg_base_impl/npu_arch_3510/cube_datamove_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

constexpr uint8_t RELU_POST_DEFAULT = 0;

constexpr bool CLIP_RELU_POST_DEFAULT = false;

constexpr uint8_t ELTWISE_OP_DEFAULT = 0;

constexpr uint64_t QUANT_POST_DEFAULT = 0;

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
        asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}
/* asc_copy_l0c2gm interface path 9. */

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm interface path 10. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm interface path 11. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2gm interface path 12. */
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2gm interface path 13. */
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2gm interface path 14. */
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm interface path 15. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2gm interface path 16. */
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2gm interface path 17. */
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm interface path 18. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm interface path 19. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}
/* asc_copy_l0c2gm interface path 20. */

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2ub interface path 1. */
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 2. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 3. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 4. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 5. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 6. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 7. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 8. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 9. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 10. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2ub interface path 11. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
            enable_clip_relu_pre, unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode,
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post,
            false, eltwise_op, eltwise_antq_en, false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
            enable_clip_relu_pre, unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode,
            /* asc_copy_l0c2ub interface path 12. */
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post,
            false, eltwise_op, eltwise_antq_en, false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}
/* asc_copy_l0c2l1 interface path 6. */

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 7. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 8. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 9. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 10. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 11. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 12. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            /* asc_copy_l0c2l1 interface path 13. */
            false, c0_pad_en, false, false, false);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 14. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        /* asc_copy_l0c2l1 interface path 15. */
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        /* asc_copy_l0c2l1 interface path 16. */
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}
/* asc_copy_l0c2l1 interface path 17. */

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
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
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

/* asc_copy_l0c2gm interface path 21. */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 22. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 23. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 24. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 25. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 26. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 27. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 28. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 29. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 30. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 31. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 32. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        /* asc_copy_l0c2gm interface path 33. */
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
    /* asc_copy_l0c2l1 interface path 18. */
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            /* asc_copy_l0c2l1 interface path 19. */
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 20. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            /* asc_copy_l0c2l1 interface path 21. */
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 22. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 23. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            /* asc_copy_l0c2l1 interface path 24. */
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 25. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 26. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 27. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2l1 interface path 28. */
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, 0,
            static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
            static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)), static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            false, false, false, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, 0,
            static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
            static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)), static_cast<uint8_t>(relu_pre_mode),
            /* asc_copy_l0c2l1 interface path 29. */
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            false, false, false, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
    /* asc_copy_l0c2ub interface path 13. */
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 14. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 15. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 16. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 17. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 18. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 19. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 20. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 21. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 22. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 23. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 24. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        /* asc_copy_l0c2ub interface path 25. */
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
